"""
Top level module reading run_config json file and holding functionality to execute the whole pipeline.
"""

import datetime
import json
import os
import subprocess
from glob import glob
from os.path import join as join_paths

from run_config import run_config
from python.libs.execution.ImagePointsCreator import ImagePointsCreator
from python.libs.paths import project_path


class GridRunHandler:
    def __init__(self, custom_config_file=None):  # TODO: Implement custom run_config location
        """
        :param custom_config_file: In case you want to specify another config file (e.g. to re-run a whole run),
                                   give the whole path to this config file.
        """
        self.this_path = os.path.abspath(__file__)
        self.run_params = run_config['run_params']
        self.post_process_params = run_config['post_process_params']
        self.run_name = self.create_run_name()
        self.run_config_file = self.set_run_config_file(custom_config_file)
        self.output_dir_path = self.setup_output_folder()
        self.store_run_config_json()
        self.input_files = {}
        self.exe_dir_name = run_config['exe_dir_name']

    def set_run_config_file(self, custom_config_file):
        if custom_config_file is not None:
            raise NotImplementedError
        else:
            return join_paths(os.path.dirname(self.this_path), 'run_config.py')

    def setup_output_folder(self):
        """
        Creates the output directory for the current run in the projects output folder.
        Copies the run_config.py file to the new location with adapted name.
        """
        output_dir_path = join_paths(project_path, 'Output', self.run_name)

        if not os.path.isdir(output_dir_path):
            os.mkdir(output_dir_path)
        else:
            print("You fool run twice in a second... Exit.")
            exit(1)

        return output_dir_path

    def store_run_config_json(self):
        with open(join_paths(self.output_dir_path, self.run_name + '_config.json'), 'w') as outfile:
            outfile.write('{}\n'.format(json.dumps(run_config, outfile, indent=4)))

    def create_input_files(self):
        input_dir_path = join_paths(self.output_dir_path, 'input_data')
        os.mkdir(input_dir_path)

        for n_points in self.run_params['n_points']:
            for img_file in self.run_params['img_files']:
                print('\nCreating points for {np} points using {img}.'.format(np=n_points, img=img_file))
                point_file_name = self.create_input_filename(n_points, img_file, 'dat')
                point_creator = ImagePointsCreator(n_points, img_file, point_file_name, input_dir_path)
                point_creator.create_points_pipeline(save_png=run_config['save_png_input'])

    def run_algorithm(self, n_cores, n_points, input_dat, input_png, algorithm, sub_size, n_iterations, dir_index,
                      sub_dir, iter_idx, img):
        """
        Function executing subprocess calls to .cpp executable forwarding parameter as
        command line inputs.
        """

        # step1: store run parameters of sub run in sub run folder
        sub_params = {'n_points': n_points, 'n_cores': n_cores, 'input_dat': input_dat, 'input_png': input_png,
                      'algorithm': algorithm, 'sub_size': sub_size, 'n_iterations': n_iterations, 'run_idx': dir_index,
                      'img': img}

        # ------------- Print parameters
        '''
        print '\n' + 30*'= '
        print '\tRun parameters:'
        for key, value in sub_params.iteritems():
            print '{:15}: {}'.format(key, value)
        print '{:15}: {}'.format('iter_idx', iter_idx)
        '''
        # -------------

        with open(join_paths(sub_dir, 'params.json'), 'w') as outfile:
            json.dump(sub_params, outfile, indent=4, sort_keys=True)

        # step2: run the algorithm specified by parameter grid inside of this folder
        exe_file = glob(join_paths(project_path, self.exe_dir_name, 'dphpc_project*'))
        assert len(exe_file) == 1, 'Could not find executable. Check you config file for correct exe_dir_name param!'

        input_file_path = join_paths(self.output_dir_path, 'input_data', input_dat)

        call_command = [exe_file[0], str(n_cores), input_file_path, algorithm, str(iter_idx)]
        call_command_str = ' '.join(call_command)

        subprocess.check_call(call_command_str, shell=True, cwd=sub_dir)

        # step3: postprocessing options
        # TODO: Postprocessing should be handled outside the GridRunHandler. Do not mix data acquisition and
        # TODO: post processing. But for now we leave it like that...

        # TODO: Implement postprocessing

    def main_grid_loop(self):
        """Main function looping over all possible parameter configurations executing the algorithms."""
        run_index = 0
        dir_index = 0
        for n_points in self.run_params['n_points']:
            for img_file in self.run_params['img_files']:

                input_file = self.create_input_filename(n_points, img_file, 'dat')

                for n_cores in self.run_params['n_cores']:
                    for algorithm in self.run_params['algorithms']:
                        for sub_size in self.run_params['sub_size']:

                            sub_dir = join_paths(self.output_dir_path, 'sub_' + str(dir_index))
                            os.mkdir(sub_dir)
                            dir_index += 1

                            n_iterations = self.run_params['n_iterations']
                            for iter_idx in range(n_iterations):

                                self.run_algorithm(n_cores=n_cores,
                                                   n_points=n_points,
                                                   input_dat=input_file,
                                                   input_png=input_file.split('.')[0] + '.png',
                                                   img=img_file,
                                                   algorithm=algorithm,
                                                   sub_size=sub_size,
                                                   n_iterations=n_iterations,
                                                   iter_idx=iter_idx,
                                                   dir_index=dir_index,
                                                   sub_dir=sub_dir)
                                run_index += 1

        self.store_all_sub_json_params()

    def store_all_sub_json_params(self):
        """Stores a single json file holding all the parameters in main run output folder."""
        all_params = {}
        all_sub_dir_paths = glob(join_paths(self.output_dir_path, 'sub_*', 'params.json'))

        for sub_idx, sub_dir in enumerate(sorted(all_sub_dir_paths)):
            with open(sub_dir, 'r') as infile:
                json_data = json.load(infile)
                all_params[sub_idx] = json_data

        with open(join_paths(self.output_dir_path, self.run_name + '_all_params.json'), 'w') as outfile:
            json.dump(all_params, outfile, indent=4)

    @staticmethod
    def create_run_name():
        dt = datetime.datetime.now()
        dt_appendix = '{:02d}{:02d}_{:02d}{:02d}{:02d}'.format(dt.month, dt.day, dt.hour, dt.minute, dt.second)
        return run_config['run_name'] + '_' + dt_appendix

    @staticmethod
    def create_input_filename(n_points, img_name, suffix):
        img_name_no_suffix = img_name.split('.')[0]
        return 'points_n_{np}_img_{img}.{suffix}'.format(np=n_points, img=img_name_no_suffix, suffix=suffix)