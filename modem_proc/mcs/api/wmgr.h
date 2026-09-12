#ifndef WMGR_H
#define WMGR_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=

                    W A K E U P    M A N A G E R

                     Wakeup Manager Header File

GENERAL DESCRIPTION

  This file provides the interface for Layer1s to inteact with wakeup manager
  Wakeup Manager provides the information about the next wakeup to Layer1s.


EXTERNALIZED FUNCTIONS

  wmgr_register_rat_cb() - Layer1s register the wakeup call back using this API.
    Wakeup manager will use the call back registered in this API to inform
    the layer1s about the status of the next wakeups.


REGIONAL FUNCTIONS

  None


INITIALIZATION AND SEQUENCING REQUIREMENTS

  Wakeup manager is functionally dependent on TRM for all its data and 
  decission making engine. TRM should be initialized before wakeup manager
  is functional.The registration of call back with wakeup manager can happen 
  independent of TRM. 
  

  Copyright (c) 2013-2015 Qualcomm Technologies Incorporated.
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

=*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=



==============================================================================

                           EDIT HISTORY FOR MODULE

$Header: //components/rel/mcs.mpss/5.2/api/wmgr.h#1 $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ---------------------------------------------------------
05/05/2016   md      LTED clients for wakeupmanager
08/26/2015   ag      Added new interface accepting pointer to provide status info 
05/01/2015   jm      RFFE Bus Contention Solution (coex & wmgr) (CR: 799141).
12/11/2014   mn      LTE EMBMS functional changes (CR: 769538).
03/03/2013   sr      SLTE Changes.
05/10/2013   mn      Adding support for TSTS.
03/26/2013   mn      Adding support for Wakeup Manager on Triton DSDS.
08/10/2012   sr      Adding new wmgr clients for Dime.
03/05/2012   sr      Add Support for TDS-CDMA in WMGR. 
07/01/2011   ag      Added api to query future chain assignment for a client.  
05/23/2011   sg      Initial Revision

============================================================================*/


/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include <comdef.h>
#include "rfm.h"             /* For rfm_device_enum_type */
#include <timetick.h>

/*============================================================================

                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

============================================================================*/



