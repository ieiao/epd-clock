EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 7
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
L Timer_RTC:PCF8563T U4
U 1 1 6123A351
P 5700 3900
F 0 "U4" H 5450 4450 50  0000 C CNN
F 1 "PCF8563T" H 5450 4350 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 5700 3900 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/PCF8563.pdf" H 5700 3900 50  0001 C CNN
	1    5700 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C21
U 1 1 61243BFA
P 5700 2750
F 0 "C21" V 5448 2750 50  0000 C CNN
F 1 "0.1uF" V 5539 2750 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5738 2600 50  0001 C CNN
F 3 "~" H 5700 2750 50  0001 C CNN
	1    5700 2750
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR051
U 1 1 61244BCB
P 5850 2750
F 0 "#PWR051" H 5850 2500 50  0001 C CNN
F 1 "GND" V 5855 2622 50  0000 R CNN
F 2 "" H 5850 2750 50  0001 C CNN
F 3 "" H 5850 2750 50  0001 C CNN
	1    5850 2750
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR048
U 1 1 6124514E
P 5550 2750
F 0 "#PWR048" H 5550 2600 50  0001 C CNN
F 1 "+3V3" V 5565 2878 50  0000 L CNN
F 2 "" H 5550 2750 50  0001 C CNN
F 3 "" H 5550 2750 50  0001 C CNN
	1    5550 2750
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR049
U 1 1 6124583B
P 5700 3500
F 0 "#PWR049" H 5700 3350 50  0001 C CNN
F 1 "+3V3" H 5715 3673 50  0000 C CNN
F 2 "" H 5700 3500 50  0001 C CNN
F 3 "" H 5700 3500 50  0001 C CNN
	1    5700 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR050
U 1 1 61245C79
P 5700 4300
F 0 "#PWR050" H 5700 4050 50  0001 C CNN
F 1 "GND" H 5705 4127 50  0000 C CNN
F 2 "" H 5700 4300 50  0001 C CNN
F 3 "" H 5700 4300 50  0001 C CNN
	1    5700 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal_Small Y1
U 1 1 61246D12
P 5100 3900
F 0 "Y1" V 5054 3812 50  0000 R CNN
F 1 "Crystal_Small" V 5145 3812 50  0000 R CNN
F 2 "Crystal:Crystal_DS26_D2.0mm_L6.0mm_Horizontal_1EP_style1" H 5100 3900 50  0001 C CNN
F 3 "~" H 5100 3900 50  0001 C CNN
	1    5100 3900
	0    -1   1    0   
$EndComp
$Comp
L Device:C C19
U 1 1 612485A7
P 4800 3700
F 0 "C19" V 4548 3700 50  0000 C CNN
F 1 "22pF" V 4639 3700 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4838 3550 50  0001 C CNN
F 3 "~" H 4800 3700 50  0001 C CNN
	1    4800 3700
	0    1    1    0   
$EndComp
$Comp
L Device:C C20
U 1 1 61248D49
P 4800 4100
F 0 "C20" V 4548 4100 50  0000 C CNN
F 1 "22pF" V 4639 4100 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4838 3950 50  0001 C CNN
F 3 "~" H 4800 4100 50  0001 C CNN
	1    4800 4100
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 3700 5100 3700
Wire Wire Line
	4950 4100 5100 4100
Wire Wire Line
	5100 4100 5100 4000
Connection ~ 5100 4100
Wire Wire Line
	5100 4100 5300 4100
Wire Wire Line
	5100 3800 5100 3700
Connection ~ 5100 3700
Wire Wire Line
	5100 3700 5300 3700
$Comp
L power:GND #PWR046
U 1 1 61249AC1
P 4650 3700
F 0 "#PWR046" H 4650 3450 50  0001 C CNN
F 1 "GND" V 4655 3572 50  0000 R CNN
F 2 "" H 4650 3700 50  0001 C CNN
F 3 "" H 4650 3700 50  0001 C CNN
	1    4650 3700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR047
U 1 1 61249FBE
P 4650 4100
F 0 "#PWR047" H 4650 3850 50  0001 C CNN
F 1 "GND" V 4655 3972 50  0000 R CNN
F 2 "" H 4650 4100 50  0001 C CNN
F 3 "" H 4650 4100 50  0001 C CNN
	1    4650 4100
	0    1    1    0   
$EndComp
Text HLabel 6100 3700 2    50   Input ~ 0
SCL
Text HLabel 6100 3800 2    50   BiDi ~ 0
SDA
Text HLabel 6550 4100 2    50   Output ~ 0
INT
NoConn ~ 6100 4000
$Comp
L Device:R R15
U 1 1 6118B5C6
P 6300 4250
F 0 "R15" H 6230 4204 50  0000 R CNN
F 1 "10K" H 6230 4295 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6230 4250 50  0001 C CNN
F 3 "~" H 6300 4250 50  0001 C CNN
	1    6300 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	6100 4100 6300 4100
Connection ~ 6300 4100
Wire Wire Line
	6300 4100 6550 4100
$Comp
L power:+3V3 #PWR052
U 1 1 6118BF3A
P 6300 4400
F 0 "#PWR052" H 6300 4250 50  0001 C CNN
F 1 "+3V3" H 6315 4573 50  0000 C CNN
F 2 "" H 6300 4400 50  0001 C CNN
F 3 "" H 6300 4400 50  0001 C CNN
	1    6300 4400
	-1   0    0    1   
$EndComp
$EndSCHEMATC
