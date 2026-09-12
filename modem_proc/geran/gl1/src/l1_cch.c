/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              L 1   C C H  R E C E I V E  P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to initiate a Control Channel (CCH)
   message receive.  This procedure executes in the L1 ISR context.

EXTERNALIZED FUNCTIONS
  L1_send_CCH
    Procedure to initiate a synchronous transmit of a CCH message.
  L1_get_CCH_message
    Procedure to query the dedicated channel buffers for a message to
    transmit.

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/GSM/L1/vcs/l1_cch.c_v   1.5   25 Apr 2002 17:33:52   dpandian  $
$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_cch.c#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
17/04/17   nm        CR2032010 make rf power class variable dual data spaced
14/08/14   ws        CR889273 Fix Static declarations of variables which are not Dual data spaced
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
13/03/15   mc        CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
04/11/13   mc        CR556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
24/10/13   cja       CR566207 Generate filler frame for FR signalling only channel.
08/10/13   cja       CR561518 Do not generate filler frame for FACCH
07/16/13   zc        CR470197 Replace memcpy with memscpy
13/03/13   mc        CR459487 : COEX Support - GERAN arbitrator framework
19/11/12   ky        409728:26.22.1-L2 Filler Bits TC failure
05/06/08   ps        Lint Cleanup
27/03/08   nf        Added NV Item handling to R-SACCH and R-FACCH
14/03/08   nt        Removed the change to stop power capping when the GCF flag is
                     set. This breaks test case 13.3.4.1
09/11/07   nf        Added FEATURE_GSM_R_SACCH
02/27/06   ws        CR#: 112782 - Fix for GSM L1 generates valid measurement
                     reports when SACCH data is not provided by L2
11/16/05   ws        More Lint Cleanup
12/23/04   dp        Fix to not cap power level if Anite GCF flag is set
06/21/04   dp        Cap reported power level based on power class
02/25/04   gw        More lint clean up.
07/11/03   gfr       Lint cleanup.
05/01/03   mk        AFC/TT cleanup.
08/23/02   plm       Added include of string.h to eliminate compiler warnings.
04/24/02   dp        Put back ms_power_level_to_report when reporting TXLEV on
                     the uplink SACCH.  This is the power on the last burst of the
                     previous reporting period
03/20/02   dp        Changed ms_power_level_used to ms_power_level_in_use
02/27/02   JC        Removed L1_set_rx_gain() and L1_process_EQU_data fcns.
12/10/01   thh       Removed hw_gsm.h include file.
09/27/01   gw/jc     Removed L1_read_CCH(), L1_send_CCH().
09/25/01   mk        General cleanup using COMMENT. Removed Ncell support.
09/20/01   jc        Use message layer functions for ta, tx_lev.
08/31/01   JC        Modified L1_get_CCH() interface to return data length.
                     Also, general cleanup. COMMENT blocks around Ps_ vars.
08/22/01   mk        Fixed if-else in L1_CCH_DECODED (MONITOR_DEBUG
                     not defined).
08/17/01   mk/gw     Added Ncell support, featurized via MONITOR_DEBUG.
07/30/01   JC        Changed cch_data length to byte length instead of word
                     length.
05/29/01   JC        Removed SPINCO_SIMULATION code.
05/16/01   JC        Moved L1_send_CCH() to this module (was in m10511.c).
                     Also added supporting functions.
05/05/01   JC        Added CCH function state 2 support for back to back
                     CCH receives.
04/24/01   jc        Added support for JUMP_TO_TCH
04/13/01   JC        Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1i.h"
#include "l1_task.h"
#include "l1_isr.h"
#include "l1_utils.h"
#include <string.h>
#include <stdio.h>
#include <stringl/stringl.h>

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
#ifdef JUMP_TO_TCH
#error code not present
#endif

extern dedicated_data_T l1_dedicated_data[];

