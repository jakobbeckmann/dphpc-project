"""
This script can be used to produce benchmark plots depending on the
timing data provided in the Output folder.
For use, see Benchmark.py class.
"""

from python.libs.postprocessing.Benchmark import Benchmark

benchmark = Benchmark()
benchmark.load_timing_data()
benchmark.plot_speedup_vs_cores(show=True, save=True)
benchmark.plot_time_vs_cores(show=True, save=True)

