/*!
  @file
  services.h

  @brief
  Declares interfaces for general services 

*/
/*===========================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/src/services.h#1 $
==============================================================================*/

#ifndef SERVICES_H_
#define SERVICES_H_

#include "global_defs.h"

//SERVICES_INVALID_ADDRESS returned in case address is invalid
#define SERVICES_INVALID_ADDRESS    0xFFFFFFFF

//user flags defining the SW behavior
extern uint32_t gUserFlags;

#ifndef UC_FEATURE_CPU_PROFILING_IRQ_HDLR_DISABLED
extern uint64_t irq_cpu_usage_time = 0;
#endif

#define IPA_UC_CPU_PROFILING_TIMER_START_VAL 0xFFFF
#define IPA_UC_CPU_PROFILING_TIMER_RESET_VAL 0

typedef enum
{
	SERVICES_ADDRESS_TYPE_DATA,         //address resides in data pool
	SERVICES_ADDRESS_TYPE_DESCRIPTOR,   //address resides in descriptors pool
	SERVICES_ADDRESS_TYPE_COUNT         //number of address pools
}SERVICES_ADDRESS_TYPE;

//#pragma pack(push,1) /*Save previous, and turn on 1 byte alignment*/

typedef struct ServicesAddressInfo_t
{
   uint64_t pciHostBaseAddress;       //pool base address in host address space
   uint32_t pciDeviceBaseAddress;     //pool base address in device address space
   uint32_t pciRangeInBytes;          //pool size in bytes
}ServicesAddressInfo_t;

//#pragma pack(pop) /* Revert alignment to what it was previously*/

/**
 * @brief   Initialize address conversion service
 *
 * @param [in]   ranges   If non-null, the ranges. 
*/
void SERVICES_InitAddressConversion(const ServicesAddressInfo_t* ranges);

/**
 * @brief   Convert host to device address service.
 *
 * @param   hostAddress          Address in host address space. 
 * @param   type                 The type of address. 
 * @param   checkIfAddressValid  TRUE - verify if address is valid. FALSE - skip address checking
 *
 * @return  Address in device address space. 
*/
uint32_t SERVICES_ConvertHostToDeivceAddress(uint64_t hostAddress, SERVICES_ADDRESS_TYPE type, uint8_t checkIfAddressValid);

/**

 * @brief   Convert device to host address service. 
 *
 * @param   deviceAddress        Address in device address space. 
 * @param   type                 The type of address. 
 * @param   checkIfAddressValid  The check if address valid. 
 *
 * @return  Address in host address space. 
*/
uint64_t SERVICES_ConvertDeviceToHostAddress(uint32_t deviceAddress, SERVICES_ADDRESS_TYPE type, uint8_t checkIfAddressValid);

/**
 * @brief   Round robin arbiter for bit request.
 *
 * @param   request        The request bitmask.
 * @param   highestPrioBit The highest priority bit (next bit to serve).
 *
 * @return  the index of the next bit to serve.
*/
uint32_t SERVICES_RoundRobinArbiter(uint32_t request, uint32_t highestPrioBit);

/**
 * @brief   Get the service Database
 *
 * @param   type        Type of services Data Base (Data/Desc)
 *
 * @return  Pointer to the corresponding services database
*/
ServicesAddressInfo_t* SERVICES_GetDB(SERVICES_ADDRESS_TYPE type);

#ifndef UC_FEATURE_CPU_PROFILING_IRQ_HDLR_DISABLED
/**
 * @brief   Function to start the countdown timer if we were 
 *          wfi. This function is only used to profile interrupt
 *          handlers at the moment.
 *
 * @param   None
 *
 * @return  None (or) FALSE if we were not in wfi. TRUE if we 
 *          were in wfi
*/
void SERVICES_StartCountdownTimer();

/**
 * @brief   Function to calculate the CPU usage of an interrupt 
 *          while we were in wfi. This function is only used to
 *          profile interrupt handlers at the moment.
 *
 * @param   None
 *
 * @return  None
*/
void SERVICES_CalcCpuUsage();
#endif //UC_FEATURE_CPU_PROFILING_IRQ_HDLR_DISABLED

/**
 * @brief   Memory Clear 
 *  
 * @param   addr        Address to be cleared
 * @param   numBytes    Number of bytes to clear 
*/
void MemClear(uint32_t addr, uint32_t numBytes);

/**
 * @brief   Function to perform number of NOPs requested
 *
 * @param   numNOPs  The number of NOPs to perform
 *
 * @return  None
*/
void uCBusySpin(uint32_t numNOPs);

uint8_t MemCmp(uint32_t src, uint32_t dest,uint8_t size);

void MemWcopy(uint32_t *src, uint32_t *dest,uint8_t size);
#endif /* SERVICES_H_ */
