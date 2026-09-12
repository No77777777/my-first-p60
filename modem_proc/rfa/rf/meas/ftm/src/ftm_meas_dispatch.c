/*----------------------------------------------------------------------------*/
/*!
                      
  @brief
  This is the FTM RF file which contains RF specific commands

Copyright (c) 2009 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rf/meas/ftm/src/ftm_meas_dispatch.c#1 $ 
  $DateTime: 2021/03/09 05:11:02 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/29/15   nv      Added NULL pointer check as a part of KW fix
05/20/15   nv      Added changes to get RXAGC value during FTM IRAT testing
04/28/15   nv      Mainline the code under feature RF_HAS_MEAS_INTF_TH_2_0
04/17/15   nv      Added changes for Half scripts and NULL2X scripts
04/16/15   rmb     Fix Compilation by removing reference on undefined FTM command.
04/07/15   nv      Merging changes from Dev branch onto Mainline with feature flag
04/06/15   rmb     Merge back meas interface changes onto Mainline with feature flag  
12/12/13   rmb     Add FTM commands for W2W IRAT.
08/13/13   pl      Fix KW warnings 
08/13/13   pl      Update FTM IRAT response packet to include script info
06/24/13   sar     Replaced memcpy calls with safe memscpy. 
01/22/13   pl      Implement code review comments 
01/20/13   pl      FTM LTE-CA IRAT support
10/16/12   ka      Added W2W FTM test 
10/09/12   pl      FTM IRAT cleanup
05/14/12   sar     Removed rtr8600 device.
05/09/11   sar     Added ftm_rf_cmd.h for CMI4.
03/24/11   pl      Fix warnings
03/07/11   jyu     Added Level 1&2 RF FTM IRAT test  
02/24/11   tnt     Merge to MDM9k PLF110
10/06/10   jyu     added test for xtoy_test_y and xtoy_test_x IRAT
09/24/10   kguo    Featurized LTOW IRAT test 
07/27/10   pl      Added test for x2DO IRAT
07/27/10   pl      Added test for x2L IRAT
06/02/10   kguo    Added support for LTOW test 
01/06/10   sar     Renamed ftm_task.h to rf_test_task_v.h.
12/21/09   jps     fixed lint error (out of bounds) for ftm_meas_extended_pkt_type 
12/01/09   ckl     Added command dispatch for W2W.
11/09/09   jfc     Lint Fix: suppress warnings 
09/15/09   jps     Initial version of the file

*/

/*------------------------------------------------------------------------------
                               INCLUDES
------------------------------------------------------------------------------*/

#include "rfa_variation.h"
#include "target.h"    /* Target specific definitions            */
#include "comdef.h"    /* Common stuff                           */
#include "stringl.h" /* for memscpy */

#include "ftmicap.h"

#ifdef FTM_HAS_IRAT

#include "ftm.h"
#include "ftm_meas_dispatch.h"
#include "ftm_meas.h"
#include "ftm_common.h"
#include "rf_test_task_v.h"
#include "rfmeas_types.h"
#include "ftm_rf_cmd.h"


/*------------------------------------------------------------------------------
                       VARIABLES
------------------------------------------------------------------------------*/
typedef PACKED union
{
  ftm_meas_extended_result_type     gen_result;
  
} ftm_meas_extended_result_union;


/*------------------------------------------------------------------------------
                       EXTERNAL FUNCTIONS
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      PUBLIC FUNCTION IMPLEMENTATIONS
------------------------------------------------------------------------------*/

#define FTM_MEAS_DEBUG_FLAG

