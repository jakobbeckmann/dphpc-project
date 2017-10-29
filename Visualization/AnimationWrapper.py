"""
Wraps the single algorithm steps into a whole animation.
"""

import os
import matplotlib.pyplot as plt
from GrahamAnimation import GrahamAnimation
from DataLoader import DataLoader
import matplotlib.animation as animation


#####################
#                   #
# PARAMETER SECTION #
#                   #
#####################

input_data_path = 'C:\Users\mathee\CLionProjects\dphpc-project\cmake-build-debug'
output_data_path = os.path.dirname(os.path.dirname(os.path.realpath(__file__))) + '/cmake-build-debug'
save_movie = True
show_labels = False

#####################

loader = DataLoader(input_data_path)
data_dict = loader.load_all_data()
n_graham_subs = len(data_dict['graham_runs'])

fig = plt.figure(figsize=(15, 8))

animations = []

ax = fig.add_subplot(111)
graham_visualizer = GrahamAnimation(data_dict, label_points=show_labels)
graham_visualizer.set_ax(ax)
graham_visualizer.plot_all_points()
n_steps = max(graham_visualizer.n_steps_graham.values()) + graham_visualizer.n_steps_merge + 1

print "n steps", n_steps

animations.append(animation.FuncAnimation(fig,
                                          graham_visualizer.animate,
                                          frames=n_steps,
                                          interval=20,
                                          blit=False,
                                          init_func=graham_visualizer.init_animation,
                                          repeat=False))


Writer = animation.writers['ffmpeg']
writer = Writer(fps=24, metadata=dict(artist='Me'), bitrate=1800)

if save_movie:
    animations[-1].save(output_data_path + '/graham_animation.mp4', writer=writer)
else:
    plt.show()