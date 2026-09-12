/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             GPL1 LOOPBACK TEST MODE UTILITY FUNCTIONS

GENERAL DESCRIPTION

   This module provides utility functions for LOOPBACK TEST MODE B for GPRS and
   EGPRS as well as Switched Radio Block LoopBack mode (SRB)

EXTERNALIZED FUNCTIONS




INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_test.c#1 $

when       who       what, where, why
--------   ---       ----------------------------------------------------------
29/09/14   hd        CR731515: dl_tbf_ptr should be checked if it is NULL whenever this pointer is accessed
15/09/14   hd        CR723813: print dl_tbf_ptr->nts when the pointer is not NULL
11/07/14   pjr       CR692400 Fix KW error
30/05/14   ws        CR657674 Data + MMS Phase 1
07/16/13   zc        CR470197 Replace memcpy with memscpy
26/02/13   ws        CR455036 - X+G DSDA: Crash in GL1 when testing Test mode B loopback mode 
28/11/12   br        Klockwork error fix
26/03/12   jj        CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
18/05/11   sjw       DSDS CR287477: GPRS Test Mode start/stop signals require GAS ID
06/09/10   ab        ENH Add TEST MODE B Reconfig and add asymetric TS support.
15/10/08   ap        CR 160526 Missing UL block in ETSI Loopback Mode B
28/07/06   ws        Don't Transmit dynamic PDCH if we have a NULL pointer - CR 97066
20/04/05   og        Addition of filtered bep params to the msg metrics log packet.
17/3/05    og        Updates to srb loopback handling following integration
                     on raven target.
17/01/05   ws        Lint updates
11/01/05   ws        Updates for TEST mode B for EGPRS
06/01/05   ws        Initial Revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "customer.h"


  #include "comdef.h"
  #include "l1_task.h"
  #include "l1_isr.h"
  #include "l1_utils.h" /* for gl1_get_FN() */
  #include "gpl1_gprs_isr.h"
  #include "gpl1_gprs_log.h"
  #include "gpl1_gprs_serving_cell.h"
  #include "gpl1_gprs_test.h"
  #include <stringl/stringl.h>
  #include "ms.h"
  #include "rr_l1.h"
#include "rr_l1_g.h"

/*===========================================================================

FUNCTION  GPL1_GPRS_GET_RLC_LOOP_DATA

DESCRIPTION
  This function Reads the loopback data out from the RLC UL FIFO for
  Transmission on the Uplink. It then deletes the data from the FIFO



DEPENDENCIES
  None

RETURN VALUE
  boolean - Success/faiure of retrieval of pointer for RLC UL FIFO
SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_get_rlc_loop_data(grlc_ul_fifo_access_block_t   *rlc_config_params,
                                    gas_id_t                      gas_id)
{
  grlc_ul_fifo_op_result_t      rlc_result;


  rlc_config_params->access_mode = READ_MODE;

  rlc_result = grlc_ul_fifo_access_block(gas_id, rlc_config_params);

  if(rlc_result == UL_FIFO_ACCESS_OK)
  {

   /* if read was ok then get blk to remove it */
   rlc_config_params->access_mode=GET_MODE;
   rlc_config_params->get_type = GET_WITH_PREVIOUS_READ;

   rlc_result = grlc_ul_fifo_access_block(gas_id, rlc_config_params);

   if(rlc_result != UL_FIFO_ACCESS_OK)
   {
     MSG_GERAN_LOW_1_G("PRE GET failed!! [%d]",gl1_get_FN( gas_id ));
     return(FALSE);
   }else
   {
     return(TRUE);
   }

  }else
  {
    MSG_GERAN_LOW_1_G("PRE READ failed!! [%d]",gl1_get_FN( gas_id ));
    return(FALSE);
  }
}


