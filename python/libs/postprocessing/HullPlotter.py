"""
Simply plots input points and final hull points for visual check of output correctness.
"""

import cv2
import numpy as np

import plot_utils


class HullPlotter:
    def __init__(self, input_points, hull_points, save_path, input_img_path=None):
        """
        :param input_points: dictionary, keys: ['x', 'y'], values: list of float for coordinates
        :param hull_points: dictionary, keys: ['x', 'y'], values: list of float for coordinates
        :param input_img_path: Full path to original input image
        """
        self.input_points = input_points
        self.hull_points = hull_points
        self.input_img_path = input_img_path
        self.save_path = save_path

    def plot_input_and_hull_points(self, save, show, plot_img=False):
        fig, ax = plot_utils.setup_figure_1ax(shrink_ax=False)
        ax.scatter(self.input_points['x'], self.input_points['y'], s=2)
        ax.scatter(self.hull_points['x'], self.hull_points['y'], s=10)

        if plot_img:
            image = cv2.imread(self.input_img_path)
            ax.imshow(np.flipud(image), origin='lower', alpha=0.3)

        plot_utils.show_and_or_save(save=save, show=show, save_file_path=self.save_path)
