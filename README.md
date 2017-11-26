# dphpc-project
Group work for the Design of Parallel and High Performance Computing course 2017  

## How-To
A tipical usage could look like this:  
- Place a .png or .jpg file in the Input folder.
- Run the input creation script (see below).
- In main.cpp specify input data and nCores array.  
- Compile using cmake and run dphpc_project.exe.
- Run the visualization script (see below).
- Run the benchmark script (see below).

## Input Creation
From the **InputCreation**, run the **create_points_from_img.py** (specify parameters at top section of file) to create point distributions using images. The input image specified has to be located in the **Input** folder (.png or .jpg is fine)!  
The Input data will be stored in a .dat file in the **Input** folder.

## Benchmark plots
From the **Benchmarking** folder, run the **benchmark_postprocessing.py** script to do speedup plots vs number of cores. Data is provided with the **Output/timing.txt** which is being created during the algorithm runs.  
Usage should be self-explanatory.

## Visualization
Run **AnimationWrapper.py** script in the **Visualization** folder. First specify the desired parameters at the top section of the script.

