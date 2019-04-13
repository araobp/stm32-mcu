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
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "RN4020 shield"
Date "2019-04-14"
Rev ""
Comp "https://github.com/araobp"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Conn_01x05 J2
U 1 1 5CB1FE13
P 3550 4300
F 0 "J2" H 3550 4600 50  0000 C CNN
F 1 "Conn_01x05" H 3550 4000 50  0000 C CNN
F 2 "" H 3550 4300 50  0001 C CNN
F 3 "" H 3550 4300 50  0001 C CNN
	1    3550 4300
	-1   0    0    -1  
$EndComp
$Comp
L Conn_01x08 J1
U 1 1 5CB1FE4D
P 3550 3300
F 0 "J1" H 3550 3700 50  0000 C CNN
F 1 "Conn_01x08" H 3550 2800 50  0000 C CNN
F 2 "" H 3550 3300 50  0001 C CNN
F 3 "" H 3550 3300 50  0001 C CNN
	1    3550 3300
	-1   0    0    -1  
$EndComp
$Comp
L Conn_01x08 J5
U 1 1 5CB1FE93
P 6050 4100
F 0 "J5" H 6050 4500 50  0000 C CNN
F 1 "Conn_01x08" H 6050 3600 50  0000 C CNN
F 2 "" H 6050 4100 50  0001 C CNN
F 3 "" H 6050 4100 50  0001 C CNN
	1    6050 4100
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x10 J4
U 1 1 5CB1FF28
P 6050 2850
F 0 "J4" H 6050 3350 50  0000 C CNN
F 1 "Conn_01x10" H 6050 2250 50  0000 C CNN
F 2 "" H 6050 2850 50  0001 C CNN
F 3 "" H 6050 2850 50  0001 C CNN
	1    6050 2850
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x06_Female J3
U 1 1 5CB1FF5D
P 4800 5100
F 0 "J3" H 4800 5400 50  0000 C CNN
F 1 "Conn_01x06_Female" H 4800 4700 50  0000 C CNN
F 2 "" H 4800 5100 50  0001 C CNN
F 3 "" H 4800 5100 50  0001 C CNN
	1    4800 5100
	0    1    1    0   
$EndComp
Text Notes 5050 5350 1    60   ~ 0
GND
Text Notes 4850 5350 1    60   ~ 0
5V
Text Notes 4750 5350 1    60   ~ 0
RX
Text Notes 4650 5350 1    60   ~ 0
TX
Text Notes 3300 3450 0    60   ~ 0
5V
Text Notes 3300 3550 0    60   ~ 0
GND
Text Notes 6200 3400 0    60   ~ 0
TX
Text Notes 6200 4350 0    60   ~ 0
RX
Wire Wire Line
	3750 3400 4800 3400
Wire Wire Line
	4800 3400 4800 4900
Wire Wire Line
	3750 3500 5000 3500
Wire Wire Line
	5000 3500 5000 4900
Wire Wire Line
	4700 4900 4700 4300
Wire Wire Line
	4700 4300 5850 4300
Wire Wire Line
	5850 3350 4600 3350
Wire Wire Line
	4600 3350 4600 4900
NoConn ~ 3750 3000
NoConn ~ 3750 3100
NoConn ~ 3750 3200
NoConn ~ 3750 3300
NoConn ~ 3750 3600
NoConn ~ 3750 3700
NoConn ~ 3750 4100
NoConn ~ 3750 4200
NoConn ~ 3750 4300
NoConn ~ 3750 4400
NoConn ~ 3750 4500
NoConn ~ 5850 2450
NoConn ~ 5850 2550
NoConn ~ 5850 2650
NoConn ~ 5850 2750
NoConn ~ 5850 2850
NoConn ~ 5850 2950
NoConn ~ 5850 3050
NoConn ~ 5850 3150
NoConn ~ 5850 3250
NoConn ~ 5850 3800
NoConn ~ 5850 3900
NoConn ~ 5850 4000
NoConn ~ 5850 4100
NoConn ~ 5850 4200
NoConn ~ 5850 4400
NoConn ~ 5850 4500
NoConn ~ 4500 4900
NoConn ~ 4900 4900
$EndSCHEMATC
