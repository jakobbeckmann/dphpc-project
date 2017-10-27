"""
Animation module for the 2D convex hull Chan algorithm.

Chan's Algorithm divides the set of data points into chunks and runs the Graham Scan
on each subset to get the respective convex hulls. Using the Jarvis March one finally
finds the total convex hull of the data set.

The data input (.csv files) have to be structured in the following way:
    - all_points.dat, sorted by ascending polar angle
        x1, x2
        ...
    - hull_points.dat,  not mandatory, same structure as all_points.dat
    - graham_scan_[idx].dat,  one file for every graham_subscan
        idx_start, idx_middle, idx_end, orientation, new_def_idx
        ...

For every step of the graham_algorithm, we want the start, middle, end point indices of the 2 interesting lines,
as well as the current orientation [-1: clockwise, 1: anticlockwise] for coloring and a possible new
definite member of the convec hull [point idx, otherwise -1].
"""

__author__ = "Matthaeus Heer"
__version__ = "1.1"
__email__ = "maheer@student.ethz.ch"


import numpy as np
import matplotlib.pyplot as plt
import scipy.integrate as integrate
import matplotlib.animation as animation
import constants as const


class PolarSort:
    def __init__(self):


class GrahamAlgorithm:
    """
    Represents the state of a Graham Scan of a subset of the data points.
    """

    def __init__(self):
        self.hull_indices = []  # The indices which are - at a particular time step - thought to be on the hull
        self.first_line_xy = [0, 0]
        self.second_line_xy = [0, 0]
        self.orientation = const.CLOCKWISE
        self.current_color = 'blue'

    def step_forward(self, step_idx):
        """
        Updates the position of the two lines and their colors based on the current orientation.
        Updates the hull_indices index array, meaning that either new point is added or one is being removed
        or no changes.
        """
        pass



class JarvisAlgorithm:
    """
    Represents the current state of Chan's Algorithm.

    Init state is
    """

    def __init__(self):
        self.definitve_indices = []


fig = plt.figure(figsize=(8,8))
ax = fig.add_subplot(111, aspect='equal', autoscale_on=False)
ax.grid()


def init():
    pass

def animate(i):
    pass


#calculate number of frames = rows in all graham scan files plus rows in all jarvis files plus number of points for sorting

n_frames = 1000

ani = animation.FuncAnimation(fig, animate, frames=n_frames,
                              interval=interval, blit=True, init_func=init)