"""
Plots the time vs number of threads.
"""

import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('data.txt')

n = data[:, 0]
time = data[:, 1]


def plot(n, time, name, xlabel, ylabel, xlim, ylim):
    fig, ax = plt.subplots(figsize=(13, 9))
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)
    ax.xaxis.set_ticks_position('bottom')
    ax.yaxis.set_ticks_position('left')
    ax.set_xlim([0, xlim])
    ax.set_ylim([0, ylim])

    ax.plot(n, time, 'ro', markeredgecolor='red')
    ax.plot(n, time, 'r--', linewidth = 2)
    if name == "speedup.png":
        ax.plot([0,24], [0,24], '--')
    plt.savefig(name)


def speed_up(time):
    speedup = time[0] / time
    print(speedup)
    return speedup

if __name__=='__main__':
    plot(n, time, 'time_vs_threads.png', 'Number of threads', 'Time [sec]', 25, 4.0)
    plot(n, speed_up(time), 'speedup.png', 'Number of threads', 'Speedup', 25, 25)
