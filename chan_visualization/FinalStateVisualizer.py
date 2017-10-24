"""
Get a quick look on the final output of Chan's Algorithm.
"""

import numpy as np
import matplotlib.pyplot as plt
import os


class FinalStateVisualizer:

    def __init__(self, all_points_file, hull_points_file):
        self.all_points = self.__class__.load_csv_file(all_points_file)
        self.hull_points = self.__class__.load_csv_file(hull_points_file)

    def plot_points(self, show_axis):
        fig, ax = plt.subplots()
        fig.set_size_inches(7, 7)

        ax.scatter(self.all_points[0], self.all_points[1], s=30, c='red', alpha=0.8, edgecolors='none')
        ax.scatter(self.hull_points[0], self.hull_points[1], s=150, c='cyan', alpha=0.5, edgecolors='none')
        ax.plot(self.hull_points[0], self.hull_points[1], 'c-')
        ax.plot([self.hull_points[0][0], self.hull_points[0][-1]],
                [self.hull_points[1][0], self.hull_points[1][-1]], 'c-')

        if not show_axis:
            ax.axis('off')

        '''for axis in [ax.get_xaxis(), ax.get_yaxis()]:
            axis.set_ticks([])'''

        plt.show()

    @staticmethod
    def load_csv_file(chan_data_file):
        data = np.loadtxt(chan_data_file, delimiter=',')
        x = [item[0] for item in data]
        y = [item[1] for item in data]
        return np.array([x, y])


def main():
    data_base_path = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'cmake-build-debug')
    all_points_file = os.path.join(data_base_path, 'all_sorted.dat')
    hull_points_file = os.path.join(data_base_path, 'hull_points.dat')

    chan_visualizer = FinalStateVisualizer(all_points_file, hull_points_file)
    chan_visualizer.plot_points(show_axis=False)

if __name__ == '__main__':

    main()