#ifndef TDSMACULTFCS_H
#define TDSMACULTFCS_H
/*===============================================================================================

                      U P L I N K   M A C  T F C S   H E A D E R   F I L E

GENERAL DESCRIPTION

  This module contains the declarations for UL TFCS functions

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


================================================================================================*/

/*===============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macultfcs.h_v   1.5   18 Jun 2001 10:00:34   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacultfcs.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

================================================================================================*/
#include "tdscdma_variation.h"

/* Pointer to ordered TFCS data */
extern  tdsmac_ul_tfcs_type    *tdsoc_ul_tfcs_ptr;

typedef struct
{
  uint8               amr_a_dch_idx;
  uint8               amr_b_dch_idx;
  uint8               amr_c_dch_idx;
  uint8               num_amr_trch;
  uint16              amr_a_tfi;
  uint16              amr_b_tfi;
  uint16              amr_c_tfi;
  uint32 amr_mode;
}tdsamr_trch_mapping_info_type;

/*================================================================================================

 FUNCTION NAME  update_ul_tfcs_info_table

 PURPOSE        Updates the uplink TFCS information table

 DESCRIPTION
================================================================================================*/
void    tdsproc_ul_tfcs_info
(
    /* UL TFCS Configuration Pointer */
    tdsmac_ul_tfcs_type        *tfcs_ptr
);

/*================================================================================================
 FUNCTION NAME  tdsselect_tfc

 DESCRIPTION
    Selects the appropriate Transport Format Combination based on :
    - TTI (10 or 20 or 40 or 80 msec)
    - Buffer occupancy and priority of uplink logical channels
    - Available Transport Format Combination Set

 RETURNS
    UL RLC Frame Format (i.e. # of PDUS/PDU Size of each logical channel)

 CONTEXT
    Layer 2 Task context

================================================================================================*/
void        tdsselect_tfc
(
    /* TTI boundary of the frame */
    tdsl1_tti_enum_type            tti,

    /* Uplink Buffer Status Pointer */
    tdsrlc_ul_buf_status_type      *tdsul_buf_status_ptr,

    /* Pointer to UL RLC Frame Format information */
    tdsrlc_ul_frame_format_type   *rlc_frame_fmt_ptr

);

/*================================================================================================
 FUNCTION NAME  tdsmac_compute_minimum_set_of_tfcs()

 PURPOSE        This function computes the minimum set.

 DESCRIPTION    The minum set would be computed every time MAC is configured with a new
                TFCS table.
================================================================================================*/
void tdsmac_compute_minimum_set_of_tfcs
(
  tdsmac_amr_mode_info_type  *mac_amr_mode_info_ptr
);

/*================================================================================================

 FUNCTION NAME  tdsmac_initialize_pwr_state_for_tfcs

 PURPOSE        Initializes the power state information after reconfig procedure.

 DESCRIPTION

================================================================================================*/
void    tdsmac_initialize_pwr_state_for_tfcs
(
  const tdsl1_ul_tfc_pwr_state_enum_type *tdstfc_excess_pwr_state_ptr
);

/*================================================================================================

 FUNCTION NAME  tdsmac_init_ratem_state_for_tfcs

 PURPOSE        Initializes the rate match state information after reconfig procedure.

 DESCRIPTION

================================================================================================*/
void    tdsmac_init_ratem_state_for_tfcs
(
  const tdsl1_ul_rm_state_enum_type *tdstfc_ratem_state_ptr
);

/*================================================================================================

 FUNCTION NAME  tdsmac_ul_assign_pdu_size

 PURPOSE        Assigns the pdu_size using TB_size for enhanced buffer status 
                computations during re-configuration of MAC parameters.

================================================================================================*/
void tdsmac_ul_assign_pdu_size( void );


