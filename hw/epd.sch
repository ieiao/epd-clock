EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 7
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
L Device:C C15
U 1 1 60F15CC6
P 8000 2950
F 0 "C15" H 8115 2996 50  0000 L CNN
F 1 "1uF/6V" H 8000 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8038 2800 50  0001 C CNN
F 3 "~" H 8000 2950 50  0001 C CNN
	1    8000 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C14
U 1 1 60F15EE3
P 7650 2950
F 0 "C14" H 7765 2996 50  0000 L CNN
F 1 "1uF/25V" H 7650 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7688 2800 50  0001 C CNN
F 3 "~" H 7650 2950 50  0001 C CNN
	1    7650 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C13
U 1 1 60F1777E
P 7300 2950
F 0 "C13" H 7415 2996 50  0000 L CNN
F 1 "1uF/25V" H 7300 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7338 2800 50  0001 C CNN
F 3 "~" H 7300 2950 50  0001 C CNN
	1    7300 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 60F17B52
P 6600 2950
F 0 "C8" H 6715 2996 50  0000 L CNN
F 1 "1uF/25V" H 6600 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6638 2800 50  0001 C CNN
F 3 "~" H 6600 2950 50  0001 C CNN
	1    6600 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 60F17D87
P 5800 2950
F 0 "C4" H 5915 2996 50  0000 L CNN
F 1 "1uF/25V" H 5800 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5838 2800 50  0001 C CNN
F 3 "~" H 5800 2950 50  0001 C CNN
	1    5800 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 60F180EC
P 6250 2200
F 0 "C7" H 6365 2246 50  0000 L CNN
F 1 "1uF/25V" H 6250 2100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6288 2050 50  0001 C CNN
F 3 "~" H 6250 2200 50  0001 C CNN
	1    6250 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 60F183B2
P 6200 2950
F 0 "C5" H 6315 2996 50  0000 L CNN
F 1 "1uF/25V" H 6200 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6238 2800 50  0001 C CNN
F 3 "~" H 6200 2950 50  0001 C CNN
	1    6200 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 60F2B86E
P 7650 3100
F 0 "#PWR024" H 7650 2850 50  0001 C CNN
F 1 "GND" H 7700 2950 50  0000 C CNN
F 2 "" H 7650 3100 50  0001 C CNN
F 3 "" H 7650 3100 50  0001 C CNN
	1    7650 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 60F2CDD7
P 5800 3100
F 0 "#PWR09" H 5800 2850 50  0001 C CNN
F 1 "GND" H 5850 2950 50  0000 C CNN
F 2 "" H 5800 3100 50  0001 C CNN
F 3 "" H 5800 3100 50  0001 C CNN
	1    5800 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 60F2D200
P 6200 3100
F 0 "#PWR010" H 6200 2850 50  0001 C CNN
F 1 "GND" H 6250 2950 50  0000 C CNN
F 2 "" H 6200 3100 50  0001 C CNN
F 3 "" H 6200 3100 50  0001 C CNN
	1    6200 3100
	1    0    0    -1  
$EndComp
Text Label 8000 2800 0    50   ~ 0
VCOM
Text Label 7650 2800 0    50   ~ 0
VSL
Text Label 7300 2800 0    50   ~ 0
VSH
Text Label 6600 2800 0    50   ~ 0
VDD
Text Label 6250 2050 0    50   ~ 0
VCI
Text Label 5800 2800 0    50   ~ 0
VGH
Text Label 6200 2800 0    50   ~ 0
VGL
$Comp
L Device:R R4
U 1 1 60F42FB6
P 6950 3650
F 0 "R4" V 6850 3650 50  0000 C CNN
F 1 "4.7K" V 7050 3650 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6880 3650 50  0001 C CNN
F 3 "~" H 6950 3650 50  0001 C CNN
	1    6950 3650
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 60F4F067
P 7750 3650
F 0 "R5" V 7650 3650 50  0000 C CNN
F 1 "4.7K" V 7850 3650 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7680 3650 50  0001 C CNN
F 3 "~" H 7750 3650 50  0001 C CNN
	1    7750 3650
	0    1    1    0   
$EndComp
Text Label 6800 3650 2    50   ~ 0
TSDA
Text Label 7600 3650 2    50   ~ 0
TSCL
$Comp
L Device:C C12
U 1 1 60F7F81D
P 7150 2200
F 0 "C12" H 7250 2250 50  0000 L CNN
F 1 "1uF/50V" H 7150 2100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7188 2050 50  0001 C CNN
F 3 "~" H 7150 2200 50  0001 C CNN
	1    7150 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D2
