"""Perform a periodic backup of data. Tested with Python 3.4"""
import os
import posixpath
import time
from subprocess import Popen, PIPE
import pickle
import logging
import threading
from collections import deque


PERIOD = 5  # seconds
# Remote host
HOST = ""  # get your own
# Remote folder
REMOTE_FOLDER = "tmp"
# Folder to which data will be saved. Needs to exist.
PATH_TO_REMOTE = posixpath.join("%s:." % HOST, REMOTE_FOLDER)
# Folder with data to be uploaded to remote
PATH_TO_DATA = os.path.join("D:\\", "temp", "data_transfer", "folder")
# The name of the log file to be used by the logging module.
# Default location:
#   cwd
LOGFILE = "autocopy.log"
LOGFILE = os.path.join(os.getcwd(), LOGFILE)
# The list of files that have been copied already - uses pickle to prevent
# having to parse the log file
FILE_LIST = "file.list"
FILE_LIST = os.path.join(os.getcwd(), FILE_LIST)
# Path to the putty scp client
PSCP = "pscp"


def check_access(fname, limit=30):
    """Return True if last file modification was more than limit seconds ago."""
    last_mod = os.stat(os.path.join(PATH_TO_DATA, fname)).st_mtime
    now = time.time()
    last = now - last_mod
    if last > limit:
        return True
    else:
        logging.info("%s excluded due to last access %d s ago.", fname, last)
        return False

def check_local():
    """Return a set of all files (not directories) in the path dir."""
    prev = os.getcwd()
    os.chdir(PATH_TO_DATA)
    file_list = set(filter(os.path.isfile, os.listdir()))
    os.chdir(prev)
    return file_list

def check_remote():
    """Return a set of files in remote directory."""
    proc = Popen(["plink", "-ssh", HOST, "ls", REMOTE_FOLDER], shell=True,
                 stdout=PIPE, stderr=PIPE)
    out, _ = proc.communicate()
    return set(line.strip() for line in out.decode('ascii').splitlines())

def copy_file(fname):
    """Return Popen object that copies file from local DATA folder to REMOTE."""
    src = os.path.join(PATH_TO_DATA, fname)
    dest = posixpath.join(PATH_TO_REMOTE, fname)
    return Popen([PSCP, src, dest], shell=True, stdout=PIPE, stderr=PIPE), fname


class FileListBuilder:
    """Handler for saving and loading the set of processed files."""
    def __init__(self, name):
        self.filename = name

    def save_list(self, set_obj, mode="a"):
        """Save the list to file.
        Mode should be: "a" or "w" to append or overwrite."""
        with open(self.filename, mode + "b") as file_:
            pickle.dump(set_obj, file_)

    def read_list(self):
        """Read all data saved in file"""
        read_set = set()
        try:
            with open(self.filename, "rb") as file_:
                # unpickles until runs into EOFError
                while True:
                    read_set.update(pickle.load(file_))
        except (FileNotFoundError, EOFError):
            pass
        return read_set

    def get_processed(self):
        """Get processed list - chooses whether to use remote or local."""
        remote_list = check_remote()
        local_list = self.read_list()
        if not local_list ^ remote_list:
            logging.info("Remote and local lists are the same")
            processed = local_list
        else:
            logging.info("Remote and local differ, choosing remote")
            self.save_list(remote_list, "w")
            processed = remote_list
        return processed


def handle_process(in_tup, deq):
    """Append the filename to deque if copying it ends with a 0 output code."""
    proc, fname = in_tup
    outcome = proc.poll()
    if outcome is None:
        outcome = proc.wait()
    if outcome == 0:
        logging.info("Transferred file: '%s'", fname)
        deq.append(fname)
    else:
        logging.error("Error in file %s, code %d", fname, outcome)
        logging.error("Error output: %s",
                      proc.stderr.read().decode("ascii").strip())

def transfer_files(files):
    """Transfer files and return list of successfully transf. ones."""
    # for each new file start copying (asynchronously)
    # only works with list compr. not gen. expr.
    processes = [copy_file(file_) for file_ in files]
    # deque for gathering data from threads
    deq = deque()
    # wait for end of transfer
    # threaded version
    threads = [threading.Thread(target=handle_process, args=(proc_tup, deq))
               for proc_tup in processes]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()
    # serial version:
    # for proc in processes:
    #     handle_process(proc, dq)
    transferred = set(deq)
    return transferred

def timing(func):
    """Decorator: prints function execution time."""
    def deco_func(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        stop = time.time()
        print("PING: Elapsed time {:4.2f}s".format(stop-start))
        return result
    return deco_func

@timing
def loop(processed, flb):
    """Main application loop."""
    # get locally available files minus the transferred ones
    files = check_local().difference(processed)
    if files:
        logging.info("Found new files: %s" % files)
        # get rid of files that are too new
        files = set(filter(check_access, files))
        # transfer files
        transferred = transfer_files(files)
        # update processed list
        processed.update(transferred)
        # pickle (append) set of transferred files
        flb.save_list(transferred)
    time.sleep(PERIOD)

def main():
    logging.info("In directory: %s", os.getcwd())
    logging.info("Backing up directory: %s", PATH_TO_DATA)
    logging.info("Remote save location: %s", PATH_TO_REMOTE)
    flb = FileListBuilder(FILE_LIST)
    processed = flb.get_processed()
    while True:
        loop(processed, flb)


if __name__ == "__main__":
    logging.basicConfig(format='%(asctime)s:%(name)s:%(levelname)s:%(message)s',
                        datefmt='%Y-%m-%d %H:%M:%S', filename=LOGFILE,
                        level=logging.INFO)
    logging.info("======Start operation======")
    main()
