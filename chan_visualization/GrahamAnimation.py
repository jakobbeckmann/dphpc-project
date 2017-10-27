"""
Class which defines the functionality to plot a graham scan.

Need to pass a data_dict to the constructor which we get using the DataLoader class.
"""


class GrahamAnimation:
    def __init__(self, data_dict, graham_sub_idx):
        """
            - state represents the 3 indices of base_idx, last_idx, last_idx - 1, used to draw the arrows
            - hull_points is the stack of points that lie on the hull (at a given moment)
        """

        self.all_points = data_dict['sub_sorted'][graham_sub_idx]
        self.hull_points = data_dict['hull_points'][graham_sub_idx]
        self.graham_run = data_dict['graham_runs'][graham_sub_idx]

        self.current_hull_points = {'x': [self.all_points['x'][idx] for idx in [-1, 0]],  # include zero and last point
                                    'y': [self.all_points['y'][idx] for idx in [-1, 0]]}

        self.idx_stack = [len(self.all_points['x']) - 1, 0, 1]

        self.state = []          # base, last, last-1 indices
        self.current_orientation = None
        self.two_lines_color = {'clockwise': 'red', 'anticlockwise': 'green'}
        self.step = 0
        self.n_steps = len(self.graham_run)

        self.ax = None
        self.two_lines = None
        self.hull_lines = None

    # not used at the moment
    def update_stack_state(self, step):

        add_rem_flag = self.graham_run[step][1]

        if add_rem_flag == 1:
            self.idx_stack.append(self.graham_run[step][0])
        elif add_rem_flag == -1:
            del self.idx_stack[-2]

    def update_graham_state(self, step):

        self.state = self.graham_run[step][:6]
        for idx, item in enumerate(self.state):
            if item < 0:
                self.state[idx] = 0

        add_point = self.graham_run[step][6]

        add_rem_x = self.graham_run[step][7]
        add_rem_y = self.graham_run[step][8]

        if add_point:
            self.current_hull_points['x'].append(add_rem_x)
            self.current_hull_points['y'].append(add_rem_y)
        else:
            self.current_hull_points['x'].remove(add_rem_x)
            self.current_hull_points['y'].remove(add_rem_y)

        orientation = self.graham_run[step][9]
        if orientation == -1:
            self.current_orientation = 'clockwise'
        elif orientation == 1:
            self.current_orientation = 'anticlockwise'

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

    def plot_all_points(self):
        # TODO: animation of quick sort, means adding points one after the other
        hull_color = 'blue'
        points_color = 'black'

        self.ax.scatter(self.all_points['x'], self.all_points['y'], s=20, c=points_color, alpha=0.8, edgecolors='none')
        self.ax.scatter(self.hull_points['x'], self.hull_points['y'], s=20, c=hull_color, alpha=0.8, edgecolors='none')

        self.ax.plot(self.hull_points['x'], self.hull_points['y'], c=hull_color, alpha=0.8, linewidth=0.4)
        self.ax.plot([self.hull_points['x'][0], self.hull_points['x'][-1]],
                  [self.hull_points['y'][0], self.hull_points['y'][-1]], c=hull_color, alpha=0.8, linewidth=0.4)

        ''' Outcomment to label points '''
        # for idx in range(len(self.all_points['x'])):
        #    self.ax.text(self.all_points['x'][idx], self.all_points['y'][idx], str(idx))

        self.ax.axis('off')

    def set_ax(self, ax):
        self.ax = ax

    def init_animation(self):

        self.two_lines, = self.ax.plot([], [], c='green', alpha=0.8, linewidth=3)  # switching red / cyan
        self.hull_lines, = self.ax.plot([], [], c='green', alpha=0.8, linewidth=3)
        
        return self.two_lines, self.hull_lines

    def animate(self, step):
        if step != 0:
            self.update_graham_state(step)
            self.two_lines.set_data(*self.get_two_lines_pos())
            self.two_lines.set_color(self.two_lines_color[self.current_orientation])
            self.hull_lines.set_data(*self.get_hull_lines_pos())
        
        return self.two_lines, self.hull_lines

