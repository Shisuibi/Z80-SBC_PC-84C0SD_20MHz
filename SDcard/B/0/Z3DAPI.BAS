100 REM ---- Z3D API Initialize & Model Build ----
110 OUT &H60,0:GOSUB 500:GOSUB 600:GOSUB 700:GOSUB 800:MR=0
120 REM ---- Ambient & Parallel Light ----
130 CF= 8.0:GOSUB 900:OUT &H79,CH:OUT &H78,CL
140 CF= 8.0:GOSUB 900:OUT &H7B,CH:OUT &H7A,CL
150 CF=-8.0:GOSUB 900:OUT &H7D,CH:OUT &H7C,CL
160 CF= 1.0:GOSUB 900:OUT &H7F,CH:OUT &H7E,CL
170 CF= 0.5:OUT &H62,CF*255:OUT &H63,CF*255
180 REM ---- Push Display Device Matrix ----
190 ML=INP(&H64):OUT &H6E,0:OUT &H64,0
200 REM ---- Push Perspective Trans Matrix ----
210 CF=  1.0:GOSUB 900:OUT &H79,CH:OUT &H78,CL
220 CF=100.0:GOSUB 900:OUT &H7B,CH:OUT &H7A,CL
230 CF= 45.0:OUT &H6F,CF*256/180:OUT &H64,0
240 REM ---- Push Multiply Matrix ----
250 OUT &H6B,ML:OUT &H64,0
260 REM ---- Clear Screen & Background Color----
270 OUT &H47,&H00:OUT &H46,&H00:OUT &H41,1
280 OUT &H47,&H86:OUT &H46,&H7D
300 REM ---- Push Viewpoint Matrix ----
310 CF=0.0:GOSUB 900:OUT &H79,CH:OUT &H78,CL
320 CF=0.0:GOSUB 900:OUT &H7B,CH:OUT &H7A,CL
330 CF=8.0:GOSUB 900:OUT &H7D,CH:OUT &H7C,CL
340 CF=1.0:GOSUB 900:OUT &H7F,CH:OUT &H7E,CL
350 ML=INP(&H64):OUT &H67,0:OUT &H6C,0:OUT &H64,0
360 REM ---- Push World Matrix ----
370 OUT &H66,0
380 OUT &H68,(MR*1) AND &H00FF
390 OUT &H69,(MR*2) AND &H00FF
400 OUT &H6A,(MR*3) AND &H00FF
410 OUT &H64,0:MR=MR+1:IF MR=256 THEN MR=0
420 REM ---- Model Entry & Pop Matrix ----
430 OUT &H6B,ML:OUT &H76,MN:OUT &H65,0
440 REM ---- Model Shade & Pop Matrix ----
450 OUT &H77,MN:OUT &H65,0
460 REM ---- Flush Screen ----
470 OUT &H61,0:GOTO 300
500 REM ---- Model Head ----
510 READ MN,MA,MM,MV,MP
520 OUT &H78,MA:OUT &H7A,MM:OUT &H7C,MV:OUT &H7E,MP
530 OUT &H71,MN:RETURN
600 REM ---- Model Material ----
610 EI=MM-1:FOR I=0 TO EI
620 READ CR,CG,CB:OUT &H78,CB:OUT &H7A,CG:OUT &H7C,CR
630 OUT &H73,I:NEXT I:RETURN
700 REM ---- Model Vertex ----
710 EI=MV-1:FOR I=0 TO EI
720 READ CF:GOSUB 900:OUT &H79,CH:OUT &H78,CL
730 READ CF:GOSUB 900:OUT &H7B,CH:OUT &H7A,CL
740 READ CF:GOSUB 900:OUT &H7D,CH:OUT &H7C,CL
750 READ CF:GOSUB 900:OUT &H7F,CH:OUT &H7E,CL
760 OUT &H74,I:NEXT I:RETURN
800 REM ---- Model Polygon ----
810 EI=MP-1:FOR I=0 TO EI
820 READ V0,V1,V2,PM
830 OUT &H78,V0:OUT &H7A,V1:OUT &H7C,V2:OUT &H7E,PM
840 OUT &H75,I:NEXT I:RETURN
900 REM ---- Coordinate (Float Point -> Fixed Point) ----
910 CH=INT(CF) AND &H00FF:CL=INT(CF*256) AND &H00FF:RETURN
1000 REM ---- Model Info ----
1010 REM      6----7
1020 REM     /|   /|    Y  Z
1030 REM    / |  / |    | /
1040 REM   /  4-/--5    |/__X
1050 REM  2----3  /
1060 REM  | /  | /
1070 REM  |/   |/  Backside Judgement : On
1080 REM  0----1   Limited Depth Sort : Off
1100 REM ---- Model Head ----
1110 REM  Number, Attribute, Material, Vertex, Polygon
1200 DATA      0,         1,        6,      8,      12
1300 REM ---- Model Material ----
1310 REM  ColR, ColG, ColB
1400 DATA  255,  255,    0
1410 DATA  255,    0,    0
1420 DATA  255,  128,    0
1430 DATA    0,    0,  255
1440 DATA    0,  255,    0
1450 DATA  255,  255,  255
1500 REM ---- Model Vertex ----
1510 REM  PosX, PosY, PosZ, PosW
1600 DATA -1.0, -1.0, -1.0,  1.0
1610 DATA  1.0, -1.0, -1.0,  1.0
1620 DATA -1.0,  1.0, -1.0,  1.0
1630 DATA  1.0,  1.0, -1.0,  1.0
1640 DATA -1.0, -1.0,  1.0,  1.0
1650 DATA  1.0, -1.0,  1.0,  1.0
1660 DATA -1.0,  1.0,  1.0,  1.0
1670 DATA  1.0,  1.0,  1.0,  1.0
1700 REM ---- Model Polygon ----
1710 REM  Ver0, Ver1, Ver2, Mate
1800 DATA    0,    3,    1,    0
1810 DATA    0,    2,    3,    0
1820 DATA    1,    7,    5,    1
1830 DATA    1,    3,    7,    1
1840 DATA    2,    4,    6,    2
1850 DATA    2,    0,    4,    2
1860 DATA    3,    6,    7,    3
1870 DATA    3,    2,    6,    3
1880 DATA    4,    1,    5,    4
1890 DATA    4,    0,    1,    4
1900 DATA    5,    6,    4,    5
1910 DATA    5,    7,    6,    5
