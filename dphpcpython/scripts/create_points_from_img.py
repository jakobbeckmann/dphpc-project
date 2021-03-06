"""
Creates a text file with points which represent extracted shapes from some input image.

Note:
    - the input image (png or jpg) has to be located in the "Input" folder of the project.
    - the output file containg the created points will be stored in the "Input" folder of the project as well.
"""


import matplotlib.pyplot as plt

from dphpcpython.libs.data_acquisition.ImagePointsCreator import ImagePointsCreator

# ------------ PARAMETER SECTION ----------- #

input_image         = 'elephant.png'
output_file         = 'bird_points.dat'
number_of_points    = 20000
show_points         = True
save_png            = False

# ------------------------------------------ #

creator = ImagePointsCreator(number_of_points, input_image, output_file)
image = creator.load_image(show_image=False)
processed_image = creator.create_dark_shape_mask(image)
points = creator.monte_carlo_sampling(processed_image)

if show_points:
    plt.scatter(points[:, 0], points[:, 1], alpha=0.7, edgecolors='none', s=3)
    plt.show()

creator.write_points_to_file(points, save_png=save_png)