/*----------------------------------------------------------------------------
  Wakeup Manager Clients
    - These enums indicate the clients that Wakeup Manager supports.
    - These enums are placed in the order of the bit position in the 
      L1 transition table
    - The order used here is same as the order of clients in TRM so as
      to help in the TRM - WMgr interaction

      Note: Always add to the end to preserve compatibility with Layer1
            transition table. The Max Clients that can be supported are 16.
            Max Client = (WMGR_CLIENT_MAX+1)/2 
            This is because each client needs 2bits. Bit X and Bit (31-X)

----------------------------------------------------------------------------*/
typedef enum
{
  WMGR_CLIENT_1X,
  WMGR_CLIENT_1X_SECONDARY,
  WMGR_CLIENT_HDR, 
  WMGR_CLIENT_HDR_SECONDARY,
  WMGR_CLIENT_GPS,
  WMGR_CLIENT_UMTS,
  WMGR_CLIENT_UMTS_SECONDARY,
  WMGR_CLIENT_GSM1,
  WMGR_CLIENT_GSM2,
  WMGR_CLIENT_WCDMA,
  WMGR_CLIENT_TDSCDMA,
  WMGR_CLIENT_TDSCDMA_SECONDARY,
  WMGR_CLIENT_UMTS_CA,
  WMGR_CLIENT_UMTS_CA_SECONDARY,
  WMGR_CLIENT_LTE,
  WMGR_CLIENT_LTE_SECONDARY,
  WMGR_CLIENT_LTE_SUB2,
  WMGR_CLIENT_LTE_SUB2_SECONDARY,
  WMGR_CLIENT_CM,
  WMGR_CLIENT_GSM3,
  WMGR_CLIENT_GSM_SECONDARY, 
  WMGR_CLIENT_RF, 
  WMGR_CLIENT_IRAT,
  WMGR_CLIENT_WLAN,
  WMGR_CLIENT_GPRS1,
  WMGR_CLIENT_GPRS2,
  WMGR_CLIENT_GSM1_SECONDARY,
  WMGR_CLIENT_GSM2_SECONDARY,
  WMGR_CLIENT_GSM3_SECONDARY,
  WMGR_CLIENT_IRAT2,
  WMGR_CLIENT_UMTS2,
  WMGR_CLIENT_UMTS2_SECONDARY,

    /* new clients for tx */
  WMGR_CLIENT_1X_TX, 
  WMGR_CLIENT_HDR_TX,
  WMGR_CLIENT_UMTS_TX,
  WMGR_CLIENT_UMTS2_TX,
  WMGR_CLIENT_UMTS_CA_TX,
  WMGR_CLIENT_GSM1_TX,
  WMGR_CLIENT_GSM2_TX,
  WMGR_CLIENT_TDSCDMA_TX,
  WMGR_CLIENT_LTE_TX,
  WMGR_CLIENT_LTE_SUB2_TX,
  WMGR_CLIENT_GPRS1_TX,
  WMGR_CLIENT_GPRS2_TX,
  WMGR_CLIENT_IRAT_TX,
  WMGR_CLIENT_IRAT2_TX,
  WMGR_CLIENT_WLAN_SECONDARY,
  WMGR_CLIENT_WLAN_5G,
  WMGR_CLIENT_WLAN_5G_SECONDARY,
  WMGR_CLIENT_LTE_CA = 61,
  WMGR_CLIENT_LTE_CA_SECONDARY,
  WMGR_CLIENT_LTE_CA_TX,
  WMGR_CLIENT_LTE_CA1,     
  WMGR_CLIENT_LTE_CA1_SECONDARY,
  WMGR_CLIENT_LTE_CA1_TX,
  WMGR_CLIENT_LTE_CA2,     
  WMGR_CLIENT_LTE_CA2_SECONDARY,
  WMGR_CLIENT_LTE_CA2_TX,
  WMGR_CLIENT_LTE_HO_SECONDARY1,
  WMGR_CLIENT_LTE_HO_SECONDARY2,
  WMGR_CLIENT_LTE_CA_4_RX0,
  WMGR_CLIENT_LTE_CA_4_RX1,
  WMGR_CLIENT_LTE_CA1_4_RX0,
  WMGR_CLIENT_LTE_CA1_4_RX1,
  WMGR_CLIENT_LTE_CA2_4_RX0,
  WMGR_CLIENT_LTE_CA2_4_RX1,
  WMGR_CLIENT_LTE_EMBMS1,
  WMGR_CLIENT_LTE_EMBMS2,
  WMGR_CLIENT_LTED,
  WMGR_CLIENT_LTED_SECONDARY,
  WMGR_CLIENT_LTED_TX,
  WMGR_CLIENT_LTE_CGI,
  WMGR_CLIENT_LTE_CGI_SECONDARY,
  /*L sub 2 support*/
  WMGR_CLIENT_LTE_SUB2_CA,
  WMGR_CLIENT_LTE_SUB2_CA_SECONDARY,
  WMGR_CLIENT_LTE_SUB2_CA_TX,
 
  WMGR_CLIENT_LTE_SUB2_CA1,     
  WMGR_CLIENT_LTE_SUB2_CA1_SECONDARY,
  WMGR_CLIENT_LTE_SUB2_CA1_TX,
     
  WMGR_CLIENT_LTE_SUB2_CA2,     
  WMGR_CLIENT_LTE_SUB2_CA2_SECONDARY,
  WMGR_CLIENT_LTE_SUB2_CA2_TX,
     
  WMGR_CLIENT_LTE_SUB2_HO_SECONDARY1,
  WMGR_CLIENT_LTE_SUB2_HO_SECONDARY2,
  WMGR_CLIENT_LTE_SUB2_CA_4_RX0,
  WMGR_CLIENT_LTE_SUB2_CA_4_RX1,
  WMGR_CLIENT_LTE_SUB2_CA1_4_RX0,
  WMGR_CLIENT_LTE_SUB2_CA1_4_RX1,
  WMGR_CLIENT_LTE_SUB2_CA2_4_RX0,
  WMGR_CLIENT_LTE_SUB2_CA2_4_RX1,
  
  WMGR_CLIENT_LTE_SUB2_EMBMS1,
  WMGR_CLIENT_LTE_SUB2_EMBMS2,
  
  WMGR_CLIENT_LTED_SUB2,
  WMGR_CLIENT_LTED_SUB2_SECONDARY,
  WMGR_CLIENT_LTED_SUB2_TX,
  
  WMGR_CLIENT_LTE_SUB2_CGI,
  WMGR_CLIENT_LTE_SUB2_CGI_SECONDARY,
  
  WMGR_CLIENT_LTE_CA3,
  WMGR_CLIENT_LTE_CA3_SECONDARY,
  WMGR_CLIENT_LTE_CA3_TX,
  WMGR_CLIENT_LTE_CA3_4_RX0,
  WMGR_CLIENT_LTE_CA3_4_RX1,
  
  WMGR_CLIENT_LTE_SUB2_CA3,
  WMGR_CLIENT_LTE_SUB2_CA3_SECONDARY,
  WMGR_CLIENT_LTE_SUB2_CA3_TX,
  WMGR_CLIENT_LTE_SUB2_CA3_4_RX0,
  WMGR_CLIENT_LTE_SUB2_CA3_4_RX1,
  
  WMGR_CLIENT_NO_CLIENT,
  WMGR_CLIENT_LAST = WMGR_CLIENT_NO_CLIENT,
  WMGR_CLIENT_MAX,

} wmgr_client_enum_t;

