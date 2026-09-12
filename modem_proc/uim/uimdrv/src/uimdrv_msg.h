#ifndef UIMDRV_MSG_H
#define UIMDRV_MSG_H
/*===========================================================================


           U I M DRIVER  L O G   H E A D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.  
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: 
 
when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
07/20/16   ks      Correcting the MSG MACROs
06/23/16   sam     Added support for additional aruments fro error messages
05/23/16   na      Support for additional arg
08/04/15   sam     Support of Powerup logging
07/31/14   ak      Feature flags cleanup
29/01/14   sam     Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

#include "uim_msg.h"
#include "uim_powerup_logging.h"


/*=============================================================================

                          UIM MSG MACROS
               MACRO Definitions used for logging throughout UIM

=============================================================================*/

/*Slot specific UIMDRV HIGH message macro definitions*/
#define UIMDRV_MSG_HIGH_0(sub, str) \
  UIM_MSG_HIGH_1("UIM_%d: " str , (sub+1))

#define UIMDRV_MSG_HIGH_1(sub, str, p1) \
  UIM_MSG_HIGH_2("UIM_%d: " str , (sub+1), (p1))

#define UIMDRV_MSG_HIGH_2(sub, str, p1, p2) \
  UIM_MSG_HIGH_3("UIM_%d: " str , (sub+1), (p1), (p2))

#define UIMDRV_MSG_HIGH_3(sub, str, p1, p2, p3) \
  UIM_MSG_HIGH_4("UIM_%d: " str , (sub+1), (p1), (p2), (p3))

