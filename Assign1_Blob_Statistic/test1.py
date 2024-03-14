import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
 
def connect4(binary_grid,num_array,x,y,equilavent):
    # check if the box should have a number first
    num=0
    if(binary_grid[y][x]==0):
        directions = [[-1,0],[0,-1],[-1,-1],[1,-1],[-1,1],[1,1],[1,0],[0,1]] # check for top cell first, then left cell for number
        
        joint_bool = False
        for dx, dy in directions:
            x_new, y_new = x + dx, y + dy

            if 0 <= x_new < binary_grid.shape[1] and 0 <= y_new < binary_grid.shape[0]:
                if num_array[y_new][x_new] != 0:
                    joint_bool=True
                    if(num==0):
                        num = num_array[y_new][x_new]
                    elif(num!=num_array[y_new][x_new]):
                        if [num, num_array[y_new][x_new]] not in equilavent:
                            equilavent.append([num, num_array[y_new][x_new]])

            

        if joint_bool ==True:
            return num
        else:
            return np.max(np.max(num_array))+1
        
    # when box is block, no number should be entered
    else: 
        return 0 
            

    




# frame = cv.imread('DEMO_components_02.png', cv.IMREAD_GRAYSCALE)
frame = cv.imread('DEMO_circle_fish_star_01.jpg', cv.IMREAD_GRAYSCALE)
# print(frame)
# frame = np.array([[1, 1, 0, 1, 1, 1, 0, 1], 
#                   [1, 1, 0, 1, 0, 1, 0, 1], 
#                   [1, 1, 1, 1, 0, 0, 0, 1], 
#                   [0, 0, 0, 0, 0, 0, 0, 1], 
#                   [1, 1, 1, 1, 0, 1, 0, 1], 
#                   [0, 0, 0, 1, 0, 1, 0, 1], 
#                   [1, 1, 1, 1, 0, 0, 0, 1], 
#                   [1, 1, 1, 1, 0, 1, 1, 1],
#                   [1, 1, 0, 0, 0, 0, 0, 1]]
#                  )

# print(frame)


assert frame is not None, "file could not be read, check with os.path.exists()"
ret,binaryFrame = cv.threshold(frame,175,255,cv.THRESH_BINARY)
binaryFrame=binaryFrame/255
 
# cv.imshow("Frame", binaryFrame)
# cv.waitKey(0)


# binaryFrame = binaryFrame[0:360][0:360]
cv.imshow("Frame", binaryFrame)
cv.waitKey(0)

# binaryFrame = frame 

##########################################################################################################################################
num_array = np.zeros((binaryFrame.shape[0], binaryFrame.shape[1]))
equilavent=[]

# first pass
for y in range(binaryFrame.shape[0]):
    for x in range(binaryFrame.shape[1]):
        num_array[y][x] = connect4(binaryFrame,num_array,x=x,y=y,equilavent=equilavent)

# print(num_array)
# print(equilavent)


# second pass, replace temporary labels with equilavent labels
for i in equilavent:
    num_array=np.where(num_array == i[0], i[1], num_array)
    
# print(num_array)
# cv.imshow("Frame", num_array)
# cv.waitKey(0)

##########################################################################################################################################
labeled_image = num_array

classes = np.unique(labeled_image)
print(classes)

# Create a color map for different classes
num_classes = len(classes)
color_map = {label: plt.cm.tab10(i) for i, label in enumerate(classes)}

# Create an array of RGB colors based on the labeled image
colored_image = np.zeros((labeled_image.shape[0], labeled_image.shape[1], 3))
for i in range(labeled_image.shape[0]):
    for j in range(labeled_image.shape[1]):
        label = labeled_image[i, j]
        colored_image[i, j] = np.array(color_map[label][:3])  # Get the color for the label

# Plot the colored image
plt.imshow(colored_image)
plt.axis('off')  # Turn off axis
plt.show()







