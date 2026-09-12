/*!
  @file
  rfgsm_core_state_machine.cpp

  @brief
  RF GSM core state machine to manage and report on call-flow validity
 
  @note
  Coding style used is camelcase form to clearly distinguish between C and C++
 
  Style for types     - Capital letter for each new word i.e ThisIsTheStyle
  Style for variables - Capital letter for each new word except the first
                        i.e thisIsTheStyle
  Class definitions   - begin class name with capital "C" and follow with
                        normal camelcase
 
  @TODO - Add state machine logging
*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rftech_gsm.mpss/3.11/rf/core/src/rfgsm_core_state_machine.cpp#1 $
  $DateTime: 2021/03/09 05:10:17 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------- 
08/20/15   sc      Add framework for split Rx/Tx state machines
08/12/15   sc      Add split Rx/Tx state machines - part 1 (deprecation)
06/08/15   tws     Allow RX_BURST in Sleep for QTA.
02/13/15   sw      Improved/revised state machine logging
11/26/14   sw/sc   Initial version

===========================================================================*/
#include "rfcom.h"
#include "msg.h"

#include "rfgsm_core_state_machine.h"
#include "rfgsm_core.h"
#include "rfgsm_core_handle.h"
#include "rfm_device_types.h"

#include "rflm_api_gsm.h"


