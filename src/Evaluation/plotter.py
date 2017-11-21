#!/usr/bin/env python

# Plots results from the evaluations in different modes.
# Provide the script with the following arguments:
# -f relative path to read the results from (the file should be in the format generated from evaluator.py)
# -m mode of the plot: speedup_cores ; speedup_numPoints
# -a list of algorithms to plot in the format "['algortihm1','algorithm2'...]"
# -n number of points

import ast
import sys
import matplotlib.pyplot as plt

# Parsing the file which should be in the following format:
# shape
# numPoints,algorithm,cores,avgTiming
# 10,someAlgorithm,2,3.444
# ...
# eof
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
        if algorithm in parsedFile:
            if cores in parsedFile[algorithm]:
                parsedFile[algorithm][cores].update({numPoints: avgTiming})
            else:
                parsedFile[algorithm].update({cores: {}})
        else:
            parsedFile.update({algorithm: {}})
    return shape, parsedFile

# TODO: extent to more than one algorithm
# Plots the speedup for given number of points for given set of algorithms.
def plotSpeedupAndCores(data, numPoints, algorithms, shape):
    sequentialTime = data[algorithms][1][numPoints]
    speedUps = []
    cores = []
    for i in range(2, len(data[algorithms]) + 1):
        speedUp = sequentialTime/data[algorithms][i][numPoints]
        speedUps.append(speedUp)
        cores.append(i)
    print cores
    print speedUps
    plot(cores, speedUps, shape + '_' + algorithms + '_SpeedUp_Cores.png', 'Cores', 'Speed up', 25, 25)

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

# Fetch command line arguments
FILEPATH = sys.argv[sys.argv.index('-f') + 1]
MODE = sys.argv[sys.argv.index('-m') + 1]
ALGORITHMS = ast.literal_eval(sys.argv[sys.argv.index('-a') + 1])
NUMBER_POINTS = int(sys.argv[sys.argv.index('-n') + 1])

# Read the result tile and parse it in appropiate format
file = open(FILEPATH, 'r')
shape, parsedFile = parseFile(file.read())

if (mode == 'speedup_cores'):
    plotSpeedupAndCores(parsedFile, NUMBER_POINTS, ALGORITHMS[0], shape)