/*!
  @file
  cfcm.h

  @brief
  Centralized Flow-control Manager(CFCM) header file.

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

  @author
  rohitj

*/

/*=======================================Åõ====================================

  Copyright (c) 2023 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

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

==============================================================================*/

/*==============================================================================

$Header: //components/rel/mcs.mpss/5.2/api/cfcm.h#10 $

==============================================================================*/

#ifndef CFCM_H
#define CFCM_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include <msgr.h>
#include "cfcm_msg.h"
#include "cfcm_ext.h"
#include "sys.h"

#define CFCM_DEPRECATED_INTERFACE

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

#define CFCM_CONV_ENUM_TO_BIT_MASK(enum_t) (1 << enum_t)

#define CFCM_IS_CLIENT_VALID(client) ((client >= CFCM_CLIENT_LTE_UL) && (client < CFCM_CLIENT_MAX))



/*! @brief CPU based flow control step timer in ms
*/
#define CFCM_CPU_BASED_FC_STEP_TIMER                            200

/*! @brief BW based flow control step timer in ms
*/
#define CFCM_BW_BASED_FC_STEP_TIMER                             100



#define CFCM_MONITOR_MASK_THERMAL_PA               CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_THERMAL_PA)
#define CFCM_MONITOR_MASK_CPU                      CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_CPU)
#define CFCM_MONITOR_MASK_DSM_LARGE_POOL           CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_LARGE_POOL)
#define CFCM_MONITOR_MASK_DSM_DUP_POOL             CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_DUP_POOL)
#define CFCM_MONITOR_MASK_DSM_SMALL_POOL           CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_SMALL_POOL)
#define CFCM_MONITOR_MASK_BW_THROTTLING            CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_BW_THROTTLING)
#define CFCM_MONITOR_MASK_THERMAL_CX               CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_THERMAL_CX)
#define CFCM_MONITOR_MASK_MDM_TEMP                 CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_MDM_TEMP)
#define CFCM_MONITOR_MASK_VDD_PEAK_CURR_EST        CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_VDD_PEAK_CURR_EST)
#define CFCM_MONITOR_MASK_DSM_LARGE_POOL_LEVEL1    CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_LARGE_POOL_LEVEL1)
#define CFCM_MONITOR_MASK_DSM_SUPER_LARGE_POOL          CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_SUPER_LARGE_POOL)
#define CFCM_MONITOR_MASK_DSM_SUPER_LARGE_POOL_LEVEL1   CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_SUPER_LARGE_POOL_LEVEL1)
#define CFCM_MONITOR_MASK_SKIN_TEMP                     CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_SKIN_TEMP)
#define CFCM_MONITOR_MASK_WLAN_TEMP                     CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_WLAN_TEMP)
#define CFCM_MONITOR_MASK_WLAN_BW                       CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_WLAN_BW)
#define CFCM_MONITOR_MASK_DDR_REFRESH_LTE               CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DDR_REFRESH_LTE)
#define CFCM_MONITOR_MASK_DDR_REFRESH_GNSS              CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DDR_REFRESH_GNSS)

/*********************************************************************************************/
/*                                              TO BE DEPRECATED                             */
/*********************************************************************************************/
#ifdef CFCM_DEPRECATED_INTERFACE
#define CFCM_MONITOR_MASK_DSM_LARGE_POOL_UL        CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_LARGE_POOL_UL)
#define CFCM_MONITOR_MASK_DSM_LARGE_POOL_DL        CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_LARGE_POOL_DL)
#define CFCM_MONITOR_MASK_DSM_DUP_POOL_UL          CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_DUP_POOL_UL)
#define CFCM_MONITOR_MASK_DSM_DUP_POOL_DL          CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_DUP_POOL_DL)
#define CFCM_MONITOR_MASK_THERMAL_PA_EX            CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_THERMAL_PA_EX)
#define CFCM_MONITOR_MASK_THERMAL_PA_EM            CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_THERMAL_PA_EM)
#define CFCM_MONITOR_MASK_DSM_SMALL_POOL_DL        CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_SMALL_POOL_DL)
#define CFCM_MONITOR_MASK_DSM_LARGE_POOL_IPA       CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_LARGE_POOL_IPA_DL)
#define CFCM_MONITOR_MASK_THERMAL_RUNAWAY          CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_THERMAL_RUNAWAY)
#define CFCM_MONITOR_MASK_THERMAL_RUNAWAY_EX       CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_THERMAL_RUNAWAY_EX)
#define CFCM_MONITOR_MASK_MDM_TEMP_EX              CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_MDM_TEMP_EX)
#define CFCM_MONITOR_MASK_VDD_PEAK_CURR_EST_EX     CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_VDD_PEAK_CURR_EST_EX)
#define CFCM_MONITOR_MASK_DSM_LARGE_POOL_LEVEL1_DL CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_LARGE_POOL_LEVEL1_DL)
#define CFCM_MONITOR_MASK_DSM_DUP_POOL_LEVEL1_DL   CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_DSM_DUP_POOL_LEVEL1_DL)
#endif
/*********************************************************************************************/
/*                                              TO BE DEPRECATED                             */
/*********************************************************************************************/

