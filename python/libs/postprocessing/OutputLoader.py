"""
Loads the complete output information of a GridRun from the Output folder based on the
run_config.py, the various params.json files of the sub runs as well as other output files, such as the timing files.
"""

import os
from os.path import join as join_paths
import json
from glob import glob


class OutputLoader:
    def __init__(self, output_dir_name):
        self.output_dir_name = output_dir_name
        self.output_dir_path = join_paths(os.path.abspath(__file__), '../../../', 'Output', output_dir_name)
        self.run_config = self.parse_run_config()
        self.sub_run_params = self.parse_sub_run_params()

    def parse_run_config(self):
        """Loads the json run_config file in the output folder into a dictionary."""
        config_file_name = self.output_dir_name + '_config.json'
        if not os.path.isfile(join_paths(self.output_dir_path, config_file_name)):
            raise IOError('Make sure the run exists. Maybe typo in run name?')

        with open(join_paths(self.output_dir_path, config_file_name), 'r') as infile:
            run_config = json.load(infile)
        return run_config

    def parse_sub_run_params(self):
        """Parses the all the json param files from the sub runs into one dict (keys are sub run indices)."""
        sub_dirs = glob(join_paths(self.output_dir_path, 'sub_?'))
        sub_run_params = {}
        for sub_idx, sub_dir in enumerate(sub_dirs):
            with open(join_paths(self.output_dir_path, 'sub_{}'.format(sub_idx), 'params.json'), 'r') as infile:
                sub_run_params[sub_idx] = json.load(infile)
        return sub_run_params

    '''Use those functions to pass data on to post processing module.'''
    def get_run_config(self):
        return self.run_config

    def get_sub_run_params(self):
        return self.sub_run_params


