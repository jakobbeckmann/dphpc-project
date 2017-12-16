#!/bin/bash

#SBATCH -J time_vs_input_size_1_core
#SBATCH --output=Output/time_vs_input_size_1_core_%j.out
#SBATCH -C mc

# resources
#SBATCH --nodes=1
#SBATCH --cpus-per-task=72 --ntasks-per-core=72

#SBATCH --partition=normal
#SBATCH --time=12:00:00

#SBATCH --account=d83

python ./python/scripts/grid_run_exe.py
