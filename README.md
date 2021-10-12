# mom_user_Mazak_Integrex_i630V

____

The part of the postprocessor for Mazak Integrex i630V.
- 2015 year.
- CAD\CAM\CAE : Unigraphics
- parts postprocessor: tcl, def and dll
- mom_user_Mazak_Integrex_i630V.tcl - replacing the dll
- Exporting function dll : EXTN_rtv_mom_cp_part_attr
used
```
EXTN_rtv_mom_cp_part_attr 2 ; # "3DCOMP"
EXTN_rtv_mom_cp_part_attr 3 ; # "POLYFORM"
EXTN_rtv_mom_cp_part_attr 6 ; # "G43_MOVE"
```
don't used
```
EXTN_rtv_mom_cp_part_attr   1 ; # = > UF_CSYS_map_point
EXTN_rtv_mom_cp_part_attr   4 ; # INPUT_RETRACT
EXTN_rtv_mom_cp_part_attr   5 ; # INPUT_RETRACT_DIST
EXTN_rtv_mom_cp_part_attr   7 ; # M107
EXTN_rtv_mom_cp_part_attr   8 ; # M107 + output file
EXTN_rtv_mom_cp_part_attr  10 ; # DLL VERSION
EXTN_rtv_mom_cp_part_attr 200 ; # G68.2 + G53.1 + ONE*POINT*DIAMETER*MEASUREMENT
EXTN_rtv_mom_cp_part_attr 201 ; # G68.2 + G53.1 + SURFACE*POINT*MEASUREMENT
EXTN_rtv_mom_cp_part_attr 202 ; # G68.2 + G53.1
EXTN_rtv_mom_cp_part_attr 203 ; # G68.2 + G53.1 + WALL*THICKNESS*MEASUREMENT
EXTN_rtv_mom_cp_part_attr 205 ; # G68.2 + G53.1 + BOSS*BORE*MEAS
EXTN_rtv_mom_cp_part_attr 206 ; # G31 + MEASURED*DIAMETER

```
____


