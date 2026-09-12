/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   Diagnostics Packet Processing

General Description
  Diagnostic packet processing routines for MUX1X sub-system access.

  Copyright (c) 2003 - 2013 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                           Edit History

$Header: //components/rel/1x.mpss/8.0/mux/src/mux1xdiag.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/12/14   sst     Add feature to distinguish between Int and Ext Builds
03/07/14   srk     Remove DEVICE_ONLY_DTX_F diag command.
10/03/13   srk     Add diag command to control T2P boost for DTX'd frames
09/26/13   dkb     Add support for FW diag buffer
06/14/13   pap     Aggregating different global variables into structures.
10/18/12   srk     Remove the diag commands used for controlling debug F3s.
09/11/12   srk     Simplify the diag interface for enabling debug F3s for DTX
08/27/12   srk     Change the dtx power boost variable name.
08/23/12   srk     Stop DTX when SCH is active.
08/02/12   srk     Added diag commands to enable/disable Device only DTX.
04/16/12   jtm     Moved QOF FCH Noise Estimation to SRCH.
01/23/12   srk     Feature Cleanup
12/01/11   srk     Added QOF FCH Noise Estimation configurability.
10/05/11   srk     Added F-FCH get stats and reset stats functions.
08/04/11   jtm     Added power boost configurability to the TT config command.
07/26/11   vks     Fix compiler warnings.
06/08/11   jtm     Added Quick Repeat capability that is configurable.
05/23/11   jtm     Provided a mechanism to allow a configurable delay from TX on
                   until we send a signaling message.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
03/28/11   jtm     Added capability for MSO RDA test to be enabled dynamically.
01/25/11   jtm     Added Mux<-->Vocoder debugging capability.
12/08/10   trc     Lint fixes
12/03/10   trc     Rewrote this to be more usable/maintainable
10/30/10   jtm     Modem 1x SU API changes.
01/20/09   mca     Updated PACKED macro
08/05/09   jtm     Lint fixes.
03/27/09   jtm     Eliminate implicit header includes.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
07/21/08   jtm     Removed FEATURE_IS2000_REL_C_DV code and fixed Klockwork issues.
10/13/04   bn      Added logic allow user to set DV logmask from QXDM menu
06/29/04   jrp     Added MUX1X_DIAG_PAGING_STAT_F to diag table
04/20/04   jrp     Fixed lint warnings.
03/09/04   sr/bkm  Featurized DV code
11/14/03   cf/vlc  Fixed compiler warnings (casting) for RVCT compiler.
10/21/03   bn      Added support for retrieving paging statistic counters via DIAG
04/15/03   bn      Created new file for DIAG MUX1X sub-system access.
===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

//mux
#include "mux1xdiag.h"
#include "rxc_i.h"       /* Interface to RXC subtask */
#include "rxccommon.h"
#include "rxcpaging.h"
#include "txctraffic_i.h"
#include "muxmdsp_i.h"

//other
#include "diagcmd.h"
#include "diagpkt.h"

/*===========================================================================

  How to define sub-commands for MUX1X sub-system.  Can be accessed via QXDM
  via: send_data 75 27 SUBCMD 00 DATA ...

  1) Add unique SUBCMD enum to the list below
  2) Using the template at the bottom of this file, add the SUBCMD handler
  3) Add an entry to the dispatch table
  4) Define request/response packet contents, internal to the above handler
     and hook up the necessary plumbing.  Try to keep this as local as
     possible to within the handler function, and inside just the response
     generation block, if possible.  See the other implementations for
     examples.

     * Note that externs must either come via #include above, or be declared
       outside of function scope, due to annoying gcc compiler warnings.
       Everything else should be able to be contained inside the handler.

     * Note the handful of helper macros below.  Not the prettiest things
       around, but helps make the implementation consistent and minimizes
       the boilerplate necessary to get something plugged into DIAG quickly.

===========================================================================*/


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* Helper macros to declare request and response packets/types */
#undef DIAG_REQ_TYPE
#define DIAG_REQ_TYPE(subcmd) subcmd##_req_type
#undef DIAG_RSP_TYPE
#define DIAG_RSP_TYPE(subcmd) subcmd##_rsp_type
#undef DIAG_REQ
#define DIAG_REQ(subcmd)                                           \
  struct subcmd##_req_tag;                                         \
  typedef PACK(struct) subcmd##_req_tag DIAG_REQ_TYPE(subcmd);     \
  PACK(struct) subcmd##_req_tag {                                  \
    diagpkt_subsys_header_type header; //lint --e{754}
