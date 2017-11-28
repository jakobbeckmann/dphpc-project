#############################################################################
#                                                                           #
#   JSON configuration file to do parameter grid scans.                     #
#                                                                           #
#############################################################################

run_config = {
    'run_name': 'test',

    'run_params':
        {
            'n_cores':              [1],
            'n_points':             [200],
            'img_files':            ['homer.jpg'],
            'algorithms':           ['chan_normal', 'quickhull'],
            'sub_size':             [None],
            'n_iterations':         1,
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
#      'chan_normal', 'chan_merge_var', 'jarvis', 'graham'                  #
#                                                                           #
#############################################################################

