# dphpc-project
Group work for the Design of Parallel and High Performance Computing course 2017  

## How-To
A tipical usage could look like this:  
- Compile your dphc-project executable
1. Place .png or .jpg files you want to use for point generation into the **Input** folder
2. Set the desired parameters in run_config.py in the **Execution** folder.
3. Run the grid_run_exe.py python script from the **Execution** folder.
4. The output will be created in the **Output** folder in a directory formatted to <run_name_from_run_config>_mmdd_hhmmss.

## Input Creation
In case you just want to create input data files from images without doing full grid runs:  
From the **InputCreation** directory, run the **create_points_from_img.py** 
(specify parameters at top section of file) to create point distributions using images. 
The input image specified has to be located in the **Input** folder (.png or .jpg is fine)!  
The Input data will be stored in a .dat file in the **Input** folder.

## Benchmark plots
--> Will be merged into the grid runs soon... Have to work on generalizing functionality.


## Visualization
Run **AnimationWrapper.py** script in the **Visualization** folder. 
First specify the desired parameters at the top section of the script.

