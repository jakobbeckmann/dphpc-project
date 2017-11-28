"""
This python module creates benchmark plots based on the timing.txt input file in the Output folder
of the project, which is a csv file (first column n cores, second column times).
"""

import os
from os.path import join as join_paths
import numpy as np
import matplotlib.pyplot as plt


class Benchmark:
    def __init__(self, timing_file='timing.txt'):
        self.timing_file = join_paths(os.path.realpath(__file__), '../../Output', timing_file)
        self.timing_data = {}
        self.output_dir = join_paths(os.path.realpath(__file__), '../../FiguresAndMovies')

    def load_timing_data(self):
        raw_data = np.loadtxt(self.timing_file, delimiter=',')
        for item in raw_data:
            self.timing_data[int(item[0])] = item[1]

    def plot_speedup_vs_cores(self, save=False, show=False):
        fig, ax = plt.subplots()
        fig.set_size_inches(13, 9)
        ax.set_xlabel('Number of cores')
        ax.set_ylabel('Speedup')

        time_1core = self.timing_data[1]
        n_cores = self.timing_data.keys()
        speedup = [time_1core / self.timing_data[n] for n in n_cores]

        ax.plot(n_cores, speedup, linewidth=2, color='green')
        ax.plot(n_cores, speedup, 'go', linewidth=2, color='green')

        if save:
            plt.savefig(join_paths(self.output_dir, 'speedup_vs_cores.png'))

        if show:
            plt.show()

    def plot_time_vs_cores(self, save=False, show=False):
        fig, ax = plt.subplots()
        fig.set_size_inches(13, 9)
        ax.set_xlabel('Number of cores')
        ax.set_ylabel('Time')

        n_cores = self.timing_data.keys()
        speedup = [self.timing_data[n] for n in n_cores]

        ax.plot(n_cores, speedup, linewidth=2, color='blue')
        ax.plot(n_cores, speedup, 'o', linewidth=2, color='blue')

        if save:
            plt.savefig(join_paths(self.output_dir, 'time_vs_cores.png'))

        if show:
            plt.show()