/*! @brief CFCM aware monitors enumeration
    NOTE: KEEP THIS IN SYNC WITH THE MONITOR MASK ABOVE
    Any change in Enum will require change in cfcm_monitor_init_monitor()
*/
typedef enum
{
  CFCM_MONITOR_THERMAL_PA = 0,             /*!<  Power Amplifier temperature */
  CFCM_MONITOR_CPU,                        /*!<  Modem SWQ6 CPU Load monintor */
  CFCM_MONITOR_DSM_LARGE_POOL,             /*!<  DSM Large Pool monitor */
  CFCM_MONITOR_DSM_DUP_POOL,               /*!<  DSM Dup Pool monitor */
  CFCM_MONITOR_DSM_SMALL_POOL,             /*!<  DSM Small Pool monitor */
  CFCM_MONITOR_BW_THROTTLING,              /*!<  BW throttling to limit the BUS bandwidth*/
  CFCM_MONITOR_THERMAL_CX,                 /*!<  Thermal Runaway */
  CFCM_MONITOR_MDM_TEMP,                   /*!<  Thermal MDM Temp monitor */
  CFCM_MONITOR_VDD_PEAK_CURR_EST,          /*!<  VDD Peak Current Est monitor */
  CFCM_MONITOR_DSM_LARGE_POOL_LEVEL1,      /*!<  DSM Large Pool monitor if same client register two levels */
  CFCM_MONITOR_DSM_SUPER_LARGE_POOL,            /*!<  DSM Super Large Pool monitor */
  CFCM_MONITOR_DSM_SUPER_LARGE_POOL_LEVEL1,     /*!<  DSM Large Pool monitor if same client register two levels */
  CFCM_MONITOR_SKIN_TEMP,                       /*!<  Skin Temp */
  CFCM_MONITOR_WLAN_TEMP,                       /*!<  WLAN Temperature monitor */
  CFCM_MONITOR_WLAN_BW,                         /*!<  WLAN Bandwidth monitor */
  CFCM_MONITOR_DDR_REFRESH_LTE,                 /*!<  DDR refresh monitor for LTE */
  CFCM_MONITOR_DDR_REFRESH_GNSS,                /*!<  DDR refresh monitor for GNSS */
  CFCM_MONITOR_MAX,                             /*!<  MAX */
  /*********************************************************************************************/
  /*                                              TO BE DEPRECATED                             */
  /*********************************************************************************************/
#ifdef CFCM_DEPRECATED_INTERFACE
  CFCM_MONITOR_THERMAL_RUNAWAY,
  CFCM_MONITOR_THERMAL_RUNAWAY_EX,
  CFCM_MONITOR_MDM_TEMP_EX,
  CFCM_MONITOR_THERMAL_PA_EX,
  CFCM_MONITOR_THERMAL_PA_EM,
  CFCM_MONITOR_DSM_LARGE_POOL_UL,
  CFCM_MONITOR_DSM_LARGE_POOL_DL,
  CFCM_MONITOR_DSM_DUP_POOL_UL,  
  CFCM_MONITOR_DSM_DUP_POOL_DL,  
  CFCM_MONITOR_DSM_SMALL_POOL_DL,
  CFCM_MONITOR_DSM_LARGE_POOL_IPA_DL,
  CFCM_MONITOR_DSM_LARGE_POOL_LEVEL1_DL,
  CFCM_MONITOR_DSM_DUP_POOL_LEVEL1_DL,
  CFCM_MONITOR_VDD_PEAK_CURR_EST_EX,
#endif
  /*********************************************************************************************/
  /*                                              TO BE DEPRECATED                             */
  /*********************************************************************************************/

} cfcm_monitor_e;

