"""
Simple helper functions for plot generation and storage.
"""

import os.path
import matplotlib.pyplot as plt
import numpy as np
import matplotlib


def setup_figure_1ax(x_label='', y_label='', size=(13, 9), shrink_ax=True):
    """Returns a figure and ax with legend on the right hand side, no spines."""
    matplotlib.rcParams.update({'font.size': 22})
    fig, ax = plt.subplots()
    fig.set_size_inches(size)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['bottom'].set_visible(False)
    ax.spines['left'].set_visible(False)
    # Shrink current axis by 20%
    if shrink_ax:
        box = ax.get_position()
        ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    return fig, ax


def sort_two_lists_wrt_first(list1, list2):
    sort_indices = np.argsort(np.array(list1))
    list1_sorted = [list1[i] for i in sort_indices]
    list2_sorted = [list2[i] for i in sort_indices]
    return list1_sorted, list2_sorted


def ordered_legend(ax, order=None):
    """
    Sorts the labels of a legend of an ax wrt provided order.
    """
    ax.legend()
    handles, labels = plt.gca().get_legend_handles_labels()
    ax.legend([handles[idx] for idx in order], [labels[idx] for idx in order],
              loc='center left', bbox_to_anchor=(1, 0.5), frameon=False)


def show_and_or_save(save, show, save_file_path=None):
    """
    Saving and/or showing the current figure.
    :param save: Save figure to file?
    :param show: Show figure?
    :param save_file_path: Full path to desired .png file to be created. Output dir has to exist.
    :return:
    """
    if save:
        assert save_file_path is not None
        # create post processing folder
        if not os.path.isdir(os.path.dirname(save_file_path)):
            raise IOError("The folder you want to store in doesn\'t exist:\n{}".format(os.path.dirname(save_file_path)))
        plt.savefig(save_file_path)
    if show:
        plt.show()