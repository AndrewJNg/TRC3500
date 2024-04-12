import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
import math
 

def inkSpreading(T1_val):
    mod = 0
    if(T1_val <= 1.5/7):
        mod = 1
    elif(T1_val >= 1.5/7 and T1_val < 2.5/7 ):
        mod = 2
    elif(T1_val >= 2.5/7 and T1_val < 3.5/7 ):
        mod = 3
    elif(T1_val >= 3.5/7 and T1_val < 4.5/7 ):
        mod = 4
    elif(T1_val >= 4.5/7  ):
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
#    if(T2==2 and T4==1 and T>(T2+T4)):
#         return "guard bit"
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
    sum= (T2 == 2*T4) +sum
    sum= (T == T1+T4) +sum
    sum= (T == T2+T4) +sum
    # print(sum)

    if(sum>=3):
       return 1
    else:
       return 0


def left_digit(list):
    table = {
      'oooooo': 0,
      'ooeoee': 1,
      'ooeeoe': 2,
      'ooeeeo': 3,
      'oeooee': 4,
      'oeeooe': 5,
      'oeeeoo': 6,
      'oeoeoe': 7,
      'oeoeeo': 8,
      'oeeoeo': 9
  }
    sum = ''
    for i in [0,1,2,3,4,5]:
        # print(list)
        sum=sum+list[i][0]
    # print(sum)
    if str(sum) in table:
        return table[str(sum)]  # Convert sum to string for dictionary lookup
    else:
        return -1


img = cv.imread("IMG_20240227_0004.jpg", cv.IMREAD_GRAYSCALE)
assert img is not None, "file could not be read, check with os.path.exists()"

blur = cv.GaussianBlur(img, (5, 5), 0)
ret3, th3 = cv.threshold(blur, 0, 255, cv.THRESH_BINARY + cv.THRESH_OTSU)
th3 = (255 - th3) / 255

for row_count in range(len(th3)):
    row_pixels = th3[row_count, :]
    row_pixels_rev = th3[row_count, :][::-1]

    consecutive_counts = analyze_consecutive_occurrences(row_pixels)
    if len(consecutive_counts) >= (6 * 4 + 34):
        output_list = []
        for i in range(0, len(consecutive_counts)-55):
            count = [i, 26 + i, 27 + i, 28 + i, 29 + i, 56 + i]
            guardSum = 0

            for j in count:
                if j < (6 * 4 + 33):
                    guardSum += isGuardBit(consecutive_counts, j)

            if guardSum >= 2:
                count1 = range(i + 3, 6 * 4 + i, 4)
                count3 = range(i + 32, 6 * 4 + i+32, 4)

                for j in count1:
                    output_list.append(analyse(consecutive_counts, j, True))
                for j in count3:
                    output_list.append(analyse(consecutive_counts, j, False))
                break
            else:
                pass

######################################################################################################################################

        first_digit = left_digit(output_list)
        if str(first_digit) != '-1':
            output_list.insert(0, str(first_digit))

            odd_sum = sum(int(output_list[i][1]) for i in [1, 3, 5, 7, 9, 11])
            even_sum = int(output_list[0]) + sum(int(output_list[i][1]) for i in [2, 4, 6, 8, 10])            
            parity = 10 - ((odd_sum * 3 + even_sum) % 10) if ((odd_sum * 3 + even_sum) % 10 != 0) else 0
            print(str(odd_sum) +'  '+ str(even_sum)+'  '+str(parity))
            if parity == int(output_list[12][1]):
                print("row " + str(row_count) + "   " + str(output_list))
    





cv.imshow('Corners', img)
cv.waitKey(0)