/*================================================================================================
 FUNCTION NAME  tdsmac_get_amr_trch_mapping_info()

 PURPOSE        This function computes the minimum set.

 DESCRIPTION    The minum set would be computed every time MAC is configured with a new
                TFCS table.
================================================================================================*/
void tdsmac_get_amr_trch_mapping_info
(
  tdsamr_trch_mapping_info_type *amr_trch_mapping
);

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
/*================================================================================================
 FUNCTION NAME  tdsmac_calculate_constant_delay

 PURPOSE        This function calculates the constant delay based on the highest tti type
                configured for the transport channels

 DESCRIPTION    For constant delay in the case of multiple DTCH with different TTI, it has to
                be atleast maxTTI. Adding an additional delay of 5 ( for the corresponding
                data to be transmitted on the uplink). Now this delay should be a multiple
                of the maxTTI since data can be transmitted only at the end of the TTI.
================================================================================================*/
uint8 tdsmac_calculate_constant_delay(tdsl1_tti_enum_type tti_type);
#endif

/*================================================================================================
 FUNCTION NAME  tdseliminate_tfc_based_on_tti()

 PURPOSE        Eliminate TFCs based on TTI constraint

 DESCRIPTION    Once TF has been selected for a given transport channel, it cannot change
                until next TTI boundary for that transport channel. Hence, in the meantime, it is
                only possible to select TFCs that have the same TF for the transport channel.
                Since TTIs are aligned for all transport channels, a TTI boundary for one TTI
                length is also a boundary for all channels that have equal or shorter TTI length.
================================================================================================*/
void    tdseliminate_tfc_based_on_tti
(
/* Current TTI boundary */
tdsl1_tti_enum_type    cur_tti,
tdsrlc_ul_buf_status_type  *ul_buf_status_ptr
);

/*====================================================================================================
 FUNCTION NAME      tdseliminate_tfc_based_on_ue_tx_pwr()

 DESCRIPTION        This function eliminates TFCs based on the UE Tx power
====================================================================================================*/

void tdseliminate_tfc_based_on_ue_tx_pwr(void);


/*===================================================================================================
 FUNCTION NAME  tdseliminate_tfc_based_on_bo ()

 DESCRIPTION    This function eliminates Transport Format Combination based on Logical
                channel buffer constraints
====================================================================================================*/
void  tdseliminate_tfc_based_on_bo
(
/* Uplink Buffer Status Pointer */
tdsrlc_ul_buf_status_type  *tdsul_buf_status_ptr
);

/*================================================================================================
 FUNCTION NAME      tdscompute_tfc_bo ()

 DESCRIPTION        This function computes buffer Occupancy of each Transport Format
                    Combination
================================================================================================*/
void    tdscompute_tfc_bo
(
void
);

/*================================================================================================
 FUNCTION NAME      tdscompute_rlc_frame_format_based_on_priorities

 DESCRIPTION
================================================================================================*/
boolean tdscompute_rlc_frame_format_based_on_priorities
(
/* Pointer to UL RLC Frame Format information */
tdsrlc_ul_frame_format_type   *rlc_frame_fmt_ptr
);

/*================================================================================================
 FUNCTION NAME      tdspick_tfc_with_highest_bo

 DESCRIPTION        This function picks the TFC with highest buffer occupancy
================================================================================================*/
boolean     tdspick_tfc_with_highest_bo (void);

/*================================================================================================
 FUNCTION NAME  tdseliminate_tfc_based_on_active_trchs

 PURPOSE        This function populates a table which contains a list of transport channels
                and a flag indicating if they are currently active or not.
================================================================================================*/
uint8 tdseliminate_tfc_based_on_active_trchs (void);

/*====================================================================================================
 FUNCTION NAME      tdseliminate_tfc_based_on_tfi()

 DESCRIPTION        This function eliminates TFCs based on TFI constraint
====================================================================================================*/
void    tdseliminate_tfc_based_on_tfi
(
/* Transport channel index */
tr_ch_id_type   tr_index,

/* Non-allowed TFI */
uint16          non_allowed_tfi
);

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) \
    || defined(FEATURE_TDSCDMA_DMO_SUPPORT)            \
    || defined(FEATURE_TDSCDMA_DSDS_QTA)
/*====================================================================================================
 FUNCTION NAME      tdseliminate_tfc_based_on_dmo()

 DESCRIPTION        This function eliminates TFCs based on dmo constraint
====================================================================================================*/
void    tdseliminate_tfc_based_on_dmo
(
void
);
#endif

#endif

