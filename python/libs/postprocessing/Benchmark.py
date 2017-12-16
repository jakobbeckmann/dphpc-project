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
import scipy.stats


class Benchmark:
    def __init__(self, run_config, all_params, all_data_dict):
        self.run_config = run_config
        self.all_params = all_params
        self.all_data_dict = all_data_dict

    def plot_runtimes_comparison(self, save=False, show=False, file_name=None):
        """
        Plots the absolute runtime vs input size for all algorithms in the run.

        For now, the runtimes are the median over all iterations for a run configuration.
        """

        if len(self.run_config['run_params']['n_cores']) > 1 or len(self.run_config['run_params']['sub_size']) > 1 \
                or len(self.run_config['run_params']['img_files']) > 1:
            raise NotImplementedError("You can't do that. Only multiple input sizes and multiple algos.")

        fig, ax = plot_utils.setup_figure_1ax(size=(13, 9),
                                              x_label='Input size [number of points]',
                                              y_label='Run time [s]')

        # set up empty data container
        algo_runtimes = {algorithm: {'input_sizes': [], 'run_times': [], 'errors': []}
                         for algorithm in self.run_config['run_params']['algorithms']}

        for key, data in self.all_data_dict.iteritems():
            algo_runtimes[data['algorithm']]['input_sizes'].append(data['n_points'])
            algo_runtimes[data['algorithm']]['run_times'].append(data['median_run_times'])
            algo_runtimes[data['algorithm']]['errors'].append(data['errors'])

        last_runtimes = []
        for algorithm, algo_data in algo_runtimes.iteritems():
            sort_indices = np.argsort(np.array(algo_data['input_sizes']))

            input_sizes = [algo_data['input_sizes'][i] for i in sort_indices]
            run_times = [algo_data['run_times'][i] for i in sort_indices]

            errors = [algo_data['errors'][i] for i in sort_indices]
            errors = [[err[0] for err in errors], [err[1] for err in errors]]

            last_runtimes.append(run_times[-1])
            ax.plot(input_sizes, run_times, linewidth=3, label=algorithm, alpha=0.5)
            ax.errorbar(input_sizes, run_times, yerr=errors, fmt='none', alpha=0.5, color='black', capsize=3)
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
        algo_t_core = {algorithm: {'n_cores': [], 'run_times': [], 'time_1core': 0.0,
                                   'last_speedup': 0.0, 'errors': []}
                       for algorithm in self.run_config['run_params']['algorithms']}

        # sorting wrt increasing n_cores
        for _, data in self.all_data_dict.iteritems():
            algo = data['algorithm']
            algo_t_core[algo]['n_cores'].append(data['n_cores'])
            algo_t_core[algo]['run_times'].append(data['median_run_times'])
            algo_t_core[algo]['errors'].append(data['errors'])
            if data['n_cores'] == 1:
                algo_t_core[algo]['time_1core'] = data['median_run_times']

        max_n_core = 1
        last_speedups = []
        for algorithm, algo_data in algo_t_core.iteritems():
            n_cores, run_times = plot_utils.sort_two_lists_wrt_first(algo_data['n_cores'], algo_data['run_times'])
            speedups = [algo_data['time_1core'] / run_times[n] for n in range(len(run_times))]
            errors = algo_data['errors']
            errors = [[err[0] for err in errors], [err[1] for err in errors]]

            if max(n_cores) > max_n_core:
                max_n_core = max(n_cores)
            last_speedups.append(speedups[-1])

            ax.plot(n_cores, speedups, linewidth=3, label=algorithm, alpha=0.5)
            ax.plot(n_cores, speedups, 'o', linewidth=3, alpha=0.3, color='black', markeredgecolor='none')
            ax.errorbar(n_cores, speedups, yerr=errors, fmt='none', alpha=0.5, color='black', capsize=3)

            print algorithm
        ax.plot([1, max_n_core], [1.0, max_n_core], 'k--', linewidth=1, alpha=0.5, label='linear speedup')

        order = np.argsort(last_speedups)[::-1]
        plot_utils.ordered_legend(ax, order)
        self.evaluate_save_show(save, show, file_name)

    def plot_shape_comparison(self, save=False, show=False, file_name=None):
        """
        Plots the absolute runtime vs input size for all algorithms in the run.

        For now, the runtimes are the median over all iterations for a run configuration.
        """

        if len(self.run_config['run_params']['n_cores']) > 1 or len(self.run_config['run_params']['sub_size']) > 1 \
                or len(self.run_config['run_params']['n_points']) > 1:
            raise NotImplementedError("You can't do that. Only multiple input images and multiple algos allowed.")

        fig, ax = plot_utils.setup_figure_1ax(size=(13, 9),
                                              y_label='Run time [s]')

        # set up empty data container
        algo_runtimes = {algorithm: {'input_imgs': [], 'run_times': [], 'errors': []}
                         for algorithm in self.run_config['run_params']['algorithms']}

        for key, data in self.all_data_dict.iteritems():
            algo_runtimes[data['algorithm']]['input_imgs'].append(data['img'])
            algo_runtimes[data['algorithm']]['run_times'].append(data['median_run_times'])
            algo_runtimes[data['algorithm']]['errors'].append(data['errors'])

        img_names = self.run_config['run_params']['img_files']
        img_names = [img_name.split('.')[0] for img_name in img_names]
        n_images = len(img_names)
        ind_pos = np.arange(n_images)

        width = 0.2
        offset = {}
        algo_idx = 0

        for algorithm in algo_runtimes.keys():
            offset[algorithm] = algo_idx * width
            algo_idx += 1

        # sort wrt to JARVIS algorithm
        sort_indices = np.argsort(np.array(algo_runtimes['jarvis']['run_times']))

        for algorithm, algo_data in algo_runtimes.iteritems():
            run_times = [algo_data['run_times'][i] for i in sort_indices]
            img_names = [algo_data['input_imgs'][i] for i in sort_indices]
            errors = [algo_data['errors'][i] for i in sort_indices]
            errors = [[err[0] for err in errors], [err[1] for err in errors]]

            print ''
            print '{:10} {}'.format('--- algo: ', algorithm)
            print '{:10} {}'.format('images: ', img_names)
            print '{:10} {}'.format('times: ', [round(t, 3) for t in run_times])

            ax.bar(ind_pos + offset[algorithm], run_times, width, label=algorithm, alpha=0.7)
            ax.errorbar(ind_pos + offset[algorithm], run_times, yerr=errors, fmt='.', capsize=3, alpha=0.7)

        ax.xaxis.set_ticks_position('none')
        ax.set_xticks(np.arange(len(img_names)))
        ax.set_xticklabels(img_names, rotation=45, ha="right")
        ax.set_title("Input size: {n_points} points".format(n_points=self.run_config['run_params']['n_points'][0]))
        plt.xticks(ha='left')

        ax.legend(loc='center left', bbox_to_anchor=(1, 0.5), frameon=False)

        self.evaluate_save_show(save, show, file_name)

    def plot_time_distributions(self, save=False, show=False, file_name=None):
        fig, ax = plot_utils.setup_figure_1ax(size=(13, 9),
                                              x_label='Run time [s]',
                                              y_label='Density')

        assert len(self.all_data_dict.keys()) == 1

        run_times = self.all_data_dict['0']['run_times']

        median = np.median(run_times)

        # confidence intervals assuming gaussian distribution
        '''
        n = len(run_times)
        se = scipy.stats.sem(run_times)
        confidence = 0.95
        h = se * scipy.stats.t.ppf((1+confidence)/2., n-1)
        ci_1 = median-h
        ci_2 = median+1
        ax.errorbar(median, max(n) / 2., xerr=np.array([[ci_1, ci_2]]).T, linewidth=1, color='black', alpha=0.5, capsize=3, fmt='o')
        '''

        # quantile lines
        q5 = np.percentile(run_times, 5)
        q95 = np.percentile(run_times, 95)

        weights = np.ones_like(run_times)/float(len(run_times))
        n, bins, patches = ax.hist(run_times, bins=2000, facecolor='cyan', alpha=0.75, weights=weights)
        ax.plot([q5, q5], [0.001, 0.15], linewidth=3, color='blue', alpha=0.5)
        ax.plot([median, median], [0.001, 0.12], linewidth=3, color='red', alpha=0.5)
        ax.plot([q95, q95], [0.001, 0.09], linewidth=3, color='blue', alpha=0.5)
        # ax.set_xscale('log')

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

