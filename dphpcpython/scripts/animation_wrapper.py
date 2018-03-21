#! /usr/bin/env python3

"""
Wraps the single algorithm steps into a whole animation.
"""

import os
from os.path import join as join_paths
import matplotlib.animation as animation
import matplotlib.pyplot as plt

from dphpcpython.libs.postprocessing.AnimDataLoader import AnimDataLoader
from dphpcpython.libs.postprocessing.ChanAnimation import ChanAnimation

# ----- SPECIFY PARAMETERS ----- #

save_movie      = False
show_labels     = False
interval        = 10  # controls the play-back speed
data_dir        = 'test_run_0321_171123/sub_0'
output_name     = 'animation.mp4'
original_image  = 'sample_image.png'  # set to None to not display any image

# ------------------------------ #

input_data_path = join_paths(os.path.dirname(os.path.dirname(os.path.realpath(__file__))) + '/Output', data_dir)
output_data_path = os.path.dirname(os.path.dirname(os.path.realpath(__file__))) + '/FiguresAndMovies'

loader = AnimDataLoader(input_data_path)
data_dict = loader.load_all_data()
n_graham_subs = len(data_dict['graham_runs'])

fig = plt.figure(figsize=(15, 9))

animations = []

ax = fig.add_subplot(111)
graham_visualizer = ChanAnimation(data_dict, label_points=show_labels, original_image=original_image)
graham_visualizer.set_ax(ax)
graham_visualizer.plot_all_points()
n_steps = max(graham_visualizer.n_steps_graham.values()) + graham_visualizer.n_steps_merge + 1
print("Total steps to process will be: " + str(n_steps))

animations.append(animation.FuncAnimation(fig,
                                          graham_visualizer.animate,
                                          frames=n_steps,
                                          interval=interval,
                                          blit=False,
                                          init_func=graham_visualizer.init_animation,
                                          repeat=False))


Writer = animation.writers['ffmpeg']
writer = Writer(fps=24, metadata=dict(artist='Me'), bitrate=1800)

if save_movie:
    animations[-1].save(output_data_path + '/' + output_name, writer=writer)
else:
    plt.show()
