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
            'n_cores':              [8],
            'n_points':             [60000],
            'clusters':             4,
            'img_files':            [u'square4.PNG'],
            'algorithms':           ['chan_normal'],
            'sub_size':             [],
            'use_sub_sizes':        False,
            'n_iterations':         1000,
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
