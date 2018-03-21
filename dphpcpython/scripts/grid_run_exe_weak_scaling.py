"""
Top level wrapper script to execute functionality from the GridRunHandler.
This file is being submitted as a batch job by some job.sh file.
"""

from dphpcpython.libs.data_acquisition.GridRunHandler import GridRunHandler

run = GridRunHandler()
run.create_input_files(weak_scaling=True)
run.main_grid_loop(weak_scaling=True)

