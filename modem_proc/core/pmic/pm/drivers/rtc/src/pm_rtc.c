/*! \file
*  
*  \brief  mpp_app.cpp ----File contains the implementation of the public APIs for RTC resource type.
*  \details Implementation file for RTC resourece type.
*  Each of the APIs checks for access and then if necessary directs
*  the call to Driver implementation or to RPC function for the master processor.
*  
*    PMIC code generation Version: 2.0.0.22
*    This file contains code for Target specific settings and modes.
*  
*  &copy; Copyright 2010 Qualcomm Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/rtc/src/pm_rtc.c#1 $ 
when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/10/12   hs      Changed the type for internalResourceIndex from int to uint8.
11/15/11   hs      Added support for pm_hard_reset() per CR316544

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_rtc_driver.h"
#include "pm_comm.h"
#include "pm_ulog.h"
/*===========================================================================

                        Definitions

===========================================================================*/

#define BASE_ADDR_RTC (rtc_ptr->rtc_reg->base_address)

#define REG_RTC(reg) ((rtc_ptr->rtc_reg->base_address) \
                      + (rtc_ptr->rtc_reg->reg))

#define SLAVE_ID_RTC (rtc_ptr->comm_ptr->slave_id)

//Size of RTC registers in bytes 
#define SIZE_RTC_COUNTER 4

#define GET_RTC_ERR_CODE(ptr)  ptr==NULL ?                           \
                               PM_ERR_FLAG__FEATURE_NOT_SUPPORTED :  \
                               PM_ERR_FLAG__INPUT_INVALID          
/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
pm_err_flag_type pm_rtc_start(uint8 pmic_chip, 
                              const pm_hal_rtc_time_type* start_time_ptr)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG__SUCCESS;
    pm_rtc_data_type      *rtc_ptr  = pm_rtc_get_data(pmic_chip);
    pm_register_data_type wdata_arr[SIZE_RTC_COUNTER] = {0,0,0,0};
    
    //Input Err Check
    if((rtc_ptr == NULL) || (start_time_ptr == NULL)) 
    {   
        errFlag = GET_RTC_ERR_CODE(rtc_ptr);
        PM_LOG_MSG_ERROR(errFlag, "error = %d, pmic=%d, time_ptr=0x%x", 
                         errFlag, pmic_chip, start_time_ptr);
        return errFlag;
    }
    
    // clear bit 7 of the SEC_RTC_CNTRL register to disable RTC 
    // This means that every time a RtcStart API is called. 
    // It will STOP RTC first before it START RTC
    errFlag |= pm_comm_write_byte_mask(SLAVE_ID_RTC, REG_RTC(en_ctl), 
                                       0x80, 0, 0);

    //Convert time data to rtc wdata
    pm_rtc_time_32_bits_to_rtc_counter_val(start_time_ptr->sec, wdata_arr);

    //Write data to wdata 0
    errFlag |= pm_comm_write_byte_array(SLAVE_ID_RTC, REG_RTC(wdata0), 
                                        SIZE_RTC_COUNTER, wdata_arr, 0);

    //set bit 7 of the SEC_RTC_CNTRL register to enable RTC
    errFlag |= pm_comm_write_byte_mask(SLAVE_ID_RTC, REG_RTC(en_ctl), 
                                       0x80, 0x80, 0);
    
    PM_LOG_MSG_ERROR(errFlag, "error = %d", errFlag);
    
    return errFlag;
}

pm_err_flag_type pm_rtc_stop(uint8 pmic_chip)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG__SUCCESS;
    pm_rtc_data_type *rtc_ptr = pm_rtc_get_data(pmic_chip);
    
    if(rtc_ptr == NULL)
    {
        errFlag = GET_RTC_ERR_CODE(rtc_ptr);        
        PM_LOG_MSG_ERROR(errFlag, "error = %d", errFlag);
        return errFlag;
    }
    
    errFlag = pm_comm_write_byte_mask(SLAVE_ID_RTC, REG_RTC(en_ctl), 
                                      0x80, 0, 0);

    PM_LOG_MSG_ERROR(errFlag, "error = %d", errFlag);

    return errFlag;
}

