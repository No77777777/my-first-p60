#ifndef __MPOWER_QSH_EXT_H__
#define __MPOWER_QSH_EXT_H__

/*=========================================================================

           M O D E M   C L O C K   A N D   P O W E R   M A N A G E R 
               
                Q S H   E X T E R N A L   H E A D E R   F I L E


  GENERAL DESCRIPTION
    This file contains QSH external interface definitions for Modem Clock and
    Power Manager (MCPM).
  
  INITIALIZATION AND SEQUENCING REQUIREMENTS
    Invoke the MCPM_Init function to initialize the MCPM.
  
        Copyright (c) 2021 by QUALCOMM Technologies, Inc.  All Rights Reserved.


==========================================================================*/

/*==========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
 
$Header: //components/rel/mpower.mpss/13.1/api/mpower_qsh_ext.h#7 $

when       who     what, where, why 
--------   ---     ---------------------------------------------------------
03/06/15   ls      PACKed crash dump structures to avoid alignment issues.
02/26/15   ls      Removed internal enums and data types. 
02/02/15   ls      Initial version.

==========================================================================*/ 


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

/*
 * Includes outside of the local directory.
 */
#include <qsh.h>
#include <mcpm_drv.h>
#include <qsh_mdump.h>


/*=========================================================================
      Constants and Macros
==========================================================================*/

#define MPOWER_QSH_MAJOR_VER                                         1
#define MPOWER_QSH_MINOR_VER                                         0

#define MCPM_DBG_TRACE_SIZE                                         10
#define MCPM_DBG_TECH_NUM                                            5

/*
 * MCPM_DBG_NUM_MCVS_CLIENTS: number of MCVS clients (non-CFG_MODEM client),
 *                            derived from MCPM_NUM_RESRC_CLIENTS.
 * 
 * MCPM_DBG_NUM_RESRC:        number of resources, derived from MCPM_NUM_RESRC.
 * 
 * MCPM_DBG_NUM_MCVS_RESRC:   number of MCVS resources, derived from:
 *                            MCPM_NUM_MCVS_RESRC.
 * 
 * IMPORTANT: whenever we update the original macros, we need to update it here. 
 * 
 */
#define MCPM_DBG_NUM_MCVS_CLIENTS                                  MCPM_NUM_RESRC_CLIENTS-1
#define MCPM_DBG_NUM_RESRC                                         MCPM_NUM_RESRC
#define MCPM_DBG_NUM_MCVS_RESRC                                    MCPM_NUM_MCVS_RESRC

/*=========================================================================
      Typedefs
==========================================================================*/

/*
 * MCPM_Trace_Event_DbgType 
 *  
 * Lite version of mcpm_trace_event_type for debug. 
 */
typedef PACK(struct)
{
  /* MCPM_Trace_EventsType */
  uint32                 trace_type;

  /* mcpm_request_type */
  uint8                  request_type;

  uint64                 ts;
} MCPM_Trace_Event_DbgType;

/*
 * MCPM_DbgType 
 *  
 * Lite version of mcpm_type for debug.
 */
typedef PACK(struct)
{
  /* Holds the current state, mcpm_state_type */
  uint8                               state;
  uint8                               prev_state;

  /* MCPM tech cfg states, MCPM_Tech_CFG_StateType */
  uint8                               tech_cfg_state;
  uint8                               prev_tech_cfg_state;

  /* Tech's active config index obtained from the system spreadsheet, MCPM_Sys_Cfg_IDType */
  uint32                              sys_cfg_id;
  uint32                              prev_sys_cfg_id;

  /* Tech's active request, mcpm_request_type */
  uint8                               eRequest;
} MCPM_DbgType;

/*
 * MCPM_RM_Resrc_Client_DbgType 
 *  
 * Lite version of MCPM_RM_Resrc_Client_Type for debug.
 */
typedef PACK(struct)
{
  uint32                          CurrState[MCPM_DBG_NUM_MCVS_RESRC];
} MCPM_RM_Resrc_Client_DbgType;