#undef DIAG_RSP
#define DIAG_RSP(subcmd)                                           \
  struct subcmd##_rsp_tag;                                         \
  typedef PACK(struct) subcmd##_rsp_tag DIAG_RSP_TYPE(subcmd);     \
  PACK(struct) subcmd##_rsp_tag {                                  \
    diagpkt_subsys_header_type header; //lint --e{754}
#undef DIAG_END
#define DIAG_END };

/* Helper macro to wrap the diag handler function */
#undef DIAG_HANDLER
#define DIAG_HANDLER(subcmd) \
  static PACK(void*) subcmd##_handler (PACK(void*)req_pkt,uint16 pkt_len)

/* Helper macro to wrap allocating a response packet */
#undef DIAG_ALLOC_RSP
#define DIAG_ALLOC_RSP(subcmd)                                     \
  (DIAG_RSP_TYPE(subcmd) *)                                        \
  diagpkt_subsys_alloc((diagpkt_subsys_id_type) DIAG_SUBSYS_MUX1X, \
                       (diagpkt_subsys_cmd_code_type) subcmd,      \
                       sizeof(DIAG_RSP_TYPE(subcmd)) )

/* Helper macro to populate the diag handler table */
#undef DIAG_TBL_ENTRY
#define DIAG_TBL_ENTRY(subcmd) {subcmd,subcmd,subcmd##_handler}


/* Global Variables */
#define MUX_DIAG_SUCCESS 0
#define MUX_DIAG_FAILURE 1

/**************************************************************************
    DIAG REQUEST/RESPONSE SUBCMD DEFINITIONS FOR MUX1X SUB-SYSTEM

    Add one entry per unique handler function below.
**************************************************************************/
enum {
  FER_F                     = 0,  /* FER sub-command */
  PAGING_STAT_F             = 1,  /* Paging statistic sub-command */
  RL_TEST_PATTERN_F         = 2,  /* Turn on/off RL frame test pattern */
  RESERVED_1_F              = 3,  /* Reserved */
  MSO_RDA_TEST_F            = 4,  /* Turn on/off MSO test capability */
  RL_TX_CONFIG_F            = 5,  /* Configure TT parameters */
  FCH_GET_STATS_F           = 6,  /* Returns the F-FCH statistics for FER and SER */
  FCH_RESET_STATS_F         = 7,  /* Resets the F-FCH statistic counters that record FER and SER*/
  CFW_DIAG_DATA_F           = 8,  /* Configure CDMA FW parameters */
  RESERVED_3_F              = 9,  /* Reserved */
};
/* end of define sub-commands for MUX1X sub-system */


/**************************************************************************
    DIAG REQUEST/RESPONSE SUBCMD HANDLER FUNCTIONS
**************************************************************************/

#ifdef FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY
#error code not present
#endif /* FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY */


#ifdef FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY
#error code not present
#endif /* FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY */


#ifdef FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY
#error code not present
#endif /* FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY */


#ifdef FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY
#error code not present
#endif /* FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY */


#ifdef FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY
#error code not present
#endif /* FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY */

/*===========================================================================
Function:  FCH_GET_STATS handler
Description:

       This function:

       - Returns the F-FCH statistics for FER and SER. The statistics for all
         data rates are recorded into 2 the same set of accumulators, so there
         is no way to distinguish between results for different data 3 rates.

       - For erasure frames, the symbol errors are determined by using the symbol
         errors associated with 5 the full rate calculations, as determined by
         the Rate Determination Algorithm.

       - Command: send_data 75 27 6 0

============================================================================*/
DIAG_HANDLER( FCH_GET_STATS_F )
{
  uint32 i;
  rxc_fer_ser_report_type local_stats;  /* Local structure variable to hold stats */

  DIAG_REQ( FCH_GET_STATS_F )
  DIAG_END

  DIAG_RSP( FCH_GET_STATS_F )
  uint8              status;
  uint32             fch_frame_total;
  uint32             fch_frame_errors;
  uint32             fch_sym_total;
  uint32             fch_sym_errors;
  uint32             fch_rate_cnt[5];
  DIAG_END

  /* Get pointers to the request and response packets */
  DIAG_REQ_TYPE( FCH_GET_STATS_F ) *req_ptr =
    (DIAG_REQ_TYPE( FCH_GET_STATS_F ) *)req_pkt;
  DIAG_RSP_TYPE( FCH_GET_STATS_F ) *rsp_ptr = NULL;

  /* Make sure the request was the correct length */
  if (pkt_len == sizeof(DIAG_REQ_TYPE( FCH_GET_STATS_F )))
  {
    /* Allocate the memory for the response */
    rsp_ptr = DIAG_ALLOC_RSP( FCH_GET_STATS_F );
  }

  /* If both request and response pointers look good, process it */
  if ((req_ptr != NULL) && (rsp_ptr != NULL))
  {
    if( (rxc_get_fer_ser_report( &local_stats, FALSE ) == 0) && (rxc_get_state() == RXC_TC_STATE) )
    {
      rsp_ptr->status = MUX_DIAG_SUCCESS;
      rsp_ptr->fch_frame_errors = local_stats.fch_frame_errors;
      rsp_ptr->fch_frame_total  = local_stats.fch_frame_total;
      rsp_ptr->fch_sym_errors   = local_stats.fch_sym_errors;
      rsp_ptr->fch_sym_total    = local_stats.fch_sym_total;

      for(i = 0; i < 5; i++)
      {
        rsp_ptr->fch_rate_cnt[i] = local_stats.fch_rate_cnt[i];
      }
    }
    else
    {
      rsp_ptr->status = MUX_DIAG_FAILURE;
    }
  }

  /* Return results/status */
  return(rsp_ptr);
} /* END OF FCH_GET_STATS HANDLER */

/*===========================================================================
Function:  FCH_RESET_STATS handler
Description:

       This function:

       - Resets the F-FCH statistic counters that record FER and SER. The FER
         and SER statistics for all 6 data rates are recorded into the same set
         of accumulators. When this function is called, the values 7 of all
         accumulators are set to 0.

       - Command: send_data 75 27 7 0

============================================================================*/
DIAG_HANDLER( FCH_RESET_STATS_F )
{
  DIAG_REQ( FCH_RESET_STATS_F )
  DIAG_END

  DIAG_RSP( FCH_RESET_STATS_F )
  uint8                status;
  DIAG_END

  /* Get pointers to the request and response packets */
  DIAG_REQ_TYPE( FCH_RESET_STATS_F ) *req_ptr =
    (DIAG_REQ_TYPE( FCH_RESET_STATS_F ) *)req_pkt;
  DIAG_RSP_TYPE( FCH_RESET_STATS_F ) *rsp_ptr = NULL;

  /* Make sure the request was the correct length */
  if (pkt_len == sizeof(DIAG_REQ_TYPE( FCH_RESET_STATS_F )))
  {
    /* Allocate the memory for the response */
    rsp_ptr = DIAG_ALLOC_RSP( FCH_RESET_STATS_F );
  }

  /* If both request and response pointers look good, process it */
  if ((req_ptr != NULL) && (rsp_ptr != NULL))
  {
    if( (rxc_get_fer_ser_report( NULL, TRUE ) == 0) && (rxc_get_state() == RXC_TC_STATE) )
    {
      rsp_ptr->status = MUX_DIAG_SUCCESS;
    }
    else
    {
      rsp_ptr->status = MUX_DIAG_FAILURE;
    }
  }

  /* Return results/status */
  return(rsp_ptr);
} /* END OF FCH_RESET_STATS HANDLER */

