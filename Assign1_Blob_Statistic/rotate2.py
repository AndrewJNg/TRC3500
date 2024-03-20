import cv2
import numpy as np

def rotate_image(image, angle):
  """Rotates an image by a given angle with a white background.

  Args:
      image: The image to be rotated.
      angle: The angle of rotation in degrees.

  Returns:
      The rotated image with a white background.
  """
  (rows, cols, channels) = image.shape

  # Create a white background with the same dimensions as the image
  white_background = np.ones((rows, cols, channels), dtype="uint8")
  white_background[:] = 255

  # Get the center of rotation
  center = (cols // 2, rows // 2)

  # Create a rotation matrix
  rotation_matrix = cv2.getRotationMatrix2D(center, angle, 1.0)

  # Perform affine transformation
  rotated_image = cv2.warpAffine(image, rotation_matrix, (cols, rows), borderValue=(255, 255, 255))

  # Combine the rotated image with the white background
  final_image = white_background.copy()
  final_image[0:rows, 0:cols] = rotated_image

  return final_image

def display_angle(image, angle):
  """
  Displays the current angle on the bottom of the image.

  Args:
      image: The image to display the angle on (numpy array).
      angle: The current angle in degrees (float).
  """
  font = cv2.FONT_HERSHEY_SIMPLEX
  font_scale = 0.7
  font_thickness = 2
  text = f"Angle: {angle:.1f} degrees"
  # Get image dimensions for text placement
  (h, w) = image.shape[:2]
  text_size, _ = cv2.getTextSize(text, font, font_scale, font_thickness)
  # Calculate text bottom position (slightly above bottom edge)
  text_bottom = h - 10
  # Put the text on the image with a black background rectangle
  cv2.rectangle(image, (0, text_bottom - text_size[1] - 5), (w, text_bottom), (0, 0, 0), cv2.FILLED)
  cv2.putText(image, text, (10, text_bottom), font, font_scale, (255, 255, 255), font_thickness)



def main():
  # Load the image
  image = cv2.imread("resistor.jpg")
  current_angle = 0  # Initialize current angle

  # Check if image loaded successfully
  if image is None:
    print("Error: Could not load image")
    return

  # Infinite loop to display and rotate the image
  while True:
    # Display the image
    rotated_image = rotate_image(image.copy(), current_angle)
    display_angle(rotated_image, current_angle)
    cv2.imshow("Rotated Resistor", rotated_image)

    # Wait for a key press
    key = cv2.waitKey(0)
    

    # Handle button press for angle adjustment (replace with desired button code)
    if key == ord('a'):  # Change 'a' to your desired button press key
      current_angle -= 5  # Decrement angle by 5 degrees
      current_angle = current_angle % 360  # Wrap around if angle goes negative

    elif key == ord('d'):  # Change 'd' to your desired button press key
      current_angle += 5  # Increment angle by 5 degrees
      current_angle = current_angle % 360  # Wrap around if angle exceeds 360

    # Exit if 'q' is pressed
    if key == ord("q"):
      break

  # Close all windows
  cv2.destroyAllWindows()

if __name__ == "__main__":
  main()