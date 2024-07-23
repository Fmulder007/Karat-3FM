<Qucs Schematic 24.2.1>
<Properties>
  <View=5,-148,1283,930,0.743349,0,0>
  <Grid=10,10,1>
  <DataSet=test_bidamp.dat>
  <DataDisplay=test_bidamp.dpl>
  <OpenDisplay=0>
  <Script=test_bidamp.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Название>
  <FrameText1=Чертил:>
  <FrameText2=Дата:>
  <FrameText3=Версия:>
</Properties>
<Symbol>
  <.ID -20 -16 SUB>
  <Line -20 20 40 0 #000080 2 1>
  <Line 20 20 0 -40 #000080 2 1>
  <Line -20 -20 40 0 #000080 2 1>
  <Line -20 20 0 -40 #000080 2 1>
</Symbol>
<Components>
  <R R1 1 360 100 15 -26 0 1 "200" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <GND * 1 720 580 0 0 0 0>
  <Diode D_1N4148_2 1 500 240 13 -26 0 1 "222p" 1 "1.65" 1 "4p" 0 "0.333" 0 "0.7" 0 "0.5" 0 "0" 0 "0" 0 "2" 0 "68.6m" 0 "5.76n" 0 "0" 0 "0" 0 "1" 0 "1" 0 "75" 0 "1u" 0 "26.85" 0 "3.0" 0 "1.11" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "1.0" 0 "normal" 0>
  <GND * 1 300 380 0 0 0 0>
  <GND * 1 440 260 0 -19 1 0>
  <C C4 1 550 150 -26 17 0 0 "100n" 1 "" 0 "neutral" 0>
  <GND * 1 100 650 0 0 0 0>
  <GND * 1 650 270 0 0 0 0>
  <Pac P1 1 100 620 18 -26 0 1 "1" 1 "200" 1 "0 dBm" 0 "10 MHz" 0 "26.85" 0 "true" 0>
  <Pac P2 1 650 200 18 -26 0 1 "2" 1 "200" 1 "0 dBm" 0 "1 MHz" 0 "26.85" 0 "true" 0>
  <.SP SP1 1 790 70 0 68 0 0 "lin" 1 "1 MHz" 1 "100 MHz" 1 "200" 1 "no" 0 "1" 0 "2" 0 "no" 0 "no" 0>
  <GND * 1 170 80 0 -19 1 0>
  <C C3 1 150 510 -26 17 0 0 "100n" 1 "" 0 "neutral" 0>
  <GND * 1 470 700 0 0 0 0>
  <_BJT T_MMDT3946_2 1 500 510 8 -26 1 0 "pnp" 0 "7.21e-016" 0 "1.00" 0 "1.00" 0 "48.6m" 0 "0.120" 0 "114" 0 "20.0" 0 "1.01p" 0 "2.00" 0 "0" 0 "2" 0 "410" 0 "4.00" 0 "0" 0 "0" 0 "0.483" 0 "1.21" 0 "4.83" 0 "10.9p" 0 "1.10" 0 "0.500" 0 "7.57p" 0 "0.300" 0 "0.300" 0 "1" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "558p" 0 "0" 0 "0" 0 "0" 0 "84.1n" 0 "26.85" 0 "0" 0 "1" 0 "1" 0 "0" 0 "1" 0 "1" 0 "0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <GND * 1 500 550 0 0 0 0>
  <C C5 1 470 660 17 -26 0 1 "100n" 1 "" 0 "neutral" 0>
  <Diode D_1N4148_4 1 280 660 13 -26 0 1 "222p" 1 "1.65" 1 "4p" 0 "0.333" 0 "0.7" 0 "0.5" 0 "0" 0 "0" 0 "2" 0 "68.6m" 0 "5.76n" 0 "0" 0 "0" 0 "1" 0 "1" 0 "75" 0 "1u" 0 "26.85" 0 "3.0" 0 "1.11" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "1.0" 0 "normal" 0>
  <GND * 1 280 700 0 0 0 0>
  <_BJT T_MMDT3946_6 1 170 150 -125 -26 1 2 "pnp" 0 "7.21e-016" 0 "1.00" 0 "1.00" 0 "48.6m" 0 "0.120" 0 "114" 0 "20.0" 0 "1.01p" 0 "2.00" 0 "0" 0 "2" 0 "410" 0 "4.00" 0 "0" 0 "0" 0 "0.483" 0 "1.21" 0 "4.83" 0 "10.9p" 0 "1.10" 0 "0.500" 0 "7.57p" 0 "0.300" 0 "0.300" 0 "1" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "558p" 0 "0" 0 "0" 0 "0" 0 "84.1n" 0 "26.85" 0 "0" 0 "1" 0 "1" 0 "0" 0 "1" 0 "1" 0 "0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <_BJT T_MMDT3946_4 1 170 280 -117 -26 0 2 "npn" 0 "5.81e-016" 0 "1.00" 0 "1.00" 0 "0.304" 0 "0.750" 0 "114" 0 "24.0" 0 "2.28p" 0 "2.00" 0 "0" 0 "2" 0 "410" 0 "4.00" 0 "0" 0 "0" 0 "0.283" 0 "0.707" 0 "2.83" 0 "9.67p" 0 "1.10" 0 "0.500" 0 "6.86p" 0 "0.300" 0 "0.300" 0 "1" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "450p" 0 "0" 0 "0" 0 "0" 0 "70.2n" 0 "26.85" 0 "0" 0 "1" 0 "1" 0 "0" 0 "1" 0 "1" 0 "0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <_BJT T_MMDT3946_7 1 500 380 8 -26 0 0 "npn" 0 "5.81e-016" 0 "1.00" 0 "1.00" 0 "0.304" 0 "0.750" 0 "114" 0 "24.0" 0 "2.28p" 0 "2.00" 0 "0" 0 "2" 0 "410" 0 "4.00" 0 "0" 0 "0" 0 "0.283" 0 "0.707" 0 "2.83" 0 "9.67p" 0 "1.10" 0 "0.500" 0 "6.86p" 0 "0.300" 0 "0.300" 0 "1" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "450p" 0 "0" 0 "0" 0 "0" 0 "70.2n" 0 "26.85" 0 "0" 0 "1" 0 "1" 0 "0" 0 "1" 0 "1" 0 "0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <R R7 1 220 590 15 -26 0 1 "200" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R6 1 360 330 15 -26 0 1 "1" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R2 1 360 220 15 -26 0 1 "47k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R3 1 360 440 15 -26 0 1 "47k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <C C2 1 440 320 17 -26 0 1 "100n" 1 "" 0 "neutral" 0>
  <C C1 1 300 350 17 -26 0 1 "100n" 1 "" 0 "neutral" 0>
  <Vdc V1 1 720 480 18 -26 0 1 "8.5" 1>
  <R R8 1 500 440 15 -26 0 1 "10" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R9 1 170 210 15 -26 0 1 "10" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <Diode D_1N4148_1 1 170 370 13 -26 1 3 "222p" 1 "1.65" 1 "4p" 0 "0.333" 0 "0.7" 0 "0.5" 0 "0" 0 "0" 0 "2" 0 "68.6m" 0 "5.76n" 0 "0" 0 "0" 0 "1" 0 "1" 0 "75" 0 "1u" 0 "26.85" 0 "3.0" 0 "1.11" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "1.0" 0 "normal" 0>
