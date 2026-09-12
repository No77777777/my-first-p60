#ifndef WL1DEC_H
#define WL1DEC_H

/*============================================================================
                           D L D E C . H
GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000 - 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.

============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/dldec.h_v   1.33   11 Jul 2002 22:14:10   gurdeeps  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1dec.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
06/30/16    svh     Changes for some decoder QSH events.
09/15/15    ac      Add new mutex for CCTrCh setup/free, double dirty bmsk
07/22/15    ac      Populate ctchIndicator in FW-SW interface during CCTrCh setup
06/22/15    ac      Increase number of BTFD info tables by number of subs
06/17/15    ac      Added MSIM version of MMCP CTCH APIs
04/22/15    ac      Datapath W+W changes
04/09/15    mk      Included l1extif_v.h.
03/19/15    ac      W + W datapath init cleanup
01/26/15    ac      Initial checkin for X+W
===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include "comdef.h"
#include "l1rrcif.h"
#include "l1macdata.h"
#include "dlcctrchlog.h"
#include "queue.h"
#include "msm.h"
#include "dldec_common.h"
#include "wdec.h"
#include "l1extif.h"
#include "l1extif_v.h"

/* force feature define based on master feature list in cust file */
#ifdef FEATURE_AS_MVS_ENHANCED_AMR_PROCESSING
#define FEATURE_DL_FAILED_TRBLK_QUERY_CB_FUNC
#endif

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Macros used for BTFD validation */
/* ------------------------------- */

/* DL_DEC_MAX_CCTRCH_WITH_BTFD indicate the total number of CCTrCh that
   can be with BTFD requirement */
#define DL_DEC_MAX_CCTRCH_WITH_BTFD  1

/* DL_DEC_MAX_CCTRCH_BTFD_INFO indicate the number of element in the array
   of the variable/structure that are required by BTFD module per sub. This
   takes in to account the 1 extra data structure required for reconfig request */
#define DL_DEC_MAX_CCTRCH_BTFD_INFO  (DL_DEC_MAX_CCTRCH_WITH_BTFD*WCDMA_NUM_SUBS + 1)

/* CcTrCh index / Id that indicates the unsued Id */
#define DL_L1_ID_INDEX_UNUSED NO_CCTRCH_INDEX

/* Maximum number of TrCh ID for BLER measurement */
#define DEC_MAX_VALID_TRCH_ID (L1_MAX_TRCH * 2)

#define DLDEC_GET_CFN_TTI_ALIGN_TYPE(cfn) \
  ((((cfn) % 2) != 0) ? L1_TTI_10MS : \
   (((cfn) % 4) != 0) ? L1_TTI_20MS : \
   (((cfn) % 8) != 0) ? L1_TTI_40MS : L1_TTI_80MS)

/* Mutex for DOB handle */
#define DEC_DOB_HANDLE_INTLOCK()  REX_ISR_LOCK(&wl1dec_ext_cntrl_params.dob_handle_crit_sect)
#define DEC_DOB_HANDLE_INTFREE()  REX_ISR_UNLOCK(&wl1dec_ext_cntrl_params.dob_handle_crit_sect)

/* Mutex for TSN Q */
#define DEC_TSN_Q_INTLOCK()  REX_ISR_LOCK(&wl1dec_ext_cntrl_params.dec_tsn_q_crit_sect)
#define DEC_TSN_Q_INTFREE()  REX_ISR_UNLOCK(&wl1dec_ext_cntrl_params.dec_tsn_q_crit_sect)

/* Mutex for CCTrCh setup/free */
#define DEC_CCTRCH_SETUP_INTLOCK()  REX_ISR_LOCK(&wl1dec_ext_cntrl_params.cctrch_setup_crit_sect)
#define DEC_CCTRCH_SETUP_INTFREE()  REX_ISR_UNLOCK(&wl1dec_ext_cntrl_params.cctrch_setup_crit_sect)

/* macro to convert the CCTrCh reference type to TrCh BF */
#define DL_CCTRCH_REF_TO_TRCH_BF(x) (cctrch_ref_to_trch_bf_table[x])

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

typedef enum
{
  /* status FIFO is enpty and can be used */
  DEC_DOB_STATUS_FIFO_EMPTY,
  /* status FIFO belongs tp CCTrCh that need BTFD decision */
  DEC_DOB_STATUS_FIFO_BTFD,
  /* status FIFO points to DOB data that need to be delivered to MAC */
  DEC_DOB_STATUS_FIFO_DELIVER
} dec_dob_status_fifo_state_enum_type;

