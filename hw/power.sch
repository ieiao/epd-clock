EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5000 3500 5250 3500
Wire Wire Line
	5000 3600 5000 3500
Wire Wire Line
	5750 3500 5750 3600
Wire Wire Line
	5550 3500 5750 3500
Connection ~ 6500 4150
Wire Wire Line
	6500 4150 6800 4150
Connection ~ 6100 4150
Wire Wire Line
	6500 4150 6500 3900
Wire Wire Line
	6100 4150 6500 4150
Wire Wire Line
	6500 3500 6500 3600
Wire Wire Line
	5850 3500 6500 3500
Wire Wire Line
	5850 3600 5850 3500
Connection ~ 5000 4150
Wire Wire Line
	6100 4150 5000 4150
Wire Wire Line
	6100 3600 6100 4150
Wire Wire Line
	5950 3600 6100 3600
Wire Wire Line
	5000 4150 5000 3900
$Comp
L Device:C C?
U 1 1 60F0A28F
P 6500 3750
AR Path="/60F0A28F" Ref="C?"  Part="1" 
AR Path="/60F00547/60F0A28F" Ref="C2"  Part="1" 
F 0 "C2" H 6615 3796 50  0000 L CNN
F 1 "100uF" H 6615 3705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6538 3600 50  0001 C CNN
F 3 "~" H 6500 3750 50  0001 C CNN
	1    6500 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:L L?
U 1 1 60F0A295
P 5400 3500
AR Path="/60F0A295" Ref="L?"  Part="1" 
AR Path="/60F00547/60F0A295" Ref="L1"  Part="1" 
F 0 "L1" V 5500 3500 50  0000 C CNN
F 1 "22uH" V 5300 3500 50  0000 C CNN
F 2 "Inductor_SMD:L_Coilcraft_XxL4020" H 5400 3500 50  0001 C CNN
F 3 "~" H 5400 3500 50  0001 C CNN
	1    5400 3500
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C?
U 1 1 60F0A29B
P 5000 3750
AR Path="/60F0A29B" Ref="C?"  Part="1" 
AR Path="/60F00547/60F0A29B" Ref="C1"  Part="1" 
F 0 "C1" H 5115 3796 50  0000 L CNN
F 1 "10uF" H 5115 3705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5038 3600 50  0001 C CNN
F 3 "~" H 5000 3750 50  0001 C CNN
	1    5000 3750
	1    0    0    -1  
$EndComp
$Comp
L custom:ME2188AXXPG U?
U 1 1 60F0A2A7
P 6050 3700
AR Path="/60F0A2A7" Ref="U?"  Part="1" 
AR Path="/60F00547/60F0A2A7" Ref="U1"  Part="1" 
F 0 "U1" V 6100 3150 50  0000 L CNN
F 1 "ME2188AXXPG" V 6400 3150 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-89-3" H 6050 3700 50  0001 C CNN
F 3 "" H 6050 3700 50  0001 C CNN
	1    6050 3700
	0    1    1    0   
$EndComp
$Comp
L Device:Q_PMOS_GSD Q1
U 1 1 60FD7D74
P 5300 3100
F 0 "Q1" V 5642 3100 50  0000 C CNN
F 1 "Q_PMOS_GSD" V 5551 3100 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5500 3200 50  0001 C CNN
F 3 "~" H 5300 3100 50  0001 C CNN
	1    5300 3100
	0    1    -1   0   
$EndComp
Wire Wire Line
	5100 3000 4550 3000
Wire Wire Line
	4550 3000 4550 3500
$Comp
L Device:R R2
U 1 1 60FDE5DB
P 5950 3000
F 0 "R2" V 5743 3000 50  0000 C CNN
F 1 "10K" V 5834 3000 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5880 3000 50  0001 C CNN
F 3 "~" H 5950 3000 50  0001 C CNN
	1    5950 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	5500 3000 5800 3000
Wire Wire Line
	6100 3000 6800 3000
Wire Wire Line
	5300 3300 6800 3300
Text HLabel 6800 3000 2    50   Input ~ 0
Vbat
Text HLabel 6800 3300 2    50   Input ~ 0
ADEN#
$Comp
L power:GND #PWR03
U 1 1 6113514D
P 6800 4150
F 0 "#PWR03" H 6800 3900 50  0001 C CNN
F 1 "GND" V 6805 4022 50  0000 R CNN
F 2 "" H 6800 4150 50  0001 C CNN
F 3 "" H 6800 4150 50  0001 C CNN
	1    6800 4150
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR02
U 1 1 6113576C
P 6800 3500
F 0 "#PWR02" H 6800 3350 50  0001 C CNN
F 1 "+3V3" V 6815 3628 50  0000 L CNN
F 2 "" H 6800 3500 50  0001 C CNN
F 3 "" H 6800 3500 50  0001 C CNN
	1    6800 3500
	0    1    1    0   
$EndComp
$Comp
L Device:Battery_Cell BT1
U 1 1 61256DF8
P 4200 3850
F 0 "BT1" H 4318 3946 50  0000 L CNN
F 1 "Battery_Cell" H 4318 3855 50  0000 L CNN
F 2 "custom:AAA_holder_smt" V 4200 3910 50  0001 C CNN
F 3 "~" V 4200 3910 50  0001 C CNN
	1    4200 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3500 4200 3650
Wire Wire Line
	4200 3950 4200 4150
Wire Wire Line
	4200 4150 5000 4150
$Comp
L power:PWR_FLAG #FLG01
U 1 1 61015ECE
P 5750 3500
F 0 "#FLG01" H 5750 3575 50  0001 C CNN
F 1 "PWR_FLAG" H 5750 3673 50  0000 C CNN
F 2 "" H 5750 3500 50  0001 C CNN
F 3 "~" H 5750 3500 50  0001 C CNN
	1    5750 3500
	1    0    0    -1  
$EndComp
Connection ~ 5750 3500
$Comp
L power:PWR_FLAG #FLG02
U 1 1 61018209
P 6500 4150
F 0 "#FLG02" H 6500 4225 50  0001 C CNN
F 1 "PWR_FLAG" H 6500 4323 50  0000 C CNN
F 2 "" H 6500 4150 50  0001 C CNN
F 3 "~" H 6500 4150 50  0001 C CNN
	1    6500 4150
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 610A5E35
P 4350 3500
F 0 "R1" V 4250 3500 50  0000 C CNN
F 1 "0" V 4450 3500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4280 3500 50  0001 C CNN
F 3 "~" H 4350 3500 50  0001 C CNN
	1    4350 3500
	0    1    1    0   
$EndComp
Connection ~ 5000 3500
Wire Wire Line
	4550 3500 5000 3500
Wire Wire Line
	4500 3500 4550 3500
Connection ~ 4550 3500
$Comp
L Device:R R3
U 1 1 610C7213
P 6650 3500
F 0 "R3" V 6550 3500 50  0000 C CNN
F 1 "0" V 6750 3500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6580 3500 50  0001 C CNN
F 3 "~" H 6650 3500 50  0001 C CNN
	1    6650 3500
	0    1    1    0   
$EndComp
Connection ~ 6500 3500
$EndSCHEMATC