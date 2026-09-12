/*!
  @file
  rfgsm_core_state_machine.h

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

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/api/rfgsm_core_state_machine.h#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ------------------------------------------------------------ 
08/20/15   sc      Add framework for split Rx/Tx state machines
08/12/15   sc      Add split Rx/Tx state machines - part 1 (deprecation)
11/26/14   sw/sc   Initial version

=============================================================================*/

#ifndef RFGSM_CORE_STATE_MACHINE_H
#define RFGSM_CORE_STATE_MACHINE_H

/*===========================================================================*/
/*                                                                           */
/*                             INCLUDE FILES                                 */
/*                                                                           */
/*===========================================================================*/
#include "comdef.h"
#include "msg.h"
#include "rfm_device_types.h"



/*===========================================================================*/
/*                                                                           */
/*                               FEATURES                                    */
/*                                                                           */
/*===========================================================================*/
/*! @define FEATURE_RFGSM_TX_SM_CRASH_ON_ERROR                               */
/*! @brief Feature to force a crash if the state machine detects an error    */
/*! @TODO turn this feature off when the state machine is enabled and        */
/*!       reconsider when state machine is stable and thoroughly tested.     */
#define FEATURE_RFGSM_SM_CRASH_ON_ERROR



/*===========================================================================*/
/*! @define FEATURE_RFGSM_SM_DEBUG                                           */
/*! @brief Feature to enable additional debug of state machine               */
/*! @TODO turn this feature off when the state machine is enabled            */
#define FEATURE_RFGSM_SM_DEBUG



/*===========================================================================*/
/*! @define FEATURE_RFGSM_SM_LOGGING                                         */
/*! @brief Feature to enable logging of state machine                        */
/*! @TODO this feature is a placeholder for when logging is being developed  */
#define FEATURE_RFGSM_SM_LOGGING






/*===========================================================================*/
/*                                                                           */
/*                              DEFINTIONS                                   */
/*                                                                           */
/*===========================================================================*/


/*! @define RFGSM_SM_MAX_SUBS                                                */
/*! @brief Default uninitialised state for all state machines                */
/*! @TODO use common definition for max subscriptions when sub3 deprecated   */
#define RFGSM_SM_MAX_SUBS 2



/*===========================================================================*/
/*! @define RFGSM_UNINIT_STATE_VAL                                           */
/*! @brief Default uninitialised state for all state machines                */
#define RFGSM_UNINIT_STATE_VAL  0x0



/*===========================================================================*/
/*! @define RFGSM_INVALID_STATE_VAL                                          */
/*! @brief Default invalid transition state for all state machines           */
#define RFGSM_INVALID_STATE_VAL 0x0000FFFF



/*===========================================================================*/
/*! @define RFGSM_RETAIN_STATE_VAL                                           */
/*! @brief Default retain state transition for all state machines            */
#define RFGSM_RETAIN_STATE_VAL  0x0000FFFE



/*===========================================================================*/
/*! @define RFGSM_PREV_STATE_VAL                                             */
/*! @brief Default return to previous state transition for all state machines*/
#define RFGSM_PREV_STATE_VAL    0x0000FFFD



/*===========================================================================*/
/*! @define RFGSM_INVALID_STATE                                              */
/*! @brief Default invalid state enum definition with assigned value         */
#define RFGSM_INVALID_STATE(type) \
                   RFGSM_##type##_INVALID_STATE = RFGSM_INVALID_STATE_VAL



/*===========================================================================*/
/*! @define RFGSM_RETAIN_STATE                                               */
/*! @brief Default retain state enum definition with assigned value          */
#define RFGSM_RETAIN_STATE(type) \
                   RFGSM_##type##_RETAIN_STATE = RFGSM_RETAIN_STATE_VAL



/*===========================================================================*/
/*! @define RFGSM_PREV_STATE                                                 */
/*! @brief Default prev state enum definition with assigned value            */
#define RFGSM_PREV_STATE(type) \
                   RFGSM_##type##_PREV_STATE = RFGSM_PREV_STATE_VAL



