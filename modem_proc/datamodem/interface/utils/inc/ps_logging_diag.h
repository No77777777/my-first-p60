#ifndef PS_LOGGING_DIAG_H
#define PS_LOGGING_DIAG_H
/*===========================================================================

                        P S _ L O G G I N G  _ D I A G . H

GENERAL DESCRIPTION
  The file defines diag sub system commands and regesters the request handler
  callback functions at powerup. This contains diag specific data. 

EXTERNALIZED FUNCTIONS
 dpl_init()
   Registers the functions(dpl request handlers) with DIAG.
   
 Copyright (c) 2004 by Qualcomm Technologies Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

 $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/inc/ps_logging_diag.h#2 $
 $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $
  
when        who     what, where, why
--------    ---     --------------------------------------------------------- 
11/30/11    su     Adding log code support for DPL logging
01/12/05    msr    Added code review comments.
01/10/05    msr    Added support for flow and link logging.
11/2/04    ks/msr   Changed Diag cmd handler code's names.
09/27/04   ks/msr   Major cleanup.
07/29/04    vd      Created the file. 
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "customer.h"
#include "ps_logging_defs.h"
#include "diagcmd.h"
#include "diagpkt.h"
#include "log.h"
#include "log_codes.h"


#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 
                      DIAG REQUEST / RESPONSE STRUCTURES 

===========================================================================*/
/*---------------------------------------------------------------------------
  NOTE : DO NOT compile in these structures. These are for informational
  purposes only.
---------------------------------------------------------------------------*/ 
/*---------------------------------------------------------------------------
struct dpl_reset_logging_req
{
  uint8   command_code;
  uint8   subsys_id;
  uint16  subsys_command_code;
}

struct dpl_reset_logging_resp
{
  uint8   command_code;
  uint8   subsys_id;
  uint16  subsys_command_code;
}

struct dpl_get_supported_ifaces_req
{
  uint8   command_code;
  uint8   subsys_id;
  uint16  subsys_command_code;
}

struct dpl_get_supported_ifaces_resp
{
  uint8   command_code;
  uint8   subsys_id;
  uint16  subsys_command_code;
  uint8   num_ifaces;
  uint8   iface_id[num_ifaces];
}

struct dpl_get_iface_desc_req
{
  uint8   command_code;
  uint8   subsys_id;
  uint16  subsys_command_code;
  uint8   iface_id;
}

struct dpl_get_iface_desc_resp
{
  uint8     command_code;
  uint8     subsys_id;
  uint16    subsys_command_code;
  uint8     iface_id;
  char    * iface_desc; // Can at most be 32 bytes and is null termnated
  uint8     num_links;
  char    * link_desc[num_links]; // Can at most be 32 bytes and is null
                                  // termnated
  uint8     num_flows;
  char    * flow_desc[num_flows]; // Can at most be 32 bytes and is null
                                  // termnated
}

struct dpl_set_iface_logging_req
{
  uint8   command_code;
  uint8   subsys_id;
  uint16  subsys_command_code;
  uint8   iface_id;
  uint8   num_log_flags;

  struct
  {
    uint32  iid;
    uint32  snaplen;
  } log_flag[num_log_flags];
}

struct dpl_set_iface_logging_resp
{
  uint8   command_code;
  uint8   subsys_id;
  uint16  subsys_command_code;
  uint8   iface_id;
  uint8   num_log_flags;

  struct
  {
    uint32  iid;
    uint32  link_type;
  } log_flag[num_log_flags];
}

struct dpl_get_supported_protocols_req
{
  uint8   command_code;
  uint8   subsys_id;
  uint16  subsys_command_code;
}

struct dpl_get_supported_protocols_resp
{
  uint8   command_code;
  uint8   subsys_id;
  uint16  subsys_command_code;
  uintt8  num_nw_protocols; // Number of supported network level protocols
  uint8   protocol[num_nw_protocols];
}
---------------------------------------------------------------------------*/


/*===========================================================================
 
                                DIAG_SUB_CMDS

===========================================================================*/

#define     PS_LOGGING_CMD_MAX_SUPPORTED_ARG    0x10

