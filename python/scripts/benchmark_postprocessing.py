"""
This script can be used to produce benchmark plots using the parameters and output of a whole grid run.
"""

from python.libs.postprocessing.OutputLoader import OutputLoader
from python.libs.postprocessing.Benchmark import Benchmark

# ------------ PARAMETER SECTION ----------- #

run_name                   = 'test_1128_18231'
chose_latest_output_folder = True            # Overwrites run_name with latest run in the Output folder
save_all_data_dict         = True

do_runtime_plot            = False
do_speedup_plot            = True

# ------------------------------------------ #


# --------------- DATA LOADING ------------- #
data_loader     = OutputLoader(run_name, latest_output=chose_latest_output_folder, save_all_data=save_all_data_dict)
run_config      = data_loader.get_run_config()
all_params      = data_loader.get_sub_run_params()
all_data_dict   = data_loader.get_all_data_dict()
data_loader.load_all_data_dict()
# ------------------------------------------ #


# -------------- POST PROCESSING ----------- #
post_processor = Benchmark(run_config, all_params, all_data_dict)

if do_runtime_plot:
    post_processor.plot_runtimes_comparison(save=False, show=True, file_name='run_time_vs_input_all_algos')
if do_speedup_plot:
    post_processor.plot_speedup_vs_cores(save=False, show=True, file_name='speedup_vs_ncores_all_algos')
# ------------------------------------------ #