/* This is a predefined SACCH measurement report which indicates:
 *
 *   a) the measurements are not valid, and 
 *   b) the UE has not yet received a BA list for the serving cell
 *
 * Note that it does not include a SACCH header (2 additional octets)
 *
 * This should only get sent in the periods where measurements are not
 * being passed up to RR (hence, no SACCH PH_DATA_REQ is provided by L2)
 * Typically, this occurs for the first reporting period after initial
 * channel assignment, as well as immediately after a handover
 */
const byte l1_invalid_sacch_meas_report[SACCH_MESSAGE_UNIT] =
{
  0x01, 0x03, 0x49, 0x06, 0x15, 0x00, 0x40, 0x01, 0xC0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static boolean l1_rsacch_ul_modify_block (byte    *op_buffer_ptr,
                                          uint16  *num_octets_copied_ptr,
                                          l1_sacch_buffer_T  *sacch_buffer_ptr,
                                          gas_id_t gas_id);
 #ifdef DEBUG_FEATURE_GSM_R_SACCH
#error code not present
#endif

/*===========================================================================

FUNCTION L1_GET_CCH_MESSAGE

DESCRIPTION
  This function gets a cch message from the L1 buffers and
  copies it to the buffer pointed to by op_buffer_ptr.

DEPENDENCIES
  boolean   use_filler_message
  channel_type  ( can have values SDCCH, FACCH or SACCH)
  Dedicated data_ptr
  op_buffer_ptr

RETURN VALUE
  use_filler_message ( there was not one available in the buffer)

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_get_CCH_message ( boolean           use_filler_message,
                             l2_channel_type_T channel_type,
                             dedicated_data_T  *ded_data,
                             byte              *op_buffer_ptr,
                             uint16            *num_octets_copied_ptr,
                             gas_id_t          gas_id )
{

   /* Locals */

   l1_dcch_buffer_T   *dcch_buffer_ptr;
   l1_sacch_buffer_T  *sacch_buffer_ptr;
   boolean             data_not_available = FALSE;
   byte                power_level_to_report_cap = 0;
   boolean             rsacch_buf_updated = FALSE;
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

   if ( channel_type == SACCH)

      /* Message is for the SACCH */

   {

      
      /*Cap TXLEV based on the power class. Where multiple TXLEVs map to 
        same output power, report just the minimum TXLEV in the range. Any 
        ARFCN in freq list will do since we don't hop across bands */
      power_level_to_report_cap = 
        (byte)gl1_msg_cap_reported_power_level ( ded_data->channel_specification.channel_info_1_after.frequency_list.channel[0],ded_data->ms_power_level_to_report, gas_id );
      MSG_GERAN_HIGH_2_G("Reported TXLEV Orig: %d Capped %d", ded_data->ms_power_level_to_report, power_level_to_report_cap);
    

      /* Create the SACCH message */
      /* This is the power on the last burst of the previous reporting period */
      *op_buffer_ptr++ = power_level_to_report_cap & 31;
      *op_buffer_ptr++ = ded_data->timing_advance_in_use & 127;
 
      /* Check to see if messages are available */

      sacch_buffer_ptr = ded_data->l1_sacch_s0_buffer_ptr;

      if(l1_dedicated_data_ptr->rsacch_ctrl.nv_active == TRUE)
      {
          rsacch_buf_updated = l1_rsacch_ul_modify_block(op_buffer_ptr, num_octets_copied_ptr, sacch_buffer_ptr, gas_id);
      }

      if (sacch_buffer_ptr->data_valid == FALSE )
      {

         /* The buffer is empty */
         /* use the filler message */

         if ( use_filler_message )
         {
            memscpy (op_buffer_ptr,SACCH_MESSAGE_UNIT,l1_invalid_sacch_meas_report,SACCH_MESSAGE_UNIT);
            *num_octets_copied_ptr = SACCH_MESSAGE_UNIT;
         }
         data_not_available = TRUE;

      }

      else 
      {
         if(rsacch_buf_updated == FALSE)
         {
         /* use the buffer message */

         memscpy (op_buffer_ptr,SACCH_MESSAGE_UNIT,sacch_buffer_ptr->sacch_data,
                 SACCH_MESSAGE_UNIT);

         *num_octets_copied_ptr = SACCH_MESSAGE_UNIT;
         }

         sacch_buffer_ptr->data_valid = FALSE;

         /* Tell L2 there is space in the buffer for another message */

         L1isr_send_PH_READY_TO_SEND_IND( SACCH, gas_id );
      }
#ifdef DEBUG_FEATURE_GSM_R_SACCH
      #error code not present
#endif
   }
   else
   {
    /* Message is for a SDCCH or a FACCH */

      /* Check to see if messages are available */

      dcch_buffer_ptr = ded_data->l1_dcch_s0_buffer_ptr;


      if (dcch_buffer_ptr->data_valid == FALSE)
      {
         /* The high buffer is empty */
         /* use the filler message */

         if ( use_filler_message )
         {
            /* test pointer before use */          
            if(op_buffer_ptr)
            {
              /* copy the first 3 octets from the fill frame */
              memscpy(op_buffer_ptr, sizeof(op_buffer_ptr), fill_frame, 3);
              /* then ask GL2 to fill the rest of the buffer */
              l2_generate_filler(op_buffer_ptr+3, DCCH_MESSAGE_UNIT-3, gas_id);

              *num_octets_copied_ptr = DCCH_MESSAGE_UNIT;
            }
            else
            {
              MSG_GERAN_ERROR_0_G("op_buffer ptr NULL!!");
            }

         } 

         data_not_available = TRUE;
      }

      else
      {
        /* test pointer before use */
        if(op_buffer_ptr)
        {
          /* use the buffer message */
          memscpy(op_buffer_ptr,
                DCCH_MESSAGE_UNIT,
                dcch_buffer_ptr->dcch_data,
                DCCH_MESSAGE_UNIT);
#ifdef FEATURE_GSM_COEX
          if(channel_type==SDCCH) /* Don't immediately clear L2 message buffer for FACCH, L1_free_CCH_message() will be used to free the buffer */
#endif
          {
          *num_octets_copied_ptr      = DCCH_MESSAGE_UNIT;
          dcch_buffer_ptr->data_valid = FALSE;

          /* Tell L2 there is space in the buffer for another message */
          L1isr_send_PH_READY_TO_SEND_IND( channel_type, gas_id );
          }
        }
        else
        {
          MSG_GERAN_ERROR_0_G("op_buffer ptr NULL!!");
        }

      }
   }

   return (data_not_available);
}

