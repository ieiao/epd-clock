EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 7
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
L Switch:SW_Push SW1
U 1 1 61231406
P 5850 2800
F 0 "SW1" H 5850 3085 50  0000 C CNN
F 1 "SW_Push" H 5850 2994 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 5850 3000 50  0001 C CNN
F 3 "~" H 5850 3000 50  0001 C CNN
	1    5850 2800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 61231B4A
P 5850 3250
F 0 "SW2" H 5850 3535 50  0000 C CNN
F 1 "SW_Push" H 5850 3444 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 5850 3450 50  0001 C CNN
F 3 "~" H 5850 3450 50  0001 C CNN
	1    5850 3250
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW3
U 1 1 612323B5
P 5850 3700
F 0 "SW3" H 5850 3985 50  0000 C CNN
F 1 "SW_Push" H 5850 3894 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 5850 3900 50  0001 C CNN
F 3 "~" H 5850 3900 50  0001 C CNN
	1    5850 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 61233949
P 5350 2800
F 0 "R10" V 5143 2800 50  0000 C CNN
F 1 "10K" V 5234 2800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5280 2800 50  0001 C CNN
F 3 "~" H 5350 2800 50  0001 C CNN
	1    5350 2800
	0    1    1    0   
$EndComp
$Comp
L Device:R R11
U 1 1 6123418A
P 5350 3250
F 0 "R11" V 5143 3250 50  0000 C CNN
F 1 "10K" V 5234 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5280 3250 50  0001 C CNN
F 3 "~" H 5350 3250 50  0001 C CNN
	1    5350 3250
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 612344CD
P 5350 3700
F 0 "R12" V 5143 3700 50  0000 C CNN
F 1 "10K" V 5234 3700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5280 3700 50  0001 C CNN
F 3 "~" H 5350 3700 50  0001 C CNN
	1    5350 3700
	0    1    1    0   
$EndComp
Wire Wire Line
	5500 2800 5600 2800
Wire Wire Line
	5500 3250 5600 3250
Wire Wire Line
	5500 3700 5600 3700
Wire Wire Line
	5050 2800 5050 3250
Wire Wire Line
	5050 3700 5200 3700
Wire Wire Line
	5200 3250 5050 3250
Connection ~ 5050 3250
Wire Wire Line
	5050 3250 5050 3700
Wire Wire Line
	5200 2800 5050 2800
Text HLabel 6050 2950 2    50   Output ~ 0
BT0
Text HLabel 6050 3400 2    50   Output ~ 0
BT1
Text HLabel 6050 3850 2    50   Output ~ 0
BT2
$Comp
L power:+3V3 #PWR046
U 1 1 610A0EA0
P 5050 2800
F 0 "#PWR046" H 5050 2650 50  0001 C CNN
F 1 "+3V3" H 5065 2973 50  0000 C CNN
F 2 "" H 5050 2800 50  0001 C CNN
F 3 "" H 5050 2800 50  0001 C CNN
	1    5050 2800
	1    0    0    -1  
$EndComp
Connection ~ 5050 2800
$Comp
L power:GND #PWR047
U 1 1 610A3FCD
P 6050 2800
F 0 "#PWR047" H 6050 2550 50  0001 C CNN
F 1 "GND" V 6055 2672 50  0000 R CNN
F 2 "" H 6050 2800 50  0001 C CNN
F 3 "" H 6050 2800 50  0001 C CNN
	1    6050 2800
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR048
U 1 1 610A4511
P 6050 3250
F 0 "#PWR048" H 6050 3000 50  0001 C CNN
F 1 "GND" V 6055 3122 50  0000 R CNN
F 2 "" H 6050 3250 50  0001 C CNN
F 3 "" H 6050 3250 50  0001 C CNN
	1    6050 3250
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR049
U 1 1 610A4989
P 6050 3700
F 0 "#PWR049" H 6050 3450 50  0001 C CNN
F 1 "GND" V 6055 3572 50  0000 R CNN
F 2 "" H 6050 3700 50  0001 C CNN
F 3 "" H 6050 3700 50  0001 C CNN
	1    6050 3700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5600 2800 5600 2950
Wire Wire Line
	5600 2950 6050 2950
Connection ~ 5600 2800
Wire Wire Line
	5600 2800 5650 2800
Wire Wire Line
	5600 3250 5600 3400
Wire Wire Line
	5600 3400 6050 3400
Connection ~ 5600 3250
Wire Wire Line
	5600 3250 5650 3250
Wire Wire Line
	5600 3700 5600 3850
Wire Wire Line
	5600 3850 6050 3850
Connection ~ 5600 3700
Wire Wire Line
	5600 3700 5650 3700
$EndSCHEMATC
