import numpy as np
import matplotlib.pyplot as plt
from matplotlib.mlab import griddata
import csv

def scatter2D(filename):
    
    x = []
    y = []
    
    with open(filename) as csvfile:
        jawnreader = csv.reader(csvfile)
        for row in jawnreader:
            x.append(row[0])
            y.append(row[1])
            
    plt.figure()
    plt.plot(x, y, ':')
    plt.show()
    
def contour2D(filename):
    
    x = []
    y = []
    z = []
    
    with open(filename) as csvfile:
        jawnreader = csv.reader(csvfile)
        for row in jawnreader:
            x.append(float(row[0]))
            y.append(float(row[1]))
            z.append(float(row[2]))
    
    xi = np.linspace(min(x), max(x), 100)
    yi = np.linspace(min(y), max(y), 100)
    Z = griddata(x, y, z, xi, yi, 'linear')
    X, Y = np.meshgrid(xi, yi)
            
    plt.figure()
    plt.contourf(X, Y, Z)
    plt.show()