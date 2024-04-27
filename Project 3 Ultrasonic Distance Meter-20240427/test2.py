
value = 126.2;  
dp_pos = 1

while (value<1000):

    value=value*10
    dp_pos+=1


digits = int(value)
digit1 = int(digits/1000)%10
digit2 = int(digits/100)%10
digit3 = int(digits/10)%10
digit4 = int(digits/1) %10

print(value)
print(dp_pos)

print(digit1)
print(digit2)
print(digit3)
print(digit4)