#define CFCM_CLIENT_MASK_LTE_UL                      CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_LTE_UL)
#define CFCM_CLIENT_MASK_LTE_RLC_DL                  CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_LTE_RLC_DL)
#define CFCM_CLIENT_MASK_A2_UL_PER                   CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_A2_UL_PER)
#define CFCM_CLIENT_MASK_UL_PER                      CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_UL_PER)
#define CFCM_CLIENT_MASK_LTE_ML1                     CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_LTE_ML1)
#define CFCM_CLIENT_MASK_LTE_PDCP_DL                 CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_LTE_PDCP_DL)
#define CFCM_CLIENT_MASK_LTE_PDCP_COMP               CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_LTE_PDCP_COMP)
#define CFCM_CLIENT_MASK_CM                          CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_CM)
#define CFCM_CLIENT_MASK_LTE_ML1_CPU                 CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_LTE_ML1_CPU)
#define CFCM_CLIENT_MASK_HDR_DS                      CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_HDR_DS)
//#define CFCM_CLIENT_MASK_WCDMA_RLC                   CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_CLIENT_WCDMA_RLC)

/*! @brief CFCM aware clients enumeration
    NOTE: Any change in Enum will require change in DEFAULT THRESHOLDS --> cpu_thrshld_g
*/
typedef enum
{
  CFCM_CLIENT_LTE_UL = 0,                       /*!<  LTE UL */
  CFCM_CLIENT_LTE_RLC_DL,                       /*!<  LTE DL */
  CFCM_CLIENT_A2_UL_PER,                        /*!<  A2 UL PER */
  CFCM_CLIENT_UL_PER = CFCM_CLIENT_A2_UL_PER,   /*!<  IPA UL PER */
  CFCM_CLIENT_LTE_ML1,                          /*!<  LTE ML1 */
  CFCM_CLIENT_LTE_PDCP_DL,                      /*!<  LTE PDCP DL */
  CFCM_CLIENT_LTE_PDCP_COMP,                    /*!<  LTE PDCP Compression */
  CFCM_CLIENT_CM,                               /*!<  Call Manager */
  CFCM_CLIENT_LTE_ML1_CPU,                      /*!<  LTE ML1 CPU FC client */
  CFCM_CLIENT_HDR_DS,                           /*!<  Data Services FC client */
  CFCM_CLIENT_WCDMA_RLC_DL,                     /*!<  WCDMA RLC DL */
  CFCM_CLIENT_WCDMA_RLC_UL,                     /*!<  WCDMA RLC UL */
  CFCM_CLIENT_GSM,                              /*!<  GSM client */
  CFCM_CLIENT_TDSCDMA_RLC_DL,                   /*!<  TDSCDMA RLC DL */
  CFCM_CLIENT_TDSCDMA_RLC_UL,                   /*!<  TDSCDMA RLC DL */
  CFCM_CLIENT_WCDMA_L1,                         /*!<  WCDMA L1 client */
  CFCM_CLIENT_DS,                               /*!<  Data Services */
  CFCM_CLIENT_LTE_RLC_LEVEL1_DL,                /*!<  LTE DL LEVEL1 FC */
  CFCM_CLIENT_GPS_MC,                           /*!<  GPS_MC FC */
  CFCM_CLIENT_LTE_ML1_MDM,                      /*!<  LTE ML1 MDM FC */  
  CFCM_CLIENT_WLAN,                             /*!<  WLAN client */
  CFCM_CLIENT_GNSS,                             /*!<  GNSS client */
  CFCM_CLIENT_LTE_RRC,                          /*!<  LTE RRC client */
  CFCM_CLIENT_MAX                               /*!<  MAX */
} cfcm_client_e;