/*===========================================================================*/
/*                                                                           */
/*                             SUB STATE MACHINE                             */
/*                                                                           */
/*===========================================================================*/
/*  ________                                                                 */
/* |        |                                                                */
/* | Uninit |                                                                */
/* | State  |                                                                */
/* |________|                                                                */
/*      |                                                                    */
/*   Sub Init                                                                */
/*      |                                                                    */
/*  ___\/____                   _________                   ___________      */
/* |         | --PRx Wakeup--> |         | --Tx Wakeup --> |           |     */
/* |  Sleep  |                 |   PRx   |                 |  PRx/Tx   |     */
/* |  State  | <--PRx Sleep -- |  State  | <--Tx Sleep --- |   State   |     */
/* |_________|                 |_________|                 |___________|     */
/*   /\   |                     /\      |                    /\      |       */
/*   |    |                    DRx     DRx                  DRx     DRx      */
/*   |    |                   Sleep   Wakeup               Sleep   Wakeup    */
/*   |    |                     |       |                    |       |       */
/*   |    |                     |______\/                   _|______\/__     */
/*   |    |__PRx+DRx Wakeup--> |         | --Tx Wakeup --> |            |    */
/*   |                         | PRx+DRx |                 | PRx+DRx/Tx |    */
/*   |______PRx+DRx Sleep_____ |  State  | <--Tx Sleep --- |   State    |    */
/*                             |_________|                 |____________|    */
/*                                                                           */
/*===========================================================================*/
/*  States are:                                   |  Inputs transitions are: */
/*  Uninit State     - State on boot up           |  Sub Init                */
/*  Sleep State      - After successful rfm init  |  PRx Wakeup              */
/*  PRx State        - When PRx active            |  PRx Sleep               */
/*  PRx+DRx State    - When PRx & DRx active      |  DRx Wakeup              */
/*  PRx/Tx State     - When PRx & Tx active       |  DRx Sleep               */
/*  PRx+DRx/Tx State - When PRx & DRx & Tx active |  PRx+DRx Wakeup          */
/*                                                |  PRx+DRx Sleep           */
/*                                                |  Tx Wakeup               */
/*                                                |  Tx Sleep                */
/*===========================================================================*/
/* NOTES:                                                                    */
/* Naming convention is to use sm for "s"tate "m"achine to shorten names.    */
/*===========================================================================*/
/*===========================================================================*/
/*                                                                           */
/*                              RX STATE MACHINE                             */
/*                                                                           */
/*===========================================================================*/
/*                               _____________                               */
/*                              |             |                              */
/*                              |Uninit State |                              */
/*                              |______ ______|                              */
/*                                     |                                     */
/*    _ Rx__                        Rx Init                       _ Rx__     */
/*   |      |                          |                         |      |    */
/*  _|_____\/_                    ____\/_____                   _|_____\/__  */
/* |          | <--Meas Enter--- |           | <--Rx Sleep --- |           | */
/* |CRAT State| ---Meas Exit --> |Sleep State| ---Rx Wakeup--> |Idle State | */
/* |__________|                  |___________|                 |___________| */
/*   |    /\                                                       |   /\    */
/*   |    |__________________________Meas Enter____________________|   |     */
/*   |_______________________________Meas Exit_________________________|     */
/*                                                                           */
/*===========================================================================*/
/*  States are:                              |  Inputs transitions are:      */
/*  Uninit State - State on boot up          |  Rx Init                      */
/*  Sleep State  - After successful rfm init |  Rx Sleep                     */
/*  Idle State   - When Rx active            |  Rx Wakeup                    */
/*  CRAT State   - When measurement active   |  Rx                           */
/*                                           |  Meas Enter                   */
/*                                           |  Meas Exit                    */
/*===========================================================================*/
/* NOTES:                                                                    */
/* Naming convention is to use sm for "s"tate "m"achine to shorten names.    */
/* CM Enter/Exit is not device-specific and therefore cannot be used to drive*/
/* the state machine.                                                        */
/* L1 can call meas enter/exit multiple times and out of sequence which is   */
/* not represented by the state machine as it is an exception to accommodate */
/* invalid call-flow.                                                        */
/*===========================================================================*/
/*===========================================================================*/
/*                                                                           */
/*                              TX STATE MACHINE                             */
/*                                                                           */
/*===========================================================================*/
/*  ________                                                                 */
/* |        |                                                                */
/* | Uninit |                                                                */
/* | State  |                                                                */
/* |________|                                                                */
/*      |                                                __Tx__              */
/*   Tx Init                                            | Band |             */
/*      |                                               |      |             */
/*  ___\/____                 _________                _\/_____|__           */
/* |         | --Tx Wakeup-> |         | --Tx Band -> |           |----      */
/* |  Sleep  |               |  Idle   |              |  Traffic  |    | Tx  */
/* |  State  | <-Tx Sleep -- |  State  |              |   State   |    |Burst*/
/* |_________|               |_________|              |___________|<---      */
/*   /\                        /\     |                |  /\     |           */
/*   |                         |_Idle_|                |  |_Idle_|           */
/*   |                           Frame                 |    Frame            */
/*   |                           Proc                  |    Proc             */
/*   |                                                 |                     */
/*   |____________________Tx Sleep_____________________|                     */
/*                                                                           */
/*===========================================================================*/
/*  States are:                              |  Inputs transitions are:      */
/*  Uninit State - State on boot up          |  Tx Init                      */
/*  Sleep State  - After successful rfm init |  Tx Sleep                     */
/*  Idle State   - When Tx active            |  Tx Wakeup                    */
/*  Traffic State- When Tx Burst active      |  Tx Band                      */
/*                                           |  Tx Burst                     */
/*                                           |  Idle Frame Processing        */
/*===========================================================================*/
/* NOTES:                                                                    */
/* Naming convention is to use sm for "s"tate "m"achine to shorten names.    */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                           FUNCTION PROTOTYPES                             */
/*                                                                           */
/*===========================================================================*/
static void RfgsmCoreSmInitTransTableSub( uint8 subId );
static void RfgsmCoreSmInitTransTableRx( rfm_device_enum_type rx_device );
static void RfgsmCoreSmInitTransTableTx( rfm_device_enum_type rx_device );




/*===========================================================================*/
/*                                                                           */
/*                              DEFINTIONS                                   */
/*                                                                           */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                                MACROS                                     */
/*                                                                           */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                             DECLARATIONS                                  */
/*                                                                           */
/*===========================================================================*/
/*! @class rfgsm_sub_sm                                                      */
/*! @brief Stage 3. in setting up a state machine                            */
/*! @details                                                                 */
CRfgsmStateMachine<RfgsmCoreSmSubStateType, 
                   RfgsmCoreSmSubInputType, 
                   RFGSM_SUB_MAX_STATES, 
                   RFGSM_SUB_MAX_INPUTS> * rfgsmSubSm[RFGSM_SM_MAX_SUBS] = { 0 };



/*===========================================================================*/
/*! @class rfgsm_rx_sm                                                       */
/*! @brief Stage 3. in setting up a state machine                            */
/*! @details                                                                 */
CRfgsmStateMachine<RfgsmCoreSmRxStateType, 
                   RfgsmCoreSmRxInputType, 
                   RFGSM_RX_MAX_STATES, 
                   RFGSM_RX_MAX_INPUTS> * rfgsmRxSm[RFM_MAX_WAN_DEVICES] = { 0 };



