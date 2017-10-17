import os
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt


class Chan2DVisualizer:

    def __init__(self, all_points_file, hull_points_file):
        self.all_points = self.__class__.load_csv_file(all_points_file)
        self.hull_points = self.__class__.load_csv_file(hull_points_file)

    def main(self):
        self.plot_points()

    @staticmethod
    def load_csv_file(chan_data_file):
        data = np.loadtxt(chan_data_file, delimiter=',')
        x = [item[0] for item in data]
        y = [item[1] for item in data]
        return np.array([x, y])

    def plot_points(self):
        fig, ax = plt.subplots(figsize=(7, 7))
        ax.scatter(self.all_points[0], self.all_points[1], s=10, c='red', alpha=0.7)
        ax.scatter(self.hull_points[0], self.hull_points[1], s=20, c='blue')

        ax.get_xaxis().set_ticks([])
        ax.get_yaxis().set_ticks([])
        plt.show()

    def start_animation(self):
        pass


if __name__ == '__main__':
    all_points_file  = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'cmake-build-debug', 'all_points.dat')
    hull_points_file = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'cmake-build-debug', 'hull_points.dat')

    chan_visualizer = Chan2DVisualizer(all_points_file, hull_points_file)
    chan_visualizer.main()