"""
This python module creates benchmark plots based on the timing.txt input file in the Output folder
of the project, which is a csv file (first column n cores, second column times).
"""

import os
from os.path import join as join_paths
import numpy as np
import matplotlib.pyplot as plt

from python.libs.paths import project_path


class Benchmark:
    def __init__(self, run_config, all_params, all_data_dict):
        self.run_config = run_config
        self.all_params = all_params
        self.all_data_dict = all_data_dict

    def plot_runtimes_comparison(self, save=False, show=False, file_name=None):
        """Plots the absolute runtime vs input size for all algorithms in the run."""
        fig, ax = plt.subplots()
        fig.set_size_inches(13, 9)
        ax.set_xlabel('Input size [number of points]')
        ax.set_ylabel('Run time [s]')

        algo_runtimes = {algorithm: {'input_sizes': [], 'run_times': []}
                         for algorithm in self.run_config['run_params']['algorithms']}

        for _, data in self.all_data_dict.iteritems():
            algo_runtimes[data['algorithm']]['input_sizes'].append(data['n_points'])
            algo_runtimes[data['algorithm']]['run_times'].append(data['mean_run_tim'])

        for algorithm, algo_data in algo_runtimes.iteritems():
            sort_indices = np.argsort(np.array(algo_data['input_sizes']))

            input_sizes = [algo_data['input_sizes'][i] for i in sort_indices]
            run_times = [algo_data['run_times'][i] for i in sort_indices]

            print('--------\n')
            print('algo: ' + algorithm)
            print('input sizes:', input_sizes)
            print('run times:  ', run_times)

            ax.plot(input_sizes, run_times, linewidth=3, label=algorithm, alpha=0.5)
            # ax.plot(input_sizes, run_times, 'o', linewidth=3, alpha=0.5, color='black', markeredgecolor='none')

        plt.legend()

        self.evaluate_save_show(save, show, file_name)

    # TODO: This is an old version. Implement for new framework.
    def plot_speedup_vs_cores(self, save=False, show=False, file_name=None):
        raise NotImplementedError
        fig, ax = plt.subplots()
        fig.set_size_inches(13, 9)
        ax.set_xlabel('Number of cores')
        ax.set_ylabel('Speedup')

        time_1core = self.timing_data[1]
        n_cores = self.timing_data.keys()
        speedup = [time_1core / self.timing_data[n] for n in n_cores]

        ax.plot(n_cores, speedup, linewidth=2, color='green')
        ax.plot(n_cores, speedup, 'go', linewidth=2, color='green')

        self.evaluate_save_show(save, show, file_name)

    # TODO: This is an old version. Implement for new framework.
    def plot_time_vs_cores(self, save=False, show=False, file_name=None):
        raise NotImplementedError
        fig, ax = plt.subplots()
        fig.set_size_inches(13, 9)
        ax.set_xlabel('Number of cores')
        ax.set_ylabel('Time')

        n_cores = self.timing_data.keys()
        speedup = [self.timing_data[n] for n in n_cores]

        ax.plot(n_cores, speedup, linewidth=2, color='blue')
        ax.plot(n_cores, speedup, 'o', linewidth=2, color='blue')

        self.evaluate_save_show(save, show, file_name)

    def evaluate_save_show(self, save, show, file_name):
        if save:
            assert file_name is not None
            # create post processing folder
            post_proc_dir = join_paths(project_path, 'Output', self.run_config['run_name'], 'post_processing')
            if not os.path.isdir(post_proc_dir):
                os.mkdir(post_proc_dir)

            plt.savefig(join_paths(post_proc_dir, file_name + '.png'))

        if show:
            plt.show()