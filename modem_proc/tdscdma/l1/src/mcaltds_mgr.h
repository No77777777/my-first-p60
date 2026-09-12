#if 0
#ifndef MCALWCDMA_MGR_H
#define MCALWCDMA_MGR_H

/*============================================================================*/
/** @file  
 * This module has definition and declaration related to MCAL Manager
 */
/*============================================================================*/

/*============================================================================
Copyright (c) 2006 - 2008 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/mcaltds_mgr.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
05/25/09    hk      Fixed compiler warnings
10/03/08    hk      Bringing in 7k Mailine fixes
01/23/08    mc      Introduced in mainline from MBMS dev branch.
01/26/07    yh      Added support for MCAL CM
03/03/07    mc      Sync up for MBMS phase2A integration
10/12/06    rc      Included a header file.
09/28/06    mc      Added to //depot/asic/msmshared/umts/wcdma/l1/mcal/

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#ifdef FEATURE_MCAL_TDSCDMA
#include "comdef.h"
#include "dsm.h"
#include "msg.h"
#include "log.h"


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/*--------------------------------------------------------------------------
Macros for reading the mDSP version information
--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
--------------------- READ AND WRITE VALUES TO THE MDSP -----------------------
-----------------------------------------------------------------------------*/

/* read a 16 bit value from the MDSP */
#define MDSP_16BIT_READ(port) (uint16) mdsp_read(((volatile uint16 *)(port)))


/* read a 32 bit value from the MDSP */
#define MDSP_32BIT_READ(port) (uint32) mdsp_read(((volatile uint32 *)(port)))


/* read a 16 bit value from the MDSP */
#define MDSP_16BIT_READ(port) (uint16) (*((volatile uint16 *) ((uint32)(port))))

/* write a 16 bit value from the MDSP */
#define MDSP_16BIT_WRITE(port, val) \
  (*((volatile uint16 *) (((uint32)(port)))) = ((uint16) (val)))

/* read a 32 bit value from the MDSP */
#define MDSP_32BIT_READ(port) (uint32) (*((volatile uint32 *) (((uint32)(port)))))

/* write a 32 bit value from the MDSP */
#define MDSP_32BIT_WRITE(port, val) \
  (*((volatile uint32 *) (((uint32)(port)))) = ((uint32) (val)))
/*---------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------
Macros for reading the mDSP version information
--------------------------------------------------------------------------*/
#define READ_MDSP_ROM_VERSION_INFO() MDSP_READ(MDSP_VERSION_ROM)

#define MDSP_MAJOR_ROM_VERSION(version) (((version) & 0xFF00) >> 8)

#define MDSP_MINOR_ROM_VERSION(version) ((version) & 0x00FF)

#define READ_MDSP_RAM_VERSION_INFO() MDSP_16BIT_READ(MDSP_VERSION_RAM)

#define MDSP_MAJOR_RAM_VERSION(version) (((version) & 0xFF00) >> 8)

#define MDSP_MINOR_RAM_VERSION(version) ((version) & 0x00FF)



/* Enumeration of types of MCAL module */
typedef enum
{
  MCALWCDMA_MODULE_MGR,
  MCALWCDMA_MODULE_SEARCH,
  MCALWCDMA_MODULE_CM,
  MCALWCDMA_MODULE_DL_DEMOD,
  MCALWCDMA_MODULE_DL,
  MCALWCDMA_MODULE_DEC,
  MCALWCDMA_MODULE_ENC,
  MCALWCDMA_MODULE_HS,
 #ifdef FEATURE_L1_LOG_ON_DEMAND
  MCALWCDMA_MODULE_LOGOD,
 #endif
  MCALWCDMA_NUM_MODULE
} mcalwcdma_mgr_module_enum_type;

/* Prototype of the MCAL module callback function for logging */
typedef void MCALWCDMA_MGR_LOG_SUBMIT_REQ_FUNC_CB(void);

/* Structure of the callback types */
typedef struct
{
  MCALWCDMA_MGR_LOG_SUBMIT_REQ_FUNC_CB *cb_func;
} mcalwcdma_mgr_log_submit_req_func_struct_type;

/* Macro to get and set a bit for the corresponding module in the module log submit request mask */
#define MCALWCDMA_MGR_GET_LOG_MODULE(module) \
  (mcalwcdma_mgr_module_log_submit_req_pending & (1 << module))

#define MCALWCDMA_MGR_SET_LOG_MODULE(module, value)  \
  mcalwcdma_mgr_module_log_submit_req_pending &= (~((uint32)(1 << module))); \
  mcalwcdma_mgr_module_log_submit_req_pending |= (value << module)

