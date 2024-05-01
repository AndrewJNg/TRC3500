/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

void draw_7Seg(const char* segmentPattern,int dp);
void display_num(double value);

double counter = 0;
int run_7_Seg = 0;

CY_ISR(ISR) // This is the ISR
{
    run_7_Seg = 1;
}

CY_ISR(MyCustomISR2) // This is the ISR
{
    counter +=0.001;
}

int main(void)
{
    // Enable interrupts
    CyGlobalIntEnable; 
    isr_7Seg_ClearPending(); 
    isr_2_ClearPending(); 

    isr_7Seg_StartEx(ISR); // Enable the interrupt service routine
    isr_2_StartEx(MyCustomISR2); // Enable the interrupt service routine

    for(;;)
    {
        
        
        display_num(counter);
        


        /* Place your application code here. */
    }
}


// ========================================================================================================================
// 7 Segment display code
const char charLookup[][8] = {
  "1111110", // 0
  "0110000", // 1
  "1101101", // 2
  "1111001", // 3
  "0110011", // 4
  "1011011", // 5
  "1011111", // 6
  "1110000", // 7
  "1111111", // 8
  "1111011", // 9
  "0000000", // Space
};
int delayTime=5; // On time for each display to be turned on for
double old_dis_val=0;

int D[4]= {0,0,0,0};
int digits = 0;
int dp[4]={0,0,0,0};
unsigned long count = 0;
        
void display_num(double value)
{       
    if (run_7_Seg==1)
    {
        if(value != old_dis_val)
        {
            // Convert double to d1,d2,d3,d4,dp_pos
            int dp_pos = 0;
        
            if (value>=1) // For values above 1
            {
                    while(value<1000)
                {
                    value=value*10;
                    dp_pos+=1;
                }
                // Extract integer and decimal parts
                digits = (int)value;
                
                D[0] = (int)(digits/1000)%10;
                D[1] = (int)(digits/100)%10;
                D[2] = (int)(digits/10)%10;
                D[3] = (int)(digits/1) %10;
            }
            else //For below 1, fix decimal point at digit 1 (eg. 0.9, 0.8, 0.01)
            {
                dp_pos=3;
                
                digits = (int) (value*1000);
                
                D[0] = (int)(digits/1000)%10;
                D[1] = (int)(digits/100)%10;
                D[2] = (int)(digits/10)%10;
                D[3] = (int)(digits/1) %10;
                
            }
        
         
            ////////////////////////////////////////////
            // fill each segments of the display
            //int dp[4]={0,0,0,0};
            dp[0] = 0;
            dp[1] = 0;
            dp[2] = 0;
            dp[3] = 0;
            dp[dp_pos]=1;
            
            old_dis_val = value;
        }
        
        
        // Loop through each display each time we run 7_seg display function
        count++;
        count = count %4; //limit count between 0 to 3
        
        //for (int i=0;i<4;i++)
        //{
        // Turn off all digits
        Dis_D1_Write(1);
        Dis_D2_Write(1);
        Dis_D3_Write(1);
        Dis_D4_Write(1);
        
        // Update new value to display
        draw_7Seg(charLookup[D[count]],dp[3-count]);
        
        // Only show on specified section
        if     (count==0)Dis_D1_Write(0);
        else if(count==1)Dis_D2_Write(0);
        else if(count==2)Dis_D3_Write(0);
        else if(count==3)Dis_D4_Write(0);
        
        run_7_Seg =0;
    }
        
        // Short delay to reduce flicker
        //CyDelay(delayTime);
        //}
        
}

int segmentPattern_array[7];  // Array to store the converted 1s and 0s (7 elements for 7 segments)
void draw_7Seg(const char* segmentPattern,int dp)
{
    // Convert string to integer
    for (int i = 0; i < 7; i++) {
        segmentPattern_array[i] = segmentPattern[i]-'0';
    }
    
    Dis_A_Write(!segmentPattern_array[0]);
    Dis_B_Write(!segmentPattern_array[1]);
    Dis_C_Write(!segmentPattern_array[2]);
    Dis_D_Write(!segmentPattern_array[3]);
    Dis_E_Write(!segmentPattern_array[4]);
    Dis_F_Write(!segmentPattern_array[5]);
    Dis_G_Write(!segmentPattern_array[6]);
    Dis_DP_Write(!dp);
}
// ========================================================================================================================



/* [] END OF FILE */
