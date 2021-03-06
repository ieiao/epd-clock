EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Memory_EEPROM:AT25xxx U3
U 1 1 6122B52B
P 6000 3400
F 0 "U3" H 5800 3700 50  0000 C CNN
F 1 "AT25xxx" H 6200 3650 50  0000 C CNN
F 2 "Package_SO:SOIC-8_5.275x5.275mm_P1.27mm" H 6000 3400 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8707-SEEPROM-AT25010B-020B-040B-Datasheet.pdf" H 6000 3400 50  0001 C CNN
	1    6000 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR039
U 1 1 6122BC9D
P 6000 3700
F 0 "#PWR039" H 6000 3450 50  0001 C CNN
F 1 "GND" H 6005 3527 50  0000 C CNN
F 2 "" H 6000 3700 50  0001 C CNN
F 3 "" H 6000 3700 50  0001 C CNN
	1    6000 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR038
U 1 1 6122C087
P 6000 3000
F 0 "#PWR038" H 6000 2850 50  0001 C CNN
F 1 "+3V3" H 6015 3173 50  0000 C CNN
F 2 "" H 6000 3000 50  0001 C CNN
F 3 "" H 6000 3000 50  0001 C CNN
	1    6000 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3000 6000 3100
Connection ~ 6000 3000
Text HLabel 6400 3300 2    50   Input ~ 0
SCK
Text HLabel 6400 3400 2    50   Input ~ 0
MOSI
Text HLabel 6400 3500 2    50   Output ~ 0
MISO
Text HLabel 5600 3500 0    50   Input ~ 0
CS
$Comp
L Device:C C18
U 1 1 6123AFE1
P 7000 3400
F 0 "C18" H 7115 3446 50  0000 L CNN
F 1 "0.1uF" H 7115 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7038 3250 50  0001 C CNN
F 3 "~" H 7000 3400 50  0001 C CNN
	1    7000 3400
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR040
U 1 1 6123B32A
P 7000 3250
F 0 "#PWR040" H 7000 3100 50  0001 C CNN
F 1 "+3V3" H 7015 3423 50  0000 C CNN
F 2 "" H 7000 3250 50  0001 C CNN
F 3 "" H 7000 3250 50  0001 C CNN
	1    7000 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR041
U 1 1 6123B7F6
P 7000 3650
F 0 "#PWR041" H 7000 3400 50  0001 C CNN
F 1 "GND" H 7005 3477 50  0000 C CNN
F 2 "" H 7000 3650 50  0001 C CNN
F 3 "" H 7000 3650 50  0001 C CNN
	1    7000 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3650 7000 3550
$Comp
L Device:R R10
U 1 1 610BAE93
P 5150 3300
F 0 "R10" V 5050 3200 50  0000 C CNN
F 1 "10K" V 5050 3400 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5080 3300 50  0001 C CNN
F 3 "~" H 5150 3300 50  0001 C CNN
	1    5150 3300
	0    1    1    0   
$EndComp
$Comp
L Device:R R11
U 1 1 610BB93F
P 5150 3400
F 0 "R11" V 5250 3300 50  0000 C CNN
F 1 "10K" V 5250 3500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5080 3400 50  0001 C CNN
F 3 "~" H 5150 3400 50  0001 C CNN
	1    5150 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	5300 3300 5600 3300
Wire Wire Line
	5600 3400 5300 3400
Wire Wire Line
	5000 3400 5000 3300
Wire Wire Line
	5000 3000 6000 3000
Connection ~ 5000 3300
Wire Wire Line
	5000 3300 5000 3000
NoConn ~ 5400 -250
$EndSCHEMATC
