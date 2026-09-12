#ifndef SRCHMEAS_H
#define SRCHMEAS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            S R C H M E A S . H                  

GENERAL DESCRIPTION
  This module handles periodic measurement requests from RRC.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000-2012 by Qualcomm Technologies, Inc. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/srchmeas.h_v   1.0   11 Mar 2002 16:57:06   cemani  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/srchmeas.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/11/19   ac      Searcher Mini Dump Changes
07/13/16   bj      Backout CR 1032140 for srchmeas and srchdlif memset 
06/29/16   bj      Search set cell dangling pointers cleanup in WL1 searcher module.
01/20/15   jd      Global re-org for W + W
07/01/14   ar      Added a parameter to srchmeas_copy_cell_info_intra_to_inter
05/29/14   ag      R10 periodic measurement support on second carrier.
05/13/14   ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
03/10/14   ks/cc   Add LTE and GSM PLT cmds
01/13/14   jk      DC-HSUPA Interf Spec CR Changes.
01/02/14   da      Fixed search measurement related KW errors
09/12/13   jd      Searcher redesign 
01/18/13   vn      Externing function srchmeas_get_meas_idx_based_on_id.
02/08/12   vs      Feature cleanup.
09/03/11   sa      Protected filter database shared between L1 and GPS task 
                   under critical section.
11/02/10   nd      Added code changes for FEATURE_WCDMA_DEFERRED_SIB11_12_READING
05/03/10   sv      Fixed compiler warnings.
09/09/09   rvs     Added function srchmeas_get_cme_filt_idx() to get filter
                   index of Event 1A for CME if available.
06/26/09   sa      Added MCM failure cause enum data type.
04/22/09   stk     Compiler warning fixes	
10/03/08   hk      Bringing in 7k Mailine fixes
03/17/07   sv      Externed the "srchmeas_print_cell_info" function. 
11/29/06   nd      Added multiple layer3 filter support for intra freq measurement
                   under FEATURE_WCDMA_MULTIPLE_L3_FILTERS_SUPPORTED
11/01/06   cpe     Mainlined the following features:
                    -FEATURE_STTD_CAPABLE
08/23/06   rc      Added support for inter-freq virtual active set under 
                   FEATURE_WCDMA_VASET_SUPPORT.
06/05/06   sv      Added prototype for update cells with meas info function.
01/10/06   ub      Added prototypes for cell info copy functions
08/05/05   ub      Added function prototypes for initializing meas filter
04/11/05   cpe     Code addition for resuming to old config upon HHO failure. 
05/26/04   cpe     Added an extern
12/09/03   yshi    Support for GSM Quadband
11/13/03   src     Added declaration for srchmeas_ext_cntrl_params.srchmeas_intra_freq_meas_performed
                   to track measurements when HHOs are performed.
11/12/03   cpe     Extern'ed a few functions.
11/04/03   cpe/src Added declaration for srchmeas_determine_sttd_behavior.
10/31/03   cpe     Removed the FEATURE_WCDMA_MULTIPLE_INTRA_MEAS_SUPPORTED 
                   feature which is always present
05/23/03   cpe     Added support for multiple intra freq measurements
03/06/03   djm     addition of srchmeas_inter_s_find_cell, a find
                   of GSM cells based on ARFCN, BSIC.
10/01/02   ddh     Added changes for InterRat processing
09/11/01   cpe     Created file.

===========================================================================*/
#include "wcdma_variation.h"
#include "customer.h"
#include "l1rrcif.h"
#include "srchset.h"
#include "srchinterf.h"
#include "wsrchi.h"

#include "srchsetutil.h"


typedef struct
{
  boolean meas_1a_existed_before;
  boolean meas_1d_existed_before;
  boolean meas_1e_existed_before;
} srchmeas_existing_meas_type;

#define SRCH_FILTER_COEF_TABLE_K_VALUE              20
#define SRCH_FILTER_COEF_TABLE_PN_SRCH_INTERVAL      5

/***************************************************************************

    D A T A     S T R U C T U R E S 
    
 ***************************************************************************/
 /* This will check the cause of the failure of MCM in l1 */
 typedef enum 
 {  
   SRCHMEAS_MCM_FAILURE_CAUSE_NO_FAILURE,
   SRCHMEAS_MCM_FAILURE_CAUSE_NUM_MEAS_EXCEED
 } srchmeas_mcm_failure_cause_enum_type;
 
#define MAX_INTRAF_NEIGHBOR_CELLS 32
       
