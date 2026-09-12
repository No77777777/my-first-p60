#ifndef TDSDLPICH_H
#define TDSDLPICH_H

/*===========================================================================
                 D L P I C H M G R . H

GENERAL DESCRIPTION
  This file contains the declarations and definitions of dlpichmgr needed
  across modules.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2007 - 2008 by Qualcomm Technologies Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdlpich.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
09/21/10    jintaoh      Initial version
                    ===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "tdsl1const.h"
#include "tdsdlchmgr.h"
#include "tdsdrxmgr.h"

/* This is the value we use to return an invalid sfn */
#define TDSDLPICH_PO_INVALID_SFN  0xFFFF

typedef struct
{
  /* paging occasion = (po_in_drx_cycle + n * DRX cycle length) *2 + Pi_q/(Npi/2)*/
  uint16 po;

  /* the po position that register to drx module*/
  uint16 po_register_to_drx;

  /*when receiv paging indicator, record this po, for PCH use*/
  uint16 po_when_receiv_pi;

  /* pich_sub_sfn = (paging occasion )*2 + Pi_q/(Npi/2) */
  uint16 pich_sub_sfn;

  /* sub-frame number contain the correct PI  for this UE*/
  uint16 next_pich_sub_sfn; 

  /* paging occasion in a drx cycle =  
         [ (IMSI / K) % (DRX cycle length / PBP) ] * PBP + frame offset + PI_n*/
  uint16 po_in_drx_cycle;

  /*pch paging occasion in a drx cycle =  
        = Paging Occasion + N_PICH + N_GAP + [ (DRX Index % NP) % NPCH ] * 2 + PI_n */
  uint16 pch_in_drx_cycle;

  /*the difference in frames between pich and pch in the same paging period*/
  uint16 diff_pich_pch ;

  /* DRX Cycle Length in 10 ms radio-frame units, MAX(2^k, PBP) */
  uint16 drx_cycle_len;

  /*25.331: 10.3.6.49
      Paging Block Periodicity,  i.e.  PICH Repetition period, Integer (4; 8; 16; 32; 64)*/
  uint8 pbp;

  /*25.331: 10.3.6.49
      PICH repetition length,  Indicates the number of radio frames for PICH transmission . 
      Default value is 4. range: 2; 4 */
  uint8 Npich;

  /*25.331: 10.3.6.49
      frame offset,  Integer (0...pbp -1)*/
  uint8 frame_offset;

  /*25.331: 10.3.6.49
      Indicates the length of one paging indicator in Bits.  Default value is 4. Integer (4, 8, 16)*/
  uint8 Lpi;
  
  /* The number of Page Indicator per radio frame; 
        =  352 (bits)/Lpi,  wherein, Lpi is the Paging indicator length (in bits)*/
  uint8 Npi;

  /* 25.304: 8.3
        The number of Page Indicator per paging block, Np
        =  pi_per_frame * Npich  */
  uint16 Np;

  /* 25.304: 8.3
        to Indicate which Paging Indicator should be decoded in a paging block
        = (IMSI/8192)%Np, wherein,  Np is The number of Page Indicator per paging block*/
  uint32 PI;
  
  /* 25.304: 8.3
          the actual PI for this UE is the qth Paging indicator in the nth sub-frame of one PICH Block
          PI_n = PI /(Npi);  
          PI_q = PI %(Npi)  */
  uint8  PI_n;
  uint8  PI_q;

  /*25.331: 10.3.6.49
        Number of frames between the last frame carrying PICH for this Paging Occasion and 
        the first frame carrying paging messages for this Paging Occasion.
        Default value is 4.,   Integer(2, 4, 8)*/
  uint8 Ngap;

  /*25.331: 10.3.6.49
        Number of paging groups. Default value is 2.  Integer(1  ..8)*/
  uint8 Npch;


  /* IMSI div K, as per 8.3 in 25.304 (version 2000-12), used in calculating paging
     occasion, where K is the number of SCCPCHs that carry a PCH.
     Obtained from RRC during SCCPCH setup*/
  uint64 imsi_div_pch_cnt;

  /* IMSI div 8192, as per 8.3 in 25.304 (version 2001-06), used in calculating PI.
     Obtained from RRC during SCCPCH setup*/
  uint64 imsi_div_8192;

  /* CCTrCh table index of the SCCPCH that carries the paging blocks */
  uint8 sccpch_cctrch_idx;

  //TODO:  check whether this variable should be removed. it's not used now
  /* Threshold for detecting PI bits; required by the mdsp to provide a Y/N decision whether
     the PI bits were +1 or -1 */
  int16 mdsp_pi_threshold;

} tdsdlpich_parms_struct_type;