/*===========================================================================*/
/*! @class rfgsm_tx_sm                                                       */
/*! @brief Stage 3. in setting up a state machine                            */
/*! @details                                                                 */
CRfgsmStateMachine<RfgsmCoreSmTxStateType, 
                   RfgsmCoreSmTxInputType, 
                   RFGSM_TX_MAX_STATES, 
                   RFGSM_TX_MAX_INPUTS> * rfgsmTxSm[RFM_MAX_WAN_DEVICES] = { 0 };



/*===========================================================================*/
/*! @fn RfgsmCoreSmInitTransTableSub                                         */
/*! @brief Stage 4. in setting up a state machine                            */
/*! @details This API assigns valid state transitions to the state machine   */
/*!          transitionTable member variable.                                */
/*! @Note: This API assumes a Sub ID sanity has already occurred             */
static void RfgsmCoreSmInitTransTableSub( uint8 subId )
{
  /* Sub State Machine from Uninitialised State */
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_UNINIT_STATE][RFGSM_SUB_INIT_INPUT] = RFGSM_SUB_SLEEP_STATE;
  
  /* Sub State Machine from Sleep State */
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_SLEEP_STATE][RFGSM_SUB_PRX_WAKEUP_INPUT]      = RFGSM_SUB_PRX_STATE;
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_SLEEP_STATE][RFGSM_SUB_PRX_DRX_WAKEUP_INPUT]  = RFGSM_SUB_PRX_DRX_STATE;
  
  /* Sub State Machine from PRx State */
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_STATE][RFGSM_SUB_PRX_SLEEP_INPUT]         = RFGSM_SUB_SLEEP_STATE;
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_STATE][RFGSM_SUB_TX_WAKEUP_INPUT]         = RFGSM_SUB_PRX_TX_STATE;
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_STATE][RFGSM_SUB_DRX_WAKEUP_INPUT]        = RFGSM_SUB_PRX_DRX_STATE;

  /* Sub State Machine from PRx+DRx State */
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_DRX_STATE][RFGSM_SUB_DRX_SLEEP_INPUT]     = RFGSM_SUB_PRX_STATE;
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_DRX_STATE][RFGSM_SUB_PRX_DRX_SLEEP_INPUT] = RFGSM_SUB_SLEEP_STATE;
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_DRX_STATE][RFGSM_SUB_TX_WAKEUP_INPUT]     = RFGSM_SUB_PRX_DRX_TX_STATE;

  /* Sub State Machine from PRx\Tx State */
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_TX_STATE][RFGSM_SUB_TX_SLEEP_INPUT]       = RFGSM_SUB_PRX_STATE;
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_TX_STATE][RFGSM_SUB_DRX_WAKEUP_INPUT]     = RFGSM_SUB_PRX_DRX_TX_STATE;

  /* Sub State Machine from PRx+DRx\Tx State */
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_DRX_TX_STATE][RFGSM_SUB_DRX_SLEEP_INPUT]  = RFGSM_SUB_PRX_TX_STATE;
  rfgsmSubSm[subId]->transitionTable[RFGSM_SUB_PRX_DRX_TX_STATE][RFGSM_SUB_TX_SLEEP_INPUT]   = RFGSM_SUB_PRX_DRX_STATE;
}



