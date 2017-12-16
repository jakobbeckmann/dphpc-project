#############################################################################
#                                                                           #
#   JSON configuration file to do parameter grid scans.                     #
#                                                                           #
#############################################################################

run_config = {
    'run_name':              'final_strong_scaling_daint',
    'exe_dir_name':          'exe',
    'save_png_input':        False,

    'run_params':
        {
            'n_cores':              [1, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72],
            'n_points':             [5000000],
            'clusters':             1,
            'img_files':            ['elephant.png'],
            'algorithms':           ['chan_normal', 'chan_merge_var', 'graham', 'jarvis',
                                     'jarvis_jarvis', 'jarvis_graham', 'jarvis_binjarvis'
                                     'jarvis_quickhull', 'graham_jarvis', 'graham_graham',
                                     'graham_quickhull', 'quickhull_binjarvis', 'quickhull_jarvis',
                                     'quickhull_graham', 'quickhull', 'quickhull_quickhull'],
            'sub_size':             [],
            'use_sub_sizes':        False,
            'n_iterations':         100,
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