/*===========================================================================

FUNCTION  GPL1_GPRS_GET_LOOP_UL_DATA

DESCRIPTION
  This function copies the appropriate data to be transmitted on the uplink
  into a local structure. Transmitting RRBP's or receiving control data will
  result in either loopback data being deleted or previous data being resent.


DEPENDENCIES
  None

RETURN VALUE
  NULL

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_get_loop_ul_data(gas_id_t gas_id)
{

  grlc_ul_fifo_access_block_t   rlc_config_params;
  uint16                        i;

  /*** First get RLC loop back data to Tx and store pointer ***/

  /* Read out data block first */
  for (i=0;i<gpl1_gprs_tm_b_store[gas_id].ul_nts;i++)
  {
    if (gpl1_gprs_get_rlc_loop_data(&rlc_config_params, gas_id) == TRUE)
    {
      /* If we have received a Control block on the downlink slot we need to re-Tx
       * the BSN we sent on the same timeslot in the previous block
       *
       * If we need to Tx an RRBP we need to delete the appropriate BSN for the
       * RLC FIFO.
       *
       * In both cases we don't want to store the data retrieved from the RLC UL FIFO
       */
      if (i==0)
      {
        /* first timeslot */
        if ( (gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status & 0x1) && (!(gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status & 0x1)))
        {
          /* If we have an RRBP block to TX and this doesn't
           * coincide with a Control block on the downlink
           * throw away RLC data.
           */
          MSG_GERAN_MED_1_G("1st tn RLC data disc [%d]",gl1_get_FN( gas_id ));
        }else
        {
          if (gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status & 0x1)
          {
            memscpy(&gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[i+1],sizeof(grlc_ul_fifo_access_block_t),&rlc_config_params,sizeof(grlc_ul_fifo_access_block_t));
          }else
          {
            memscpy(&gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[i],sizeof(grlc_ul_fifo_access_block_t),&rlc_config_params,sizeof(grlc_ul_fifo_access_block_t));
          }
        }

      }else if (i==1)
      {
        /* second timeslot if present */
        if (  (gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status & 0x2) && (!(gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status & 0x2)))
        {
          /* If we have an RRBP block to TX and this doesn't
           * coincide with a Control block on the downlink
           * throw away RLC data
           */
          MSG_GERAN_MED_1_G("2nd tn RLC data disc [%d]",gl1_get_FN( gas_id ));
        }else
        {
          if (gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status & 0x2)
          {
            memscpy(&gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[i+1],sizeof(grlc_ul_fifo_access_block_t),&rlc_config_params,sizeof(grlc_ul_fifo_access_block_t));
          }else
          {
            memscpy(&gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[i],sizeof(grlc_ul_fifo_access_block_t),&rlc_config_params,sizeof(grlc_ul_fifo_access_block_t));
          }
        }
      }
      else if ( i == 2 )
      {
        /* third timeslot if present */
#ifdef FEATURE_GSM_GPRS_MSC33
        if ((gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status & 0x4) && (!(gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status & 0x4)))
        {
          /* If we have an RRBP block to TX and this doesn't
           * coincide with a Control block on the downlink
           * throw away RLC data
           */
          MSG_GERAN_HIGH_1_G( "3rd tn RLC data disc [%d]",gl1_get_FN( gas_id ));
        }
        else
#endif /* FEATURE_GSM_GPRS_MSC33 */
        {
#ifdef FEATURE_GSM_GPRS_MSC33
          if (gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status & 0x4)
          {
            memscpy(&gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[i+1],sizeof(grlc_ul_fifo_access_block_t),&rlc_config_params,sizeof(grlc_ul_fifo_access_block_t));
          }else
#endif /* FEATURE_GSM_GPRS_MSC33 */
          {
            memscpy(&gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[i],sizeof(grlc_ul_fifo_access_block_t),&rlc_config_params,sizeof(grlc_ul_fifo_access_block_t));
          }
        }
      }
      else if ( i == 3 )
      {
        /* 4th timeslot if present , for EDA , this will be 1DL 4UL 2D 4UL CLASS 33*/
        memscpy(&gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[i],sizeof(grlc_ul_fifo_access_block_t),&rlc_config_params,sizeof(grlc_ul_fifo_access_block_t));
      }
    }
    else
    {
      MSG_GERAN_MED_0_G("RLC data Not Ready");
    }
  }
}


