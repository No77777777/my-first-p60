/*!
  @file
  ds_3gppi_utils.h

  @brief
  Internal utility functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009 - 2019 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/inc/ds_3gppi_utils.h#9 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/02/13   vs      NV item to check call origination is allowed before PS attach
03/11/13   nd      Added NV item for Dual-IP fallback cause codes.
11/22/12   nd      Added NV item ds_3gpp_num_persistent_profiles.
11/22/12   nd      Added NV item ds_3gpp_send_ps_data_avail_on_power_save.
08/13/12   ss      Moving efs write operation to dsutil performed during 
                   bootup.
08/08/12   ss      Added NV item 3gpp_tcp_threshold_ack_cnt.
02/23/12   kv      Replaced FEATURE_DATA_TEST_LOOPBACK_HDLR with
                   FEATURE_DATA_TEST_LOOPBACK.
11/22/11   sa      Added NV item 3gpp_auto_answer_enable.
07/15/11   sa      Added NV item 3gpp_test_loopback_enable.
06/06/11   sa      Added NV item 3gpp_data_pkt_orig_str.
05/25/11   sa      Added support for 3gpp_rel_version NV.
04/04/10   vk      Added support for attach profile configuration
04/03/10   vk      Added support for MSG2.0 based logging
04/02/10   vrk     Added NO_ARG macro for compilation issues.
01/23/10   vk      Initial version

===========================================================================*/

#ifndef DS_3GPPI_UTILS_H
#define DS_3GPPI_UTILS_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "sys.h"
#include "modem_mem.h"
#include "data_msg.h"
#include "dsutil.h"
#include "ds_profile_3gpp.h"
#include "ds_3gpp_roaming_hdlr.h"
#include "ds3gmmgsdiif.h"
#include "ds_3gpp_pre_attach_hdlr.h"
#include "ps_sys_event.h"
#include "dsumtspdpreg.h"
/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*---------------------------------------------------------------------------
  Logging utilities: 3GPP Controlled messages for f3 supression
---------------------------------------------------------------------------*/

#define DS_3GPP_C_MSG0(ds_f3_mask, fmtstr)           \
{\
  if( ds_f3_mask & ds_nv_read_mask ) \
  {                 \
     DATA_3GPP_MSG0(MSG_LEGACY_HIGH, fmtstr); \
  } \
  else\
  { \
     DATA_3GPP_MSG0(MSG_LEGACY_LOW, fmtstr);  \
  }\
}

#define DS_3GPP_C_MSG1(ds_f3_mask, fmtstr, arg1)          \
{\
  if( ds_f3_mask & ds_nv_read_mask )\
  {                      \
    DATA_3GPP_MSG1(MSG_LEGACY_HIGH, fmtstr, arg1); \
  }\
  else\
  {                                        \
    DATA_3GPP_MSG1(MSG_LEGACY_LOW, fmtstr, arg1)  ;\
  }\
}

#define DS_3GPP_C_MSG2(ds_f3_mask, fmtstr, arg1, arg2)          \
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                           \
    DATA_3GPP_MSG2(MSG_LEGACY_HIGH, fmtstr, arg1, arg2) ;\
  }\
  else\
  {                                              \
    DATA_3GPP_MSG2(MSG_LEGACY_LOW, fmtstr, arg1, arg2)  ;\
  }\
}

#define DS_3GPP_C_MSG3(ds_f3_mask, fmtstr, arg1, arg2, arg3)          \
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                                 \
    DATA_3GPP_MSG3(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3); \
  }\
  else\
  {                                                    \
    DATA_3GPP_MSG3(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3)  ;\
  }\
}

#define DS_3GPP_C_MSG4(ds_f3_mask, fmtstr, arg1, arg2, arg3, arg4)          \
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                                      \
    DATA_3GPP_MSG4(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4);\
  }\
  else\
  {                                                         \
    DATA_3GPP_MSG4(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4) ;\
  }\
}

#define DS_3GPP_C_MSG5(ds_f3_mask, fmtstr, arg1, arg2, arg3, arg4, arg5) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_3GPP_MSG5(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5);\
  }\
  else\
  {                                                         \
    DATA_3GPP_MSG5(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5);\
  }\
}

#define DS_3GPP_C_MSG6(ds_f3_mask, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_3GPP_MSG6(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6);\
  }\
  else\
  {                                                         \
    DATA_3GPP_MSG6(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6);\
  }\
}

#define DS_3GPP_C_MSG7(ds_f3_mask, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6,\
                                                                        arg7) \
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                                      \
    DATA_3GPP_MSG7(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5, \
                                                                  arg6, arg7);\
  } \
  else\
  {                                                         \
    DATA_3GPP_MSG7(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5, \
                                                                  arg6, arg7);\
  }\
}

#define DS_3GPP_C_MSG8(ds_f3_mask, fmtstr, arg1, arg2, arg3, arg4, arg5, \
                                                            arg6, arg7, arg8) \
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                                      \
    DATA_3GPP_MSG8(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5, \
                                                            arg6, arg7, arg8);\
  }\
  else\
  { \
    DATA_3GPP_MSG8(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5, \
                                                            arg6, arg7, arg8);\
  }\
}

#define DS_3GPP_C_MSG9(ds_f3_mask, fmtstr, arg1, arg2, arg3, arg4, arg5, \
                                                      arg6, arg7, arg8, arg9) \
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                                                                    \
    DATA_3GPP_MSG9(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5, \
                                                      arg6, arg7, arg8, arg9);\
  }\
  else\
  {                                                                                       \
    DATA_3GPP_MSG9(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5, \
                                                     arg6, arg7, arg8, arg9); \
  }\
}


/*===========================================================================
  3GPP SS F3 supression related macros to support SPRINTF functionality
===========================================================================*/

#define DATA_3GPP_C_MSG_SPRINTF_1(ds_f3_mask, xx_fmt, xx_arg1) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_3GPP_MSG_SPRINTF_1(MSG_LEGACY_HIGH, xx_fmt, xx_arg1);\
  }\
  else\
  {                                                         \
    DATA_3GPP_MSG_SPRINTF_1(MSG_LEGACY_LOW, xx_fmt, xx_arg1);\
  }\
}
/*---------------------------------------------------------------------------
  For F3 supression. This is the macro for sprintf messages with 2 parameters
---------------------------------------------------------------------------*/
#define DATA_3GPP_C_MSG_SPRINTF_2(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2) \
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                                      \
    DATA_3GPP_MSG_SPRINTF_2(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2);\
  }\
  else\
  {                                                         \
    DATA_3GPP_MSG_SPRINTF_2(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2);\
  }\
}
/*---------------------------------------------------------------------------
  For F3 supression. This is the macro for sprintf messages with 3 parameters
---------------------------------------------------------------------------*/
#define DATA_3GPP_C_MSG_SPRINTF_3(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3)\
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_3GPP_MSG_SPRINTF_3(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3);\
  } \
  else\
  {                                                         \
    DATA_3GPP_MSG_SPRINTF_3(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3);\
  }\
}
/*---------------------------------------------------------------------------
  For F3 supression. This is the macro for sprintf messages with 4 parameters
---------------------------------------------------------------------------*/
#define DATA_3GPP_C_MSG_SPRINTF_4(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                                      xx_arg4)\
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                                      \
    DATA_3GPP_MSG_SPRINTF_4(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                                      xx_arg4);\
  }\
  else\
  {                                                         \
    DATA_3GPP_MSG_SPRINTF_4(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                                      xx_arg4);\
  }\
}

//--
/*===========================================================================
  DS707 SS F3 supression related macros to support SPRINTF functionality
===========================================================================*/

#define DATA_IS707_C_MSG_SPRINTF_1(ds_f3_mask, xx_fmt, xx_arg1) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_IS707_MSG_SPRINTF_1(MSG_LEGACY_HIGH, xx_fmt, xx_arg1);\
  }\
  else\
  {                                                         \
    DATA_IS707_MSG_SPRINTF_1(MSG_LEGACY_LOW, xx_fmt, xx_arg1);\
  }\
}
/*---------------------------------------------------------------------------
  For F3 supression. This is the macro for sprintf messages with 2 parameters
---------------------------------------------------------------------------*/
#define DATA_IS707_C_MSG_SPRINTF_2(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2) \
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                                      \
    DATA_IS707_MSG_SPRINTF_2(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2);\
  }\
  else\
  {                                                         \
    DATA_IS707_MSG_SPRINTF_2(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2);\
  }\
}
/*---------------------------------------------------------------------------
  For F3 supression. This is the macro for sprintf messages with 3 parameters
---------------------------------------------------------------------------*/
#define DATA_IS707_C_MSG_SPRINTF_3(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3)\
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_IS707_MSG_SPRINTF_3(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3);\
  } \
  else\
  {                                                         \
    DATA_IS707_MSG_SPRINTF_3(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3);\
  }\
}
/*---------------------------------------------------------------------------
  For F3 supression. This is the macro for sprintf messages with 4 parameters
---------------------------------------------------------------------------*/
#define DATA_IS707_C_MSG_SPRINTF_4(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                                      xx_arg4)\
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                                      \
    DATA_IS707_MSG_SPRINTF_4(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                                      xx_arg4);\
  }\
  else\
  {                                                         \
    DATA_IS707_MSG_SPRINTF_4(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                                      xx_arg4);\
  }\
}



/*---------------------------------------------------------------------------
  Logging utilities: DS707 Controlled messages for f3 supression
---------------------------------------------------------------------------*/

#define DATA_IS707_C_MSG0(ds_f3_mask, xx_fmt)           \
{\
  if( ds_f3_mask & ds_nv_read_mask )\
  {                 \
    DATA_IS707_MSG0(MSG_LEGACY_HIGH, xx_fmt); \
  }\
  else\
  {                                   \
    DATA_IS707_MSG0(MSG_LEGACY_LOW, xx_fmt);  \
  }\
}

#define DATA_IS707_C_MSG1(ds_f3_mask, xx_fmt, xx_arg1)          \
{\
  if( ds_f3_mask & ds_nv_read_mask ) \
  {                      \
    DATA_IS707_MSG1(MSG_LEGACY_HIGH, xx_fmt, xx_arg1); \
  } \
  else\
  {                                        \
    DATA_IS707_MSG1(MSG_LEGACY_LOW, xx_fmt, xx_arg1)  ;\
  }\
}

#define DATA_IS707_C_MSG2(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2) \
{\
  if ( ds_f3_mask & ds_nv_read_mask )\
  {                           \
    DATA_IS707_MSG2(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2) ;\
  }\
  else\
  {                                              \
    DATA_IS707_MSG2(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2)  ;\
  }\
}

#define DATA_IS707_C_MSG3(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                 \
    DATA_IS707_MSG3(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3); \
  } \
  else\
  {                                                    \
    DATA_IS707_MSG3(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3)  ;\
  }\
}

#define DATA_IS707_C_MSG4(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)\
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_IS707_MSG4(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4 );\
  } \
  else \
  {                                                         \
    DATA_IS707_MSG4(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4 );\
  }\
}

#define DATA_IS707_C_MSG5(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4,\
                                                               xx_arg5) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_IS707_MSG5(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                            xx_arg4, xx_arg5);\
  } \
  else \
  {                                                         \
    DATA_IS707_MSG5(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                            xx_arg4, xx_arg5);\
  }\
}


#define DATA_IS707_C_MSG6(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4,\
                                                            xx_arg5, xx_arg6) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_IS707_MSG6(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                   xx_arg4, xx_arg5, xx_arg6);\
  } \
  else \
  {                                                         \
    DATA_IS707_MSG6(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                                   xx_arg4, xx_arg5, xx_arg6);\
  }\
}

