import numpy as np
import cv2

# read image
img = cv2.imread('IMG_20240227_0003.jpg')

# thresholding
blur = cv2.GaussianBlur(img, (5,5), 1)
hls = cv2.cvtColor(blur, cv2.COLOR_BGR2HLS)
low = np.array([0, 70, 0])
high = np.array([255, 255, 85])
thresh = cv2.inRange(hls, low, high)

# morphological operations to get the paper
kclose = np.ones((3,3), dtype=np.uint8)
kopen = np.ones((5,5), dtype=np.uint8)
closing = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kclose, iterations=2)
opening = cv2.morphologyEx(closing, cv2.MORPH_OPEN, kopen, iterations=6)

# corner detection
opening = cv2.GaussianBlur(opening, (3,3), 1)
opening = np.float32(opening)
dst = cv2.cornerHarris(opening, 2, 3, 0.04)

# drawing corners
dst = cv2.dilate(dst, None)
img[dst>0.01*dst.max()]=[0,0,255]

cv2.imshow('Corners', img)
cv2.waitKey(0)