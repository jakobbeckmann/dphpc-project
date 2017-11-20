#!/usr/bin/env python

# -i initial number of points
# -r range of coordinate values
# -s shape of hull
# -p parallelism index

import sys
import subprocess

# Fetch command line arguments
INITIAL_NUMBER_POINTS = int (sys.argv[sys.argv.index('-i') + 1])
RANGE = int (sys.argv[sys.argv.index('-r') + 1])
SHAPE = sys.argv[sys.argv.index('-s') + 1]
PARALLELISM = int (sys.argv[sys.argv.index('-p') + 1])

# Set up executables
# subprocess.call('make clean')
# subprocess.call('make')
subprocess.call('make -C ../PointsGenerator/', shell=True)
subprocess.call('make -C ../', shell=True)

# Set up results container
MAX_RUNS = 5
MAX_NUMBER_POINTS = 100
REPETITION = 1;
ALGORITHMS = ['version1']

result = {}

for points in range(INITIAL_NUMBER_POINTS, MAX_NUMBER_POINTS, 20):
    result.update({points : {}})

for points in range(INITIAL_NUMBER_POINTS, MAX_NUMBER_POINTS, 20):
    # Generate points
    subprocess.call('../PointsGenerator/PointsGenerator ' + str(points) + ' ' + str(RANGE) + ' ' + str(SHAPE) + ' > input.log', shell=True)

    for i in range(0, len(ALGORITHMS)):
        output = subprocess.check_output(
            'cat input.log | ../AlgorithmsRunner ' + str(PARALLELISM) + ' ' + str(points) + ' ' + ALGORITHMS[i], shell=True)
        print output.split('\n')
        time = int(output.split('\n')[3])
        result[points].update({ALGORITHMS[i]: time})

for points in result:
    for algorithm in result[points]:
        print str(points) + ' ' + algorithm + ' ' + result[points][algorithm]


subprocess.call('make clean -C ../PointsGenerator/', shell=True)
subprocess.call('make clean -C ../', shell=True)