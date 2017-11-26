#!/usr/bin/env python
'''
    Takes care of handling command line arguments used for evaluator_main.py.
'''

# The flags have the following meanings:
# -f relative path to read the results from (the file should be in the format generated from evaluator_main.py)
# -n number of points to plot for if plot is only for certain data set
# -m mode of the plot: speedup_cores ; speedup_points
# -alg list of algorithms to plot

import argparse

parser = argparse.ArgumentParser(description='Provide parameters for the evaluation.')

parser.add_argument('-f', default='100', dest='file', nargs='?',
                    help='Relative path to read results from generated via evaluator_main.py')
parser.add_argument('-n', default='1000', dest='points', type=int, nargs='?',
                    help='Number of points to plot for if plot is only for certain data set')
parser.add_argument('-m', default='speedup_cores', dest='mode', nargs='?',
                    choices=['speedup_cores', 'speedup_points'], help='Mode of the plot')
parser.add_argument('-alg', default=['graham', 'jarvis', 'chan1'], dest='alg', nargs='+',
                    help='List of algorithms to plot')
parser.add_argument('-minprl', default='1', dest='minprl', type=int, nargs='?',
                    help='Minimum number of cores used in the evaluation')

args = parser.parse_args()

# From command line
FILE = args.file
NUMBER_OF_POINTS = args.points
MODE = args.mode
ALGORITHMS = args.alg
MIN_PARALLELISM = args.minprl