/*===========================================================================*/
/*! @fn RfgsmCoreSmInitTransTableRx                                          */
/*! @brief Stage 4. in setting up a state machine                            */
/*! @details This API assigns valid state transitions to the state machine   */
/*!          transitionTable member variable.                                */
/*! @Note: This API assumes a device ID sanity has already occurred          */
static void RfgsmCoreSmInitTransTableRx( rfm_device_enum_type rxDevice )
{
  /* Rx State Machine from Uninitialised State */
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_UNINIT_STATE][RFGSM_RX_INIT_INPUT]      = RFGSM_RX_SLEEP_STATE;
  
  /* Rx State Machine from Sleep State */
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_SLEEP_STATE][RFGSM_RX_WAKEUP_INPUT]     = RFGSM_RX_IDLE_STATE;
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_SLEEP_STATE][RFGSM_RX_MEAS_ENTER_INPUT] = RFGSM_RX_CRAT_STATE;
  
  /* Rx State Machine from Idle State */
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_IDLE_STATE][RFGSM_RX_BURST_INPUT]       = RFGSM_RX_RETAIN_STATE;
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_IDLE_STATE][RFGSM_RX_SLEEP_INPUT]       = RFGSM_RX_SLEEP_STATE;
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_IDLE_STATE][RFGSM_RX_MEAS_ENTER_INPUT]  = RFGSM_RX_CRAT_STATE;
  
  /* Rx State Machine from CRAT State */
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_CRAT_STATE][RFGSM_RX_BURST_INPUT]       = RFGSM_RX_RETAIN_STATE;
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_CRAT_STATE][RFGSM_RX_MEAS_EXIT_INPUT]   = RFGSM_RX_PREV_STATE;
  
  /* CALLFLOW EXCEPTIONS */
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_SLEEP_STATE][RFGSM_RX_MEAS_EXIT_INPUT]  = RFGSM_RX_RETAIN_STATE;
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_IDLE_STATE][RFGSM_RX_MEAS_EXIT_INPUT]   = RFGSM_RX_RETAIN_STATE;
  rfgsmRxSm[rxDevice]->transitionTable[RFGSM_RX_CRAT_STATE][RFGSM_RX_MEAS_ENTER_INPUT]  = RFGSM_RX_RETAIN_STATE;
}



/*===========================================================================*/
/*! @fn RfgsmCoreSmInitTransTableTx                                          */
/*! @brief Stage 4. in setting up a state machine                            */
/*! @details This API assigns valid state transitions to the state machine   */
/*!          transitionTable member variable.                                */
/*! @Note: This API assumes a device ID sanity has already occurred          */
static void RfgsmCoreSmInitTransTableTx( rfm_device_enum_type txDevice )
{
  rfgsmTxSm[txDevice]->transitionTable[RFGSM_TX_UNINIT_STATE][RFGSM_TX_INIT_INPUT]    = RFGSM_TX_SLEEP_STATE;
  
  /* Tx State Machine from Sleep State */
  rfgsmTxSm[txDevice]->transitionTable[RFGSM_TX_SLEEP_STATE][RFGSM_TX_WAKEUP_INPUT]   = RFGSM_TX_IDLE_STATE;
  
  /* Tx State Machine from Idle State */
  rfgsmTxSm[txDevice]->transitionTable[RFGSM_TX_IDLE_STATE][RFGSM_TX_BAND_INPUT]      = RFGSM_TX_TRAFFIC_STATE; 
  rfgsmTxSm[txDevice]->transitionTable[RFGSM_TX_IDLE_STATE][RFGSM_TX_SLEEP_INPUT]     = RFGSM_TX_SLEEP_STATE;
  rfgsmTxSm[txDevice]->transitionTable[RFGSM_TX_IDLE_STATE][RFGSM_TX_IDLE_FRAME_PROC_INPUT] = RFGSM_TX_RETAIN_STATE;
  
  /* Tx State Machine from Traffic State */
  rfgsmTxSm[txDevice]->transitionTable[RFGSM_TX_TRAFFIC_STATE][RFGSM_TX_BAND_INPUT]   = RFGSM_TX_RETAIN_STATE;
  rfgsmTxSm[txDevice]->transitionTable[RFGSM_TX_TRAFFIC_STATE][RFGSM_TX_BURST_INPUT]  = RFGSM_TX_RETAIN_STATE;
  rfgsmTxSm[txDevice]->transitionTable[RFGSM_TX_TRAFFIC_STATE][RFGSM_TX_IDLE_FRAME_PROC_INPUT] = RFGSM_TX_RETAIN_STATE;
  rfgsmTxSm[txDevice]->transitionTable[RFGSM_TX_TRAFFIC_STATE][RFGSM_TX_SLEEP_INPUT]  = RFGSM_TX_SLEEP_STATE;
}



/*===========================================================================*/
/*! @fn RfgsmCoreSmDriveSub                                                  */
/*! @brief Stage 5. in setting up a state machine                            */
/*! @details                                                                 */
void RfgsmCoreSmDriveSub( uint8 subId, 
                          RfgsmCoreSmSubInputType input )
{
  /* Validate the state machine */
  if( rfgsmSubSm[subId] == NULL )
  {
    ERR_FATAL("RfgsmCoreSmDriveSub() state machine not created for sub %d", subId, 0, 0);
	return;
  }
  
  /* Drive the state machine */
  rfgsmSubSm[subId]->DriveStateMachine( input );
}



