#ifndef RFDEVICE_ASM_TYPES
#define RFDEVICE_ASM_TYPES

/*!
  @file
  rfdevice_asm_types.h

  @brief
  This file contains the type definitions required by ASM driver.

*/

/*===========================================================================

Copyright (c) 2013-25 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_asm.mpss/1.32/api/rfdevice_asm_types.h#2 $

when       who     what, where, why
-------------------------------------------------------------------------------
07/23/25   sr      Adding RFDEVICE_ASM_CTRIG_DISABLE_DATA to fix compilation
12/17/24   sr      Added ag_script_present to check if the ag scripts are present,if disable scripts are absent,we ignore the case
07/05/17   dh      Added RFDEVICE_ASM_LPM_WAKEUP_DATA and RFDEVICE_ASM_LPM_SLEEP_DATA
06/29/17   dh      Updated asm_data enum with RFDEVICE_ASM_WAKEUP_DATA
06/02/15   sn      Added CA split flag for scc/pcc disable
08/05/14   cc      Common Setting Script support 
05/05/14   sn      ASM class based API
01/11/14   vv      Added an enum to support init ag settings on certain ASMs
02/19/13   sr      Split the autogen with on/off/trigger scripts.
01/28/13   sr      init version
============================================================================*/
#include "comdef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RFDEVICE_ASM_REG_INVALID -1
#define RFDEVICE_ASM_DEFAULT_MASK 0xFF

  typedef enum
  {
    RFDEVICE_ASM_RX,
    RFDEVICE_ASM_TX,
    RFDEVICE_ASM_RXTX_MAX,
    RFDEVICE_ASM_RXTX_INVALID = RFDEVICE_ASM_RXTX_MAX,
  }
  rfdevice_asm_rxtx_enum_type;

  typedef enum
  {
    RFDEVICE_ASM_ON_DATA,
    RFDEVICE_ASM_OFF_DATA,
    RFDEVICE_INIT_DATA,
    RFDEVICE_ASM_TRIGGER_DATA,
    RFDEVICE_ASM_RESET_DATA,
    RFDEVICE_ASM_THERM_READ_DATA,
    RFDEVICE_ASM_THERM_ON_DATA,
    RFDEVICE_ASM_THERM_OFF_DATA,
    RFDEVICE_ASM_WAKEUP_DATA,
    RFDEVICE_ASM_SLEEP_DATA,
	RFDEVICE_ASM_LPM_WAKEUP_DATA,
    RFDEVICE_ASM_LPM_SLEEP_DATA,
    RFDEVICE_ASM_CTRIG_DISABLE_DATA,
    RFDEVICE_ASM_DATA_REQ_INVALID
  } rfdevice_asm_data_req_enum_type;

  typedef struct
  {
    int32 mfg_id;
    int32 prd_id;
    int32 prd_rev;
    int32 num_ports;
    boolean separate_regs_for_ca;
  } rfdevice_asm_info_type;

  typedef struct
  {
    boolean ag_script_present;
    rfdevice_asm_data_req_enum_type req;
    uint8 port;
  } rfdevice_asm_cfg_params_type;


  typedef struct
  {
    uint8 num_regs;
    uint8 *addr;
    int16 *data;
  } rfdevice_asm_reg_settings_type;

  typedef enum
  {
    RFDEVICE_ASM_CMD_WRITE,
    RFDEVICE_ASM_CMD_WRITE_EXT,
    RFDEVICE_ASM_CMD_DELAY,
    RFDEVICE_ASM_CMD_READ,
    RFDEVICE_ASM_CMD_INVALID
  } rfdevice_asm_cmd_enum_type;

  typedef struct
  {
    rfdevice_asm_cmd_enum_type cmd;
    uint8 addr;
    uint8 data;
	uint8 mask;
  } rfdevice_asm_cmd_struct_type;

  typedef struct
  {
    uint8 num_cmds;
    rfdevice_asm_cmd_struct_type *cmds;
  } rfdevice_asm_cmd_seq_type;


#ifdef __cplusplus
}
#endif

#endif


