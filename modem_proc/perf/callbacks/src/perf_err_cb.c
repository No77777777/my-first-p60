/*==========================================================================
 * FILE:         per_err_cb.c
 *
 * DESCRIPTION:  This file provides a list implementation of pre_STM callbacks
 *               
 *
 * Copyright (c) 2015 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
=============================================================================*/


/*===========================================================================

            EDIT HISTORY FOR MODULE

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/26/15   rr      Created file
06/22/16   rr      Added callback function to register err_cb during the 
                   pre_STM callback
===========================================================================*/
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "err.h"
#include "perf_err_cb.h"
#include "atomic_ops.h"

#define MAX_PRE_STM_CBS 16
#define MAX_ERR_CB 4

extern void dog_force_kick( void );

perf_err_cb_ptr_u32 perf_pre_STM_cb_arr[MAX_PRE_STM_CBS];
perf_err_cb_ptr perf_err_crash_cb_register[MAX_ERR_CB];
perf_err_cb_ptr_u32 perf_err_crash_cb_register_u32[MAX_ERR_CB];
perf_err_cb_ptr_u32 perf_err_crash_cb_register_u32_late_api;
perf_err_cb_ptr internal_perf_err_crash_cb_register[MAX_ERR_CB];
/*check if functions where registered into pre-flush successfully*/
boolean perf_err_crash_cb_register_result[MAX_ERR_CB];
boolean internal_perf_err_crash_cb_register_result[MAX_ERR_CB];
err_cb_error_t perf_err_crash_cb_register_result_u32[MAX_ERR_CB];


void perf_err_call_pre_STM_CB(uint32_t tid)
{
    
   int ii;
   err_cb_info_t cb_info;
   err_cb_error_t rv = ERRCB_E_FAILURE;
   /*First register err_callbacks that happen after STM*/
   for(ii = 0; ii < MAX_ERR_CB; ii++)
   {
      if(internal_perf_err_crash_cb_register[ii] != NULL)
      {
        internal_perf_err_crash_cb_register_result[ii] = err_crash_cb_register(internal_perf_err_crash_cb_register[ii]);
      }
   }
   
   for(ii = 0; ii < MAX_ERR_CB; ii++)
   {
      if(perf_err_crash_cb_register[ii] != NULL)
      {
        perf_err_crash_cb_register_result[ii] = err_crash_cb_register(perf_err_crash_cb_register[ii]);
      }
   }
   
   for(ii = 0; ii < MAX_ERR_CB; ii++)
   {
      if(perf_err_crash_cb_register_u32[ii] != NULL)
      {
        cb_info.type = ERRCB_TYPE_UINT32 ;
        cb_info.err_cb.cb_u32 = perf_err_crash_cb_register_u32[ii];
        if (perf_err_crash_cb_register_u32_late_api==perf_err_crash_cb_register_u32[ii])
		{
			cb_info.bucket = ERRCB_BUCKET_LATE;
		}
		else
		{
        cb_info.bucket = ERRCB_BUCKET_NORMAL;
		}
        cb_info.order = ERRCB_ORDER_NORMAL;
        perf_err_crash_cb_register_result_u32[ii] = err_cb_enable(&cb_info);
      }
   }
   
   for (ii = 0; ii < MAX_PRE_STM_CBS; ii++)
   {
      if(perf_pre_STM_cb_arr[ii] != NULL){
	    dog_force_kick();
		perf_pre_STM_cb_arr[ii](tid);
	  }
   }
}
void perf_pre_STM_init(void)
{
  boolean status;
  status = err_crash_cb_reg_pre_STM(perf_err_call_pre_STM_CB);
}

int perf_err_crash_cb_reg_pre_STM(perf_err_cb_ptr_u32 cb)
{
  int ii;
  boolean ret = 0;
  for(ii = 0; ii < MAX_PRE_STM_CBS; ii++)
  {
    if(perf_pre_STM_cb_arr[ii] == NULL)
    {
      if (atomic_compare_and_set(&perf_pre_STM_cb_arr[ii],0, cb))
      {
        ret = 1;
        break;
      }
    }
  }
  return ret;
}

