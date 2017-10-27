"""
Loads data output from the C++ output files into python dictionary for easy access.
"""

from os.path import join as join_paths
from glob import glob
import pandas as pd


class DataLoader:

    def __init__(self, output_folder):
        self.output_folder = output_folder

    def load_sub_files(self, filename_template, file_format):
        """
        Loading the sorted points, hull points and graham history from every graham sub run.

        :param filename_template:
        :param file_format:
        :return: sub_files_dict, keys are subrun indices, entries are np arrays, data from csv file
        """
        assert file_format in ['xy_positions', 'graham_history']

        sub_files = glob(join_paths(self.output_folder, filename_template))
        sub_files_dict = {}

        for idx, sub_file in enumerate(sub_files):
            data = self.load_csv_file(sub_file.format(idx))

            if file_format == 'xy_positions':
                x = [item[0] for item in data]
                y = [item[1] for item in data]
                sub_files_dict[idx] = {'x': x, 'y': y}
            elif file_format == 'graham_history':
                sub_files_dict[idx] = data

        return sub_files_dict

    def load_all_data(self):

        all_data_dict = {
            'sub_sorted':  self.load_sub_files('sorted_sub_*.dat', 'xy_positions'),
            'hull_points': self.load_sub_files('hull_points_*.dat', 'xy_positions'),
            'graham_runs': self.load_sub_files('graham_sub_*.dat', 'graham_history'),
            'total_hull': self.load_all_hull_points('hull_points.dat')
        }

        return all_data_dict

    def load_all_hull_points(self, filename):
        return self.load_csv_file(join_paths(self.output_folder, filename))

    @staticmethod
    def load_csv_file(file_path):
        return pd.read_csv(file_path, header=None).as_matrix()


if __name__ == '__main__':

    '''Example usage...'''
    loader = DataLoader('C:\Users\mathee\CLionProjects\dphpc-project\cmake-build-debug')
    data_dict = loader.load_all_data()