/*----------------------------------------------------------------------------
  Wakeup Manager Status Info Clients
  - This enum holds the list of all the clients that could be interested in
    the wakeup information and the current chain owner information.
  - These clients can register with Wakeup Manager for call backs and it will
     indicate the clients with the wakeup information
  - Add new members to the end of the list, but before WMGR_STATUS_INFO_CLIENT_LAST.
----------------------------------------------------------------------------*/
typedef enum
{
  /* Wakeup Manager MCPM Status Client */
  WMGR_STATUS_INFO_CLIENT_MCPM,

  /* Wakeup Manager LTE Status Client */
  WMGR_STATUS_INFO_CLIENT_LTE,
  
  /* Wakeup Manager 1X Status Client */
  WMGR_STATUS_INFO_CLIENT_1X,
  
  /* Wakeup Manager HDR Status Client */
  WMGR_STATUS_INFO_CLIENT_HDR,

  /* Wakeup Manager GSM Status Client */
  WMGR_STATUS_INFO_CLIENT_GSM,

  /* Max client - Used for reference */
  WMGR_STATUS_INFO_CLIENT_MAX

} wmgr_status_info_client_enum_t;

/*----------------------------------------------------------------------------
  Macros of Layer1 transitions
    - These Macros are used by Wakeup Manager to encode the information
    - These Macros can be used by L1 to decode the information

                   16 BITS OF LAYER1 FROM TRANSITIONS
  |---------------------------------------------------------------|
  |31 |30 |29 |28 |27 |26 |25 |24 |23 |22 |21 |20 |19 |18 |17 |16 |                 
  |---------------------------------------------------------------|
  |1X |1XS| H | HS|GPS| U |US |G1 |G2 | W |res|res|res|res|res|res|
  |---------------------------------------------------------------|
  |<--------------------------- FROM ---------------------------->|
 
                   16 BITS OF LAYER1 TO TRANSITIONS
  |---------------------------------------------------------------|
  |15 |14 |13 |12 |11 |10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |                 
  |---------------------------------------------------------------|
  |res|res|res|res|res|res| W | G2| G1| US| U |GPS| HS| H |1XS| 1X|
  |---------------------------------------------------------------|
  |<----------------------------- TO ---------------------------->|
  
----------------------------------------------------------------------------*/

/* Utility MACRO to Set FROM RAT */
#define WMGR_SET_FROM_RAT(bitmask, rat) \
        (bitmask) | (1 << (WMGR_CLIENT_MAX - (rat)))

/* Utility MACRO to Check  whether FROM RAT is set */
#define WMGR_IS_SET_FROM_RAT(bitmask, rat) \
        (bitmask) & (1 << (WMGR_CLIENT_MAX - (rat)))

/* Utility MACRO to Set TO RAT */
#define WMGR_SET_TO_RAT(bitmask, rat) \
        (bitmask) | (1 << (rat))

/* Utility MACRO to Check if TO RAT is set */
#define WMGR_IS_SET_TO_RAT(bitmask, rat) \
        (bitmask) & (1 << (rat))

/* Utilized in owner info to determine RF ownership duration */
#define WMGR_TIME_INDEFINITE (TRM_MAX_DURATION)

