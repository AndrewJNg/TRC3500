import cv2
import numpy as np

def rotate_image(image, angle):
  """
  Rotates an image by a specified angle.

  Args:
      image: The image to be rotated (numpy array).
      angle: The rotation angle in degrees (float).

  Returns:
      The rotated image (numpy array).
  """
  (h, w) = image.shape[:2]
  center = (w // 2, h // 2)
  # Create rotation matrix
  M = cv2.getRotationMatrix2D(center, angle, 1.0)
  # Apply rotation to the image
  return cv2.warpAffine(image, M, (w, h))

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
  # Load the image of the horizontal resistor
  image = cv2.imread("resistor.jpg")
  current_angle = 0  # Initialize current angle

  while True:
    # Rotate and display the image
    rotated_image = rotate_image(image.copy(), current_angle)
    display_angle(rotated_image, current_angle)
    cv2.imshow("Rotated Resistor", rotated_image)

    # Get keyboard input
    key = cv2.waitKey(1) & 0xFF

    # Handle button press for angle adjustment (replace with desired button code)
    if key == ord('a'):  # Change 'a' to your desired button press key
      current_angle -= 5  # Decrement angle by 5 degrees
      current_angle = current_angle % 360  # Wrap around if angle goes negative

    elif key == ord('d'):  # Change 'd' to your desired button press key
      current_angle += 5  # Increment angle by 5 degrees
      current_angle = current_angle % 360  # Wrap around if angle exceeds 360

    # Exit on 'q' press
    elif key == ord('q'):
      break

  cv2.destroyAllWindows()

if __name__ == "__main__":
  main()