/*===========================================================================*/
/*! @fn RfgsmCoreSmDriveRx                                                   */
/*! @brief Stage 5. in setting up a state machine                            */
/*! @details                                                                 */
void RfgsmCoreSmDriveRx( rfm_device_enum_type rxDevice, 
                         RfgsmCoreSmRxInputType input )
{
  /* Validate the state machine */
  if( rfgsmRxSm[rxDevice] == NULL )
  {
    ERR_FATAL("RfgsmCoreSmDriveRx() state machine not created for Rx device %d", rxDevice, 0, 0);
    return;
  }
  
  /* Drive the state machine */
  rfgsmRxSm[rxDevice]->DriveStateMachine( input );
}



/*===========================================================================*/
/*! @fn RfgsmCoreSmDriveTx                                                   */
/*! @brief Stage 5. in setting up a state machine                            */
/*! @details                                                                 */
void RfgsmCoreSmDriveTx( rfm_device_enum_type txDevice, 
                         RfgsmCoreSmTxInputType input )
{
  /* Validate the state machine */
  if( rfgsmTxSm[txDevice] == NULL )
  {
    ERR_FATAL("RfgsmCoreSmDriveTx() state machine not created for Tx device %d", txDevice, 0, 0);
    return;
  }
  
  /* Drive the state machine */
  rfgsmTxSm[txDevice]->DriveStateMachine( input );
}



/*===========================================================================*/
/*! @class RfgsmCoreSmInitSub                                                */
/*! @brief Stage 6. in setting up a state machine                            */
/*! @details This API creates a state machine object and initialises it with */
/*!          Sub state machine specific data.                                */
/*! @Note: This API assumes a device ID sanity has already occurred          */
void RfgsmCoreSmInitSub( uint8 subId )
{
  /* Validate the state machine */
  if( rfgsmSubSm[subId] != NULL )
  {
    ERR_FATAL("RfgsmCoreSmInitSub() state machine already present for sub %d", subId, 0, 0);
    return;
  }
  
  /* Create Rx state machine */
  rfgsmSubSm[subId] = new CRfgsmStateMachine<RfgsmCoreSmSubStateType, 
                                             RfgsmCoreSmSubInputType, 
                                             RFGSM_SUB_MAX_STATES, 
                                             RFGSM_SUB_MAX_INPUTS>( RFGSM_SM_ID_SUB, subId );
											
  /* Ensure the state machine was created */
  if( rfgsmSubSm[subId] == NULL )
  {
    ERR_FATAL("RfgsmCoreSmInitSub() failed to create sub %d state machine", subId, 0, 0);
    return;
  }
  
  /* Initialise the state machine */
  rfgsmSubSm[subId]->InitStateMachine();
  
  /* Initialise the valid state transition table for Rx state machine */
  RfgsmCoreSmInitTransTableSub( subId );
  
  /* Drive state machine to starting sleep state */
  RfgsmCoreSmDriveSub( subId, RFGSM_SUB_INIT_INPUT );
}



/*===========================================================================*/
/*! @class RfgsmCoreSmInitRx                                                 */
/*! @brief Stage 6. in setting up a state machine                            */
/*! @details This API creates a state machine object and initialises it with */
/*!          Rx state machine specific data.                                 */
/*! @Note: This API assumes a device ID sanity has already occurred          */
void RfgsmCoreSmInitRx( rfm_device_enum_type rxDevice )
{
  /* Validate the state machine */
  if( rfgsmRxSm[rxDevice] != NULL )
  {
    ERR_FATAL("RfgsmCoreSmInitRx() state machine already present for Rx device %d", rxDevice, 0, 0);
    return;
  }
  
  /* Create Rx state machine */
  rfgsmRxSm[rxDevice] = new CRfgsmStateMachine<RfgsmCoreSmRxStateType, 
                                               RfgsmCoreSmRxInputType, 
                                               RFGSM_RX_MAX_STATES, 
                                               RFGSM_RX_MAX_INPUTS>( RFGSM_SM_ID_RX, rxDevice );
											
  /* Ensure the state machine was created */
  if( rfgsmRxSm[rxDevice] == NULL )
  {
    ERR_FATAL("RfgsmCoreSmInitRx() failed to create Rx device %d state machine", rxDevice, 0, 0);
    return;
  }
  
  /* Initialise the state machine */
  rfgsmRxSm[rxDevice]->InitStateMachine();
  
  /* Initialise the valid state transition table for Rx state machine */
  RfgsmCoreSmInitTransTableRx( rxDevice );
  
  /* Drive state machine to starting sleep state */
  RfgsmCoreSmDriveRx( rxDevice, RFGSM_RX_INIT_INPUT );
}



