import cv2

def detect_ean13_guard_bars(image):
  """
  Detects the guard bars in an EAN-13 barcode image.

  Args:
      image (numpy.ndarray): The grayscale image of the barcode.

  Returns:
      tuple (int, int): The starting and ending indices of the guard bars (or None if not found).
  """

  # Preprocessing
  grayscale_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
  thresh, binary_image = cv2.threshold(grayscale_image, 127, 255, cv2.THRESH_BINARY)

  # Horizontal projection
  horizontal_projection =  cv2.reduce(binary_image, 0, cv2.REDUCE_SUM, dtype=cv2.CV_32S)
 

  # Peak detection (sliding window)
  window_size = 5  # Adjust based on barcode width and expected guard bar width
  max_values = []
  for i in range(len(horizontal_projection) - window_size + 1):
    window = horizontal_projection[i:i+window_size]
    max_value = max(window)
    max_values.append(max_value)

  # Threshold for guard bar height (adjust based on image quality)
  guard_bar_threshold = 0.8 * sum(max_values) / len(max_values)

  # Find potential guard bar peaks
  potential_peaks = []
  for i, value in enumerate(max_values):
    if value > guard_bar_threshold:
      potential_peaks.append(i)

  # Validate guard bar pattern (single peak at beginning and end)
  if len(potential_peaks) != 2 or potential_peaks[0] != 0 or potential_peaks[-1] != len(max_values) - 1:
    return None

  # Return guard bar indices
  start_index = potential_peaks[0]
  end_index = potential_peaks[-1] + window_size - 1  # Adjust for window size
  return start_index, end_index

# Example usage (replace 'path/to/image.jpg' with your image path)
image = cv2.imread('9319133331372.png')
start_index, end_index = detect_ean13_guard_bars(image)

if start_index is not None and end_index is not None:
  print(f"Guard bars detected between indices: {start_index} - {end_index}")
else:
  print("Guard bars not found in the image.")