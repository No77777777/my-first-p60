#ifndef _GPIOINTINTERFACE_H_
#define _GPIOINTINTERFACE_H_
/*===========================================================================

  GPIOInt Settings   

DESCRIPTION
  This file contains enums, constants and structure defintions required
  for the GPIOInt settings interface

REFERENCES

       Copyright © 2016 Qualcomm Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/GPIOInt/interface/GPIOIntInterface.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/24/16   jg      Initial version
===========================================================================*/

#include "DALStdDef.h"
#include "DDIGPIOInt.h"
#include "qurt_rmutex.h"

#define GPIOINTF_PROXY                        0x0001
#define GPIOINTF_MPMINT                       0x0002
#define GPIOINTF_WAKEUP                       0x0004
#define GPIOINTF_REGISTERED                   0x0008
#define GPIOINTF_EXTERNAL_CONFIG              0x0010 /** Reserved for external entity */
#define GPIOINTF_UGPIOINT_CONFIG              0x0020 /** Reserved for uGPIOInt */
#define GPIOINTF_DIRECTINT_CONFIG             0x0040 /** Direct line to IntC */

#define GPIOINTF_TRIGGER_LEVEL_HIGH           0x0080
#define GPIOINTF_TRIGGER_LEVEL_LOW            0x0100
#define GPIOINTF_TRIGGER_EDGE_RISING          0x0200
#define GPIOINTF_TRIGGER_EDGE_FALLING         0x0400
#define GPIOINTF_TRIGGER_EDGE_DUAL            0x0800

#define GPIOINT_DEFAULT_PRIORITY              4

/* 
 * GPIOIntInterruptType 
 *  
 * This enum is used to define types of GPIO interrupts direct 
 * connection versus summary. 
 */
typedef enum
{
  GPIOINT_DIRECTCONNECT_0  =  0,  
  GPIOINT_DIRECTCONNECT_1  =  1, 
  GPIOINT_DIRECTCONNECT_2  =  2, 
  GPIOINT_DIRECTCONNECT_3  =  3, 
  GPIOINT_DIRECTCONNECT_4  =  4, 
  GPIOINT_DIRECTCONNECT_5  =  5, 
  GPIOINT_DIRECTCONNECT_6  =  6, 
  GPIOINT_DIRECTCONNECT_7  =  7, 
  GPIOINT_DIRECTCONNECT_8  =  8, 
  GPIOINT_DIRECTCONNECT_9  =  9, 
  GPIOINT_DIRECTCONNECT_10 = 10, 
  GPIOINT_DIRECTCONNECT_11 = 11, 
  GPIOINT_DIRECTCONNECT_12 = 12, 
  GPIOINT_DIRECTCONNECT_13 = 13, 
  GPIOINT_DIRECTCONNECT_14 = 14, 
  GPIOINT_DIRECTCONNECT_15 = 15, 
  GPIOINT_SPECIALCONN      = 0x7D,
  GPIOINT_SUMMARY          = 0x7E,
  GPIOINT_DEFAULT          = 0x7F,
} GPIOIntInterruptType;

//HVM:ToDo: Remove GPIOIntProcessorType from DDIGPIOInt.H and reinstate this enum definition
#if 0
/*
 * GPIOIntProcessorType 
 * 
 * This enum is used to define the destination of the GPIO interrupt i.e. the receiving processor for the incoming interrupt.
 * In multiprocessor subsystem the Top level mode Mux pins or general purpose IO pins (GPIO)
 * are shared between processors. Hence it becomes necessary to define their destination processor.
 */
typedef enum{
  GPIOINT_DEVICE_MODEM, /**< The Modem device is usually the QDSP6 processor. */
  GPIOINT_DEVICE_SPS,    /**< The SPS device usually is the Sensors processor. */
  GPIOINT_DEVICE_LPA_DSP,  /**< The LPA DSP device is usually the LPASS processor */
  GPIOINT_DEVICE_RPM,  /**< The RPM device usually is the resource power manager processor */
  GPIOINT_DEVICE_APPS, /**< The APPS device usually is the applications processor */
  GPIOINT_DEVICE_WCN, /**< The WCN device usually is the RIVA processor */
  GPIOINT_DEVICE_DSP, /**< On legacy 9k devices the DSP is the modem processor */
  GPIOINT_DEVICE_TURING, /**< Turing processor for compute */
  GPIOINT_DEVICE_NONE, /**< The default target processor value. */
  PLACEHOLDER_GPIOIntProcessorType = 0x7fffffff
} GPIOIntProcessorType;
#endif

/*
 * GPIOIntConfigMapType
 * Structure storing the interrupt data map accross all processors.
 * uint32          gpio;                     GPIO pin number
 * uint32          interrupt_id; l1 main interrupt line for direct connects.
 * uint32          polarity;                 polarity of the interrupt.
 */
