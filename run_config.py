#############################################################################
#                                                                           #
#   JSON configuration file to do parameter grid scans.                     #
#                                                                           #
#############################################################################

run_config = {
    'run_name':         'test',
    'exe_dir_name':     '',
    'save_png_input':   True,

    'run_params':
        {
            'n_cores':              [2],
            'n_points':             [50000],
            'img_files':            ['homer.jpg'],
            'algorithms':           ['chan_normal', 'chan_merge_var', 'graham', 'jarvis',
                                     'jarvis_jarvis', 'jarvis_binjarvis', 'jarvis_graham',
                                     'jarvis_quickhull', 'graham_jarvis', 'graham_graham',
                                     'graham_quickhull', 'quickhull_binjarvis', 'quickhull_jarvis',
                                     'quickhull_graham', 'quickhull_quickhull', 'quickhull'],
            'sub_size':             [None],
            'n_iterations':         5,
        },

    'post_process_params':
        {
            'store_final_plots':    False,
            'store_movies':         False,
            'store_bm_plots':       False
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
