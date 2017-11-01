"""
Loads data output from the C++ output files into python dictionary for easy access.
"""

from os.path import join as join_paths
from glob import glob
import pandas as pd
import numpy as np
import os


class DataLoader:

    def __init__(self, output_folder):
        self.output_folder = output_folder
        self.n_graham_subs = 0

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
        self.clear_unused_files()

        all_data_dict = {
            'n_graham_subs': self.n_graham_subs,
            'sub_sorted':  self.load_sub_files('out_sorted_sub_*.dat', 'xy_positions'),
            'hull_points': self.load_sub_files('out_hull_points_*.dat', 'xy_positions'),
            'graham_runs': self.load_sub_files('out_graham_sub_*.dat', 'graham_history'),
            'total_hull': self.load_single_pos_file('out_hull_points.dat'),
            'merge_hulls': self.load_single_pos_file('out_merge_hulls.dat')
        }

        return all_data_dict

    def clear_unused_files(self):
        with open(join_paths(self.output_folder, 'out_n_graham_subs.dat'), 'r') as infile:
            n_graham_subs = infile.readline()
        self.n_graham_subs = int(float(n_graham_subs))

        all_files = glob(join_paths(self.output_folder, 'out_*'))

        for outfile in all_files:
            last_letter_before_point = outfile.split('.')[0].split('_')[-1]
            if self.isinteger(last_letter_before_point):
                if int(last_letter_before_point) > (self.n_graham_subs - 1):
                    os.remove(outfile)

    def load_single_pos_file(self, filename):
        data = self.load_csv_file(join_paths(self.output_folder, filename))
        x = [item[0] for item in data]
        y = [item[1] for item in data]
        return {'x': x, 'y': y}

    @staticmethod
    def load_csv_file(file_path):
        return pd.read_csv(file_path, header=None).as_matrix()

    @staticmethod
    def isinteger(a):
        try:
            int(a)
            return True
        except ValueError:
            return False

if __name__ == '__main__':

    '''Example usage...'''
    loader = DataLoader('../Output')
    data_dict = loader.load_all_data()
