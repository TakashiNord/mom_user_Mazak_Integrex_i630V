//////////////////////////////////////////////////////////////////////////////
//
//  mom_user.cpp
//
//  Description:
//      Contains Unigraphics entry points for the application.
//
//////////////////////////////////////////////////////////////////////////////

// #define _CRT_SECURE_NO_DEPRECATE 1

//  Include files
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <uf.h>
#include <uf_exit.h>
#include <uf_ui.h>
#include <uf_mom.h>

#include <uf_csys.h>
#include <uf_vec.h>

#include <windows.h>

#include "mom_user.h"

//----------------------------------------------------------------------------
static int EXTN_rtv_mom_cp_part_attr
(
  void *client_data,    /* This is the mom object */
  void *interp,         /* Pointer to the TCL interpreter */
  int  argc,            /* Count of the numnber of arguments */
  char **argv           /* Array of arguments where the
                           argv[0] - The TCL extension */
);


//----------------------------------------------------------------------------
//  Activation Methods
//----------------------------------------------------------------------------

//  Explicit Activation
//      This entry point is used to activate the application explicitly, as in
//      "File->Execute UG/Open->User Function..."
//----- (10001010) --------------------------------------------------------
extern "C" DllExport void entry_rtv_mom_cp_part_attr( char *parm, int *returnCode, int rlen )
{
  int errorCode ;
  UF_MOM_id_t mom ;
  void *interp = NULL ;

  if ( !UF_is_initialized() )
  {
    errorCode = UF_initialize();
    *returnCode = errorCode;
    if ( errorCode ) return ;
  }

  /* Get the TCL interpreter id from the ufusr param */
  UF_MOM_ask_interp_from_param ( parm, &interp ) ;
  /* Get the MOM id from the ufusr param */
  UF_MOM_ask_mom (parm, &mom) ;
  /*Создаем функции в интепретаторе*/
  UF_MOM_extend_xlator ( mom , "EXTN_rtv_mom_cp_part_attr", EXTN_rtv_mom_cp_part_attr ) ;

  *returnCode = 0;
  if ( errorCode == 1 )  errorCode = UF_terminate();
  *returnCode = errorCode;
}


//----------------------------------------------------------------------------
//  Utilities
//----------------------------------------------------------------------------

// Unload Handler
//     This function specifies when to unload your application from Unigraphics.
//     If your application registers a callback (from a MenuScript item or a
//     User Defined Object for example), this function MUST return
//     "UF_UNLOAD_UG_TERMINATE".
extern "C" int ufusr_ask_unload( void )
{
     /* unload immediately after application exits*/
     //return ( UF_UNLOAD_IMMEDIATELY );

     /*via the unload selection dialog... */
     //return ( UF_UNLOAD_SEL_DIALOG );
     /*when UG terminates...              */
     return ( UF_UNLOAD_UG_TERMINATE );
}


/*-----------------------------------------------------------------------
 * Function Name: EXTN_rtv_mom_cp_part_attr
 *
 *
 -----------------------------------------------------------------------*/
