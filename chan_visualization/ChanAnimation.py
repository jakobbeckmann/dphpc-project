import os
import numpy as np
# import matplotlib as mpl
import matplotlib.pyplot as plt
import itertools
import matplotlib.animation as animation

from DataLoader import DataLoader


class ChanAnimation:
    def __init__(self, data_dict):
        """
            - state represents the 3 indices of base_idx, last_idx, last_idx - 1, used to draw the arrows
            - hull_points is the stack of points that lie on the hull (at a given moment)
        """

        self.all_points = data_dict['all_sorted']
        self.hull_points = data_dict['hull_points']
        # TODO: For multiple graham runs
        self.graham_run = data_dict['graham_runs'][0]

        self.current_hull_points = [0]
        self.state = [0, 0, 0]
        self.current_orientation = None
        self.color_orientation = {'clockwise': 'black', 'anticlockwise': 'green'}
        self.step = 0

    def update_state(self, step):

        self.state = self.graham_run[step][:3]
        for idx, item in enumerate(self.state):
            if item < 0:
                self.state[idx] = 0

        """
        added = self.graham_run[step][3]
        if added != -1:
            self.current_hull_points.append(added)
            print 'added ', added

        removed = self.graham_run[step][4]
        if removed != -1:
            print 'removed ', removed
            self.current_hull_points.remove(removed)
        """
        orientation = self.graham_run[step][5]
        if orientation == -1:
            self.current_orientation = 'clockwise'
        elif orientation == 1:
            self.current_orientation = 'anticlockwise'

    def get_two_lines_pos(self):
        x = [self.all_points['x'][idx] for idx in self.state]
        y = [self.all_points['y'][idx] for idx in self.state]
        return x, y

    def get_hull_lines_pos(self):
        x = [self.all_points['x'][idx] for idx in self.current_hull_points]
        y = [self.all_points['y'][idx] for idx in self.current_hull_points]
        return x, y

    def plot_all_points(self, axes):
        # TODO: animation of quick sort, means adding points one after the other
        axes.scatter(self.all_points['x'], self.all_points['y'], s=300, c='red', alpha=0.8, edgecolors='none')
        axes.scatter(self.hull_points['x'], self.hull_points['y'], s=300, c='magenta', alpha=0.8, edgecolors='none')
        axes.plot(self.hull_points['x'], self.hull_points['y'], c='magenta', alpha=0.8, linewidth=0.4)
        axes.plot([self.hull_points['x'][0], self.hull_points['x'][-1]],
                  [self.hull_points['y'][0], self.hull_points['y'][-1]], c='magenta', alpha=0.8, linewidth=0.4)

        for idx in range(len(self.all_points['x'])):
            axes.text(self.all_points['x'][idx]-0.2, self.all_points['y'][idx]-0.2, str(idx))

        axes.axis('off')

# --------------------------------
# Initializing DataLoader and GrahamVisualizer
loader = DataLoader('C:\Users\mathee\CLionProjects\dphpc-project\cmake-build-debug')
all_data = loader.load_all_data()
visualizer = ChanAnimation(all_data)


# --------------------------------
# Initializing matplotlib stuff and dummy line objects
fig = plt.figure(figsize=(15, 9))
ax = fig.add_subplot(111)
visualizer.plot_all_points(axes=ax)

steps_text = ax.text(0.02, 0.95, '', transform=ax.transAxes)
two_lines, = ax.plot([], [], alpha=0.8, linewidth=3)  # switching red / cyan
hull_lines, = ax.plot([], [], c='orange', alpha=0.8, linewidth=3)


def init():
    pass
    two_lines.set_data([], [])
    hull_lines.set_data([], [])
    return two_lines, hull_lines


def animate(step):
    global visualizer
    print 'step ', 0
    print 'state ', visualizer.state

    visualizer.update_state(step)
    two_lines.set_data(*visualizer.get_two_lines_pos())
    two_lines.set_color(visualizer.color_orientation[visualizer.current_orientation])
    hull_lines.set_data(*visualizer.get_hull_lines_pos())
    return two_lines, hull_lines


ani = animation.FuncAnimation(fig, animate, frames=300,
                              interval=500, blit=True, init_func=init)

#ani.save('animation.mp4', fps=30)
plt.show()