pm_err_flag_type pm_dev_hal_rtc_get_time(uint8 pmic_chip, 
                                         pm_hal_rtc_time_type  *time_ptr)
{
    pm_err_flag_type      errFlag  = PM_ERR_FLAG__SUCCESS;
    pm_rtc_data_type      *rtc_ptr = pm_rtc_get_data(pmic_chip);
    pm_register_data_type rtcONOFF = 0;
    pm_register_data_type rdata_arr1[SIZE_RTC_COUNTER] = {0,0,0,0};
    pm_register_data_type rdata_arr2[SIZE_RTC_COUNTER] = {0,0,0,0};
    uint32 time1 = 0, time2 = 0;
    uint8 num_reads = 10;
    
    //Input Error Check     
    if ((rtc_ptr == NULL) || (time_ptr == NULL)) 
    {
        errFlag = GET_RTC_ERR_CODE(rtc_ptr);          
        PM_LOG_MSG_ERROR(errFlag, "error = %d, pmic=%d, time_ptr=0x%x", 
                         errFlag, pmic_chip, time_ptr);
        return errFlag;
    }
    
    // Check if RTC HW is currently running (enabled) by looking 
    // at RTC_EN bit of SEC_RTC_CNTRL register.
    errFlag |= pm_comm_read_byte_mask(SLAVE_ID_RTC, REG_RTC(en_ctl), 
                                     0x80 , &rtcONOFF,0);
    
    //If input errors or RTC disabled, return error    
    if ((errFlag != PM_ERR_FLAG__SUCCESS) || (!(rtcONOFF & 0x80)))
    {   
        if(!(rtcONOFF & 0x80))
            errFlag =  PM_ERR_FLAG__RTC_HALTED;
        
        PM_LOG_MSG_ERROR(errFlag, "error = %d", errFlag);
        
        return errFlag;
    }
    
    //Read rdata registers
    errFlag |= pm_comm_read_byte_array(SLAVE_ID_RTC, REG_RTC(rdata0), 
                                SIZE_RTC_COUNTER, rdata_arr1, 0);
                                
    time1 = pm_rtc_counter_val_to_time_32_bits(rdata_arr1);
    
    do
    {
        errFlag |= pm_comm_read_byte_array(SLAVE_ID_RTC, REG_RTC(rdata0),
                                SIZE_RTC_COUNTER, rdata_arr2, 0);                       
        time2 = pm_rtc_counter_val_to_time_32_bits(rdata_arr2);
        
        if(time1 == time2)
            break; 
        
        time1 = time2;
        time2 = 0;
        num_reads--;
        
    }while(num_reads);
    
    if(num_reads == 0)
    {
        errFlag = PM_ERR_FLAG__RTC_READ_FAILED;
        time_ptr->sec = 0;
    }
    else 
    {
        time_ptr->sec = time2;
    }
    
    PM_LOG_MSG_ERROR(errFlag, "error = %d", errFlag);

    return errFlag;
}

pm_err_flag_type pm_rtc_set_time_adjust(uint8 pmic_chip, uint8  time_adjust)
{
    pm_err_flag_type    errFlag  = PM_ERR_FLAG__SUCCESS;
    pm_rtc_data_type    *rtc_ptr = pm_rtc_get_data(pmic_chip);
    
    if ((rtc_ptr == NULL) || (time_adjust > 0x7F))
    {
        errFlag = GET_RTC_ERR_CODE(rtc_ptr);
        PM_LOG_MSG_ERROR(errFlag, "error = %d, pmic=%d, time_adjust=0x%x", 
                         errFlag, pmic_chip, time_adjust);

        return errFlag;
    }
        
    errFlag = pm_comm_write_byte_mask(SLAVE_ID_RTC, REG_RTC(time_adj),
                                      0x7F, time_adjust, 0 );
    
    PM_LOG_MSG_ERROR(errFlag, "error = %d", errFlag);
    
    return errFlag;
}

pm_err_flag_type pm_rtc_get_time_adjust(uint8 pmic_chip, 
                                        uint8  *time_adjust_ptr)
{
    pm_err_flag_type      errFlag   = PM_ERR_FLAG__SUCCESS;
    pm_rtc_data_type *rtc_ptr = pm_rtc_get_data(pmic_chip);

    if ((rtc_ptr == NULL) || (time_adjust_ptr==NULL))
    {
        errFlag = GET_RTC_ERR_CODE(rtc_ptr);
        PM_LOG_MSG_ERROR(errFlag, "error = %d, pmic=%d, time_ptr=0x%x", 
                         errFlag, pmic_chip, time_adjust_ptr);
        return errFlag;
    }
    
    // TODO: Revist this API to fix input parameter "time adjust". This will affect CMI. 
    // time_adjust should get changed from raw data to doc values. 
    // Otherwise 0x7F needs to be defined in strucuture for cleaner code 
    // Unless.. is there a reason why we return the data raw? without using the given equation? 
    // TODO: Also bit<7> is reserved and not used for time adjust, but from 7x30, 
    // it is included. Is there a reason why? 
    errFlag = pm_comm_read_byte_mask(SLAVE_ID_RTC, REG_RTC(time_adj), 
                                     0x7F,  time_adjust_ptr, 0);
    
    PM_LOG_MSG_ERROR(errFlag, "error = %d", errFlag);
    
    return errFlag;
}



/*===========================================================================

                        INTERNAL FUNCTION DEFINITIONS

===========================================================================*/
void pm_rtc_time_32_bits_to_rtc_counter_val(uint32  sec_32_bits, 
                                            pm_register_data_type *arr)
{
   arr[3]=(pm_register_data_type)sec_32_bits>>PM_RTC_VAL3_SHIFT;
   arr[2]=(pm_register_data_type)sec_32_bits>>PM_RTC_VAL2_SHIFT;
   arr[1]=(pm_register_data_type)sec_32_bits>>PM_RTC_VAL1_SHIFT;
   arr[0]=(pm_register_data_type)sec_32_bits>>PM_RTC_VAL0_SHIFT;
   
    return;
}


uint32 pm_rtc_counter_val_to_time_32_bits(pm_register_data_type *t)
{
    uint32 time = 0 ;   
    
    time = (((uint32)t[3])<<PM_RTC_VAL3_SHIFT) | 
           (((uint32)t[2])<<PM_RTC_VAL2_SHIFT) | 
           (((uint32)t[1])<<PM_RTC_VAL1_SHIFT) | 
           (((uint32)t[0])<<PM_RTC_VAL0_SHIFT);
    
    return time ;
}


/*===========================================================================

                        DEPRECATED FUNCTIONS

===========================================================================*/

pm_err_flag_type pm_hal_rtc_get_time(pm_hal_rtc_time_type *time_ptr)
{
    return pm_dev_hal_rtc_get_time(0, time_ptr);
}


