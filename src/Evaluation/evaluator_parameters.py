#!/usr/bin/env python
'''
    Takes care of handling command line arguments used for evaluator_main.py.
'''

# The flags have the following meanings:
# -minp minimum number of data points to be generated
# -maxp maximum number of data points to be generated
# -r maximum absolute range of coordinate values
# -s shape of hull
# -minpr minimum parallelism index (minimum number of threads used for concurrent running)
# -maxpr maximum parallelism index (maximum number of threads used for concurrent running)
# -otf set to true if on-the-fly generated points are used as input otherwise to false and -f should be provided
# -f relative path to a file that would be used as an input file instead of on-the-fly generated points
# -alg algorithms for which the evaluation is run for
# -t how many times each test configuration should be run
# -st by how many points each test should be increased with

import argparse

parser = argparse.ArgumentParser(description='Provide parameters for the evaluation.')

parser.add_argument('-minp', default='100', dest='minp', type=int, nargs='?',
                    help='Minimum number of data points to be generated')
parser.add_argument('-maxp', default='1000', dest='maxp', type=int, nargs='?',
                    help='Maximum number of data points to be generated')
parser.add_argument('-r', default='100', dest='range', type=int, nargs='?',
                    help='Maximum absolute range of coordinate values')
parser.add_argument('-s', default='random', dest='shape', nargs='?',
                    choices=['random', 'circle', 'square'], help='Shape of the data points input')
parser.add_argument('-minpr', default='1', dest='minpr', type=int, nargs='?',
                    help='Minimum parallelism index')
parser.add_argument('-maxpr', default='1', dest='maxpr', type=int, nargs='?',
                    help='Maximum parallelism index')
parser.add_argument('-otf', default='true', dest='otf', type=bool, nargs='?',
                    help='Should it generate data points on-the-fly')
parser.add_argument('-f', default='none', dest='file', nargs='?',
                    help='Relative path to e file if on-the-fly generation of data points is not used.')
parser.add_argument('-alg', default=['graham', 'jarvis', 'chan1'], dest='alg', nargs='+',
                    help='Algorithms for which the evaluation is run for')
parser.add_argument('-t', default='10', dest='runtimes', type=int, nargs='?',
                    help='How many times each test configuration should be run')
parser.add_argument('-st', default='100', dest='step', type=int, nargs='?',
                    help='By how many points each test should be increased with')

args = parser.parse_args()

# From command line
MINIMUM_NUMBER_POINTS = args.minp
MAXIMUM_NUMBER_POINTS = args.maxp
RANGE = args.range
SHAPE = args.shape
MIN_PARALLELISM = args.minpr
MAX_PARALLELISM = args.maxpr
ON_THE_FLY = args.otf
INPUT_FILE = args.file
ALGORITHMS = args.alg
RUNTIMES = args.runtimes
STEP = args.step

# Relative paths to script
EXECUTABLES = ['PointsGenerator/', '']
POINT_GENERATOR = '../PointsGenerator/PointsGenerator'
ALGORITHM_RUNNER = '../AlgorithmsRunner'
