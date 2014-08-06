#!/usr/bin/python
import sys
import numpy as np
import matplotlib.pyplot as plt

def plot():
    filename = sys.argv[1]
    data = np.loadtxt(filename, dtype=str)
    data = data[:, 0].astype('int32')
    data = data[data > 900]
    data = data[data < 1100]
    plt.hist(data, bins=50)
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "Usage: plot_trigger.py filename"
        sys.exit(1)
    plot()

