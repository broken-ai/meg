"""

This file generates and empty map with clouds and sky on the 0-3 layers
See: SkyMap.tcm

"""
import os
import random


width = 100
height = 50
n_sm_cloud = 100
n_med_cloud = 100
n_big_cloud = 50

sky = 57
big_cloud = [
    [34, 35, 36],
    [44, 45, 46]
]
med_cloud_1 = [[24, 25]]
med_cloud_2 = [
    [26, 27],
    [0, 37]
]
sm_cloud = [[47]]
layer_str_first = '''
{
    [
'''
layer_str_last = '''		
    ]
	width = %i
	height = %i
	xOffset = 0
	yOffset = 0
	xSpeed = %.2f
	ySpeed = %.2f
	xSpeedAuto = 0
	ySpeedAuto = 0
}
'''
file_str = '''tileset_file = test.tct
'''


def place_matrix(to_matrix, from_matrix):
    """
    Place the content of the from_matrix to the to_matrix tile
    """
    # Get the initial position
    x_pos = round(random.random() * (len(to_matrix[0]) - len(from_matrix[0])))
    y_pos = round(random.random() * (len(to_matrix) - len(from_matrix)))
   
    for i, row in enumerate(from_matrix):
        for j, col in enumerate(row):
            to_matrix[y_pos + i][x_pos + j] = col

    return to_matrix


layers = [  
    [[sky for j in range(width)] for i in range(height)],  # Fill the first layer with sky
    [[0 for j in range(width)] for i in range(height)],
    [[0 for j in range(width)] for i in range(height)],
    [[0 for j in range(width)] for i in range(height)]
] 


for _ in range(n_sm_cloud):
    place_matrix(layers[1], sm_cloud)
for _ in range(n_med_cloud // 2):
    place_matrix(layers[2], med_cloud_1)
    place_matrix(layers[2], med_cloud_2)
for _ in range(n_big_cloud):
    place_matrix(layers[3], big_cloud)
    

# Adding empty layers
for i in range(4):
    layers.append([[0 for j in range(width)] for i in range(height)])


# Creating the file_str
for i, layer in enumerate(layers):
    file_str += layer_str_first
    
    for row in layer:
         file_str += '\t' + str(row)[1:][:-1] + '\n'
    
    file_str += layer_str_last % (width, height, i * .25, i * .25)


# Writing to file
with open('SkyMap.tcm', 'w+') as file:
    file.write(file_str)

print()
print('Map generated with the name: ' + os.path.join(os.getcwd(), 'SkyMap.tcm'))
print()