/*===========================================================================*/
/*! @define RFGSM_STATE_TYPE_HEADER                                          */
/*! @brief Header definition for adding required enums to top of state enum  */
#define RFGSM_STATE_TYPE_HEADER(type) \
                   RFGSM_##type##_UNINIT_STATE = RFGSM_UNINIT_STATE_VAL



/*===========================================================================*/
/*! @define RFGSM_STATE_TYPE_FOOTER                                          */
/*! @brief Footer definition for adding required enums to bottom of state    */
/*         enum                                                              */
#define RFGSM_STATE_TYPE_FOOTER(type) \
                   RFGSM_PREV_STATE(type), \
                   RFGSM_RETAIN_STATE(type), \
                   RFGSM_INVALID_STATE(type)



/*===========================================================================*/
/*! @define RFGSM_SM_MSG_HEADER                                              */
/*! @brief Header to all class F3 messages                                   */
#define RFGSM_SM_MSG_HEADER "CRfgsmStateMachine id %d idx %d "



/*===========================================================================*/
/*! @define RFGSM_SM_MSG_FATAL                                               */
/*! @brief Fatal on error signalled from the state machine                   */
/*! IMPORTANT: THIS MSG WILL ONLY WORK WHEN USED BY SM CLASS MEMBER          */
#ifdef FEATURE_RFGSM_SM_CRASH_ON_ERROR
#define RFGSM_SM_MSG_FATAL( msg, arg1 ) \
                    ERR_FATAL( RFGSM_SM_MSG_HEADER msg, smId, smIdx, arg1)
#else
#define RFGSM_SM_MSG_FATAL( msg, arg1 ) \
                    MSG_3( MSG_SSID_RF, MSG_LEGACY_ERROR, RFGSM_SM_MSG_HEADER msg, smId, smIdx, arg1)
#endif



#ifdef FEATURE_RFGSM_SM_DEBUG
/*===========================================================================*/
/*! @define RFGSM_SM_MSG                                                     */
/*! @brief Print F3 with state machine id and index                          */
/*! IMPORTANT: THIS MSG WILL ONLY WORK WHEN USED BY SM CLASS MEMBER          */
#define RFGSM_SM_MSG( type, msg) \
                    MSG_2( MSG_SSID_RF, type, RFGSM_SM_MSG_HEADER msg, smId, smIdx, arg1 )


/*===========================================================================*/
/*! @define RFGSM_SM_MSG_1                                                   */
/*! @brief Print F3 with state machine id and index and accept 1 argument    */
/*! IMPORTANT: THIS MSG WILL ONLY WORK WHEN USED BY SM CLASS MEMBER          */
#define RFGSM_SM_MSG_1( type, msg, arg1 ) \
                    MSG_3( MSG_SSID_RF, type, RFGSM_SM_MSG_HEADER msg, smId, smIdx, arg1 )



/*===========================================================================*/
/*! @define RFGSM_SM_MSG_2                                                   */
/*! @brief Print F3 with state machine id and index and accept 1 argument    */
/*! IMPORTANT: THIS MSG WILL ONLY WORK WHEN USED BY SM CLASS MEMBER          */
#define RFGSM_SM_MSG_2( type, msg, arg1, arg2 ) \
                    MSG_4( MSG_SSID_RF, type, RFGSM_SM_MSG_HEADER msg, smId, smIdx, arg1, arg2 )



/*===========================================================================*/
/*! @define RFGSM_SM_MSG_3                                                   */
/*! @brief Print F3 with state machine id and index and accept 1 argument    */
/*! IMPORTANT: THIS MSG WILL ONLY WORK WHEN USED BY SM CLASS MEMBER          */
#define RFGSM_SM_MSG_3( type, msg, arg1, arg2, arg3 ) \
                    MSG_5( MSG_SSID_RF, type, RFGSM_SM_MSG_HEADER msg, smId, smIdx, arg1, arg2, arg3 )
#else
#define RFGSM_SM_MSG( type, msg ) { /*Currently do nothing */ }
#define RFGSM_SM_MSG_1( type, msg, arg1 ) { /*Currently do nothing */ }
#define RFGSM_SM_MSG_2( type, msg, arg1, arg2 ) { /*Currently do nothing */ }
#define RFGSM_SM_MSG_3( type, msg, arg1, arg2, arg3 ) { /*Currently do nothing */ }
#endif /*FEATURE_RFGSM_SM_DEBUG*/