U 1 1 60F82627
P 7250 4350
F 0 "D2" H 7300 4450 50  0000 R CNN
F 1 "MBR0530" H 7500 4250 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-123" H 7250 4350 50  0001 C CNN
F 3 "~" H 7250 4350 50  0001 C CNN
	1    7250 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D1
U 1 1 60F82F8B
P 7250 4050
F 0 "D1" H 7250 3950 50  0000 C CNN
F 1 "MBR0530" H 7200 4150 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 7250 4050 50  0001 C CNN
F 3 "~" H 7250 4050 50  0001 C CNN
	1    7250 4050
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Schottky D3
U 1 1 60F83561
P 7250 4600
F 0 "D3" H 7350 4550 50  0000 L CNN
F 1 "MBR0530" H 7000 4700 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-123" H 7250 4600 50  0001 C CNN
F 3 "~" H 7250 4600 50  0001 C CNN
	1    7250 4600
	-1   0    0    1   
$EndComp
$Comp
L Device:C C10
U 1 1 60F8D060
P 6850 4400
F 0 "C10" H 7000 4350 50  0000 C CNN
F 1 "4.7uF/50V" H 7150 4450 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6888 4250 50  0001 C CNN
F 3 "~" H 6850 4400 50  0001 C CNN
	1    6850 4400
	-1   0    0    1   
$EndComp
$Comp
L Device:C C9
U 1 1 60F80D7C
P 6700 2200
F 0 "C9" H 6800 2250 50  0000 L CNN
F 1 "1uF/50V" H 6700 2100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6738 2050 50  0001 C CNN
F 3 "~" H 6700 2200 50  0001 C CNN
	1    6700 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:L L2
U 1 1 60F9E8E4
P 6550 4600
F 0 "L2" V 6500 4550 50  0000 L CNN
F 1 "10uH" V 6650 4500 50  0000 L CNN
F 2 "Inductor_SMD:L_Coilcraft_XxL4020" H 6550 4600 50  0001 C CNN
F 3 "~" H 6550 4600 50  0001 C CNN
	1    6550 4600
	0    1    1    0   
$EndComp
$Comp
L Device:C C6
U 1 1 60F9FCFD
P 6200 4850
F 0 "C6" H 6050 4800 50  0000 C CNN
F 1 "10uF/6V" H 5950 4900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6238 4700 50  0001 C CNN
F 3 "~" H 6200 4850 50  0001 C CNN
	1    6200 4850
	-1   0    0    1   
$EndComp
$Comp
L Device:Q_NMOS_GSD Q2
U 1 1 60FA58A2
P 6750 5000
F 0 "Q2" H 7000 5050 50  0000 C CNN
F 1 "IRLML6344" H 7150 4950 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 6950 5100 50  0001 C CNN
F 3 "~" H 6750 5000 50  0001 C CNN
	1    6750 5000
	1    0    0    -1  
$EndComp
Text Label 4800 5150 0    50   ~ 0
NC
Text Label 4800 5050 0    50   ~ 0
GDR
Text Label 4800 4850 0    50   ~ 0
VGL
Text Label 4800 4750 0    50   ~ 0
VGH
Text Label 4800 4650 0    50   ~ 0
TSCL
Text Label 4800 4550 0    50   ~ 0
TSDA
Text Label 4800 4450 0    50   ~ 0
BS1
Text Label 4800 4350 0    50   ~ 0
BUSY
Text Label 4800 4250 0    50   ~ 0
RST#
Text Label 4800 4150 0    50   ~ 0
DC#
Text Label 4800 4050 0    50   ~ 0
CS#
Text Label 4800 3950 0    50   ~ 0
CLK
Text Label 4800 3850 0    50   ~ 0
DIN
Text Label 4800 3750 0    50   ~ 0
VDDIO
Text Label 4800 3650 0    50   ~ 0
VCI
Text Label 4800 4950 0    50   ~ 0
RESE
Text Label 4800 3450 0    50   ~ 0
VDD
Text Label 4800 3350 0    50   ~ 0
VPP
Text Label 4800 3250 0    50   ~ 0
VSH
Text Label 4800 3150 0    50   ~ 0
PREVGH
Text Label 4800 3050 0    50   ~ 0
VSL
Text Label 4800 2950 0    50   ~ 0
PREVGL
Text Label 4800 2850 0    50   ~ 0
VCOM
$Comp
L Connector_Generic_MountingPin:Conn_01x24_MountingPin J1
U 1 1 60FDC045
P 4600 3950
F 0 "J1" H 4522 5267 50  0000 C CNN
F 1 "Conn_01x24_MountingPin" H 4522 5176 50  0000 C CNN
F 2 "Connector_FFC-FPC:TE_2-1734839-4_1x24-1MP_P0.5mm_Horizontal" H 4600 3950 50  0001 C CNN
F 3 "~" H 4600 3950 50  0001 C CNN
	1    4600 3950
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 60FE3581
P 4600 5350
F 0 "#PWR05" H 4600 5100 50  0001 C CNN
F 1 "GND" H 4605 5177 50  0000 C CNN
F 2 "" H 4600 5350 50  0001 C CNN
F 3 "" H 4600 5350 50  0001 C CNN
	1    4600 5350
	1    0    0    -1  