/*---------------------------------------------------------------------------
  The sub-system id's specific commands we handle.  There is a 
  higher-level command defined in DIAG for DIAG_SUBSYS_PS

     Diag command- send_data 75 <PS ID> <Command ID (two bytes)> <Data byte 1> <Data byte 2> <Data byte 3>
     Example command: send_data 75 42 03 00 01 01 01
     PS diag ID - 42
     Command ID - 03 00
     Data Byte 1(Argument 1)  - 01
     Data Byte 2(Argument 2)  - 01 
     Data Byte 3(Argument 3)  - 01
---------------------------------------------------------------------------*/
typedef enum
{
  PS_LOGGING_DIAG_MIN                     = 1,  
  PS_LOGGING_DIAG_DATA_PATH               = PS_LOGGING_DIAG_MIN,  
  /* e.g send_data 75 42 01 00 01 01 01 */
  PS_LOGGING_DIAG_DS_MUX                  = 2,
  /*e.g send_data 75 42 02 00 01 01 01 */
  PS_LOGGING_DIAG_EXT_DEBUG_LOGGING       = 3,
  /* e.g  send_data 75 42 03 00 01 01 01 */
  PS_LOGGING_DIAG_MAX                     = 4
} ps_logging_diag_ssid_cmd_type;

/*---------------------------------------------------------------------------
  Define  a internal lookup table for QXDM commands
  Below mention table can be extended based on requirements.
---------------------------------------------------------------------------*/
typedef  struct
{
  uint8                       arg_list[PS_LOGGING_CMD_MAX_SUPPORTED_ARG]; 
}ps_logging_diag_input_type;

typedef void (*ps_logging_diag_command_notify_cb_fcn)
(
  uint16                      cmd_id,
  ps_logging_diag_input_type  input_params
);

typedef struct 
{
  uint32                                     client_id;
  ps_logging_diag_command_notify_cb_fcn      call_back;
} ps_logging_diag_internal_lookup_info;


/*===========================================================================
 
                                  EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION    PS_LOGGING_DIAG_INIT

DESCRIPTION
  Initialize data protocol logging and register with diag for QXDM commands

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_logging_diag_init
(
  void
);

/*===========================================================================
FUNCTION      DPL_INIT

DESCRIPTION
  Registers functions with DIAG.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void dpl_init
(
  void
);

/*===========================================================================
FUNCTION      PS-DIAG_CMD_INIT

DESCRIPTION   This function initializes the ps diag command module and registers 
              ps diag cmds with the diag framework.
 
DEPENDENCIES  None 

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ps_diag_cmd_init(void);

/*===========================================================================
FUNCTION DPL_GET_DPL_STATE
 
DESCRIPTION
  This method will return current DPL state
   
RETURN VALUE
     None
 
DEPENDENCIES
  	 None 
  
SIDE EFFECTS
    None
===========================================================================*/
dpl_state_enum_type dpl_get_dpl_state
(
  void
);

/*===========================================================================
FUNCTION dpli_update_logging_config

DESCRIPTION
  This method enables the logging in iface, sets protocol and packet
  log length
 
PARAMETERS
  ps_iface_ptr[in]   Pointer to the interface 
   
RETURN VALUE
    None
 
DEPENDENCIES
    None 
  
SIDE EFFECTS
    None
===========================================================================*/
 void dpli_update_logging_config
(
  ps_iface_type       *ps_iface_ptr,
  dpl_state_enum_type  dpl_state
);

/*===========================================================================
FUNCTION ps_logging_diag_register_cmd_cb

DESCRIPTION
  PS internal client can register for QXDM command with ps logging diag framework
 
PARAMETERS
  client_id[in]   Client ID 
  notify_cb[in]   Notify call back
RETURN VALUE
    None
 
DEPENDENCIES
    None 
  
SIDE EFFECTS
    None
===========================================================================*/
 void ps_logging_diag_register_cmd_cb
(
  uint32                                     client_id,
  ps_logging_diag_command_notify_cb_fcn      notify_cb

);


#ifdef __cplusplus
}
#endif

#endif /* PS_LOGGING_DIAG_H */