/*===========================================================================*/
/*! @define RFGSM_SM_CREATED_MSG                                             */
/*! @brief Print F3 with state machine id and index informing creation of a  */
/*!        new instance                                                      */
/*! IMPORTANT: THIS MSG WILL ONLY WORK WHEN USED BY SM CLASS MEMBER          */
#define RFGSM_SM_CREATED_MSG() \
                    MSG_4( MSG_SSID_RF, MSG_LEGACY_HIGH, RFGSM_SM_MSG_HEADER  \
                           "created size %d, addr %d", \
                           smId, smIdx, sizeof(CRfgsmStateMachine), this )



/*===========================================================================*/
/*! @define RFGSM_SM_INITIALISED_MSG                                         */
/*! @brief Print F3 with state machine id and index informing initialisation */
/*!        of a state machine object                                         */
/*! IMPORTANT: THIS MSG WILL ONLY WORK WHEN USED BY SM CLASS MEMBER          */
#define RFGSM_SM_INITIALISED_MSG() \
                    MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, RFGSM_SM_MSG_HEADER  \
                           "initialised", \
                           smId, smIdx )



/*===========================================================================*/
/*! @enum RfgsmSmidType                                                      */
/*! @brief definitions for type of state machine                             */
/*! @details Used for debug and logging of state machine                     */
typedef enum
{
  RFGSM_SM_ID_TYPE_DEFAULT,
  RFGSM_SM_ID_SUB,
  RFGSM_SM_ID_RX,
  RFGSM_SM_ID_TX,
  RFGSM_SM_MAX_ID_TYPES
}RfgsmSmidType;



/*===========================================================================*/
/*! @enum rfgsm_core_sm_sub_state_type                                       */
/*! @brief Stage 1. in setting up a state machine                            */
/*! @details This is the states enum for subscription based state machine    */
/*! @TODO consider adding meas APIs to sub state machine                     */
typedef enum
{
  /* Header to create UNINIT state */
  RFGSM_STATE_TYPE_HEADER(SUB),
  
  RFGSM_SUB_SLEEP_STATE,
  RFGSM_SUB_PRX_STATE,
  RFGSM_SUB_PRX_DRX_STATE,
  RFGSM_SUB_PRX_TX_STATE,
  RFGSM_SUB_PRX_DRX_TX_STATE,
  RFGSM_SUB_MAX_STATES,
  
  /* Footer to create PREV, RETAIN, INVALID states */
  RFGSM_STATE_TYPE_FOOTER(SUB)
}RfgsmCoreSmSubStateType;



/*===========================================================================*/
/*! @enum rfgsm_core_sm_sub_input_type                                       */
/*! @brief Stage 2. in setting up a state machine                            */
/*! @details This is the inputs enum for subscription based state machine    */
typedef enum
{
  RFGSM_SUB_DEFAULT_INPUT,
  RFGSM_SUB_INIT_INPUT,
  RFGSM_SUB_PRX_WAKEUP_INPUT,
  RFGSM_SUB_PRX_SLEEP_INPUT,
  RFGSM_SUB_DRX_WAKEUP_INPUT,
  RFGSM_SUB_DRX_SLEEP_INPUT,
  RFGSM_SUB_PRX_DRX_WAKEUP_INPUT,
  RFGSM_SUB_PRX_DRX_SLEEP_INPUT,
  RFGSM_SUB_TX_WAKEUP_INPUT,
  RFGSM_SUB_TX_SLEEP_INPUT,
  RFGSM_SUB_MAX_INPUTS
}RfgsmCoreSmSubInputType;



/*===========================================================================*/
/*! @enum rfgsm_core_sm_rx_state_type                                        */
/*! @brief Stage 1. in setting up a state machine                            */
/*! @details This is the states enum for Rx device based state machine       */
typedef enum
{
  /* Header to create UNINIT state */
  RFGSM_STATE_TYPE_HEADER(RX),
  
  RFGSM_RX_SLEEP_STATE,
  RFGSM_RX_IDLE_STATE,
  RFGSM_RX_CRAT_STATE,
  RFGSM_RX_MAX_STATES,
  
  /* Footer to create PREV, RETAIN, INVALID states */
  RFGSM_STATE_TYPE_FOOTER(RX)
}RfgsmCoreSmRxStateType;



