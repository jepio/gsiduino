"""Perform a periodic backup of data. Tested with Python 3.4"""
import os
import time
from subprocess import Popen, PIPE
import pickle


PERIOD = 2  # seconds
PATH_TO_DATA = os.path.join(r"D:\\", "temp", "data_transfer", "folder")
PATH_TO_REMOTE = os.path.join(r"D:\\", "temp", "data_transfer", "folder2")
LOGFILE = "transfer.log"
LOGFILE = os.path.join(os.getcwd(), LOGFILE)
FILE_LIST = "file.list"
FILE_LIST = os.path.join(os.getcwd(), "file.list")
PROCESSED_FILES = None


def add_log_entry(filename, timestamp=None, operation="TRANS"):
    """\
    Adds an entry to the logfile specifiying what file was copied and when."""
    if not timestamp:
        timestamp = time.time()
    with open(LOGFILE, 'a', encoding="utf-8", newline='\n') as file_h:
        file_h.write("{time_:.0f} {op} {name}\n".format(
            time_=timestamp, name=filename, op=operation))


def check_files():
    """Return a set of all files (not directories) in the cwd."""
    return set(filter(os.path.isfile, os.listdir()))


def copy_file(filename):
    """Copy file from local DATA folder to REMOTE."""
    src = os.path.join(PATH_TO_DATA, filename)
    dest = os.path.join(PATH_TO_REMOTE, filename)
    return Popen(["copy", src, dest], shell=True, stdout=PIPE, stderr=PIPE)


class FileListBuilder:
    """Save and load the set of processed files from a backup file"""

    def __init__(self, name):
        self.filename = name

    def save_list(self, set_obj):
        with open(self.filename, "wb") as fh:
            pickle.dump(set_obj, fh)

    def read_list(self):
        try:
            with open(self.filename, "rb") as fh:
                return pickle.load(fh)
        except (FileNotFoundError, EOFError):
            # either the file is missing or it is corrupt, in any case its
            # better to start from scratch
            return set()


def main():
    global PROCESSED_FILES
    print(os.getcwd())
    os.chdir(PATH_TO_DATA)
    print(os.getcwd())
    try:
        os.mkdir(PATH_TO_REMOTE)
    except FileExistsError:
        # this is perfectly fine, means the directory exists already
        pass

    # serialized list of copied files
    flb = FileListBuilder(FILE_LIST)
    # read saved one if possible, if not creates empty set
    PROCESSED_FILES = flb.read_list()

    start = time.time()
    while True:
        # load file list from current directory and subtract the ones that have
        # already been processed
        files = check_files() - PROCESSED_FILES
        
        if files:
            processes = []
            # for each new file start copying and write to log
            for file_ in files:
                processes.append(copy_file(file_))
                add_log_entry(file_)
            # for each copy process check the output, and wait for the longest
            # running one
            for proc in processes:
                outcome = proc.poll()
                if outcome is not None:
                    continue
                else:
                    outcome = proc.wait()
                if outcome != 0:
                    raise RuntimeError("Outcome of copy is non-zero")
            # update list of PROCESSED_FILES
            PROCESSED_FILES |= files
            # backup list
            flb.save_list(PROCESSED_FILES)

        stop = time.time()
        print("PING: Elapsed time {}".format(stop-start))
        start = time.time()
        time.sleep(PERIOD)

if __name__ == "__main__":
    add_log_entry("", operation="=== STARTING LOG ===")
    main()
