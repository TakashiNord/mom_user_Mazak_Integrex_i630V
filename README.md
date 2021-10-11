# mom_user_Mazak_Integrex_i630V

____

The part of the postprocessor for Mazak Integrex i630V.
- 2016 year.
- CAD\CAM\CAE : Unigraphics
- parts postprocessor: tcl, def and dll
- Exporting function dll : EXTN_rtv_mom_cp_part_attr
used
```
EXTN_rtv_mom_cp_part_attr 2 ; # "3DCOMP"
EXTN_rtv_mom_cp_part_attr 3 ; # "POLYFORM"
EXTN_rtv_mom_cp_part_attr 6 ; # "G43_MOVE"
```
don't use
```
EXTN_rtv_mom_cp_part_attr "PLANE_MILL"
EXTN_rtv_mom_cp_part_attr "PLANE_MILL2"
EXTN_rtv_mom_cp_part_attr "PLANE_TURN"
EXTN_rtv_mom_cp_part_attr "FOR_TURN"
```
____