#define DATA_IS707_C_MSG7(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4,\
                                                   xx_arg5, xx_arg6, xx_arg7) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_IS707_MSG7(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                          xx_arg4, xx_arg5, xx_arg6, xx_arg7);\
  } \
  else \
  {                                                         \
    DATA_IS707_MSG7(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                          xx_arg4, xx_arg5, xx_arg6, xx_arg7);\
  }\
}


#define DATA_IS707_C_MSG8(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4,\
                                          xx_arg5, xx_arg6, xx_arg7, xx_arg8) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_IS707_MSG8(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                 xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8);\
  } \
  else \
  {                                                         \
    DATA_IS707_MSG8(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                                 xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8);\
  }\
}


#define DATA_IS707_C_MSG9(ds_f3_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4,\
                                 xx_arg5, xx_arg6, xx_arg7, xx_arg8, xx_arg9) \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_IS707_MSG9(MSG_LEGACY_HIGH, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                        xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8, xx_arg9);\
  } \
  else \
  {                                                         \
    DATA_IS707_MSG9(MSG_LEGACY_LOW, xx_fmt, xx_arg1, xx_arg2, xx_arg3,\
                        xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8, xx_arg9);\
  }\
}


/*---------------------------------------------------------------------------
  Logging utilities: 3GPP
---------------------------------------------------------------------------*/


#define DS_3GPP_MSG9_LOW(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
				  DATA_3GPP_MSG9(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
		
#define DS_3GPP_MSG9_MED(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
				  DATA_3GPP_MSG9(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
		
#define DS_3GPP_MSG9_HIGH(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
          DATA_3GPP_MSG9(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
		
#define DS_3GPP_MSG9_ERROR(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
				  DATA_3GPP_MSG9(MSG_LEGACY_ERROR, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

	
#define DS_3GPP_MSG8_LOW(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
			  DATA_3GPP_MSG8(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	
#define DS_3GPP_MSG8_MED(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
			  DATA_3GPP_MSG8(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	
#define DS_3GPP_MSG8_HIGH(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
          DATA_3GPP_MSG8(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	
#define DS_3GPP_MSG8_ERROR(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
			  DATA_3GPP_MSG8(MSG_LEGACY_ERROR, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)



#define DS_3GPP_MSG7_LOW(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
          DATA_3GPP_MSG7(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define DS_3GPP_MSG7_MED(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
          DATA_3GPP_MSG7(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define DS_3GPP_MSG7_HIGH(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
          DATA_3GPP_MSG7(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define DS_3GPP_MSG7_ERROR(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
          DATA_3GPP_MSG7(MSG_LEGACY_ERROR, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7)


#define DS_3GPP_MSG6_LOW(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6) \
          DATA_3GPP_MSG6(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6)

#define DS_3GPP_MSG6_MED(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6) \
          DATA_3GPP_MSG6(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6)

#define DS_3GPP_MSG6_HIGH(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6) \
          DATA_3GPP_MSG6(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6)

#define DS_3GPP_MSG6_ERROR(fmtstr, arg1, arg2, arg3, arg4, arg5, arg6) \
          DATA_3GPP_MSG6(MSG_LEGACY_ERROR, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6)


#define DS_3GPP_MSG5_LOW(fmtstr, arg1, arg2, arg3, arg4, arg5) \
          DATA_3GPP_MSG5(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5)

#define DS_3GPP_MSG5_MED(fmtstr, arg1, arg2, arg3, arg4, arg5) \
          DATA_3GPP_MSG5(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3, arg4, arg5)

#define DS_3GPP_MSG5_HIGH(fmtstr, arg1, arg2, arg3, arg4, arg5) \
          DATA_3GPP_MSG5(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5)

#define DS_3GPP_MSG5_ERROR(fmtstr, arg1, arg2, arg3, arg4, arg5) \
          DATA_3GPP_MSG5(MSG_LEGACY_ERROR, fmtstr, arg1, arg2, arg3, arg4, arg5)


#define DS_3GPP_MSG4_LOW(fmtstr, arg1, arg2, arg3, arg4) \
          DATA_3GPP_MSG4(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4)

#define DS_3GPP_MSG4_MED(fmtstr, arg1, arg2, arg3, arg4) \
          DATA_3GPP_MSG4(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3, arg4)

#define DS_3GPP_MSG4_HIGH(fmtstr, arg1, arg2, arg3, arg4) \
          DATA_3GPP_MSG4(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4)

#define DS_3GPP_MSG4_ERROR(fmtstr, arg1, arg2, arg3, arg4) \
          DATA_3GPP_MSG4(MSG_LEGACY_ERROR, fmtstr, arg1, arg2, arg3, arg4)
          

#define DS_3GPP_MSG3_LOW(fmtstr, arg1, arg2, arg3) \
          DATA_3GPP_MSG3(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3)

#define DS_3GPP_MSG3_MED(fmtstr, arg1, arg2, arg3) \
          DATA_3GPP_MSG3(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3)

#define DS_3GPP_MSG3_HIGH(fmtstr, arg1, arg2, arg3) \
          DATA_3GPP_MSG3(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3)

#define DS_3GPP_MSG3_ERROR(fmtstr, arg1, arg2, arg3) \
          DATA_3GPP_MSG3(MSG_LEGACY_ERROR, fmtstr, arg1, arg2, arg3)


#define DS_3GPP_MSG2_LOW(fmtstr, arg1, arg2) \
          DATA_3GPP_MSG2(MSG_LEGACY_LOW, fmtstr, arg1, arg2)

#define DS_3GPP_MSG2_MED(fmtstr, arg1, arg2) \
          DATA_3GPP_MSG2(MSG_LEGACY_MED, fmtstr, arg1, arg2)

#define DS_3GPP_MSG2_HIGH(fmtstr, arg1, arg2) \
          DATA_3GPP_MSG2(MSG_LEGACY_HIGH, fmtstr, arg1, arg2)

#define DS_3GPP_MSG2_ERROR(fmtstr, arg1, arg2) \
          DATA_3GPP_MSG2(MSG_LEGACY_ERROR, fmtstr, arg1, arg2)


#define DS_3GPP_MSG1_LOW(fmtstr, arg1) \
          DATA_3GPP_MSG1(MSG_LEGACY_LOW, fmtstr, arg1)

#define DS_3GPP_MSG1_MED(fmtstr, arg1) \
          DATA_3GPP_MSG1(MSG_LEGACY_MED, fmtstr, arg1)

#define DS_3GPP_MSG1_HIGH(fmtstr, arg1) \
          DATA_3GPP_MSG1(MSG_LEGACY_HIGH, fmtstr, arg1)

#define DS_3GPP_MSG1_ERROR(fmtstr, arg1) \
          DATA_3GPP_MSG1(MSG_LEGACY_ERROR, fmtstr, arg1)

#define DS_3GPP_MSG0_ERROR(fmtstr) \
          DATA_3GPP_MSG0(MSG_LEGACY_ERROR, fmtstr)

#define DS_3GPP_MSG0_LOW(fmtstr) \
          DATA_3GPP_MSG0(MSG_LEGACY_LOW, fmtstr)

#define DS_3GPP_MSG0_MED(fmtstr) \
          DATA_3GPP_MSG0(MSG_LEGACY_MED, fmtstr)

#define DS_3GPP_MSG0_HIGH(fmtstr) \
          DATA_3GPP_MSG0(MSG_LEGACY_HIGH, fmtstr)

#define DS_3GPP_MSG0_ERROR(fmtstr) \
          DATA_3GPP_MSG0(MSG_LEGACY_ERROR, fmtstr)

	
#define DS_3GPP_MSG9_HIGH_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
          DATA_3GPP_MSG9_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
	
#define DS_3GPP_MSG8_HIGH_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
          DATA_3GPP_MSG8_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#define DS_3GPP_MSG7_HIGH_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
          DATA_3GPP_MSG7_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define DS_3GPP_MSG6_HIGH_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6) \
          DATA_3GPP_MSG6_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6)

#define DS_3GPP_MSG5_HIGH_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5) \
          DATA_3GPP_MSG5_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5)

#define DS_3GPP_MSG4_HIGH_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4) \
          DATA_3GPP_MSG4_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2, arg3, arg4)

#define DS_3GPP_MSG3_HIGH_EX(subs_id, fmtstr, arg1, arg2, arg3) \
          DATA_3GPP_MSG3_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2, arg3)

#define DS_3GPP_MSG2_HIGH_EX(subs_id, fmtstr, arg1, arg2) \
          DATA_3GPP_MSG2_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2)

#define DS_3GPP_MSG1_HIGH_EX(subs_id, fmtstr, arg1) \
          DATA_3GPP_MSG1_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1)

#define DS_3GPP_MSG0_HIGH_EX(subs_id, fmtstr) \
          DATA_3GPP_MSG0_EX(MSG_LEGACY_HIGH, subs_id, fmtstr)
          
#define DS_3GPP_MSG9_MED_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
            DATA_3GPP_MSG9_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
    
#define DS_3GPP_MSG8_MED_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
            DATA_3GPP_MSG8_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
  
#define DS_3GPP_MSG7_MED_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
            DATA_3GPP_MSG7_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
  
#define DS_3GPP_MSG6_MED_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6) \
            DATA_3GPP_MSG6_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6)
  
#define DS_3GPP_MSG5_MED_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5) \
            DATA_3GPP_MSG5_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3, arg4, arg5)
  
#define DS_3GPP_MSG4_MED_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4) \
            DATA_3GPP_MSG4_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3, arg4)
  
#define DS_3GPP_MSG3_MED_EX(subs_id, fmtstr, arg1, arg2, arg3) \
            DATA_3GPP_MSG3_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3)
  
#define DS_3GPP_MSG2_MED_EX(subs_id, fmtstr, arg1, arg2) \
            DATA_3GPP_MSG2_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2)
  
#define DS_3GPP_MSG1_MED_EX(subs_id, fmtstr, arg1) \
            DATA_3GPP_MSG1_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1)
  
#define DS_3GPP_MSG0_MED_EX(subs_id, fmtstr) \
            DATA_3GPP_MSG0_EX(MSG_LEGACY_MED, subs_id, fmtstr)

/*---------------------------------------------------------------------------
  Logging utilities: LTE CMSG
---------------------------------------------------------------------------*/

#define DS_LTE_C_MSG0(ds_f3_mask, fmtstr)           \
{\
  if( ds_f3_mask & ds_nv_read_mask ) \
  {                 \
    DATA_LTE_MSG0(MSG_LEGACY_HIGH, fmtstr); \
  } \
  else\
  {                                   \
    DATA_LTE_MSG0(MSG_LEGACY_LOW, fmtstr);  \
  }\
}

#define DS_LTE_C_MSG1(ds_f3_mask, fmtstr, arg1)          \
{\
  if( ds_f3_mask & ds_nv_read_mask ) \
  {                      \
    DATA_LTE_MSG1(MSG_LEGACY_HIGH, fmtstr, arg1); \
  } \
  else\
  {                                        \
    DATA_LTE_MSG1(MSG_LEGACY_LOW, fmtstr, arg1)  ;\
  }\
}

#define DS_LTE_C_MSG2(ds_f3_mask, fmtstr, arg1, arg2)          \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                           \
    DATA_LTE_MSG2(MSG_LEGACY_HIGH, fmtstr, arg1, arg2) ;\
  }\
  else\
  {                                              \
    DATA_LTE_MSG2(MSG_LEGACY_LOW, fmtstr, arg1, arg2)  ;\
  }\
}

#define DS_LTE_C_MSG3(ds_f3_mask, fmtstr, arg1, arg2, arg3)          \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) {                                 \
    DATA_LTE_MSG3(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3); \
  }\
  else\
  {                                                    \
    DATA_LTE_MSG3(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3)  ;\
  }\
}