/*===========================================================================

FUNCTION L1_FREE_CCH_MESSAGE

DESCRIPTION
  This function frees a cch message from the L1 buffers and signals back
  to L2 that there is space in the buffer for another message.

DEPENDENCIES
  channel_type  ( can have values SDCCH, FACCH or SACCH)
  Dedicated data_ptr

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_free_CCH_message(l2_channel_type_T channel_type, dedicated_data_T  *ded_data, gas_id_t gas_id)
{
   if(channel_type == SACCH) /* Message is for the SACCH */
   {
     ded_data->l1_sacch_s0_buffer_ptr->data_valid = FALSE;
   }
   else /* Message is for a SDCCH or a FACCH */
   {
     ded_data->l1_dcch_s0_buffer_ptr->data_valid = FALSE;
   }

   /* Tell L2 there is space in the buffer for another message */
   L1isr_send_PH_READY_TO_SEND_IND( channel_type, gas_id );
}

/*===========================================================================

FUNCTION L1_GET_CCH_FILLER_MESSAGE

DESCRIPTION
  This function gets a filler message for the specified channel

DEPENDENCIES
  channel_type  ( can have values SDCCH, FACCH or SACCH)
  op_buffer_ptr

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_get_CCH_filler_message( l2_channel_type_T channel_type,
                                byte              *op_buffer_ptr,
                                uint16            *num_octets_copied_ptr,
                                gas_id_t           gas_id)
{
   if(channel_type == SACCH) /* Message is for the SACCH */
   {
     memscpy (op_buffer_ptr, sizeof(op_buffer_ptr), l1_invalid_sacch_meas_report,SACCH_MESSAGE_UNIT);
     *num_octets_copied_ptr = SACCH_MESSAGE_UNIT;
   }
   else /* Message is for a SDCCH or a FACCH */
   {
      /* copy the first 3 octets from the fill frame */
      memscpy(op_buffer_ptr, sizeof(op_buffer_ptr), fill_frame, 3);
      /* then ask GL2 to fill the rest of the buffer */
      l2_generate_filler(op_buffer_ptr+3, DCCH_MESSAGE_UNIT-3, gas_id);

      *num_octets_copied_ptr = DCCH_MESSAGE_UNIT;
    }
}