/*! @brief commands that can be issued by CFCM enumeration
*/
typedef enum
{
  CFCM_CMD_INVALID       = -1,    /*!<Invalid command */
  CFCM_CMD_FC_OFF        = 0,    /*!<turn off flow control */
  CFCM_CMD_UP            = 1,    /*!<go up one level, when step timer expires, 
                                    go up one level further unless hit top*/
  CFCM_CMD_FREEZE        = 3,    /*!<freeze: stay at the same fc state */
  CFCM_CMD_DOWN          = 7,    /*!<go down one level when step timer expires, 
                                    go down one level more unless hit bottom */
  CFCM_CMD_SET_MIN       = 15,   /*!<go down all the way to minimal level*/
  CFCM_CMD_SHUT_DOWN     = 31,   /*!<shut down completely */
  CFCM_CMD_SET_VALUE     = 63    /*!< Set Data rate directly */
} cfcm_cmd_e;

typedef enum
{
  CFCM_DSM_MEM_LEVEL_INVALID,         /*!<  DSM Mem Level Invalid */
  CFCM_DSM_MEM_LEVEL_MANY,            /*!<  DSM Mem Level MANY */
  CFCM_DSM_MEM_LEVEL_FEW,             /*!<  DSM Mem Level FEW */
  CFCM_DSM_MEM_LEVEL_DNE,             /*!<  DSM Mem Level DNE */
  CFCM_DSM_MEM_LEVEL_MAX              /*!<  MAX */
} cfcm_dsm_mem_level_e;


/*! @brief flow control command that is sent by CFCM
*/
typedef struct
{
  /*!< flow control command for this monitor */
  cfcm_cmd_e           cmd;

  /*!< Load: CPU Load percentage */
  uint32               load;

  /*!< step timer in ms */
  uint32               step_timer;
} cfcm_cpu_monitor_data_type_s;

typedef struct
{
  /*!< flow control command for this monitor */
  cfcm_cmd_e           cmd;

  /*!< Load: NPA Node data */
  uint32               state;

  /*!< step timer in ms */
  uint32               step_timer;
} cfcm_npa_node_data_type_s;

typedef struct
{
  /*!< flow control command for this monitor */
  cfcm_cmd_e           cmd;

  /*!< Level for this monitor */
  uint8           level;

  /*!< step timer in ms */
  uint32               step_timer;
} cfcm_bw_data_type_s;

typedef struct
{
  /*!< flow control command for this monitor */
  cfcm_cmd_e           cmd;

  /*!< operation that triggered the event */
  cfcm_dsm_mem_level_e     level;

  /*!< step timer in ms */
  uint32               step_timer;
} cfcm_dsm_data_type_s;

/*! @brief different level for each monitor type
*/
typedef union
{
 uint32                   cpu_load; 

 uint32                   npa_state;

 uint8                    bw_level;

 cfcm_dsm_mem_level_e     dsm_level;

}cfcm_monitor_data_level;

/*! @brief flow control command that is sent by CFCM
*/
typedef struct
{
  /* Thermal PA monitor Data */
  cfcm_npa_node_data_type_s     pa;

  /*!< Load: CPU Load percentage */
  cfcm_cpu_monitor_data_type_s  cpu;

  /* DSM monitor Data */
  cfcm_dsm_data_type_s          large_pool;

  /* DSM monitor Data */
  cfcm_dsm_data_type_s          dup_pool;

  /* DSM monitor Data */
  cfcm_dsm_data_type_s          small_pool;

  /* Bus Bandwidth monitor Data */
  cfcm_npa_node_data_type_s           bw;

  /* Thermal RunAway (CX) monitor Data */
  cfcm_npa_node_data_type_s     cx;

  /* Thermal MDM Temp monitor Data */
  cfcm_npa_node_data_type_s     mdm_temp;

  /* Thermal VDD Peak Curr monitor Data */
  cfcm_npa_node_data_type_s     vdd_peak_curr;

  /* DSM Large Pool Level1 monitor Data */
  cfcm_dsm_data_type_s          large_pool_level1;

  /* DSM monitor Data */
  cfcm_dsm_data_type_s          super_large_pool;

  /* DSM Super Large Pool Level1 monitor Data */
  cfcm_dsm_data_type_s          super_large_pool_level1;

  /* Skin Temp monitor Data */
  cfcm_npa_node_data_type_s     skin_temp;

  /* WLAN temp monitor Data */
  cfcm_npa_node_data_type_s     wlan_temp;

  /* WLAN bw monitor Data */
  cfcm_npa_node_data_type_s     wlan_bw;
  /* DDR refresh monitor Data */
  cfcm_npa_node_data_type_s     ddr_refresh_lte;

  /* DDR refresh monitor Data */
  cfcm_npa_node_data_type_s     ddr_refresh_gnss;
} cfcm_monitor_data_s;