$EndComp
NoConn ~ 4800 5150
Text Label 5950 3650 2    50   ~ 0
BS1
Text Notes 5950 3800 0    50   ~ 0
4 wire spi
Text Label 7400 4350 0    50   ~ 0
PREVGL
Text Label 7400 4600 0    50   ~ 0
PREVGH
Text Notes 4300 5100 0    50   ~ 0
GDR
Text Notes 4300 5000 0    50   ~ 0
RESE
Text Notes 4300 4900 0    50   ~ 0
VGL
Text Notes 4300 4800 0    50   ~ 0
VGH
Text Notes 4300 4700 0    50   ~ 0
TSCL
Text Notes 4300 4600 0    50   ~ 0
TSDA
Text Notes 4300 4500 0    50   ~ 0
BS1
Text Notes 4300 4400 0    50   ~ 0
BUSY
Text Notes 4300 4300 0    50   ~ 0
RST#
Text Notes 4300 4200 0    50   ~ 0
DC#
Text Notes 4300 4100 0    50   ~ 0
CS#
Text Notes 4300 4000 0    50   ~ 0
CLK
Text Notes 4300 3900 0    50   ~ 0
DIN
Text Notes 4300 3800 0    50   ~ 0
VDDIO
Text Notes 4300 3700 0    50   ~ 0
VCI
Text Notes 4300 3600 0    50   ~ 0
GND
Text Notes 4300 3500 0    50   ~ 0
VDD
Text Notes 4300 3400 0    50   ~ 0
VPP
Text Notes 4300 3300 0    50   ~ 0
VSH
Text Notes 4250 3200 0    50   ~ 0
PREVGH
Text Notes 4300 3100 0    50   ~ 0
VSL
Text Notes 4250 3000 0    50   ~ 0
PREVGL
Text Notes 4300 2900 0    50   ~ 0
VCOM
$Comp
L Device:C C3
U 1 1 61059A0B
P 5800 2200
F 0 "C3" H 5915 2246 50  0000 L CNN
F 1 "0.1uF/25V" H 5800 2100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5838 2050 50  0001 C CNN
F 3 "~" H 5800 2200 50  0001 C CNN
	1    5800 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 6105A8A0
P 5800 2350
F 0 "#PWR08" H 5800 2100 50  0001 C CNN
F 1 "GND" H 5850 2200 50  0000 C CNN
F 2 "" H 5800 2350 50  0001 C CNN
F 3 "" H 5800 2350 50  0001 C CNN
	1    5800 2350
	1    0    0    -1  
$EndComp
Text Label 5800 2050 0    50   ~ 0
VDDIO
Text Label 6950 2800 0    50   ~ 0
VPP
$Comp
L power:GND #PWR019
U 1 1 6102F49A
P 6950 3100
F 0 "#PWR019" H 6950 2850 50  0001 C CNN
F 1 "GND" H 7000 2950 50  0000 C CNN
F 2 "" H 6950 3100 50  0001 C CNN
F 3 "" H 6950 3100 50  0001 C CNN
	1    6950 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 6102F494
P 6950 2950
F 0 "C11" H 7065 2996 50  0000 L CNN
F 1 "1uF/25V" H 6950 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6988 2800 50  0001 C CNN
F 3 "~" H 6950 2950 50  0001 C CNN
	1    6950 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 60F2BEE7
P 6600 3100
F 0 "#PWR016" H 6600 2850 50  0001 C CNN
F 1 "GND" H 6625 2950 50  0000 C CNN
F 2 "" H 6600 3100 50  0001 C CNN
F 3 "" H 6600 3100 50  0001 C CNN
	1    6600 3100
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR012
U 1 1 61124FF6
P 6200 4500
F 0 "#PWR012" H 6200 4350 50  0001 C CNN
F 1 "VCC" V 6200 4625 50  0000 L CNN
F 2 "" H 6200 4500 50  0001 C CNN
F 3 "" H 6200 4500 50  0001 C CNN
	1    6200 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 611389ED
