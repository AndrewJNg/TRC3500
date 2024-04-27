# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\Monash\Y3S2\TRC3500\TRC3500-Lab\Project 3 Ultrasonic Distance Meter-20240427\Project3_Ultrasonic\7Seg_Display.cydsn\7Seg_Display.cyprj
# Date: Sat, 27 Apr 2024 06:13:43 GMT
#set_units -time ns
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]


# Component constraints for D:\Monash\Y3S2\TRC3500\TRC3500-Lab\Project 3 Ultrasonic Distance Meter-20240427\Project3_Ultrasonic\7Seg_Display.cydsn\TopDesign\TopDesign.cysch
# Project: D:\Monash\Y3S2\TRC3500\TRC3500-Lab\Project 3 Ultrasonic Distance Meter-20240427\Project3_Ultrasonic\7Seg_Display.cydsn\7Seg_Display.cyprj
# Date: Sat, 27 Apr 2024 06:13:41 GMT