/*! @brief flow control command that is sent by the CFCM
*/
typedef struct
{
  cfcm_client_e        client_id;     /*!< client ID, CFCM only works with known client */

  cfcm_monitor_e       monitor_id; /*Monitor which has triggered current CFCM Cmd*/

  uint32               monitors_mask; /*Monitors Bitmask, triggered for client*/

  cfcm_monitor_data_s   monitor_data; /*Monitor Data for triggered monitor*/

  uint32               data_rate; /*!< This will be valid only when CMD is CFCM_CMD_SET_VALUE */

  /*********************************************************************************************/
  /*                                              TO BE DEPRECATED                             */
  /*********************************************************************************************/
#ifdef CFCM_DEPRECATED_INTERFACE
  cfcm_cmd_e           cmd;        /*!< flow control command */

  uint32               step_timer; /*!< step timer in ms, shall be ignored unless 
                                        the cmd is up or down */
  uint32               monitors_active;  /*!<Bit mask of monitors which set command,
                                          e.g. CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_CPU)*/
#endif
  /*********************************************************************************************/
  /*                                              TO BE DEPRECATED                             */
  /*********************************************************************************************/

  cfcm_monitor_data_level  level; /*Monitor Level for triggered Monitor*/

} cfcm_cmd_type_s;


/*! @brief flow control request message format sent by CFCM
*/
typedef struct
{
  msgr_hdr_struct_type    hdr;    /*!< standard msgr header */
  cfcm_cmd_type_s         fc_cmd; /*!<  flow control cmd */
} cfcm_cmd_msg_type_s;

/*! @brief flow control callback for clients registered with 
     cfcm_client_reg_with_cb
*/
typedef void(*cfcm_fc_evt_cb_type)(cfcm_cmd_type_s *);


/*! @brief client registration indication message format
*/
typedef struct
{

  cfcm_client_e          client_id;    /*!<  client ID, CFCM only works with known client*/

  uint32                 monitor_mask; /*!<monitors bit mask, the client requested CFCM to monitor,
                                           e.g. CFCM_CONV_ENUM_TO_BIT_MASK(CFCM_MONITOR_CPU)*/

  msgr_umid_type         req_umid;     /*!< UMID for the command sent to the client */

  cfcm_fc_evt_cb_type    req_cb;       /*!< Callback  for the command sent to the client */

  sys_modem_as_id_e_type    variant_id;  /*!<variant for the command sent to the client */

} cfcm_reg_req_type_s;

typedef struct
{
  msgr_hdr_struct_type   hdr;          /*!< standard msgr header */

  cfcm_reg_req_type_s    cfcm_req;

} cfcm_reg_req_msg_type_s;


/*! @brief client deregistration indication message format
*/
typedef struct
{
  
  cfcm_client_e     client_id;    /*!<  client ID, CFCM only works with known client*/

  sys_modem_as_id_e_type    variant_id; /*!<variant for the command sent to the client */

} cfcm_dereg_req_type_s;

typedef struct
{
  msgr_hdr_struct_type   hdr;     /*!< standard msgr header */
  
  cfcm_dereg_req_type_s  client;  /*!<  client ID */

} cfcm_dereg_req_msg_type_s;


/*----------------------------------------------------------------------------
  CFCM Default data structure for Step Timers
----------------------------------------------------------------------------*/

typedef struct 
{
  /* CPU FC step timer in msecs */
  uint32  cpu_step_timer;

  /* DSM FC step timer in msecs */
  uint32  dsm_step_timer;

  /*! Thermal step timer in unit of seconds */
  uint8   thermal_step_timer;

  /*! Bus BW step timer in msecs */
  uint32   bw_step_timer;

}cfcm_default_step_timer_type_s;



/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*==============================================================================

  FUNCTION:  cfcm_task_is_active