/*===========================================================================*/
/*! @enum rfgsm_core_sm_rx_input_type                                        */
/*! @brief Stage 2. in setting up a state machine                            */
/*! @details This is the inputs enum for Rx device based state machine       */
typedef enum
{
  RFGSM_RX_DEFAULT_INPUT,
  RFGSM_RX_INIT_INPUT,
  RFGSM_RX_SLEEP_INPUT,
  RFGSM_RX_WAKEUP_INPUT,
  RFGSM_RX_BURST_INPUT,
  RFGSM_RX_MEAS_ENTER_INPUT,
  RFGSM_RX_MEAS_EXIT_INPUT,
  RFGSM_RX_MAX_INPUTS
}RfgsmCoreSmRxInputType;



/*===========================================================================*/
/*! @enum rfgsm_core_sm_tx_state_type                                        */
/*! @brief Stage 1. in setting up a state machine                            */
/*! @details This is the states enum for Tx device based state machine       */
typedef enum
{
  /* Header to create UNINIT state */
  RFGSM_STATE_TYPE_HEADER(TX),
  
  RFGSM_TX_SLEEP_STATE,
  RFGSM_TX_IDLE_STATE,
  RFGSM_TX_TRAFFIC_STATE,
  RFGSM_TX_MAX_STATES,
  
  /* Footer to create PREV, RETAIN, INVALID states */
  RFGSM_STATE_TYPE_FOOTER(TX)
}RfgsmCoreSmTxStateType;



/*===========================================================================*/
/*! @enum rfgsm_core_sm_tx_input_type                                        */
/*! @brief Stage 2. in setting up a state machine                            */
/*! @details This is the inputs enum for Tx device based state machine       */
typedef enum
{
  RFGSM_TX_DEFAULT_INPUT,
  RFGSM_TX_INIT_INPUT,
  RFGSM_TX_SLEEP_INPUT,
  RFGSM_TX_WAKEUP_INPUT,
  RFGSM_TX_BAND_INPUT,
  RFGSM_TX_BURST_INPUT,
  RFGSM_TX_IDLE_FRAME_PROC_INPUT,
  RFGSM_TX_MAX_INPUTS
}RfgsmCoreSmTxInputType;



/*===========================================================================*/
/*                                                                           */
/*                         FUNCTION DEFINITIONS                              */
/*                                                                           */
/*===========================================================================*/

void RfgsmCoreSmDriveSub( uint8 subId, 
                          RfgsmCoreSmSubInputType input );

/*---------------------------------------------------------------------------*/

void RfgsmCoreSmInitSub( uint8 subId );

/*---------------------------------------------------------------------------*/

void RfgsmCoreSmDeinitSub( uint8 subId );

/*---------------------------------------------------------------------------*/

void RfgsmCoreSmDriveRx( rfm_device_enum_type rxDevice, 
                         RfgsmCoreSmRxInputType input );

/*---------------------------------------------------------------------------*/

void RfgsmCoreSmInitRx( rfm_device_enum_type rxDevice );

/*---------------------------------------------------------------------------*/

void RfgsmCoreSmDeinitRx( rfm_device_enum_type rxDevice );

/*---------------------------------------------------------------------------*/

void RfgsmCoreSmDriveTx( rfm_device_enum_type txDevice, 
                         RfgsmCoreSmTxInputType input );

/*---------------------------------------------------------------------------*/

void RfgsmCoreSmInitTx( rfm_device_enum_type txDevice );

/*---------------------------------------------------------------------------*/

void RfgsmCoreSmDeinitTx( rfm_device_enum_type txDevice );