/*===========================================================================

FUNCTION L1_CCH_MESSAGE_PENDING

DESCRIPTION
  This function checks if a cch message from L2 is pending without
  actually retrieving it.

DEPENDENCIES
  channel_type  ( can have values SDCCH, FACCH or SACCH)
  gas_id

RETURN VALUE
  use_filler_message ( there was not one available in the buffer)

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_CCH_message_pending ( l2_channel_type_T channel_type, gas_id_t gas_id )
{
   boolean rc = FALSE;

   if (channel_type == SACCH)
   {
      /* Message is for a SACCH - not currently required */
   }
   else
   {
      /* Message is for a SDCCH or a FACCH */
      rc = l1_dedicated_data[gas_id].l1_dcch_s0_buffer_ptr->data_valid;
   }

   return rc;
}

/*===========================================================================

FUNCTION l1_rsacch_ul_modify_block

DESCRIPTION
  This function performs all UL R-SACCH modifcations to the Block 
DEPENDENCIES
  None
RETURN VALUE
  boolean
SIDE EFFECTS
  None
===========================================================================*/
static boolean l1_rsacch_ul_modify_block (byte    *op_buffer_ptr,
                                          uint16  *num_octets_copied_ptr,
                                          l1_sacch_buffer_T  *sacch_buffer_ptr,
                                          gas_id_t           gas_id)
{
    boolean rsacch_buf_updated=FALSE;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    if((op_buffer_ptr != NULL) && (sacch_buffer_ptr != NULL))
    {
        /*Set SRR bit in the SACCH header*/
        if(l1_dedicated_data_ptr->rsacch_ctrl.l2_repetition_info!=L2_USE_REPETITION_CANDIDATE)
        {
          /*Set SRR bit [Sacch Repeat Request] based on r_sacch_srr_flag
            this would only happen when not sending a repetition block*/
          if(l1_dedicated_data_ptr->rsacch_ctrl.srr_flag==TRUE)
          {
              /*SRR - Octect 1 : Bit 7 (of 8)*/
              *(op_buffer_ptr - L1_SACCH_HDR_LEN) |= L1_SACCH_HDR_SRO_BIT;
          }
          else
          {
              *(op_buffer_ptr - L1_SACCH_HDR_LEN) &= ~L1_SACCH_HDR_SRO_BIT;
          }
        
          MSG_GERAN_HIGH_3_G("R-SACCH: UL SRR flag %d, SRR bit %d, Hdr 0x%x",
                    l1_dedicated_data_ptr->rsacch_ctrl.srr_flag,
                    ((*(op_buffer_ptr - L1_SACCH_HDR_LEN) & L1_SACCH_HDR_SRO_BIT)>>L1_SACCH_SRO_BIT_RIGHT_SHIFT),
                    *(op_buffer_ptr - L1_SACCH_HDR_LEN));
        }
    
        if(l1_dedicated_data_ptr->rsacch_ctrl.l2_repetition_info==L2_USE_REPETITION_CANDIDATE)
        {
          if(l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid==TRUE)
          {  
              /*Replace current buffer with saved RSACCH message*/
              op_buffer_ptr -= L1_SACCH_HDR_LEN;
              memscpy(op_buffer_ptr,
			  	     L1_SACCH_MESSAGE_LEN_23_OCT,
                     &l1_dedicated_data_ptr->rsacch_ctrl.buffer[0],
                     L1_SACCH_MESSAGE_LEN_23_OCT);
              op_buffer_ptr += L1_SACCH_HDR_LEN;
        
              *num_octets_copied_ptr = SACCH_MESSAGE_UNIT;
              rsacch_buf_updated     = TRUE;
        
              MSG_GERAN_HIGH_0_G("R-SACCH: UL USE stored buffer");
          }
          else
          {
              /*Corner Case...L1 reports sro=1 to L2, L1 is then given a new TCH configuration
                all L1 RSACCH params are reset. L2 is unaware and acts on the SRO but L1s Repeat
                buffer is no lonfger valid.
                Fix....L2 will now always send a valid measurement report in the PH_DATA_REQ even 
                if the USE_REPETITION flag is set.*/

              /*Set SRR bit [Sacch Repeat Request] based on r_sacch_srr_flag
                this would only happen when not sending a repetition block*/
              if(l1_dedicated_data_ptr->rsacch_ctrl.srr_flag==TRUE)
              {
                  /*SRR - Octect 1 : Bit 7 (of 8)*/
                  *(op_buffer_ptr - L1_SACCH_HDR_LEN) |= L1_SACCH_HDR_SRO_BIT;
              }
              else
              {
                  *(op_buffer_ptr - L1_SACCH_HDR_LEN) &= ~L1_SACCH_HDR_SRO_BIT;
              }

              MSG_GERAN_HIGH_0_G("R-SACCH: UL USE L1 buffer NOT VALID used default instead");
          }
          l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid = FALSE;
        }
        else if(l1_dedicated_data_ptr->rsacch_ctrl.l2_repetition_info==L2_SET_REPETITION_CANDIDATE)
        {
          if (sacch_buffer_ptr->data_valid == TRUE)
          {
              /*Save SACCH buffer as its a possible Repetition Candidate,
                need to move the ptr back as the Octects 1 and 2 have been added, Pwr + TA*/
              l1_dedicated_data_ptr->rsacch_ctrl.buffer[0] = *(op_buffer_ptr-L1_SACCH_HDR_LEN);
              l1_dedicated_data_ptr->rsacch_ctrl.buffer[1] = *(op_buffer_ptr-(L1_SACCH_HDR_LEN-1));
              memscpy(&l1_dedicated_data_ptr->rsacch_ctrl.buffer[2],
			  	      sizeof(l1_dedicated_data_ptr->rsacch_ctrl.buffer)-2,
                      &sacch_buffer_ptr->sacch_data[0], 
                      (L1_SACCH_MESSAGE_LEN_23_OCT-2));
    
              *num_octets_copied_ptr                     = SACCH_MESSAGE_UNIT;
              l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid = TRUE;

              MSG_GERAN_HIGH_3_G("R-SACCH: UL SET stored Buffer, BufV %d [0]0x%x[1]0x%x",
                        l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid,
                        l1_dedicated_data_ptr->rsacch_ctrl.buffer[0],
                        l1_dedicated_data_ptr->rsacch_ctrl.buffer[1]);
          }
          else
          {
              l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid = FALSE;

              MSG_GERAN_ERROR_3_G("R-SACCH: UL SET data not valid, sacch data valid %d, sacch buf valid %d, RepInfo %d",
                            sacch_buffer_ptr->data_valid,
                            l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid,
                            l1_dedicated_data_ptr->rsacch_ctrl.l2_repetition_info);
          }
        }
        
        /*Set to the default state*/
        l1_dedicated_data_ptr->rsacch_ctrl.l2_repetition_info = L2_NOT_REPETITION_CANDIDATE;
    }
    else
    {
        MSG_GERAN_ERROR_2_G("R-SACCH: pointers are not initailised %d %d",op_buffer_ptr,sacch_buffer_ptr);
    }

    return rsacch_buf_updated;
}

 #ifdef DEBUG_FEATURE_GSM_R_SACCH
#error code not present
#endif

