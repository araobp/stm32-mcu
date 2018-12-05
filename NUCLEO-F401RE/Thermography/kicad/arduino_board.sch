EESchema Schematic File Version 2
LIBS:arduino_board-rescue
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
LIBS:arduino_board-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Arduino shield for infrared array sensor AMG8833"
Date "2018-12-04"
Rev ""
Comp "https://github.com/araobp"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Conn_01x06_Male J2
U 1 1 5C06642E
P 4250 5500
F 0 "J2" H 4250 5800 50  0000 C CNN
F 1 "Conn_01x06_Male" H 4250 5100 50  0000 C CNN
F 2 "" H 4250 5500 50  0001 C CNN
F 3 "" H 4250 5500 50  0001 C CNN
	1    4250 5500
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x08_Male J1
U 1 1 5C066455
P 4250 3400
F 0 "J1" H 4250 3800 50  0000 C CNN
F 1 "Conn_01x08_Male" H 4250 2900 50  0000 C CNN
F 2 "" H 4250 3400 50  0001 C CNN
F 3 "" H 4250 3400 50  0001 C CNN
	1    4250 3400
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x08_Male J4
U 1 1 5C066474
P 7400 5450
F 0 "J4" H 7400 5850 50  0000 C CNN
F 1 "Conn_01x08_Male" H 7400 4950 50  0000 C CNN
F 2 "" H 7400 5450 50  0001 C CNN
F 3 "" H 7400 5450 50  0001 C CNN
	1    7400 5450
	-1   0    0    -1  
$EndComp
$Comp
L Conn_01x10_Male J3
U 1 1 5C066495
P 7400 3200
F 0 "J3" H 7400 3700 50  0000 C CNN
F 1 "Conn_01x10_Male" H 7400 2600 50  0000 C CNN
F 2 "" H 7400 3200 50  0001 C CNN
F 3 "" H 7400 3200 50  0001 C CNN
	1    7400 3200
	-1   0    0    -1  
$EndComp
NoConn ~ 6150 4500
Wire Wire Line
	4450 3400 6900 3400
Wire Wire Line
	6900 3400 6900 4700
Wire Wire Line
	6900 4700 6150 4700
Wire Wire Line
	4450 3600 6800 3600
Wire Wire Line
	6800 3600 6800 4600
Wire Wire Line
	6800 4600 6150 4600
Wire Wire Line
	4700 1550 4700 3400
Connection ~ 4700 3400
Wire Wire Line
	6600 2800 6600 4300
Wire Wire Line
	6600 4300 6150 4300
Wire Wire Line
	6500 2900 7200 2900
Wire Wire Line
	6500 2900 6500 4400
Wire Wire Line
	6500 4400 6150 4400
NoConn ~ 7200 3000
NoConn ~ 7200 3100
NoConn ~ 7200 3200
NoConn ~ 7200 3300
NoConn ~ 7200 3400
NoConn ~ 7200 3500
NoConn ~ 7200 3600
NoConn ~ 7200 3700
NoConn ~ 4450 3100
NoConn ~ 4450 3200
NoConn ~ 4450 3300
NoConn ~ 4450 3500
NoConn ~ 4450 3700
NoConn ~ 4450 3800
NoConn ~ 4450 5300
NoConn ~ 4450 5400
NoConn ~ 4450 5500
NoConn ~ 4450 5600
NoConn ~ 4450 5700
NoConn ~ 4450 5800
Text Notes 4000 3450 0    60   ~ 0
3.3V
Text Notes 4000 3650 0    60   ~ 0
GND
Text Notes 7500 2850 0    60   ~ 0
SCL
Text Notes 7500 2950 0    60   ~ 0
SDA
$Comp
L GND #PWR01
U 1 1 5C066FE4
P 4700 5000
F 0 "#PWR01" H 4700 4750 50  0001 C CNN
F 1 "GND" H 4700 4850 50  0000 C CNN
F 2 "" H 4700 5000 50  0001 C CNN
F 3 "" H 4700 5000 50  0001 C CNN
	1    4700 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3600 4700 5000
Connection ~ 4700 3600
$Comp
L +3.3V #PWR02
U 1 1 5C06703B
P 4700 1550
F 0 "#PWR02" H 4700 1400 50  0001 C CNN
F 1 "+3.3V" H 4700 1690 50  0000 C CNN
F 2 "" H 4700 1550 50  0001 C CNN
F 3 "" H 4700 1550 50  0001 C CNN
	1    4700 1550
	1    0    0    -1  
$EndComp
NoConn ~ 7200 5150
NoConn ~ 7200 5250
NoConn ~ 7200 5350
NoConn ~ 7200 5450
NoConn ~ 7200 5550
NoConn ~ 7200 5650
NoConn ~ 7200 5750
NoConn ~ 7200 5850
$Comp
L PWR_FLAG #FLG03
U 1 1 5C067422
P 4950 1850
F 0 "#FLG03" H 4950 1925 50  0001 C CNN
F 1 "PWR_FLAG" H 4950 2000 50  0000 C CNN
F 2 "" H 4950 1850 50  0001 C CNN
F 3 "" H 4950 1850 50  0001 C CNN
	1    4950 1850
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG04
U 1 1 5C067444
P 4950 4650
F 0 "#FLG04" H 4950 4725 50  0001 C CNN
F 1 "PWR_FLAG" H 4950 4800 50  0000 C CNN
F 2 "" H 4950 4650 50  0001 C CNN
F 3 "" H 4950 4650 50  0001 C CNN
	1    4950 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 1850 4950 2050
Wire Wire Line
	4950 2050 4700 2050
Connection ~ 4700 2050
Wire Wire Line
	4950 4650 4950 4800
Wire Wire Line
	4950 4800 4700 4800
Connection ~ 4700 4800
$Comp
L Conta_thermography_AMG8833 U2
U 1 1 5C0670D6
P 5600 4500
F 0 "U2" H 5450 4100 60  0000 C CNN
F 1 "Conta_thermography_AMG8833" H 5600 4900 60  0000 C CNN
F 2 "" H 5700 4500 60  0001 C CNN
F 3 "" H 5700 4500 60  0001 C CNN
	1    5600 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2800 7200 2800
$Comp
L R R?
U 1 1 5C06EFF4
P 7000 2500
F 0 "R?" V 7080 2500 50  0000 C CNN
F 1 "10k" V 7000 2500 50  0000 C CNN
F 2 "" V 6930 2500 50  0001 C CNN
F 3 "" H 7000 2500 50  0001 C CNN
	1    7000 2500
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5C06F2F1
P 6800 2500
F 0 "R?" V 6880 2500 50  0000 C CNN
F 1 "10k" V 6800 2500 50  0000 C CNN
F 2 "" V 6730 2500 50  0001 C CNN
F 3 "" H 6800 2500 50  0001 C CNN
	1    6800 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2200 7000 2200
Wire Wire Line
	7000 2200 7000 2350
Connection ~ 4700 2200
Wire Wire Line
	6800 2350 6800 2200
Connection ~ 6800 2200
Wire Wire Line
	6800 2650 6800 2900
Connection ~ 6800 2900
Wire Wire Line
	7000 2650 7000 2800
Connection ~ 7000 2800
$EndSCHEMATC
