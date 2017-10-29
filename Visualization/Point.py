"""
For the animation, the data set will hold a list of points, those points represent either a left
or right hand side turn which is then represented by their color.
They also belong to a certain graham subset.
"""


class Point:
    def __init__(self, x, y, turn=None):
        self.x = x
        self.y = y
        self.turn = turn
        self.graham_idx = None
        self.color = None

    def set_color(self, color):
        self.color = color

    def set_turn(self, turn):
        assert turn in ['left', 'right']
        self.turn = turn

    def set_xy(self, x, y):
        self.x = x
        self.y = y

    def set_graham_idx(self, idx):
        assert idx >= 0
        self.graham_idx = idx
