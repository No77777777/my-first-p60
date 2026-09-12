/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         Diagnostics over QSHTRACE

General Description
This routines in this file are used to send QSHTRACE data over STM using the debugtrace
interfaces or over diagbuf.

Copyright (c) 2016 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                              Edit History

 $Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diag_qshtrace_i.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/05/16   sa     File creation
===========================================================================*/



/*===========================================================================

FUNCTION DIAG QSHTRACE CONFI MODE

DESCRIPTION
  This function checks if STM/QSHTRACE is enable and switches the function pointer to enable writing to STM/DIAGBUF

PARAMETERS
  None

DEPENDENCIES
  None

RETURN
  mode -
  DIAG_QSHTRACE_MODE_DIAGBUF - DIAG will send data thorugh diagbuf
  DIAG_QSHTRACE_MODE_DIAGBUF - Diag will send data thorugh STM

===========================================================================*/
uint8 diag_qshtrace_config_mode();


//Varibale to check which mode the QHSTRACE is running right now.Can be used for debugging and checking the mode.
uint8 diag_qshtrace_mode;


//Modes used by the QSHTRACE
#define DIAG_QSHTRACE_MODE_STM     1
#define DIAG_QSHTRACE_MODE_DIAGBUF 2
#define DIAG_QSHTRACE_MODE_MAX     3