/*===========================================================================

FUNCTION  GPL1_GPRS_SET_LOOP_B_DATA

DESCRIPTION

  The function Assigns a pointer to the data to be transmitted from the local
  strucure written to via gpl1_gprs_get_loop_ul_data().

  It also picks up the approprate MCS, PS or CS to use for the transmission

DEPENDENCIES
  None

RETURN VALUE
  pointer to the message to transmit

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_set_loop_b_data(gl1_defs_coding_type *coding_scheme,gl1_defs_puncturing_type puncturing_scheme[],uint8 *msg_data[]
                               , gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK       *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  transfer_data_T               *transfer_data_ptr;
  uint8                         tx_idx = 0;

  /* assign shortcut pointer's for use in function */
  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  tx_idx = transfer_data_ptr->dynamic_tbf_ptr->sched_msgs;

  /* Check RRBP Bitmap for timeslot of RRBP TX
   * If RRBP is Txd on Tn 0 access loopback data from TN 1
   * If RRBP is Txd on Tn 1 access loopback data from TN 0
   */
  if (gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status & 0x1)
  {
    tx_idx = 1;
    MSG_GERAN_HIGH_1_G("RRBP TXD on TN 1 [%d]",gl1_get_FN( gas_id ));
  }

  /* Check tbf mode to work out where data to tx is GPRS or EGPRS */
  if (transfer_data_ptr->egprs_or_gprs_tbf == TBF_MODE_EGPRS)
  {

    /* Check blk_ptr to ensure that it's valid before using it */
    if(gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr == NULL)
    {
      MSG_GERAN_ERROR_1_G(" bad blk_ptr, setting data to NULL! tx_idx=%d",tx_idx);
      /* Assign pointer to current data block */
      msg_data[0] = NULL;
      msg_data[1] = NULL;
      msg_data[2] = NULL;

      /* Setup default Coding scheme and puncturing scheme */
      *coding_scheme = GL1_DEFS_CS1_CODING;
      puncturing_scheme[0] =  GL1_DEFS_PUNCTURING_1;
      puncturing_scheme[1] =  GL1_DEFS_PUNCTURING_1;

    }else
    {
      /* Assign pointer to current data block */
      msg_data[0] = gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr->elm.egprs.hdr;
      msg_data[1] = gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr->elm.egprs.msg1;
      msg_data[2] = gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr->elm.egprs.msg2;

      /* Assign coding scheme value and puncturing schemes  */
      *coding_scheme = (gl1_defs_coding_type)(gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr->elm.egprs.mcs + MSC_CS_OFFSET);
      puncturing_scheme[0] =  (gl1_defs_puncturing_type)(gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr->elm.egprs.ps1);
      puncturing_scheme[1] =  (gl1_defs_puncturing_type)(gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr->elm.egprs.ps2);
    }

  } else
  {
     /* Check blk_ptr to ensure that it's valid before using it */
    if(gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr == NULL)
    {
      MSG_GERAN_ERROR_0_G(" bad blk_ptr, setting data to NULL!");

      /* Setup default Coding scheme */
      msg_data[0] = NULL;
      *coding_scheme = GL1_DEFS_CS1_CODING;

    }else
    {
      /* Assign pointer to current data block */
      msg_data[0] = gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr->elm.gprs.block;

      /* Assign coding scheme value  */
      *coding_scheme = (gl1_defs_coding_type)gpl1_gprs_tm_b_store[gas_id].gpl1_gprs_tx_loop[tx_idx].blk_ptr->elm.gprs.cs;

      /* No need to set puncturing schemes for CS1-4, NPL1 only needs them for MCS1-9 */
    }

  }


}


