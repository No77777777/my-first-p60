#ifndef HAL_WCSS_DBG_CSR_H
#define HAL_WCSS_DBG_CSR_H

/*=============================================================================

FILE:         wcss_dbg_csr.h

DESCRIPTION:  

================================================================================
            Copyright (c) 2015 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/
#include "HALcomdef.h"

/*-------------------------------------------------------------------------*/

/**
  @brief Initialized the WCSS CSR block.
         Called internally by qdss_control

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_init(void);


#define WCSSDBG_CMB_OWNER_ARB    0x0 
#define WCSSDBG_CMB_OWNER_TRACER 0x1
#define WCSSDBG_CMB_OWNER_APBMON 0x2
#define WCSSDBG_CMB_OWNER_SWGEN  0x3

/*-------------------------------------------------------------------------*/

/**
  @brief  Sets the owner of CMB data
   
  @param                  val [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_cmb_owner(uint8 val);


/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param                  val [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_mem_layout(uint8 val);



#define WCSSDBG_MEM_LAYOUT_ETB    0
#define WCSSDBG_MEM_LAYOUT_TRACER 1


/*-------------------------------------------------------------------------*/

/**
  @brief  Type definition for callback function to be called before
          mem layout is changed.
  
  @param  [in ] :  new_mem_layout - New mem layout.

 */
typedef void (* wcss_dbg_mem_layout_change_handler)(uint8 new_mem_layout);

/*-------------------------------------------------------------------------*/

/**
  @brief  This function allows registration of a callback function that
          will be called any time wcss_dbg mem_layout is changed. 
  
  @param              handler [in ] :  Callback function to be called
                                       The argument to the new function

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_register_mem_handler(wcss_dbg_mem_layout_change_handler handler);


#define WCSSDBG_MEM_OWNER_ETB    0
#define WCSSDBG_MEM_OWNER_TRACER 1

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param                  val [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_mem_owner(uint8 val);

#define WCSSDBGCSR_TSTMP_SOURCE_QDSS     0
#define WCSSDBGCSR_TSTMP_SOURCE_WCSSDBG  1
#define WCSSDBGCSR_TSTMP_SOURCE_AUTO     2



/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param               source [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_timestamp_source(uint8 source);


/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param              bEnable [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_sleep_correction(uint8 bEnable);

/*-------------------------------------------------------------------------*/

/**
  @brief             Sets the chip XO frequency to help with sleep correction
  
  @param             freq_khz [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_cxo_freq(uint32 freq_khz);
/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param          events_mask [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_GenDsbEvents(uint32 events_mask);

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param            event_num [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_GenDsbEvent(uint8 event_num);

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param           match_ctrl [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_apbmon_match_ctrl(uint32 match_ctrl);

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param        match_details [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_apbmon_match_details(uint32 match_details);

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param              latency [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_apbmon_latency(uint32 latency);

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param                 addr [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_apbmon_match_addr_low(uint32 addr);

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param                 addr [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_apbmon_match_addr_high(uint32 addr);

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param                state [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_apbmon_cmb_trace(uint8 state);

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param                state [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_csr_set_apbmon_cmb_data(uint8 state);

/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param              bEnable [in ] :  

  @return 0 if successful, error code otherwise
 */
int wcss_dbg_tsgen_set_ts_counter(uint8 bEnable);


/*-------------------------------------------------------------------------*/

/**
  @brief 
  
  @param               offset [in ] :  
  @param                value [in ] :  

  @return 0 if successful, error code otherwise
 */
int  wcss_dbg_gen_cmb(uint32 offset,uint32 value);


#define WCSS_DBG_CSR_SET_CMB_OWNER               0
#define WCSS_DBG_CSR_SET_MEM_LAYOUT              1
#define WCSS_DBG_CSR_SET_MEM_OWNER               2
#define WCSS_DBG_CSR_SET_TIMESTAMP_SOURCE        3
#define WCSS_DBG_CSR_SET_SLEEP_CORRECTION        4
#define WCSS_DBG_CSR_SET_APBMON_MATCH_CTRL       5
#define WCSS_DBG_CSR_SET_APBMON_MATCH_DETAILS    6
#define WCSS_DBG_CSR_SET_APBMON_LATENCY          7
#define WCSS_DBG_CSR_SET_APBMON_MATCH_ADDR_LOW   8
#define WCSS_DBG_CSR_SET_APBMON_MATCH_ADDR_HIGH  9
#define WCSS_DBG_CSR_SET_APBMON_CMB_TRACE        10
#define WCSS_DBG_CSR_SET_APBMON_CMB_DATA         11
#define WCSS_DBG_TSGEN_SET_TS_COUNTER            12

#endif //HAL_WCSS_DBG_CSR_H
