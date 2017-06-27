import matplotlib.pyplot as plt
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