</Components>
<Wires>
  <360 130 360 150 "" 0 0 0 "">
  <360 360 360 380 "" 0 0 0 "">
  <360 470 360 510 "" 0 0 0 "">
  <360 380 360 410 "" 0 0 0 "">
  <360 380 440 380 "" 0 0 0 "">
  <360 150 490 150 "" 0 0 0 "">
  <500 150 500 210 "" 0 0 0 "">
  <360 280 360 300 "" 0 0 0 "">
  <300 280 360 280 "" 0 0 0 "">
  <300 280 300 320 "" 0 0 0 "">
  <490 150 500 150 "" 0 0 0 "">
  <500 150 520 150 "" 0 0 0 "">
  <650 230 650 270 "" 0 0 0 "">
  <580 150 650 150 "" 0 0 0 "">
  <650 150 650 170 "" 0 0 0 "">
  <180 510 190 510 "" 0 0 0 "">
  <100 510 100 590 "" 0 0 0 "">
  <100 510 120 510 "" 0 0 0 "">
  <720 510 720 580 "" 0 0 0 "">
  <500 270 500 350 "" 0 0 0 "">
  <440 380 470 380 "" 0 0 0 "">
  <360 510 470 510 "" 0 0 0 "">
  <500 540 500 550 "" 0 0 0 "">
  <470 690 470 700 "" 0 0 0 "">
  <280 630 470 630 "" 0 0 0 "">
  <280 690 280 700 "" 0 0 0 "">
  <170 80 170 120 "" 0 0 0 "">
  <200 150 360 150 "" 0 0 0 "">
  <200 280 300 280 "" 0 0 0 "">
  <720 70 720 450 "" 0 0 0 "">
  <360 70 720 70 "" 0 0 0 "">
  <220 620 220 630 "" 0 0 0 "">
  <220 630 280 630 "" 0 0 0 "">
  <290 600 360 600 "" 0 0 0 "">
  <290 560 290 600 "" 0 0 0 "">
  <220 560 290 560 "" 0 0 0 "">
  <360 150 360 190 "" 0 0 0 "">
  <360 250 360 280 "" 0 0 0 "">
  <440 260 440 290 "" 0 0 0 "">
  <440 350 440 380 "" 0 0 0 "">
  <500 470 500 480 "" 0 0 0 "">
  <170 240 170 250 "" 0 0 0 "">
  <360 510 360 600 "" 0 0 0 "">
  <170 310 170 340 "" 0 0 0 "">
  <170 400 170 430 "" 0 0 0 "">
  <170 430 190 430 "" 0 0 0 "">
  <190 510 360 510 "" 0 0 0 "">
  <190 430 190 510 "" 0 0 0 "">
  <650 150 650 150 "out" 680 120 0 "">
  <100 510 100 510 "in" 130 480 0 "">
</Wires>
<Diagrams>
  <Rect 830 701 412 181 3 #c0c0c0 1 10 1 1e+06 1 1e+08 1 -0.0471236 0.2 0.8 1 -1 0.5 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/ac.v(s_1_1)@frequency" #ff0000 0 3 0 0 0>
	<"ngspice/ac.v(s_2_2)@frequency" #0000ff 0 3 0 0 0>
  </Rect>
  <Rect 820 440 412 181 3 #c0c0c0 1 10 1 1e+06 1 1e+08 1 -1.18094 5 12.9906 1 -1 0.5 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/ac.v(s_2_1)@frequency" #ff0000 0 3 0 0 0>
	  <Mkr 1.34372e+07 72 -243 3 1 0>
	  <Mkr 2.93568e+07 112 -299 3 1 0>
	  <Mkr 4.77638e+07 170 -361 3 1 0>
	<"ngspice/ac.v(s_1_2)@frequency" #0000ff 0 3 0 0 0>
  </Rect>
</Diagrams>
<Paintings>
</Paintings>