P 7150 2350
F 0 "#PWR021" H 7150 2100 50  0001 C CNN
F 1 "GND" H 7300 2200 50  0000 R CNN
F 2 "" H 7150 2350 50  0001 C CNN
F 3 "" H 7150 2350 50  0001 C CNN
	1    7150 2350
	1    0    0    -1  
$EndComp
Text HLabel 3350 3700 2    50   Input ~ 0
DIN
Text HLabel 3350 3600 2    50   Input ~ 0
CLK
Text HLabel 3350 3500 2    50   Input ~ 0
CS#
Text HLabel 3350 3400 2    50   Input ~ 0
DC#
Text HLabel 3350 3300 2    50   Input ~ 0
RST#
Text HLabel 3350 3200 2    50   Input ~ 0
BUSY
Wire Wire Line
	3150 3200 3350 3200
Wire Wire Line
	3150 3300 3350 3300
Wire Wire Line
	3150 3400 3350 3400
Wire Wire Line
	3150 3500 3350 3500
Wire Wire Line
	3150 3600 3350 3600
Wire Wire Line
	3150 3700 3350 3700
Text Label 3150 3700 0    50   ~ 0
DIN
Text Label 3150 3600 0    50   ~ 0
CLK
Text Label 3150 3500 0    50   ~ 0
CS#
Text Label 3150 3400 0    50   ~ 0
DC#
Text Label 3150 3300 0    50   ~ 0
RST#
Text Label 3150 3200 0    50   ~ 0
BUSY
Text HLabel 3350 3800 2    50   Input ~ 0
PWR
$Comp
L power:VCC #PWR04
U 1 1 6119A3DC
P 3200 4700
F 0 "#PWR04" H 3200 4550 50  0001 C CNN
F 1 "VCC" H 3215 4873 50  0000 C CNN
F 2 "" H 3200 4700 50  0001 C CNN
F 3 "" H 3200 4700 50  0001 C CNN
	1    3200 4700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR022
U 1 1 611B06C7
P 7300 3100
F 0 "#PWR022" H 7300 2850 50  0001 C CNN
F 1 "GND" H 7350 2950 50  0000 C CNN
F 2 "" H 7300 3100 50  0001 C CNN
F 3 "" H 7300 3100 50  0001 C CNN
	1    7300 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR026
U 1 1 611B425C
P 8000 3100
F 0 "#PWR026" H 8000 2850 50  0001 C CNN
F 1 "GND" H 8050 2950 50  0000 C CNN
F 2 "" H 8000 3100 50  0001 C CNN
F 3 "" H 8000 3100 50  0001 C CNN
	1    8000 3100
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR020
U 1 1 611BC164
P 7100 3650
F 0 "#PWR020" H 7100 3500 50  0001 C CNN
F 1 "VCC" V 7115 3778 50  0000 L CNN
F 2 "" H 7100 3650 50  0001 C CNN
F 3 "" H 7100 3650 50  0001 C CNN
	1    7100 3650
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR025
U 1 1 611C3951
P 7900 3650
F 0 "#PWR025" H 7900 3500 50  0001 C CNN
F 1 "VCC" V 7915 3778 50  0000 L CNN
F 2 "" H 7900 3650 50  0001 C CNN
F 3 "" H 7900 3650 50  0001 C CNN
	1    7900 3650
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR011
U 1 1 611E4B10
P 6200 3650
F 0 "#PWR011" H 6200 3400 50  0001 C CNN
F 1 "GND" V 6200 3500 50  0000 R CNN
F 2 "" H 6200 3650 50  0001 C CNN
F 3 "" H 6200 3650 50  0001 C CNN
	1    6200 3650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR014
U 1 1 611FBD36
P 6250 2350
F 0 "#PWR014" H 6250 2100 50  0001 C CNN
F 1 "GND" H 6300 2200 50  0000 C CNN
F 2 "" H 6250 2350 50  0001 C CNN
F 3 "" H 6250 2350 50  0001 C CNN
	1    6250 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 6121586C
P 6700 2350
F 0 "#PWR017" H 6700 2100 50  0001 C CNN
F 1 "GND" H 6850 2200 50  0000 R CNN
F 2 "" H 6700 2350 50  0001 C CNN
F 3 "" H 6700 2350 50  0001 C CNN
	1    6700 2350
	1    0    0    -1  