/*----------------------------------------------------------------------------
  Wakeup Manager Result Events
    - These events indicate Layer1s about the status of next wakeup

----------------------------------------------------------------------------*/
typedef enum
{
  /* Both MDSP dload and RF warmup required */
  WMGR_RESULT_MDSP_DLOAD_RF_WARMUP_REQ,

  /* No MDSP dload but RF warmup required */
  WMGR_RESULT_RF_WARMUP_REQ,

  /* Neither MDSP dload not RF warmup is required */
  WMGR_RESULT_MDSP_DLOAD_RF_WARMUP_NOT_REQ,

  /* TRM collision so do not wakeup this slot */
  WMGR_RESULT_WAKEUP_CANCEL,

  /* Error */
  WMGR_RESULT_ERROR,

  /* Used for arrays */
  WMGR_RESULT_MAX
  
}wmgr_result_type;

/*----------------------------------------------------------------------------
  Wakeup Manager Wakeup Info Structure
    - This structure holds the information for each which made a reservation.
    - Gives information on whether the wakeup will be successful or not.
----------------------------------------------------------------------------*/
typedef struct 
{
  /* Provides who is currently owning a RF device */
  wmgr_client_enum_t       client_id;
  
  /* Denotes the device which was under contention.
     The value is valid only if collision_type is WMGR_NO_COLLISION.
     The value is RFM_MAX_DEVICES otherwise. */
  rfm_device_enum_type     device;
  
  /* Wake up time Information from current time */
  timetick_type            wakeup_time;

  /* Absolute wakeup time */
  uint64                   wakeup_abs_time;

  /* Duration Information */
  timetick_type            duration;
  
} wmgr_wakeup_info_type;

/*----------------------------------------------------------------------------
  Wakeup Manager Owner Info Structure
    - This structure holds the information for each RF device owner
    - If corresponding owner is WMGR_CLIENT_NO_CLIENT, then corresponding
      information found here is invalid.
----------------------------------------------------------------------------*/
typedef struct 
{  
  /* Request time Information */
  timetick_type            request_time;

  /* Time from now when the chain would be available
     - If duration is WMGR_TIME_INDEFINITE, then client
       is holding the chain indefinitely (ex connected state) */
  int32                    available;
  
} wmgr_owner_info_type;

/*----------------------------------------------------------------------------
  Wakeup Manager Client Structure
    - This structure holds the information for each client of wake up manager
    - This structure will be used for the client state machine   
----------------------------------------------------------------------------*/
typedef struct 
{
  /* Provides who is currently owning a RF device. A client Id of 
     WMGR_CLIENT_NO_CLIENT indicates that no client is currently holding
     the device. */
  wmgr_client_enum_t rfdev_owners[RFM_MAX_DEVICES]; 

  /* Information about clients owning an RF device. This information
     maps to the 1:1 to the RF device ownership above */
  wmgr_owner_info_type owner_info[RFM_MAX_DEVICES];
  
  /* Information about technologies that can successfully 
     wakeup or not */
  wmgr_wakeup_info_type wakeup_info[WMGR_CLIENT_LAST];
  
  /* Number of valid entries in wakeup_info */
  uint8 num_wakeup_info;
} wmgr_status_info_type;


/*============================================================================

                            FUNCTION DECLARATIONS

============================================================================*/

/*===========================================================================

FUNCTION WMGR_GET_RFCHAIN

DESCRIPTION
  This function will be used to query the future chain information for a
  particular client. 
    
DEPENDENCIES
  None

RETURN VALUE
  rfm_device_enum_type - if the client has made a trm reservation and can be
  assigned a chain.
  RFM_MAX_DEVICES - if the client is colliding with some other client.
  The information returned is based on all the current TRM clients reservation 
  states.

SIDE EFFECTS
  None
  
===========================================================================*/
rfm_device_enum_type wmgr_get_rfchain 
(
  /* Client RAT ID */
  wmgr_client_enum_t client
);

/*===========================================================================

CALLBACK WMGR_REGISTER_RAT_CB

DESCRIPTION
  The prototype for wakeup manager call back function to indicate
  Layer1s with the next wakeup information.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
typedef void (*wmgr_rat_cb_type) 
(
  /* RAT ID */
  wmgr_client_enum_t  client,

  /* The enum with the result */
  wmgr_result_type    result,
  
  /* Layer1 transitions */
  uint32              reserved1,

  /* Reserved for enhancements */
  uint32              reserved2,

  /* User defined data */
  void                *user_data
);


