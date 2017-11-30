"""
This python module creates benchmark plots based on the timing.txt input file in the Output folder
of the project, which is a csv file (first column n cores, second column times).
"""

import os
from os.path import join as join_paths
import numpy as np
import matplotlib.pyplot as plt

from python.libs.paths import project_path
import plot_utils


class Benchmark:
    def __init__(self, run_config, all_params, all_data_dict):
        self.run_config = run_config
        self.all_params = all_params
        self.all_data_dict = all_data_dict

    def plot_runtimes_comparison(self, save=False, show=False, file_name=None):
        """
        Plots the absolute runtime vs input size for all algorithms in the run.

        For now, the runtimes are the mean over all iterations for a run configuration.
        """

        if len(self.run_config['run_params']['n_cores']) > 1 or len(self.run_config['run_params']['sub_size']) > 1 \
                or len(self.run_config['run_params']['img_files']) > 1:
            raise NotImplementedError("You can't do that. Only multiple input sizes and multiple algos.")

        fig, ax = self.setup_figure_1ax(size=(13, 9),
                                        x_label='Input size [number of points]',
                                        y_label='Run time [s]')

        # set up empty data container
        algo_runtimes = {algorithm: {'input_sizes': [], 'run_times': []}
                         for algorithm in self.run_config['run_params']['algorithms']}

        for key, data in self.all_data_dict.iteritems():
            algo_runtimes[data['algorithm']]['input_sizes'].append(data['n_points'])
            algo_runtimes[data['algorithm']]['run_times'].append(data['mean_run_tim'])

        last_runtimes = []
        for algorithm, algo_data in algo_runtimes.iteritems():
            sort_indices = np.argsort(np.array(algo_data['input_sizes']))

            input_sizes = [algo_data['input_sizes'][i] for i in sort_indices]
            run_times = [algo_data['run_times'][i] for i in sort_indices]

            last_runtimes.append(run_times[-1])
            ax.plot(input_sizes, run_times, linewidth=3, label=algorithm, alpha=0.5)
            # ax.plot(input_sizes, run_times, 'o', linewidth=3, alpha=0.3, color='black', markeredgecolor='none')

        ax.legend(loc='center left', bbox_to_anchor=(1, 0.5), frameon=False)

        order = np.argsort(last_runtimes)[::-1]
        plot_utils.ordered_legend(ax, order)
        self.evaluate_save_show(save, show, file_name)

    def plot_speedup_vs_cores(self, save=False, show=False, file_name=None):
        """
        Plots the speedup vs number of cores for all algorithms in the run.
        """
        if len(self.run_config['run_params']['n_points']) > 1 or len(self.run_config['run_params']['sub_size']) > 1 \
                or len(self.run_config['run_params']['img_files']) > 1:
            raise NotImplementedError("You can't do that. Only multiple numbers of cores and multiple algos.")

        fig, ax = plot_utils.setup_figure_1ax(size=(13, 9),
                                        x_label='# cores',
                                        y_label='Speedup')

        # set up empty data container holding n cores, run times, speedup of max n cores
        algo_t_core = {algorithm: {'n_cores': [], 'run_times': [], 'time_1core': 0.0, 'last_speedup': 0.0}
                       for algorithm in self.run_config['run_params']['algorithms']}

        # sorting wrt increasing n_cores
        for _, data in self.all_data_dict.iteritems():
            algo = data['algorithm']
            algo_t_core[algo]['n_cores'].append(data['n_cores'])
            algo_t_core[algo]['run_times'].append(data['mean_run_tim'])
            if data['n_cores'] == 1:
                algo_t_core[algo]['time_1core'] = data['mean_run_tim']

        max_n_core = 1
        last_speedups = []
        for algorithm, algo_data in algo_t_core.iteritems():
            n_cores, run_times = plot_utils.sort_two_lists_wrt_first(algo_data['n_cores'], algo_data['run_times'])
            speedups = [algo_data['time_1core'] / run_times[n] for n in range(len(run_times))]

            if max(n_cores) > max_n_core:
                max_n_core = max(n_cores)
            last_speedups.append(speedups[-1])

            ax.plot(n_cores, speedups, linewidth=3, label=algorithm, alpha=0.5)
            ax.plot(n_cores, speedups, 'o', linewidth=3, alpha=0.3, color='black', markeredgecolor='none')
            print algorithm
        ax.plot([1, max_n_core], [1.0, max_n_core], 'k--', linewidth=1, alpha=0.5, label='linear speedup')

        order = np.argsort(last_speedups)[::-1]
        plot_utils.ordered_legend(ax, order)
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