/*===========================================================================

FUNCTION  GPL1_GPRS_IS_RLC_DL_DATA_DISC

DESCRIPTION

  The function determines what received RLC data needs to be discarded and what data
  should be passed up to RLC  in terms up number of blocks to be looped back on the Uplink
  The function considers various UL/DL Ts allocations.

DEPENDENCIES
  None

RETURN VALUE
  0: Discard RLC DL data
  num : Pass RLC data block into DL FIFO

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_is_rlc_dl_data_disc(gl1_defs_tn_type rx_tn, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint8               data_blk_cnt = 0;
  transfer_data_T     *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  if (transfer_data_ptr->dl_tbf_ptr == NULL)
  {
    return data_blk_cnt;
  }

  /* Loop back based on num of dl ts */
  switch (transfer_data_ptr->dl_tbf_ptr->nts)
  {
    case 1 : /* 1 dl_nts */
      switch (gpl1_gprs_tm_b_store[gas_id].ul_nts) /* ul_nts */
      {
        case 1 : /* 1 dl 1 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
          {
            data_blk_cnt = 1;
          }
          break;

        case 2 : /* 1 dl 2 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
          {
            data_blk_cnt = 2;
          }
          break;

        case 3 : /* 1 dl 3 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
          {
            data_blk_cnt = 3;
          }
          break;

        case 4 : /* 1 dl 4 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
          {
            data_blk_cnt = 4;
          }
          break;

#ifdef FEATURE_GSM_GPRS_MSC33
        case 5 :  /* 1 dl 5 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
          {
            data_blk_cnt = 5;
          }
          break;
#endif /* FEATURE_GSM_GPRS_MSC33 */

        default :
          MSG_GERAN_ERROR_1_G("TEST_MODE_B  invalid ul ts %d",gpl1_gprs_tm_b_store[gas_id].ul_nts);
         break;
      }
      break;

    case 2 :  /* 2 dl_nts */

      switch (gpl1_gprs_tm_b_store[gas_id].ul_nts) /* ul_nts */
      {
        case 1 : /* 2 dl 1 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
          {
            data_blk_cnt = 1;
          }
          break;

        case 2: /* 2 dl 2 ul */
          /* always data block = 1*/
          data_blk_cnt = 1;
          break;

        case 3 : /* 2 dl 3 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
          {
            data_blk_cnt = 1;
          }

          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[1])
           {
              data_blk_cnt = 2;
           }
           break;

#ifdef FEATURE_GSM_GPRS_MSC33
        case 4 : /* 2 dl 4 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
          {
            data_blk_cnt = 1;
          }

          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[1])
          {
            data_blk_cnt = 3;
          }
          break;
#endif /* FEATURE_GSM_GPRS_MSC33 */
        default:
          MSG_GERAN_ERROR_1_G("TEST_MODE_B  invalid ul ts %d",gpl1_gprs_tm_b_store[gas_id].ul_nts);
          break;
      }
      break;

    case 3 :  /* 3 dl_nts */

      switch (gpl1_gprs_tm_b_store[gas_id].ul_nts) /* ul_nts */
      {
        case 1 : /* 3 dl 1 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
      {
        data_blk_cnt = 1;
      }
      break;

        case 2 : /* 3 dl 2 ul */
          if (((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0]) ||
              ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[1]))
          {
            data_blk_cnt = 1;
          }
          break;

#ifdef  FEATURE_GSM_GPRS_MSC33
        case 3 : /* 3 dl 3 ul */
          /* always data block = 1*/
          data_blk_cnt = 1;
          break;
#endif /* FEATURE_GSM_GPRS_MSC33 */

    default:
          MSG_GERAN_ERROR_1_G("TEST_MODE_B  invalid ul ts %d",gpl1_gprs_tm_b_store[gas_id].ul_nts);
        break;
      }
      break;

    case 4 :  /* 4 dl_nts */

      switch (gpl1_gprs_tm_b_store[gas_id].ul_nts) /* ul_nts */
       {
        case 1 : /* 4 dl 1 ul */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
           {
             data_blk_cnt = 1;
           }
           break;

#ifdef FEATURE_GSM_GPRS_MSC33
        case 2 : /* 4 dl 2 ul */
          if (((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0]) ||
             ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[1]))
          {
            data_blk_cnt = 1;
          }
          break;
