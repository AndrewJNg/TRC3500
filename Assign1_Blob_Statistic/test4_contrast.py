import cv2
import numpy as np

def nothing(x):
    pass

# Load image
# img = cv2.imread("FYI_components_01.png")
# img = cv2.imread("FYI_components_03.png")
img = cv2.imread("FYI_components_06.png")

image = cv2.resize(img,(1280,720))

# Create a window
cv2.namedWindow('image')

# Create trackbar for contrast adjustment
cv2.createTrackbar('Contrast', 'image', 1, 20, nothing)  # Range from 1 (low) to 10 (high)

while True:
    # Get current contrast value from trackbar
    contrast = cv2.getTrackbarPos('Contrast', 'image')

    # Apply contrast adjustment using convertScaleAbs
    alpha = contrast / 5  # Adjust scaling factor as needed
    beta = 0  # No brightness adjustment
    adjusted_image = cv2.convertScaleAbs(image, alpha=alpha, beta=beta)

    # Display adjusted image
    cv2.imshow('image', adjusted_image)

    # Exit on 'q' key press
    if cv2.waitKey(10) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()