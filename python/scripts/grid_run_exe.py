"""
Top level wrapper script to execute functionality from the GridRunHandler.
This file is being submitted as a batch job by some job.sh file.
"""

from python.libs.execution.GridRunHandler import GridRunHandler

run = GridRunHandler()
run.create_input_files()
run.main_grid_loop()