#endif /* FEATURE_GSM_GPRS_MSC33 */

        default :
          MSG_GERAN_ERROR_1_G("TEST_MODE_B  invalid ul ts %d",gpl1_gprs_tm_b_store[gas_id].ul_nts);
          break;
      }
      break;

#ifdef FEATURE_GSM_GPRS_MSC33
    case 5 :  /* 5 dl_nts */

      switch (gpl1_gprs_tm_b_store[gas_id].ul_nts) /* ul_nts */
      {
        case 1 : /* 5 DL 1 UL */
          if ((uint8)rx_tn == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
          {
            data_blk_cnt = 1;
          }
          break;

        default :
          MSG_GERAN_ERROR_1_G("TEST_MODE_B  invalid ul ts %d",gpl1_gprs_tm_b_store[gas_id].ul_nts);
          break;
}
      break;
#endif /* FEATURE_GSM_GPRS_MSC33 */

    default :
       MSG_GERAN_ERROR_1_G("TEST_MODE_B  invalid dl ts %d",transfer_data_ptr->dl_tbf_ptr->nts);
       break;
  }

  return data_blk_cnt;
}  /* gpl1_gprs_is_rlc_dl_data_disc_new */


/*===========================================================================

FUNCTION  GPL1_GPRS_SETUP_TEST_B

DESCRIPTION
  This function Determines which downlink timeslots need to be looped back
  on the uplink. This is done based on test_mode_tn which indicates the
  first burst to loopback.
DEPENDENCIES
  None

RETURN VALUE
  NULL
SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_setup_test_B(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  uint8               usl_low_tn,usl_high_tn;
  transfer_data_T     *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  usl_low_tn = gpl1_gprs_ts_conv_unslam(gprs_serv_cell_meas_ptr->lowest_tn.tn,gas_id);
  usl_high_tn = gpl1_gprs_ts_conv_unslam(gprs_serv_cell_meas_ptr->highest_tn,gas_id);

  if(transfer_data_ptr->dl_tbf_ptr != NULL)
  {
    MSG_GERAN_HIGH_3_G(" test_tn %d dl_nts %d ul_nts %d",
                        transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn,
                        transfer_data_ptr->dl_tbf_ptr->nts,
                        gpl1_gprs_tm_b_store[gas_id].ul_nts);
  }
  else
  {
    MSG_GERAN_HIGH_2_G(" test_tn %d ul_nts %d",
                        transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn,
                        gpl1_gprs_tm_b_store[gas_id].ul_nts);
  }
  if (transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn > usl_high_tn)
  {
    gpl1_gprs_tm_b_store[gas_id].ul_tn[0] = (usl_low_tn + (transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn - usl_high_tn) -1);

    /* Special case for single slot operation */
    if (usl_low_tn == usl_high_tn)
    {
      gpl1_gprs_tm_b_store[gas_id].ul_tn[0] = usl_low_tn;
    }
  }
  else
  {
    gpl1_gprs_tm_b_store[gas_id].ul_tn[0] = transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn;
  }

  /* As DL Ts data is reflected on UL , ul_tn[] should only be valid if DL is being loop back */
  if ( (transfer_data_ptr->dl_tbf_ptr != NULL) && (transfer_data_ptr->dl_tbf_ptr->nts >= 2) && (gpl1_gprs_tm_b_store[gas_id].ul_nts > 1) )
  {
    if ((transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn + 1) > usl_high_tn)
    {
      gpl1_gprs_tm_b_store[gas_id].ul_tn[1] = (usl_low_tn + (transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn - usl_high_tn));
    }else
    {
      gpl1_gprs_tm_b_store[gas_id].ul_tn[1] = (transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn + 1);
    }
  }
  else
  {
    /* Initialize , helps when changing from 3DL2UL to 1DL1UL */
    gpl1_gprs_tm_b_store[gas_id].ul_tn[1] = 0xFF;
  }

