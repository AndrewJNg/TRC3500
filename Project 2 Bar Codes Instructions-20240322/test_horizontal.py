import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
import math
 
def EAN_13_decode(inputCode):
  table = {
      '0001101': 0,
      '0011001': 1,
      '0010011': 2,
      '0111101': 3,
      '0100011': 4,
      '0110001': 5,
      '0101111': 6,
      '0111011': 7,
      '0110111': 8,
      '0001011': 9,
      
      '0100111': 0,
      '0110011': 1,
      '0011011': 2,
      '0100001': 3,
      '0011101': 4,
      '0111001': 5,
      '0000101': 6,
      '0010001': 7,
      '0001001': 8,
      '0010111': 9,
      
      '1110010': 0,
      '1100110': 1,
      '1101100': 2,
      '1000010': 3,
      '1011100': 4,
      '1001110': 5,
      '1010000': 6,
      '1000100': 7,
      '1001000': 8,
      '1110100': 9
  }
  inputCode = str(inputCode)
  return table[inputCode]

def inkSpreading(T1_val):
    mod = 0
    if(T1_val <= 1.5/7):
        mod = 1
    elif(T1_val >= 1.5/7 and T1_val < 2.5/7 ):
        mod = 2
    elif(T1_val >= 2.5/7 and T1_val < 3.5/7 ):
        mod = 3
    elif(T1_val >= 3.5/7 and T1_val < 4.8/7 ):
        mod = 4
    elif(T1_val >= 4.8/7  ):
        mod = 5


    return mod

def ink_decoder(T1,T2,T4, T):
   table = [[['e6','e6','e6'], ['o0','o0','o0'] , ['e4','e4','e4']  , ['o3','o3','o3']],
            [['o9','o9','o9'], ['','e2','e8']   , ['o7','o1','']    , ['e5','e5','e5']],
            [['e9','e9','e9'], ['o8','o2','']   , ['e1','e7','']    , ['o5','o5','o5']],
            [['o6','o6','o6'], ['e0','e0','e0'] , ['o4','o4','o4']  , ['e3','e3','e3']]
            ]
#    print(T1)
#    print(T2)
#    print(T4)
#    print(table[T1-2][T2-2])
   
   T1_bar = inkSpreading(T1/T)
   T2_bar = inkSpreading(T2/T)
   T4_bar = inkSpreading(T4/T)
   if(T2==2 and T4==1 and T>(T2+T4)):
        return "guard bit"
   if T4_bar>3:
      T4_bar=3

   return table[T1_bar-2][T2_bar-2][T4_bar-1]

def analyse(consecutive_counts,i,dir_left):

    if(dir_left==True):
        T4 = consecutive_counts[i]
        T2 = consecutive_counts[i+1]+consecutive_counts[i+2]
        T1 = consecutive_counts[i+2]+consecutive_counts[i+3]
    else:
        T1 = consecutive_counts[i]+consecutive_counts[i+1]
        T2 = consecutive_counts[i+1]+consecutive_counts[i+2]
        T4 = consecutive_counts[i+3]

    T = consecutive_counts[i]+consecutive_counts[i+1]+consecutive_counts[i+2]+consecutive_counts[i+3]

    # print(str(T1)+"  " + str(T2)+"  "  + str(T4)+"  "  + str(T))
    # print(str(inkSpreading(T1/T))+"  " + str(inkSpreading(T2/T))+"  "  + str(inkSpreading(T4/T)))
    return ink_decoder(T1,T2,T4, T)

def extract_middle_bits(row_pixels):
    """
    Extracts the middle section of pixels from a row, assuming a 1,0,1 pattern
    at the beginning and end, and removes any leading and trailing zeros.

    Args:
        row_pixels (list[int]): The modified row of pixels with 1,0,1 pattern (any length).

    Returns:
        list[int]: The middle section of the pixels (excluding the 1,0,1 patterns).

    Raises:
        ValueError: If the input list does not contain a valid 1,0,1 pattern.
    """

    if len(row_pixels) < 5:
        raise ValueError("Input list must have at least 5 elements to extract a middle section.")

    # Find the starting index of the non-zero elements (excluding trailing zeros)
    start_index = 0
    while start_index < len(row_pixels) and row_pixels[start_index] == 0:
        start_index += 1

    # Find the ending index of the non-zero elements (excluding leading zeros)
    end_index = len(row_pixels) - 1
    while end_index >= 0 and row_pixels[end_index] == 0:
        end_index -= 1

    # Check if a valid pattern exists (at least 5 non-zero elements)
    if end_index - start_index + 1 < 5:
        raise ValueError("Input list does not contain a valid 1,0,1 pattern.")

    # Check for valid pattern at the beginning and end (considering start_index and end_index)
    if row_pixels[start_index : start_index + 3] != [1, 0, 1] or row_pixels[end_index - 2 : end_index + 1] != [1, 0, 1]:
        raise ValueError("Input list does not have the expected 1,0,1 pattern at the beginning and end.")

    # Extract the middle section (excluding the first and last three elements)
    middle_bits = row_pixels[start_index + 3 : end_index - 2]

    return middle_bits