typedef struct
{
  uint32                                 gpio;
  GPIOIntInterruptType                   interrupt;
  uint32                                 interrupt_id;
  uint32                                 flags;
} GPIOIntConfigMapType;


/*
 * GPIOIntr_CfgType
 *
 * <InterruptType, InterruptID> Tuple config data Structure to be passed
 * through to respective GPIOInt lower-level drivers to populate.
 */
typedef struct
{
  uint32                         nInterruptType;
  uint32                         nInterruptID;
  GPIOIntTriggerType             eIntrTrigger;
} GpioIntr_CfgType;


/*
 * GpioIntr_Attribute
 */ 
typedef struct
{
  GPIOIntTriggerType eTrigger;
  uint32             nPriority;
  uint32             nUInterrupt;
  uint32             nflags;
  uint32             mpm_intr_id;
} GpioIntr_Attribute;


/*
 * GPIOIntr_Initdatatype
 * Structure storing the details for init data
 */
typedef struct
{
  qurt_mutex_t*      pLock;
  DalDeviceHandle *  hDalMpmHandle;
} GPIOIntr_InitDataType;

/*
 * GPIOIntFnDescType
 *
 * Internal GPIOInt function pointer structure
 */
typedef DALResult ( *GPIOIntFn_InitType ) ( GPIOIntr_InitDataType *GPIOIntr_InitData );
typedef DALResult ( *GPIOIntFn_DeInitType ) ( void );
typedef DALResult ( *GPIOIntFn_IsSupportedType ) ( uint32 nGpio, GpioIntr_Attribute *pAttr );
typedef DALResult ( *GPIOIntFn_ConfigType ) ( uint32 nGpio, GpioIntr_Attribute *pAttr, GpioIntr_CfgType *pIntrCfgData );
typedef DALResult ( *GPIOIntFn_DeconfigType ) ( uint32 nGpio, GpioIntr_CfgType *pIntrCfgData );
typedef DALResult ( *GPIOIntFn_RegisterType ) ( uint32 nGpio, void *pfnIsr, void *nArg );
typedef DALResult ( *GPIOIntFn_DeregisterType ) ( uint32 nGpio );
typedef DALResult ( *GPIOIntFn_EnableType ) ( uint32 nGpio );
typedef DALResult ( *GPIOIntFn_DisableType ) ( uint32 nGpio );
typedef DALResult ( *GPIOIntFn_SetTriggerType ) ( uint32 nGpio, uint32 nNewTrigger );
typedef DALResult ( *GPIOIntFn_TriggerInterruptType ) ( uint32 nGpio );
typedef DALResult ( *GPIOIntFn_ClearInterruptType ) ( uint32 nGpio );
typedef DALResult ( *GPIOIntFn_IsInterruptSetType ) ( uint32* state, uint32 nGpio );
typedef DALResult ( *GPIOIntFn_IsInterruptPendingType ) ( uint32* state, uint32 nGpio );
typedef DALResult ( *GPIOIntFn_IsInterruptEnabledType ) ( uint32* state, uint32 nGpio );
typedef DALResult ( *GPIOIntFn_DecrementNonMPMInterruptsType ) ( uint32 nGpio );

typedef struct 
{
  GPIOIntFn_InitType               GPIOIntFn_Init;
  GPIOIntFn_DeInitType             GPIOIntFn_DeInit;
  GPIOIntFn_IsSupportedType        GPIOIntFn_IsSupported;
  GPIOIntFn_ConfigType             GPIOIntFn_Config;
  GPIOIntFn_DeconfigType           GPIOIntFn_Deconfig;
  GPIOIntFn_RegisterType           GPIOIntFn_Register;
  GPIOIntFn_DeregisterType         GPIOIntFn_Deregister;
  GPIOIntFn_EnableType             GPIOIntFn_Enable;
  GPIOIntFn_DisableType            GPIOIntFn_Disable;
  GPIOIntFn_SetTriggerType         GPIOIntFn_SetTrigger;
  GPIOIntFn_TriggerInterruptType   GPIOIntFn_TriggerInterrupt;
  GPIOIntFn_ClearInterruptType     GPIOIntFn_ClearInterrupt;
  GPIOIntFn_IsInterruptSetType     GPIOIntFn_IsInterruptSet;
  GPIOIntFn_IsInterruptPendingType GPIOIntFn_IsInterruptPending;
  GPIOIntFn_IsInterruptEnabledType GPIOIntFn_IsInterruptEnabled;
  GPIOIntFn_DecrementNonMPMInterruptsType GPIOIntFn_DecrementNonMPMInterrupts; 

} GPIOIntFnDescType;

extern GPIOIntFnDescType PdcGpio_Procs;
extern GPIOIntFnDescType DirConnGpio_Procs;
extern GPIOIntFnDescType SummaryGpio_Procs;

#endif /* _GPIOINTINTERFACE_H_ */