==============================================================================*/
/*!
    @brief
    returns whether or not the CFCM task is running.

    @return
    TRUE if CFCM task is running; FALSE otherwise.
*/
/*============================================================================*/
extern boolean cfcm_task_is_active(void);

/*=======================================================================
FUNCTION CFCM_CLIENT_REGISTER

DESCRIPTION
  This function is externally exposed to allow registration of the client

DEPENDENCIES
  MSGR should be initialized

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/
void cfcm_client_register
( 
  cfcm_reg_req_type_s* client_req
);

/*=======================================================================
FUNCTION CFCM_CLIENT_DEREGISTER

DESCRIPTION
  This function is externally exposed to allow deregistration of the client

DEPENDENCIES
  MSGR should be initialized

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/
void cfcm_client_deregister
(
  cfcm_dereg_req_type_s*  client_req
);

/*=======================================================================
FUNCTION CFCM_CLIENT_REGISTER_VARIANT

DESCRIPTION
  This function is externally exposed to allow registration of the client

DEPENDENCIES
  MSGR should be initialized

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/
void cfcm_client_register_variant
( 
  cfcm_reg_req_type_s* client_req,
  sys_modem_as_id_e_type  variant_id

);

/*=======================================================================
FUNCTION CFCM_CLIENT_DEREGISTER_VARIANT

DESCRIPTION
  This function is externally exposed to allow deregistration of the client

DEPENDENCIES
  MSGR should be initialized

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/
void cfcm_client_deregister_variant
(
  cfcm_dereg_req_type_s*  client_req,
  sys_modem_as_id_e_type  variant_id

);

/*=======================================================================
FUNCTION CFCM_CLIENT_WLAN_REGISTER

DESCRIPTION
  This function is externally exposed to allow registration of wlan

DEPENDENCIES
  MSGR should be initialized

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/
void cfcm_client_wlan_register
(
   uint32 monitor_mask
);

/*=======================================================================
FUNCTION CFCM_CLIENT_WLAN_DEREGISTER

DESCRIPTION
  This function is externally exposed to allow deregistration of wlan

DEPENDENCIES
  MSGR should be initialized

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/
void cfcm_client_wlan_deregister(void);

/*=======================================================================
FUNCTION CFCM_CLIENT_GET_STEP_TIMER_VALS

DESCRIPTION
  This function is externally exposed for clients to get different step timer values

DEPENDENCIES
  CFCM should be initialized

RETURN VALUE
  TRUE/FALSE - Thresholds Valid or NOT
  cpu_step_timer - CPU FC step timer in msecs
  dsm_step_timer - DSM FC step timer in msecs
  thermal_step_timer - thermal step timer in unit of seconds

SIDE EFFECTS
  None
========================================================================*/
boolean cfcm_client_get_step_timer_vals
( 
  cfcm_client_e     client_id,    /*!<  client ID, CFCM only works with known client*/
  cfcm_default_step_timer_type_s* timers
);

/*==============================================================================

  FUNCTION:  cfcm_client_get_diag_test_enable_mask

==============================================================================*/
/*!
    @brief
    this function returns CFCM testing via Diag command enabled based on the EFS/NV read

    @return
    TRUE/FALSE
*/
/*============================================================================*/
boolean cfcm_client_get_diag_test_enable_mask( void );

/*==============================================================================

  FUNCTION:  cfcm_monitor_ddr_refresh_lte_event_cb

==============================================================================*/
/*!
    @brief
    API which core uses to set ddr refresh monitor level

    @return
    None
*/
/*============================================================================*/
EXTERN void cfcm_monitor_ddr_refresh_lte_event_cb
(  
  uint8  state      /*!< level */  
);

/*==============================================================================

  FUNCTION:  cfcm_monitor_ddr_refresh_gnss_event_cb

==============================================================================*/
/*!
    @brief
    API which core uses to set ddr refresh monitor level

    @return
    None
*/
/*============================================================================*/
EXTERN void cfcm_monitor_ddr_refresh_gnss_event_cb
(  
  uint8  state      /*!< level */  
);

/*=======================================================================
                         UNIT TEST
========================================================================*/
void cfcm_test_diag_command_inputs
( 
 uint8 input
);


#endif /* CFCM_H */