/*===========================================================================*/
/*! @class RfgsmCoreSmInitTx                                                 */
/*! @brief Stage 6. in setting up a state machine                            */
/*! @details This API creates a state machine object and initialises it with */
/*!          Tx state machine specific data.                                 */
/*! @Note: This API assumes a device ID sanity has already occurred          */
void RfgsmCoreSmInitTx( rfm_device_enum_type txDevice )
{
  /* Validate the state machine */
  if( rfgsmTxSm[txDevice] != NULL )
  {
    ERR_FATAL("RfgsmCoreSmInitTx() state machine already present for Tx device %d", txDevice, 0, 0);
    return;
  }
  
  /* Create Rx state machine */
  rfgsmTxSm[txDevice] = new CRfgsmStateMachine<RfgsmCoreSmTxStateType, 
                                               RfgsmCoreSmTxInputType, 
                                               RFGSM_TX_MAX_STATES, 
                                               RFGSM_TX_MAX_INPUTS>( RFGSM_SM_ID_TX, txDevice );
											
  /* Ensure the state machine was created */
  if( rfgsmTxSm[txDevice] == NULL )
  {
    ERR_FATAL("RfgsmCoreSmInitTx() failed to create Tx device %d state machine", txDevice, 0, 0);
    return;
  }
  
  /* Initialise the state machine */
  rfgsmTxSm[txDevice]->InitStateMachine();
  
  /* Initialise the valid state transition table for Rx state machine */
  RfgsmCoreSmInitTransTableTx( txDevice );
  
  /* Drive state machine to starting sleep state */
  RfgsmCoreSmDriveTx( txDevice, RFGSM_TX_INIT_INPUT );
}




/*===========================================================================*/
/*! @class RfgsmCoreSmDeinitSub                                              */
/*! @brief Stage 7. in setting up a state machine                            */
/*! @details This API deletes a state machine object and should be called    */
/*!          from rfgsm_mc_deinit via shim.                                  */
/*! @Note: This API assumes a sub ID sanity has already occurred             */
void RfgsmCoreSmDeinitSub( uint8 subId )
{
  /* Validate the state machine */
  if( rfgsmSubSm[subId] == NULL )
  {
    ERR_FATAL("RfgsmCoreSmDeinitSub() Sub %d state machine already NULL", subId, 0, 0);
	return;
  }
  
  /* Destroy Sub state machine */
  delete rfgsmSubSm[subId];
  rfgsmSubSm[subId] = NULL;
}



/*===========================================================================*/
/*! @class RfgsmCoreSmDeinitRx                                               */
/*! @brief Stage 7. in setting up a state machine                            */
/*! @details This API deletes a state machine object and should be called    */
/*!          from rfgsm_mc_deinit via shim.                                  */
/*! @Note: This API assumes an Rx device ID sanity has already occurred      */
void RfgsmCoreSmDeinitRx( rfm_device_enum_type rxDevice )
{
  /* Validate the state machine */
  if( rfgsmRxSm[rxDevice] == NULL )
  {
    ERR_FATAL("RfgsmCoreSmDeinitRx() Rx device %d state machine already NULL", rxDevice, 0, 0);
	return;
  }
  
  /* Destroy Rx state machine */
  delete rfgsmRxSm[rxDevice];
  rfgsmRxSm[rxDevice] = NULL;
}



/*===========================================================================*/
/*! @class RfgsmCoreSmDeinitTx                                               */
/*! @brief Stage 7. in setting up a state machine                            */
/*! @details This API deletes a state machine object and should be called    */
/*!          from rfgsm_mc_deinit via shim.                                  */
/*! @Note: This API assumes an Rx device ID sanity has already occurred      */
void RfgsmCoreSmDeinitTx( rfm_device_enum_type txDevice )
{
  /* Validate the state machine */
  if( rfgsmTxSm[txDevice] == NULL )
  {
    ERR_FATAL("RfgsmCoreSmDeinitTx() Tx device %d state machine already NULL", txDevice, 0, 0);
    return;
  }

  /* Destroy Tx state machine */
  delete rfgsmTxSm[txDevice];
  rfgsmTxSm[txDevice] = NULL;
}

