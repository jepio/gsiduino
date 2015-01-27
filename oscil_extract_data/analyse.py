import sys
import numpy as np
from scipy import stats as sp
import matplotlib.pyplot as plt 

n = np.loadtxt("data.in", usecols=(2,))

bw = float(sys.argv[1]) if len(sys.argv) > 1 else 0.1
kde = sp.gaussian_kde(n)

x = np.linspace(0,70)
y = kde(x)

plt.plot(x,y)
plt.show()
