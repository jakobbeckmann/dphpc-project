import os
import numpy as np
# import matplotlib as mpl
import matplotlib.pyplot as plt
import itertools


class Chan2DVisualizer:

    def __init__(self, all_points_file, hull_points_file):
        self.all_points = self.__class__.load_csv_file(all_points_file)
        self.hull_points = self.__class__.load_csv_file(hull_points_file)

    def plot_points(self):
        fig, ax = plt.subplots()
        fig.set_size_inches(7, 7)

        ax.scatter(self.all_points[0], self.all_points[1], s=10, c='red', alpha=0.7)
        ax.scatter(self.hull_points[0], self.hull_points[1], s=20, c='blue')

        '''for axis in [ax.get_xaxis(), ax.get_yaxis()]:
            axis.set_ticks([])'''

        plt.show()

    def start_animation(self):
        pass

    @staticmethod
    def load_csv_file(chan_data_file):
        data = np.loadtxt(chan_data_file, delimiter=',')
        x = [item[0] for item in data]
        y = [item[1] for item in data]
        return np.array([x, y])


def main():
    data_base_path = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'cmake-build-debug')
    all_points_file = os.path.join(data_base_path, 'all_points.dat')
    hull_points_file = os.path.join(data_base_path, 'hull_points.dat')

    chan_visualizer = Chan2DVisualizer(all_points_file, hull_points_file)
    chan_visualizer.plot_points()

if __name__ == '__main__':

    main()