#define DS_LTE_C_MSG4(ds_f3_mask, fmtstr, arg1, arg2, arg3, arg4)          \
{\
  if ( ds_f3_mask & ds_nv_read_mask ) \
  {                                      \
    DATA_LTE_MSG4(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4);\
  }\
  else\
  {                                                         \
    DATA_LTE_MSG4(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4) ;\
  }\
}

/*---------------------------------------------------------------------------
  Logging utilities: LTE
---------------------------------------------------------------------------*/

#ifdef FEATURE_DATA_LTE

#define DS_LTE_MSG5_LOW(fmtstr, arg1, arg2, arg3, arg4, arg5) \
          DATA_LTE_MSG5(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4, arg5)

#define DS_LTE_MSG5_MED(fmtstr, arg1, arg2, arg3, arg4, arg5) \
          DATA_LTE_MSG5(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3, arg4, arg5)

#define DS_LTE_MSG5_HIGH(fmtstr, arg1, arg2, arg3, arg4, arg5) \
          DATA_LTE_MSG5(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4, arg5)


#define DS_LTE_MSG4_LOW(fmtstr, arg1, arg2, arg3, arg4) \
          DATA_LTE_MSG4(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3, arg4)

#define DS_LTE_MSG4_MED(fmtstr, arg1, arg2, arg3, arg4) \
          DATA_LTE_MSG4(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3, arg4)

#define DS_LTE_MSG4_HIGH(fmtstr, arg1, arg2, arg3, arg4) \
          DATA_LTE_MSG4(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3, arg4)

#define DS_LTE_MSG4_ERROR(fmtstr, arg1, arg2, arg3, arg4) \
          ds_error_log_4_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE, \
            fmtstr, (uint32)arg1, (uint32)arg2, (uint32)arg3, (uint32)arg4)


#define DS_LTE_MSG3_LOW(fmtstr, arg1, arg2, arg3) \
          DATA_LTE_MSG3(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3)

#define DS_LTE_MSG3_MED(fmtstr, arg1, arg2, arg3) \
          DATA_LTE_MSG3(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3)

#define DS_LTE_MSG3_HIGH(fmtstr, arg1, arg2, arg3) \
          DATA_LTE_MSG3(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3)

#define DS_LTE_MSG3_ERROR(fmtstr, arg1, arg2, arg3) \
          ds_error_log_3_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE,\
            fmtstr, (uint32)arg1, (uint32)arg2, (uint32)arg3)



#define DS_LTE_MSG2_LOW(fmtstr, arg1, arg2) \
          DATA_LTE_MSG2(MSG_LEGACY_LOW, fmtstr, arg1, arg2)

#define DS_LTE_MSG2_MED(fmtstr, arg1, arg2) \
          DATA_LTE_MSG2(MSG_LEGACY_MED, fmtstr, arg1, arg2)

#define DS_LTE_MSG2_HIGH(fmtstr, arg1, arg2) \
          DATA_LTE_MSG2(MSG_LEGACY_HIGH, fmtstr, arg1, arg2)

#define DS_LTE_MSG2_ERROR(fmtstr, arg1, arg2) \
          ds_error_log_2_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE,\
            fmtstr, (uint32)arg1, (uint32)arg2)



#define DS_LTE_MSG1_LOW(fmtstr, arg1) \
          DATA_LTE_MSG1(MSG_LEGACY_LOW, fmtstr, arg1)

#define DS_LTE_MSG1_MED(fmtstr, arg1) \
          DATA_LTE_MSG1(MSG_LEGACY_MED, fmtstr, arg1)

#define DS_LTE_MSG1_HIGH(fmtstr, arg1) \
          DATA_LTE_MSG1(MSG_LEGACY_HIGH, fmtstr, arg1)

#define DS_LTE_MSG1_ERROR(fmtstr, arg1) \
         ds_error_log_1_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE,\
           fmtstr, (uint32)arg1)


#define DS_LTE_MSG0_LOW(fmtstr) \
          DATA_LTE_MSG0(MSG_LEGACY_LOW, fmtstr)

#define DS_LTE_MSG0_MED(fmtstr) \
          DATA_LTE_MSG0(MSG_LEGACY_MED, fmtstr)

#define DS_LTE_MSG0_HIGH(fmtstr) \
          DATA_LTE_MSG0(MSG_LEGACY_HIGH, fmtstr)

#define DS_LTE_MSG0_ERROR(fmtstr) \
          ds_error_log_0_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE,\
            fmtstr)


#define DS_LTE_MSG4_HIGH_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4) \
          DATA_LTE_MSG4_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2, arg3, arg4)

#define DS_LTE_MSG3_HIGH_EX(subs_id, fmtstr, arg1, arg2, arg3) \
          DATA_LTE_MSG3_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2, arg3)

#define DS_LTE_MSG2_HIGH_EX(subs_id, fmtstr, arg1, arg2) \
          DATA_LTE_MSG2_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1, arg2)

#define DS_LTE_MSG1_HIGH_EX(subs_id, fmtstr, arg1) \
          DATA_LTE_MSG1_EX(MSG_LEGACY_HIGH, subs_id, fmtstr, arg1)

#define DS_LTE_MSG0_HIGH_EX(subs_id, fmtstr) \
          DATA_LTE_MSG0_EX(MSG_LEGACY_HIGH, subs_id, fmtstr)


#define DS_LTE_MSG4_MED_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4) \
          DATA_LTE_MSG4_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3, arg4)

#define DS_LTE_MSG3_MED_EX(subs_id, fmtstr, arg1, arg2, arg3) \
          DATA_LTE_MSG3_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3)

#define DS_LTE_MSG2_MED_EX(subs_id, fmtstr, arg1, arg2) \
          DATA_LTE_MSG2_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2)

#define DS_LTE_MSG1_MED_EX(subs_id, fmtstr, arg1) \
          DATA_LTE_MSG1_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1)

#define DS_LTE_MSG0_MED_EX(subs_id, fmtstr) \
          DATA_LTE_MSG0_EX(MSG_LEGACY_MED, subs_id, fmtstr)

#define DS_LTE_MSG4_ERROR_EX(subs_id, fmtstr, arg1, arg2, arg3, arg4) \
          ds_error_log_4_ex_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE,\
            subs_id, fmtstr, (uint32)arg1, (uint32)arg2, (uint32)arg3, \
            (uint32)arg4)
            
#define DS_LTE_MSG3_ERROR_EX(subs_id, fmtstr, arg1, arg2, arg3) \
          ds_error_log_3_ex_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE,\
            subs_id, fmtstr, (uint32)arg1, (uint32)arg2, (uint32)arg3)
            
#define DS_LTE_MSG2_ERROR_EX(subs_id, fmtstr, arg1, arg2) \
          ds_error_log_2_ex_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE,\
            subs_id, fmtstr, (uint32)arg1, (uint32)arg2)
            
#define DS_LTE_MSG1_ERROR_EX(subs_id, fmtstr, arg1) \
         ds_error_log_1_ex_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE,\
           subs_id, fmtstr, (uint32)arg1)
           
#define DS_LTE_MSG0_ERROR_EX(subs_id, fmtstr) \
          ds_error_log_0_ex_wrapper(__FILENAME__ ,__LINE__, MSG_SSID_DS_LTE,\
            subs_id, fmtstr)

#endif 

/*---------------------------------------------------------------------------
  Logging utilities: WCDMA
---------------------------------------------------------------------------*/

#ifdef FEATURE_DATA_WCDMA_PS

#define DS_WCDMA_MSG3_LOW(fmtstr, arg1, arg2, arg3) \
          DATA_WCDMA_MSG3(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3)

#define DS_WCDMA_MSG3_MED(fmtstr, arg1, arg2, arg3) \
          DATA_WCDMA_MSG3(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3)

#define DS_WCDMA_MSG3_HIGH(fmtstr, arg1, arg2, arg3) \
          DATA_WCDMA_MSG3(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3)

#define DS_WCDMA_MSG3_ERROR(fmtstr, arg1, arg2, arg3) \
          DATA_WCDMA_MSG3(MSG_LEGACY_ERROR, fmtstr, arg1, arg2, arg3)

#define DS_WCDMA_MSG2_LOW(fmtstr, arg1, arg2) \
          DATA_WCDMA_MSG2(MSG_LEGACY_LOW, fmtstr, arg1, arg2)

#define DS_WCDMA_MSG2_MED(fmtstr, arg1, arg2) \
          DATA_WCDMA_MSG2(MSG_LEGACY_MED, fmtstr, arg1, arg2)

#define DS_WCDMA_MSG2_HIGH(fmtstr, arg1, arg2) \
          DATA_WCDMA_MSG2(MSG_LEGACY_HIGH, fmtstr, arg1, arg2)

#define DS_WCDMA_MSG2_ERROR(fmtstr, arg1, arg2) \
          DATA_WCDMA_MSG2(MSG_LEGACY_ERROR, fmtstr, arg1, arg2)

#define DS_WCDMA_MSG1_LOW(fmtstr, arg1) \
          DATA_WCDMA_MSG1(MSG_LEGACY_LOW, fmtstr, arg1)

#define DS_WCDMA_MSG1_MED(fmtstr, arg1) \
          DATA_WCDMA_MSG1(MSG_LEGACY_MED, fmtstr, arg1)

#define DS_WCDMA_MSG1_HIGH(fmtstr, arg1) \
          DATA_WCDMA_MSG1(MSG_LEGACY_HIGH, fmtstr, arg1)

#define DS_WCDMA_MSG1_ERROR(fmtstr, arg1) \
          DATA_WCDMA_MSG1(MSG_LEGACY_ERROR, fmtstr, arg1)

#define DS_WCDMA_MSG0_LOW(fmtstr) \
          DATA_WCDMA_MSG0(MSG_LEGACY_LOW, fmtstr)

#define DS_WCDMA_MSG0_MED(fmtstr) \
          DATA_WCDMA_MSG0(MSG_LEGACY_MED, fmtstr)

#define DS_WCDMA_MSG0_HIGH(fmtstr) \
          DATA_WCDMA_MSG0(MSG_LEGACY_HIGH, fmtstr)

#define DS_WCDMA_MSG0_ERROR(fmtstr) \
          DATA_WCDMA_MSG0(MSG_LEGACY_ERROR, fmtstr)


#define DS_WCDMA_MSG3_MED_EX(subs_id, fmtstr, arg1, arg2, arg3) \
          DATA_WCDMA_MSG3_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2, arg3)

#define DS_WCDMA_MSG2_MED_EX(subs_id, fmtstr, arg1, arg2) \
          DATA_WCDMA_MSG2_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1, arg2)

#define DS_WCDMA_MSG1_MED_EX(subs_id, fmtstr, arg1) \
          DATA_WCDMA_MSG1_EX(MSG_LEGACY_MED, subs_id, fmtstr, arg1)

#define DS_WCDMA_MSG0_MED_EX(subs_id, fmtstr) \
          DATA_WCDMA_MSG0_EX(MSG_LEGACY_MED, subs_id, fmtstr)

#endif 

/*---------------------------------------------------------------------------
  Logging utilities: GPRS
---------------------------------------------------------------------------*/

#ifdef FEATURE_GSM_GPRS

#define DS_GPRS_MSG3_LOW(fmtstr, arg1, arg2, arg3) \
          DATA_GPRS_MSG3(MSG_LEGACY_LOW, fmtstr, arg1, arg2, arg3)

#define DS_GPRS_MSG3_MED(fmtstr, arg1, arg2, arg3) \
          DATA_GPRS_MSG3(MSG_LEGACY_MED, fmtstr, arg1, arg2, arg3)