/*=========================================================================

    V A R I A B L E     D E C L A R A T I O N S


 ===========================================================================*/

/* This global will keep track of the situation when there are no NW configured
filter. In that case the default filter will be used. Later when NW configures a filter
this flag will be turned false */

       
#define SRCH_MEAS_FILT_DB_LOCK() REX_ISR_LOCK(&wsrch_ext_cntrl_params[wsub_id].srch_meas_filt_crit_sec)
#define SRCH_MEAS_FILT_DB_FREE() REX_ISR_UNLOCK(&wsrch_ext_cntrl_params[wsub_id].srch_meas_filt_crit_sec)

typedef struct
{
  boolean srchmeas_no_filt_configured_frm_nw;

  /* To check the MCM failure cause */
  srchmeas_mcm_failure_cause_enum_type srchmeas_mcm_failure_cause;

  /* Intra frequency measuremtn which are ongoing */
  uint16 srch_num_intraf_meas_in_progress[SRCHMEAS_MAX_NUM_CARR];

  /* The following variable determines if the intra frequency measurements need to be
    restarted after an HHO. If the variable is set to TRUE; then intra frequency measure
    -ments are performed */
  boolean srchmeas_intra_freq_meas_performed;

  uint16 srch_intraf_filt_coeff_val;
  srch_intraf_filt_idx_struct_type srch_intraf_filt_idx_s;
  srch_intraf_rach_rpt_info_struct_type srch_intraf_rach_rpt_info_s;
}srchmeas_ext_cntrl_params_mdump_type;

typedef struct
{
  boolean srch_sib_mod_meas_req_update;
  boolean srch_no_internal_measurements_as_hho_in_progress;
  boolean srch_no_inter_rat_meas_as_hho_in_progress;
  #ifdef FEATURE_WCDMA_TO_LTE
  #ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
  boolean srch_sib_mod_prior_lte_meas_req_update;
  #endif

  #endif

  #ifdef FEATURE_FEMTO_CSG
  boolean srch_sib_mod_csg_meas_req_update;
  #endif

  boolean srch_sib_mod_inter_f_meas_req_update;

  /* The following flags block measurements from taking place during HHO */
  boolean srch_no_inter_freq_meas_as_hho_in_progress;
  uint16 srch_intraf_filt_coeff_aset_len;
  uint16 srch_intraf_filt_coeff_aset_rxdiv_len;
  boolean srch_intraf_evt_trig_rpts_requested[SRCHMEAS_MAX_NUM_CARR];
  uint16 srch_intraf_filt_coeff_nset_len;
  boolean srch_intraf_periodic_rpts_requested[SRCHMEAS_MAX_NUM_CARR];

  /* Saved CPHY_MEASUREMENT_REQ, used during SIB modification update */
  l1_meas_cmd_type srch_saved_meas_req_cmd;
  l1_meas_cmd_type srch_saved_inter_f_meas_req_cmd;
  #ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
  l1_meas_cmd_type srch_saved_prior_lte_meas_req_cmd;
  #endif

  #ifdef FEATURE_FEMTO_CSG
  l1_meas_cmd_type srch_saved_csg_meas_req_cmd;
  #endif

  #ifdef FEATURE_WCDMA_INTER_FREQ_RACH_REPORTING
  #error code not present
#endif

  srch_intra_freq_meas_parms_struct_type srch_intra_freq_meas_db[SRCHMEAS_MAX_NUM_CARR][SRCH_MAX_INTRAF_MEAS_ALLOWED];

  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  srchmeas_ext_cntrl_params_mdump_type srchmeas_ext_cntrl_params_mdump;
}srchmeas_ext_cntrl_params_type;

extern srchmeas_ext_cntrl_params_type srchmeas_ext_cntrl_params[WCDMA_NUM_SUBS];

typedef struct
{
  uint16 srch_num_saved_intraf_meas_during_hho[SRCHMEAS_MAX_NUM_CARR];
}srchmeas_int_cntrl_params_mdump_type;

typedef struct
{

  srch_cell_struct_type *tmp_cell_ptr_array[MAX_INTRAF_NEIGHBOR_CELLS];
  srch_intra_freq_meas_parms_struct_type srch_saved_intra_freq_meas_db[SRCHMEAS_MAX_NUM_CARR][SRCH_MAX_INTRAF_MEAS_ALLOWED];
  #ifdef FEATURE_FEMTO_CSG
  l1_meas_parm_type srch_saved_csg_meas_parm;
  #endif

  l1_meas_parm_type srch_saved_meas_parm;
  l1_meas_parm_type srch_saved_inter_f_meas_parm;
  #ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
  l1_meas_parm_type srch_saved_prior_lte_meas_parm;
  #endif
  
  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  srchmeas_int_cntrl_params_mdump_type srchmeas_int_cntrl_params_mdump;
}srchmeas_int_cntrl_params_type;

