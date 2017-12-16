"""
Top level wrapper script to execute functionality from the GridRunHandler.
This file is being submitted as a batch job by some job.sh file.
"""

import argparse

from python.libs.data_acquisition.GridRunHandler import GridRunHandler

parser = argparse.ArgumentParser()
parser.add_argument('-c', '--config', default='run_config.py',
                    help='chose other config file than default run_config.py')
args = parser.parse_args()

run = GridRunHandler(custom_config=args.config)
run.create_input_files()
run.main_grid_loop()