/* DOB handle information */
typedef struct
{
  /* Link for the queue routines to manage this item on the queue */
  q_link_type link;
  /* index of this element in array */
  uint8 index;

  /* information is valid or not */
  boolean valid;

  boolean mult_tf_pass;        /* Indicator that multiple transport formats*/
                               /* had passing CRCs (BTFD)                  */

  /* State of DOB handle */
  dec_dob_status_fifo_state_enum_type state;
  /* DOB handle from MCAL */
  mcalwcdma_dec_dob_status_handle_type dob_handle;
} dec_dob_status_handle_type;

/* Decoder call back function type.
   This function type is called after any TBset status read */
typedef void DEC_CCTRCH_T_CB_FUNC_TYPE(uint8, wsub_id_e_type);

/* This enum defines the BCH usage statets that can be set by demod */
typedef enum {
     DL_BCH_USE_OFF,             /* Turn the BCH TB processing off */
     DL_BCH_USE_STATUS_TO_DEMOD, /* Start giving TB set status to demod */
     DL_BCH_USE_DELIVER_TO_MAC   /* start deliverying BCH TB to MAC */
} dl_bch_usage_enum_type;

/* This enum defines the reference type of the CCTrCh */
typedef enum
{
     DL_CCTRCH_SBCH_TYPE,         /* Serving cell BCH CCTrCh set by RRC */
     DL_CCTRCH_NBCH_TYPE,         /* neighbor BCH CCTrCh set by RRC */
     DL_CCTRCH_DCH_TYPE,          /* DCH CCTrCh set by RRC */
     DL_CCTRCH_DSCH_TYPE,         /* DSCH CCTrCh set by RRC */
     DL_CCTRCH_PCH_TYPE,          /* PCH CCTrCh set by RRC */
     DL_CCTRCH_FACH_TYPE,         /* FACH CCTrCh set by RRC */
     DL_CCTRCH_PCH_FACH_TYPE,     /* PCH_FACH CCTrCh set by RRC */
     DL_CCTRCH_HS_DSCH_TYPE,      /* HS DSCH CCTrCh set by RRC and used for cctrch profiling */
     DL_CCTRCH_INVALID,           /* Invalid CCTrCh */
     DL_NUM_CCTRCH_REF_TYPE       /* number of CCTrCh reference types */
} dl_cctrch_ref_type_enum_type;

/* define the CCTrCh setup command type */
typedef struct
{
     l1_dl_cctrch_info_struct_type   *dl_cctrch_info; /* CCTrCh info */
     l1_dl_ctfc_info_struct_type     *dl_ctfc_info;   /* CTFC info */
     l1_setup_ops_enum_type          dl_op_type;      /* Phychan operation ADD or CFG*/
     l1_dl_phychan_enum_type         dl_phychan_type; /* ADD/CFG PhyChan type */
     l1_dl_phychan_db_struct_type    *dl_phychan_db;  /* PhyChan info */
     uint8                           drop_cctrch_idx; /* CCTrCh index being dropped */
     l1_internal_cctrch_setup_struct_type internal_info;   /* L1 internal setup flags */
} dl_cctrch_setup_cmd_type;


/* CCTRCH history profiling. profiles at given instance cctrch is allocated or deallocated */
typedef enum
{
  /* CCTRCH History. CCTRCH Allocated */
  DEC_CCTRCH_ALLOCATED,
  /* CCTRCH History. CCTRCH Deallocated */
  DEC_CCTRCH_DEALLOCATED
} dec_cctrch_hist_alloc_dealloc_enum_type;

/* defines the structure for saving the error information for the
   TB header information required as the part of detailed info
   for debug purpose only */
typedef struct
{
  /* Number of transport blocks in this transport block set. */
  uint8 num_total_tb;
  
  /* CCTrCh Ids for the transport block set */
  uint8 cctrch_id;
  
  /* TrCh Ids for the transport block set */
  uint8 trch_id;
  
  uint8 tfi; /* TF index for this TBset or TrCh */

  uint32 crc_result_bits; /* CRC pass fail result bits for each TB */
  boolean tfci_decode_error; /* TFCI decode error for this TBset */
  
  /* TrCh reference type for the transport block set */
  l1_dl_trch_ref_type_enum_type trch_ref_type;
  
  /* CFN for this transport block set */
  uint8 cfn;
} dl_dec_tbset_hdr_fail_drop_info;

