"""
Top level module reading run_config json file and holding functionality to execute the whole pipeline.
"""

import datetime
import json
import os
import subprocess
from glob import glob
from os.path import join as join_paths
import numpy as np
import shutil

from python.libs.data_acquisition.ImagePointsCreator import ImagePointsCreator
from python.libs.paths import project_path
from python.libs.postprocessing.HullPlotter import HullPlotter


class GridRunHandler:
    def __init__(self, custom_config='run_config.py', custom_out_dir='Output'):
        """
        :param custom_config: In case you want to specify another config file (e.g. to re-run a whole run),
                                   give the name of the run, e.g. reproduce_run_name = 'test_1130_180553'
        """
        self.run_config_file = self.set_run_config_file(custom_config)
        self.output_dir_path = self.setup_output_folder(custom_out_dir)
        self.run_config = self.load_run_config()
        self.this_path = os.path.abspath(__file__)
        self.run_params = self.run_config['run_params']
        self.post_process_params = self.run_config['post_process_params']
        self.run_name = self.create_run_name()
        self.store_run_config_json()
        self.input_files = {}
        self.exe_dir_name = self.run_config['exe_dir_name']

    def set_run_config_file(self, custom_config):
        if custom_config is not 'run_config.py':
            custom_config = join_paths(os.path.dirname(self.this_path), 'run_config.py')
            if os.path.isfile(custom_config):
                print "Choosing config: {}".format(custom_config)
                return join_paths(custom_config)
        else:
            print "Choosing config: {}".format(os.path.dirname(self.this_path), 'run_config.py')
            return join_paths(os.path.dirname(self.this_path), 'run_config.py')

    def load_run_config(self):
        with open(self.run_config_file, 'r') as config_file:
            run_config = eval(config_file.read())
        return run_config

    def setup_output_folder(self, custom_out_dir):
        """
        Creates the output directory for the current run in the projects output folder.
        Copies the run_config.py file to the new location with adapted name.
        """
        if custom_out_dir is not 'Output':
            assert os.path.exists(os.path.dirname(custom_out_dir))
            output_dir_path = join_paths(custom_out_dir, self.run_name)
        else:
            output_dir_path = join_paths(project_path, custom_out_dir, self.run_name)

        if not os.path.isdir(output_dir_path):
            os.mkdir(output_dir_path)
        else:
            print("You fool run twice in a second... Exit.")
            exit(1)

        return output_dir_path

    def store_run_config_json(self):
        with open(join_paths(self.output_dir_path, self.run_name + '_config.json'), 'w') as outfile:
            outfile.write('{}\n'.format(json.dumps(self.run_config, outfile, indent=4)))

    def create_input_files(self, weak_scaling=False):
        input_dir_path = join_paths(self.output_dir_path, 'input_data')
        os.mkdir(input_dir_path)

        for n_points_ in self.run_params['n_points']:
            for img_file in self.run_params['img_files']:
                for scale in (self.run_params['n_cores'] if weak_scaling else [1]):
                    if weak_scaling:
                        n_points = n_points_*scale
                    else:
                        n_points = n_points_

                    print('\nCreating points for {np} points using {img}.'.format(np=n_points, img=img_file))
                    point_file_name = self.create_input_filename(n_points, img_file, 'dat')
                    point_creator = ImagePointsCreator(n_points, img_file, point_file_name, input_dir_path)
                    point_creator.create_points_pipeline(save_png=self.run_config['save_png_input'], clusters=self.run_params['clusters'])
                    shutil.copy(join_paths(project_path, 'Input', img_file),
                                join_paths(input_dir_path))

    def run_algorithms(self, n_cores, n_points, input_dat, input_png, algorithm, sub_size, n_iterations, img, sub_dir,
                       sub_dir_idx):
        """
        Function executing subprocess calls to .cpp executable forwarding parameter as
        command line inputs.
        """

        # step1: store run parameters of sub run in sub run folder
        sub_params = {'n_points': n_points, 'n_cores': n_cores, 'input_dat': input_dat, 'input_png': input_png,
                      'algorithm': algorithm, 'sub_size': sub_size, 'n_iterations': n_iterations,
                      'img': img, 'dir_idx': sub_dir_idx}

        self.print_dict(sub_params)

        with open(join_paths(sub_dir, 'params.json'), 'w') as outfile:
            json.dump(sub_params, outfile, indent=4, sort_keys=True)

        # step2: run the algorithms specified by parameter grid inside of this folder
        exe_file = glob(join_paths(project_path, self.exe_dir_name, 'dphpc_project*'))
        assert len(exe_file) == 1, 'Could not find executable. Check your config file for correct exe_dir_name param!\n' \
                                   'Files found in {}: {}'.format(join_paths(project_path, self.exe_dir_name),
                                                                             glob(join_paths(project_path, self.exe_dir_name)))

        input_file_path = join_paths(self.output_dir_path, 'input_data', input_dat)

        call_command = [exe_file[0], str(n_cores), str(sub_size), input_file_path, str(n_points), str(n_iterations), algorithm]
        call_command_str = ' '.join(call_command)

        subprocess.check_call(call_command_str, shell=True, cwd=sub_dir)

        # step3: for debugging purposes, you can directly create the final hull plot of the last iteration
        if self.post_process_params['store_final_plots']:
            # loading hull points
            hull_data = np.loadtxt(join_paths(sub_dir, 'hull_points_{iter_idx}.dat'.format(iter_idx=n_iterations-1)),
                                   delimiter=',')
            hull_points = {'x': hull_data[:, 0], 'y': hull_data[:, 1]}
            # loading input points
            in_data = np.loadtxt(join_paths(self.output_dir_path, 'input_data', input_dat))
            input_points = {'x': in_data[:, 0], 'y': in_data[:, 1]}

            plot_name = 'last_iter_hull.png'
            plotter = HullPlotter(input_points, hull_points, save_path=join_paths(sub_dir, plot_name),
                                  input_img_path=join_paths(self.output_dir_path, 'input_data', img))
            plotter.plot_input_and_hull_points(save=True, show=False, plot_img=True)

    def main_grid_loop(self, weak_scaling=False):
        """Main function looping over all possible parameter configurations executing the algorithms."""
        dir_index = 0
        for n_points_ in self.run_params['n_points']:
            for img_file in self.run_params['img_files']:
                for n_cores in self.run_params['n_cores']:
                    if weak_scaling:
                        n_points = n_points_ * n_cores
                    else:
                        n_points = n_points_

                    input_file = self.create_input_filename(n_points, img_file, 'dat')

                    sub_sizes = [n_points//n_cores]
                    if self.run_params['use_sub_sizes']:
                        sub_sizes = self.run_params['sub_size']

                    for sub_size in sub_sizes:
                        for algorithm in self.run_params['algorithms']:

                            sub_dir = join_paths(self.output_dir_path, 'sub_' + str(dir_index))
                            os.mkdir(sub_dir)

                            n_iterations = self.run_params['n_iterations']

                            self.run_algorithms(n_cores=n_cores,
                                                n_points=n_points,
                                                input_dat=input_file,
                                                input_png=input_file.split('.')[0] + '.png',
                                                img=img_file,
                                                algorithm=algorithm,
                                                sub_size=sub_size,
                                                n_iterations=n_iterations,
                                                sub_dir=sub_dir,
                                                sub_dir_idx=dir_index)
                            dir_index += 1

        self.store_all_sub_json_params()

    def store_all_sub_json_params(self):
        """Stores a single json file holding all the parameters in main run output folder."""
        all_params = {}
        all_sub_dir_paths = glob(join_paths(self.output_dir_path, 'sub_*', 'params.json'))

        for sub_dir in all_sub_dir_paths:
            sub_idx = os.path.split(sub_dir)[-2].split('_')[-1]  # parsing the sub_idx out of the sub_dir path
            with open(sub_dir, 'r') as infile:
                json_data = json.load(infile)
                all_params[sub_idx] = json_data

        with open(join_paths(self.output_dir_path, self.run_name + '_all_params.json'), 'w') as outfile:
            json.dump(all_params, outfile, indent=4)

    def create_run_name(self):
        dt = datetime.datetime.now()
        dt_appendix = '{:02d}{:02d}_{:02d}{:02d}{:02d}'.format(dt.month, dt.day, dt.hour, dt.minute, dt.second)
        return self.run_config['run_name'] + '_' + dt_appendix

    @staticmethod
    def create_input_filename(n_points, img_name, suffix):
        img_name_no_suffix = img_name.split('.')[0]
        return 'points_n_{np}_img_{img}.{suffix}'.format(np=n_points, img=img_name_no_suffix, suffix=suffix)

    @staticmethod
    def print_dict(dictionary):
        print '\n' + 30*'= '
        for key, value in dictionary.iteritems():
            print '{:15}: {}'.format(key, value)

    @staticmethod
    def parse_json_points(directory, file_name):
        with open(join_paths(directory, file_name), 'r') as json_file:
            return json.load(json_file)