#define DS_GPRS_MSG3_HIGH(fmtstr, arg1, arg2, arg3) \
          DATA_GPRS_MSG3(MSG_LEGACY_HIGH, fmtstr, arg1, arg2, arg3)

#define DS_GPRS_MSG3_ERROR(fmtstr, arg1, arg2, arg3) \
          DATA_GPRS_MSG3(MSG_LEGACY_ERROR, fmtstr, arg1, arg2, arg3)

#define DS_GPRS_MSG2_LOW(fmtstr, arg1, arg2) \
          DATA_GPRS_MSG2(MSG_LEGACY_LOW, fmtstr, arg1, arg2)

#define DS_GPRS_MSG2_MED(fmtstr, arg1, arg2) \
          DATA_GPRS_MSG2(MSG_LEGACY_MED, fmtstr, arg1, arg2)

#define DS_GPRS_MSG2_HIGH(fmtstr, arg1, arg2) \
          DATA_GPRS_MSG2(MSG_LEGACY_HIGH, fmtstr, arg1, arg2)

#define DS_GPRS_MSG2_ERROR(fmtstr, arg1, arg2) \
          DATA_GPRS_MSG2(MSG_LEGACY_ERROR, fmtstr, arg1, arg2)

#define DS_GPRS_MSG1_LOW(fmtstr, arg1) \
          DATA_GPRS_MSG1(MSG_LEGACY_LOW, fmtstr, arg1)

#define DS_GPRS_MSG1_MED(fmtstr, arg1) \
          DATA_GPRS_MSG1(MSG_LEGACY_MED, fmtstr, arg1)

#define DS_GPRS_MSG1_HIGH(fmtstr, arg1) \
          DATA_GPRS_MSG1(MSG_LEGACY_HIGH, fmtstr, arg1)

#define DS_GPRS_MSG1_ERROR(fmtstr, arg1) \
          DATA_GPRS_MSG1(MSG_LEGACY_ERROR, fmtstr, arg1)

#define DS_GPRS_MSG0_LOW(fmtstr) \
          DATA_GPRS_MSG0(MSG_LEGACY_LOW, fmtstr)

#define DS_GPRS_MSG0_MED(fmtstr) \
          DATA_GPRS_MSG0(MSG_LEGACY_MED, fmtstr)

#define DS_GPRS_MSG0_HIGH(fmtstr) \
          DATA_GPRS_MSG0(MSG_LEGACY_HIGH, fmtstr)

#define DS_GPRS_MSG0_ERROR(fmtstr) \
          DATA_GPRS_MSG0(MSG_LEGACY_ERROR, fmtstr)

#endif

/*---------------------------------------------------------------------------
  Logging utilities: eMBMS
---------------------------------------------------------------------------*/

#ifdef FEATURE_DATA_EMBMS

#define DS_EMBMS_MSG3_LOW(fmtstr, arg1, arg2, arg3) \
          DS_3GPP_MSG3_LOW(fmtstr, arg1, arg2, arg3)

#define DS_EMBMS_MSG3_MED(fmtstr, arg1, arg2, arg3) \
          DS_3GPP_MSG3_MED(fmtstr, arg1, arg2, arg3)

#define DS_EMBMS_MSG3_HIGH(fmtstr, arg1, arg2, arg3) \
          DS_3GPP_MSG3_HIGH(fmtstr, arg1, arg2, arg3)

#define DS_EMBMS_MSG3_ERROR(fmtstr, arg1, arg2, arg3) \
          DS_3GPP_MSG3_ERROR(fmtstr, arg1, arg2, arg3)



#define DS_EMBMS_MSG2_LOW(fmtstr, arg1, arg2) \
          DS_3GPP_MSG2_LOW(fmtstr, arg1, arg2)

#define DS_EMBMS_MSG2_MED(fmtstr, arg1, arg2) \
          DS_3GPP_MSG2_MED(fmtstr, arg1, arg2)

#define DS_EMBMS_MSG2_HIGH(fmtstr, arg1, arg2) \
          DS_3GPP_MSG2_HIGH(fmtstr, arg1, arg2)

#define DS_EMBMS_MSG2_ERROR(fmtstr, arg1, arg2) \
          DS_3GPP_MSG2_ERROR(fmtstr, arg1, arg2)



#define DS_EMBMS_MSG1_LOW(fmtstr, arg1) \
          DS_3GPP_MSG1_LOW(fmtstr, arg1)

#define DS_EMBMS_MSG1_MED(fmtstr, arg1) \
          DS_3GPP_MSG1_MED(fmtstr, arg1)

#define DS_EMBMS_MSG1_HIGH(fmtstr, arg1) \
          DS_3GPP_MSG1_HIGH(fmtstr, arg1)

#define DS_EMBMS_MSG1_ERROR(fmtstr, arg1) \
          DS_3GPP_MSG1_ERROR(fmtstr, arg1)


#define DS_EMBMS_MSG0_LOW(fmtstr) \
          DS_3GPP_MSG0_LOW(fmtstr)

#define DS_EMBMS_MSG0_MED(fmtstr) \
          DS_3GPP_MSG0_MED(fmtstr)

#define DS_EMBMS_MSG0_HIGH(fmtstr) \
          DS_3GPP_MSG0_HIGH(fmtstr)

#define DS_EMBMS_MSG0_ERROR(fmtstr) \
          DS_3GPP_MSG0_ERROR(fmtstr)

#endif /* FEATURE_DATA_EMBMS */

typedef enum
{
  DS_3GPPI_LOCAL_ERR_VAL_NONE = -1,
  DS_3GPPI_LOCAL_ERR_VAL_0 = 0,
  DS_3GPPI_LOCAL_ERR_VAL_1,
  DS_3GPPI_LOCAL_ERR_VAL_2,
  DS_3GPPI_LOCAL_ERR_VAL_3,
  DS_3GPPI_LOCAL_ERR_VAL_4,
  DS_3GPPI_LOCAL_ERR_VAL_5,
  DS_3GPPI_LOCAL_ERR_VAL_6,
  DS_3GPPI_LOCAL_ERR_VAL_7,
  DS_3GPPI_LOCAL_ERR_VAL_8,
  DS_3GPPI_LOCAL_ERR_VAL_9,
  DS_3GPPI_LOCAL_ERR_VAL_10,
  DS_3GPPI_LOCAL_ERR_VAL_11,
  DS_3GPPI_LOCAL_ERR_VAL_12,
  DS_3GPPI_LOCAL_ERR_VAL_13,
  DS_3GPPI_LOCAL_ERR_VAL_14,
  DS_3GPPI_LOCAL_ERR_VAL_15,
  DS_3GPPI_LOCAL_ERR_VAL_16,
  DS_3GPPI_LOCAL_ERR_VAL_17,
  DS_3GPPI_LOCAL_ERR_VAL_18,
  DS_3GPPI_LOCAL_ERR_VAL_19,
  DS_3GPPI_LOCAL_ERR_VAL_20,
  DS_3GPPI_LOCAL_ERR_VAL_21,
  DS_3GPPI_LOCAL_ERR_VAL_22,
  DS_3GPPI_LOCAL_ERR_VAL_23,
  DS_3GPPI_LOCAL_ERR_VAL_24,
  DS_3GPPI_LOCAL_ERR_VAL_25,
  DS_3GPPI_LOCAL_ERR_VAL_26,
  DS_3GPPI_LOCAL_ERR_VAL_27,
  DS_3GPPI_LOCAL_ERR_VAL_28,
  DS_3GPPI_LOCAL_ERR_VAL_29,
  DS_3GPPI_LOCAL_ERR_VAL_30,
  DS_3GPPI_LOCAL_ERR_VAL_31,
  DS_3GPPI_LOCAL_ERR_VAL_32,
  DS_3GPPI_LOCAL_ERR_VAL_33,
  DS_3GPPI_LOCAL_ERR_VAL_34,
  DS_3GPPI_LOCAL_ERR_VAL_35,
  DS_3GPPI_LOCAL_ERR_VAL_36,
  DS_3GPPI_LOCAL_ERR_VAL_37,
  DS_3GPPI_LOCAL_ERR_VAL_38,
  DS_3GPPI_LOCAL_ERR_VAL_39,
  DS_3GPPI_LOCAL_ERR_VAL_40,
  DS_3GPPI_LOCAL_ERR_VAL_41,
  DS_3GPPI_LOCAL_ERR_VAL_42,
  DS_3GPPI_LOCAL_ERR_VAL_43,
  DS_3GPPI_LOCAL_ERR_VAL_44,
  DS_3GPPI_LOCAL_ERR_VAL_45,
  DS_3GPPI_LOCAL_ERR_VAL_46,
  DS_3GPPI_LOCAL_ERR_VAL_47,
  DS_3GPPI_LOCAL_ERR_VAL_48,
  DS_3GPPI_LOCAL_ERR_VAL_49,
  DS_3GPPI_LOCAL_ERR_VAL_50
} ds_3gppi_local_err_val_e_type;

typedef enum {
    DS_3GPPI_SYS_MODE_MIN = -1,
    DS_3GPPI_SYS_MODE_LTE = 0,
    DS_3GPPI_SYS_MODE_WCDMA,
    DS_3GPPI_SYS_MODE_GSM,
    DS_3GPPI_SYS_MODE_TDS,
    DS_3GPPI_SYS_MODE_5G,
    DS_3GPPI_SYS_MODE_ILLEGAL
} ds_3gppi_sys_mode_e_type;

extern int ds_3gppi_mode_index[SYS_SYS_MODE_MAX];

/* This is the variable where the NV Value read will be stored
   for f3 diag supression purposes.                     */
extern uint32 ds_nv_read_mask;

/* These are the Functional masks that are used to identify a message as belonging to
  group "i" of a particular Subsystem. This allows the user to selectively
  turn groups of messages within a Subsystem to ON or OFF. */

/* Macro for supressing f3 msgs for PSM handler functions*/
#define DS_PSM_F3_MASK       (0x00000001)

/* Macro for supressing DS throughput related f3 msgs*/
#define DS_THRPT_F3_MASK     (0x00000002)

/* Generic Macro for supressing DS707 related f3 msgs*/
#define DS707_F3_MASK        (0x00000004)

/* Generic Macro for supressing DS 3GPP related f3 msgs*/
#define DS_3GPP_F3_MASK      (0x00000008)

/* Generic Macro for supressing DS 3GMGR related f3 msgs*/
#define DS3G_F3_MASK         (0x00000010)

/* Generic Macro for supressing DS PDN throttling related f3 msgs*/
#define DS_THROT_F3_MASK         (0x00000020)

#define DS_3GPPI_SYS_MODE_MAX ((int)DS_3GPPI_SYS_MODE_ILLEGAL)

#define DS_3GPPI_MODE_INDEX(mode) ds_3gppi_mode_index[mode]

#define DS_3GPPI_VERIFY_MODE_INDEX(index) \
          ((index >= 0) && (index < DS_3GPPI_SYS_MODE_MAX) ? TRUE : FALSE )

#define DS_3GPPI_VF_FPN(func) func##_f

#define DS_3GPPI_VFR_CALL_PER_MODE(ftbl, rval, func, mode, ...)      	 \
        {                                                                \
          int                  index;                                    \
                                                                         \
        	if (mode < SYS_SYS_MODE_MAX && mode > SYS_SYS_MODE_NONE) 	  \
        	{															  \
            index = DS_3GPPI_MODE_INDEX(mode);                           \
                                                                         \
            ASSERT(DS_3GPPI_VERIFY_MODE_INDEX(index));                   \
        	  if (ftbl[index].DS_3GPPI_VF_FPN(func))					  \
        	  {													          \
        		  rval = ftbl[index].DS_3GPPI_VF_FPN(func)( __VA_ARGS__ );\
        	  } 														  \
        	  else 													      \
            {                                                             \
              DS_3GPP_MSG2_LOW(                                          \
                "DS_3GPPI_VFR_CALL: No reg fnc in tbl %x for index %d",  \
                ftbl, index);                                            \
            }                                                            \
        	} 														      \
        	else 													      \
        	{													          \
            DS_3GPP_MSG1_HIGH("Invalid mode %x",mode);                   \
          }                                                              \
        }

