#ifndef MODEM_AC_TARGET_H
#define MODEM_AC_TARGET_H

/**
* @file modem_xpu_int.h
* @brief Modem XPU Interrupt Handler
*
* This file implements the XPU interrupt Handling. All XPU violations are reported
* over DIAG (Subsystem QDSP6) and to a ring buffer referenced by 'modem_sec_xpu_buffer'.
*
*/
/*===========================================================================
   Copyright (c) 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/core.mpss/10.0/securemsm/modem_sec/inc/chipset/kamorta/modem_ac_target.h#2 $
  $DateTime: 2020/04/01 03:35:40 $
  $Author: pwbldsvc $

===========================================================================*/

#define MAX_RG_SUPPORTED_IN_TARGET 10
#define MSS_TZ_SHARED_CHANNEL_BASE_ADDR 0x462C0000
#define MSS_TZ_SHARED_CHANNEL_END_ADDR 0x462C1000
#define MSS_TZ_AC_DATA_STRUCT_OFFSET  0xE00        /* Maximum of 16 RGs supported */

#define KAMORTA_TARGET 		0
#define AGATTI_TARGET  		1
#define AGATTI_TCSR_SOC_HW_DEVICE_NUM 	0x9003 		/* 0x90030100 */

#define NUM_OF_SUPPORTED_TARGETS 2				/* Kamorta and Agatti */

#endif