static int EXTN_rtv_mom_cp_part_attr
(
  void *client_data,    /* This is the mom object */
  void *interp,         /* Pointer to the TCL interpreter */
  int  argc,            /* Count of the numnber of arguments */
  char **argv           /* Array of arguments where the
                           argv[0] - The TCL extension */
)
{
  UF_MOM_id_t mom_id = ( UF_MOM_id_t ) client_data ;

  int     ii ;
  char    Dest[256] ;
  double  angle1 , angle2 ;
  double  x , y , z , i , j , k ;
  int br = 0 ;

  double  angle_tool ;
  const char *angle_tool_new , *input_zero_int;

  const char *kin_retract_distance ;
  double  retract_distance ;
  const char *kin_reengage_distance ;
  double  reengage_distance ;
  double  dx , dy , dz ;

 // double ci,cj,ck ;
  double point[3] ;

  double pi = 3.1415926 ;

  int  ia4[2]={0,0},
       ip6[2] = {1,1},
       resp;
  double ra5[2] = { 100, 100 } ;
  char   menu[2][16]; //={":Отвод=",":Врезание="};

  double v_vector[3] , contact_normal[3];
  double t[3] , cn[3] ;
  double dot = 1.0 ;
  double tolerance = 0.0000000001 ;
  double magnitude = 1.0 ;
  double xmm6 = 0, xmm8 = 0 , xmm9 = 0 ;

  double vec[3]={0,0,0};
  double unit_vec[3];

  double vec_from[3]={1,0,0},vec_ccw[3]={0,0,1};
  double angle ;

  double matrix_values[9] ;
  double csys_origin[3];
  tag_t matrix_id ;
  tag_t csys_id ;

  int sw =- 1;

  sprintf(Dest, "Number of args %ld", argc);
  if ( argc > 0 )
  {
    for (ii=0;ii<argc;ii++) sprintf(Dest, "Arg 0 %s", argv[ii]);
    sw=atoi(argv[1])-1; // вычитаем
  }

  switch ( sw ) {
  case 0:

     UF_MOM_ask_assoc_double_array(mom_id, "post_point", "0", &point[0]);
     UF_MOM_ask_assoc_double_array(mom_id, "post_point", "1", &point[1]);
     UF_MOM_ask_assoc_double_array(mom_id, "post_point", "2", &point[2]);

/* coordinate space of the root part which is the whole part being displayed */
//#define UF_CSYS_ROOT_COORDS     1
/* coordinate space of the work part */
//#define UF_CSYS_WORK_COORDS     2
/* work coordinate space of the root part */
//#define UF_CSYS_ROOT_WCS_COORDS 3
     UF_CSYS_map_point(UF_CSYS_ROOT_COORDS,point,UF_CSYS_ROOT_WCS_COORDS,point);

     UF_MOM_set_assoc_double_array(mom_id, "post_point_pos", "0", point[0]);
     UF_MOM_set_assoc_double_array(mom_id, "post_point_pos", "1", point[1]);
     UF_MOM_set_assoc_double_array(mom_id, "post_point_pos", "2", point[2]);

  break ;
  case 1: // "3DCOMP"

     UF_MOM_ask_assoc_double_array(mom_id, "contact_normal", "0", &cn[0]);
     UF_MOM_ask_assoc_double_array(mom_id, "contact_normal", "1", &cn[1]);
     UF_MOM_ask_assoc_double_array(mom_id, "contact_normal", "2", &cn[2]);
     UF_MOM_ask_assoc_double_array(mom_id, "tool_axis", "0", &t[0]);
     UF_MOM_ask_assoc_double_array(mom_id, "tool_axis", "1", &t[1]);
     UF_MOM_ask_assoc_double_array(mom_id, "tool_axis", "2", &t[2]);

     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);

     xmm6=angle1*(-pi)/180 ;
     xmm8=angle2*pi/180 ;

     xmm9=cos(xmm6)*cn[0]-sin(xmm6)*cn[2];

     contact_normal[0]=cos(xmm8)*xmm9 -  sin(xmm8)*cn[1];
     contact_normal[1]=cos(xmm8)*cn[1] + sin(xmm8)*xmm9 ;
     contact_normal[2]=sin(xmm6)*cn[0] + cos(xmm6)*cn[2];

     UF_VEC3_dot(contact_normal,t,&dot);

     v_vector[0]=t[0]*dot-contact_normal[0];
     v_vector[1]=t[1]*dot-contact_normal[1];
     v_vector[2]=t[2]*dot-contact_normal[2];

     UF_VEC3_unitize(v_vector,tolerance,&magnitude,v_vector) ;

     UF_MOM_set_assoc_double_array(mom_id, "v_vector", "0", v_vector[0]);
     UF_MOM_set_assoc_double_array(mom_id, "v_vector", "1", v_vector[1]);
     UF_MOM_set_assoc_double_array(mom_id, "v_vector", "2", v_vector[2]);
     UF_MOM_set_assoc_double_array(mom_id, "contact_normal", "0", contact_normal[0]);
     UF_MOM_set_assoc_double_array(mom_id, "contact_normal", "1", contact_normal[1]);
     UF_MOM_set_assoc_double_array(mom_id, "contact_normal", "2", contact_normal[2]);
     UF_MOM_set_assoc_double_array(mom_id, "comp_vector", "0", t[0]-v_vector[0]);
     UF_MOM_set_assoc_double_array(mom_id, "comp_vector", "1", t[1]-v_vector[1]);
     UF_MOM_set_assoc_double_array(mom_id, "comp_vector", "2", t[2]-v_vector[2]);

  break ;
  case 2: //"POLYFORM" ?

     UF_MOM_ask_assoc_double_array(mom_id, "pos", "0", &vec[0]);
     UF_MOM_ask_assoc_double_array(mom_id, "pos", "1", &vec[1]);

     UF_VEC3_unitize(vec,tolerance,&magnitude,unit_vec);

     UF_VEC3_angle_between(vec_from,vec,vec_ccw,&angle);

     angle=angle*180.0/pi ;
     if (angle>180.) angle=angle-360 ;

     UF_MOM_set_double(mom_id, "poly_ang",  angle);
     UF_MOM_set_double(mom_id, "poly_dlina",magnitude);

  break ;
  case 3: // INPUT_RETRACT

     strcpy(&menu[0][0], ":Отвод (мм)=");
     strcpy(&menu[1][0], ":Врезание (мм)=");

     UF_UI_close_listing_window();
     UF_UI_lock_ug_access(UF_UI_FROM_CUSTOM);
     resp = uc1608(":Введите значения отвода и врезания инструмента:", menu, 2, ia4, ra5, ip6);

     UF_MOM_set_double(mom_id, "kin_retract_distance", ra5[0]);
     UF_MOM_set_double(mom_id, "kin_reengage_distance", ra5[1]);
     UF_UI_unlock_ug_access(UF_UI_FROM_CUSTOM);

  break ;
  case 4: // INPUT_RETRACT_DIST

     uc1601(":Ввод дистанций отвода и врезания:", 0);
     UF_MOM_ask_assoc_double_array(mom_id, "prev_mcs_goto", "0", &x);
     UF_MOM_ask_assoc_double_array(mom_id, "prev_mcs_goto", "1", &y);
     UF_MOM_ask_assoc_double_array(mom_id, "prev_mcs_goto", "2", &z);
     UF_MOM_ask_assoc_double_array(mom_id, "prev_tool_axis", "0", &i);
     UF_MOM_ask_assoc_double_array(mom_id, "prev_tool_axis", "1", &j);
     UF_MOM_ask_assoc_double_array(mom_id, "prev_tool_axis", "2", &k);
     UF_MOM_ask_string(mom_id, "kin_retract_distance", &kin_retract_distance);
     retract_distance=atof(kin_retract_distance);
     //UF_free(&kin_retract_distance);
     UF_MOM_ask_string(mom_id, "kin_reengage_distance", &kin_reengage_distance);
     reengage_distance=atof(kin_reengage_distance);
     //UF_free(&kin_reengage_distance);

     dx=i*retract_distance + x ;
     dy=j*retract_distance + y ;
     dz=k*retract_distance + z ;

     UF_MOM_set_assoc_double_array(mom_id, "pos", "0", dx);
     UF_MOM_set_assoc_double_array(mom_id, "pos", "1", dy);
     UF_MOM_set_assoc_double_array(mom_id, "pos", "2", dz);

     dx=i*reengage_distance + x ;
     dy=j*reengage_distance + y ;
     dz=k*reengage_distance + z ;

     UF_MOM_set_assoc_double_array(mom_id, "pos_reen", "0", dx);
     UF_MOM_set_assoc_double_array(mom_id, "pos_reen", "1", dy);
     UF_MOM_set_assoc_double_array(mom_id, "pos_reen", "2", dz);

  break ;
  case 5: // "G43_MOVE" ?

     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
     x = cos(angle1*pi/180.);
     sprintf( Dest, "MOM_output_literal \"\043600=\133\133\043\13360000+\0433020\135+\04358203\135*\133%.8lf\135\135\"", x);
     UF_MOM_execute_command(mom_id, Dest);

  break ;
  case 6: // ?

     UF_MOM_ask_string(mom_id, "angle_tool_new", &angle_tool_new);
     UF_MOM_ask_string(mom_id, "input_zero_int", &input_zero_int);
     angle_tool = atof(angle_tool_new);
     sprintf(Dest,"MOM_output_literal \"G0 B%.3lf\"", angle_tool);
     UF_MOM_execute_command(mom_id, Dest);
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"M107\"");

  break ;
  case 7: // ?

     UF_MOM_ask_string(mom_id, "angle_tool_new", &angle_tool_new);
     UF_MOM_ask_string(mom_id, "input_zero_int", &input_zero_int);
     angle_tool = atof(angle_tool_new);
     sprintf(Dest,"MOM_output_literal \"G0 B%.3lf\"", angle_tool);
     UF_MOM_execute_command(mom_id, Dest);
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"M107\"");
     sprintf(Dest,"puts $ofile  \"#600=[[#[60000+#3020]+300.086]]\"");
     UF_MOM_execute_command(mom_id, Dest);

  break ;
  case 8:

     br = 0; // for break

  break ;
  case 9:

     UF_MOM_execute_command(mom_id, "MOM_output_literal \"(DLL VERSION 24.02.2015)\"");

  break ;
  case 199:
           //http://www.postprocessor.ru, ЗАО КГ "ПОСТПРОЦЕССОР"
     uc1601("http://www.postprocessor.ru, ¦L+ ¦+ \"¦+TT¦¦+г+TT+¦\"",0);

     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
     UF_MOM_execute_command(mom_id, "set G43_4_flag 0");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G49\"");

     sprintf(Dest,"MOM_output_literal \"G68.2 P1 X%.3lf Y%.3lf Z%.3lf I0.0 J%.3lf K%.3lf\"",x,y,z,angle1,angle2);
     UF_MOM_execute_command(mom_id, Dest);

     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G53.1\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#600=\[#\[60000+#3020\]+#58203\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 G43 G91 Z\[-#600\] F5.0 H$mom_tool_number\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G90\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X0 Y0 Z0 F[format %.3f $mom_feed_rate]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#501 = #5061 - #600*SIN\[[format %.3f $mom_out_angle_pos(0)]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#502 = #5062\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#504 = 2.0*SQRT\[#501*#501+#502*#502\]-2.0*2.978\"");

     dz=sqrt(x*x+y*y);
     sprintf(Dest,"MOM_output_literal \"#500 = %.3lf\"",dz);
     UF_MOM_execute_command(mom_id, Dest);

     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[---------------ONE*POINT*DIAMETER*MEASUREMENT----------------\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[MEASURED*DIAMETER*#504\[53\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[NOMINAL*DIAMETER *#500\[53\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#501 = #500-#504\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[-------------------------------------------------------------\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G69\"");

  break ;
  case 200:
           //http://www.postprocessor.ru, ЗАО КГ "ПОСТПРОЦЕССОР"
     uc1601("http://www.postprocessor.ru, ¦L+ ¦+ \"¦+TT¦¦+г+TT+¦\"",0);

     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
     UF_MOM_execute_command(mom_id, "set G43_4_flag 0");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G49\"");

     sprintf(Dest,"MOM_output_literal \"G68.2 P1 X%.3lf Y%.3lf Z%.3lf I0.0 J%.3lf K%.3lf\"",x,y,z,angle1,angle2);
     UF_MOM_execute_command(mom_id, Dest);

     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G53.1\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#600=\[#\[60000+#3020\]+#58203\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 G43 G91 Z\[-#600\] F5.0 H$mom_tool_number\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G90\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X0 Y0 Z0 F[format %.3f $mom_feed_rate]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#511 = #5061 - #600*SIN\[[format %.3f $mom_out_angle_pos(0)]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#512 = #5062\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#501 = #511*COS\[[format %.3f $mom_out_angle_pos(1)]\] - #512*SIN\[[format %.3f $mom_out_angle_pos(1)]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#502 = #511*SIN\[[format %.3f $mom_out_angle_pos(1)]\] + #512*COS\[[format %.3f $mom_out_angle_pos(1)]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#503 = #5063 - #600*COS\[[format %.3f $mom_out_angle_pos(0)]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#504 = SQRT\[\[#501-\[[format %.3f $mom_mcs_goto(0)]\]\]*\[#501-\[[format %.3f $mom_mcs_goto(0)]\]\]+\[#502-\[[format %.3f $mom_mcs_goto(1)]\]\]*\[#502-\[[format %.3f $mom_mcs_goto(1)]\]\]+ \[#503-\[[format %.3f $mom_mcs_goto(2)]\]\]*\[#503-\[[format %.3f $mom_mcs_goto(2)]\]\]\]-2.978\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[---------------SURFACE*POINT*MEASUREMENT---------------------\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[DATA**#3011\[80\]***TIME**#3012\[60\]--------------------\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[MEASURED**X**#501\[43\]***Y*=*#502\[43\]***Z*=*#503\[43\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[ERROR********#504\[43\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[-------------------------------------------------------------\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G69\"");

  break ;
  case 201:
           //http://www.postprocessor.ru, ЗАО КГ "ПОСТПРОЦЕССОР"
     uc1601("http://www.postprocessor.ru, ¦L+ ¦+ \"¦+TT¦¦+г+TT+¦\"",0);

     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
     UF_MOM_execute_command(mom_id, "set meas_pt1(0) $mom_mcs_goto(0)");
     UF_MOM_execute_command(mom_id, "set meas_pt1(1) $mom_mcs_goto(1)");
     UF_MOM_execute_command(mom_id, "set meas_pt1(2) $mom_mcs_goto(2)");
     UF_MOM_execute_command(mom_id, "set G43_4_flag 0");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G49\"");

     sprintf(Dest,"MOM_output_literal \"G68.2 P1 X%.3lf Y%.3lf Z%.3lf I0.0 J%.3lf K%.3lf\"",x,y,z,angle1,angle2);
     UF_MOM_execute_command(mom_id, Dest);

     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G53.1\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#600=\[#\[60000+#3020\]+#58203\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 G43 G91 Z\[-#600\] F5.0 H$mom_tool_number\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G90\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X0 Y0 Z0 F[format %.3f [expr abs($mom_programmed_feed_rate)]]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#501 = #5061 - #600*SIN\[[format %.3f $mom_out_angle_pos(0)]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#502 = #5062\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#503 = #5063 - #600*COS\[[format %.3f $mom_out_angle_pos(0)]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G69\"");

  break ;
  case 202:
           //http://www.postprocessor.ru, ЗАО КГ "ПОСТПРОЦЕССОР"
     uc1601("http://www.postprocessor.ru, ¦L+ ¦+ \"¦+TT¦¦+г+TT+¦\"",0);

     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
     UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
     UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
     UF_MOM_execute_command(mom_id, "set G43_4_flag 0");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G49\"");

     sprintf(Dest,"MOM_output_literal \"G68.2 P1 X%.3lf Y%.3lf Z%.3lf I0.0 J%.3lf K%.3lf\"",x,y,z,angle1,angle2);
     UF_MOM_execute_command(mom_id, Dest);

     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G53.1\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#600=\[#\[60000+#3020\]+#58203\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 G43 G91 Z\[-#600\] F5.0 H$mom_tool_number\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G90\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X0 Y0 Z0 F[format %.3f [expr abs($mom_programmed_feed_rate)]]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#511 = #5061 - #600*SIN\[[format %.3f $mom_out_angle_pos(0)]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#512 = #5062\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#513 = #5063 -#600*COS\[[format %.3f $mom_out_angle_pos(0)]\]\"");
     UF_MOM_execute_command(mom_id, "if {$mom_programmed_feed_rate > 0.0} { MOM_output_literal \"#504 = SQRT\[\[#501-#511\]*\[#501-#511\]+\[#502-#512\]*\[#502-#512\]+\[#503-#513\]*\[#503-#513\]\]-2.0*2.978\" }");
     UF_MOM_execute_command(mom_id, "if {$mom_programmed_feed_rate < 0.0} { MOM_output_literal \"#504 = SQRT\[\[#501-#511\]*\[#501-#511\]+\[#502-#512\]*\[#502-#512\]+\[#503-#513\]*\[#503-#513\]\]+2.0*2.978\" }");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#505 = [format%.3f [expr sqrt(($mom_mcs_goto(0)-$meas_pt1(0))*($mom_mcs_goto(0)-$meas_pt1(0)) + ($mom_mcs_goto(1)-$meas_pt1(1))*($mom_mcs_goto(1)-$meas_pt1(1)) + ($mom_mcs_goto(2)-$meas_pt1(2))*($mom_mcs_goto(2)-$meas_pt1(2)))]]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[---------------WALL*THICKNESS*MEASUREMENT----------------\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[MEASURED*AT*POINT***X=#501\[53\]*Y=#502\[53\]*Z=#503\[53\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[NOMINAL*THICKNESS****#505\[53\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[MEASURED*THICKNESS****#504\[53\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#506 = #505-#504\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[ERROR*****************#506\[34\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[---------------------------------------------------------\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G69\"");

  break ;
  case 203:
           //http://www.postprocessor.ru, ЗАО КГ "ПОСТПРОЦЕССОР"
     uc1601("http://www.postprocessor.ru, ¦L+ ¦+ \"¦+TT¦¦+г+TT+¦\"",0);

     UF_MOM_ask_assoc_double_array(mom_id, "msys_matrix", "0", &matrix_values[0]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_matrix", "1", &matrix_values[1]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_matrix", "2", &matrix_values[2]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_matrix", "3", &matrix_values[3]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_matrix", "4", &matrix_values[4]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_matrix", "5", &matrix_values[5]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_matrix", "6", &matrix_values[6]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_matrix", "7", &matrix_values[7]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_matrix", "8", &matrix_values[8]);

     UF_MOM_ask_assoc_double_array(mom_id, "msys_origin", "0", &csys_origin[0]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_origin", "1", &csys_origin[1]);
     UF_MOM_ask_assoc_double_array(mom_id, "msys_origin", "2", &csys_origin[2]);

     UF_CSYS_create_matrix(matrix_values,&matrix_id);

     UF_CSYS_create_csys(csys_origin,matrix_id,&csys_id);

     UF_CSYS_set_wcs(csys_id);

     UF_MOM_ask_assoc_double_array(mom_id, "post_arc_center", "0", &point[0]);
     UF_MOM_ask_assoc_double_array(mom_id, "post_arc_center", "1", &point[1]);
     UF_MOM_ask_assoc_double_array(mom_id, "post_arc_center", "2", &point[2]);

/* coordinate space of the root part which is the whole part being displayed */
//#define UF_CSYS_ROOT_COORDS     1
/* coordinate space of the work part */
//#define UF_CSYS_WORK_COORDS     2
/* work coordinate space of the root part */
//#define UF_CSYS_ROOT_WCS_COORDS 3
     UF_CSYS_map_point(UF_CSYS_ROOT_COORDS,point,UF_CSYS_ROOT_WCS_COORDS,point);

     UF_MOM_set_assoc_double_array(mom_id, "post_arc_center", "0", point[0]);
     UF_MOM_set_assoc_double_array(mom_id, "post_arc_center", "1", point[1]);
     UF_MOM_set_assoc_double_array(mom_id, "post_arc_center", "2", point[2]);

  break ;
  case 204:

     UF_MOM_execute_command(mom_id, "set G43_4_flag 0");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G49\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G68.2 P1 X[format %.3f $mom_post_arc_center(0)] Y[format %.3f $mom_post_arc_center(1)] Z[format %.3f $mom_post_arc_center(2)] I0.0 J[format %.3f $mom_out_angle_pos(0)] K[format %.3f $mom_out_angle_pos(1)]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G53.1\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#600=\[#\[60000+#3020\]+#58203\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 G43 G91 Z\[-#600\] F5.0 H$mom_tool_number\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G90\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 X0.0 Y0.0 Z[format %.3f $mom_post_z] F[format %.3f $mom_post_f]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X[format %.3f [expr 0.5*$mom_post_d]] Y0.0 Z[format %.3f $mom_post_z] F100.0\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#501=#5161\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#502=#5162\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#503=#5163\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 X0.0 Y0.0 Z[format %.3f $mom_post_z] F[format %.3f $mom_post_f]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X0.0 Y[format %.3f [expr 0.5*$mom_post_d]] Z[format %.3f $mom_post_z] F100.0\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#511=#5161\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#512=#5162\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#513=#5163\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 X0.0 Y0.0 Z[format %.3f $mom_post_z] F[format %.3f $mom_post_f]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X[format %.3f [expr -0.5*$mom_post_d]] Y0.0 Z[format %.3f $mom_post_z] F100.0\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#521=#5161\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#522=#5162\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#523=#5163\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 X0.0 Y0.0 Z[format %.3f $mom_post_z] F[format %.3f $mom_post_f]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#1=\[#501-#511\]*\[#501-#511\]+\[#502-#512\]*\[#502-#512\]+\[#503-#513\]*\[#503-#513\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#2=\[#501-#521\]*\[#501-#521\]+\[#502-#522\]*\[#502-#522\]+\[#503-#523\]*\[#503-#523\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#3=\[#511-#521\]*\[#511-#521\]+\[#512-#522\]*\[#512-#522\]+\[#513-#523\]*\[#513-#523\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#4=SQRT\[2.0*#1*#2+2.0*#1*#3+2.0*#3*#2-#1*#1-#2*#2-#3*#3\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#4=2.0*2.978+2.0*SQRT\[#1*#2*#3\]/#4\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#514=[format %.4f $mom_out_angle_pos(0)]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#515=[format %.4f $mom_out_angle_pos(1)]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#516=[format %.4f $mom_post_z]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#5=#3*\[#1+#2-#3\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#6=#2*\[#1+#3-#2\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#7=#1*\[#2+#3-#1\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#8=#5+#6+#7\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#517=\[#501*#5 + #511*#6 + #521*#7\]/#8\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#518=\[#502*#5 + #512*#6 + #522*#7\]/#8\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#519=\[#503*#5 + #513*#6 + #523*#7\]/#8\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#520=SQRT\[#517*#517+#518*#518\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[---------------BOSS*BORE*MEAS----------------\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[MEASURED*DIAMETER*FOR*Z=#516\[44\]*=*#4\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\\[ARC\\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[POINT***#501\[44\]***#502\[44\]***#503\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[POINT***#511\[44\]***#512\[44\]***#513\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[POINT***#521\[44\]***#522\[44\]***#523\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[XC*#517\[44\]***YC*#518\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[CENTER*DEVIATION***#520\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[********#600\[44\]***#514\[44\]***#515\[44\]\]\"");

  break ;
  case 205:

     UF_MOM_execute_command(mom_id, "set mom_post_z $mom_post_second_dep");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 X0.0 Y0.0 Z[format %.3f $mom_post_z] F[format %.3f $mom_post_f]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X[format %.3f [expr 0.5*$mom_post_d]] Y0.0 Z[format %.3f $mom_post_z] F100.0\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#501=#5161\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#502=#5162\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#503=#5163\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 X0.0 Y0.0 Z[format %.3f $mom_post_z] F[format %.3f $mom_post_f]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X0.0 Y[format %.3f [expr 0.5*$mom_post_d]] Z[format %.3f $mom_post_z] F100.0\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#511=#5161\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#512=#5162\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#513=#5163\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 X0.0 Y0.0 Z[format %.3f $mom_post_z] F[format %.3f $mom_post_f]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G31 X[format %.3f [expr -0.5*$mom_post_d]] Y0.0 Z[format %.3f $mom_post_z] F100.0\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#521=#5161\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#522=#5162\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#523=#5163\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"G1 X0.0 Y0.0 Z[format %.3f $mom_post_z] F[format %.3f $mom_post_f]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#1=\[#501-#511\]*\[#501-#511\]+\[#502-#512\]*\[#502-#512\]+\[#503-#513\]*\[#503-#513\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#2=\[#501-#521\]*\[#501-#521\]+\[#502-#522\]*\[#502-#522\]+\[#503-#523\]*\[#503-#523\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#3=\[#511-#521\]*\[#511-#521\]+\[#512-#522\]*\[#512-#522\]+\[#513-#523\]*\[#513-#523\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#4=SQRT\[2.0*#1*#2+2.0*#1*#3+2.0*#3*#2-#1*#1-#2*#2-#3*#3\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#4=2.0*2.978+2.0*SQRT\[#1*#2*#3\]/#4\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#516=[format %.4f $mom_post_z]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#5=#3*\[#1+#2-#3\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#6=#2*\[#1+#3-#2\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#7=#1*\[#2+#3-#1\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#8=#5+#6+#7\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#527=\[#501*#5 + #511*#6 + #521*#7\]/#8\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#528=\[#502*#5 + #512*#6 + #522*#7\]/#8\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#529=\[#503*#5 + #513*#6 + #523*#7\]/#8\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#520=SQRT\[#527*#527+#528*#528\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#530=SQRT\[\[#527-#517\]*\[#527-#517\] + \[#528-#518\]*\[#528-#518\] + \[#529-#519\]*\[#529-#519\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"#531=ACOS\[\[#519 - #529\]/#530\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"+++++++++++++++++++++++++++++++++\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[MEASURED*DIAMETER*FOR*Z=#516\[44\]*=*#4\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[ARC\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[POINT***#501\[44\]***#502\[44\]***#503\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[POINT***#511\[44\]***#512\[44\]***#513\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[POINT***#521\[44\]***#522\[44\]***#523\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[XC*#527\[44\]***YC*#528\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[CENTER*DEVIATION***#520\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[AXIS***DEVIATION***#531\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"DPRNT\[********#600\[44\]***#514\[44\]***#515\[44\]\]\"");
     UF_MOM_execute_command(mom_id, "MOM_output_literal \"++++++++\"");

  break ;

  default :
    UF_MOM_set_string(mom_id, "var_unknow", "0");
  break ;
  }

  UF_terminate();

  return ( 0 ) ;
}