/*===========================================================================
Function:  CFW_DIAG_DATA_F handler
Description:
============================================================================*/
DIAG_HANDLER( CFW_DIAG_DATA_F )
{
  DIAG_REQ( CFW_DIAG_DATA_F )
    uint8 data_start;
  DIAG_END

  DIAG_RSP( CFW_DIAG_DATA_F )
  uint8                status;
  DIAG_END

  /* Get pointers to the request and response packets */
  DIAG_REQ_TYPE( CFW_DIAG_DATA_F ) *req_ptr =
    (DIAG_REQ_TYPE( CFW_DIAG_DATA_F ) *)req_pkt;
  DIAG_RSP_TYPE( CFW_DIAG_DATA_F ) *rsp_ptr = NULL;

  /* Allocate the memory for the response */
  rsp_ptr = DIAG_ALLOC_RSP( CFW_DIAG_DATA_F );

  /* If both request and response pointers look good, process it */
  if ((req_ptr != NULL) && (rsp_ptr != NULL))
  {
    rsp_ptr->status = MUX_DIAG_FAILURE;

    if ( pkt_len < sizeof(req_ptr->header) )
    {
     M1X_MSG( MUX, LEGACY_ERROR,
       "Invalid packet length %u",
       pkt_len );
    }
    else if ( 0 == muxmdsp_update_fw_diag_buf( &req_ptr->data_start,
                                               pkt_len - sizeof(req_ptr->header)
                                             ) )
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Successfully updated FW diag data buffer." );
      rsp_ptr->status = MUX_DIAG_SUCCESS;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Failed to update FW diag data buffer!" );
    }
  }

  /* Return results/status */
  return(rsp_ptr);
} /* END OF CFW_DIAG_DATA_F HANDLER */

/**************************************************************************
** DISPATCH TABLE.
Note:  Any new commands for MUX1X need to be added here.
****************************************************************************/
static const diagpkt_user_table_entry_type mux1xdiag_tbl[] =
{
  #ifdef FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY
  #error code not present
#endif /* FEATURE_MODEM_1X_INTERNAL_BUILDS_ONLY */
  DIAG_TBL_ENTRY(FCH_GET_STATS_F),
  DIAG_TBL_ENTRY(FCH_RESET_STATS_F),
  DIAG_TBL_ENTRY(CFW_DIAG_DATA_F),
};


/*--------------------------------------------------------------------------
  MUX1XDIAG_INIT() - Perform initial registration of DIAG packets
--------------------------------------------------------------------------*/
void mux1xdiag_init (void)
{
  DIAGPKT_DISPATCH_TABLE_REGISTER ((uint16)DIAG_SUBSYS_MUX1X, mux1xdiag_tbl);
}


/* Helper template for a diag handler function */
#if 0
/*===========================================================================
Function:  SUBCMD_NAME handler
Description:
============================================================================*/
DIAG_HANDLER( SUBCMD_NAME )
{
  DIAG_REQ( SUBCMD_NAME )
    //optional request members go here
    //uint8           foo;
  DIAG_END

  DIAG_RSP( SUBCMD_NAME )
  //optional response members go here
  //uint8             bar;
  DIAG_END

  /* Get pointers to the request and response packets */
  DIAG_REQ_TYPE( SUBCMD_NAME ) *req_ptr =
    (DIAG_REQ_TYPE( SUBCMD_NAME ) *)req_pkt;
  DIAG_RSP_TYPE( SUBCMD_NAME ) *rsp_ptr = NULL;

  /* Make sure the request was the correct length */
  if (pkt_len == sizeof(DIAG_REQ_TYPE( SUBCMD_NAME )))
  {
    /* Allocate the memory for the response */
    rsp_ptr = DIAG_ALLOC_RSP( SUBCMD_NAME );
  }

  /* If both request and response pointers look good, process it */
  if ((req_ptr != NULL) && (rsp_ptr != NULL))
  {
  }

  /* Return results/status */
  return(rsp_ptr);
} /* END OF SUBCMD_NAME HANDLER */
#endif /* end of handler template */
