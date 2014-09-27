# vim: ai:si:number:ts=4:et:sw=4:st=4
"""
A performing the merging of files during the experiment.
"""

import os
import logging
import time
import glob
import cPickle as pickle

############
# Settings #
############
# data
DATA_DIR = "/hera/sids/oscillation_test/autocopy_tests"
RSA51 = os.path.join(DATA_DIR, "rsa51")
RSA52 = os.path.join(DATA_DIR, "rsa52")
RSA30 = os.path.join(DATA_DIR, "rsa30")
OSC_DIR = os.path.join(DATA_DIR, "osc")
OSC_CHANS = ("C1", "C2", "C3", "C4")
REF_CHAN = os.path.join(OSC_DIR, "C2")
# path to time2root
T2R = "time2root"
OUTPUT_DIR = os.path.join(DATA_DIR, "root")
LOGFILE = os.path.join(DATA_DIR, "merging.log")
PERIOD = 30  # seconds


class TimeExtractor(object):

    """
    A collection of methods used to extract times from various instruments.
    """
    osc_time = "%Y.%m.%d.%H.%M.%S"
    rsa50_time = "%Y.%m.%d.%H.%M.%S.%f.TIQ"
    rsa30_time = "%Y%m%d-%H%M%S"

    @classmethod
    def rsa30(cls, name):
        """Extract time from RSA30 IQT files."""
        name = name.split('/')[-1]
        name = name.split('-')[:-1]
        name = "-".join(name)
        return time.strptime(name, cls.rsa30_time)

    @classmethod
    def rsa50(cls, name):
        """Extract time from RSA50 TIQ files."""
        name = name.split('/')[-1]
        name = name.split('-')[1]
        return time.strptime(name, cls.rsa50_time)

    @classmethod
    def osc(cls, name):
        """Extract time from LeCroy CSV files."""
        name = name.split('/')[-1]
        name = name.split('_')[1]
        return time.strptime(name, cls.osc_time)


def get_injections(processed):
    """
    Find and return new injections. Injections are classified according to
    their starting point in time.

    Arg:
        processed (set): a collection of previously processed injections.

    Returns:
        A list of tuples, where each tuple contains the starting time of the
        injection and the starting time of the next injection.
    """
    previous = os.getcwd()
    os.chdir(REF_CHAN)

    file_list = glob.glob("C2*inj.csv")
    # possibly the S/A files have not been transferred yet, remove last
    file_list = file_list[:-1]
    file_list = [TimeExtractor.osc(f) for f in file_list]
    file_list = [f for f in file_list if f not in processed]
    file_list.sort(key=lambda x: time.mktime(x))

    # creates tuples of 2 subsequent injection times
    intervals = zip(file_list[:-1], file_list[1:])

    os.chdir(previous)
    return intervals


def create_range_predicate(start, stop, tolerance=0):
    """
    Create a filtering function that checks if time was within the expected
    time range.
    """
    start = time.mktime(start)
    stop = time.mktime(stop)
    if stop < start:
        raise ValueError("Start time is after stop time")

    def predicate(data_time):
        data_time = time.mktime(data_time)
        return start - tolerance < data_time < stop + tolerance

    return predicate


def get_inj_files(start):
    """Retrieve oscilloscope injection files"""
    data = []
    for channel in OSC_CHANS:
        glob_str = "{osc}/{ch}/{ch}_{tm}_*.csv".format(
            osc=OSC_DIR, ch=channel,
            tm=time.strftime(TimeExtractor.osc_time, start))
        found_files = glob.glob(glob_str)
        data.extend(found_files)
    return data


def get_ext_files(predicate):
    """Retrieve oscilloscope extraction files"""
    data = []
    for channel in OSC_CHANS:
        glob_str = "{osc}/{ch}/{ch}_*.csv".format(osc=OSC_DIR, ch=channel)
        found_files = [f for f in glob.glob(glob_str)
                       if predicate(TimeExtractor.osc(f))]
        data.extend(found_files)
    return data


def get_osc_files(start, predicate):
    """Retrieve oscilloscope files."""
    data = get_inj_files(start)
    data += get_ext_files(predicate)
    return data


def get_rsa50_files(predicate):
    data = []
    for rsa in (RSA51, RSA52):
        glob_str = "{rsa}/*.TIQ".format(rsa=rsa)
        found_files = [f for f in glob.glob(glob_str)
                       if predicate(TimeExtractor.rsa50(f))]
        data += found_files
    return data


def get_rsa30_files(predicate):
    glob_str = "{rsa}/*.iqt".format(rsa=RSA30)
    found_files = [f for f in glob.glob(glob_str)
                   if predicate(TimeExtractor.rsa30(f))]
    return found_files


def merge(start, data):
    outfile = "{out}/{name}".format(out=OUTPUT_DIR, name=time.mktime(start))
    with open(outfile, "w") as file_:
        file_.write("\n".join(data))
        file_.write("\n")


def save_processed(filename, processed):
    """
    Pickle a data collection (set) and append it to a file.

    Args:
        filename (str): the name of the file to which to write.
        processed (set): the collection which will be appended to file.
    """
    with open(filename, "ab") as file_:
        pickle.dump(processed, file_, pickle.HIGHEST_PROTOCOL)


def get_processed(filename):
    """
    Unpickle all sets contained within a file.

    Arg:
        filename (str): the name of the file from which to read.

    Returns:
        A union of all sets contained within the file.
    """
    processed = set()
    try:
        with open(filename, "rb") as processed_file:
            while True:
                processed.update(pickle.load(processed_file))
    except EOFError:
        pass
    except IOError:
        # if file doesn't exist will create a file with an empty set.
        save_processed(filename, processed)
    return processed


def loop(processed):
    """
    The program loop, made up of the following steps:

        1. Find all not processed injection files from oscilloscope
           (default: C2)
        2. Find S/A and extraction files belonging to each injection.
        3. Create a root file if all raw files have been found or log failure.

    Args:
        processed (set): a set of already processed injections as returned
                         by :py:func:`get_processed`.
    """
    injections = get_injections(processed)
    for start, stop in injections:
        predicate = create_range_predicate(start, stop)
        data2merge = []
        data2merge += get_osc_files(start, predicate)
        data2merge += get_rsa50_files(predicate)
        data2merge += get_rsa30_files(predicate)
        if len(data2merge) != 11:
            logging.error("Injection@%s could not be merged",
                          time.strftime("%m.%d.%H.%M.%S", start))
        else:
            merge(start, data2merge)
            processed.add(start)
            logging.info("Successfully merged injection@%s",
                         time.strftime("%m.%d.%H.%M.%S", start))
            save_processed("processed.list", set([start]))
    logging.info("Finished loop")


def main():
    """
    The main function of the application. It consists of an application
    loop and sleeping till the end of time.
    """
    os.chdir(DATA_DIR)
    processed = get_processed("processed.list")
    while True:
        time.sleep(PERIOD)
        loop(processed)


def config_logging():
    """Set the parameters for the logfile."""
    logging.basicConfig(
        format='%(asctime)s:%(name)s:%(levelname)s:%(message)s',
        datefmt='%Y-%m-%d %H:%M:%S', filename=LOGFILE,
        level=logging.INFO)
    logging.info("====== Start operation ======")


if __name__ == "__main__":
    config_logging()
    main()

