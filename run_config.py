#############################################################################
#                                                                           #
#   JSON configuration file to do parameter grid scans.                     #
#                                                                           #
#############################################################################

run_config = {
    'run_name':              'test',
    'exe_dir_name':          'cmake-build-debug',
    'save_png_input':        False,

    'run_params':
        {
            'n_cores':              [1, 2, 4, 6, 8],
            'n_points':             [300000],
            'img_files':            ['bird.jpg'],
            'algorithms':           ['chan_normal', 'chan_merge_var', 'graham', 'jarvis',
                                     'jarvis_jarvis', 'jarvis_graham',
                                     'jarvis_quickhull', 'graham_jarvis', 'graham_graham',
                                     'graham_quickhull', 'quickhull_binjarvis', 'quickhull_jarvis',
                                     'quickhull_graham', 'quickhull'],
            'sub_size':             [],
            'use_sub_sizes':        False,
            'n_iterations':         2,
        },

    'post_process_params':
        {
            'store_final_plots':    True,
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
