#ifndef UIM_SELECTIVE_LOGGING_H
#define UIM_SELECTIVE_LOGGING_H
/*===========================================================================


           U I M   S E L E C T I V E   L O G   H E A D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016-17 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/common/inc/uim_selective_logging.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
12/28/17   dt      GSTK F3 reduction
11/02/16   me      NVRUIM F3 reduction  
10/07/16   shr     GSTK/ESTK F3 reduction
07/05/16   ar      Removing array for selective logging
06/22/16   ar      Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

#include "uim_msg.h"
#include "uim_common_efs.h"

/*=============================================================================

                                 EXTERN VARIABLES

=============================================================================*/

extern uint8   uim_selective_log_eons;
extern uint8   uim_selective_log_toolkit;
extern uint8   uim_selective_log_nvruim;

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
#define UIM_LOG_EONS                      uim_selective_log_eons
#define UIM_LOG_TOOLKIT                   uim_selective_log_toolkit
#define UIM_LOG_NVRUIM                    uim_selective_log_nvruim


/*=============================================================================

                           FUNCTION DECLARATIONS

=============================================================================*/
/*===========================================================================
   FUNCTION:      UIM_SELECTIVE_LOGGING_INIT

   DESCRIPTION:
     This function initialise the selective logging
     functionality by reading NV item 73880

   DEPENDENCIES:
     None

   LIMITATIONS:
     None


   RETURN VALUE:
    Void

   SIDE EFFECTS:
     None

=============================================================================*/
void uim_selective_logging_init
(
  void
);


/*=============================================================================

                                 MACROS
               MACRO Definitions used for logging throughout UIM

=============================================================================*/
#define UIM_SELECTIVE_MSG_HIGH_0(module_enabled, xx_fmt)           \
  if (module_enabled)                                              \
  {                                                                \
    UIM_MSG_HIGH_0(xx_fmt);                                        \
  }

#define UIM_SELECTIVE_MSG_HIGH_1(module_enabled, xx_fmt, xx_arg1) \
  if (module_enabled)                                             \
  {                                                               \
    UIM_MSG_HIGH_1(xx_fmt, xx_arg1);                              \
  }

#define UIM_SELECTIVE_MSG_HIGH_2(module_enabled, xx_fmt, xx_arg1, xx_arg2) \
  if (module_enabled)                                                      \
  {                                                                        \
    UIM_MSG_HIGH_2(xx_fmt, xx_arg1, xx_arg2);                              \
  }

#define UIM_SELECTIVE_MSG_HIGH_3(module_enabled, xx_fmt, xx_arg1, xx_arg2, xx_arg3)  \
  if (module_enabled)                                                                \
  {                                                                                  \
    UIM_MSG_HIGH_3(xx_fmt, xx_arg1, xx_arg2, xx_arg3);                               \
  }

#define UIM_SELECTIVE_MSG_HIGH_4(module_enabled, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)  \
  if (module_enabled)                                                                         \
  {                                                                                           \
    UIM_MSG_HIGH_4(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4);                               \
  }

#define UIM_SELECTIVE_MSG_HIGH_5(module_enabled, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5)   \
  if (module_enabled)                                                                                   \
  {                                                                                                     \
    UIM_MSG_HIGH_5(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5);                                \
  }

#define UIM_SELECTIVE_MSG_HIGH_6(module_enabled, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6)   \
if (module_enabled) 												 \
  {														 \
    UIM_MSG_HIGH_6(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6);                                \
  }

#define UIM_SELECTIVE_MSG_HIGH_7(module_enabled, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7) \
  if (module_enabled)                                                                                                   \
  { 															\
    UIM_MSG_HIGH_7(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7);				\
  }

#define UIM_SELECTIVE_MSG_HIGH_8(module_enabled, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8) \
  if (module_enabled) 														 \
  {																 \
    UIM_MSG_HIGH_8(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8);				 \
  }

#define UIM_SELECTIVE_MSG_HIGH_9(module_enabled, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8, xx_arg9) \
  if (module_enabled)															  \
  {                                                                                                                                       \
    UIM_MSG_HIGH_9(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8, xx_arg9);                              \
  }

#define UIM_SELECTIVE_MSG_ERR_0(module_enabled, xx_fmt)            \
  if (module_enabled)                                              \
  {                                                                \
    UIM_MSG_ERR_0(xx_fmt);                                         \
  }

#define UIM_SELECTIVE_MSG_ERR_1(module_enabled, xx_fmt, xx_arg1)  \
  if (module_enabled)                                             \
  {                                                               \
    UIM_MSG_ERR_1(xx_fmt, xx_arg1);                               \
  }

#define UIM_SELECTIVE_MSG_ERR_2(module_enabled, xx_fmt, xx_arg1, xx_arg2)  \
  if (module_enabled)                                                      \
  {                                                                        \
    UIM_MSG_ERR_2(xx_fmt, xx_arg1, xx_arg2);                               \
  }

#define UIM_SELECTIVE_MSG_ERR_3(module_enabled, xx_fmt, xx_arg1, xx_arg2, xx_arg3)  \
  if (module_enabled)                                                               \
  {                                                                                 \
    UIM_MSG_ERR_3(xx_fmt, xx_arg1, xx_arg2, xx_arg3);                               \
  }

#define UIM_SELECTIVE_MSG_ERR_4(module_enabled, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)  \
  if (module_enabled)                                                                        \
  {                                                                                          \
    UIM_MSG_ERR_4(xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4);                               \
  }

#endif /* UIM_SELECTIVE_LOGGING_H */
