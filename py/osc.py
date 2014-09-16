"""Module dealing with the renaming and proper renaming of the files saved
by the oscilloscope."""
import glob, time, os

LIMIT = 2  # discriminating pulsewidth (fwhm) in microseconds


def parse_time(line):
    """Parse time from lecroy data and return time to use for filename."""
    # remove first and last columns
    line = line.split(' ', 3)[1:-1]
    line = " ".join(line)
    # parse time from file
    input_format = "%d-%b-%Y %H:%M:%S"
    print(line)
    time_str = time.strptime(line, input_format)
    # parse output time
    output_format = "%Y%m%d%H%M%S"
    # reformat time_str
    time_str = time.strftime(output_format, time_str)
    return time_str

def find_kind(data, cutoff=LIMIT):
    """Determine kind of measurement (extraction/injection)."""
    max_val = max(data, key=lambda x: x[1])[1]
    min_val = max(data, key=lambda x: x[1])[1]
    middle = (max_val-min_val)*0.5
    delta_t = data[1][0] - data[0][0]
    pulse_len = sum(1 if entry[1] > middle else 0 for entry in data)
    pulse_len *= delta_t
    return "ext" if pulse_len>cutoff*1e-6 else "inj"

def rename(old_name):
    """Rename saved file to the correct format."""
    with open(old_name) as fh:
        # skip first 3 lines
        for i in range(3):
            next(fh)
        # next line has time
        time_str = parse_time(next(fh))
        # next line is worthless
        next(fh)
        data = [tuple(map(float, line.split())) for line in fh]
        # find max amplitude
        kind = find_kind(data)
    channel = old_name[:2]
    new_name = "{tm}_{ch}_{tp}.{ext}".format(tm=time_str, ch=channel,
                                             tp=kind, ext="txt")
    os.rename(old_name, new_name)
    

file_list = glob.glob('*.txt')
for measurement in file_list:
    rename(measurement)
    
