"""
This file exists to import paths conveniently from other modules.
"""

import os

this_file = os.path.abspath(__file__)
this_dir = os.path.dirname(this_file)

project_path = os.path.dirname(os.path.dirname(this_dir))
