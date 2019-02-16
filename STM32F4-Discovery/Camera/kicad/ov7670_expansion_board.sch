EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ov7670_expansion_board-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "OV7670 expansion board for STM32F4 DISCO"
Date "2019-02-13"
Rev ""
Comp "https://github.com/araobp"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Conn_02x25_Odd_Even J1
U 1 1 5C63F53B
P 2600 2600
F 0 "J1" H 2650 3900 50  0000 C CNN
F 1 "Conn_02x25_Odd_Even" H 2650 1300 50  0000 C CNN
F 2 "" H 2600 2600 50  0001 C CNN
F 3 "" H 2600 2600 50  0001 C CNN
	1    2600 2600
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x25_Odd_Even J3
U 1 1 5C63F576
P 7050 2600
F 0 "J3" H 7100 3900 50  0000 C CNN
F 1 "Conn_02x25_Odd_Even" H 7100 1300 50  0000 C CNN
F 2 "" H 7050 2600 50  0001 C CNN
F 3 "" H 7050 2600 50  0001 C CNN
	1    7050 2600
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x09_Odd_Even J2
U 1 1 5C63F59D
P 5050 5150
F 0 "J2" H 5100 5650 50  0000 C CNN
F 1 "Conn_02x09_Odd_Even" H 5100 4650 50  0000 C CNN
F 2 "" H 5050 5150 50  0001 C CNN
F 3 "" H 5050 5150 50  0001 C CNN
	1    5050 5150
	0    1    1    0   
$EndComp
Text Label 6650 1600 0    60   ~ 0
3V3
Text Label 6650 1400 0    60   ~ 0
GND
$Comp
L R R2
U 1 1 5C63F766
P 5950 3050
F 0 "R2" V 6030 3050 50  0000 C CNN
F 1 "10k" V 5950 3050 50  0000 C CNN
F 2 "" V 5880 3050 50  0001 C CNN
F 3 "" H 5950 3050 50  0001 C CNN
	1    5950 3050
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5C63F789
P 5750 3050
F 0 "R1" V 5830 3050 50  0000 C CNN
F 1 "10k" V 5750 3050 50  0000 C CNN
F 2 "" V 5680 3050 50  0001 C CNN
F 3 "" H 5750 3050 50  0001 C CNN
	1    5750 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 1600 6850 1600
Wire Wire Line
	5450 1600 5450 4950
Wire Wire Line
	6850 1400 5600 1400
Wire Wire Line
	5600 1400 5600 5600
Wire Wire Line
	5600 5600 5450 5600
Wire Wire Line
	5450 5600 5450 5450
Wire Wire Line
	5750 2900 5750 1600
Connection ~ 5750 1600
Wire Wire Line
	5950 2900 5950 1600
Connection ~ 5950 1600
Wire Wire Line
	5750 3200 5750 3400
Wire Wire Line
	5750 3400 5350 3400
Wire Wire Line
	5350 2300 6850 2300
Wire Wire Line
	5350 2300 5350 4950
Wire Wire Line
	5350 5450 5350 5800
Wire Wire Line
	5350 5800 9000 5800
Wire Wire Line
	5950 5800 5950 3200
Connection ~ 5350 3400
Text Label 6150 2300 0    60   ~ 0
SIOC/SCL(PB8)
Wire Wire Line
	9000 5800 9000 2300
Connection ~ 5950 5800
Wire Wire Line
	9100 2200 9100 4650
Wire Wire Line
	9100 4650 4650 4650
Wire Wire Line
	4650 4650 4650 4950
Text Label 7400 2200 0    60   ~ 0
RESET(PE1)
Text Label 7400 2300 0    60   ~ 0
SIOD/SDA(PB9)
Wire Wire Line
	6850 3500 6050 3500
Wire Wire Line
	6050 3500 6050 5900
Wire Wire Line
	6050 5900 5150 5900
Wire Wire Line
	5150 5900 5150 5450
Text Label 6050 3500 0    60   ~ 0
MCO/XCLK(PA8)
Wire Wire Line
	6850 3700 4050 3700
Wire Wire Line
	4050 3700 4050 5950
Wire Wire Line
	4050 5950 4750 5950
Wire Wire Line
	4750 5950 4750 5450
Text Label 6200 3700 0    60   ~ 0
DCMI_D0(PC6)
Wire Wire Line
	7350 3700 8700 3700
Wire Wire Line
	8700 3700 8700 4550
Wire Wire Line
	8700 4550 4750 4550
