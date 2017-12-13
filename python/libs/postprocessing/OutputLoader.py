"""
Loads the complete output information of a GridRun from the Output folder based on the
run_config.py, the various params.json files of the sub runs as well as other output files, such as the timing files.
"""

import os
from os.path import join as join_paths
import json
import numpy as np

from python.libs.paths import project_path


class OutputLoader:
    def __init__(self, output_dir_name, latest_output=False, save_all_data=True):
        self.output_dir_name = self.set_output_dir_name(output_dir_name, latest_output)
        self.output_dir_path = join_paths(project_path, 'Output', self.output_dir_name)
        self.all_data_dict = {}
        self.run_config = self.parse_run_config()
        self.sub_run_params = self.parse_sub_run_params()
        pass
    def parse_run_config(self):
        """Loads the json run_config file in the output folder into a dictionary."""
        config_file_name = self.output_dir_name + '_config.json'
        if not os.path.isfile(join_paths(self.output_dir_path, config_file_name)):
            raise IOError('Make sure the run exists. Maybe typo in run name?')

        with open(join_paths(self.output_dir_path, config_file_name), 'r') as infile:
            run_config = json.load(infile)

        return run_config

    @staticmethod
    def set_output_dir_name(output_dir_name, latest_output):
        if latest_output:
            global_out_dir = join_paths(project_path, 'Output')

            all_output_paths = [d for d in os.listdir(global_out_dir) if os.path.isdir(join_paths(global_out_dir, d))]

            latest_mmdd = 0
            latest_hhmmss = 0
            latest_out_dir = ''
            for out_dir in all_output_paths:
                name = os.path.split(out_dir)[-1]
                mmdd, hhmmss = int(name.split('_')[-2]), int(name.split('_')[-1])
                if mmdd > latest_mmdd:
                    latest_mmdd = mmdd
                    latest_out_dir = out_dir
                elif mmdd == latest_mmdd:
                    if hhmmss > latest_hhmmss:
                        latest_hhmmss = hhmmss
                        latest_out_dir = out_dir
            output_dir_name = os.path.split(latest_out_dir)[-1]
            print('\nChose latest output run: ' + output_dir_name)
            return output_dir_name
        else:
            return output_dir_name

    def parse_sub_run_params(self):
        """Parses the all the json param files from the sub runs into one dict (keys are sub run indices)."""
        all_params_file_path = join_paths(self.output_dir_path, self.output_dir_name + '_all_params.json')
        with open(all_params_file_path, 'r') as infile:
            sub_run_params = json.load(infile)
            self.all_data_dict = sub_run_params  # initially they are the same, data is being added later
        return sub_run_params

    def load_all_data_dict(self, save_all_data=True):
        sub_indices = self.sub_run_params.keys()

        for sub_idx in sub_indices:
            algorithm = self.sub_run_params[sub_idx]['algorithm']
            times = np.loadtxt(join_paths(self.output_dir_path, 'sub_{}'.format(sub_idx), 'timing_{}.txt'.format(algorithm)))
            self.all_data_dict[sub_idx]['run_times'] = list(times[:, 1])
            self.all_data_dict[sub_idx]['mean_run_tim'] = np.median(times[:, 1])

        if save_all_data:
            with open(join_paths(self.output_dir_path, 'all_data.json'), 'w') as outfile:
                json.dump(self.all_data_dict, outfile, indent=4, sort_keys=True)

    '''Use those functions to pass data on to post processing module.'''
    def get_all_data_dict(self):
        return self.all_data_dict

    def get_run_config(self):
        return self.run_config

    def get_sub_run_params(self):
        return self.sub_run_params


