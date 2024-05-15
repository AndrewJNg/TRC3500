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

// ========================================================================================================================
#include "project.h"

void display_num(double value);
void draw_7Seg(int number,int dp);
CY_ISR(seg7_ISR);
CY_ISR(MyCustomISR2);
void print_uart(int num);
void send_ultra_pulse();
CY_ISR(MyCustomISR1);
CY_ISR(Receive_Ultra);
double ultra_reading();
double current_second();
void BuzzerSound(double duration);


// ========================================================================================================================
double counter = 0;

int time_passed_step=0;

CY_ISR(MyCustomISR1) //Select button interrupt
{
    // Launch 1 ultrasonic cycle
    send_ultra_pulse();
}

double previous_buzz=0;

CY_ISR(MyCustomISR2) //Right button interrupt
{
    Timer_1_Stop();
    time_passed_step = Timer_1_ReadPeriod()-Timer_1_ReadCounter() ;
    previous_buzz = current_second();
}





int main(void)
{
    // Enable interrupts
    CyGlobalIntEnable; 
    
    // Clear any existing interrupts
    isr_7Seg_ClearPending(); 
    isr_1_ClearPending(); 
    isr_2_ClearPending(); 
    isr_3_ClearPending(); 

    // Enable button interrupts
    isr_7Seg_StartEx(seg7_ISR);  
    isr_1_StartEx(MyCustomISR1);  
    isr_2_StartEx(MyCustomISR2);  
    
    //  Ultrasonic transmitter setup
    Timer_1_SetInterruptMode(3);
    isr_3_StartEx(Receive_Ultra);  
    
    // Ultrasonic receiver setup
    PGA_1_Start();
    Opamp_1_Start();
    Comp_1_Start();
    VDAC8_1_Start();
    Count7_1_Start(); 
    
    // Global timer 
    Timer_2_SetInterruptMode(3);
    Timer_2_Start();
    
    // Setup EEPROM
    EEPROM_1_Start(); 
    //EEPROM_1_Enable();
    UART_1_Start();
    
    for(;;)
    {
        
        //display_num( ultra_reading());
        display_num( current_second());
        BuzzerSound(0.2);
    }
}
double current_second()
{
    double time = (double)(Timer_2_ReadPeriod()-Timer_2_ReadCounter());
    time = time/1000;
    
    return time;
}

// ========================================================================================================================
/* UART communications */

uint8 value_text[16];
void print_uart(int num)
{ 
    int num_digits = 0;
    int temp = num;
    
    // Count the number of digits (optional for pre-allocation)
    while (temp != 0) {
        temp /= 10;
        num_digits++;
    }

    for (int i = num_digits-1; i >= 0; --i) {
        value_text[i] = num % 10;
        num /= 10;
    }
    
    for (int i = 0; i < num_digits; ++i) {
        UART_1_WriteTxData(value_text[i]+0x30); //display number
        CyDelay(5);
    }
    
}

// ========================================================================================================================
/* Buzzer */

void BuzzerSound(double duration)
{
    long time_diff = current_second()-previous_buzz;
    if(time_diff<=duration) Buzzer_Write(0);  // Turn on buzzer
    else Buzzer_Write(1); // Turn off buzzer
}


// ========================================================================================================================
/* Ultrasinic launch function*/

double value = 0; 
double distance = 0;

double ultra_reading()
{
    send_ultra_pulse();
    
    // TODO: may need to write blocking code to obtain ultrasonic values
    value =  ((double)time_passed_step/10000)-0.5; //in ms
    distance = (34*value)/2;
    
    return distance;
}
void send_ultra_pulse()
{
    Control_Reg_1_Write(1); // Reset counter to do more pulses
    Timer_1_Start();        // Start receiver timer
    Control_Reg_1_Write(0);
}

uint8 InterruptCnt;
CY_ISR(Receive_Ultra)  // Ultrasonic Receiver
{
    Timer_1_Stop();
    time_passed_step = Timer_1_ReadPeriod()-Timer_1_ReadCounter() ;
}


// ========================================================================================================================
/* Mid Level 7 Segment display code*/

//Global variable
int run_7_Seg = 0;

// Interrupt routine for displaying each sections
CY_ISR(seg7_ISR) 
{
    run_7_Seg = 1;
}

double old_dis_val=0;

int D[4]= {0,0,0,0};
int dp[4]={0,0,0,0};
unsigned long count = 0;
        
void display_num(double value){       
    if (run_7_Seg==0) return; // Ignore the rest of the code when interrupt is not triggered
    
    //////////////// Code after this would only run once per interrupt////////////////
    // Only update the value when it gets a new value (for consistency and save computation for pooling loop)
    if((value != old_dis_val)){ 
        // Convert double to d1,d2,d3,d4,dp_pos
        int dp_pos = 0;
        
        if (value>=1){ // For values above 1
        while(value<1000){
            value=value*10;
             dp_pos+=1;
        }
                
        // Extract integer and decimal parts
        int digits = (int)value;
                
        D[0] = (int)(digits/1000)%10;
        D[1] = (int)(digits/100)%10;
        D[2] = (int)(digits/10)%10;
        D[3] = (int)(digits/1) %10;
    }
        else { //For below 1, fix decimal point to always show 4 significant figure (eg. 0.900, 0.805, 0.010)
        dp_pos=3;
        int digits = (int) (value*1000);
                
        D[0] = (int)(digits/1000)%10;
        D[1] = (int)(digits/100)%10;
        D[2] = (int)(digits/10)%10;
        D[3] = (int)(digits/1) %10;
    }
        
    ////////////////////////////////////////////
    // fill each segments of the display
    for (int i = 0; i < 4; ++i) dp[i] = 0; // Reset all 0 
        dp[dp_pos]=1;
            
        old_dis_val = value;
    }
        
    // Loop through each display each time we run 7_seg display function
    count++;
    count = count %4; //limit count between 0 to 3
        
    // Turn off all sections
    Dis_D1_Write(1);
    Dis_D2_Write(1);
    Dis_D3_Write(1);
    Dis_D4_Write(1);
        
    // Update to show a specific value
    draw_7Seg(D[count],dp[3-count]);
        
    // Only show on specified section
    if     (count==0)Dis_D1_Write(0);
    else if(count==1)Dis_D2_Write(0);
    else if(count==2)Dis_D3_Write(0);
    else if(count==3)Dis_D4_Write(0);
        
    run_7_Seg =0;
    
}

// ========================================================================================================================
/* Low level 7 segment display conversion code*/
// Lookup table for 0 to 9
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

// update pins to show a specific number and turn on the decimal point if required
int segmentPattern_array[7];  // Array to store the converted 1s and 0s (7 elements for 7 segments)
void draw_7Seg(int number,int dp)
{
    const char* segmentPattern = charLookup[number];
    
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
