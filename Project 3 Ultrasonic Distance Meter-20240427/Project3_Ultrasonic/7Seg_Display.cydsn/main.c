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
void BuzzerRuntime(double duration);
void seg_runtime(double value);
void runtime();

void EEPROM_writeDouble(uint16 address, double value);
double EEPROM_readDouble(uint16 address);
void writeDoubleArrayToEEPROM(uint16 startAddress, double* array, uint16 arraySize);
void readDoubleArrayFromEEPROM(uint16 startAddress, double* array, uint16 arraySize);
void EEPROM_writeInt(uint16 address, int value);
int EEPROM_readInt(uint16 address) ;


// ========================================================================================================================
double counter = 0;

int time_passed_step=0;

CY_ISR(MyCustomISR1) //Select button interrupt
{
    // Launch 1 ultrasonic cycle
    send_ultra_pulse();
}

double previous_buzz= 0;

double buzz_duration=0;
double seg_num =0;

CY_ISR(MyCustomISR2) //Right button interrupt
{
    Timer_1_Stop();
    time_passed_step = Timer_1_ReadPeriod()-Timer_1_ReadCounter() ;
    
    //previous_buzz = current_second();
    //buzz_duration=0.2;
    BuzzerSound(0.1);
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
    EEPROM_Start(); 
    
    // Setup UART
    UART_1_Start();
    
    
    double setup_previous_time =current_second();
    while((current_second()-setup_previous_time)<=4)
    {
        if((int)(current_second()-setup_previous_time) ==0) display_num(-5);
        if((int)(current_second()-setup_previous_time) ==1) display_num(-6);
        if((int)(current_second()-setup_previous_time) ==2) display_num(-7);
        if((int)(current_second()-setup_previous_time) ==3) display_num(-8);
        runtime();
    }
    
    // EEPROM Write startup
    /*
    // Place your initialization/startup code here (e.g. MyInst_Start())
    int teamNumber = 200;
    EEPROM_writeInt(0, teamNumber);
    int nextAddress = sizeof(int);
    char measurementSystem = 'c';
    EEPROM_WriteByte((uint8)measurementSystem, nextAddress);
    nextAddress += sizeof(char);
    double calibration[55] = {0.0};
    writeDoubleArrayToEEPROM(nextAddress, calibration, 55);
    */
    
    //int eprom_read = EEPROM_ReadByte(0x00);
    //int eprom_write =eprom_read+1;
    //EEPROM_WriteByte(eprom_write,0x0000);
    
    
    //int eprom_num = EEPROM_ReadByte(55*sizeof(double)+sizeof(char));
    int eprom_num = EEPROM_ReadByte(0);
    char eprom_in = EEPROM_ReadByte(sizeof(int));
    
    
    
    for(;;)
    {
        
        //display_num( ultra_reading());
        //display_num( current_second());
        if (eprom_in =='c') display_num(-1);
        if (eprom_in =='i') display_num(-2);
        
        //display_num( eprom_num);
        //display_num( 12 );
        //display_num(-3);
        //BuzzerRuntime(0.2);
        runtime();
        
        
    }
}
void runtime()
{
    BuzzerRuntime(buzz_duration);
    seg_runtime(seg_num);
    
}

double current_second()
{
    double time = (double)(Timer_2_ReadPeriod()-Timer_2_ReadCounter());
    time = time/1000;
    
    return time;
}
// ========================================================================================================================
/* EEPROM */
/*
void EEPROM_writeDouble(uint16 address, double value) {
    uint8 ptr = (uint8)(void*)&value;  // Convert double to byte array
    for (uint8 i = 0; i < sizeof(double); i++) {
        EEPROM_WriteByte(ptr[i], address + i);
    }
}

double EEPROM_readDouble(uint16 address) {
    double value = 0.0;
    uint8 ptr = (uint8)(void*)&value;
    for (uint8 i = 0; i < sizeof(double); i++) {
        ptr[i] = EEPROM_ReadByte(address + i);
    }
    return value;
}

void writeDoubleArrayToEEPROM(uint16 startAddress, double* array, uint16 arraySize) {
    for (uint16 i = 0; i < arraySize; i++) {
        EEPROM_writeDouble(startAddress + i * sizeof(double), array[i]);
    }
}

void readDoubleArrayFromEEPROM(uint16 startAddress, double* array, uint16 arraySize) {
    for (uint16 i = 0; i < arraySize; i++) {
        array[i] = EEPROM_readDouble(startAddress + i * sizeof(double));
    }
}

void EEPROM_writeInt(uint16 address, int value) {
    uint8 ptr = (uint8)(void*)&value;  // Convert int to byte array
    for (uint8 i = 0; i < sizeof(int); i++) {
        EEPROM_WriteByte(ptr[i], address + i);
    }
}

int EEPROM_readInt(uint16 address) {
    int value = 0;
    uint8 ptr = (uint8)(void*)&value;
    for (uint8 i = 0; i < sizeof(int); i++) {
        ptr[i] = EEPROM_ReadByte(address + i);
    }
    return value;
}
*/

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
    previous_buzz = current_second();
    buzz_duration= duration;
}
void BuzzerRuntime(double duration)
{
    double time_diff = current_second()-previous_buzz;
    
    if(time_diff<duration) Buzzer_Write(0);  // Turn on buzzer
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

void display_num(double value)
{
    seg_num = value;
}
void seg_runtime(double value){
      
    if (run_7_Seg==0) return; // Ignore the rest of the code when interrupt is not triggered
    
    if(value>0)
    {
    
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
            
    }
    else if(value== -1) //Show cm icon
    {
        D[0] = 15; //C
        D[1] = 16; //E
        D[2] = 18; //N
        D[3] = 17; //t
    }
    else if(value== -2) //Show inch icon
    {
        D[0] = 13; //i 
        D[1] = 12; //n 
        D[2] = 11; //c
        D[3] = 14; //h
        
    }
    else if(value== -3) // Dot
    {
        D[0] = 10; //space
        D[1] = 10; //space
        D[2] = 10; //space
        D[3] = 10; //space
        dp[1]=1;
    }
    else if(value== -4) // Nothing
    {
        D[0] = 10; //space
        D[1] = 10; //space
        D[2] = 10; //space
        D[3] = 10; //space
    }
    else // reserve for startup
    {
        D[0] = 10; //space
        D[1] = 10; //space
        D[2] = 10; //space
        D[3] = 10; //space
        
        if(value ==-5) D[0] = 8; 
        if(value ==-6) D[1] = 8; 
        if(value ==-7) D[2] = 8; 
        if(value ==-8) D[3] = 8; 
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
  "0000000", // Space - 10
  "0001101", // c -11
  "0010101", // n -12
  "0010000", // i -13
  "0010111", // h -14
  "1001110", // big C -15
  "1001111", // big E -16
  "0001111", // t -17
  "1110110", // big N -18
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
