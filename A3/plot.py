import matplotlib.pyplot as plt
import numpy as np
data = np.loadtxt("data.txt",delimiter=',')
x = data[:,0]
y = data[:,1]
plt.figure()
plt.plot(x,y)
plt.savefig("./plot.png")