/* This structure holds the reponse functions for channel type: PICH*/
extern tdsdlchmgr_resp_func_cb_struct_type tdsdlpich_resp_func_cb;

/* This structure holds the index management callback functions to pich manager*/
extern tdsdlchmgr_idx_mgmt_func_struct_type tdsdlpich_idx_mgmt_func;

/* This structure holds the pre and post operation callbacks for pich*/
extern tdsdlchmgr_op_cb_func_struct_type tdsdlpich_op_cb_func;

extern tdsdrxmgr_channel_cb_struct_type  tdsdlpich_drx_cb_functions;

extern tdsdlchmgr_schedule_para_cb_func_struct_type tdsdlpich_schedule_para_cb_func;

extern tdsdlpich_parms_struct_type tdsdlpich_parms;

//extern boolean tdsdlpich_actual_wakeup;

/*===========================================================================
FUNCTION        tdsdlpich_init

DESCRIPTION     This function initializes the pich manager. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void tdsdlpich_init( void );

/*===========================================================================
FUNCTION        tdsdlpich_get_chmgr_cfg_idx

DESCRIPTION     This function returns the cfg index of PICH channel in the 
                of the channel.channel manager cfg db. SCCPCH manager can 
                use the returned index to do operations on PICH.

DEPENDENCIES    None

RETURN VALUE
                cfg-db-idx: The cfg db index of PICH in chmgr.

SIDE EFFECTS    None.
===========================================================================*/
extern tdsdlchmgr_cfg_db_idx_type tdsdlpich_get_chmgr_cfg_idx( void );

/*===========================================================================
FUNCTION        tdsdlpich_pre_cell_trans_cb

DESCRIPTION     This function should be called by DLChMgr before starting to process 
                CPHY_CELL_TRANSITION_REQ from RRC.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void tdsdlpich_pre_cell_trans_cb(void);


/*===========================================================================
FUNCTION        tdsdlpich_get_bitmask_cb

DESCRIPTION     This function calculate the bitmask for pich.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
tdsdlchmgr_spec_tdm_bitmask_type tdsdlpich_get_bitmask_cb(uint16 activation_time);

/*===========================================================================
FUNCTION     PICH_NEXT_FRAME_WANTED_CB

DESCRIPTION
  DRX Manager invokes this Channel Manager callback function to find out
  PICH scheduling information, for example if it wants to go to sleep and
  for how long.

DEPENDENCIES
  None.

PARAMETERS
  entity_handle - DRX handle identifying PICH.

RETURN VALUE
  sleep_info_ptr - Pointer to sleep information returned to the caller.
===========================================================================*/
extern boolean tdsdlpich_next_frame_wanted_cb(
  tdsdrxmgr_entity_handle_type  entity_handle,
  tdsdrxmgr_sleep_struct_type  *sleep_info_ptr);

//#endif
/*===========================================================================
FUNCTION        tdsdlpich_is_both_pich_sccpch_op_done

DESCRIPTION     This function returns whether both SCCPCH0 and PICH operations 
                have been completed after they have been started together. This 
                helps in sending a single response to L1M.

DEPENDENCIES    None

RETURN VALUE
                TRUE: Both SCCPCH0 and PICH operations are done.
                FALSE: At least one among the SCCPCH0 and PICH operations is due.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean tdsdlpich_is_both_pich_sccpch_op_done(void);

/*===========================================================================
FUNCTION        tdsdlpich_reset_pich_sccpch_op_status

DESCRIPTION     This function reset pich and sccpch operation status

DEPENDENCIES    None

RETURN VALUE
               
SIDE EFFECTS    None.
===========================================================================*/
extern void tdsdlpich_reset_pich_sccpch_op_status(void);