def analyze_consecutive_occurrences(data):
  """
  Analyzes the consecutive occurrences of values in a given array.

  Args:
      data: A list of integer values.

  Returns:
      A list of integers, where each element represents the number of consecutive
      occurrences of the corresponding value in the input array.
  """
  consecutive_counts = []
  current_value = None
  current_count = 0

  for value in data:
    if value != current_value:
      # New value encountered, update counts
      if current_value is not None:
        consecutive_counts.append(current_count)
      current_value = value
      current_count = 1
    else:
      # Same value encountered, increment count
      current_count += 1

  # Add the count for the last value
  if current_value is not None:
    consecutive_counts.append(current_count)

  return consecutive_counts


def isGuardBit(consecutive_counts,i):
    T1 = consecutive_counts[i]+consecutive_counts[i+1]
    T2 = consecutive_counts[i+1]+consecutive_counts[i+2]
    T4 = consecutive_counts[i+2]
    T = consecutive_counts[i]+consecutive_counts[i+1]+consecutive_counts[i+2]
    # print(str(T1)+"  " + str(T2)+"  "  + str(T4)+"  "  + str(T))

    sum= 0 
    sum= (T1 == T2) +sum
    sum= (T1 == 2*T4) +sum
    sum= (T == T1+T4) +sum
    sum= (T == T2+T4) +sum


    # print(sum)

    if(sum>=3):
       return 1
    else:
       return 0






img = cv.imread("9319133331372.png", cv.IMREAD_GRAYSCALE)
assert img is not None, "file could not be read, check with os.path.exists()"
 
# Otsu's thresholding after Gaussian filtering
blur = cv.GaussianBlur(img,(5,5),0)
ret3,th3 = cv.threshold(blur,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)
th3 = (255-th3)/255
 
row_5_pixels = th3[4, :]
# row_5_pixels = [0,0,0,1,0,1,   
                # 0,1,1,1,1,0,1,     0,1,1,0,0,1,1,    0,0,1,0,1,1,1,    0,0,1,1,0,0,1,  0,1,0,0,0,0,1,   0,1,1,1,1,0,1,    
                # 0,1,0,1,0,
                # 1,0,0,0,0,1,0,     1,0,0,0,0,1,0,    1,1,0,0,1,1,0,    1,0,0,0,0,1,0,  1,0,0,0,1,0,0,   1,1,0,1,1,0,0,
                # 1,0,1,0,0,0
                #   ]


# row_5_pixels = np.insert(th3[4, :], 0, 0) # start with a 0, then add in the rest of the numbers, this ensures the barcode is decoded properly
print("Pixel values in row 5:", row_5_pixels)
# extracted_bits = extract_middle_bits(row_5_pixels)
# print(extracted_bits)

######################################################################
# Consecutive counter for that row
consecutive_counts = analyze_consecutive_occurrences(row_5_pixels)
print(consecutive_counts)
print(len(consecutive_counts))

i=1
# for i in range(1,len(consecutive_counts)):
print(i)
# if(analyse(consecutive_counts,i,True) == 'guard bit' or analyse(consecutive_counts,i+55,False) == 'guard bit'):
count1 = range(i+3,6*4+i,4)
count2 = range(i+26,i+30,1)
count3 = range(i+32,6*4+i+32,4)

print("-------")
# print(analyse(consecutive_counts,i,True)) #[1]
print(isGuardBit(consecutive_counts,i))

for j in count1: #[4 8 12 16 20 24]
    print(analyse(consecutive_counts,j,True))
    
for j in count2:  #[27 28 29 30]
    print(isGuardBit(consecutive_counts,j))
    
for j in count3: #[33 37 41 45 49 53]
    print(analyse(consecutive_counts,j,False))
    
# print(analyse(consecutive_counts,i+55,False))#[56]
print(isGuardBit(consecutive_counts,i+56))
print("-------")
# break


# else:
#     # print(i)
#     print("wasn't able to find")




  
print("answer: || o3 e1 e9 o1 e3 o3 || e3 e3 e1 e3 e7 e2 ||")
# print(ink_decoder(2,2,1, 4))


# print(EAN_13_decode("0000101"))



# cv.imshow('Corners', th3)
# cv.waitKey(0)