#define DS_3GPPI_VFR_CALL(ftbl, rval, func, subs_id, ...)                \
        {                                                                \
          sys_sys_mode_e_type  mode;                                     \
          mode = ds3gpp_get_current_network_mode(subs_id);               \
          DS_3GPPI_VFR_CALL_PER_MODE(ftbl, rval, func, mode, __VA_ARGS__)\
        }

#define DS_3GPPI_VF_CALL(ftbl, func, subs_id, ...)                    \
        {                                                             \
          sys_sys_mode_e_type mode;                                   \
          int index;                                                  \
                                                                      \
          mode = ds3gpp_get_current_network_mode(subs_id);            \
          if (mode < SYS_SYS_MODE_MAX && mode > SYS_SYS_MODE_NONE) {  \
            index = DS_3GPPI_MODE_INDEX(mode);                        \
                                                                      \
            ASSERT(DS_3GPPI_VERIFY_MODE_INDEX(index));                \
            if (ftbl[index].DS_3GPPI_VF_FPN(func)) {                  \
              ftbl[index].DS_3GPPI_VF_FPN(func)( __VA_ARGS__ );       \
            } else {                                                  \
              DS_3GPP_MSG2_LOW(                                       \
                "DS_3GPPI_VF_CALL: No reg fnc in tbl %x for index %d",\
                ftbl, index);                                         \
            }                                                         \
          } else {                                                    \
            DS_3GPP_MSG1_HIGH(                                        \
                  "DS_3GPPI_VF_CALL:Invalid mode %x",mode);           \
          }                                                           \
        }

#define DS_3GPPI_VF_CALL_NO_ARG(ftbl, func, subs_id)                  \
        {                                                             \
          sys_sys_mode_e_type mode;                                   \
          int index;                                                  \
                                                                      \
          mode = ds3gpp_get_current_network_mode(subs_id);            \
          if(mode < SYS_SYS_MODE_MAX && mode > SYS_SYS_MODE_NONE) {   \
            index = DS_3GPPI_MODE_INDEX(mode);                        \
                                                                      \
            ASSERT(DS_3GPPI_VERIFY_MODE_INDEX(index));                \
            if (ftbl[index].DS_3GPPI_VF_FPN(func)) {                  \
              ftbl[index].DS_3GPPI_VF_FPN(func)( );                   \
            } else {                                                  \
              DS_3GPP_MSG2_LOW(                                       \
                "DS_3GPPI_VF_CALL: No reg fnc in tbl %x for index %d",\
                ftbl, index);                                         \
            }                                                         \
          } else {                                                    \
            DS_3GPP_MSG1_HIGH("Invalid mode %x",mode);                \
          }                                                           \
        }          
        
#define DS_3GPPI_VFR_CALL_NO_ARG(ftbl, rval, func, subs_id)             \
        {                                                               \
          sys_sys_mode_e_type mode;                                     \
          int index;                                                    \
                                                                        \
          mode = ds3gpp_get_current_network_mode(subs_id);              \
          if (mode < SYS_SYS_MODE_MAX && mode > SYS_SYS_MODE_NONE) {    \
            index = DS_3GPPI_MODE_INDEX(mode);                          \
                                                                        \
            ASSERT(DS_3GPPI_VERIFY_MODE_INDEX(index));                  \
            if (ftbl[index].DS_3GPPI_VF_FPN(func)) {                    \
              rval = ftbl[index].DS_3GPPI_VF_FPN(func)( );              \
            } else {                                                    \
              DS_3GPP_MSG2_LOW(                                         \
                "DS_3GPPI_VFR_CALL: No reg fnc in tbl %x for index %d", \
                ftbl, index);                                           \
            }                                                           \
          } else {                                                      \
            DS_3GPP_MSG1_HIGH("Invalid mode %x",mode);                  \
          }                                                             \
        }
            
#define DS_3GPPI_VF_REG(ftbl, mode, func, fp)                  \
        {                                                      \
          int index;                                           \
          if(mode < SYS_SYS_MODE_MAX) {                        \
            index = DS_3GPPI_MODE_INDEX(mode);                 \
                                                               \
            ASSERT(DS_3GPPI_VERIFY_MODE_INDEX(index));         \
            ftbl[index].DS_3GPPI_VF_FPN(func) = fp;            \
          } else {                                             \
            DS_3GPP_MSG1_HIGH("Invalid mode %x",mode);         \
          }                                                    \
        }                                                       

#define DS_3GPPI_VOBJ_REG(objtbl, mode, objp)                  \
        {                                                      \
          int index;                                           \
          if(mode < SYS_SYS_MODE_MAX) {                        \
            index = DS_3GPPI_MODE_INDEX(mode);                 \
            ASSERT(DS_3GPPI_VERIFY_MODE_INDEX(index));         \
                                                               \
            objtbl[index] = objp;                              \
          } else {                                             \
            DS_3GPP_MSG1_HIGH("Invalid mode %x",mode);         \
          }                                                    \
        }                                                      

#define DS_3GPPI_VOBJ(objtbl, mode)                 \
          objtbl[DS_3GPPI_MODE_INDEX(mode)]         \

/*---------------------------------------------------------------------------
  Critical section utilities
---------------------------------------------------------------------------*/
#define DS_3GPP_INIT_CRIT_SECTION( rex_crit_section )   \
    { rex_init_crit_sect( rex_crit_section ); }

#define DS_3GPP_ENTER_CRIT_SECTION( rex_crit_section )  \
    { rex_enter_crit_sect( rex_crit_section ); }

#define DS_3GPP_LEAVE_CRIT_SECTION( rex_crit_section )  \
    { rex_leave_crit_sect( rex_crit_section ); }

#define DS_3GPP_DEL_CRIT_SECTION( rex_crit_section )  \
    { rex_del_crit_sect( rex_crit_section ); }

/*---------------------------------------------------------------------------
  Enumeration of EFS configuration items
---------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_LTE
#define DS_3GPP_CFG_PROFILES_MAX  (5)

#endif 

/* dual ip fallback emm or sm cause codes */
#define DS_3GPP_DUAL_IP_FALLBACK_CAUSE_CODE_MAX (8)

#define DS_3GPPI_CFG_TABLE_LTE                                                    \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_ATTACH_PROFILE,                         \
                              "Attach Profile",                                   \
                              "modem/data/3gpp/lteps/attach_profile",             \
                              sizeof(uint16)                                      \
                           )  /* NV-item number: 65883 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_PROFILE_LIST,                           \
                              "Profile List",                                     \
                              "modem/data/3gpp/lteps/profile_list",               \
                              (sizeof(uint16)* DS_3GPP_CFG_PROFILES_MAX)          \
                           )  /* NV-item number: 65884 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_ALWAYS_CONNECT_DEF_PDN,                 \
                              "Auto Connect Def PDN",                             \
                              "modem/data/3gpp/lteps/auto_connect_def_pdn",       \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 66008 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_ATTACH_PDN_DUAL_IP_FALLBACK_IP_TYPE,    \
                              "Specify Attach PDN dual IP Fall back IP type",     \
                              "modem/data/3gpp/lteps/attach_fallback_ip_type",    \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 71595 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_DUAL_IP_FALLBACK_ENABLED,                   \
                              "Enable/Disable Dual IP fallback",                  \
                              "modem/data/3gpp/ps/dual_ip_fallback",              \
                              sizeof(ds_eps_attach_fb_info_type)                  \
                           ) /* NV-item number: 72604 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_ENABLE_SEND_TRM_PRIORITY,                   \
                              "Enable/Disables sending of TRM priority to RRC",   \
                              "modem/data/3gpp/lteps/enable_send_trm_priority",   \
                              sizeof(boolean)                                     \
                           )  /* NV-item number: 73562 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_RAB_RETRY_TIMER,                            \
                              "Enable/Disables RAB retry timer value",            \
                              "data/3gpp/rab_retry_timer",                        \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 73952 */


#define DS_3GPPI_CFG_TABLE_TEST_LOOPBACK                                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_TEST_LOOPBACK_ENABLE,                       \
                              "3GPP Test Loopback Enable",                        \
                              "modem/data/3gpp/ps/3gpp_test_loopback_enable",     \
                              sizeof(uint8)                                       \
                           ) /* NV-item number: 66052 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_LOOPBACK_FLOW_CTRL_DISABLE,                 \
                              "3GPP Disable Loopback flow control",               \
                              "modem/data/3gpp/ps/3gpp_loopback_flow_ctrl_disable", \
                              sizeof(uint8)                                       \
                           ) /* NV-item number: 70365 */                          \