/*===========================================================================
FUNCTION        tdsdlpich_upd_sccpch_op_status

DESCRIPTION     This function marks that the SCCPCH operation has been 
                completed in the PICH cfg db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void tdsdlpich_upd_sccpch_op_status(void);

/*===========================================================================
FUNCTION        tdsdlpich_post_cell_trans_cb

DESCRIPTION     This function should be called by DlChMgr before sending response for 
                TDSL1_CPHY_CELL_TRANSITION_REQ to RRC.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void tdsdlpich_post_cell_trans_cb(boolean cell_trans_status);

/*===========================================================================
FUNCTION        tdsdlpich_get_cfg_db_status

DESCRIPTION     This function should be called by DlChMgr before sending response for 
                CPHY_CELL_TRANSITION_REQ to RRC.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern boolean  tdsdlpich_get_cfg_db_status(void);

/*===========================================================================
FUNCTION        tdsdlpich_set_drx_cycle_len

DESCRIPTION     This function set pich parameters of drx cycle length,called by drx.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void tdsdlpich_set_drx_cycle_len(uint16 drx_cycle_len);

/*===========================================================================
FUNCTION        tdsdlpich_get_actual_systemframe_pch

DESCRIPTION     This function return actual pch in subframe.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern uint16 tdsdlpich_get_actual_systemframe_pch(void);
/*===========================================================================
FUNCTION        tdsdlpich_calc_po_in_npccpch_bm

DESCRIPTION     This function return pich po from NBCH bitmask which is not conflicted

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern uint16 tdsdlpich_calc_po_in_npccpch_bm(uint16 current_po_sub_sfn);
/*===========================================================================
FUNCTION        tdsdlpich_get_next_subsfn_pich_for_wakeup_cancel

DESCRIPTION     This function return next pich paging ocassion in subframe. 
   IE. the PO is x, then if input is[x-1, x+drx*2 -2] then return x+drx*2

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern boolean tdsdlpich_get_next_subsfn_pich_for_wakeup_cancel(
  tdsdrxmgr_entity_handle_type         entity_handle,
  tdsdrxmgr_sleep_struct_type *sleep_info_ptr,
  uint16 subsfn_input);
/*===========================================================================
FUNCTION        tdsdlpich_get_drx_cycle_len

DESCRIPTION     This function set pich parameters of drx cycle length,called by drx.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern uint16 tdsdlpich_get_drx_cycle_len(void);

/*===========================================================================
FUNCTION        tdsdlpich_get_sfn_pch_in_first_drx

DESCRIPTION     This function return pch in first drx.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern uint16 tdsdlpich_get_sfn_pch_in_first_drx(void);

//n-pccpch high priority than pich
/*===========================================================================
FUNCTION        tdsdlpich_get_actual_subsfn_pich

DESCRIPTION     This function return actual pich paging ocassion in subframe.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern uint16 tdsdlpich_get_actual_subsfn_pich( uint16 current_sub_sfn);

/*===========================================================================
FUNCTION        tdsdlpich_record_po_when_receive_pi

DESCRIPTION     This function return actual pich position 
                which receive paging indicator for calculate pch position

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlpich_record_po_when_receive_pi(void);

/*===========================================================================
FUNCTION        tdsdlpich_record_po_when_receive_pi

DESCRIPTION     This function return actual pich position 
                which receive paging indicator for calculate pch position

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
uint16 tdsdlpich_get_po_when_receive_pi(void);

/*===========================================================================
FUNCTION        tdsdlpich_get_register_drx_subsfn_pich

DESCRIPTION     This function return actual pich position 
                which has register to DRX last time.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern uint16 tdsdlpich_get_register_drx_subsfn_pich(void);

#endif

