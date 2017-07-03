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

def contour2D(filename, titlestring, xtickarray, ytickarray):

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
    plt.title(titlestring)
    plt.xticks(xtickarray)
    plt.yticks(ytickarray)
    plt.grid(b = True, which = 'major', color = 'k', linestyle = '-')
    plt.contourf(X, Y, Z, cmap = 'jet')
    plt.show()

if __name__ == "__main__":
    step = 2000
    xtickarray = [0, 70, 140, 210]
    ytickarray = [0, 16, 32, 48]
    contour2D("output/rho/" + str(step) + ".csv", "Rho", xtickarray, ytickarray)
    contour2D("output/u/" + str(step) + ".csv", "U", xtickarray, ytickarray)
    contour2D("output/v/" + str(step) + ".csv", "V", xtickarray, ytickarray)