#define DS_3GPPI_CFG_TABLE                                                        \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_REL_VERSION,                                \
                              "3GPP Release Version",                             \
                              "modem/data/3gpp/ps/3gpp_rel_version",              \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 66030 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_DATA_PKT_ORIG_STR,                          \
                              "3GPP Pkt Data Def String",                         \
                              "modem/data/3gpp/ps/3gpp_data_pkt_orig_str",        \
                              (sizeof(uint8)* NV_PKT_DIAL_STRING_SIZE)            \
                           ) /* NV-item number: 66046 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_DUN_ACCEL_CFG,                              \
                              "DUN Data Path Acceleration config",                \
                              "modem/data/3gpp/ps/3gpp_data_dun_acc_cfg",         \
                               sizeof(uint8)                                      \
                           ) /* NV-item number: 67313 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_AUTH_TYPE_FALLBACK,                     \
                              "Retry with PAP if CHAP authentication fails",      \
                              "modem/data/3gpp/ps/3gpp_auth_type_fallback",       \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 67215 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_AUTO_ANSWER_ENABLE,                         \
                              "3GPP MT Auto Answer Enable",                       \
                              "modem/data/3gpp/ps/3gpp_auto_answer_enable",       \
                               sizeof(uint8)                                      \
                           )  /* NV-item number: 67269 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_DISABLE_A2_BRIDGING,                    \
                              "Disable A2 Bridging",                              \
                              "modem/data/3gpp/ps/3gpp_data_disable_a2_bridge",   \
                              sizeof(uint8)                                       \
                           ) /* NV-item number: 67343 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_IGNORE_INACTIVITY_TIMER_FOR_LAST_PDN,   \
                              "Ignore Inactivity timer for last PDN",             \
                              "modem/data/3gpp/ignore_last_pdn_ia_timer",         \
                              sizeof(uint8)                                       \
                           ) /* NV-item number: 67334 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_ALLOW_INFINITE_THROT,                   \
                              "Allow Infinite Throttling",                        \
                              "modem/data/3gpp/ps/allow_infinite_throt",          \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 67344 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_SRV_REQ_THROTTLE_BEHAVIOR,              \
                              "Specify behavior for service request throttling",  \
                              "modem/data/3gpp/ps/ser_req_throttle_behavior",     \
                              sizeof(uint32)                                      \
                           ) /* NV-item number: 69710 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_TCP_THRESHOLD_ACK_CNT,                      \
                              "Threshold ack_cnt which triggers BP removal",      \
                              "modem/data/3gpp/ps/tcp_threshold_ack_cnt",         \
                              sizeof(uint16)                                      \
                           ) /* NV-item number: 70190 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_UM_WM_THRESHOLD,                        \
                              "3GPP UM WM Threshold",                             \
                              "data/3gpp/ds_3gpp_um_wm_threshold",                \
                              sizeof(ds_bearer_cntxt_um_wm_threshold_struct_type) \
                           )  /* NV-item number: 67271 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_MULTI_PDN_SAME_APN,                     \
                              "3GPP Multi PDN Same APN",                          \
                              "data/3gpp/ds_3gpp_multi_pdn_same_apn",             \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 67248 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_MTU,                                    \
                              "3GPP MTU",                                         \
                              "data/3gpp/ds_3gpp_mtu",                            \
                              sizeof(uint16)                                      \
                           )  /* NV-item number: 65814 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_QOS_SUPPORT,                            \
                              "3GPP QOS Support",                                 \
                              "modem/data/3gpp/umtsps/qos_support",               \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 66044*/                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_SEND_PS_DATA_AVAIL_ON_POWER_SAVE,           \
                              "Send data available indication in power save mode",\
                              "modem/data/3gpp/ds_3gpp_send_ps_data_avail_on_power_save",\
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 70264 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_SEND_ENFORCE_PS_DATA_AVAIL,                 \
                              "Send data available indication in power save mode for IMS PDN",\
                              "modem/data/3gpp/ps/enforce_ps_data_avail",         \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 73990 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_NUM_PERSISTENT_PROFILES,                    \
                              "3GPP Number of Persistent profiles",               \
                              "modem/data/3gpp/num_persistent_profiles",          \
                               sizeof(uint8)                                      \
                           )  /* NV-item number: 70244 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_ALLOW_DEF_LTE_PDN_TEARDOWN,                 \
                              "Allow def PDN Teardown",                           \
                              "modem/data/3gpp/lteps/allow_def_lte_pdn_teardown", \
                              sizeof(uint8) /* NV-item number: 70260 */           \
                           )                                                      \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_DUAL_IP_FALLBACK_CAUSE_CODES,               \
                              "Dual IP Fallback Cause Codes",                     \
                              "modem/data/3gpp/dual_ip_fallback_cause_codes",     \
                              (sizeof(uint16) * DS_3GPP_DUAL_IP_FALLBACK_CAUSE_CODE_MAX)\
                           )  /* NV-item number: 70313 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_REMOVE_UNUSED_PDN,                          \
                              "Remove unused pdn",                                \
                              "modem/data/3gpp/ps/remove_unused_pdn",             \
                              sizeof(uint8)                                       \
                           ) /* NV-item number: 70334 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_TDS_WM_THRESHOLD,                       \
                              "TDS WM config",                                    \
                              "modem/data/3gpp/tds_um_wm_threshold",              \
                              sizeof(ds_bearer_cntxt_tds_um_wm_threshold_struct_type)\
                           ) /* NV-item number: 70346 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_ENABLE_UMTS_NETWORK_INITIATED_QOS,          \
                              "Enable/Disable UMTS Network initiated QOS",        \
                              "modem/data/3gpp/umts_nw_initiated_qos_support",    \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 70331 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CALL_ORIG_ALLOWED_BEFORE_PS_ATTACH,         \
                              "Call origination is allowed before ps attach",     \
                              "modem/data/3gpp/call_orig_allowed_before_ps_attach", \
                              sizeof(boolean)                                     \
                           )  /* NV-item number: 70332 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_REL_10_THROTTLING,                          \
                              "3GPP Release 10 Throttling",                       \
                              "modem/data/3gpp/ps/rel_10_throttling",             \
                              sizeof(uint8)                                       \
                           )  /* NV-item number: 70328 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_PDN_CONN_LIMIT_HANDLING,                    \
                              "Enable/Disable PDN Conn Limit Handling",           \
                              "modem/data/3gpp/lteps/pdn_conn_limit_handling",    \
                              (sizeof(boolean) + sizeof(uint16))                  \
                           )  /* NV-item number: 71517 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_REDIAL_FOR_NO_CAUSE_CODE,                   \
                              "3GPP Redial For No Cause",                         \
                              "modem/data/3gpp/ps/ds_3gpp_redial_for_no_cause",   \
                              sizeof(uint8)                                       \
                           ) /* NV-item number: 72564 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_GLOBAL_THROTTLING,                      \
                              "Enable/Disable PDP, PDN and service request throttling",\
                              "modem/data/3gpp/global_throttling",                \
                              sizeof(ds_3gpp_global_throttle_s)                   \
                           )  /* NV-item number: 71520 */                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                            DS_3GPP_CFG_IOT_THROTTLING,                           \
                            "Enable/Disable IOT, Update timer for IOT throttling",\
                            "data/3gpp/iot_device",                               \
                            sizeof(ds_3gpp_iot_throttle_s)                        \
                         )  /* NV-item number: 73948 */                           \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_MULTI_PDN_DUN_ENABLED,                  \
                              "Enable/Disable Multi-PDN DUN",                     \
                              "modem/data/3gpp/umts/enable_multi_pdn_dun",        \
                              sizeof(boolean)                                     \
                           ) /* NV-item number: 71541 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_REGULAR_PDN_THROTTLING,                 \
                              "Enable/Disable Regular PDN Throttling",            \
                              "modem/data/3gpp/ps/reg_pdn_throttling",            \
                              sizeof(boolean)                                     \
                           ) /* NV-item number: 72560 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_REL10_THROTTLING_PER_PLMN,              \
                              "Enable/Disable Release 10 Throttling Per plmn",    \
                              "modem/data/3gpp/ps/rel_10_throttling_per_plmn",    \
                              sizeof(boolean)                                     \
                           ) /* NV-item number: 72551 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_SWITCH_APN_ON_REJECT,                   \
                              "Switch APN on reject",                             \
                              "modem/data/3gpp/lteps/switch_apn_on_reject",       \
                              sizeof(boolean)                                     \
                           ) /* NV-item number: 72602 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_L_TO_W_QOS_CONTINUITY,                      \
                              "Enable/Disable QoS continuity between L to W and W to L", \
                              "modem/data/3gpp/L_to_W_qos_continuity",            \
                              sizeof(ds_3gpp_l_to_w_qos_cont_s)                   \
                           ) /* NV-item number: 72576 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_BCM_INFO_VALIDATION,                        \
                              "Enable/Disable Bearer control mode validation during bearer modify", \
                              "modem/data/3gpp/bcm_validation_on_bearer_mod",     \
                              sizeof(ds_3gpp_bcm_info_validation_s)               \
                           ) /* NV-item number: 72580 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_HOME_PLMN_LIST,                         \
                              "Set the home PLMN List",                           \
                              "modem/data/3gpp/ps/home_plmn_list",                \
                              sizeof(ds_3gpp_roaming_efs_info_s)                  \
                           ) /* NV-item number: 72561 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_APN_PARAM_CHG,                          \
                              "Enable/Disable APN PARAM CHG",                     \
                              "modem/data/3gpp/ps/enable_apn_param_chg",          \
                              sizeof(boolean)                                     \
                           ) /* NV-item number: 72603 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_RPM_PARAMETER,                          \
                              "RPM parameters",                                   \
                              "data/3gpp/rpm_params",                             \
                              sizeof(ds_3gpp_rpm_params_type)                     \
                           ) /* NV-item number:  67204*/                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_RPM_SUPPORTED_SIM,                      \
                              "SIM RPM parametrs",                                \
                              "data/3gpp/rpm_suppported_sim",                     \
                              (sizeof(ds_3gpp_rpm_sim_info_s_type) * DS_3GPP_RPM_MAX_SIM_SUPPORTED)  \
                           ) /* NV-item number: 67205 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_DISABLE_APN_MATCHING,                       \
                              "Enable/Disable APN matching",                      \
                              "modem/data/3gpp/lteps/disable_apn_matching",       \
                              sizeof(boolean)                                     \
                           ) /*NV-item number: 73529 */                           \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_DL_TPUT_T_ACCUMULATE,                       \
                              "LTE DL tput t_accumulate/Disable APN matching",    \
                              "modem/data/3gpp/lteps/t_accumulate",                \
                              sizeof(uint32)                                     \
                            ) /*NV-item number: 73587 */                          \
   DS_3GPPI_CFG_ITEM_ENTRY (                                                      \
                              DS_3GPP_APM_APN_SWITCHING,                          \
                              "Enable/Disable Apn Switching",                     \
                              "data/dsd/ds_apn_switching",                        \
                              sizeof(boolean)                                     \
                           ) /*NV-item number: 69769 */                           \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_MAX_RAB_REESTAB_COUNT,                      \
                              "Max Rab Reestab Count",                            \
                              "data/3gpp/max_rab_reestab_retry_cnt",              \
                              sizeof(uint32)                                      \
                           ) /*NV-item number: 65780 */                           \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_ULDL_TPUT_T_MEAS,                           \
                              "Tmeas for ULDL Throughput reporting",              \
                              "modem/data/3gpp/ps/t_meas_uldl",                   \
                              sizeof(uint32)                                      \
                           ) /*NV-item number: 65780 */                           \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_TPUT_OPT_PARAMS,                            \
                              "3GPP TPUT OPT PARAMETERS",                         \
                              "data/common/tput_opt_params",                      \
                              sizeof(ds_bearer_cntxt_bp_removal_struct_type)      \
                           ) /*NV-item number: 66051 */                            \
    DS_3GPPI_CFG_ITEM_ENTRY (                                                     \
                               DS_3GPP_ENABLE_PDN_THROTTLING_PER_RAT,             \
                               "3GPP Ignore Data Settings",                       \
                               "modem/data/3gpp/ps/enable_pdn_throttling_per_rat",\
                               sizeof(boolean)                                    \
                             ) /* NV 73591 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY (                                                     \
                               DS_3GPP_ENABLE_EMERGENCY_PDN_THROTTLING,           \
                               "3GPP enable emergency pdn throttling",            \
                               "data/3gpp/enable_emergency_pdn_throttling",       \
                               sizeof(boolean)                                    \
                             ) /* NV 73966 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY (                                                     \
                               DS_3GPP_CHECK_EHPLMN_LIST,                         \
                               "Check EHPLMN List",                               \
                               "modem/data/3gpp/ps/check_ehplmn_list",            \
                               sizeof(boolean)                                    \
                             ) /* NV 73590 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY (                                                     \
                               DS_3GPP_IGNORE_DATA_SETTINGS,                      \
                               "Ignore Data Settings",                            \
                               "modem/data/3gpp/ps/ignore_data_settings",         \
                               sizeof(boolean)                                    \
                             ) /* NV 73589 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY (                                                     \
                               DS_3GPP_ENABLE_DYNAMIC_WCDMA_WM_TUNING,            \
                               "Enable/Disable WCDMA WM Tuning",                  \
                               "data/3gpp/enable_dynamic_wcdma_wm_tuning",        \
                               sizeof(boolean)                                    \
                             ) /* NV 73689 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY (                                                     \
                               DS_3GPP_DATA_SHARK_ENABLE,                         \
                               "3GPP Data Performance Shark loopback Enable",     \
                               "modem/data/3gpp/ps/data_shark_enable",            \
                               sizeof(boolean)                                    \
                             ) /* NV 73700 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_NULL_APN_SUPPORT_FOR_NON_ATTACH_PDN,        \
                              "3GPP NULL APN SUPPORT NON ATTACH PDN",             \
                              "modem/data/3gpp/lteps/null_apn_support_for_non_attach_pdn",\
                              sizeof(boolean)                                     \
                           ) /* NV 73716 */                                       \
   DS_3GPPI_CFG_ITEM_ENTRY(                                                       \
                               DS_3GPP_UPLINK_TPUT_WCDMA_TACCUM,                  \
                               "Taccum for UPLINK Wcdma Throughput reporting",    \
                               "modem/data/3gpp/ps/wcdma_taccum_uplink",          \
                               sizeof(uint32)                                     \
                            ) /* NV 73728 */                                      \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_UPLINK_TPUT_LTE_TACCUM,                    \
                               "Taccum for UPLINK LTE  Throughput reporting",     \
                               "modem/data/3gpp/ps/lte_taccum_uplink",            \
                               sizeof(uint32)                                     \
                            ) /* NV 73729 */                                      \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_EFNAS_CONFIG_PATH,                      \
                              "EFS NAS CONFIG PATH",                              \
                              "modem/nas/efnas_config",                           \
                              DS_3GPP_EFNAS_CONFIG_SIZE                           \
                           ) /*No NV number here*/                                \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_LTE_CA_DISPLAY_THRESHOLD,                   \
                              "LTE CA Display Threshold",                         \
                              "data/3gpp/lteps/carrier_aggregation_display_threshold",\
                              sizeof(ds_eps_ca_display_threshold_s)               \
                           ) /* NV 73770 */                                       \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_MNC_LEN,                                \
                              "Set MNC LEN for IMSI",                             \
                              "modem/data/3gpp/ps/mnc_len",                       \
                              sizeof(uint8)                                       \
                           ) /* NV 73817 */                                       \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_REL12_QCI_SUPPORT,                         \
                               "3GPP REL12 QCI SUPPORT",                          \
                               "modem/data/3gpp/ps/rel12_qci_supported",          \
                               sizeof(boolean)                                    \
                            )  /* NV 73819 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_MANAGE_ATTACH_ON_PWR_UP,                   \
                               "3GPP control attach on pwr up",                   \
                               "modem/data/3gpp/ps/manage_attach_on_pwr_up",      \
                               sizeof(ds_3gpp_pre_attach_pwr_up_manage_s)         \
                            )  /* NV 73856 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_F3_SUPPRESS,                               \
                               "Supression supoort for f3 diag flood",            \
                               "data/3gpp/ds_debug_msg_enable_mask",              \
                               sizeof(uint32)                                      \
                            )  /* NV 73910 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_PLATFORM_CONFIG,                           \
                               "Device Platform Config",                          \
                               "data/3gpp/platform_config",                       \
                               sizeof(uint8)                                      \
                            )  /* NV 73890 */								      \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_PERSISTENT_PROFILE_OPTIMIZATION,           \
                               "3GPP persistent profile optimization",            \
                               "modem/data/3gpp/ps/persistent_profile_optimization",\
                               sizeof(boolean)                                    \
                            )  /* **** */                                         \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_SWITCH_ATTACH_APN_OVER_DUN,                \
                               "Switch Attach APN Over DUN",                      \
                               "modem/data/3gpp/ps/switch_attach_apn_over_dun",   \
                               sizeof(boolean)                                    \
                            )  /* NV 73940 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_NW_OVERRIDE_CLEANUP,                       \
                               "Enable/Disable nw override cleanup when bearer is deactivated by NW",\
                               "modem/data/3gpp/ps/nw_override_cleanup",          \
                               sizeof(boolean)                                    \
                            )  /* NV 74009 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_CFG_MODEM_DEF_PROF_SUPPORT,                \
                               "Support for modem_def_profile ",                  \
                               "modem/data/3gpp/ps/modem_def_prof_support",       \
                               sizeof(boolean)                                    \
                            )  /* NV 74039 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_EFNAS_FAILURE_DEF_RETRY_TIMER,             \
                               "Read EFNAS Failure Def Retry timer",              \
                               "modem/data/3gpp/ps/efnas_failure_def_retry_timer",\
                               sizeof(uint32)                                    \
                            )  /* NV ***** */                                    \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_EFNAS_FAILURE_DEF_RETRY_TIMER_PER_RAT,             \
                               "Read EFNAS Failure Def Retry timer Per Rat",              \
                               "modem/data/3gpp/ps/efnas_failure_def_retry_timer_per_rat",\
                               sizeof(ds_3gpp_nv_efnas_failure_def_retry_timer_info)         \
                            )  /* NV ***** */                                    \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_ALLOW_SINGLEIP_DUNCALL_DUALIP_PROFILE,             \
                               "Allows Single Ip DUN call for Dual IP profile",              \
                               "modem/data/3gpp/ps/single_ip_dun_call_in_dual_ip_profile",\
                               sizeof(ds_3gpp_dun_call_config_type)                                    \
                            )  /* NV 74047 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_CFG_ATTACH_ACTION_TYPE,                     \
                              "Specify behavior for attach action type",          \
                              "modem/data/3gpp/ps/attach_action_type",            \
                              sizeof(uint32)                                      \
                           ) /* NV-item number: 74074 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                               DS_3GPP_DISABLE_MTU_REQUEST,                       \
                               "Disable MTU Request",                             \
                               "modem/data/3gpp/ps/disable_mtu_request",          \
                               sizeof(boolean)                                    \
                            )  /* NV 74033 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_NW_OVERRIDE_ENHANCEMENT,                    \
                              "3GPP Network override enhancement",                \
                              "modem/data/3gpp/ps/nw_override_enhancement",       \
                              sizeof(boolean)                                     \
                           )  /*  NV 74035 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_TEARDOWN_UNUSED_PDN_AFTER_IRAT_BITMASK_PATH,    \
                              "Tear down unused pdn after irat Feature",      \
                              "modem/data/3gpp/ps/teardown_unused_pdn_after_irat_bmask", \
                              sizeof(uint8)                                     \
                           )  /*  NV 74286 */                                     \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_ATTACH_WITH_NW_OVERRIDE_ENHANCEMENT,        \
                              "3GPP Attach with Network override enhancement",    \
                              "modem/data/3gpp/ps/attach_with_nw_override",       \
                              sizeof(boolean)                                     \
                           )  /*   */                                             \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_LTE_4_5G_UI_DISPLAY,                        \
                              "LTE 4.5G and 4.5G+ UI Display",                    \
                              "data/3gpp/lteps/ui_display_4_5g_support",          \
                              sizeof(uint8)                                       \
                           ) /* NV 74089 */                                       \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_FCF_PARAMS,                                 \
                              "3GPP FCF params",                                  \
                              "modem/data/3gpp/ps/fcf_params",                    \
                              sizeof(ds_3gpp_fcf_bidir_struct_type)      \
                           ) /* NV 74073 */                                       \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_PSDATAOFF_EXEMPT_SERVICES_PATH,             \
                              "PS Data Off Exempt Services Feature",              \
                              "modem/data/3gpp/ps/ps_data_off_feature",           \
                              sizeof(boolean)                                     \
                           ) /* NV 74081 */                                       \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_GENERIC_PDN_THROT_CONFIG,                   \
                              "Enable/Disable default generic PDN throttling",    \
                              "modem/data/3gpp/ps/enable_default_pdn_throt",      \
                              sizeof(boolean)                                     \
                           ) /* NV-item number: 74212 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_GENERIC_THROT_SKIP_SM_RETRY,                   \
                              "Generic throttling for failures that would "        \
                              "otherwise be throttled using SM_RetryWaitTime",    \
                              "modem/data/3gpp/ps/generic_throt_skip_sm_retry",      \
                              sizeof(boolean)                                     \
                           ) /* NV-item number: 74808 */                          \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_RAN_BIT_RATE_SUPPORT,                       \
                              "Configuration for recommended and query bit rate", \
                              "modem/data/3gpp/ps/ran_bit_rate_support",          \
                              sizeof(ds_3gpp_ran_bit_data_rate_support_config)    \
                           )  /* NV ***** */                                      \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_W_TO_L_REDIR,                               \
                              "WCDMA to LTE Redirection Config",                  \
                              "modem/data/3gpp/ps/w_to_l_redir_config",           \
                              sizeof(ds3gtputmgr_w2l_redir_config_s)              \
                           ) /* NV 74113 */                                       \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_IGNORE_PRECED_CONFLICT_ON_SAME_BEARER,      \
                              "Ignore precedence conflict on the same bearer",    \
                              "modem/data/3gpp/ps/ignore_preced_conflict_on_same_bearer",\
                              sizeof(boolean)                                     \
                           ) /* NV 74185 */                                      \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_LTE_ATTACH_PENDING,                         \
                              "LTE attach pending",                               \
                              "modem/data/3gpp/lteps/lte_attach_pending",            \
                              sizeof(boolean)                                     \
                           )  /* NV 74227 */                                      \
 DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                            DS_3GPP_KAMGR_ALWAYS_ON,                     \
                            "keep KAMGR on regardless of Wifi or UI setting",            \
                            "modem/data/3gpp/ps/kamgr_always_on",        \
                            sizeof(boolean)                                      \
                           )  /* NV 74309 */                                           \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                      \
                              DS_3GPP_USR_PWD_CASE_SENSITIVE,                     \
                              "user name and password case sensitive",            \
                              "modem/data/3gpp/ps/usr_pwd_case_sensitive",        \
                              sizeof(uint16)                                      \
                           )  /* NV 74304 */                                      \
    DS_3GPPI_CFG_ITEM_ENTRY(                                                          \
                               DS_3GPP_FORCE_DISCONNECT_CONFIG,                   \
                               "Force PDN disconnection",                         \
                               "modem/data/3gpp/ps/force_disconnection",          \
                                sizeof(boolean)                                   \
                          )  /* NV 74414 */                                       \
  DS_3GPPI_CFG_ITEM_ENTRY(                                                        \
                           DS_3GPP_IGNORE_IMS_SETTING,                            \
                           "3GPP DCM requirement",                                \
                           "modem/data/3gpp/lteps/ignore_ims_setting",            \
                           sizeof(boolean)                                        \
                         )  /*  NV 74343 */                                