#ifdef  FEATURE_GSM_GPRS_MSC33
  if ( (transfer_data_ptr->dl_tbf_ptr != NULL) && (gpl1_gprs_tm_b_store[gas_id].ul_nts == 3) && (transfer_data_ptr->dl_tbf_ptr->nts == 3))
  {
    gpl1_gprs_tm_b_store[gas_id].ul_tn[2] = gpl1_gprs_tm_b_store[gas_id].ul_tn[1] + 1;
  }
  else
  {
    /* Initialize , helps when changing from 3DL2UL to 1DL1UL */
    gpl1_gprs_tm_b_store[gas_id].ul_tn[2] = 0xFF;
  }
#else
    /* Initialize, */
    gpl1_gprs_tm_b_store[gas_id].ul_tn[2] = 0xFF;
#endif /* FEATURE_GSM_GPRS_MSC33 */

  MSG_GERAN_MED_3_G(" TMB setup TEST_MODE_B tn[0] %d tn[1] %d tn[2] %d",
                            gpl1_gprs_tm_b_store[gas_id].ul_tn[0],
                            gpl1_gprs_tm_b_store[gas_id].ul_tn[1],
                            gpl1_gprs_tm_b_store[gas_id].ul_tn[2]);
}

/*===========================================================================

FUNCTION  gpl1_l1_test_mode_start_ind( )

DESCRIPTION
  Posts MPH_L1_TEST_MODE_START_IND to RR.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_l1_test_mode_start_ind(  gas_id_t gas_id )
{
  /* statically allocate memory for the message */
  mph_l1_test_mode_start_ind_T  mph_l1_test_mode_start_ind;

  /* Fill in the message header */
  mph_l1_test_mode_start_ind.message_header.message_set = (uint8) MS_RR_L1;
  mph_l1_test_mode_start_ind.message_header.message_id  = (uint8) MPH_L1_TEST_MODE_START_IND;

  /* Post the message to RR */
  PUT_IMH_LEN( sizeof ( mph_l1_test_mode_start_ind_T ) - sizeof ( IMH_T ),
               &mph_l1_test_mode_start_ind ); /*lint !e778: constant evaluates to zero */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  mph_l1_test_mode_start_ind.gas_id = gas_id;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  ( void ) L1_isr_send_message( GS_QUEUE_RR,
                                &mph_l1_test_mode_start_ind,
                                TRUE, gas_id ); /* Make sure message is sent */

  MSG_GERAN_MED_3_G( " PL1 -> RR: MPH_L1_TEST_MODE_START_IND [%d]",
           gl1_get_FN( gas_id  ),
           0,
           0 );
}

/*===========================================================================

FUNCTION  gpl1_l1_test_mode_stop_ind( )

DESCRIPTION
  Posts MPH_L1_TEST_MODE_STOP_IND to RR.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_l1_test_mode_stop_ind( gas_id_t gas_id )
{
  /* statically allocate memory for the message */
  mph_l1_test_mode_stop_ind_T  mph_l1_test_mode_stop_ind;

  /* Fill in the message header */
  mph_l1_test_mode_stop_ind.message_header.message_set = (uint8) MS_RR_L1;
  mph_l1_test_mode_stop_ind.message_header.message_id  = (uint8) MPH_L1_TEST_MODE_STOP_IND;

  /* Post the message to RR */
  PUT_IMH_LEN( sizeof ( mph_l1_test_mode_stop_ind_T ) - sizeof ( IMH_T ),
               &mph_l1_test_mode_stop_ind ); /*lint !e778: constant evaluates to zero */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  mph_l1_test_mode_stop_ind.gas_id = gas_id;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  ( void ) L1_isr_send_message( GS_QUEUE_RR,
                                &mph_l1_test_mode_stop_ind,
                                TRUE, gas_id ); /* Make sure message is sent */

  MSG_GERAN_MED_3_G( " PL1 -> RR: MPH_L1_TEST_MODE_STOP_IND [%d]",
           gl1_get_FN( gas_id  ),
           0,
           0 );
}