extern uint16 srchmeas_get_meas_idx_based_on_id(uint8, uint16, wsub_id_e_type wsub_id);
extern boolean srchmeas_inter_s_sib_update(l1_meas_cmd_type *, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION        SRCHMEAS_RESET_NEIGHBOR_CELL_LISTS
FILE NAME       

DESCRIPTION     This function resets the neighbor cell lists

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void srchmeas_reset_neighbor_cell_lists(uint8, wsub_id_e_type wsub_id);       
       
/*===========================================================================
FUNCTION        SRCHMEAS_SEND_PERIODIC_MEAS_REPORT
FILE NAME       

DESCRIPTION     This function prepares and sends the measurement report
                to RRC
DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void srchmeas_send_periodic_meas_report(uint8 carr_idx,uint16 meas_id, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     SRCHMEAS_INTER_S_FIND_CELL

DESCRIPTION
  This function will search all neighbor set cells to find one that
  has an ARFCN and BSIC that match those provided.

DEPENDENCIES
  None

RETURN VALUE
  srch_gsm_cell_struct_type * - a pointer to the GSM cell for which
                                <ARFCN,BSIC> match, NULL if not found.

SIDE EFFECTS
  None
===========================================================================*/
extern srch_gsm_cell_struct_type * srchmeas_inter_s_find_cell(
                                              W_ARFCN_T arfcn,
                                              uint8 bsic_ncc,
                                              uint8 bsic_bcc
                                            , wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION SRCHMEAS_INTER_S_RMV_CELL

DESCRIPTION
    Used to remove cell one cell from the inter_rat cell list

RETURN VALUE
    boolean
=======================================================================*/
extern void srchmeas_inter_s_rmv_cell( uint16 cellidx, srch_gsm_set_enum_type  set , wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION SRCHMEAS_INTER_S_RMV_CELLS

DESCRIPTION
    Used to remove cells from the Inter-RAT Neighbor set and srch list 
    using the information received in a CPHY_MEASUREMENT_REQ message.

RETURN VALUE
    boolean
=======================================================================*/
void srchmeas_inter_s_rmv_cells
(
  l1_inter_sys_cell_list_struct_type *cell_list
, wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION SRCHMEAS_INTER_S_RMV_ALL_CELLS

DESCRIPTION
    Used to remove *all* cells from the Inter-RAT database

RETURN VALUE
    None
=======================================================================*/
extern void srchmeas_inter_s_rmv_all_cells ( wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION SRCHMEAS_INTER_S_GET_CELLPTR

DESCRIPTION
    Used to add cells to the Inter-RAT Neighbor set and srch list using
    the information received in a CPHY_MEASUREMENT_REQ command message.

RETURN VALUE
    boolean

=======================================================================*/
srch_gsm_cell_struct_type * srchmeas_inter_s_get_cellptr
(
  uint16 cell_id
, wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION SRCHMEAS_INTER_S_ADD_CELLS

DESCRIPTION
    Used to add cells to the Inter-RAT Neighbor set and srch list using
    the information received in a CPHY_MEASUREMENT_REQ command message.

RETURN VALUE
    boolean

=======================================================================*/
boolean srchmeas_inter_s_add_cells
(
  l1_inter_sys_cell_list_struct_type *cell_list,
  l1_meas_ctrl_enum_type               parm_type,
  boolean bsic_verify_reqd,
  uint32  meas_id
, wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION SRCHMEAS_DETERMINE_STTD_BEHAVIOR

DESCRIPTION
    This function is used to determine STTD behavior     

RETURN VALUE
    void

=======================================================================*/
extern void srchmeas_determine_sttd_behavior(void);

/*===========================================================================
FUNCTION        SRCHMEAS_SUSPEND_MEASUREMENTS_FOR_HHO

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    
  void.

SIDE EFFECTS    
===========================================================================*/
extern void srchmeas_suspend_measurements_for_hho( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHMEAS_RESTORE_MEASUREMENTS_UPON_RESUMPTION_ON_ORIG_FREQ

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    
  void.

SIDE EFFECTS    
===========================================================================*/
extern void srchmeas_restore_measurements_upon_resumption_on_orig_freq( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHMEAS_RESTORE_MEASUREMENTS_UPON_SUCCESSFUL_HHO

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    
  void.

SIDE EFFECTS    
===========================================================================*/
extern void srchmeas_restore_measurements_upon_successful_hho( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHMEAS_INIT_ASET_ENG_AND_FILT_LEN

FILE NAME       

DESCRIPTION     
  Used to initialize the filt_eng and filt_len of serving cell

DEPENDENCIES    

RETURN VALUE    
  void.

SIDE EFFECTS    
  Filt energy is initialized to 0 and filter is restarted
===========================================================================*/
extern void srchmeas_init_aset_eng_and_filt_len( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHMEAS_INIT_INTRA_ENG_AND_FILT_LEN

FILE NAME       

DESCRIPTION     
  Used to initialize the filt_eng, filt_len, tot_eng and max_eng[0] of
  all intra freq neighbor cells and intra freq detected cells

DEPENDENCIES    

RETURN VALUE    
  void.

SIDE EFFECTS    
  Total and filt energy are initialized to 0 and filter is restarted
===========================================================================*/
extern void srchmeas_init_intra_eng_and_filt_len( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHMEAS_COPY_CELL_INFO_INTER_TO_INTRA

DESCRIPTION
  Copy cell info from inter freq cell structure to intra freq cell structure

DEPENDENCIES
  None.

PARAMETERS
  pointer to inter freq cell structure and intra freq cell structure
 
RETURN VALUE
  none

SIDE EFFECTS
  inter freq cell info copied to intra freq cell info
===========================================================================*/
extern void srchmeas_copy_cell_info_inter_to_intra(
              srch_interf_cell_struct_type *inter_f_cell_ptr,
              srch_cell_struct_type *intra_f_cell_ptr);

/*===========================================================================

FUNCTION     SRCHMEAS_COPY_CELL_INFO_INTRA_TO_INTER

DESCRIPTION
  Copy cell info from intra freq cell structure to inter freq cell structure

DEPENDENCIES
  None.

PARAMETERS
  pointer to intra freq cell structure and inter freq cell structure
 
RETURN VALUE
  none

SIDE EFFECTS
  inter freq cell info copied to intra freq cell info
===========================================================================*/
extern void srchmeas_copy_cell_info_intra_to_inter(
              srch_cell_struct_type *intra_f_cell_ptr,
              srch_interf_cell_struct_type *inter_f_cell_ptr, boolean reset_filt);

extern void srchmeas_update_cells_with_cells_for_meas_info(uint8 carr_idx, wsub_id_e_type wsub_id);

extern boolean srchmeas_get_cell_for_meas_info(srch_cell_struct_type *cell_ptr, uint16 meas_idx, wsub_id_e_type wsub_id);

extern uint16 srchmeas_get_filt_idx( wsub_id_e_type wsub_id);

extern uint16 srchmeas_get_cme_filt_idx( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_DEFERRED_SIB11_12_READING
/*===========================================================================

FUNCTION     SRCHMEAS_DEFERRED_SIB_MEAS_REQ_UPDATE

DESCRIPTION
    This function handle the deferred SIB meas request from RRC and update the parameters.

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  boolean - TRUE - if it successful, FALSE otherwise

SIDE EFFECTS
  
===========================================================================*/
boolean srchmeas_deferred_sib_meas_req_update(l1_meas_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_WCDMA_DC_HSUPA
/*===========================================================================

FUNCTION     SRCHMEAS_DC_UL_FREQ_DECONFIG

DESCRIPTION
  This function will do the step of procedure in searchers on deconfiguration of DC in UL.

DEPENDENCIES
  None.

PARAMETERS
  None
 
RETURN VALUE
  none

SIDE EFFECTS
  Reset the Measurement and SFN related parameters on DC carrier 
===========================================================================*/
extern void srchmeas_dc_ul_freq_deconfig( wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION srchmeas_ext_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize shared variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void srchmeas_ext_cntrl_params_init( void );

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
  FUNCTION     srchmeas_qsh_mdump_collect

  DESCRIPTION
  This function dumps all the required parameters from srchmeas internal and 
  external control parameter structures

  DEPENDENCIES
  None

  RETURN VALUE
  None

  SIDE EFFECTS
  None
  ===========================================================================*/

extern QSH_MDUMP_FN_ATTR void srchmeas_qsh_mdump_collect(wsub_id_e_type wsub_id);
#endif


#endif /* SRCHMEAS_H */
