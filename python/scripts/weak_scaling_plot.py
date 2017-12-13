import re
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

from python.libs.postprocessing import plot_utils

filename = sys.argv[1] # Containing the output of a grid_run_exe_weak_scaling run

assume_correct = 'chan_normal' #'graham'

def parse_result(s):
	res = {}
	entries = [re.split(': *', i) for i in s.split('\n')]
	for e in entries:
		res[e[0]] = e[1]
	return res


with open(filename, 'r') as infile:
	content = infile.read()

	c = content.split('=========Result=========\n')
	e = []
	for i in c:
		if i.startswith("Algorithm:"):
			e.append(parse_result('\n'.join(i.split('\n')[:8]))) # Currently we have 8 lines of somewhat interesting output

	per_algo_per_size = {}
	for i in e:
		if i['Algorithm'] not in per_algo_per_size:
			per_algo_per_size[i['Algorithm']] = {}
		per_size = per_algo_per_size[i['Algorithm']]

		if i['Input size'] not in per_size:
			per_size[i['Input size']] = { 'size': i['N hull points'], 'run_times': [] }
		cur = per_size[i['Input size']]
		if i['N hull points'] != cur['size']:
			print(i['Algorithm'] + " is non-deterministic")

		cur['run_times'].append(float(i['Time used']))

	# Check output for correctness
	per_algo_per_size_correct = {}
	for algo in per_algo_per_size:
		per_algo_per_size_correct[algo] = {}
		per_size_correct = per_algo_per_size_correct[algo]

		per_size = per_algo_per_size[algo]
		for size in per_size:
			hull_size = per_algo_per_size[assume_correct][size]['size']
			if per_size[size]['size'] != hull_size:
				print(algo + " has different output size from "+assume_correct+" at size " + size)
				print(per_size[size]['size'], hull_size)
			else:
				per_size_correct[size] = per_size[size]
				per_size_correct[size]['mean_run_time'] = np.mean(per_size[size]['run_times'])

	input_size_per_core = sorted(per_algo_per_size_correct[assume_correct])[0]

	fig, ax = plot_utils.setup_figure_1ax(size=(13, 9), x_label='# cores\nInput size (scaled with #cores): '+input_size_per_core, y_label="Run time") # scale

	# Only show integer number of cores
	ax.xaxis.set_major_locator(MaxNLocator(integer=True))


	order = []
	for algo in sorted(per_algo_per_size_correct):
		if not per_algo_per_size_correct[algo]:
			continue

		# Or just show input_sizes/input_sizes[0], which is the number of cores used
		# and show the input size per core somewhere else
		input_sizes = [int(i)/int(input_size_per_core) for i in sorted(per_algo_per_size_correct[algo])]
		mean_run_times = [per_algo_per_size_correct[algo][size]['mean_run_time'] for size in sorted(per_algo_per_size_correct[algo])]
		order.append(per_algo_per_size_correct[algo][sorted(per_algo_per_size_correct[algo])[-1]])

		ax.plot(input_sizes, mean_run_times, linewidth=3, label=algo)
		ax.plot(input_sizes, mean_run_times, 'o', linewidth=3, color='black', markeredgecolor='none')
		# TODO also list num cores for input size

		ax.legend(loc='center left', bbox_to_anchor=(1, 0.5), frameon=False)

	order = np.argsort(order)[::-1]
	plot_utils.ordered_legend(ax, order)

	plt.show()
	