/*=============================================================================

                       STATE MACHINE CLASS DEFINITION

=============================================================================*/
/*                                                                           */
/*                            STATE MACHINE MODEL                            */
/*                                                                           */
/*===========================================================================*/
/* Normal operation is starting in a "state" and based on a driving "input"  */
/* the state machine determines whether the result is:                       */
/* 1. Entering a new state                                                   */
/*  _______                _________________                                 */
/* |       |              |                 |                                */
/* | State | -- Input --> | Resultant State |                                */
/* |_______|              |_________________|                                */
/*                                                                           */
/*                                                                           */
/* 2. Retaining the current state                                            */
/*  _______                                                                  */
/* |       | ----                                                            */
/* | State |   Input                                                         */
/* |_______| <---                                                            */
/*                                                                           */
/*                                                                           */
/* 3. Returning to the state it previously came from                         */
/*    A "Resultant State" may be driven to by multiple other states (State X */
/*    or State Y) but by the same input (Input a) and therefore it is        */
/*    important that the returning state, driven by the same input (Input b),*/
/*    is the one in which it came from.                                      */
/*  _________                  _________________                  _________  */
/* |         | -- Input a --> |                 | <- Input a --- |         | */
/* | State X |                | Resultant State |                | State Y | */
/* |_________| <- Input b --- |_________________| -- Input b --> |_________| */
/*                                                                           */
/*                                                                           */
/* 4. Declaring the state transition attempted is an invalid one             */
/*  _______                                                                  */
/* |       |                                                                 */
/* | State | -- Input --> INVALID - THROW ERROR                              */
/* |_______|                                                                 */
/*                                                                           */
/*                                                                           */
/*===========================================================================*/
/*                          State Machine Template                           */
/* The state machine has been modelled into a template to reduce code        */
/* repetition and standardise a state machine model to make it easy to add   */
/* additional state machines in the future.                                  */
/* Templates can hurt the compilers ability to produce efficient code because*/
/* it is unclear to the compiler what ones intention may be. In this case    */
/* however the state machine code is rather simple and therefore has limited */
/* compiler optimisations available.                                         */
/*                                                                           */
/* How do I set up a state machine?                                          */
/* 1. Define a states enum (e.g states_enum_type) using the following header */
/*    and footer macros to ensure commonality in the state machine driver    */
/*    RFGSM_STATE_TYPE_HEADER(name) & RFGSM_STATE_TYPE_FOOTER(name)          */
/* 2. Define an inputs enum (e.g inputs_enum_type)                           */
/* 3. Define global state machine pointer/s                                  */
/*    MyStateMachine< states_enum_type,                                      */
/*                    inputs_enum_type,                                      */
/*                    MAX_STATES_from_states_enum_type,                      */
/*                    MAX_INPUTS_from_inputs_enum_type> * state_machine_name;*/
/* 4. Define a new function to initialise all valid transitions in the       */
/*    "transition_table[][]" in the "state_machine_name" object              */
/*    Only the successful transitions need to be assigned.                   */ 
/* 5. Add a new state machine driver function to execute the template        */
/*    DriveStateMachine member function using "state_machine_name" object ptr*/
/*   (created in 3.)                                                         */
/* 6. Add a new state machine init function to create the state machine      */
/*    object and assign to (3.).                                             */
/* 7. Add a new state machine deinit function to destroy the state machine   */
/*    object.                                                                */
/*                                                                           */
/*===========================================================================*/
#ifdef __cplusplus
/*===========================================================================*/
/*! @class CRfgsmStateMachine                                                */
/*! @brief Template class to define new state machine                        */
/*!        The template enables user defined state machine states and input  */
/*!        types whilst using the common state machine framework.            */
/*! @details An object created of this class will have state machine logic   */
/*!          by simply calling the DriveStateMachine member function.        */
template<typename STATE_TYPE, typename INPUT_TYPE, uint32 MAX_STATES, uint32 MAX_INPUTS>
class CRfgsmStateMachine
{
  public:
    /* State Machine constructor */
    CRfgsmStateMachine( RfgsmSmidType id, uint32 idx )
    {
      /* Assign private member variables for: */
      /* o type of state machine */
      smId = id;

      /* o index of that type of state machine */
      smIdx = idx;

      /* Print message to log the initialisation of the state machine */
      RFGSM_SM_CREATED_MSG();
    }
	
    /* State Machine destructor */
    ~CRfgsmStateMachine(){}
	
