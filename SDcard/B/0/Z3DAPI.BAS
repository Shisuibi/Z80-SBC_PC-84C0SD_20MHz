100 REM ---- Z3D API Initialize & Model Build ----
110 OUT &H60,0:GOSUB 600:GOSUB 700:GOSUB 800:MR=0
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
430 OUT &H6B,ML:OUT &H74,MN:OUT &H65,0
440 REM ---- Model Shade & Pop Matrix ----
450 OUT &H75,MN:OUT &H65,0
460 REM ---- Flush Screen ----
470 OUT &H61,0:GOTO 300
600 REM ---- Model Head ----
610 READ MN,MV,MP,MA:OUT &H78,MV:OUT &H7A,MP:OUT &H7C,MA
620 OUT &H70,MN:RETURN
700 REM ---- Model Vertex ----
710 EI=MV-1:FOR I=0 TO EI
720 READ CF:GOSUB 900:OUT &H79,CH:OUT &H78,CL
730 READ CF:GOSUB 900:OUT &H7B,CH:OUT &H7A,CL
740 READ CF:GOSUB 900:OUT &H7D,CH:OUT &H7C,CL
750 READ CF:GOSUB 900:OUT &H7F,CH:OUT &H7E,CL
760 OUT &H72,I:NEXT I:RETURN
800 REM ---- Model Polygon ----
810 EI=MP-1:FOR I=0 TO EI
820 READ V0,V1,V2:OUT &H78,V0:OUT &H79,V1:OUT &H7A,V2
830 READ CR,CG,CB:OUT &H7C,CB:OUT &H7D,CG:OUT &H7E,CR
840 OUT &H73,I:NEXT I:RETURN
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
1110 REM  Number, Vertex, Polygon, Attribute
1200 DATA      0,      8,      12,         1
1300 REM ---- Model Vertex ----
1310 REM  PosX, PosY, PosZ, PosW
1400 DATA -1.0, -1.0, -1.0,  1.0
1410 DATA  1.0, -1.0, -1.0,  1.0
1420 DATA -1.0,  1.0, -1.0,  1.0
1430 DATA  1.0,  1.0, -1.0,  1.0
1440 DATA -1.0, -1.0,  1.0,  1.0
1450 DATA  1.0, -1.0,  1.0,  1.0
1460 DATA -1.0,  1.0,  1.0,  1.0
1470 DATA  1.0,  1.0,  1.0,  1.0
1500 REM ---- Model Polygon ----
1510 REM  V0, V1, V2,  CR,  CG,  CB
1600 DATA  0,  3,  1, 255, 255,   0
1610 DATA  0,  2,  3, 255, 255,   0
1620 DATA  1,  7,  5, 255,   0,   0
1630 DATA  1,  3,  7, 255,   0,   0
1640 DATA  2,  4,  6, 255, 128,   0
1650 DATA  2,  0,  4, 255, 128,   0
1660 DATA  3,  6,  7,   0,   0, 255
1670 DATA  3,  2,  6,   0,   0, 255
1680 DATA  4,  1,  5,   0, 255,   0
1690 DATA  4,  0,  1,   0, 255,   0
1700 DATA  5,  6,  4, 255, 255, 255
1710 DATA  5,  7,  6, 255, 255, 255