#define DS_3GPPI_CFG_ITEM_ENTRY(ds_3gppi_cfg_item, ...) ds_3gppi_cfg_item,

typedef enum 
{
#ifdef FEATURE_DATA_LTE
  DS_3GPPI_CFG_TABLE_LTE
#endif /* FEATURE_DATA_LTE */
#ifdef FEATURE_DATA_TEST_LOOPBACK
  DS_3GPPI_CFG_TABLE_TEST_LOOPBACK
#endif /* FEATURE_DATA_TEST_LOOPBACK */
  DS_3GPPI_CFG_TABLE
    DS_3GPP_CFG_ILLEGAL
} ds_3gpp_cfg_item_e;

#undef DS_3GPPI_CFG_ITEM_ENTRY

/* @brief
  Structure to hold the apn identifiers read
  from the EFS 
  */
typedef struct
{
  ds_profile_3gpp_param_enum_type   profile_parm;
  void*                             identifier;
}ds_3gpp_apn_disconn_order_list_type;

typedef enum
{
    DS_3GPPI_UTILS_ATTACH_FB_IP_TYPE_MIN =0,
    DS_3GPPI_UTILS_ATTACH_FB_IP_TYPE_IPV4,
    DS_3GPPI_UTILS_ATTACH_FB_IP_TYPE_IPV6,
    DS_3GPPI_UTILS_ATTACH_FB_IP_TYPE_MAX
}ds_3gppi_utils_attach_fb_type;

/*------------------------------------------------------------------------------
  Structure that contains the platform config information extracted from EFS
------------------------------------------------------------------------------*/
typedef enum {
  PLATFORM_CONFIG_LINUX = 0,
  PLATFORM_CONFIG_WP = 1,
  PLATFORM_CONFIG_MAX,
} ds_3gpp_platform_config_e_type;