/*
 * MCPM_RM_Resrc_Tech_Info_DbgType 
 *  
 * Lite version of MCPM_RM_Resrc_Tech_Info_Type for debug.
 */
typedef PACK(struct)
{
  MCPM_RM_Resrc_Client_DbgType    ClientState[MCPM_DBG_NUM_MCVS_CLIENTS];
  uint32                          CurrState[MCPM_DBG_NUM_RESRC];
} MCPM_RM_Resrc_Tech_Info_DbgType;

/*
 * MCPM_RM_Resrc_InfoDbgType
 *
 * Resource information
 * 
 * State       - Resource state.
 */
typedef PACK(struct)
{
  uint32                          State[MCPM_DBG_NUM_RESRC];
} MCPM_RM_Resrc_Info_DbgType;


/*
 * MCPM_Dbg_DataType 
 *  
 * MCPM Debug Data type.
 */
typedef PACK(struct)
{
  uint64                             mcpm_version;

  /* MCPM_FW_PC_status */
  uint8                              fw_status;

  /* mcpm_tech_type */
  uint8                              TechID[MCPM_DBG_TECH_NUM];

  MCPM_Trace_Event_DbgType           mcpm_trace[MCPM_DBG_TRACE_SIZE];
  MCPM_DbgType                       mcpm[MCPM_DBG_TECH_NUM];
  MCPM_RM_Resrc_Tech_Info_DbgType    aMCPMTechResrcInfo[MCPM_DBG_TECH_NUM];
  MCPM_RM_Resrc_Info_DbgType         aMCPMResrcLastCommittedInfo;
  MCPM_RM_Resrc_Info_DbgType         aMCPMResrcMuxInfo;
} MCPM_Dbg_DataType;


/*
 * mpower_qsh_metric_e 
 *  
 * mpower qsh metric enum defintion.
 */
typedef enum
{
  MPOWER_QSH_METRIC_DEFAULT = 0,
  MPOWER_QSH_METRIC_MAX
} mpower_qsh_metric_e;


/*
 * mpower_qsh_dump_tag_e 
 *  
 * mpower qsh dump tag defintion.
 */
typedef enum
{
  MPOWER_QSH_DUMP_TAG_DEFAULT = 0,
  MPOWER_QSH_DUMP_TAG_MAX = 31
} mpower_qsh_dump_tag_e;


/*
 * MCPM_QSH_Metric_EventType
 *  
 * MCPM internal event defintion to identify event in the metric.
 */
typedef enum
{
  MCPM_QSH_METRIC_REQUEST,      /* indicates we received a new request from L1      */
  MCPM_QSH_METRIC_CONFIG_ID,    /* indicates a new config ID found for the request  */
  MCPM_QSH_METRIC_CLK           /* indicates we committed a clock resource          */
} MCPM_QSH_Metric_EventType;


/*
 * MCPM_Dbg_Runtime_Metric_DataType
 *  
 * MCPM metric data structure.
 */
typedef PACK(struct) {
  uint8                      eTech;      /* indicates the tech, mcpm_tech_type       */
  MCPM_QSH_Metric_EventType  eEventType; /* indicates the event type                 */
  uint32                     nData1;     /* custom field for event data              */
  uint32                     nData2;     /* custom field for event data              */
} MCPM_Metric_DataType;


typedef PACK(struct) {

  /* this header will have a tag and length */
  qsh_dump_tag_hdr_s hdr;

  MCPM_Dbg_DataType  tag_value;

} mpower_qsh_dump_tag_debug_dump_s;  


typedef struct 
{
  /* This header has the timestamp (uint16) */
  qsh_metric_hdr_s hdr; 

  MCPM_Metric_DataType tag_value;

} mpower_qsh_metric_data_s;



/*==========================================================================
               FUNCTION DECLARATIONS FOR MODULE
==========================================================================*/


#endif /* __MPOWER_QSH_EXT_H__ */