#ifdef FEATURE_DL_FAILED_TRBLK_QUERY_CB_FUNC
/* call back function type for failed block passing to higher layer info */
typedef uint16 (*DL_DEC_FAILED_BLK_QUERY_CB_FUNC_TYPE)(void);
#endif /* FEATURE_DL_FAILED_TRBLK_QUERY_CB_FUNC */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
typedef struct
{
  /* This variable contains the TrCh Ids corresponding to the TrCh table index.
   This is used for getting the TrCh table Ids from the table index passed
   in the link FIFO as the mDSP version of TrCh Id. */
  uint8 dl_trch_table_index_id[DEC_TOTAL_CCTRCH_TABLES][DEC_TOTAL_TRCHT_PER_CCTRCH];
  /* This variable contains the CCTrCh Ids corresponding to the CCTrCh table
   index. This is used for getting the CCTrCh table Ids from the table index
   passed in the link FIFO as the mDSP version of CCTrCh Id. */
  uint8 dl_cctrch_table_index_id[DEC_TOTAL_CCTRCH_TABLES];
  /* This variable keep the CCTrCh enabled state. At the setup time the CCTrCh
   is initialized to enabled = FALSE. Demod calls the function in decoder
   module to enable the CCTrCh after it has setup the corresponding PhCh.
   In the disabled state no action is taken for any TB received. */
  boolean dl_cctrch_enabled[DEC_TOTAL_CCTRCH_TABLES];
}wl1dec_ext_cntrl_params_mdmp_type;

