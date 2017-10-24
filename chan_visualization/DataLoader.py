"""
Loads data output from the C++ output files into python dictionary for easy access.
"""

import numpy as np
from os.path import join as join_paths
from glob import glob


class DataLoader:

    def __init__(self, output_folder):
        self.output_folder = output_folder

    def load_points_xy(self, filename):
        """Returns a numpy array containing two arrays, one for x, one for y."""
        data = self.load_csv_file(join_paths(self.output_folder, filename))
        x = [item[0] for item in data]
        y = [item[1] for item in data]
        return {'x': x, 'y': y}

    def load_graham_histories(self):
        """Loading the graham subscan files and storing all lines in numpy ndarray."""
        graham_sub_files = glob(join_paths(self.output_folder, 'graham_sub_*.dat'))

        graham_dict = {}
        for idx, graham_file in enumerate(graham_sub_files):
            graham_dict[idx] = self.load_csv_file(graham_file.format(idx)).astype(int)

        return graham_dict

    def load_all_data(self):

        all_data_dict = {
            'all_sorted':  self.load_points_xy('all_sorted.dat'),
            'hull_points': self.load_points_xy('hull_points.dat'),
            'graham_runs': self.load_graham_histories()
        }

        return all_data_dict

    @staticmethod
    def load_csv_file(file_path):
        return np.loadtxt(file_path, delimiter=',')

if __name__ == '__main__':
    loader = DataLoader('C:\Users\mathee\CLionProjects\dphpc-project\cmake-build-debug')
    data_dict = loader.load_all_data()

    pass
