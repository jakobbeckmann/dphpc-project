#############################################################################
#                                                                           #
#   JSON configuration file to do parameter grid scans.                     #
#                                                                           #
#############################################################################

{
    'run_name':              'final_time_vs_input_xps_15_low_iter',
    'exe_dir_name':          'cmake-build-debug',
    'save_png_input':        False,

    'run_params':
        {
            'n_cores':              [1],
            'n_points':             [1000000, 2000000, 3000000, 4000000, 5000000],
            'clusters':             1,
            'img_files':            ['elephant.png'],
            'algorithms':           ['jarvis', 'graham', 'quickhull'],
            'sub_size':             [],
            'use_sub_sizes':        False,
            'n_iterations':         10,
        },

    'post_process_params':
        {
            'store_final_plots':    False,
        }
}


#############################################################################
#                                                                           #
#   Note:                                                                   #
#                                                                           #
#   1) Input images must be placed in the "Input" folder (.png or .jpg)     #
#                                                                           #
#   2) Output is stored in the "Output" folder in a directory called        #
#      <run_name>_MMDD_hhmmss                                               #
#                                                                           #
#   3) Possible inputs for "algorithms" list:                               #
#      'chan_normal', 'chan_merge_var', 'jarvis', 'graham'. 'quickhull'     #
#                                                                           #
#############################################################################