/*----------------------------------------------------------------------------*/
/*!
   @brief
   This function handles requests to run MEAS tests and other primitives
   by dispatching the appropriate functions.

   @param
   cmd_ptr - 

   @retval
   Response packet
*/
ftm_rsp_pkt_type ftm_meas_extended_dispatch(ftm_meas_extended_pkt_type *cmd_ptr)
{
  uint16 serv_cell = 0, serv_chan= 0, meas_band = 0, meas_chan = 0;
  boolean tune_neighbour;
  uint16 num_trx = 0, num_trx_tasks = 0, num_non_trx = 0, num_non_trx_tasks = 0, exec_time_us = 0;
  rfm_meas_generic_params_type test_param;
  rfm_meas_generic_params_type test_param_half_scripts_in_generic_format;
  rfm_meas_generic_params_type test_param_null_scripts_in_generic_format;
  rfm_meas_null2x_params_type test_param_null2x;
  rfm_meas_half_scripts_params_type test_param_half_scripts;
  ftm_meas_rxagc_type rx_agc_results;
  ftm_meas_cell_type cell_type = SRC;
  rfm_mode_enum_type cell_tech = RFM_PARKED_MODE;
  
  
  	
  ftm_rsp_pkt_type               rsp_pkt = {FTM_RSP_DO_LEGACY, 0, NULL, FALSE};
  ftm_composite_cmd_header_type  rsp_headers;
  /* local_buffer and extra_data_len allow return data size to depend on a 
     request parameter. Unused for now */
  uint16                         *local_buffer  = NULL;
  uint16                         extra_data_len = 0; 
  ftm_meas_extended_result_union   result;
  /* Pointer to parameter buffer */
  uint16                           *buff_ptr = NULL;

  /*Return at the start of the command if input is NULL*/
  if (cmd_ptr == NULL) 
  {
    rsp_pkt.cmd = FTM_RSP_NO_RSP;
    rsp_pkt.pkt_len = 0;
    rsp_pkt.pkt_payload = NULL;
    rsp_pkt.delete_payload = FALSE;
    return rsp_pkt;
  }

  /* Initialize response packet size to length of the common headers */
  /* the size of the return parameters will be added in the dispatch cases below */
  rsp_headers.ftm_hdr.cmd_rsp_pkt_size = sizeof(ftm_composite_cmd_header_type);

  buff_ptr = (uint16*)&(cmd_ptr->ftm_meas_extended_params[0]);

  
  if (buff_ptr == NULL) 
  {
    rsp_pkt.cmd = FTM_RSP_NO_RSP;
    rsp_pkt.pkt_len = 0;
    rsp_pkt.pkt_payload = NULL;
    rsp_pkt.delete_payload = FALSE;
    return rsp_pkt;
  }
  
  switch (cmd_ptr->ftm_hdr.cmd_id)
  {  
   case FTM_MEAS_XTOY_TEST_Y:    
   {         
    /*Copy the input from Diag into FTM IRAT Generic structure*/		
    if(!ftm_meas_irat_copy_params(&test_param,buff_ptr))
    {
     result.gen_result.error_code = (int32)FTM_FAILURE;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
     break;
    }
  
    /*Pass it on to next layer to process Tuneaway command*/
    ftm_meas_xtoy_test_y(&test_param);
  
    /*Enable the flag for debugging*/
    #ifdef FTM_MEAS_DEBUG_FLAG
    ftm_meas_debug_params(&test_param);
    #endif

    result.gen_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
   }	
   break;
  
   case FTM_MEAS_XTOY_TEST_X:
   {
    /*Copy the input from Diag into FTM IRAT Generic structure*/		
    if(!ftm_meas_irat_copy_params(&test_param,buff_ptr))
    {
     result.gen_result.error_code = (int32)FTM_FAILURE;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
     break;
    }
  
    /*Pass it on to next layer to process Tuneback command*/
    ftm_meas_xtoy_test_x(&test_param);
  
    /*Enable the flag for debugging*/
    #ifdef FTM_MEAS_DEBUG_FLAG
    ftm_meas_debug_params(&test_param);
    #endif      
  
    result.gen_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
   }
   break;

   case FTM_MEAS_HALF_SCRIPT_HOMEFREQ_2_NBR_X:
   {
    /*Copy the input from Diag into FTM IRAT half script structure*/			
    if(!ftm_meas_half_script_copy_params(&test_param_half_scripts,buff_ptr))
    {
     result.gen_result.error_code = (int32)FTM_FAILURE;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
     break;
    }
  
    /*We need to convert halfscript parameters into generic params before passing it to meas level APIs*/
    /*This conversion is required as all the Tech level APIs expect params in generic format*/
    FTM_MSG_HIGH("Half script testing: HOMEFREQ to NBR X ",0,0,0);
    ftm_meas_convert_halfscript_to_generic_structure(&test_param_half_scripts,&test_param_half_scripts_in_generic_format);
  
    /*Pass it on to next layer to process half script command with appropiate half script type*/
    ftm_meas_half_script(&test_param_half_scripts_in_generic_format,RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_X);

    /*Enable the flag for debugging*/
    #ifdef FTM_MEAS_DEBUG_FLAG
    ftm_meas_debug_params(&test_param_half_scripts_in_generic_format);
    #endif
	 
    result.gen_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
   }
   break;
  
   case FTM_MEAS_HALF_SCRIPT_HOMEFREQ_2_NBR_Y:
   {         
    /*Copy the input from Diag into FTM IRAT half script structure*/				
    if(!ftm_meas_half_script_copy_params(&test_param_half_scripts,buff_ptr))
    {
     result.gen_result.error_code = (int32)FTM_FAILURE;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
     break;
    }

    /*We need to convert halfscript parameters into generic params before passing it to meas level APIs*/
    /*This conversion is required as all the Tech level APIs expect params in generic format*/	 
    FTM_MSG_HIGH("Half script testing: HOMEFREQ to NBR Y ",0,0,0);
    ftm_meas_convert_halfscript_to_generic_structure(&test_param_half_scripts,&test_param_half_scripts_in_generic_format);	
  
    /*Pass it on to next layer to process half script command with appropiate half script type*/
    ftm_meas_half_script(&test_param_half_scripts_in_generic_format,RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_Y);

    /*Enable the flag for debugging*/
    #ifdef FTM_MEAS_DEBUG_FLAG
    ftm_meas_debug_params(&test_param_half_scripts_in_generic_format);
    #endif
   
    result.gen_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
    }
    break;

    case FTM_MEAS_HALF_SCRIPT_NBR_X_2_NBR_Y:
    {
     /*Copy the input from Diag into FTM IRAT half script structure*/					
     if(!ftm_meas_half_script_copy_params(&test_param_half_scripts,buff_ptr))
     {
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
     }

     /*We need to convert halfscript parameters into generic params before passing it to meas level APIs*/
     /*This conversion is required as all the Tech level APIs expect params in generic format*/	      
     ftm_meas_convert_halfscript_to_generic_structure(&test_param_half_scripts,&test_param_half_scripts_in_generic_format);	

     /*Pass it on to next layer to process half script command with appropiate half script type*/
     FTM_MSG_HIGH("Half script testing: NBR X to NBR Y ",0,0,0);
     ftm_meas_half_script(&test_param_half_scripts_in_generic_format,RFM_HALF_SCRIPT_NBR_X_2_NBR_Y);

     /*Enable the flag for debugging*/
     #ifdef FTM_MEAS_DEBUG_FLAG
     ftm_meas_debug_params(&test_param_half_scripts_in_generic_format);
     #endif

     result.gen_result.error_code = (int32)FTM_SUCCESS;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
    }
    break;

    case FTM_MEAS_HALF_SCRIPT_NBR_X_2_HOMEFREQ:
    {
     /*Copy the input from Diag into FTM IRAT half script structure*/					
     if(!ftm_meas_half_script_copy_params(&test_param_half_scripts,buff_ptr))
     {
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
     }

     /*We need to convert halfscript parameters into generic params before passing it to meas level APIs*/
     /*This conversion is required as all the Tech level APIs expect params in generic format*/	 
     FTM_MSG_HIGH("Half script testing: NBR X to HOMEFREQ ",0,0,0);
     ftm_meas_convert_halfscript_to_generic_structure(&test_param_half_scripts,&test_param_half_scripts_in_generic_format); 

     /*Pass it on to next layer to process half script command with appropiate half script type*/	 
     ftm_meas_half_script(&test_param_half_scripts_in_generic_format,RFM_HALF_SCRIPT_NBR_X_2_HOMEFREQ);

     /*Enable the flag for debugging*/
     #ifdef FTM_MEAS_DEBUG_FLAG
     ftm_meas_debug_params(&test_param_half_scripts_in_generic_format);
     #endif

     result.gen_result.error_code = (int32)FTM_SUCCESS;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);    
    }
    break;

    case FTM_MEAS_HALF_SCRIPT_NBR_Y_2_NBR_X:
    {
     /*Copy the input from Diag into FTM IRAT half script structure*/						
     if(!ftm_meas_half_script_copy_params(&test_param_half_scripts,buff_ptr))
     {
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
     }
    
     /*We need to convert halfscript parameters into generic params before passing it to meas level APIs*/
     /*This conversion is required as all the Tech level APIs expect params in generic format*/	 	 
     ftm_meas_convert_halfscript_to_generic_structure(&test_param_half_scripts,&test_param_half_scripts_in_generic_format);

     /*Pass it on to next layer to process half script command with appropiate half script type*/	 
     FTM_MSG_HIGH("Half script testing: NBR Y to NBR X ",0,0,0);
     ftm_meas_half_script(&test_param_half_scripts_in_generic_format,RFM_HALF_SCRIPT_NBR_Y_2_NBR_X);

     /*Enable the flag for debugging*/
     #ifdef FTM_MEAS_DEBUG_FLAG
     ftm_meas_debug_params(&test_param_half_scripts_in_generic_format);
     #endif

     result.gen_result.error_code = (int32)FTM_SUCCESS;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);    
    }
    break;

    case FTM_MEAS_HALF_SCRIPT_NBR_Y_2_HOMEFREQ:
    {
     /*Copy the input from Diag into FTM IRAT half script structure*/							
     if(!ftm_meas_half_script_copy_params(&test_param_half_scripts,buff_ptr))
     {         
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
     }

     /*We need to convert halfscript parameters into generic params before passing it to meas level APIs*/
     /*This conversion is required as all the Tech level APIs expect params in generic format*/	 	 
     ftm_meas_convert_halfscript_to_generic_structure(&test_param_half_scripts,&test_param_half_scripts_in_generic_format);

     /*Pass it on to next layer to process half script command with appropiate half script type*/	 	
     FTM_MSG_HIGH("Half script testing: NBR Y to HOMEFREQ",0,0,0);
     ftm_meas_half_script(&test_param_half_scripts_in_generic_format,RFM_HALF_SCRIPT_NBR_Y_2_HOMEFREQ);

     /*Enable the flag for debugging*/
     #ifdef FTM_MEAS_DEBUG_FLAG
     ftm_meas_debug_params(&test_param_half_scripts_in_generic_format);
     #endif

     result.gen_result.error_code = (int32)FTM_SUCCESS;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);    
    }
    break;

  
    case FTM_MEAS_NULL2X_SCRIPT_START:
    {         
     /*Copy the input from Diag into FTM IRAT NULL2X script structure*/								
     if(!ftm_meas_null2x_copy_params(&test_param_null2x,buff_ptr))
     {
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
     }

     /*We need to convert NULL2X parameters into generic params before passing it to meas level APIs*/
     /*This conversion is required as all the Tech level APIs expect params in generic format*/	 	  
     ftm_meas_convert_null2xscript_to_generic_structure(&test_param_null2x,&test_param_null_scripts_in_generic_format);

     /*Pass it on to next layer to process NULL2X START*/
     FTM_MSG_HIGH("NULL2X script testing: START SCRIPT",0,0,0);
     ftm_meas_null2x_start(&test_param_null_scripts_in_generic_format);

     /*Enable the flag for debugging*/
     #ifdef FTM_MEAS_DEBUG_FLAG
     ftm_meas_debug_params(&test_param_null_scripts_in_generic_format);
     #endif

     result.gen_result.error_code = (int32)FTM_SUCCESS;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);    
    }	
    break;  

    case FTM_MEAS_NULL2X_SCRIPT_EXIT:
    {         
     /*Copy the input from Diag into FTM IRAT NULL2X script structure*/									
     if(!ftm_meas_null2x_copy_params(&test_param_null2x,buff_ptr))
     {
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
     }

     /*We need to convert NULL2X parameters into generic params before passing it to meas level APIs*/
     /*This conversion is required as all the Tech level APIs expect params in generic format*/	 
     ftm_meas_convert_null2xscript_to_generic_structure(&test_param_null2x,&test_param_null_scripts_in_generic_format);

     /*Pass it on to next layer to process NULL2X STOP*/
     FTM_MSG_HIGH("NULL2X script testing: STOP SCRIPT",0,0,0);
     ftm_meas_null2x_stop(&test_param_null_scripts_in_generic_format);

     /*Enable the flag for debugging*/
     #ifdef FTM_MEAS_DEBUG_FLAG
     ftm_meas_debug_params(&test_param_null_scripts_in_generic_format);
     #endif

     result.gen_result.error_code = (int32)FTM_SUCCESS;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);    
    }
    break;  

    case FTM_MEAS_GET_RXAGC:	 
     /*Sanity check: Verify if the tech parameter is valid*/ 
     if(*buff_ptr >= RFM_INVALID_MODE)
     {
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
     }	
     else
     {
      cell_tech = *buff_ptr++;
     }
     
     /*Sanity Check: Verify if the cell for which RXAGC is requested is valid*/ 
     if(*buff_ptr >= MAX_CELLS)
     {
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
     }		   	
     else
     {
      cell_type = *buff_ptr++;
     }	 

     /*Get RXAGC result*/ 
     if(!ftm_meas_get_rxagc_results(cell_tech,cell_type,&rx_agc_results))
     {
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
     }

     /*Incase of no failures, fill the result field as FTM_SUCCESS*/     
     result.gen_result.error_code = (int32)FTM_SUCCESS;
     rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type); 
     break;	

    /*  Illegal, or unknown MEAS test id  */
    default:
      result.gen_result.error_code = (int32)FTM_FAILURE;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_meas_extended_result_type);
      break;
  }

  /* Create response packet */
  /* Copy common header information from the request to the response */
  rsp_headers.diag_hdr             = cmd_ptr->diag_hdr;
  rsp_headers.ftm_hdr.cmd_id       = cmd_ptr->ftm_hdr.cmd_id;
  rsp_headers.ftm_hdr.cmd_data_len = cmd_ptr->ftm_hdr.cmd_data_len;


  switch (cmd_ptr->ftm_hdr.cmd_id)
  {  
   case FTM_MEAS_GET_RXAGC:   
   local_buffer = ftm_malloc(sizeof(ftm_meas_rxagc_type));
   if(local_buffer != NULL)
   {
    memscpy(local_buffer,sizeof(ftm_meas_rxagc_type),&rx_agc_results,sizeof(ftm_meas_rxagc_type));
    extra_data_len = sizeof(ftm_meas_rxagc_type);   
   }      
   break;	
		
   default:    
    local_buffer = ftm_malloc(sizeof(uint16)*10);
    if (local_buffer != NULL)
    {
     /* Get the additional information to complete the response packet */
     ftm_meas_get_irat_script_info(RFM_MEAS_PROGRAM_STARTUP,
                                   &num_trx,
                                   &num_trx_tasks,
                                   &num_non_trx,
                                   &num_non_trx_tasks,
                                   &exec_time_us);
     local_buffer[0] = num_trx;
     local_buffer[1] = num_trx_tasks;
     local_buffer[2] = num_non_trx;
     local_buffer[3] = num_non_trx_tasks;
     local_buffer[4] = exec_time_us;
     ftm_meas_get_irat_script_info(RFM_MEAS_PROGRAM_CLEANUP,
                                   &num_trx,								   
                                   &num_trx_tasks,
                                   &num_non_trx,
                                   &num_non_trx_tasks,
                                   &exec_time_us);
     local_buffer[5] = num_trx;
     local_buffer[6] = num_trx_tasks;
     local_buffer[7] = num_non_trx;
     local_buffer[8] = num_non_trx_tasks;
     local_buffer[9] = exec_time_us;
     extra_data_len = sizeof(uint16)*10;
   }
   break;
  }
     	
  /* At this point the response packet size equals the size of the headers plus
     the size of the fixed return parameters. If the response contains extra data
     whose size is determined by a parameter in the request, then we add it here. */
  rsp_pkt = ftmdiag_create_new_pkt(rsp_headers.ftm_hdr.cmd_rsp_pkt_size + extra_data_len);
  if ( rsp_pkt.pkt_payload != NULL )
  {
   /* copy header information */
   memscpy((void*)rsp_pkt.pkt_payload,
           sizeof(ftm_composite_cmd_header_type), 
           (void*)&rsp_headers, 
           sizeof(ftm_composite_cmd_header_type));

   /* copy fixed return parameters */
   memscpy((void*)((byte*)rsp_pkt.pkt_payload + sizeof(ftm_composite_cmd_header_type)), 
           (rsp_headers.ftm_hdr.cmd_rsp_pkt_size - sizeof(ftm_composite_cmd_header_type)),
           (void*)&result,
           (rsp_headers.ftm_hdr.cmd_rsp_pkt_size - sizeof(ftm_composite_cmd_header_type)));

   if ( extra_data_len > 0 )
   {
    /* copy extra variable length return data if present */
    memscpy((void*)((byte*)rsp_pkt.pkt_payload + rsp_headers.ftm_hdr.cmd_rsp_pkt_size),
             extra_data_len, 
             local_buffer, 
            extra_data_len);
   }
  }
  else
  {
    FTM_MSG_ERROR("Cannot allocate memory for response packet payload, cmd id = %d", 
                  cmd_ptr->ftm_hdr.cmd_id, 0, 0);
  }

  if ( local_buffer != NULL )
  {
   ftm_free(local_buffer);
  }

  return rsp_pkt;
} /* end ftm_meas_extended_dispatch */

