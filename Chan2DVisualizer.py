import numpy as np
import matplotlib.pyplot as plt


class Chan2DVisualizer:

    def __init__(self, chan_data_file):
        self.chan_data = self.__class__.load_chan_output_file(chan_data_file)

    def main(self):
        pass

    @staticmethod
    def load_chan_output_file(chan_data_file):
        return np.loadtxt(chan_data_file)

    def start_animation(self):
        pass

if __name__ == '__main__':
    chan_output_data_file = 'some_path'

    chan_visualizer = Chan2DVisualizer(chan_data_file=chan_output_data_file)
    chan_visualizer.main()