    /* State machine transition table */
    /* Stores transition specification for each state based on input */
    STATE_TYPE transitionTable[MAX_STATES][MAX_INPUTS];

    /*=======================================================================*/
    /*                         State Machine Init                            */
    /*=======================================================================*/
    void InitStateMachine( void )
    {
      /* Initialise all state machine variables */
      currState = (STATE_TYPE)RFGSM_UNINIT_STATE_VAL;
      prevState = (STATE_TYPE)RFGSM_UNINIT_STATE_VAL;
	  
      /* Initialise all transitions in the new transition table to invalid */
      for( uint32 stateIdx = 0; stateIdx < MAX_STATES; stateIdx++ )
      {
        for( uint32 inputIdx = 0; inputIdx < MAX_INPUTS; inputIdx++ )
        {
          transitionTable[stateIdx][inputIdx] = (STATE_TYPE)RFGSM_INVALID_STATE_VAL;
        }
      }

      /* Print message to log the initialisation of the state machine */
      RFGSM_SM_INITIALISED_MSG();
    }
	
    /*=======================================================================*/
    /*                         State Machine driver                          */
    /*=======================================================================*/
    /*! @fn DriveStateMachine                                                */
    /*! @brief Controls the transitions between states based on a given input*/
    /*! @details Transition table uses current state private member variable */
    /*!          and input from the caller to determine whether the input is */
    /*!          valid and if we are:                                        */
    /*!          o transitioning to a new state                              */
    /*!          o retaining the state                                       */
    /*!          o returning to the previous state                           */
    void DriveStateMachine( INPUT_TYPE input )
    {
      /* Validate the input to the state machine driver */
      if( input >= MAX_INPUTS )
      {
        RFGSM_SM_MSG_FATAL("invalid input %d", input);
      }

      /* Based on input and current state attempt to drive the state machine */
      switch( transitionTable[currState][input] )
      {
        /* Transitioning to a new state */
        default:
          /* Previous state is now the current state */
          prevState = currState;

          /* Current state is now the new transitioned state */
          currState = transitionTable[currState][input];

          /* Print message to log the successful transition */
          RFGSM_SM_MSG_3( MSG_LEGACY_HIGH, 
                          "valid transition from state %d to state %d with input %d", 
                          prevState, currState, input );
          break;

        /* Remaining in current state */
        case RFGSM_RETAIN_STATE_VAL:
          /* Print message to log the successful transition */
          RFGSM_SM_MSG_2( MSG_LEGACY_HIGH, 
                          "valid transition retain state %d with input %d", 
                          currState, input );
          /* Do nothing because previous and current states are same */
          break;

        /* Returning to previous state */
        case RFGSM_PREV_STATE_VAL:
          /* Print message to log the successful transition */
          RFGSM_SM_MSG_3( MSG_LEGACY_HIGH, 
                          "valid transition return to previous state %d with input %d from state %d ", 
                          prevState, input, currState );

          /* Swap previous state with current state */
          /*@TODO possible use of intrinsic here to ensure register used and */
          /*      remove requirement for prevStateSaved member variable.   */
          prevStateSaved = prevState;
          prevState = currState;
          currState = prevStateSaved;
          break;

        /* Attempted transition is invalid */
        case RFGSM_INVALID_STATE_VAL:
          /* Print an error here and do nothing as transition is invalid */
          RFGSM_SM_MSG_2( MSG_LEGACY_ERROR, 
                          "invalid transition input %d with from state %d", 
                          input, currState );
          break;
      }
    }

  private:
    /* State the state machine is currently in */
    STATE_TYPE currState;
	
    /* State the state machine was previously in */
    STATE_TYPE prevState;
	
    /* Temporary previous state to enable state reverting and logging */
    STATE_TYPE prevStateSaved;
	
    /* Store an ID/Description of state machine */
    RfgsmSmidType smId;
	
    /* Store the index i.e rfm device or sub ID */
    uint32 smIdx;
};
#else
/* Need this for compilation if included in non-c++ context */
struct rfgsm_compilaton_type;
typedef struct rfgsm_compilaton_type CRfgsmStateMachine;
#endif /* #ifdef __cplusplus */


#endif /* RFGSM_CORE_STATE_MACHINE_H */

