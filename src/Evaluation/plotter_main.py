#!/usr/bin/env python

# Plots results from an evaluation in different modes

import ast
import sys
import matplotlib.pyplot as plt
from plotter_utilities import *

# Reading arguments from command line
from plotter_parameters import *

# Read the result tile and parse it in appropiate format
file = open(FILE, 'r')
shape, parsedFile = parseFile(file.read())

if (mode == 'speedup_cores'):
    plotSpeedupAndCores(parsedFile, NUMBER_OF_POINTS, ALGORITHMS[0], shape)