#ifdef FEATURE_L1_LOG_ON_DEMAND
/* CCTrCh log on demand bitmask */
#define MCALWCDMA_MGR_LOGOD_DL_CCTRCH_LOG_PKT_BMSK   0x01
#define MCALWCDMA_MGR_LOGOD_DL_CCTRCH_LOG_PKT_SHFT   0

/* TFC log on demand bitmask */
#define MCALWCDMA_MGR_LOGOD_DL_TFC_LOG_PKT_BMSK   0x02
#define MCALWCDMA_MGR_LOGOD_DL_TFC_LOG_PKT_SHFT   1

/* Macro to extract the bit field value from the EQ control mask */
#define MCALWCDMA_MGR_GET_LOGOD_BF_PARAM(buffer, type) \
  ((buffer & MCALWCDMA_MGR_LOGOD_##type##_BMSK) >> MCALWCDMA_MGR_LOGOD_##type##_SHFT)

/* Macro to set the bit foeld value in the EQ control mask */
#define MCALWCDMA_MGR_SET_LOGOD_BF_PARAM(buffer, type, value) \
  buffer = ((buffer & ~(MCALWCDMA_MGR_LOGOD_##type##_BMSK)) | ((value << MCALWCDMA_MGR_LOGOD_##type##_SHFT) \
  & MCALWCDMA_MGR_LOGOD_##type##_BMSK))
#endif

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/* External APIs */
/*=========================================================================== 

FUNCTION mcalwcdma_mgr_init

DESCRIPTION
  This function is the bootup time initialization of the state variables
  in this module.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void mcaltds_mgr_init(void);

/*===========================================================================

FUNCTION mcalwcdma_mgr_mcal_proc_cb

DESCRIPTION
  This function is the callback function as a response to the signal set in L1 
  TCB. It scans for all the modules in MCAL and if their bit is set in the module 
  mask, the callback function for that module is called which will submit all the 
  pending log packets in that module.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  mcalwcdma_submit_log_cmd_pending set to FALSE if TRUE.

===========================================================================*/

extern void mcaltds_mgr_mcal_proc_cb(void);


/*=========================================================================== 

FUNCTION mcalwcdma_mgr_post_download_mdsp

DESCRIPTION
  This is the post mDSP download function that waits until the mDSP is ready
  and then checks for mDSP version.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void mcaltds_mgr_post_download_mdsp(void);

#ifdef FEATURE_L1_LOG_ON_DEMAND
#if 0
/*=========================================================================== 

FUNCTION mcalwcdma_mgr_submit_logod_cmd

DESCRIPTION
  This function is the callback function of the log on demand for CCTrCh
  and TFC log packets. It requests the MCAL manager for task context in
  which the log packets can be submitted.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

log_on_demand_status_enum_type mcalwcdma_mgr_submit_logod_cmd(
  /* Log code for LOGOD */
  log_code_type log_code
);

/*=========================================================================== 

FUNCTION mcalwcdma_mgr_logod_init

DESCRIPTION
  This function registers the callback function for the CCTrCh log on
  demand log packet.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void mcalwcdma_mgr_logod_init(void);
#endif
/*=========================================================================== 

FUNCTION mcalwcdma_mgr_process_log_on_demand

DESCRIPTION
  This function is called when the task context is captured by the MCAL manager
  to log the on demand packets. This function checks for the log on demand 
  packets requested and services them serially.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void mcaltds_mgr_process_log_on_demand(void);
#endif


/* Internal APIs to MCAL */
/*===========================================================================

FUNCTION MCALWCDMA_POST_LOG_SUBMIT_CMD

DESCRIPTION
  This function is called by any MCAL module that needs a log packet to be 
  submitted. This function checks if the requestor MCAL module has a callback
  function registered and if so, it sets the global log request pending flag 
  and also sets a bit in the log submit request status mask corresponding to 
  the requestor module. It then sets a signal in L1 TCB 
  
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sets a signal in L1 TCB

===========================================================================*/

extern void mcaltds_mgr_post_log_submit_cmd(
  /* Module that wants to submit local command for logging */
  mcalwcdma_mgr_module_enum_type module);

/* APIs that are implemented by HOST task */
/*===========================================================================

FUNCTION tdsl1m_mcaltds_mgr_set_mcal_proc_req

DESCRIPTION
  This function sets the MCAL process signal in L1 TCB
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  signal set in L1 TCB

===========================================================================*/

extern void tdsl1m_mcaltds_mgr_set_mcal_proc_req(void);
#endif /* FEATURE_MCAL_TDSCDMA */
#endif /* MCALWCDMA_MGR_H */
#endif