Wire Wire Line
	4750 4550 4750 4950
Text Label 7450 3700 0    60   ~ 0
DCMI_D1(PC7)
Wire Wire Line
	6850 3600 6150 3600
Wire Wire Line
	6150 3600 6150 5700
Wire Wire Line
	6150 5700 4850 5700
Wire Wire Line
	4850 5700 4850 5450
Wire Wire Line
	7350 3600 8800 3600
Wire Wire Line
	8800 3600 8800 4750
Wire Wire Line
	8800 4750 4850 4750
Wire Wire Line
	4850 4750 4850 4950
Text Label 7450 3600 0    60   ~ 0
DCMI_D3(PC9)
Text Label 6150 3600 0    60   ~ 0
DCMI_D2(PC8)
Wire Wire Line
	4250 2000 6850 2000
Wire Wire Line
	4250 2000 4250 5600
Wire Wire Line
	4250 5600 4950 5600
Wire Wire Line
	4950 5600 4950 5450
Text Label 6200 2000 0    60   ~ 0
DCMI_D4(PE4)
Wire Wire Line
	6850 2500 4950 2500
Wire Wire Line
	4950 2500 4950 4950
Text Label 6200 2500 0    60   ~ 0
DCMI_D5(PB6)
Wire Wire Line
	7350 2000 9200 2000
Wire Wire Line
	9200 2000 9200 6000
Wire Wire Line
	9200 6000 5050 6000
Wire Wire Line
	5050 6000 5050 5450
Text Label 7400 2000 0    60   ~ 0
DCMI_D6(PE5)
Wire Wire Line
	6850 1900 5050 1900
Wire Wire Line
	5050 1900 5050 4950
Text Label 6200 1900 0    60   ~ 0
DCMI_D7(PE6)
Wire Wire Line
	2900 2200 5150 2200
Text Label 2950 2200 0    60   ~ 0
DCMI_PIXCK/PCLK(PA6)
Wire Wire Line
	5150 2200 5150 4950
Wire Wire Line
	7350 2500 8900 2500
Wire Wire Line
	8900 2500 8900 4850
Wire Wire Line
	8900 4850 5250 4850
Wire Wire Line
	5250 4850 5250 4950
Text Label 7400 2500 0    60   ~ 0
VSYNC(PB7)
Wire Wire Line
	2900 2100 4150 2100
Wire Wire Line
	4150 2100 4150 6100
Wire Wire Line
	4150 6100 5250 6100
Wire Wire Line
	5250 6100 5250 5450
Text Label 2950 2100 0    60   ~ 0
DCMI_HSYNC/HREF(PA4)
NoConn ~ 4650 5450
NoConn ~ 2400 1400
NoConn ~ 2400 1500
NoConn ~ 2400 1600
NoConn ~ 2400 1700
NoConn ~ 2400 1800
NoConn ~ 2400 1900
NoConn ~ 2400 2100
NoConn ~ 2400 2200
NoConn ~ 2400 2300
NoConn ~ 2400 2400
NoConn ~ 2400 2500
NoConn ~ 2400 2600
NoConn ~ 2400 2700
NoConn ~ 2400 2800
NoConn ~ 2400 2900
NoConn ~ 2400 3000
NoConn ~ 2400 3100
NoConn ~ 2400 3200
NoConn ~ 2400 3300
NoConn ~ 2400 3400
NoConn ~ 2400 3500
NoConn ~ 2400 3600
NoConn ~ 2400 3700
NoConn ~ 2400 3800
NoConn ~ 2900 3800
NoConn ~ 2900 3700
NoConn ~ 2900 3600
NoConn ~ 2900 3500
NoConn ~ 2900 3400
NoConn ~ 2900 3300
NoConn ~ 2900 3200
NoConn ~ 2900 3100
NoConn ~ 2900 3000
NoConn ~ 2900 2900
NoConn ~ 2900 2800
NoConn ~ 2900 2700
NoConn ~ 2900 2600
NoConn ~ 2900 2500
NoConn ~ 2900 2400
NoConn ~ 2900 2300
NoConn ~ 2900 1900
NoConn ~ 2900 1800
NoConn ~ 2900 1700
NoConn ~ 2900 1600
NoConn ~ 2900 1500
NoConn ~ 2900 1400
NoConn ~ 6850 1500
NoConn ~ 7350 1400
NoConn ~ 7350 1500
NoConn ~ 7350 1600
NoConn ~ 6850 1700
NoConn ~ 6850 1800
NoConn ~ 7350 1700
NoConn ~ 7350 1800
NoConn ~ 7350 1900
NoConn ~ 7350 2400
NoConn ~ 6850 2600
NoConn ~ 6850 2700
NoConn ~ 7350 2600
NoConn ~ 7350 2700
NoConn ~ 7350 2800
NoConn ~ 6850 2800
NoConn ~ 6850 2900
NoConn ~ 6850 3000
NoConn ~ 6850 3100
NoConn ~ 6850 3200
NoConn ~ 6850 3300
NoConn ~ 6850 3400
NoConn ~ 7350 3400
NoConn ~ 7350 3300
NoConn ~ 7350 3200
NoConn ~ 7350 3100
NoConn ~ 7350 3000
NoConn ~ 7350 2900
NoConn ~ 7350 3500
NoConn ~ 6850 3800
NoConn ~ 7350 3800
NoConn ~ 6850 2100
NoConn ~ 6850 2400
NoConn ~ 7350 2100
Wire Wire Line
	9000 2300 7350 2300
