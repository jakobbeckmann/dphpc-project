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

        self.current_hull_points = {'x': [], 'y': []}  # indices

        self.idx_stack = [len(self.all_points['x']) - 1, 0, 1]

        self.state = []          # base, last, last-1 indices
        self.current_orientation = None
        self.color_orientation = {'clockwise': 'red', 'anticlockwise': 'green'}
        self.step = 0
        self.n_steps = len(self.graham_run)

    def update_stack_state(self, step):

        add_rem_flag = self.graham_run[step][1]

        if add_rem_flag == 1:
            self.idx_stack.append(self.graham_run[step][0])
        elif add_rem_flag == -1:
            del self.idx_stack[-2]

    def update_state(self, step):

        self.state = self.graham_run[step][:6]
        for idx, item in enumerate(self.state):
            if item < 0:
                self.state[idx] = 0

        add_point = self.graham_run[step][6]

        add_rem_x = self.graham_run[step][7]
        add_rem_y = self.graham_run[step][8]

        print 'add point', add_point
        if add_point:
            self.current_hull_points['x'].append(add_rem_x)
            self.current_hull_points['y'].append(add_rem_y)
            print 'add'
        else:
            print 'remove', add_rem_x, add_rem_y
            self.current_hull_points['x'].remove(add_rem_x)
            self.current_hull_points['y'].remove(add_rem_y)

        orientation = self.graham_run[step][9]
        if orientation == -1:
            self.current_orientation = 'clockwise'
        elif orientation == 1:
            self.current_orientation = 'anticlockwise'
        print self.current_orientation

    def get_two_lines_pos(self):
        # x = [self.all_points['x'][idx] for idx in self.idx_stack[-3:]]
        # y = [self.all_points['y'][idx] for idx in self.idx_stack[-3:]]
        x = self.state[0: 3]
        y = self.state[3: 6]
        return x, y

    def get_hull_lines_pos(self):
        # x = [self.all_points['x'][idx] for idx in self.idx_stack]
        # y = [self.all_points['y'][idx] for idx in self.idx_stack]
        x = self.current_hull_points['x']
        y = self.current_hull_points['y']
        return x, y

    def plot_all_points(self, axes):
        # TODO: animation of quick sort, means adding points one after the other
        axes.scatter(self.all_points['x'], self.all_points['y'], s=20, c='red', alpha=0.8, edgecolors='none')
        axes.scatter(self.hull_points['x'], self.hull_points['y'], s=20, c='green', alpha=0.8, edgecolors='none')
        axes.plot(self.hull_points['x'], self.hull_points['y'], c='green', alpha=0.8, linewidth=0.4)
        axes.plot([self.hull_points['x'][0], self.hull_points['x'][-1]],
                  [self.hull_points['y'][0], self.hull_points['y'][-1]], c='green', alpha=0.8, linewidth=0.4)

        #for idx in range(len(self.all_points['x'])):
        #    axes.text(self.all_points['x'][idx], self.all_points['y'][idx], str(idx))

        axes.axis('off')

# --------------------------------
# Initializing DataLoader and GrahamVisualizer
loader = DataLoader('C:\Users\mathee\CLionProjects\dphpc-project\cmake-build-debug')
#loader = DataLoader('C:\Users\mathee\Desktop\Master\Design of Parallel and High-Performance Computing HS16\Project\RustOutputFiles')
all_data = loader.load_all_data()
visualizer = ChanAnimation(all_data)


# --------------------------------
# Initializing matplotlib stuff and dummy line objects
fig = plt.figure(figsize=(15, 8))
ax = fig.add_subplot(111)
visualizer.plot_all_points(axes=ax)

# steps_text = ax.text(0.02, 0.95, '', transform=ax.transAxes)
two_lines, = ax.plot([], [], c='cyan', alpha=0.8, linewidth=3)  # switching red / cyan
hull_lines, = ax.plot([], [], c='cyan', alpha=0.8, linewidth=3)

'''If you just want to see the final result without animation.'''
# plt.show()


def init():
    two_lines.set_data([], [])
    hull_lines.set_data([], [])
    return two_lines, hull_lines


def animate(step):
    global visualizer
    visualizer.update_state(step)
    two_lines.set_data(*visualizer.get_two_lines_pos())
    two_lines.set_color(visualizer.color_orientation[visualizer.current_orientation])
    hull_lines.set_data(*visualizer.get_hull_lines_pos())
    return two_lines, hull_lines


ani = animation.FuncAnimation(fig, animate, frames=visualizer.n_steps,
                              interval=10, blit=True, init_func=init, repeat=False)

'''Save animation: Needs FFmpeg.'''
# ani.save('animation.mp4', fps=30)

plt.show()