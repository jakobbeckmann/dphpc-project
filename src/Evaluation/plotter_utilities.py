import matplotlib.pyplot as plt

# Parsing the file which should be in the following format:
'''
shape   <- name of the shape: circle, random, etc.
numPoints,algorithm,cores,avgTiming <- labels
10,someAlgorithm,2,3.444 <- values
... <- more values
eof <- empty for eof
'''
def parseFile(file):
    allParts = file.split('\n')
    shape = allParts[0]
    parsedFile = {}
    for i in range(2, len(allParts) - 1):
        lineParts = allParts[i].split(',')
        numPoints = int(lineParts[0])
        algorithm = lineParts[1]
        cores = int(lineParts[2])
        avgTiming = float(lineParts[3])
        if not algorithm in parsedFile:
            parsedFile.update({algorithm: {}})
        if not cores in parsedFile[algorithm]:
            parsedFile[algorithm].update({cores: {}})
        parsedFile[algorithm][cores].update({numPoints: avgTiming})
    return shape, parsedFile

# Plots the speedup for given number of points for given algorithm
def plotSpeedupAndCores(data, numPoints, algorithm, shape, minParallelism):
    sequentialTime = data[algorithm][minParallelism][numPoints]
    speedUps = []
    cores = []
    for i in range(2, len(data[algorithm]) + 1):
        speedUp = sequentialTime/data[algorithm][i][numPoints]
        speedUps.append(speedUp)
        cores.append(i)
    plot(cores, speedUps, shape + '_' + algorithm + '_SpeedUp_Cores.png', 'Cores', 'Speed up', 25, 25)

# Constructs simple plot with given x and y values
def plot(n, time, name, xlabel, ylabel, xlim, ylim):
    fig, ax = plt.subplots(figsize=(13, 9))
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)
    ax.xaxis.set_ticks_position('bottom')
    ax.yaxis.set_ticks_position('left')
    ax.set_xlim([0, xlim])
    ax.set_ylim([0, ylim])

    ax.plot(n, time, 'ro', markeredgecolor='red')
    ax.plot(n, time, 'r--', linewidth = 2)
    if name == "speedup.png":
        ax.plot([0,24], [0,24], '--')
    plt.savefig(name)