$EndComp
Text Notes 4300 5200 0    50   ~ 0
NC
$Comp
L power:VCC #PWR07
U 1 1 612D8530
P 5100 3750
F 0 "#PWR07" H 5100 3600 50  0001 C CNN
F 1 "VCC" V 5115 3878 50  0000 L CNN
F 2 "" H 5100 3750 50  0001 C CNN
F 3 "" H 5100 3750 50  0001 C CNN
	1    5100 3750
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 3650 4800 3750
Wire Wire Line
	5100 3750 4800 3750
Connection ~ 4800 3750
$Comp
L power:GND #PWR06
U 1 1 612DB287
P 5100 3550
F 0 "#PWR06" H 5100 3300 50  0001 C CNN
F 1 "GND" V 5100 3400 50  0000 R CNN
F 2 "" H 5100 3550 50  0001 C CNN
F 3 "" H 5100 3550 50  0001 C CNN
	1    5100 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5100 3550 4800 3550
Wire Wire Line
	5950 3650 6200 3650
Text Label 6700 2050 0    50   ~ 0
PREVGH
Text Label 7150 2050 0    50   ~ 0
PREVGL
Wire Wire Line
	6850 4600 6850 4800
Connection ~ 6850 4600
Wire Wire Line
	6850 4600 7100 4600
$Comp
L Device:R R2
U 1 1 613BA793
P 6450 5400
F 0 "R2" H 6520 5446 50  0000 L CNN
F 1 "1M" H 6520 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6380 5400 50  0001 C CNN
F 3 "~" H 6450 5400 50  0001 C CNN
	1    6450 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 611EE51A
P 6200 5000
F 0 "#PWR013" H 6200 4750 50  0001 C CNN
F 1 "GND" H 6300 4850 50  0000 R CNN
F 2 "" H 6200 5000 50  0001 C CNN
F 3 "" H 6200 5000 50  0001 C CNN
	1    6200 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 613C1AF0
P 6450 5550
F 0 "#PWR015" H 6450 5300 50  0001 C CNN
F 1 "GND" H 6550 5400 50  0000 R CNN
F 2 "" H 6450 5550 50  0001 C CNN
F 3 "" H 6450 5550 50  0001 C CNN
	1    6450 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 613C295F
P 6850 5400
F 0 "R3" H 6920 5446 50  0000 L CNN
F 1 "3R" H 6920 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6780 5400 50  0001 C CNN
F 3 "~" H 6850 5400 50  0001 C CNN
	1    6850 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 613C3292
P 6850 5550
F 0 "#PWR018" H 6850 5300 50  0001 C CNN
F 1 "GND" H 6850 5400 50  0000 C CNN
F 2 "" H 6850 5550 50  0001 C CNN
F 3 "" H 6850 5550 50  0001 C CNN
	1    6850 5550
	1    0    0    -1  
$EndComp
Text Notes 7100 5500 0    50   ~ 0
2R2\n3R\n0.47R
$Comp
L power:GND #PWR023
U 1 1 613EDF0D
P 7400 4050
F 0 "#PWR023" H 7400 3800 50  0001 C CNN
F 1 "GND" V 7400 3850 50  0000 C CNN
F 2 "" H 7400 4050 50  0001 C CNN
F 3 "" H 7400 4050 50  0001 C CNN
	1    7400 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6550 5000 6450 5000
Wire Wire Line
	6700 4600 6850 4600
Wire Wire Line
	6200 4500 6200 4600
Wire Wire Line
	6200 4600 6400 4600
Connection ~ 6200 4600
Wire Wire Line
	6200 4600 6200 4700
Wire Wire Line
	7100 4350 7100 4200
Wire Wire Line
	7100 4200 6850 4200
Connection ~ 7100 4200
Wire Wire Line
	7100 4200 7100 4050
Wire Wire Line
	6850 4600 6850 4550
Wire Wire Line
	6850 4200 6850 4250
Text Label 6450 5150 0    50   ~ 0
GDR
Text Label 6850 5250 0    50   ~ 0
RESE
Wire Wire Line
	6450 5000 6450 5250
Wire Wire Line
	6850 5250 6850 5200
Text HLabel 3350 3900 2    50   Input ~ 0
PWR_EN#
Wire Wire Line
	3200 3800 3350 3800
Wire Wire Line
	3200 3800 3200 4700
NoConn ~ 3350 3900
$EndSCHEMATC