/*===========================================================================

FUNCTION WMGR_REGISTER_RAT_CB

DESCRIPTION
  This function will be used by Layer1s to register their wake-up call
  back with Wakeup Manager.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE - If the client successfully registers
  FALSE - If the client registration fails

SIDE EFFECTS
  This call will trigger Wakeup manager interaction with the L1.
  L1s might have to program the Sleep controller in the call back.
  
===========================================================================*/
boolean wmgr_register_rat_cb 
(
  /* Client RAT ID */
  wmgr_client_enum_t  client,
  
  /* Call Back function pointer */
  wmgr_rat_cb_type    wake_up_callback,
  
  /* User defined data */
  void                *user_data
);


/*===========================================================================

FUNCTION WMGR_INIT

DESCRIPTION
  This function will initialize Wakeup Manager.
  
DEPENDENCIES
  Should be called before Wakeup Manager can be used by Layer1s

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void wmgr_init ( void );

/*===========================================================================

FUNCTION WMGR_GET_STATUS_INFO

DESCRIPTION
  This function will return the current wakeup information that is maintained
  at Wakeup Manager. 
    
DEPENDENCIES
  None

RETURN VALUE
  Struct with status information.

SIDE EFFECTS
  None
  
===========================================================================*/
wmgr_status_info_type wmgr_get_status_info( void );

/*============================================================================

FUNCTION WMGR_STATUS_INFO_CB_TYPE

DESCRIPTION
  Prototype of Wmgr wakeup information call back function.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.  

============================================================================*/
typedef void (*wmgr_status_info_cb_type)
(
  /* WMGR information required for wakeup manager */
  wmgr_status_info_type result
);

/*============================================================================

FUNCTION WMGR_STATUS_INFO_CB_WITH_PTR_TYPE

DESCRIPTION
  Prototype of Wmgr wakeup information call back function which 
  takes in a pointer to memory location as argument.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.  

============================================================================*/
typedef void (*wmgr_status_info_cb_with_ptr_type)
(
  /* WMGR information required for wakeup manager */
  wmgr_status_info_type *result
);

/*----------------------------------------------------------------------------
  Structure for Status Info registration.
----------------------------------------------------------------------------*/
typedef struct
{
  /* ID of client registering for call back */
  wmgr_status_info_client_enum_t client;

  /* Call back function without any argument.. 
     If this is not set to NULL, then this callback will be called... 
     And this api provided the status info output as argument i.e on stack */
  wmgr_status_info_cb_type cb;
  
  /* Callback function which takes a pointer as argument... 
     This will be called only if the above callback pointer is set as NULL */
  wmgr_status_info_cb_with_ptr_type cb_with_ptr_arg;

  /* Pointer to memory location where the status info will be populated */
  wmgr_status_info_type *status_info_ptr;
  
} wmgr_status_info_reg_input;


/*----------------------------------------------------------------------------
  Structure for Status Info deregistration.
----------------------------------------------------------------------------*/
typedef struct
{
  /* ID of client deregistering for call back */
  wmgr_status_info_client_enum_t id;

} wmgr_status_info_dereg_input;

/*============================================================================

FUNCTION WMGR_REG_STATUS_INFO_CB

DESCRIPTION
  A client can register with Wakeup Manager to receive information about the
  clients that will be waking up.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE  - If the registration with WMGR was successful 
  FALSE - If the registration failed

SIDE EFFECTS
  None

============================================================================*/
boolean wmgr_reg_status_info_cb
(
  /* status registration information */
  wmgr_status_info_reg_input  reg_info
);




/*============================================================================

FUNCTION WMGR_DEREGISTER_STATUS_INFO_CB

DESCRIPTION
  A client can deregister with Wakeup Manager from getting status info.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE  - If the deregistration with WMGR was successful 
  FALSE - If the deregistration failed

SIDE EFFECTS
  None

============================================================================*/
boolean wmgr_deregister_status_info_cb
(
  /* status deregistration Information */
  wmgr_status_info_dereg_input  dereg_info
);

/*===========================================================================

FUNCTION WMGR_INIT_DEFAULTS

DESCRIPTION
  This function will initialize Wakeup Manager default values.
  
DEPENDENCIES
  Should be called before Wakeup Manager can be used by Layer1s

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void wmgr_init_defaults ( void );

#endif /* WMGR_H */

