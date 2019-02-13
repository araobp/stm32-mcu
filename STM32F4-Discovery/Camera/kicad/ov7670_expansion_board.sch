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
P 3400 3000
F 0 "J1" H 3450 4300 50  0000 C CNN
F 1 "Conn_02x25_Odd_Even" H 3450 1700 50  0000 C CNN
F 2 "" H 3400 3000 50  0001 C CNN
F 3 "" H 3400 3000 50  0001 C CNN
	1    3400 3000
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x25_Odd_Even J3
U 1 1 5C63F576
P 6450 3000
F 0 "J3" H 6500 4300 50  0000 C CNN
F 1 "Conn_02x25_Odd_Even" H 6500 1700 50  0000 C CNN
F 2 "" H 6450 3000 50  0001 C CNN
F 3 "" H 6450 3000 50  0001 C CNN
	1    6450 3000
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x09_Odd_Even J2
U 1 1 5C63F59D
P 5050 5550
F 0 "J2" H 5100 6050 50  0000 C CNN
F 1 "Conn_02x09_Odd_Even" H 5100 5050 50  0000 C CNN
F 2 "" H 5050 5550 50  0001 C CNN
F 3 "" H 5050 5550 50  0001 C CNN
	1    5050 5550
	0    1    1    0   
$EndComp
Text Label 6050 2000 0    60   ~ 0
3V3
Text Label 5700 1800 0    60   ~ 0
GND
$Comp
L R R2
U 1 1 5C63F766
P 5950 4750
F 0 "R2" V 6030 4750 50  0000 C CNN
F 1 "10k" V 5950 4750 50  0000 C CNN
F 2 "" V 5880 4750 50  0001 C CNN
F 3 "" H 5950 4750 50  0001 C CNN
	1    5950 4750
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5C63F789
P 5750 4750
F 0 "R1" V 5830 4750 50  0000 C CNN
F 1 "10k" V 5750 4750 50  0000 C CNN
F 2 "" V 5680 4750 50  0001 C CNN
F 3 "" H 5750 4750 50  0001 C CNN
	1    5750 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 2000 5450 2000
Wire Wire Line
	5450 2000 5450 5350
Wire Wire Line
	6250 1800 5600 1800
Wire Wire Line
	5600 1800 5600 6000
Wire Wire Line
	5600 6000 5450 6000
Wire Wire Line
	5450 6000 5450 5850
Wire Wire Line
	5750 4600 5750 2000
Connection ~ 5750 2000
Wire Wire Line
	5950 4600 5950 2000
Connection ~ 5950 2000
Wire Wire Line
	5750 4900 5750 5050
Wire Wire Line
	5750 5050 5350 5050
Wire Wire Line
	5350 2600 5350 5350
Wire Wire Line
	5350 5850 5350 6200
Wire Wire Line
	5350 6200 7000 6200
Wire Wire Line
	5950 6200 5950 4900
Wire Wire Line
	6250 2600 5350 2600
Connection ~ 5350 5050
Text Label 5000 2600 0    60   ~ 0
SIOC/SCL(PB8)
Wire Wire Line
	6750 2600 7000 2600
Wire Wire Line
	7000 2600 7000 6200
Connection ~ 5950 6200
Wire Wire Line
	6750 2500 7150 2500
Wire Wire Line
	7150 2500 7150 4550
Wire Wire Line
	7150 4550 4650 4550
Wire Wire Line
	4650 4550 4650 5350
Text Label 7150 2900 0    60   ~ 0
RESET(PE1)
Text Label 7000 4800 0    60   ~ 0
SIOD/SDA(PB9)
Wire Wire Line
	6250 3900 6100 3900
Wire Wire Line
	6100 3900 6100 6350
Wire Wire Line
	6100 6350 5150 6350
Wire Wire Line
	5150 6350 5150 5850
Text Label 6100 5450 0    60   ~ 0
MCO/XCLK(PA8)
$EndSCHEMATC
