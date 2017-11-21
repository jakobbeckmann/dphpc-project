"""
Module to create various kinds of input point distributions. Output files are being created in "Output" folder
in the project directory.
"""


import numpy as np
import os
import random
import cv2


class ImagePointsCreator:
    def __init__(self, n_points, image_file, output_file):
        self.n_points = n_points
        self.image_file = image_file
        self.exe_file = os.path.realpath(__file__)
        self.output_file_path = self.check_output_file(output_file, overwrite_file=True)
        self.image_shape = {}

    def load_image(self, show_image=False):
        """Loads the provided image into numpy array."""
        image = cv2.imread(os.path.join(os.path.dirname(self.exe_file), '..', 'Input', self.image_file))
        if show_image:
            cv2.imshow('image', image)
            cv2.waitKey(0)
            cv2.destroyAllWindows()
        y_pixels, x_pixels, n_channels = image.shape
        self.image_shape = {'y': y_pixels, 'x': x_pixels}
        print ("Loaded image {name} with dimensions: ({y_pixels}, {x_pixels}) pixels."
               .format(name=self.image_file.split('/')[-1], y_pixels=y_pixels, x_pixels=x_pixels))
        return image

    @staticmethod
    def create_dark_shape_mask(image):
        lower = np.array([0, 0, 0])
        upper = np.array([10, 10, 10])

        shape_mask = cv2.inRange(image, lower, upper)
        return shape_mask

    @staticmethod
    def extract_largest_patch(image):
        """Extracts the largest cohesive patch in the image."""
        # TODO: implement.
        processed_image = image
        return processed_image

    def monte_carlo_sampling(self, processed_image):
        """Takes the pre-processed image and randomly samples self.n_points into the region of interest."""
        points = []
        for _ in range(self.n_points):
            x = random.randint(0, self.image_shape['x'] - 1)
            y = random.randint(0, self.image_shape['y'] - 1)
            if processed_image[y, x] != 0:
                x_noise = random.uniform(-1, 1)
                y_noise = random.uniform(-1, 1)
                points.append([x + x_noise, (self.image_shape['y'] - y) + y_noise])
        return np.array(points)

    def check_output_file(self, output_file, overwrite_file=False):
        """Checks whether output file is .csv and doesn't overwrite existing file."""
        output_file_path = os.path.join(os.path.dirname(self.exe_file), '..', 'Input', output_file)
        if not overwrite_file:
            if os.path.isfile(output_file_path):
                raise IOError('Output file {file_path} already exists. Abort.'.format(file_path=output_file_path))
        return output_file_path

    def write_points_to_file(self, points):
        np.savetxt(self.output_file_path, points, '%5.2f')
        print("Written output to " + self.output_file_path + '\n')

    def create_points_pipeline(self):
        """Pipeline to create point distribution from a single image."""
        image = self.load_image()
        processed_image = self.extract_largest_patch(image)
        points = self.monte_carlo_sampling(processed_image)
        self.write_points_to_file(points)
