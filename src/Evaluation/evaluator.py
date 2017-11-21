#!/usr/bin/env python

# Provide the script with the following arguments
# -i minimum number of points
# -m maximum number of points
# -r maximum absolute range of coordinate values
# -s shape of hull
# -minp minimum parallelism index
# -maxp maximum parallelism index
# -otf set to true if on-the-fly generated points are used as input otherwise to false and -f should be provided
# -f relative path to a file that would be used as an input file instead of on-the-fly generated points

import sys
import subprocess
import datetime
import numpy as np

def runCorrectAlgorithm(numOfPoints, dataFile):
    output = subprocess.check_output(
        'cat ' + dataFile + ' | ../AlgorithmsRunner 1 ' + str(numOfPoints) + ' version1', shell=True)
    return parseOutput(output)

def checkCorrectnessOfOutput(outputToCheck, correctOutput):
    parsedOutputToCheck = parseOutput(outputToCheck)
    if len(parsedOutputToCheck) == len(correctOutput):
        for point in parsedOutputToCheck:
            if not (point in correctOutput):
                return False
    else:
        return False

def parseOutput(output):
    allParts = output.split('\n')
    points = [];
    for i in range(4, len(allParts)):
        xy = allParts[i].split(' ')
        if (len(xy) > 2):
            points.append((xy[0], xy[1]))
    return points

# Fetch command line arguments
MINIMUM_NUMBER_POINTS = int (sys.argv[sys.argv.index('-i') + 1])
MAXIMUM_NUMBER_POINTS = int (sys.argv[sys.argv.index('-m') + 1])
RANGE = int (sys.argv[sys.argv.index('-r') + 1])
SHAPE = sys.argv[sys.argv.index('-s') + 1]
MIN_PARALLELISM = int (sys.argv[sys.argv.index('-minp') + 1])
MAX_PARALLELISM = int (sys.argv[sys.argv.index('-maxp') + 1])
ON_THE_FLY = sys.argv[sys.argv.index('-otf') + 1]
INPUT_FILE = ''
if not ON_THE_FLY:
    INPUT_FILE = sys.argv[sys.argv.index('-f') + 1]

# Build executables
subprocess.call('make -C ../PointsGenerator/', shell=True)
subprocess.call('make -C ../', shell=True)

# Set up test and result environment
RUNTIMES = 10;
STEP = 20;
ALGORITHMS = ['version1']
result = {}
for numOfPoints in range(MINIMUM_NUMBER_POINTS, MAXIMUM_NUMBER_POINTS, STEP):
    result.update({numOfPoints : {}})

# For each number of points, generate data set of points,
# run on 1 to maximum number of parallelism
#  each algorithm RUNTIMES number of times saving the running time for each run
for numOfPoints in range(MINIMUM_NUMBER_POINTS, MAXIMUM_NUMBER_POINTS, STEP):
    dataFile = ''
    if len(INPUT_FILE) > 0:
        dataFile = INPUT_FILE
    else:
        # Generate points and save data set file uniquely for given datetime and number of points
        now = datetime.datetime.now()
        dataFile = 'data_' + str(numOfPoints) + '_' + str(now.month) + '_' + str(now.day) + '_' + str(now.hour) + '_' + str(now.minute) + '_' \
                   + str(now.second)
    subprocess.call('../PointsGenerator/PointsGenerator ' + str(numOfPoints) + ' ' + str(RANGE) + ' ' + str(SHAPE)
                    + ' > ' + dataFile, shell=True)
    # Calculate expected output using the safest algorithm
    correctOutput = runCorrectAlgorithm(numOfPoints, dataFile)
    # Run each algorithm with the given data set
    for i in range(0, len(ALGORITHMS)):
        result[numOfPoints].update({ALGORITHMS[i]: {}})
        # Run each algorithm for 1 to PARALLELISM number of cores
        for c in range(MIN_PARALLELISM, MAX_PARALLELISM + 1):
            result[numOfPoints][ALGORITHMS[i]].update({c : []})
            # Run each algorithm RUNTIMES number of times
            for j in range(0, RUNTIMES):
                output = subprocess.check_output(
                    'cat ' + dataFile + ' | ../AlgorithmsRunner ' + str(c) + ' ' + str(numOfPoints) + ' '
                    + ALGORITHMS[i], shell=True)
                # Check if output of algorithm was correct
                if checkCorrectnessOfOutput(output, correctOutput):
                    # Save the running time for given number of points and algorithm
                    time = float(output.split('\n')[3])
                    result[numOfPoints][ALGORITHMS[i]][c].append(time)
                else:
                    print 'Wrong results for ' + ALGORITHMS[i] + ' running on '+ str(c) + '. Setting time to -10000.\n'
                    result[numOfPoints][ALGORITHMS[i]][c].append(-10000)

# Writing results to uniquely named file
now = datetime.datetime.now()
resultFile = 'result_' + str(numOfPoints) + '_' + str(now.month) + '_' + str(now.day) + '_' + str(now.hour) + '_' + str(now.minute) + '_' \
             + str(now.second)
file = open(resultFile, 'w')
file.write('numPoints,algorithm,numCores,avgTimingOf' + str(RUNTIMES) + '\n')
for points, algorithms in result.iteritems():
    for algorithm, cores in algorithms.iteritems():
        for core, timings in cores.iteritems():
            avg = np.mean(timings)
            file.write(str(points) + ',' + algorithm + ',' + str(core) + ',' +str(avg) + '\n')
file.close()

subprocess.call('make clean -C ../PointsGenerator/', shell=True)
subprocess.call('make clean -C ../', shell=True)