/*!
   @brief
   This function prints all the parameters sent by FTM QRCT GUI.

   @param
   test_ptr - 

   @retval
   None
*/
void ftm_meas_debug_params(rfm_meas_generic_params_type *test_ptr)
{

 FTM_MSG_HIGH("test_ptr->src_tech %d, test_ptr->src_ca_path0_device %d,test_ptr->src_ca_path0_band %d",test_ptr->src_tech,test_ptr->src_ca_path0_device,test_ptr->src_ca_path0_band);
 FTM_MSG_HIGH("test_ptr->src_ca_path0_car_type %d",test_ptr->src_ca_path0_car_type,0,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path0_chan_car0 %d, test_ptr->src_ca_path0_chan_car1 %d,test_ptr->src_ca_path0_chan_car2 %d",test_ptr->src_ca_path0_chan_car0,test_ptr->src_ca_path0_chan_car1,test_ptr->src_ca_path0_chan_car2);
 FTM_MSG_HIGH("test_ptr->src_ca_path0_bw %d test_ptr->src_ca_path0_div_en %d",test_ptr->src_ca_path0_bw,test_ptr->src_ca_path0_div_en,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path0_div_device_id1 %d, test_ptr->src_ca_path0_div_device_id2 %d,test_ptr->src_ca_path0_div_device_id3 %d",test_ptr->src_ca_path0_div_device_id1,test_ptr->src_ca_path0_div_device_id2,test_ptr->src_ca_path0_div_device_id3);
 FTM_MSG_HIGH("test_ptr->src_ca_path0_tx_en %d test_ptr->src_ca_path0_tx_device %d",test_ptr->src_ca_path0_tx_en,test_ptr->src_ca_path0_tx_device,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path0_txlm_buf_idx %d test_ptr->src_ca_path0_prx_rxlm_buf_idx %d",test_ptr->src_ca_path0_txlm_buf_idx,test_ptr->src_ca_path0_prx_rxlm_buf_idx,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx %d test_ptr->src_ca_path0_drx_device_2_rxlm_buf_idx %d test_ptr->src_ca_path0_drx_device_3_rxlm_buf_idx %d",
 	           test_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx,test_ptr->src_ca_path0_drx_device_2_rxlm_buf_idx,test_ptr->src_ca_path0_drx_device_3_rxlm_buf_idx);


 FTM_MSG_HIGH("test_ptr->src_ca_path1_flag_enable %d, test_ptr->src_ca_path1_device %d,test_ptr->src_ca_path1_band %d",test_ptr->src_ca_path1_flag_enable,test_ptr->src_ca_path1_device,test_ptr->src_ca_path1_band);
 FTM_MSG_HIGH("test_ptr->src_ca_path1_car_type %d",test_ptr->src_ca_path1_car_type,0,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path1_chan_car0 %d, test_ptr->src_ca_path1_chan_car1 %d,test_ptr->src_ca_path1_chan_car2 %d",test_ptr->src_ca_path1_chan_car0,test_ptr->src_ca_path1_chan_car1,test_ptr->src_ca_path1_chan_car2);
 FTM_MSG_HIGH("test_ptr->src_ca_path1_bw %d test_ptr->src_ca_path1_div_en %d",test_ptr->src_ca_path1_bw,test_ptr->src_ca_path1_div_en,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path1_div_device_id1 %d, test_ptr->src_ca_path1_div_device_id2 %d,test_ptr->src_ca_path1_div_device_id3 %d",test_ptr->src_ca_path1_div_device_id1,test_ptr->src_ca_path1_div_device_id2,test_ptr->src_ca_path1_div_device_id3);
 FTM_MSG_HIGH("test_ptr->src_ca_path1_tx_en %d test_ptr->src_ca_path1_tx_device %d",test_ptr->src_ca_path1_tx_en,test_ptr->src_ca_path1_tx_device,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path1_txlm_buf_idx %d test_ptr->src_ca_path1_prx_rxlm_buf_idx %d",test_ptr->src_ca_path1_txlm_buf_idx,test_ptr->src_ca_path1_prx_rxlm_buf_idx,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx %d test_ptr->src_ca_path1_drx_device_2_rxlm_buf_idx %d test_ptr->src_ca_path1_drx_device_3_rxlm_buf_idx %d",
 	           test_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx,test_ptr->src_ca_path1_drx_device_2_rxlm_buf_idx,test_ptr->src_ca_path1_drx_device_3_rxlm_buf_idx);


 FTM_MSG_HIGH("test_ptr->src_ca_path2_flag_enable %d, test_ptr->src_ca_path2_device %d,test_ptr->src_ca_path2_band %d",test_ptr->src_ca_path2_flag_enable,test_ptr->src_ca_path2_device,test_ptr->src_ca_path2_band);
 FTM_MSG_HIGH("test_ptr->src_ca_path2_car_type %d",test_ptr->src_ca_path2_car_type,0,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path2_chan_car0 %d, test_ptr->src_ca_path2_chan_car1 %d,test_ptr->src_ca_path2_chan_car2 %d",test_ptr->src_ca_path2_chan_car0,test_ptr->src_ca_path2_chan_car1,test_ptr->src_ca_path2_chan_car2);
 FTM_MSG_HIGH("test_ptr->src_ca_path2_bw %d test_ptr->src_ca_path2_div_en %d",test_ptr->src_ca_path2_bw,test_ptr->src_ca_path2_div_en,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path2_div_device_id1 %d, test_ptr->src_ca_path2_div_device_id2 %d,test_ptr->src_ca_path2_div_device_id3 %d",test_ptr->src_ca_path2_div_device_id1,test_ptr->src_ca_path2_div_device_id2,test_ptr->src_ca_path2_div_device_id3);
 FTM_MSG_HIGH("test_ptr->src_ca_path2_tx_en %d test_ptr->src_ca_path2_tx_device %d",test_ptr->src_ca_path2_tx_en,test_ptr->src_ca_path2_tx_device,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path2_txlm_buf_idx %d test_ptr->src_ca_path2_prx_rxlm_buf_idx %d",test_ptr->src_ca_path2_txlm_buf_idx,test_ptr->src_ca_path2_prx_rxlm_buf_idx,0);
 FTM_MSG_HIGH("test_ptr->src_ca_path2_drx_device_1_rxlm_buf_idx %d test_ptr->src_ca_path2_drx_device_2_rxlm_buf_idx %d test_ptr->src_ca_path2_drx_device_3_rxlm_buf_idx %d",
 	           test_ptr->src_ca_path2_drx_device_1_rxlm_buf_idx,test_ptr->src_ca_path2_drx_device_2_rxlm_buf_idx,test_ptr->src_ca_path2_drx_device_3_rxlm_buf_idx);


 FTM_MSG_HIGH("test_ptr->test_level %d",test_ptr->test_level,0,0);

 FTM_MSG_HIGH("test_ptr->tgt_tech %d,test_ptr->tgt_device %d,test_ptr->tgt_band %d ",test_ptr->tgt_tech,test_ptr->tgt_device,test_ptr->tgt_band);
 FTM_MSG_HIGH("test_ptr->tgt_chan %d,test_ptr->tgt_bw %d ",test_ptr->tgt_chan,test_ptr->tgt_bw,0);
 FTM_MSG_HIGH("test_ptr->tgt_div_en %d,test_ptr->tgt_div_device %d",test_ptr->tgt_div_en,test_ptr->tgt_div_device,0);
 FTM_MSG_HIGH("test_ptr->tgt_prx_rxlm_buf_idx %d test_ptr->tgt_drx_rxlm_buf_idx %d",test_ptr->tgt_prx_rxlm_buf_idx,test_ptr->tgt_drx_rxlm_buf_idx,0);

}


/*!
   @brief
    This function copies the parameters sent by FTM QRCT GUI into FTM MEAS GENERIC structure.

   @param
   test_param - The pointer to structure where parameters are copied.

   @param
   buff_ptr - The pointer to structure received from diag.

   @retval
   SUCCESS if we dont encounter any unexpected parameters while copying, else return FAILURE.
*/
boolean ftm_meas_irat_copy_params(rfm_meas_generic_params_type *test_param,uint16 *buff_ptr)
{

 boolean sanity_test_flag = TRUE;
 /* Copying parameter for CA 0*/  

 /* Initialize test_param to Zero*/
 memset(test_param,0,sizeof(rfm_meas_generic_params_type)); 

  /*SRC Tech*/
 test_param->src_tech = (rfm_mode_enum_type)(*buff_ptr++);   
 test_param->src_ca_path0_device     = (rfm_device_enum_type)(*buff_ptr++);     
 test_param->src_ca_path0_band     = *buff_ptr++;
 test_param->src_ca_path0_car_type   = *buff_ptr++;
 test_param->src_ca_path0_chan_car0   = *buff_ptr++;
 test_param->src_ca_path0_chan_car1   = *buff_ptr++;
 test_param->src_ca_path0_chan_car2   = *buff_ptr++; 
 test_param->src_ca_path0_bw     = (rfcom_lte_bw_type)(*buff_ptr++);  
 test_param->src_ca_path0_div_en     = *buff_ptr++;
 test_param->src_ca_path0_div_device_id1 = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path0_div_device_id2 = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path0_div_device_id3 = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path0_tx_en          = (rfm_device_enum_type)(*buff_ptr++); 
 test_param->src_ca_path0_tx_device   = (rfm_device_enum_type)(*buff_ptr++);   
  
  /* Check if Device ID is 0xFFFF and set it to invalid. Ideally, the value given from tools should match the enum value RFM_INVALID_DEVICE. But this value keeps changing as the number of devices change.*/
  /* Hence, we have asked tools to set it to RFM_ALL_DEVICES(0xFFFF) and we are modifying it in Embedded code */     
 test_param->src_ca_path0_device   = ((test_param->src_ca_path0_device  == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path0_device);
 test_param->src_ca_path0_div_device_id1 = ((test_param->src_ca_path0_div_device_id1 == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path0_div_device_id1);
 test_param->src_ca_path0_div_device_id2 = ((test_param->src_ca_path0_div_device_id2 == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path0_div_device_id2);
 test_param->src_ca_path0_div_device_id3 = ((test_param->src_ca_path0_div_device_id3 == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path0_div_device_id3);
 test_param->src_ca_path0_tx_device   = ((test_param->src_ca_path0_tx_device  == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path0_tx_device);
  


 test_param->tgt_tech      = (rfm_mode_enum_type)(*buff_ptr++);
 test_param->tgt_device     = (rfm_device_enum_type)(*buff_ptr++); 
 test_param->tgt_band      = *buff_ptr++;
 test_param->tgt_chan      = *buff_ptr++;   
 test_param->tgt_bw     = (rfcom_lte_bw_type)(*buff_ptr++);  
 test_param->tgt_div_en     = *buff_ptr++;
 test_param->tgt_div_device   = (rfm_device_enum_type)(*buff_ptr++); 

 /* Check if Device ID is 0xFFFF and set it to invalid. Ideally, the value given from tools should match the enum value RFM_INVALID_DEVICE. But this value keeps changing as the number of devices change.*/
 /* Hence, we have asked tools to set it to RFM_ALL_DEVICES(0xFFFF) and we are modifying it in Embedded code */	  
 test_param->tgt_device     = ((test_param->tgt_device == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->tgt_device);  
 test_param->tgt_div_device   = ((test_param->tgt_div_device == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->tgt_div_device);
  
  /* Test level*/
 test_param->test_level = *buff_ptr++;
  
  /* Copying parameter for CA 1*/ 
 test_param->src_ca_path1_flag_enable    = *buff_ptr++;
 test_param->src_ca_path1_device     = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path1_band     = *buff_ptr++;
 test_param->src_ca_path1_car_type   = *buff_ptr++;
 test_param->src_ca_path1_chan_car0   = *buff_ptr++;
 test_param->src_ca_path1_chan_car1   = *buff_ptr++;
 test_param->src_ca_path1_chan_car2   = *buff_ptr++; 
 test_param->src_ca_path1_bw     = *buff_ptr++;
 test_param->src_ca_path1_div_en     = *buff_ptr++;
 test_param->src_ca_path1_div_device_id1 = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path1_div_device_id2 = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path1_div_device_id3 = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path1_tx_en          = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path1_tx_device   = (rfm_device_enum_type)(*buff_ptr++);
  
  /* Check if Device ID is 0xFFFF and set it to invalid. Ideally, the value given from tools should match the enum value RFM_INVALID_DEVICE. But this value keeps changing as the number of devices change.*/
  /* Hence, we have asked tools to set it to RFM_ALL_DEVICES(0xFFFF) and we are modifying it in Embedded code */     
 test_param->src_ca_path1_device = ((test_param->src_ca_path1_device == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path1_device);
 test_param->src_ca_path1_div_device_id1 = ((test_param->src_ca_path1_div_device_id1 == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path1_div_device_id1);
 test_param->src_ca_path1_div_device_id2 = ((test_param->src_ca_path1_div_device_id2 == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path1_div_device_id2);
 test_param->src_ca_path1_div_device_id3 = ((test_param->src_ca_path1_div_device_id3 == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path1_div_device_id3);
 test_param->src_ca_path1_tx_device   = ((test_param->src_ca_path1_tx_device  == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path1_tx_device);
  
  /* Copying parameter for CA 2*/ 
 test_param->src_ca_path2_flag_enable    = *buff_ptr++;
 test_param->src_ca_path2_device     = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path2_band     = *buff_ptr++;
 test_param->src_ca_path2_car_type   = *buff_ptr++;
 test_param->src_ca_path2_chan_car0   = *buff_ptr++;
 test_param->src_ca_path2_chan_car1   = *buff_ptr++;
 test_param->src_ca_path2_chan_car2   = *buff_ptr++; 
 test_param->src_ca_path2_bw     = *buff_ptr++;
 test_param->src_ca_path2_div_en     = *buff_ptr++;
 test_param->src_ca_path2_div_device_id1 = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path2_div_device_id2 = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path2_div_device_id3 = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path2_tx_en          = (rfm_device_enum_type)(*buff_ptr++);
 test_param->src_ca_path2_tx_device = (rfm_device_enum_type)(*buff_ptr);
  
  /* Check if Device ID is 0xFFFF and set it to invalid. Ideally, the value given from tools should match the enum value RFM_INVALID_DEVICE. But this value keeps changing as the number of devices change.*/
  /* Hence, we have asked tools to set it to RFM_ALL_DEVICES(0xFFFF) and we are modifying it in Embedded code */     
 test_param->src_ca_path2_device = ((test_param->src_ca_path2_device == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path2_device);
 test_param->src_ca_path2_div_device_id1 = ((test_param->src_ca_path2_div_device_id1 == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path2_div_device_id1);
 test_param->src_ca_path2_div_device_id2 = ((test_param->src_ca_path2_div_device_id2 == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path2_div_device_id2);
 test_param->src_ca_path2_div_device_id3 = ((test_param->src_ca_path2_div_device_id3 == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path2_div_device_id3);
 test_param->src_ca_path2_tx_device     = ((test_param->src_ca_path2_tx_device    == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_ca_path2_tx_device); 

 /*Sanity check for source CA PATH 0 parameters */ 
 if( (test_param->src_tech >= RFM_NUM_MODES) || (test_param->src_ca_path0_device >= RFM_MAX_WAN_DEVICES) || (test_param->src_ca_path0_bw >= RFCOM_NUM_LTE_BWS))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC parameters are incorrect: SRC tech %d SRC device %d SRC BW %d",test_param->src_tech ,test_param->src_ca_path0_device,test_param->src_ca_path0_bw);	
 }
 if( (test_param->src_ca_path0_div_en == TRUE) && !( (test_param->src_ca_path0_div_device_id1 < RFM_MAX_WAN_DEVICES)  
                          || (test_param->src_ca_path0_div_device_id2 < RFM_MAX_WAN_DEVICES) || (test_param->src_ca_path0_div_device_id3 < RFM_MAX_WAN_DEVICES) ))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC CA PATH 0 DIV is enabled but none of the DIV DEVICE IDs are valid. src_ca_path0_div_device_id1 %d src_ca_path0_div_device_id2 %d"
       	"src_ca_path0_div_device_id3 %d",test_param->src_ca_path0_div_device_id1 ,test_param->src_ca_path0_div_device_id2,test_param->src_ca_path0_div_device_id3);	
 }
 if( (test_param->src_ca_path0_tx_en == TRUE) && !(test_param->src_ca_path0_tx_device < RFM_MAX_WAN_DEVICES))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC CA PATH 0 Tx is enabled but Tx device is populated incorrectly %d",test_param->src_ca_path0_tx_device,0,0);	
 }

 /*Sanity check for source CA PATH 1 parameters */ 
 if(test_param->src_ca_path1_bw >= RFCOM_NUM_LTE_BWS)
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC CA PATH 1 BW %d is incorrect",test_param->src_ca_path1_bw,0,0);	
 }
 if( (test_param->src_ca_path1_div_en == TRUE) && !( (test_param->src_ca_path1_div_device_id1 < RFM_MAX_WAN_DEVICES)  
                          || (test_param->src_ca_path1_div_device_id2 < RFM_MAX_WAN_DEVICES) || (test_param->src_ca_path1_div_device_id3 < RFM_MAX_WAN_DEVICES) ))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC CA PATH 1 DIV is enabled but none of the DIV DEVICE IDs are valid. src_ca_path1_div_device_id1 %d src_ca_path1_div_device_id2 %d"
       	"src_ca_path1_div_device_id3 %d",test_param->src_ca_path1_div_device_id1 ,test_param->src_ca_path1_div_device_id2,test_param->src_ca_path1_div_device_id3);	
 }
 if( (test_param->src_ca_path1_tx_en == TRUE) && !(test_param->src_ca_path1_tx_device < RFM_MAX_WAN_DEVICES))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC CA PATH 1 Tx is enabled but Tx device is populated incorrectly %d",test_param->src_ca_path1_tx_device,0,0);	
 }


 /*Sanity check for source CA PATH 2 parameters */ 
 if(test_param->src_ca_path2_bw >= RFCOM_NUM_LTE_BWS)
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC CA PATH 2 BW %d is incorrect",test_param->src_ca_path2_bw,0,0);	
 }
 if( (test_param->src_ca_path2_div_en == TRUE) && !( (test_param->src_ca_path2_div_device_id1 < RFM_MAX_WAN_DEVICES)  
                          || (test_param->src_ca_path2_div_device_id2 < RFM_MAX_WAN_DEVICES) || (test_param->src_ca_path2_div_device_id3 < RFM_MAX_WAN_DEVICES) ))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC CA PATH 2 DIV is enabled but none of the DIV DEVICE IDs are valid. src_ca_path2_div_device_id1 %d src_ca_path2_div_device_id2 %d"
       	"src_ca_path2_div_device_id3 %d",test_param->src_ca_path2_div_device_id1 ,test_param->src_ca_path2_div_device_id2,test_param->src_ca_path2_div_device_id3);	
 }
 if( (test_param->src_ca_path2_tx_en == TRUE) && !(test_param->src_ca_path2_tx_device < RFM_MAX_WAN_DEVICES))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC CA PATH 2 Tx is enabled but Tx device is populated incorrectly %d",test_param->src_ca_path2_tx_device,0,0);	
 }

 /*Sanity check for TGT parameters*/
 if( (test_param->tgt_tech >= RFM_NUM_MODES) || (test_param->tgt_device >= RFM_MAX_WAN_DEVICES) || (test_param->tgt_bw >= RFCOM_NUM_LTE_BWS))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("TGT parameters are incorrect: TGT tech %d TGT device %d TGT BW %d",test_param->tgt_tech ,test_param->tgt_device,test_param->tgt_bw);	
 }
 if( (test_param->tgt_div_en == TRUE) && !(test_param->tgt_div_device <= RFM_MAX_WAN_DEVICES))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("TGT DIV is enabled but DIV device id %d is incorrect",test_param->tgt_div_device,0,0);	
 }

 return sanity_test_flag;

}

/*!
   @brief
   This function converts parameters stored in halfscript structure format to generic structure format .

   @param
   test_param_half_script_ptr - The pointer to structure where parameters are stored in halfscript format.

   @param
   test_param_generic_ptr      - The pointer to structure where parameters are converted from halfscript format and stored in generic format.

   @retval
   None
*/
void ftm_meas_convert_halfscript_to_generic_structure(rfm_meas_half_scripts_params_type *test_param_half_script_ptr,rfm_meas_generic_params_type *test_param_generic_ptr)
{
 memset(test_param_generic_ptr,0,sizeof(rfm_meas_generic_params_type)); 
 test_param_generic_ptr->src_tech                               = test_param_half_script_ptr->src_tech;
 test_param_generic_ptr->src_ca_path0_device                    = test_param_half_script_ptr->src_device;
 test_param_generic_ptr->src_ca_path0_band                      = test_param_half_script_ptr->src_band;
 test_param_generic_ptr->src_ca_path0_chan_car0                 = test_param_half_script_ptr->src_chan;
 test_param_generic_ptr->src_ca_path0_bw                        = test_param_half_script_ptr->src_bw;
 test_param_generic_ptr->src_ca_path0_div_en                    = test_param_half_script_ptr->src_div_en;
 test_param_generic_ptr->src_ca_path0_div_device_id1            = test_param_half_script_ptr->src_div_device;
 test_param_generic_ptr->src_ca_path0_prx_rxlm_buf_idx          = test_param_half_script_ptr->src_prx_rxlm_buf_idx;
 test_param_generic_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx = test_param_half_script_ptr->src_drx_rxlm_buf_idx;
 test_param_generic_ptr->tgt_tech                               = test_param_half_script_ptr->tgt_tech;
 test_param_generic_ptr->tgt_device                             = test_param_half_script_ptr->tgt_device;
 test_param_generic_ptr->tgt_band                               = test_param_half_script_ptr->tgt_band;
 test_param_generic_ptr->tgt_chan                               = test_param_half_script_ptr->tgt_chan;
 test_param_generic_ptr->tgt_bw                                 = test_param_half_script_ptr->tgt_bw;
 test_param_generic_ptr->tgt_div_en                             = test_param_half_script_ptr->tgt_div_en;
 test_param_generic_ptr->tgt_div_device                         = test_param_half_script_ptr->tgt_div_device;
 test_param_generic_ptr->tgt_prx_rxlm_buf_idx                   = test_param_half_script_ptr->tgt_prx_rxlm_buf_idx;
 test_param_generic_ptr->tgt_drx_rxlm_buf_idx                   = test_param_half_script_ptr->tgt_drx_rxlm_buf_idx;
 test_param_generic_ptr->test_level                             = test_param_half_script_ptr->test_level; 
}

/*!
   @brief
   This function converts parameters stored in NULL2X structure format to generic structure format .

   @param
   test_param_null2x_script_ptr - The pointer to structure where parameters are stored in NULL2X format.

   @param
   test_param_generic_ptr         - The pointer to structure where parameters are converted from halfscript format and stored in generic format.

   @retval
   None
*/

void ftm_meas_convert_null2xscript_to_generic_structure(rfm_meas_null2x_params_type *test_param_null2x_script_ptr,rfm_meas_generic_params_type *test_param_generic_ptr)
{
 memset(test_param_generic_ptr,0,sizeof(rfm_meas_generic_params_type)); 
 test_param_generic_ptr->tgt_tech                               = test_param_null2x_script_ptr->tgt_tech;
 test_param_generic_ptr->tgt_device                             = test_param_null2x_script_ptr->tgt_device;
 test_param_generic_ptr->tgt_band                               = test_param_null2x_script_ptr->tgt_band;
 test_param_generic_ptr->tgt_chan                               = test_param_null2x_script_ptr->tgt_chan;
 test_param_generic_ptr->tgt_bw                                 = test_param_null2x_script_ptr->tgt_bw;
 test_param_generic_ptr->tgt_div_en                             = test_param_null2x_script_ptr->tgt_div_en;
 test_param_generic_ptr->tgt_div_device                         = test_param_null2x_script_ptr->tgt_div_device;
 test_param_generic_ptr->tgt_prx_rxlm_buf_idx                   = test_param_null2x_script_ptr->tgt_prx_rxlm_buf_idx;
 test_param_generic_ptr->tgt_drx_rxlm_buf_idx                   = test_param_null2x_script_ptr->tgt_drx_rxlm_buf_idx;
 test_param_generic_ptr->test_level                             = test_param_null2x_script_ptr->test_level; 
}

/*!
   @brief
   This function prints all the parameters sent by FTM QRCT GUI for Half script.

   @param
   test_param - The pointer to structure where parameters are copied.

   @param
   buff_ptr - The pointer to structure received from diag.

   @retval
   SUCCESS if we dont encounter any unexpected parameters while copying, else return FAILURE.
*/
boolean ftm_meas_half_script_copy_params(rfm_meas_half_scripts_params_type *test_param,uint16 *buff_ptr)
{

 /*Start sanity flag as TRUE and make it FALSE if either SRC or TGT parameters are invalid */
 boolean sanity_test_flag = TRUE;

 memset(test_param,0,sizeof(rfm_meas_half_scripts_params_type)); 
 
 /*Copying SRC Tech*/
 test_param->src_tech = (rfm_mode_enum_type)(*buff_ptr++);   
 test_param->src_device     = (rfm_device_enum_type)(*buff_ptr++);      
 test_param->src_band     = *buff_ptr++;
 test_param->src_chan   = *buff_ptr++;  
 test_param->src_bw   = *buff_ptr++; 
 test_param->src_div_en   = *buff_ptr++;
 test_param->src_div_device   = *buff_ptr++; 
 test_param->src_div_device   = ((test_param->src_div_device == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->src_div_device); 

 /*Copying TGT Tech*/
 test_param->tgt_tech      = (rfm_mode_enum_type)(*buff_ptr++);
 test_param->tgt_device     = (rfm_device_enum_type)(*buff_ptr++);
 test_param->tgt_band      = *buff_ptr++;
 test_param->tgt_chan      = *buff_ptr++;   
 test_param->tgt_bw     = (rfcom_lte_bw_type)(*buff_ptr++);  
 test_param->tgt_div_en     = *buff_ptr++;
 test_param->tgt_div_device   = (rfm_device_enum_type)(*buff_ptr++); 
 test_param->tgt_div_device   = ((test_param->tgt_div_device == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->tgt_div_device);
  
 /* Test level*/
 test_param->test_level = *buff_ptr++;

 /*Sanity check for source parameters*/
 if( (test_param->src_tech >= RFM_NUM_MODES) || (test_param->src_device >= RFM_MAX_WAN_DEVICES) || (test_param->src_bw >= RFCOM_NUM_LTE_BWS))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("SRC parameters are incorrect: SRC tech %d SRC device %d SRC BW %d",test_param->src_tech ,test_param->src_device,test_param->src_bw);	
 }	

 /*Sanity check for TGT parameters*/
 if( (test_param->tgt_tech >= RFM_NUM_MODES) || (test_param->tgt_device >= RFM_MAX_WAN_DEVICES) || (test_param->tgt_bw >= RFCOM_NUM_LTE_BWS))
 {
  sanity_test_flag =  FALSE;
  FTM_MSG_ERROR("TGT parameters are incorrect: TGT tech %d TGT device %d TGT BW %d",test_param->tgt_tech ,test_param->tgt_device,test_param->tgt_bw);	
 }

 return sanity_test_flag;
}


/*!
   @brief
   This function prints all the parameters sent by FTM QRCT GUI for NULL2X script.

   @param
   test_param - The pointer to structure where parameters are copied.

   @param
   buff_ptr - The pointer to structure received from diag.

   @retval
   SUCCESS if we dont encounter any unexpected parameters while copying, else return FAILURE.
*/
boolean ftm_meas_null2x_copy_params(rfm_meas_null2x_params_type *test_param,uint16 *buff_ptr)
{ 

 /*Start sanity flag as TRUE and make it FALSE if either SRC or TGT parameters are invalid */
 boolean sanity_test_flag = TRUE;

 memset(test_param,0,sizeof(rfm_meas_null2x_params_type)); 
 
 test_param->tgt_tech      = (rfm_mode_enum_type)(*buff_ptr++);
 test_param->tgt_device     = (rfm_device_enum_type)(*buff_ptr++);
 test_param->tgt_device     = ((test_param->tgt_device == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->tgt_device);   
 test_param->tgt_band      = *buff_ptr++;
 test_param->tgt_chan      = *buff_ptr++;   
 test_param->tgt_bw     = (rfcom_lte_bw_type)(*buff_ptr++);  
 test_param->tgt_div_en     = *buff_ptr++;
 test_param->tgt_div_device   = (rfm_device_enum_type)(*buff_ptr++); 
 test_param->tgt_div_device   = ((test_param->tgt_div_device == RFM_ALL_DEVICES)?RFM_INVALID_DEVICE:test_param->tgt_div_device);
  
  /* Test level*/
 test_param->test_level = *buff_ptr++;  

 /*Sanity check for TGT parameters*/
 if( (test_param->tgt_tech >= RFM_NUM_MODES) || (test_param->tgt_device >= RFM_MAX_WAN_DEVICES) || (test_param->tgt_bw >= RFCOM_NUM_LTE_BWS))
 {
  sanity_test_flag = FALSE;
  FTM_MSG_ERROR("TGT parameters are incorrect: TGT tech %d TGT device %d TGT BW %d",test_param->tgt_tech ,test_param->tgt_device,test_param->tgt_bw);	
 }

 return sanity_test_flag;  

}
#endif /* FTM_HAS_IRAT */