#define UIMDRV_MSG_HIGH_4(sub, str, p1, p2, p3, p4) \
  UIM_MSG_HIGH_5("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4))

#define UIMDRV_MSG_HIGH_5(sub, str, p1, p2, p3, p4, p5) \
  UIM_MSG_HIGH_6("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5))

#define UIMDRV_MSG_HIGH_6(sub, str, p1, p2, p3, p4, p5, p6) \
  UIM_MSG_HIGH_7("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5), (p6))

#define UIMDRV_MSG_HIGH_7(sub, str, p1, p2, p3, p4, p5, p6, p7) \
  UIM_MSG_HIGH_8("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5), (p6), (p7))


/*Slot specific UIMDRV MED message macro definitions*/
#define UIMDRV_MSG_MED_0(sub, str) \
  UIM_MSG_MED_1("UIM_%d: " str , (sub+1))

#define UIMDRV_MSG_MED_1(sub, str, p1) \
  UIM_MSG_MED_2("UIM_%d: " str , (sub+1), (p1))

#define UIMDRV_MSG_MED_2(sub, str, p1, p2) \
  UIM_MSG_MED_3("UIM_%d: " str , (sub+1), (p1), (p2))

#define UIMDRV_MSG_MED_3(sub, str, p1, p2, p3) \
  UIM_MSG_MED_4("UIM_%d: " str , (sub+1), (p1), (p2), (p3))

#define UIMDRV_MSG_MED_4(sub, str, p1, p2, p3, p4) \
  UIM_MSG_MED_5("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4))

#define UIMDRV_MSG_MED_5(sub, str, p1, p2, p3, p4, p5) \
  UIM_MSG_MED_6("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5))

#define UIMDRV_MSG_MED_6(sub, str, p1, p2, p3, p4, p5, p6) \
  UIM_MSG_MED_7("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5), (p6))

#define UIMDRV_MSG_MED_7(sub, str, p1, p2, p3, p4, p5, p6, p7) \
  UIM_MSG_MED_8("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5), (p6), (p7))

#define UIMDRV_MSG_MED_8(sub, str, p1, p2, p3, p4, p5, p6, p7, p8) \
  UIM_MSG_MED_9("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8))


/*Slot specific UIMDRV LOW message macro definitions*/
#define UIMDRV_MSG_LOW_0(sub, str) \
  UIM_MSG_LOW_1("UIM_%d: " str , (sub+1))

#define UIMDRV_MSG_LOW_1(sub, str, p1) \
  UIM_MSG_LOW_2("UIM_%d: " str , (sub+1), (p1))

#define UIMDRV_MSG_LOW_2(sub, str, p1, p2) \
  UIM_MSG_LOW_3("UIM_%d: " str , (sub+1), (p1), (p2))

#define UIMDRV_MSG_LOW_3(sub, str, p1, p2, p3) \
  UIM_MSG_LOW_4("UIM_%d: " str , (sub+1), (p1), (p2), (p3))

#define UIMDRV_MSG_LOW_4(sub, str, p1, p2, p3, p4) \
  UIM_MSG_LOW_5("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4))

#define UIMDRV_MSG_LOW_5(sub, str, p1, p2, p3, p4, p5) \
  UIM_MSG_LOW_6("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5))

#define UIMDRV_MSG_LOW_6(sub, str, p1, p2, p3, p4, p5, p6) \
  UIM_MSG_LOW_7("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5), (p6))

#define UIMDRV_MSG_LOW_7(sub, str, p1, p2, p3, p4, p5, p6, p7) \
  UIM_MSG_LOW_8("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5), (p6), (p7))
  


/*Slot specific UIMDRV ERROR message macro definitions*/
#define UIMDRV_MSG_ERR_0(sub, str) \
  UIM_MSG_ERR_1("UIM_%d: " str , (sub+1))

#define UIMDRV_MSG_ERR_1(sub, str, p1) \
  UIM_MSG_ERR_2("UIM_%d: " str , (sub+1), (p1))

#define UIMDRV_MSG_ERR_2(sub, str, p1, p2) \
  UIM_MSG_ERR_3("UIM_%d: " str , (sub+1), (p1), (p2))

#define UIMDRV_MSG_ERR_3(sub, str, p1, p2, p3) \
  UIM_MSG_ERR_4("UIM_%d: " str , (sub+1), (p1), (p2), (p3))

#define UIMDRV_MSG_ERR_4(sub, str, p1, p2, p3, p4) \
  UIM_MSG_ERR_5("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4))

#define UIMDRV_MSG_ERR_5(sub, str, p1, p2, p3, p4, p5) \
  UIM_MSG_ERR_6("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5))

#define UIMDRV_MSG_ERR_6(sub, str, p1, p2, p3, p4, p5, p6) \
  UIM_MSG_ERR_7("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5), (p6))

#define UIMDRV_MSG_ERR_7(sub, str, p1, p2, p3, p4, p5, p6, p7) \
  UIM_MSG_ERR_8("UIM_%d: " str , (sub+1), (p1), (p2), (p3), (p4), (p5), (p6), (p7))


#define MSG_FATAL_UIM( sub , str, p1, p2, p3)\
MSG_4 (MSG_SSID_DFLT, MSG_LEGACY_FATAL, "UIM_%d" str , (sub+1), (p1), (p2), (p3))

#define UIMDRV_PUP_MSG_HIGH_0(t, sub, str) \
  UIM_PUP_MSG_HIGH_1(t,  "UIM_%d: " str , (sub+1))

#define UIMDRV_PUP_MSG_HIGH_1(t, sub, str, p1) \
  UIM_PUP_MSG_HIGH_2(t, "UIM_%d: " str , (sub+1), (p1))

#define UIMDRV_PUP_MSG_HIGH_2(t, sub, str, p1, p2) \
  UIM_PUP_MSG_HIGH_3(t, "UIM_%d: " str , (sub+1), (p1), (p2))

#define UIMDRV_PUP_MSG_HIGH_3(t, sub, str, p1, p2, p3) \
  UIM_PUP_MSG_HIGH_4(t, "UIM_%d: " str , (sub+1), (p1), (p2), (p3))

#ifdef FEATURE_UIM_POWERUP_LOGGING
#define UIM_PUP_MSG_HIGH_0(t, xx_fmt) \
  UIM_MSG_HIGH_0(xx_fmt);\
  (void)uim_pup_log_add_0 ((uint16)t)

#define UIM_PUP_MSG_HIGH_1(t, xx_fmt, xx_arg1) \
  UIM_MSG_HIGH_1(xx_fmt, xx_arg1);\
  (void)uim_pup_log_add_1 ((uim_pup_enum)t,(uint32) (xx_arg1))
	
#define UIM_PUP_MSG_HIGH_2(t, xx_fmt, xx_arg1, xx_arg2) \
  UIM_MSG_HIGH_2(xx_fmt, xx_arg1, xx_arg2);\
  (void)uim_pup_log_add_2((uim_pup_enum)t,(uint32) (xx_arg1), (uint32) (xx_arg2))

#define UIM_PUP_MSG_HIGH_3(t, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  UIM_MSG_HIGH_3( xx_fmt, xx_arg1, xx_arg2, xx_arg3);\
  (void)uim_pup_log_add_3((uim_pup_enum)t,(uint32) (xx_arg1), (uint32) (xx_arg2), (uint32) (xx_arg3))
	  
#define UIM_PUP_MSG_HIGH_4(t, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  UIM_MSG_HIGH_4(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4);\
  (void)uim_pup_log_add_4((uim_pup_enum)t,(uint32) (xx_arg1), (uint32) (xx_arg2), (uint32) (xx_arg3), (uint32) (xx_arg4))

#else
#define UIM_PUP_MSG_HIGH_0(t, xx_fmt) \
  UIM_MSG_HIGH_0(xx_fmt)

#define UIM_PUP_MSG_HIGH_1(t, xx_fmt, xx_arg1) \
  UIM_MSG_HIGH_1(xx_fmt, xx_arg1)
	
#define UIM_PUP_MSG_HIGH_2(t, xx_fmt, xx_arg1, xx_arg2) \
  UIM_MSG_HIGH_2(xx_fmt, xx_arg1, xx_arg2)

#define UIM_PUP_MSG_HIGH_3(t, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  UIM_MSG_HIGH_3( xx_fmt, xx_arg1, xx_arg2, xx_arg3)
	  
#define UIM_PUP_MSG_HIGH_4(t, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  UIM_MSG_HIGH_4(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)
#endif /*FEATURE_UIM_POWERUP_LOGGING*/


#define UIMDRV_PUP_MSG_ERR_0(t, sub, str) \
  UIM_PUP_MSG_ERR_1(t, "UIM_%d: " str , (sub+1))

#define UIMDRV_PUP_MSG_ERR_1(t, sub, str, p1) \
  UIM_PUP_MSG_ERR_2(t, "UIM_%d: " str , (sub+1), (p1))

#define UIMDRV_PUP_MSG_ERR_2(t, sub, str, p1, p2) \
  UIM_PUP_MSG_ERR_3(t, "UIM_%d: " str , (sub+1), (p1), (p2))

#define UIMDRV_PUP_MSG_ERR_3(t, sub, str, p1, p2, p3) \
  UIM_PUP_MSG_ERR_4(t, "UIM_%d: " str , (sub+1), (p1), (p2), (p3))


#ifdef FEATURE_UIM_POWERUP_LOGGING
#define UIM_PUP_MSG_ERR_0(t, xx_fmt) \
  UIM_MSG_ERR_0(xx_fmt); \
  (void)uim_pup_log_add_0((uim_pup_enum)t)

#define UIM_PUP_MSG_ERR_1(t, xx_fmt, xx_arg1) \
  UIM_MSG_ERR_1(xx_fmt, xx_arg1); \
  (void)uim_pup_log_add_1((uim_pup_enum)t,(uint32) (xx_arg1))
  
#define UIM_PUP_MSG_ERR_2(t, xx_fmt, xx_arg1, xx_arg2) \
  UIM_MSG_ERR_2(xx_fmt, xx_arg1, xx_arg2); \
  (void)uim_pup_log_add_2((uim_pup_enum)t,(uint32) (xx_arg1), (uint32) (xx_arg2))
  
#define UIM_PUP_MSG_ERR_3(t, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  UIM_MSG_ERR_3(xx_fmt, xx_arg1, xx_arg2, xx_arg3); \
  (void)uim_pup_log_add_3((uim_pup_enum)t,(uint32) (xx_arg1), (uint32) (xx_arg2), (uint32) (xx_arg3))

#define UIM_PUP_MSG_ERR_4(t, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  UIM_MSG_ERR_4(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4); \
  (void)uim_pup_log_add_4((uim_pup_enum)t,(uint32) (xx_arg1), (uint32) (xx_arg2), (uint32) (xx_arg3), (uint32) (xx_arg4))

#else
#define UIM_PUP_MSG_ERR_0(t, xx_fmt) \
  UIM_MSG_ERR_0(xx_fmt)

#define UIM_PUP_MSG_ERR_1(t, xx_fmt, xx_arg1) \
  UIM_MSG_ERR_1(xx_fmt, xx_arg1)
  
#define UIM_PUP_MSG_ERR_2(t, xx_fmt, xx_arg1, xx_arg2) \
  UIM_MSG_ERR_2(xx_fmt, xx_arg1, xx_arg2)
  
#define UIM_PUP_MSG_ERR_3(t, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
  UIM_MSG_ERR_3(xx_fmt, xx_arg1, xx_arg2, xx_arg3)

#define UIM_PUP_MSG_ERR_4(t, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4) \
  UIM_MSG_ERR_4(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)
#endif /*FEATURE_UIM_POWERUP_LOGGING*/

#define UIM_ERROR_MSG_0(str) UIM_MSG_ERR_0(str)
#define UIM_ERROR_MSG_1(str,a) UIM_MSG_ERR_1(str,a)
#define UIM_ERROR_MSG_2(str,a,b) UIM_MSG_ERR_2(str,a,b)
#define UIM_ERROR_MSG_3(str,a,b,c) UIM_MSG_ERR_3(str,a,b,c)

#define UIM_PUP_ERROR_MSG_0(t, str) UIM_PUP_MSG_ERR_0(t, str)
#define UIM_PUP_ERROR_MSG_1(t, str,a) UIM_PUP_MSG_ERR_1(t, str,a)
#define UIM_PUP_ERROR_MSG_2(t, str,a,b) UIM_PUP_MSG_ERR_2(t, str,a,b)
#define UIM_PUP_ERROR_MSG_3(t, str,a,b,c) UIM_PUP_MSG_ERR_3(t, str,a,b,c)

#define UIMDRV_USB_MSG_ERR_0(str)   UIM_MSG_ERR_0("UIM USB:" str)
#define UIMDRV_USB_MSG_ERR_1(str,a)   UIM_MSG_ERR_1("UIM USB:" str,a,0)
#define UIMDRV_USB_MSG_ERR_2(str,a,b)   UIM_MSG_ERR_2("UIM USB:" str,a,b)
#define UIMDRV_USB_MSG_ERR_3(str,a,b,c)   UIM_MSG_ERR_3("UIM USB:" str,a,b,c)

#ifdef FEATURE_UIM_POWERUP_LOGGING
#define PUP_MSG_HIGH_2(t, xx_fmt, xx_arg1, xx_arg2) \
  (void)uim_pup_log_add_2((uim_pup_enum)t,(uint32) (xx_arg1), (uint32) (xx_arg2))

#else
#define PUP_MSG_HIGH_2(t, xx_fmt, xx_arg1, xx_arg2)

#endif /*FEATURE_UIM_POWERUP_LOGGING*/


#endif /* UIMDRV_MSG_H */
