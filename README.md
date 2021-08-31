# mom_user_Mazak_Integrex_i630V
 mom_user for Mazak Integrex i630V - 2016

Список того, что спрятано в DLL:

#EXTN_rtv_mom_cp_part_attr "3DCOMP"
EXTN_rtv_mom_cp_part_attr 2

#EXTN_rtv_mom_cp_part_attr "PLANE_MILL"

#EXTN_rtv_mom_cp_part_attr "POLYFORM"
EXTN_rtv_mom_cp_part_attr 3

EXTN_rtv_mom_cp_part_attr 6

#EXTN_rtv_mom_cp_part_attr "G43_MOVE"
EXTN_rtv_mom_cp_part_attr 6

#EXTN_rtv_mom_cp_part_attr "PLANE_MILL2"

#EXTN_rtv_mom_cp_part_attr "PLANE_TURN"

#EXTN_rtv_mom_cp_part_attr "FOR_TURN"

---
2 bit = 4294967295, а для 64 bit = 18446744073709551615

константы

2	qword_1800062D0	dq 4000000000000000h
360	qword_1800062D8	dq 4076800000000000h
0	qword_1800062E0	dq 3C32725DD1D243ACh
180	qword_1800062E8	dq 4066800000000000h
pi	qword_1800062F0	dq 400921FB54442D18h

Число ?, равное 3,1415926535897932384626433832795, можно представить как 32-битное число 40490FDBh, можно как 64-битое 400921FB54442D18h


-pi	qword_1800062F8	dq 0C00921FB54442D18h
10	qword_180006300	dq 4024000000000000h
100	qword_180006308	dq 4059000000000000h

4024000000000000h <-> 10 = 1010b

100    = 4059000000000000h
200    = 4069000000000000h


xmm6=angle1*(-pi)/180
xmm8=angle2*pi/180

qword_1800078E0=cos(xmm6)
qword_1800078E8=xmm8

xmm9=cos(xmm6)*ci-sin(xmm6)*ck

qword_180007850=cos(xmm8)*xmm9-sin(xmm8)*cj
qword_180007858=cos(xmm8)*cj+sin(xmm8)*xmm9
qword_180007860=sin(xmm6)*ci + cos(xmm6)*ck

UF_VEC3_dot  = UF_VEC3_dot(qword_180007850,i,&qword_1800078F0)

qword_1800078A8=i*qword_1800078F0-qword_180007850
qword_1800078B0=j*qword_1800078F0-qword_180007858
qword_1800078B8=k*qword_1800078F0-qword_180007860

xmm1=qword_1800062E0

UF_VEC3_unitize    = UF_VEC3_unitize(qword_1800078A8,tolerance,&magnitude=unk_1800078C0,qword_1800078A8)