Wire Wire Line
	7350 2200 9100 2200
NoConn ~ 6850 2200
Text Notes 4350 6300 0    60   ~ 0
HiLetgo OV7670 camera module
Text Notes 4700 1100 0    60   ~ 0
STM32F4 Discovery
Wire Notes Line
	2050 1150 2050 4000
Wire Notes Line
	2050 4000 8200 4000
Wire Notes Line
	8200 4000 8200 1150
Wire Notes Line
	8200 1150 2050 1150
Wire Notes Line
	4400 4900 4400 6200
Wire Notes Line
	4400 6200 5750 6200
Wire Notes Line
	5750 6200 5750 4900
Wire Notes Line
	5750 4900 4400 4900
Text Label 5450 4950 1    60   ~ 0
3V3
Text Label 5450 5650 1    60   ~ 0
GND
Text Label 5350 4950 1    60   ~ 0
SIOC
Text Label 5350 5700 1    60   ~ 0
SIOD
Text Label 5250 4950 1    60   ~ 0
VSYNC
Text Label 5250 5700 1    60   ~ 0
HREF
Text Label 5150 4950 1    60   ~ 0
PCLK
Text Label 5150 5700 1    60   ~ 0
XCLK
Text Label 5050 4900 1    60   ~ 0
D7
Text Label 5050 5600 1    60   ~ 0
D6
Text Label 4950 4900 1    60   ~ 0
D5
Text Label 4950 5600 1    60   ~ 0
D4
Text Label 4850 4900 1    60   ~ 0
D3
Text Label 4850 5600 1    60   ~ 0
D2
Text Label 4750 4900 1    60   ~ 0
D1
Text Label 4750 5600 1    60   ~ 0
D0
Text Label 4650 4900 1    60   ~ 0
RESET
$Comp
L Conn_01x04 J4
U 1 1 5C65E357
P 2700 5200
F 0 "J4" H 2700 5400 50  0000 C CNN
F 1 "Conn_01x04" H 2700 4900 50  0000 C CNN
F 2 "" H 2700 5200 50  0001 C CNN
F 3 "" H 2700 5200 50  0001 C CNN
	1    2700 5200
	0    1    1    0   
$EndComp
NoConn ~ 2800 5000
NoConn ~ 2700 5000
Wire Wire Line
	2400 2000 1550 2000
Wire Wire Line
	1550 2000 1550 4200
Wire Wire Line
	1550 4200 2600 4200
Wire Wire Line
	2600 4200 2600 5000
Wire Wire Line
	2900 2000 3700 2000
Wire Wire Line
	3700 2000 3700 4300
Wire Wire Line
	3700 4300 2500 4300
Wire Wire Line
	2500 4300 2500 5000
Text Label 1600 2000 0    60   ~ 0
USART2_RX(PA3)
Text Label 2950 2000 0    60   ~ 0
USART2_TX(PA2)
Text Label 2600 4900 1    60   ~ 0
TX
Text Label 2500 4900 1    60   ~ 0
RX
Wire Notes Line
	2250 4950 2250 5600
Wire Notes Line
	2250 5600 3000 5600
Wire Notes Line
	3000 5600 3000 4950
Wire Notes Line
	3000 4950 2250 4950
Text Notes 2000 5700 0    60   ~ 0
USB-Serial converter module
Text Notes 2750 1300 0    60   ~ 0
P1
Text Notes 7250 1300 0    60   ~ 0
P2
$EndSCHEMATC
