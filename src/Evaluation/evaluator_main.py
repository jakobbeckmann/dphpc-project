#!/usr/bin/env python

import atexit
import numpy as np
from evaluator_utilities import *

# Reading arguments from command line
from evaluator_parameters import *

# Building executables
buildExecutables(EXECUTABLES)

# Making sure executables are always cleaned at the end
atexit.register(cleanExecutables, EXECUTABLES)

# Set up result container
result = {}
for numOfPoints in range(MINIMUM_NUMBER_POINTS, MAXIMUM_NUMBER_POINTS, STEP):
    result.update({numOfPoints : {}})

# For each number of points generate data set specified by shape,
# run each algorithm n number of times as specified by RUNTIMES
# on minimum number of parallelism to maximum number of parallelism
# saving the running time for each run

print 'Starting evaluations...\n'

for numOfPoints in range(MINIMUM_NUMBER_POINTS, MAXIMUM_NUMBER_POINTS, STEP):
    print 'Generating data set for ' + str(numOfPoints) + ' points\n'

    # Check if data set is generated on the fly or read from provided file
    dataFile = ''
    if not ON_THE_FLY:
        dataFile = INPUT_FILE
    else:
        # Generate points and save data set file uniquely for given datetime and number of points
        dataFile = getUniqueName('data', numOfPoints, SHAPE)
        subprocess.call(' '.join([POINT_GENERATOR, str(numOfPoints), str(RANGE), SHAPE, '>', dataFile]), shell=True)

    # Calculate expected output using the safest algorithm (Graham Scan)
    correctOutput = getCorrectOutput(numOfPoints, dataFile, ALGORITHM_RUNNER)

    # Run each algorithm with the given data set
    for alg in ALGORITHMS:
        result[numOfPoints].update({alg: {}})

        # Run each algorithm for the specified parallelism range
        for prl in range(MIN_PARALLELISM, MAX_PARALLELISM + 1):
            result[numOfPoints][alg].update({prl : []})

            print 'Evaluation for ' + alg + ' on ' + str(prl) + ' cores' + ' with ' + str(numOfPoints) + ':\n'

            # Run each algorithm n number of times (n = RUNTIMES)
            for rt in range(0, RUNTIMES):
                # Save output from the run
                actualOutput = subprocess.check_output(' '.join(['cat', dataFile, '|', ALGORITHM_RUNNER, str(prl), str(numOfPoints),
                                                           alg]), shell=True)

                # Check if output of algorithm was correct
                if isOutputCorrect(actualOutput, correctOutput):
                    # Save the running time
                    time = float(actualOutput.split('\n')[0])
                    result[numOfPoints][alg][prl].append(time)
                else:
                    print 'Wrong results for ' + alg + ' running on ' + str(prl) + '. Setting time to -10000.\n'
                    result[numOfPoints][alg][prl].append(-10000)
    print 'Finished evaluation for ' + alg + '\n'

# Writing results to uniquely named file
print 'Writing results to file...\n'
resultFile = getUniqueName('result', numOfPoints, SHAPE)
file = open(resultFile, 'w')
# Mark shape of hull and meaning of columns
file.write(SHAPE + '\nnumPoints,algorithm,numCores,avgTimingOf' + str(RUNTIMES) + '\n')
# Extract data from result container
for points, algorithms in result.iteritems():
    for algorithm, cores in algorithms.iteritems():
        for core, timings in cores.iteritems():
            # Aggregate the running time
            avg = np.mean(timings)
            file.write(str(points) + ',' + algorithm + ',' + str(core) + ',' +str(avg) + '\n')
file.close()

