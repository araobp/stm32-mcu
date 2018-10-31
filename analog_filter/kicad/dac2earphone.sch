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
LIBS:mcu
LIBS:dac2earphone-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "DAC to earphone adaptor"
Date "2018-10-02"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Conn_01x03_Male J1
U 1 1 5BB2C91B
P 3600 3250
F 0 "J1" H 3600 3450 50  0000 C CNN
F 1 "Conn_01x03_Male" H 3600 3050 50  0000 C CNN
F 2 "mcu:Pin_Header_3P" H 3600 3250 50  0001 C CNN
F 3 "" H 3600 3250 50  0001 C CNN
	1    3600 3250
	1    0    0    -1  
$EndComp
Text Label 3500 3200 0    60   ~ 0
L
Text Label 3500 3300 0    60   ~ 0
R
Text Label 3400 3400 0    60   ~ 0
GND
$Comp
L R R1
U 1 1 5BB2CB9F
P 5200 3150
F 0 "R1" V 5280 3150 50  0000 C CNN
F 1 "120" V 5200 3150 50  0000 C CNN
F 2 "mcu:Register" V 5130 3150 50  0001 C CNN
F 3 "" H 5200 3150 50  0001 C CNN
	1    5200 3150
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 5BB2CBCE
P 5200 3350
F 0 "R2" V 5280 3350 50  0000 C CNN
F 1 "120" V 5200 3350 50  0000 C CNN
F 2 "mcu:Register" V 5130 3350 50  0001 C CNN
F 3 "" H 5200 3350 50  0001 C CNN
	1    5200 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 3150 5050 3150
Wire Wire Line
	3800 3250 4650 3250
Wire Wire Line
	4650 3250 4650 3350
Wire Wire Line
	4650 3350 5050 3350
$Comp
L C_Small C1
U 1 1 5BB2CBF7
P 5650 3750
F 0 "C1" H 5660 3820 50  0000 L CNN
F 1 "0.1u" H 5660 3670 50  0000 L CNN
F 2 "mcu:Capacitor" H 5650 3750 50  0001 C CNN
F 3 "" H 5650 3750 50  0001 C CNN
	1    5650 3750
	1    0    0    -1  
$EndComp
$Comp
L C_Small C2
U 1 1 5BB2CC28
P 6100 3750
F 0 "C2" H 6110 3820 50  0000 L CNN
F 1 "0.1u" H 6110 3670 50  0000 L CNN
F 2 "mcu:Capacitor" H 6100 3750 50  0001 C CNN
F 3 "" H 6100 3750 50  0001 C CNN
	1    6100 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3350 6300 3350
Wire Wire Line
	5650 3350 5650 3650
Wire Wire Line
	5350 3150 6300 3150
Wire Wire Line
	6100 3150 6100 3650
$Comp
L GND #PWR01
U 1 1 5BB2CC91
P 5650 4350
F 0 "#PWR01" H 5650 4100 50  0001 C CNN
F 1 "GND" H 5650 4200 50  0000 C CNN
F 2 "" H 5650 4350 50  0001 C CNN
F 3 "" H 5650 4350 50  0001 C CNN
	1    5650 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 3850 5650 4350
Wire Wire Line
	6100 3850 6100 4200
Wire Wire Line
	6100 4200 5650 4200
Connection ~ 5650 4200
$Comp
L C C3
U 1 1 5BB2CCDA
P 6450 3150
F 0 "C3" H 6350 3250 50  0000 L CNN
F 1 "220u" H 6250 3050 50  0000 L CNN
F 2 "mcu:audio_capacitor" H 6488 3000 50  0001 C CNN
F 3 "" H 6450 3150 50  0001 C CNN
	1    6450 3150
	0    1    1    0   
$EndComp
$Comp
L C C4
U 1 1 5BB2CD25
P 6450 3350
F 0 "C4" H 6475 3450 50  0000 L CNN
F 1 "220u" H 6475 3250 50  0000 L CNN
F 2 "mcu:audio_capacitor" H 6488 3200 50  0001 C CNN
F 3 "" H 6450 3350 50  0001 C CNN
	1    6450 3350
	0    1    1    0   
$EndComp
Connection ~ 6100 3150
Connection ~ 5650 3350
Wire Wire Line
	6600 3350 6900 3350
Wire Wire Line
	6600 3150 7350 3150
$Comp
L GND #PWR02
U 1 1 5BB2CDFD
P 7150 4350
F 0 "#PWR02" H 7150 4100 50  0001 C CNN
F 1 "GND" H 7150 4200 50  0000 C CNN
F 2 "" H 7150 4350 50  0001 C CNN
F 3 "" H 7150 4350 50  0001 C CNN
	1    7150 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 5BB2CE5F
P 4350 4350
F 0 "#PWR03" H 4350 4100 50  0001 C CNN
F 1 "GND" H 4350 4200 50  0000 C CNN
F 2 "" H 4350 4350 50  0001 C CNN
F 3 "" H 4350 4350 50  0001 C CNN
	1    4350 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 3350 4350 3350
Wire Wire Line
	4350 3350 4350 4350
$Comp
L Conn_01x03_Male J2
U 1 1 5BB2CEB5
P 7550 3250
F 0 "J2" H 7550 3450 50  0000 C CNN
F 1 "Conn_01x03_Male" H 7550 3050 50  0000 C CNN
F 2 "mcu:audio_jack_3.5mm" H 7550 3250 50  0001 C CNN
F 3 "" H 7550 3250 50  0001 C CNN
	1    7550 3250
	-1   0    0    1   
$EndComp
Wire Wire Line
	7350 3250 6900 3250
Wire Wire Line
	6900 3250 6900 3350
Wire Wire Line
	7350 3350 7150 3350
Wire Wire Line
	7150 3350 7150 4350
Text Label 7600 3200 0    60   ~ 0
L
Text Label 7600 3300 0    60   ~ 0
R
Text Label 7600 3400 0    60   ~ 0
GND
Text Notes 7900 3250 0    60   ~ 0
earphone with 18 ohm impedance
Text Notes 2550 3300 0    60   ~ 0
DAC on STM32L4
$Comp
L PWR_FLAG #FLG04
U 1 1 5BB2D51D
P 4050 2650
F 0 "#FLG04" H 4050 2725 50  0001 C CNN
F 1 "PWR_FLAG" H 4050 2800 50  0000 C CNN
F 2 "" H 4050 2650 50  0001 C CNN
F 3 "" H 4050 2650 50  0001 C CNN
	1    4050 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 2650 4050 3150
Connection ~ 4050 3150
$Comp
L PWR_FLAG #FLG05
U 1 1 5BB2D763
P 4450 2650
F 0 "#FLG05" H 4450 2725 50  0001 C CNN
F 1 "PWR_FLAG" H 4450 2800 50  0000 C CNN
F 2 "" H 4450 2650 50  0001 C CNN
F 3 "" H 4450 2650 50  0001 C CNN
	1    4450 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2650 4450 3250
Connection ~ 4450 3250
$EndSCHEMATC