/*------------------------------------------------------------------------------
  Structure that contains the information extracted any EFS file
------------------------------------------------------------------------------*/
typedef struct
{
  q_link_type          link;
  char                *extracted_data;
  uint16               extracted_data_length;
}ds_3gppi_utils_extracted_info_type;
/*------------------------------------------------------------------------------
  Maximum number of PLMNs we would support simultaneously
------------------------------------------------------------------------------*/
#define DS_3GPP_MAX_PLMN_SUPPORTED 8


/* Memory allocation wrapper */
#define DS_3GPP_MEM_ALLOC(size)  modem_mem_alloc((size), MODEM_MEM_CLIENT_DATA)
#define DS_3GPP_MEM_FREE(ptr)    modem_mem_free((void *)ptr, MODEM_MEM_CLIENT_DATA)

#define DS_3GPP_EFS_CONFIGURATION_FILE_PATH  \
               "/nv/item_files/conf/data_config_info.conf"

/*===========================================================================

                           FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION ds_3gpp_is_rel_11_or_above_supported

DESCRIPTION
  Determine if UE is release 11 ready or not

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE 
   TRUE - if device is release 11 ready
   FALSE - Otherwise
  
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gppi_is_rel_11_or_above_supported
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_is_rel_12_or_above_supported

DESCRIPTION
  Determine if UE is release 12 ready or not

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE 
   TRUE - if device is release 12 ready
   FALSE - Otherwise
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gppi_is_rel_12_or_above_supported
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPPI_IS_REL_13_OR_ABOVE_SUPPORTED

DESCRIPTION
  Determine if UE is release 13 ready or not

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE 
   TRUE - if device is release 13 ready
   FALSE - Otherwise
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gppi_is_rel_13_or_above_supported
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPPI_IS_REL_14_OR_ABOVE_SUPPORTED

DESCRIPTION
  Determine if UE is release 14 ready or not

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE 
   TRUE - if device is release 14 ready
   FALSE - Otherwise
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gppi_is_rel_14_or_above_supported
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPPI_IS_REL_15_OR_ABOVE_SUPPORTED

DESCRIPTION
  Determine if UE is release 15 ready or not

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE 
   TRUE - if device is release 15 ready
   FALSE - Otherwise
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gppi_is_rel_15_or_above_supported
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPPI_ONLY_REL_11_SUPPORTED

DESCRIPTION
  Determine if UE has release only 11 support or not

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE 
   TRUE - if device is release 11 ready only
   FALSE - Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gppi_only_rel_11_supported
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_is_lte_rel_11_or_below_supported

DESCRIPTION
  Determine if UE supports LTE release 11 or below versions

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE 
   TRUE - if device is <= release 11
   FALSE - Otherwise
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gppi_is_lte_only_rel_11_or_below_supported
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION ds_3gppi_utils_read_items_from_efs

DESCRIPTION 
  This function reads the EFS NV and saves for caching 
 
PARAMETERS
  None
DEPENDENCIES
  None.

RETURN VALUE
  None
SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gppi_utils_read_items_from_efs(void);

/*===========================================================================
FUNCTION DS_3GPPI_UTILS_READ_ITEMS_FROM_EFS_PER_SUBS_ID

DESCRIPTION 
  This function reads the EFS items  per subsid
PARAMETERS
  None
DEPENDENCIES
  None.

RETURN VALUE
  None
SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gppi_utils_read_items_from_efs_per_subs_id
(
  sys_modem_as_id_e_type  subs_id
);


/*===========================================================================
FUNCTION DS_3GPPI_UTILS_INIT

DESCRIPTION
  Initializes utilities. Must be done at init time once when task is started.
  
PARAMETERS  
  None.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gppi_utils_init(void);

/*===========================================================================
FUNCTION DS_3GPP_CFG_SET

DESCRIPTION
  Sets configuration item in non-volatile store (EFS).
  
PARAMETERS  
  item - configuration item to set
  val  - location of bytes to write to store
  len  - size of the configuration item in bytes
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  boolean - TRUE if item was set successfully, FALSE otherwise.
   
SIDE EFFECTS 
  None.
===========================================================================*/
boolean ds_3gpp_cfg_set
(
    ds_3gpp_cfg_item_e  item, 
    unsigned char     * val, 
    size_t              len
);

/*===========================================================================
FUNCTION DS_3GPP_CFG_GET

DESCRIPTION
  Gets configuration item from non-volatile store (EFS).
  
PARAMETERS  
  item - configuration item to get
  val  - location where value read should be written
  len  - size of the configuration item in bytes
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
 int - number of bytes read successfully from the file
       (-1, on error).
   
SIDE EFFECTS 
  None.
===========================================================================*/
int ds_3gpp_cfg_get
(
    ds_3gpp_cfg_item_e  item, 
    unsigned char     * val, 
    size_t             len
);


/*===========================================================================
FUNCTION DS_3GPP_CFG_GET

DESCRIPTION
  Gets configuration item from non-volatile store (EFS) per subscription id
  
PARAMETERS  
  item - configuration item to get
  val  - location where value read should be written
  len  - size of the configuration item in bytes
  subs - subscription id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
 int - number of bytes read successfully from the file
       (-1, on error).
   
SIDE EFFECTS 
  None.
===========================================================================*/


int ds_3gpp_cfg_get_subsid
(
    ds_3gpp_cfg_item_e  item, 
    unsigned char     * val, 
    size_t             len,
    sys_modem_as_id_e_type subs_id
);



/*===========================================================================
  FUNCTION DS_3GPP_UPDATE_DATA_CONFIG_INFO
  
  DESCRIPTION
    Puts all the required nv item file paths in the data_config_info.conf 
    file.
    Would be called from dsutils during powerup.
    
  PARAMETERS  
    file_paths_buf:    data_config_info file path.
    file_path_buf_len: length of above string
      
  DEPENDENCIES 
    None.
    
  RETURN VALUE 
    0 for success.
    -1 for failure.
     
  SIDE EFFECTS 
    None.
    
===========================================================================*/
int32 ds_3gpp_update_data_config_info
(
  char  *file_paths_buf,
  uint32 file_paths_buf_len
);

#define DS_3GPP_DATA_CONF_BUF_SIZE ds_3gpp_get_data_conf_buf_size()

/*===========================================================================
  FUNCTION DS_3GPP_GET_DATA_CONF_BUF_SIZE
  
  DESCRIPTION
    This function returns the buffer size to allocate for storing 
    3GPP EFS Item File paths.
    
  PARAMETERS  
    None.
      
  DEPENDENCIES 
    None.
    
  RETURN VALUE 
    Buffer Size (uint32)
     
  SIDE EFFECTS 
    None.
    
===========================================================================*/
uint32 ds_3gpp_get_data_conf_buf_size 
(
  void
);

/*===========================================================================
  FUNCTION DS_3GPP_CHANGE_BYTE_ORDER
  
  DESCRIPTION
    Changes the byte order
    
  PARAMETERS  
    uint32 containing the original byte order
      
  DEPENDENCIES 
    None.
    
  RETURN VALUE 
    unit32 containing the changed (reverse) byte order
     
  SIDE EFFECTS 
    None.
    
===========================================================================*/

uint32 ds_3gpp_chng_byte_order
(
   uint32
);
/*===========================================================================
FUNCTION DS_3GPPI_UTILS_IS_APN_DISCONN_ORDER_LIST_EMPTY

DESCRIPTION
  Checks if the APN disconnect order list is empty 
  The list will be used while applying Bearer mgmt algorithm to allocate
  bearers on priority for upcoming PDN.

PARAMETERS
  None             

DEPENDENCIES
  The list is empty, if there is no EFS configuration

RETURN VALUE
  TRUE     - If the apn disconnect priority list is empty
  FALSE    - otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gppi_utils_is_apn_disconn_order_list_empty(sys_modem_as_id_e_type subs_id);


void ds_3gppi_utils_clear_apn_disconn_order_list_per_subsid
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION DS_3GPPI_UTILS_GET_APN_DISCONN_ORDER_LIST_INFO

DESCRIPTION
  Given the Index, this utility function provides the APN disconnect
  information available at index
PARAMETERS
  None             

DEPENDENCIES
  The list is empty, if there is no EFS configuration

RETURN VALUE
  void*     - Returns a pointer to the memory location at queried index
  NULL      - If the index is invalid

SIDE EFFECTS
  None.

===========================================================================*/
void* ds_3gppi_utils_get_apn_disconn_order_list_info
(  
  uint8 index,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION      DS_READ_REJECT_APN_FROM_EFS

DESCRIPTION   This function will read from the efs file to see if the reject 
              apn has been set 

              EFS File Format - Param_Name:Param_Val;
              For example     - reject_apn:test.apn;

DEPENDENCIES  None

RETURN VALUE  TRUE - If parsing is successful 
              FALSE - Otherwise 

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gppi_utils_read_apn_info_from_efs 
(
  char                        *comparison_tag,
  char                        *efs_file_name,
  q_type                      *efs_extracted_info_q,
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
  FUNCTION DS_3GPP_EXTRACT_MCC_MNC_FROM_PLMN_ID
  
  DESCRIPTION
    This function extracts MCC and MNC from PLMN ID
    
  PARAMETERS  
   sys_plmn_id_s_type          *current_plmn_id
   uint16                      *ret_mcc (MCC ptr)
   uint16                      *ret_mnc (MNC ptr)
      
  DEPENDENCIES 
    None.
    
  RETURN VALUE 
    void
     
  SIDE EFFECTS 
    None.
    
===========================================================================*/

void ds_3gpp_extract_mcc_mnc_from_plmn_id
(
   sys_plmn_id_s_type          *current_plmn_id,
   uint16                      *ret_mcc,
   uint16                      *ret_mnc,
   sys_modem_as_id_e_type       subs_id   
);

/*===========================================================================
FUNCTION DS3G_COMPARE_APN_NAMES
 
DESCRIPTION
  This function compares apn names being and return TRUE if they are same 

PARAMETERS 
  byte apn_src : First APN string
  byte apn_dest: Second APN string
 
DEPENDENCIES 
  None. 
  
RETURN VALUE 
  boolean TRUE: APN's passed are same
          FALSE: APN's passed are different
 
SIDE EFFECTS 
  None 
  
===========================================================================*/
boolean ds_3gpp_compare_apn_names
(
  byte *apn_src,
  byte *apn_dest
);

/*===========================================================================
FUNCTION DS_3GPP_READ_LOW_PRIO_BIT
 
DESCRIPTION
  This function takes a byte stream and checks to see if Low_prio_signaling
  is present in the byte stream.
 
  It returns TRUE if Low priority bit is present in the file.
  The value of the low priority bit is returned through a ptr

PARAMETERS 
  Byte stream
  Length of the byte stream
  Output ptr to write the low prio bit.
 
DEPENDENCIES 
  None. 
  
RETURN VALUE 
  Boolean 
 
SIDE EFFECTS 
  None 
  
===========================================================================*/
boolean ds_3gpp_read_read_nas_config_file
(
  ds3g_mmgsdi_tag_e_type    tag_id,
  sys_modem_as_id_e_type    subs_id,
  void                      *tag_value_p
);

/*===========================================================================
FUNCTION  DS_3GPP_MAP_DS_UMTS_PDP_TYPE_TO_PS_SYS_PDP_TYPE

DESCRIPTION    
  map ds umts pdp type to ps sys pdp type
 
PARAMETERS 
  pdn_pdp_type
 
DEPENDENCIES   
  NONE

RETURN VALUE   
  NONE

SIDE EFFECTS   
  NONE
===========================================================================*/
ps_sys_pdp_enum_type  ds_3gpp_map_ds_umts_pdp_type_to_ps_sys_pdp_type
(
   ds_umts_pdp_type_enum_type  pdn_pdp_type
);
#endif /* DS_3GPPI_UTILS_H */
