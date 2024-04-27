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


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    double counter = 0;
    for(;;)
    {
        counter +=0.01;
        display_num(counter);
        


        /* Place your application code here. */
    }
}

int delayTime=5; // On time for each display to be turned on for
void display_num(double value)
{       
    
        // Convert double to d1,d2,d3,d4,dp_pos
        int dp_pos = 0;
        
        while(value<1000)
        {
            value=value*10;
            dp_pos+=1;
        }
        
        // Extract integer and decimal parts
        int D[4]= {0,0,0,0};
        int digits = (int)value;
        D[0] = (int)(digits/1000)%10;
        D[1] = (int)(digits/100)%10;
        D[2] = (int)(digits/10)%10;
        D[3] = (int)(digits/1) %10;
     
    
        ////////////////////////////////////////////
        // fill each segments of the display
        int dp[4]={0,0,0,0};
        dp[dp_pos]=1;
    
        
        for (int i=0;i<4;i++)
        {
        // Turn off all digits
        Dis_D1_Write(1);
        Dis_D2_Write(1);
        Dis_D3_Write(1);
        Dis_D4_Write(1);
        
        draw_7Seg(charLookup[D[i]],dp[3-i]);
        
           
        if     (i==0)Dis_D1_Write(0);
        else if(i==1)Dis_D2_Write(0);
        else if(i==2)Dis_D3_Write(0);
        else if(i==3)Dis_D4_Write(0);
        CyDelay(delayTime);
        }
         /*
        
        // Turn off all digits
        Dis_D1_Write(1);
        Dis_D2_Write(1);
        Dis_D3_Write(1);
        Dis_D4_Write(1);
        
        
        // Display the needed value
        draw_7Seg(charLookup[d1],dp[3]);
        
        // Turn on needed digit
        Dis_D1_Write(0);
        Dis_D2_Write(1);
        Dis_D3_Write(1);
        Dis_D4_Write(1);
        
        CyDelay(delayTime);
        
        //////////////////////////////////
        // Turn off all digits
        Dis_D1_Write(1);
        Dis_D2_Write(1);
        Dis_D3_Write(1);
        Dis_D4_Write(1);

        // Display the needed value
        draw_7Seg(charLookup[d2],dp[2]);
        
        // Turn on needed digit
        Dis_D1_Write(1);
        Dis_D2_Write(0);
        Dis_D3_Write(1);
        Dis_D4_Write(1);
        
        CyDelay(delayTime);
        //////////////////////////////////
        // Turn off all digits
        Dis_D1_Write(1);
        Dis_D2_Write(1);
        Dis_D3_Write(1);
        Dis_D4_Write(1);

        // Display the needed value
        draw_7Seg(charLookup[d3],dp[1]);
        
        // Turn on needed digit
        Dis_D1_Write(1);
        Dis_D2_Write(1);
        Dis_D3_Write(0);
        Dis_D4_Write(1);
        
        CyDelay(delayTime);
        //////////////////////////////////
        // Turn off all digits
        Dis_D1_Write(1);
        Dis_D2_Write(1);
        Dis_D3_Write(1);
        Dis_D4_Write(1);

        // Display the needed value
        draw_7Seg(charLookup[d4],dp[0]);
        
        // Turn on needed digit
        Dis_D1_Write(1);
        Dis_D2_Write(1);
        Dis_D3_Write(1);
        Dis_D4_Write(0);
        
        CyDelay(delayTime);
        */
        
}


int segmentPattern_array[7];  // Array to store the converted 1s and 0s (7 elements for 7 segments)
void draw_7Seg(const char* segmentPattern,int dp)
{
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


/* [] END OF FILE */
