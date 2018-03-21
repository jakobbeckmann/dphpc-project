# dphpc-project
The goal of this project was to implement various parllel 2D convex hull solver algorithms and perform
benchmark tests among themselfs. For further information, check the **Report**/report pdf. 

## How-To
1. ```cd <project_root_folder>```
2. Do  ``` mkdir build```, compile using cmake and put the executable named **dphpc_project** into the build folder
3. Make sure your desired input image (.png or .jpg) is placed in the **Input** folder 
(used for point generation). A default "sample_image.png" is provided from the repo.
4. Set the desired parameters in run_config.py in the **Project root folder** folder.  
Important: the exe_dir_name has to match the folder name where you placed your dphpc_project executable!
5. Start the runs by running **dphpcpython/scripts/grid_run_exe.py** from your project root folder, you might specify a different run config
5. Run **benchmark_postprocessing.py** in **dphpcpython/scripts/** folder to create runtime and speedup plots.

### Parameters
- run_name: the run will appear in the **Output** folder as <run_name>_mmhh_ssssss
- exe_dir_name: the name of the folder containing the **dphpc_project** executable file
- save_png_input: whether to save the created input points as an img to look at
- n_cores: specify on what numbers of cores to run as a list, e.g. [1, 2, 4] runs all configurations on 1, 2 and 4 cores
- n_points: specify number of input points as a list, e.g. [5000, 10000, 100000]
- img_files: specify the name of images you want to run on as a list, files have to be in **Input** folder
- algorithms: specify names of algorithms to run, note all possible algorithms are given at the bottom
- sub_size: specify sub sizes on which you want to run the algorithms
- use_sub_sizes: whether you want to use this feature or not
- n_iterations: number of times to repeat a given configuration to gain statistics information

### Output
The output folder in **Output**/<run_name>_mmhh_ssssss will contain  
- input_data folder: holds the input images and data files for created input points
- sub_<run_id> folders: for every run configuration one sub folder holding hull points data, json param file and timing file
- a all_params.json file holding all configurations
- a config.json file holding the run config in json format

## Input Creation
In case you just want to create input data files from images without doing full parameter grid runs:  
From the **dphpcpython/scripts/** directory, run the **create_points_from_img.py** 
(specify parameters at top section of file) to create point distributions using images. 
The input image specified has to be located in the **Input** folder (.png or .jpg is fine)!  
The Input data will be stored in a .dat file in the **Input** folder.


## Visualization
Run **animation_wrapper.py** script in the **dphpcpython/scripts/** folder. 
First specify the desired parameters at the top section of the script. However, there is some tweeking needed, it might not work
since some output files might not have been written :), sorry...