typedef struct
{
  /* This variable stores CCTrCh table reference type */
  dl_cctrch_ref_type_enum_type dl_cctrch_ref_type[DEC_TOTAL_CCTRCH_TABLES];

  /* This array stores the number of TrCh of a CCTrCh */
  uint8 dl_num_trch[DEC_TOTAL_CCTRCH_TABLES];

  /* This array stores the TF information for all CCTrCh . This is required by
     the transport block handling module to get the information about the
     transport block set size and number of transport blocks. */
  l1_dl_tf_info_struct_type *dl_tf_info[DEC_TOTAL_CCTRCH_TABLES][DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* This array holds the TrCh reference type information of the TrChs
     (Tb set for DOB data) */
  l1_dl_trch_ref_type_enum_type dl_trch_ref_type[DEC_TOTAL_CCTRCH_TABLES][DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* This array saves the TTI of each TrCh in all CCTrCh */
  l1_tti_enum_type dl_trch_tti[DEC_TOTAL_CCTRCH_TABLES][DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* Critical section mutex for TSN info Q */
  rex_crit_sect_type dec_tsn_q_crit_sect;

  /* Critical section mutex for DOB handle */
  rex_crit_sect_type dob_handle_crit_sect;

  /* Critical section mutex for init */
  rex_crit_sect_type dec_init_crit_sect;

  /* Critical section mutex for CCTrCh setup */
  rex_crit_sect_type cctrch_setup_crit_sect;

  /* TTI boundry check pending */
  boolean dl_trblk_tti_bdry_check_pending[DEC_TOTAL_CCTRCH_TABLES][DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* This array stores the status fifo information. This information is refered
   to by all other function to get the starting address of the DOB. This
   information is also used by header and data reading function to get the
   transport format Id to get the transport block size and number of transport
   blocks */
  dec_dob_status_handle_type dec_dob_status_handle[MCALWCDMA_DEC_MAX_DOB_HANDLES];

  /* This variable stores CCTrCh table availability status. mDSP has 6 CCTrCh
   tables out of which any 3 can be used as active ones. Other 3 are used
   to reconfigure the 3 currently active CCTrCh without the loss of data */
  boolean dl_cctrch_avail_status[DEC_TOTAL_CCTRCH_TABLES];

  /* This array holds the crc length information of the TrChs */
  uint8 dl_trch_crc_length[DEC_TOTAL_CCTRCH_TABLES][DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* CFN at which CCTrCh is enabled */
  uint8 dl_cctrch_enable_cfn[DEC_TOTAL_CCTRCH_TABLES];

  /* this array hold the boolean values that indicate that
     1. If the CCTrCh is TFCI case: Then this TrCh has non zero CRC type.
     2. No-TFCI case: This TrCh has all the TF having non zero CRC. That means
                      no TF has 0 number of TB.
     This decision is used at number of places so it is good to calculate it
     once and use it at different places */
  boolean dl_non_zero_crc_criteria[DEC_TOTAL_CCTRCH_TABLES][DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* Q for DOB status FIFO */
  q_type dec_dob_status_fifo_free_q;

  /* This array stores the number of TF in the TrChs of a CCTrCh */
  uint8 dl_num_tf[DEC_TOTAL_CCTRCH_TABLES][DEC_TOTAL_TRCHT_PER_CCTRCH];

  wl1dec_ext_cntrl_params_mdmp_type mdmp;
}wl1dec_ext_cntrl_params_type;

typedef struct
{
  /* CCTrCh dirty bitmask.
     This is used to send indication to FW that CCTrCh info changed - one bit per CCTrCh */
  uint32 dec_cctrch_dirty_bmsk;
}wl1dec_ext_cntrl_per_sub_params_mdmp_type;

typedef struct
{
  /* Variable for automatically measure BLER.
   Value 0 means that timer is infinite */
  uint16 dl_bler_meas_duration_ms;
  boolean dl_override_bler_meas_duration;

  /* Number of times SFN mismatch is detected */
  uint32 dl_num_times_sfn_mismatch;

  /* DL cipher information */
  l1_cipher_key_info_struct_type dec_cipher_info;

  /* auto measurement REX timer */
  rex_timer_type dl_bler_meas_delay_timer;

  /* This array keeps the CCTrCh table index to which the TrCh Id passed
   by RRC, belongs */
  uint8 dl_trch_id_cctrcht_index[L1_MAX_TRCH_ID + 1];

  /* set this variable to TRUE if the duration is required to be updated.
   The time will be updated in next 10 ms */
  boolean dl_bler_meas_timer_update;

  /* This array keeps the TrCh table index to which the TrCh Id passed
   by RRC, belongs */
  uint8 dl_trch_id_table_index[L1_MAX_TRCH_ID + 1];

  /* Q for DOB status FIFO */
  q_type dec_dob_status_fifo_q;

  /* This flag indicates if there is any pending R99_DATA relinquish
   local cmd yet to be handled by wl1m */
  boolean dec_r99_data_relinquish_cmd_pending;

  /* This array has TF index of TrChs in CCTrCh that has max blocks and is the
   part of TFC table. If there is no TF index existing in that TrCh then value
   is set to -1 */
  int8 dl_dch_trch_max_blk_tf_idx[DEC_TOTAL_TRCHT_PER_CCTRCH];

  #ifdef FEATURE_DL_PATH_PROFILING
  dec_hdr_rd_isr_struct_type dec_hdr_rd_isr[64];
  dec_data_rd_struct_type dec_data_rd[64];

  uint8 dec_data_rd_idx;
  uint8 dec_hdr_rd_idx;
  #endif

  wl1dec_ext_cntrl_per_sub_params_mdmp_type mdmp;

 #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  /* Used to keep track of on which CFN page is expected. */
  uint16 pich_sfn;
 #endif
} wl1dec_ext_cntrl_per_sub_params_type;

extern wl1dec_ext_cntrl_params_type wl1dec_ext_cntrl_params;
extern wl1dec_ext_cntrl_per_sub_params_type wl1dec_ext_cntrl_per_sub_params[WCDMA_NUM_SUBS];

extern const uint8 cctrch_ref_to_trch_bf_table[DL_NUM_CCTRCH_REF_TYPE];

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================
FUNCTION dec_free_status_handle

DESCRIPTION
This function frees the DOB status FIFO in MDSP.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dec_free_status_handle(
  /* status FIFO index in mDSP to be freed */
  uint16 fifo_index, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_dec_init_stack

DESCRIPTION
  This function initialize the decoder sub modules. This is called by L1
  manager at stack startup. It performs the following initializations.
  - Sets the total number of CcTrCh used to 0 and initialize the cctrch table
    index for all id to 0xFF, all CCTrCh table Ids for all table index to
    0xFF. Sets the CCTrCh table availability status to TRUE.. Sets the TFCI
    table index availabilty to TRUE and set the TFCI index for CCTrCh table
    index to 0xFF.
  - Calls all the initialization function in decoder sub modules.
  - Set the read done status for header information to TRUE
  - Set the read done status for data information to TRUE
  - Initialize the buffer pointer to the transport block data to NULL.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void dl_dec_init_stack( wsub_id_e_type wsub_id);
extern void dl_dec_deinit_stack( wsub_id_e_type wsub_id);

extern void dl_dec_init_connected(wsub_id_e_type wsub_id);
extern void dl_dec_deinit_connected(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_set_ciphering_keys

DESCRIPTION
  This function validates and sets the ciphering key information for downlink
  deciphering operation.

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE validate and update result.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean dl_set_ciphering_keys(
     /* DL ciphering info structure */
     l1_cipher_key_info_struct_type *dl_cipher_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dec_cctrch_alloc_hist_profile

DESCRIPTION
  This function profiles cctrch allocation and deallocation history to debug related issues.

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/

extern void dec_cctrch_alloc_hist_profile(uint8 cctrch_id,
                                          dl_cctrch_ref_type_enum_type cctrch_ref_type,
                                          dec_cctrch_hist_alloc_dealloc_enum_type cctrch_alloc_dealloc, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION dl_cctrch_setup

DESCRIPTION
  This function configure or modify the CCTrCh information. This is called
  by L1 manager. It goes through the currently existing CCTrCh Id and if
  CCTrCh Id in this info structure is active it assumes this function call
  as modify request otherwise as new configure request. This function calls
  the functions to calculate the rate matching parameters, rate matched size,
  code block parameters (number and size) and setup mDSP tables.

DEPENDENCIES
  None

RETURN VALUE
  Returns the CCTrCh table index setup. It retuens 0xFF in case of any
  failure. The reason for the failure can be as follows:
   - No free mDSP CCTrCh table exists to update CCTrCh info to mDSP.

SIDE EFFECTS
  None
===========================================================================*/

extern uint8 dl_cctrch_setup(
     /* DL CCTrCh setup command packet */
     dl_cctrch_setup_cmd_type *cctrch_setup_cmd_pkt
  , wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION dl_set_cctrch_enable_state

DESCRIPTION
  This function sets the CCTrCh enable state. Before setting the state of the
  CCTrCh table index it checks if that CCTrCh table index is in use and is
  a valid CCTrCh index.

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE based on success of state change

SIDE EFFECTS
  Updates the state in array element of array dl_cctrch_enabled.
===========================================================================*/

extern boolean dl_set_cctrch_enable_state(
     /* CCTrCh table index for which the enable state is to be set */
     uint8 cctrch_index,
     /* CCTrCh state to be set */
     boolean state,
     /* CFN at which corresponding H/W ch is enabled/diabled */
     uint8 en_dis_cfn, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_set_trch_enable_state

DESCRIPTION
  This function sets the TrCh enable state. It need the CCTrCh table index
  and TrCh reference type to enable or disable the state. Before setting the
  state of the TrCh table index it checks if that CCTrCh table index is in
  use and is a valid CCTrCh index.

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE based on success of state change

SIDE EFFECTS
  Updates the state in array element of array dl_cctrch_enabled.
===========================================================================*/

extern boolean dl_set_trch_enable_state(
  /* CCTrCh table index for which the enable state is to be set */
  uint8 cctrch_index,
  /* TrCh reference type */
  l1_dl_trch_ref_type_enum_type trch_ref_type,
  /* CCTrCh state to be set */
  boolean state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_set_trch_idx_enable_state

DESCRIPTION
  This function sets the TrCh index enable state. It need the CCTrCh table index
  and TrCh index to enable or disable the state. Before setting the
  state of the TrCh table index it checks if that CCTrCh table index is in
  use and is a valid CCTrCh index.

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE based on success of state change

SIDE EFFECTS
  Updates the state in array element of array dl_cctrch_enabled.
===========================================================================*/

extern boolean dl_set_trch_idx_enable_state(
  /* CCTrCh table index for which the enable state is to be set */
  uint8 cctrch_index,
  /* TrCh index */
  uint8 trch_index,
  /* TrCh state to be set */
  boolean state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_send_cctrch_dirty_bit_update_cmd

DESCRIPTION
  This function sends a sync command to FW if the CCTrCh dirty bitmask is 
  non-zero. This means that some Trch enable state has changed and FW needs
  to be notofoed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_send_cctrch_dirty_bit_update_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_set_trch_state_interf_resel

DESCRIPTION
  This function sets the TrCh enable state based on Resel start or stop

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_set_trch_state_interf_resel(
  /* TRUE: Resel start - TrCh to be disabled 
     FALSE: Resel stop - Trch to be re-enabled */
  boolean state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_validate_cctrch_setup_req

DESCRIPTION
  This function validates the parameters for the CCTrCh set command pacaket.
  In navigates through all the paramaters and check wheather the values are
  valid and with in range.

DEPENDENCIES
  None

RETURN VALUE
  TRUE of FALSE as per the validation result.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean dl_validate_cctrch_setup_req(
     /* DL CCTrCh setup command packet */
     dl_cctrch_setup_cmd_type *cctrch_setup_cmd_pkt,
     /* shared PhCh CCTrCh table index */
     uint8 shared_phch_cctrch_table_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_cctrch_setup_bch

DESCRIPTION
  This function setup the a CCTrCh table for initial BCH setup for getting
  the timing and SFN count of the BCH or for the neighbor BCH to get the SFN
  value. This is called demod module.

DEPENDENCIES
  None

RETURN VALUE
  Table index of neighbour BCH, 0xFF if not successful.

SIDE EFFECTS
  Set the boolean dl_send_bch_init_trblk_flag to false.
===========================================================================*/

extern uint8 dl_cctrch_setup_bch( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_get_cctrch_ref_type

DESCRIPTION
  This function returns the CCTrCh reference type for the CCTrCh table index.

DEPENDENCIES
  None

RETURN VALUE
  CCTrCh reference type of the CCTrCh.

SIDE EFFECTS
  None
===========================================================================*/

extern dl_cctrch_ref_type_enum_type dl_get_cctrch_ref_type(
     /* CCTrCh mDSP table index for which the ref type is to be returned */
     uint8 cctrch_table_index, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_gen_cctrch_ref_type

DESCRIPTION
  This function generate the CCTrCh type depending on the TrCh ref types of
  the transport channel contained in it.

DEPENDENCIES
  None

RETURN VALUE
  CCTrCh reference type of the CCTrCh.

SIDE EFFECTS
  None
===========================================================================*/

extern dl_cctrch_ref_type_enum_type dl_gen_cctrch_ref_type(
     /* pointer to CCTrCh info strcuture for which the ref. type is to be
        generated */
     l1_dl_cctrch_info_struct_type *cctrch_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_get_longest_tti

DESCRIPTION
  This function returns the longest TTI among the TrChs in a CCTrCh. The
  CCTrCh table index is passed as function parameter.

DEPENDENCIES
  None

RETURN VALUE
  l1_tti_enum_type -> L1_TTI_10MS, L1_TTI_20MS, L1_TTI_40MS, or L1_TTI_80MS.

SIDE EFFECTS
  None
===========================================================================*/

extern l1_tti_enum_type dl_get_longest_tti(
  /* CcTrCh table index in mDSP */
  uint8 cctrch_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_get_trch_longest_tti

DESCRIPTION
  This function returns the longest TTI among the specific TrChs reference
  type in a CCTrCh. The CCTrCh table index and TrCh reference type is passed
  as function argument. Use ORed constant L1_DL_TRCH_REFTYPE_XXXX_BF defined
  in l1sapcommon.h to pass desired TrCh ref type bit field

DEPENDENCIES
  None

RETURN VALUE
  l1_tti_enum_type -> L1_TTI_10MS, L1_TTI_20MS, L1_TTI_40MS, or L1_TTI_80MS.
  If all of the TrCh BF is not in this CCTrCh then it returns L1_NUM_TTI as
  invalid TTI type.

SIDE EFFECTS
  None
===========================================================================*/

extern l1_tti_enum_type dl_get_trch_longest_tti(
  /* CcTrCh table index in mDSP */
  uint8 cctrch_idx,
  /* TrCh reference type bit field */
  uint8 trch_ref_type_bf, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_get_shortest_tti

DESCRIPTION
  This function returns the shortest TTI among the TrChs in a CCTrCh. The
  CCTrCh table index is passed as function parameter.

DEPENDENCIES
  None

RETURN VALUE
  l1_tti_enum_type -> L1_TTI_10MS, L1_TTI_20MS, L1_TTI_40MS, or L1_TTI_80MS.

SIDE EFFECTS
  None
===========================================================================*/

extern l1_tti_enum_type dl_get_shortest_tti(
  /* CcTrCh table index in mDSP */
  uint8 cctrch_idx, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION dl_query_dch_cctrch_loopback_status

DESCRIPTION
  This function queries whether any TrCh of the DCH CCTrCh is configured in
  loopback mode. If so it returns TRUE else FALSE. It queries the DL API to
  get the CCTrCh index associated with the DPCH combiner and then cross 
  verifies it with internal decoder database. Looping through all the TrCh 
  of the CCTrCh, it finds out if any TrCh is configured for any kind of 
  loopback mode.

DEPENDENCIES
  This function is supposed to be called only when L1 is in DCH state. If
  not, then it will print out error message that DCH CCTrCh does not exist.

RETURN VALUE
  TRUE: DCH CCTrCh has some TrCh configured for loopback mode
  FALSE: None of the DCH TrCh is in loopback mode.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean dl_query_dch_cctrch_loopback_status( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_cctrch_free

DESCRIPTION
  This function set the status of the CCTrCh table index to available. It
  also sets the status TFCI table index (if any) associated with this CCTrCh
  index to available. When the CCTrCh is removed or modified the currently
  associated CCTrCh table index is freed after the successful completion of
  the action taking place in the demod module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the CCTrCh has the transport channal of the type BCH_INIT or NBCH it
  sets the corresponding flags to FALSE.
===========================================================================*/

extern void dl_cctrch_free(
     /* CCTrCh index in mDSP tables passed allocated to CCTrCh */
     uint8 cctrch_index, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_get_cctrch_index

DESCRIPTION
  This function gets the CCTrCh table entry index associated with CCTrCh Id.
  This function is called by demod block to get the CCTrCh Id for setting up
  or reconfiguring the channels.

DEPENDENCIES
  None

RETURN VALUE
  CCTrCh table index. The possible values are from 0 to 5. Returns 0xFF if
  no such CCTrCh Id exists.

SIDE EFFECTS
  None
===========================================================================*/

extern uint8 dl_get_cctrch_index(
  /* CCTrCh id passed by RRC for this CCTrCh */
  uint8 cctrch_id, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_dec_maint_event_handler

DESCRIPTION
  This function do periodic things that are required to be done once
  every 10 ms.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_dec_maint_event_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_trblk_header_read_event_handler

DESCRIPTION
  This function is a event handler function for the transport channel block
  header read event. This function is set in the initialization to be called
  periodically every radio frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_trblk_header_read_event_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_trblk_data_read_event_handler

DESCRIPTION
  This function is a event handler function for the transport channel block
  data read event. This function is set in the initialization to be called
  periodically every radio frame after the header read event.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_trblk_data_read_event_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  dl_bch_ctrl

DESCRIPTION
  This function allows for control of the use of the BCH. The BCH can be
  turned off, which disables the decoder events on the currently running
  timeline. It can be set to be enabled but have dec report crc status to
  demod and have the transport blocks discarded, or it can be set to have
  transport blocks delivered to MAC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The decoder module behavior with respect to handling BCH transport blocks
  may be altered.
===========================================================================*/

extern void dl_bch_ctrl(
     /* the BCH CCTrCh table index of the BCH CCTrCh for which the BCH sub
        state is to be setup */
     uint8 cctrch_table_index,
     /* BCH usage type to be swtiched to */
     dl_bch_usage_enum_type bch_usage, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  dl_flush_all_dob_status_fifo

DESCRIPTION
  This function clear all DOB status FIFO element in decoder S/W. This
  function is intended for cleanup purpose like in prep for sleep

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The decoder module behavior with respect to handling BCH transport blocks
  may be altered.
===========================================================================*/
extern void dl_flush_all_dob_status_fifo( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION  dl_flush_hs_info_from_dob

DESCRIPTION
  This function scans thro' HDOB status FIFO Q and checks if it HS type and
  if HS type then if HS info table index is the one to be flushed. If all
  conditions are met then that DOB status FIO is removed from Q and is freed

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_flush_hs_info_from_dob(
  /* HS table index that need to be flushed from DOB status FIFO Q */
  uint8 hs_table_index, wsub_id_e_type wsub_id);


#ifdef FEATURE_DL_FAILED_TRBLK_QUERY_CB_FUNC
/*===========================================================================
FUNCTION  dl_dec_register_failed_trblk_query_cb_func

DESCRIPTION
  This function registers call back funtion to query transport block
  size of TrCh for which TFCI or BTFD decision have failed

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_dec_register_failed_trblk_query_cb_func(
  /* CCTrCh id */
  uint8 cctrch_id,
  /* TrCh Id */
  uint8 trch_id,
  /* function pointer to be registered */
  DL_DEC_FAILED_BLK_QUERY_CB_FUNC_TYPE cb_func, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  dl_dec_get_failed_trch_tf_to_pass_thru

DESCRIPTION
  This function gets the failed TrCh TF index whose block should be passed up.

DEPENDENCIES
  None

RETURN VALUE
  Valid TF index. In case unable to find valid TF index, return invalid index
  as indication of failure.

SIDE EFFECTS
  None
===========================================================================*/

extern uint8 dl_dec_get_failed_trch_tf_to_pass_thru(
  /* CCTrCh table index for which to pass the failed TF block to higher layers */
  uint8 cctrch_index,
  /* TrCh table index for which to pass the TF block to higher layers */
  uint8 trch_index, wsub_id_e_type wsub_id);

#endif /* FEATURE_DL_FAILED_TRBLK_QUERY_CB_FUNC */

/*===========================================================================
FUNCTION  dl_auto_bler_meas

DESCRIPTION
  This function is called by L1 auto bler measure timer signal from l1 task.
  THis is used to calculate the bler for various TrCh.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_auto_bler_meas(uint8 cctrch_index, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_SUBMIT_PENDING_LOGPKTS

FILE NAME       dlcmd.c

DESCRIPTION     This function calls functions those submit various 
                         periodic log pkts.

DEPENDENCIES

RETURN VALUE 

SIDE EFFECTS
===========================================================================*/
extern void dl_dec_submit_pending_logpkts(
                     dl_cctrch_ref_type_enum_type cctrch_ref, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION DL_DECODE_SFN_PRIME

DESCRIPTION
   This function is meant to be called directly by L1 in order to decode the
   SFN-Prime from a BCH transport block. Since the SFN-Prime is the second
   IE in the transport block, it can easily be extracted and shifted around
   to form the value.

   Assumptions:
     * The processor is big-endian
     * The SFN-Prime is eleven bits long
     * The BCH transport block holds a valid System Information Message
     * Unaligned PER was used to encode the System Information Message
     * The SFN-Prime is the second IE in the System Information message
     * Based on 25.331 V3.2.0 (2000-03)

DEPENDENCIES
   None.

RETURN VALUE
   The unencoded SFN-Prime from the transport block.

SIDE EFFECTS
   None.

===========================================================================*/

unsigned short dl_decode_sfn_prime
(
   void *transport_block               /* Pointer to the BCH transport
                                          block to be decoded */
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dldec_sbch_sfn_seq_cfn_sanity_check_cmd_handler

DESCRIPTION
  This function checks the sanity of sequencer SFN/CFN. It is intended to be
  used as call back function from decoder for CcTrCh reference type SBCH
  in non DCH state. This call back function assumes that it is called near
  the middle of radio frame (avoid frame boundry by safe distance) and in
  interrupt context (to avoid any significant premetion).

  It gets sequencer SFN and compares it with SFN decoded from PCCPCH if CRC
  has passed on that TTI.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Correct sequencer SFN/CFN if found misaligned w.r.t. serving cell PCCPCH
===========================================================================*/
extern void dldec_sbch_sfn_seq_cfn_sanity_check_cmd_handler(
  /* decoder SW DOB status FIFO index */
  uint16 fifo_idx, wsub_id_e_type wsub_id);
  
/*===========================================================================
FUNCTION dl_dec_register_ctch_pdu_decode_error_cb

DESCRIPTION
  This function registers the callback for the PDU decode error indication.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_dec_register_ctch_pdu_decode_error_cb(
  /* pointer to the callback */
  DL_DEC_CTCH_PDU_DECODE_ERROR_CB_FUNC_TYPE ctch_pdu_decode_error_cb);

/*===========================================================================
FUNCTION dl_dec_register_ctch_pdu_decode_error_cb_msim

DESCRIPTION
  This function registers the callback for the PDU decode error indication.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_dec_register_ctch_pdu_decode_error_cb_msim(
  /* pointer to the callback */
  DL_DEC_CTCH_PDU_DECODE_ERROR_CB_FUNC_TYPE ctch_pdu_decode_error_cb,
  wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION dl_dec_deregister_ctch_pdu_decode_error_cb

DESCRIPTION
  This function deregisters the callback for the PDU decode error indication.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dl_dec_deregister_ctch_pdu_decode_error_cb( void);

/*===========================================================================
FUNCTION dl_dec_deregister_ctch_pdu_decode_error_cb_msim

DESCRIPTION
  This function deregisters the callback for the PDU decode error indication.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void dl_dec_deregister_ctch_pdu_decode_error_cb_msim( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DL_DEC_GET_SUB_ID_FROM_CCTRCH_IDX

DESCRIPTION
  Gets sub ID from CCTrCh index

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Subscription ID of CCTrCh. WL1_INVALID_SUB_ID on errors.

SIDE EFFECTS
  None.
===========================================================================*/
extern wsub_id_e_type dl_get_sub_id_from_cctrch_idx(uint8 cctrch_idx);

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*===========================================================================
FUNCTION     dl_dec_set_pich_sfn

DESCRIPTION
  Called once PICH decode is done and if UE is paged.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void dl_dec_set_pich_sfn(uint16 sfn, wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION  wl1_qsh_mdump_collect_wl1dec

DESCRIPTION
  Collect WL1DEC variables.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_wl1dec(wsub_id_e_type wsub_id);
#endif
#endif /* DLDEC_H */
