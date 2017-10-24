import os
import numpy as np
# import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

from Point import Point


class Chan2DVisualizer:

    def __init__(self, fig, ax):
        self.fig = fig
        self.ax = ax
        self.ax.set_xlim(0, 1)
        self.ax.set_ylim(0, 1)

        # holding the points which have been plotted already
        self.x_points = []
        self.y_points = []
        self.x_lines = []
        self.x_lines = []

        self.points, = self.ax.plot(self.x_points, self.y_points, 'go')
        self.line, = self.ax.plot(self.x_points, self.y_points, linestyle='-')

        self.all_points = None
        self.all_hull_points = None
        self.hull_data = {}

        self.subset_sizes = []
        self.current_subset_idx = 0
        self.current_point_idx = 0

        self.colors = ['red', 'blue', 'green']

    def plot_points(self):
        """Plots all points and all hull points in two different colors."""
        x_all = [point.x for point in self.all_points]
        y_all = [point.y for point in self.all_points]
        x_hull = [point.x for point in self.all_hull_points]
        y_hull = [point.y for point in self.all_hull_points]

        self.ax.scatter(x_all, y_all, s=10, c='red', alpha=0.7)
        self.ax.scatter(x_hull, y_hull, s=200, c='black')

        for axis in [self.ax.get_xaxis(), self.ax.get_yaxis()]:
            axis.set_ticks([])

    @staticmethod
    def load_points_only(chan_data_file):
        data = np.loadtxt(chan_data_file, delimiter=',')
        x_list = [item[0] for item in data]
        y_list = [item[1] for item in data]

        return [Point(x_list[i], y_list[i]) for i in range(len(x_list))]

    @staticmethod
    def load_points_turn(chan_data_file):
        data = np.loadtxt(chan_data_file, delimiter=',')

        x_list = [item[0] for item in data]
        y_list = [item[1] for item in data]
        turn = [item[2] for item in data]

        return [Point(x_list[i], y_list[i], turn[i]) for i in range(len(x_list))]

    def load_graham_subsets(self, filename_template, n_files):
        """Loads all graham subset histories into a dictionary where the keys are graham subset indices
        and every subset consist of a list of points."""

        file_names = [filename_template.format(idx) for idx in range(n_files)]
        for idx, file_name in enumerate(file_names):
            self.hull_data[idx] = self.load_points_turn(file_name)

    def load_all_points(self, all_points_file, all_hull_points_file):
        """Stores the initial particles list into self.all_points and the final (after chan) hull particles
        into self.all_hull_points."""
        self.all_points = self.load_points_only(all_points_file)
        self.all_hull_points = self.load_points_only(all_hull_points_file)

    def update(self, frame):
        """Updating the included data to plot."""
        if self.current_point_idx == self.subset_sizes[self.current_subset_idx]:
            self.current_subset_idx += 1
            self.current_point_idx = 0
            if self.current_subset_idx == len(self.subset_sizes):
                time.sleep(2000)
            print 'subset', self.current_subset_idx


        #print 'frame', frame
        print 'current frame', self.current_point_idx

        self.points.set_color(self.colors[self.current_subset_idx])
        self.x_points.append(self.hull_data[self.current_subset_idx][self.current_point_idx].x)
        self.y_points.append(self.hull_data[self.current_subset_idx][self.current_point_idx].y)

        self.points.set_data(self.x_points, self.y_points)
        self.line.set_data(self.x_points, self.y_points)

        self.current_point_idx += 1

        return self.points,


def main():
    data_base_path = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'cmake-build-debug')
    all_points_file = os.path.join(data_base_path, 'all_points.dat')
    hull_points_file = os.path.join(data_base_path, 'hull_points.dat')

    fig, ax = plt.subplots()
    fig.set_size_inches(7, 7)
    chan_visualizer = Chan2DVisualizer(fig, ax)
    graham_template = os.path.join(data_base_path, 'graham_history_{}.dat')
    chan_visualizer.load_graham_subsets(graham_template, n_files=3)

    chan_visualizer.load_all_points(all_points_file, hull_points_file)
    chan_visualizer.plot_points()

    chan_visualizer.subset_sizes = [len(graham_subset) for graham_subset in chan_visualizer.hull_data.values()]
    n_points = sum(chan_visualizer.subset_sizes)
    print n_points

    print chan_visualizer.subset_sizes
    ani = animation.FuncAnimation(fig,
                                  chan_visualizer.update,
                                  frames=n_points)
    
    
    plt.show()
if __name__ == '__main__':

    main()