int perf_crash_cb_dereg_pre_STM(perf_err_cb_ptr_u32 cb)
{
  int ii;
  boolean ret =0;
  for(ii = 0; ii < MAX_PRE_STM_CBS; ii++)
  {
    if(perf_pre_STM_cb_arr[ii] == cb)
    {
      if (atomic_compare_and_set(&perf_pre_STM_cb_arr[ii],cb, 0))
      {
        ret = 1;
        break;
      }
    }
  }
  return ret;
}

/*Store functions to register into err_cb_register during the pre_STM callback*/
int internal_perf_err_cb_register_in_pre_STM(perf_err_cb_ptr cb)
{
  int ii;
  boolean ret = 0;
  for(ii = 0; ii < MAX_ERR_CB; ii++)
  {
    
    if (internal_perf_err_crash_cb_register[ii] == NULL)
    {
      if (atomic_compare_and_set(&internal_perf_err_crash_cb_register[ii],0, cb))
      {
        ret = 1;
        break;
      }
    }
  }
  return ret;
}

/*Remove functions from buffer that will contain function to be registered in
pre_STM callback*/
int internal_perf_err_cb_dereg_in_pre_STM(perf_err_cb_ptr cb)
{
  int ii;
  boolean ret = 0;
  for(ii = 0; ii < MAX_ERR_CB; ii++ )
  {
    if(internal_perf_err_crash_cb_register[ii] == cb)
    {
      if (atomic_compare_and_set(&internal_perf_err_crash_cb_register[ii],cb, 0))
      {
        ret = 1;
        break;
      }
    }
  }
  return ret;
}

/*Store functions to register into err_cb_register during the pre_STM callback*/
int perf_err_cb_register_in_pre_STM(perf_err_cb_ptr cb)
{
  int ii;
  boolean ret = 0;
  for(ii = 0; ii < MAX_ERR_CB; ii++)
  {
    
    if (perf_err_crash_cb_register[ii] == NULL)
    {
      if (atomic_compare_and_set(&perf_err_crash_cb_register[ii],0, cb))
      {
        ret = 1;
        break;
      }
    }
  }
  return ret;
}

/*Remove functions from buffer that will contain function to be registered in
pre_STM callback*/
int perf_err_cb_dereg_in_pre_STM(perf_err_cb_ptr cb)
{
  int ii;
  boolean ret = 0;
  for(ii = 0; ii < MAX_ERR_CB; ii++ )
  {
    if(perf_err_crash_cb_register[ii] == cb)
    {
      if (atomic_compare_and_set(&perf_err_crash_cb_register[ii],cb, 0))
      {
        ret = 1;
        break;
      }
    }
  }
  return ret;
}

int perf_err_cb_register_in_pre_STM_u32(perf_err_cb_ptr_u32 cb)
{
  int ii;
  boolean ret = 0;
  for(ii = 0; ii < MAX_ERR_CB; ii++)
  {
    
    if (perf_err_crash_cb_register_u32[ii] == NULL)
    {
      if (atomic_compare_and_set(&perf_err_crash_cb_register_u32[ii],0, cb))
      {
        ret = 1;
        break;
      }
    }
  }
  return ret;
}

int perf_err_cb_register_in_pre_STM_u32_late(perf_err_cb_ptr_u32 cb)
{
  int ii;
  boolean ret = 0;
  for(ii = 0; ii < MAX_ERR_CB; ii++)
  {
    
    if (perf_err_crash_cb_register_u32[ii] == NULL)
    {
      if (atomic_compare_and_set(&perf_err_crash_cb_register_u32[ii],0, cb))
      {
        ret = 1;
        break;
      }
    }
  }
  perf_err_crash_cb_register_u32_late_api=cb;
  return ret;
}





int perf_err_cb_dereg_in_pre_STM_u32(perf_err_cb_ptr_u32 cb)
{
  int ii;
  boolean ret = 0;
  for(ii = 0; ii < MAX_ERR_CB; ii++ )
  {
    if(perf_err_crash_cb_register_u32[ii] == cb)
    {
      if (atomic_compare_and_set(&perf_err_crash_cb_register_u32[ii],cb, 0))
      {
        ret = 1;
        break;
      }
    }
  }
  return ret;
}
