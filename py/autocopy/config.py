import ConfigParser
import os

__all__ = ["get_config"]

KEYS = ("PERIOD", "THREAD_LIMIT", "HOST", "REMOTE_FOLDER", "PATH_TO_REMOTE",
        "PATH_TO_DATA", "LOGFILE", "FILE_LIST", "PSCP", "GLOBSTR", "rename")

def get_config(filename):
    parser = ConfigParser.SafeConfigParser()
    parser.optionxform = str 
    parser.read(filename)

    dict_ = {var: val for var, val in parser.items("Settings")}
    dict_["PERIOD"] = int(dict_["PERIOD"])
    dict_["THREAD_LIMIT"] = int(dict_["THREAD_LIMIT"])
    dict_["LOGFILE"] = os.path.join(os.getcwd(), dict_["LOGFILE"])
    dict_["FILE_LIST"] = os.path.join(os.getcwd(), dict_["FILE_LIST"])
    dict_["rename"] = bool(dict_["rename"])

    return dict_
