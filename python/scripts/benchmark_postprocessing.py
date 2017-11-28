"""
This script can be used to produce benchmark plots depending on the
timing data provided in the Output folder.
For use, see Benchmark.py class.
"""

from python.libs.postprocessing.OutputLoader import OutputLoader
from python.libs.postprocessing.Benchmark import Benchmark

# ------------ PARAMETER SECTION ----------- #

run_name                   = 'test_1128_18231'
chose_latest_output_folder = True  # This way no need to change run_name all the time

# ------------------------------------------ #


data_loader = OutputLoader(run_name, latest_output=chose_latest_output_folder)
run_config = data_loader.get_run_config()
all_params = data_loader.get_sub_run_params()
all_data_dict = data_loader.get_all_data_dict()
data_loader.load_all_data_dict()


post_processor = Benchmark(run_config, all_params, all_data_dict)
post_processor.plot_runtimes_comparison(save=False, show=True, file_name='run_time_vs_input_all_algos')

# benchmark.plot_speedup_vs_cores(show=True, save=True)
# benchmark.plot_time_vs_cores(show=True, save=True)

