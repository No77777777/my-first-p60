/*===========================================================================

                                  R R C L C M

GENERAL DESCRIPTION
 The RRCLCM module consists of Layer 2 ACK Manager and RLC Logical Channel ID 
 Manager.

EXTERNALIZED FUNCTIONS

  tdsrrclcm_init_ul_rlc_lc_info()
    This function will initialize Uplink RLC Logical Channel Info to the 
    default values. All the Uplink RLC Logical Channel entities will be set to
    "Not in use" status.

  tdsrrclcm_init_dl_rlc_lc_info()
    This function will initialize Downlink RLC Logical Channel Info to the 
    default values. All the Downlink RLC Logical Channel entities will be set
    to "Not in use" status.

  tdsrrclcm_allocate_ul_rlc_lc_id()
    This function will allocate Uplink RLC Logical Channel Id for the 
    requested RLC service entity.

  tdsrrclcm_allocate_dl_rlc_lc_id()
    This function will allocate Downlink RLC Logical Channel Id for the
    requested RLC service entity.

  tdsrrclcm_check_ul_rlc_lc_id()
    This function will check whether Uplink RLC Logical Channel Id is alloated
    or not for the requested RLC service entity.

  tdsrrclcm_check_dl_rlc_lc_id()
    This function will check whether Downlink RLC Logical Channel Id is alloated
    or not for the requested RLC service entity.

  tdsrrclcm_deallocate_ul_rlc_lc_id()
    This function will deallocate Uplink RLC Logical Channel Id requested

  tdsrrclcm_deallocate_dl_rlc_lc_id()
    This function will deallocate Downlink RLC Logical Channel Id requested
    
  tdsrrclcm_get_dl_lc_type()
    This function will identify the Logical Channel to which the OTA message
    belongs. The RRC procedure uses this infomation for processing the 
    received message.

  tdsrrclcm_get_ul_lc_type()
    This function will identify the Logical Channel to which the OTA message
    belongs. The RRC procedure uses this infomation for sending the OTA 
    message.

  tdsrrclcm_identify_proc()
    This function will processes the event from the Dispatcher and identifies
    the procedure to which the L2 ACK belongs.

  tdsrrclcm_get_mui()
    This function builds the MUI for the RRC procedure and RRC procedure can
    include the same in the SDU if it is transmitting SDU in Acknowledged mode.
    
  tdsrrclcm_init_l2_ack_info()
    This function will initialize the L2 ACK info of all RRC procedures.

  tdsrrclcm_post_event_for_dl_sdu()
    This function will post an internal event to RRC for the downlink SDU.

  tdsrrclcm_get_dl_watermark_ptr()
    This function will get watermark pointer corresponding to the RLC Logical
    channel Id.

  tdsrrclcm_get_ul_watermark_ptr()
    This function will get watermark pointer corresponding to the RLC Logical
    channel Id.

  tdsrrclcm_get_dl_rb_id()
    This function will get radio bearer Id  corresponding to the RLC Logical
    channel Id.

  tdsrrclcm_get_ul_rb_id()
    This function will get radio bearer Id  corresponding to the RLC Logical
    channel Id.

  tdsrrclcm_clear_rlc_lc_info()
    This function clears and initializes the RLC Logical Channel Id
    info for both Uplink and Downlink.

  tdsrrclcm_get_ul_rlc_mode
    This function will check whether Uplink RLC Logical Channel Id is alloated 
    or not for the requested RLC service entity. If allocated then it returns
    assoctaed mode of transmission.

  tdsrrclcm_get_dl_rlc_mode
    This function will check whether Downlink RLC Logical Channel Id is alloated 
    or not for the requested RLC service entity. If allocated then it returns 
    associated mode reception.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrclcm.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   yzh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
11/02/10   as      Fixed compiler error
10/28/10   as      Added changes to check for DL ACK for Tuneaway
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "tdscdma_variation.h"
#include "tdsrrclcm.h"
#include "msg.h"
#include "err.h"
#include "tdsuecomdef.h"
#include "tdsrrcllc.h"
//CRMQTF
//#ifdef TEST_FRAMEWORK
//#include "tds_rrc_stub_functions.h"
//#endif
#include "tdsrrcwrm.h"
#include "tdsrlcrrcif.h"
#include "rrcdata.h" 
#include "tdsrrc_dependancy.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/


/*===========================================================================

               Data structures needed for communicating with RLC.

===========================================================================*/
               
static dsm_watermark_type tdsrrclcm_rb32_bcch_dl_wmq;   
                                           /* WaterMark Queue used when SDU
                                              is received from RLC on RB 32 
                                              using BCCH Logical channel
                                              for serving cell. */
static q_type tdsrrclcm_rb32_bcch_dl_q;        /* Queue around which 
                                              tdsrrclcm_rb32_bcch_dl_wmq is 
                                              built. */

static dsm_watermark_type tdsrrclcm_rb33_bcch_dl_wmq;
                                           /* WaterMark Queue used when SDU
                                              is receiveed from RLC on RB 33
                                              using BCCH Logical Channel for 
                                              neighbor cell. */

static q_type tdsrrclcm_rb33_bcch_dl_q;         /* Queue around which 
                                              tdsrrclcm_rb33_bcch_dl_wmq is 
                                              built. */
static dsm_watermark_type tdsrrclcm_rb34_bcch_fach_dl_wmq;
                                           /* WaterMark Queue used when SDU
                                              is receiveed from RLC on RB 34
                                              using BCCH Logical Channel 
                                              which is mapped to FACH transport
                                              channel. */

static q_type tdsrrclcm_rb34_bcch_fach_dl_q;   /* Queue around which 
                                              tdsrrclcm_rb34_bcch_fach_dl_wmq is 
                                              built. */
static dsm_watermark_type tdsrrclcm_rb35_pcch_dl_wmq;
                                           /* WaterMark Queue used when SDU
                                              is receiveed from RLC on RB 35
                                              using PCCH Logical Channel. */

static q_type tdsrrclcm_rb35_pcch_dl_q;        /* Queue around which 
                                              tdsrrclcm_rb35_pcch_dl_wmq is 
                                              built. */
static dsm_watermark_type tdsrrclcm_rb0_ccch_dl_wmq;   
                                           /* WaterMark Queue used when SDU
                                              is received from RLC on RB 0 
                                              using CCCH Logical channel.*/
static q_type tdsrrclcm_rb0_ccch_dl_q;         /* Queue around which 
                                              tdsrrclcm_rb0_ccch_dl_wmq is 
                                              built. */

static dsm_watermark_type tdsrrclcm_rb0_ccch_ul_wmq;
                                           /* WaterMark Queue used when SDU
                                              is transmitted to RLC on RB 0
                                              using CCCH Logical Channel. */

static q_type tdsrrclcm_rb0_ccch_ul_q;         /* Queue around which 
                                              tdsrrclcm_rb0_ccch_ul_wmq is 
                                              built. */
static dsm_watermark_type tdsrrclcm_rb1_dcch_dl_wmq;   
                                           /* WaterMark Queue used when SDU
                                              is received from RLC on RB 1 
                                              using DCCH Logical channel. */
static q_type tdsrrclcm_rb1_dcch_dl_q;         /* Queue around which 
                                              tdsrrclcm_rb1_dcch_dl_wmq is 
                                              built. */

static dsm_watermark_type tdsrrclcm_rb1_dcch_ul_wmq;
                                           /* WaterMark Queue used when SDU
                                              is transmitted to RLC on RB 1
                                              using DCCH Logical Channel. */

static q_type tdsrrclcm_rb1_dcch_ul_q;         /* Queue around which 
                                              tdsrrclcm_rb1_dcch_ul_wmq is 
                                              built. */
static dsm_watermark_type tdsrrclcm_rb2_dcch_dl_wmq;   
                                           /* WaterMark Queue used when SDU
                                              is received from RLC on RB 2 
                                              using DCCH Logical channel. */
static q_type tdsrrclcm_rb2_dcch_dl_q;         /* Queue around which 
                                              tdsrrclcm_rb2_dcch_dl_wmq is 
                                              built. */

static dsm_watermark_type tdsrrclcm_rb2_dcch_ul_wmq;
                                           /* WaterMark Queue used when SDU
                                              is transmitted to RLC on RB 2
                                              using DCCH Logical Channel. */

static q_type tdsrrclcm_rb2_dcch_ul_q;         /* Queue around which 
                                              tdsrrclcm_rb2_dcch_ul_wmq is 
                                              built. */
static dsm_watermark_type tdsrrclcm_rb3_dcch_dl_wmq;   
                                           /* WaterMark Queue used when SDU
                                              is received from RLC on RB 3 
                                              using DCCH Logical channel. */
static q_type tdsrrclcm_rb3_dcch_dl_q;         /* Queue around which 
                                              tdsrrclcm_rb3_dcch_dl_wmq is 
                                              built. */

static dsm_watermark_type tdsrrclcm_rb3_dcch_ul_wmq;
                                           /* WaterMark Queue used when SDU
                                              is transmitted to RLC on RB 3
                                              using DCCH Logical Channel. */

static q_type tdsrrclcm_rb3_dcch_ul_q;         /* Queue around which 
                                              tdsrrclcm_rb3_dcch_ul_wmq is 
                                              built. */
static dsm_watermark_type tdsrrclcm_rb4_dcch_dl_wmq;   
                                           /* WaterMark Queue used when SDU
                                              is received from RLC on RB 4 
                                              using DCCH Logical channel. */
static q_type tdsrrclcm_rb4_dcch_dl_q;         /* Queue around which 
                                              tdsrrclcm_rb4_dcch_dl_wmq is 
                                              built. */

static dsm_watermark_type tdsrrclcm_rb4_dcch_ul_wmq;
                                           /* WaterMark Queue used when SDU
                                              is transmitted to RLC on RB 4
                                              using DCCH Logical Channel. */

static q_type tdsrrclcm_rb4_dcch_ul_q;         /* Queue around which 
                                              tdsrrclcm_rb4_dcch_ul_wmq is 
                                              built. */

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
static dsm_watermark_type tdsrrclcm_srb5_dcch_dl_wmq;   
                                           /* WaterMark Queue used when SDU
                                              is received from RLC on SRB#5 
                                              using DCCH_TM Logical channel
                                              for serving cell. */
static q_type tdsrrclcm_srb5_dcch_dl_q;        /* Queue around which 
                                              tdsrrclcm_srb5_dcch_dl_wmq is 
                                              built. */
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */


static boolean tdsrrclcm_init_current_mui_array_at_poweup = TRUE;
/*===========================================================================
  Defining the static array that stores items for L2 ACK management and RLC
  Logical Channel ID Management
===========================================================================*/

static tdsrrclcm_l2_ack_info_type 
  tdsrrclcm_l2_ack_info[TDSMAX_NUM_OF_PROCEDURES][TDSUE_MAX_UL_AM_DATA_CHANNEL - 1];
                                           
static tdsrrclcm_ul_rlc_lc_info_type tdsrrclcm_ul_rlc_lc_info
                                  [TDSUE_MAX_UL_LOGICAL_CHANNEL];

static tdsrrclcm_dl_rlc_lc_info_type tdsrrclcm_dl_rlc_lc_info
                                  [TDSUE_MAX_DL_LOGICAL_CHANNEL];       

/*===========================================================================
** -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */

/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this …. 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */


/*===========================================================================


FUNCTION tdsrrclcm_check_dl_rlc_lc_id

DESCRIPTION
  This function will check whether Downlink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. RRC LLC can use this
  function to check whether a particular Logical channel entity is already 
  setup or not. If this function returns TDSRRCLCM_RLC_LC_ID_NOT_FOUND, then the
  RRC LLC needs to setup Logical Channel entity  and the corresponding 
  physical channel for receiving SDU on the downlink.

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If it is not alreday allocated,
   it'll return TDSRRCLCM_RLC_LC_ID_NOT_FOUND.

SIDE EFFECTS
  None
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

rlc_lc_id_type tdsrrclcm_check_dl_rlc_lc_id
(    
  uecomdef_logchan_e_type dl_lc_type,       /* Downlink Logical Channel Type. */
  tdsrrc_RB_Identity rb_id,                    /* Radio Bearer Id assoicated with
                                               the above Downlink Logical 
                                               Channel type. */
  uecomdef_logch_mode_e_type mode           /* Mode of transmission for the RLC 
                                               service entity. */
 
)
{
  int i=0;
  boolean dl_lc_id_found = FALSE;

  switch (mode) 
  {
  /* Checks for Logical Channel Type and Radio Bearer Id match for Transparent
     mode. */
    case (UE_MODE_TRANSPARENT): 
      for( i =TDSRRCLCM_DL_TM_START_IDX ; i < UE_MAX_DL_TM_CHANNEL ; i++ )
      {
        if( dl_lc_type == tdsrrclcm_dl_rlc_lc_info[i].lc_type && 
             rb_id == tdsrrclcm_dl_rlc_lc_info[i].rb_id &&
             tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
        {
          dl_lc_id_found = TRUE;           /* Match for the requested service 
                                              is found */
          break;
        }
      }
      break;
  /* Checks for Logical Channel Type and Radio Bearer Id match for 
     Unacknowledged mode. */
    case (UE_MODE_UNACKNOWLEDGED): 
      for( i = TDSRRCLCM_DL_UM_START_IDX ; i < (TDSRRCLCM_DL_UM_START_IDX +
               TDSUE_MAX_DL_UM_CHANNEL) ; i++ )
      {
        if( dl_lc_type == tdsrrclcm_dl_rlc_lc_info[i].lc_type && 
             rb_id == tdsrrclcm_dl_rlc_lc_info[i].rb_id &&
             tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
        {
          dl_lc_id_found = TRUE;           /* Match for the requested service 
                                              is found */
          break;
        }
      }
      break;  
  /* Checks for Logical Channel Type and Radio Bearer Id match for 
     Acknowledged Data mode. */
    case (UE_MODE_ACKNOWLEDGED_DATA): 
      for( i = (TDSRRCLCM_DL_AM_DATA_START_IDX); 
           i < (TDSRRCLCM_DL_AM_DATA_START_IDX + TDSUE_MAX_DL_AM_DATA_CHANNEL) ;
           i++ )
      {
        if( dl_lc_type == tdsrrclcm_dl_rlc_lc_info[i].lc_type && 
             rb_id == tdsrrclcm_dl_rlc_lc_info[i].rb_id &&
             tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
        {
          dl_lc_id_found = TRUE;           /* Match for the requested service 
                                              is found */
          break;
        }
      }
      break;              
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"This should never happen !!");
      break;
  }
  if(dl_lc_id_found)
  {
    return (tdsrrclcm_dl_rlc_lc_info[i].lc_id);
  }
  else
  {
    return (TDSRRCLCM_RLC_LC_ID_NOT_FOUND);   /* Match for the requested service 
                                              is not found. The RRC LLC needs 
                                              to request for allocation. */
  }
} /* tdsrrclcm_check_dl_rlc_lc_id */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION tdsrrclcm_get_ul_rb_id

DESCRIPTION
  This function will get radio bearer Id  corresponding to the RLC Logical
  channel Id.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrc_RB_Identity.

SIDE EFFECTS
  None
===========================================================================*/
 /***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_RB_Identity tdsrrclcm_get_ul_rb_id
(    
  rlc_lc_id_type ul_lc_id                  /* The uplink RLC Logical
                                              Channel Id. */   
)
{
  if(ul_lc_id >= TDSUE_MAX_UL_LOGICAL_CHANNEL)
  {
    ERR_FATAL("ul_lc_id out of bounds : 0x%x",ul_lc_id,0,0);
  }
  return tdsrrclcm_ul_rlc_lc_info[ul_lc_id].rb_id ;
} /* rrclcm_get_rb_dl_id */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================

FUNCTION tdsrrclcm_create_ul_dl_rlc_lc_q

DESCRIPTION
  This function will crete the Uplink/Downlinkwatermark queues

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrclcm_create_ul_dl_rlc_lc_q
(
  void
)
{
    /* Set up Queue for Transmitting the data to RLC on RB 0 using CCCH 
     Logical Channe. */
  (void) dsm_queue_init(&tdsrrclcm_rb0_ccch_ul_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb0_ccch_ul_q);
  tdsrrclcm_rb0_ccch_ul_wmq.q_ptr = &tdsrrclcm_rb0_ccch_ul_q;
  tdsrrclcm_rb0_ccch_ul_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb0_ccch_ul_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb0_ccch_ul_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for Transmitting the data to RLC on RB 1 using DCCH 
     Logical Channe. */
  (void) dsm_queue_init(&tdsrrclcm_rb1_dcch_ul_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb1_dcch_ul_q);
  tdsrrclcm_rb1_dcch_ul_wmq.q_ptr = &tdsrrclcm_rb1_dcch_ul_q;
  tdsrrclcm_rb1_dcch_ul_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb1_dcch_ul_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb1_dcch_ul_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for Transmitting the data to RLC on RB 2 using DCCH 
     Logical Channe. */
  (void) dsm_queue_init(&tdsrrclcm_rb2_dcch_ul_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb2_dcch_ul_q);
  tdsrrclcm_rb2_dcch_ul_wmq.q_ptr = &tdsrrclcm_rb2_dcch_ul_q;
  tdsrrclcm_rb2_dcch_ul_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb2_dcch_ul_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb2_dcch_ul_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for Transmitting the data to RLC on RB 3 using DCCH 
     Logical Channe. */
  (void) dsm_queue_init(&tdsrrclcm_rb3_dcch_ul_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb3_dcch_ul_q);
  tdsrrclcm_rb3_dcch_ul_wmq.q_ptr = &tdsrrclcm_rb3_dcch_ul_q;
  tdsrrclcm_rb3_dcch_ul_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb3_dcch_ul_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb3_dcch_ul_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for Transmitting the data to RLC on RB 4 using DCCH 
     Logical Channe. */
  (void) dsm_queue_init(&tdsrrclcm_rb4_dcch_ul_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb4_dcch_ul_q);
  tdsrrclcm_rb4_dcch_ul_wmq.q_ptr = &tdsrrclcm_rb4_dcch_ul_q;
  tdsrrclcm_rb4_dcch_ul_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb4_dcch_ul_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb4_dcch_ul_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

   /* Set up Queue for receiving the data from RLC on RB 32 using BCCH 
     Logical Channel for serving cell. */
  (void) dsm_queue_init(&tdsrrclcm_rb32_bcch_dl_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb32_bcch_dl_q);
  tdsrrclcm_rb32_bcch_dl_wmq.q_ptr = &tdsrrclcm_rb32_bcch_dl_q;
  tdsrrclcm_rb32_bcch_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb32_bcch_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb32_bcch_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for receiving the data from RLC on RB 33 using BCCH 
     Logical Channel for neighbor cell. */
  (void) dsm_queue_init(&tdsrrclcm_rb33_bcch_dl_wmq, 8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb33_bcch_dl_q);
  tdsrrclcm_rb33_bcch_dl_wmq.q_ptr = &tdsrrclcm_rb33_bcch_dl_q;
  tdsrrclcm_rb33_bcch_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb33_bcch_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb33_bcch_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for receiving the data from RLC on RB 34 using BCCH 
     Logical Channel mapped to FACH transport channel. */
  (void) dsm_queue_init(&tdsrrclcm_rb34_bcch_fach_dl_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb34_bcch_fach_dl_q);
  tdsrrclcm_rb34_bcch_fach_dl_wmq.q_ptr = &tdsrrclcm_rb34_bcch_fach_dl_q;
  tdsrrclcm_rb34_bcch_fach_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb34_bcch_fach_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb34_bcch_fach_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

   /* Set up Queue for receiving the data from RLC on RB 35 using PCCH 
     Logical Channel. */
  (void) dsm_queue_init(&tdsrrclcm_rb35_pcch_dl_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb35_pcch_dl_q);
  tdsrrclcm_rb35_pcch_dl_wmq.q_ptr = &tdsrrclcm_rb35_pcch_dl_q;
  tdsrrclcm_rb35_pcch_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb35_pcch_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb35_pcch_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

/* Set up Queue for receiving the data from RLC on RB 0 using CCCH 
     Logical Channel. */
  (void) dsm_queue_init(&tdsrrclcm_rb0_ccch_dl_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb0_ccch_dl_q);
  tdsrrclcm_rb0_ccch_dl_wmq.q_ptr = &tdsrrclcm_rb0_ccch_dl_q;
  tdsrrclcm_rb0_ccch_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb0_ccch_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb0_ccch_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for receiving the data from RLC on RB 1 using DCCH 
     Logical Channel. */
  (void) dsm_queue_init(&tdsrrclcm_rb1_dcch_dl_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb1_dcch_dl_q);
  tdsrrclcm_rb1_dcch_dl_wmq.q_ptr = &tdsrrclcm_rb1_dcch_dl_q;
  tdsrrclcm_rb1_dcch_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb1_dcch_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;  
  tdsrrclcm_rb1_dcch_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for receiving the data from RLC on RB 2 using DCCH 
     Logical Channel. */
  (void) dsm_queue_init(&tdsrrclcm_rb2_dcch_dl_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb2_dcch_dl_q);
  tdsrrclcm_rb2_dcch_dl_wmq.q_ptr = &tdsrrclcm_rb2_dcch_dl_q;
  tdsrrclcm_rb2_dcch_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb2_dcch_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb2_dcch_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for receiving the data from RLC on RB 3 using DCCH 
     Logical Channel. */
  (void) dsm_queue_init(&tdsrrclcm_rb3_dcch_dl_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb3_dcch_dl_q);
  tdsrrclcm_rb3_dcch_dl_wmq.q_ptr = &tdsrrclcm_rb3_dcch_dl_q;
  tdsrrclcm_rb3_dcch_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb3_dcch_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb3_dcch_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

  /* Set up Queue for receiving the data from RLC on RB 4 using DCCH 
     Logical Channel. */
  (void) dsm_queue_init(&tdsrrclcm_rb4_dcch_dl_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_rb4_dcch_dl_q);
  tdsrrclcm_rb4_dcch_dl_wmq.q_ptr = &tdsrrclcm_rb4_dcch_dl_q;
  tdsrrclcm_rb4_dcch_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb4_dcch_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_rb4_dcch_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  /* Set up Queue for Transmitting the data to RLC on RB 5 using DCCH 
     Logical Channel. */
  (void) dsm_queue_init(&tdsrrclcm_srb5_dcch_dl_wmq,8 * TDSRRCLCM_WATERMARK_SIZE,&tdsrrclcm_srb5_dcch_dl_q);
  tdsrrclcm_srb5_dcch_dl_wmq.q_ptr = &tdsrrclcm_srb5_dcch_dl_q;
  tdsrrclcm_srb5_dcch_dl_wmq.dont_exceed_cnt = 8 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_srb5_dcch_dl_wmq.hi_watermark = 6 * TDSRRCLCM_WATERMARK_SIZE;
  tdsrrclcm_srb5_dcch_dl_wmq.lo_watermark = 2 * TDSRRCLCM_WATERMARK_SIZE;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

}
/*===========================================================================

FUNCTION tdsrrclcm_init_ul_rlc_lc_info

DESCRIPTION
  This function will initialize the Uplink RLC Logical Channel Id Info with 
  the default value. This function also maps the Watermark queues with the
  respective RLC Logical Channel IDs.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_init_ul_rlc_lc_info( void)
{
  uint8 i;

  
 /* Intializes Transparent mode Uplink RLC Logical Channel Ids. */
  for( i = TDSRRCLCM_UL_TM_START_IDX; i < UE_MAX_UL_TM_CHANNEL; i++ )
  {
    switch(i)
  {
      case (TDSRRCLCM_UL_TM_START_IDX): 
         tdsrrclcm_ul_rlc_lc_info[i].lc_id = i;
         tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
         tdsrrclcm_ul_rlc_lc_info[i].lc_type = UE_LOGCHAN_CCCH;
         tdsrrclcm_ul_rlc_lc_info[i].mode = UE_MODE_TRANSPARENT;
         tdsrrclcm_ul_rlc_lc_info[i].rb_id = CCCH_RADIO_BEARER_ID;
         tdsrrclcm_ul_rlc_lc_info[i].tx_queue = &tdsrrclcm_rb0_ccch_ul_wmq;
         break;
      default:
         tdsrrclcm_ul_rlc_lc_info[i].lc_id = i;
         tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
         tdsrrclcm_ul_rlc_lc_info[i].lc_type = UE_LOGCHAN_DTCH;
         tdsrrclcm_ul_rlc_lc_info[i].mode = UE_MODE_TRANSPARENT;
         tdsrrclcm_ul_rlc_lc_info[i].rb_id = TDSRRCLCM_RB_ID_NOT_ALLOCATED;
         tdsrrclcm_ul_rlc_lc_info[i].tx_queue = NULL;
         break;
    }
  }

/* Intializes Unacknowledged mode Uplink RLC Logical Channel Ids. */

  for( i = TDSRRCLCM_UL_UM_START_IDX; i < (TDSRRCLCM_UL_UM_START_IDX + 
       TDSUE_MAX_UL_UM_CHANNEL); i++ )
  {
     
    switch(i)
    {
      case (TDSRRCLCM_UL_UM_START_IDX) : 
        tdsrrclcm_ul_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_ul_rlc_lc_info[i].lc_type = UE_LOGCHAN_DCCH;
        tdsrrclcm_ul_rlc_lc_info[i].mode = UE_MODE_UNACKNOWLEDGED;
        tdsrrclcm_ul_rlc_lc_info[i].rb_id = DCCH_UM_RADIO_BEARER_ID;
        tdsrrclcm_ul_rlc_lc_info[i].tx_queue = &tdsrrclcm_rb1_dcch_ul_wmq;
        break;
      default :
        tdsrrclcm_ul_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_ul_rlc_lc_info[i].lc_type = UE_LOGCHAN_DTCH;
        tdsrrclcm_ul_rlc_lc_info[i].mode = UE_MODE_UNACKNOWLEDGED;
        tdsrrclcm_ul_rlc_lc_info[i].rb_id = TDSRRCLCM_RB_ID_NOT_ALLOCATED;
        tdsrrclcm_ul_rlc_lc_info[i].tx_queue = NULL;
        break;
    }
  }

/* Intializes Acknowledged mode Uplink RLC Logical Channel Ids for Data. */

  for( i = TDSRRCLCM_UL_AM_DATA_START_IDX ; 
       i < (TDSRRCLCM_UL_AM_DATA_START_IDX + TDSUE_MAX_UL_AM_DATA_CHANNEL) ;
       i++ )
  {
    switch(i)
    {
      case (TDSRRCLCM_UL_AM_DATA_START_IDX) :
        tdsrrclcm_ul_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_ul_rlc_lc_info[i].lc_type = UE_LOGCHAN_DCCH;
        tdsrrclcm_ul_rlc_lc_info[i].mode = UE_MODE_ACKNOWLEDGED_DATA;
        tdsrrclcm_ul_rlc_lc_info[i].rb_id = DCCH_AM_RADIO_BEARER_ID;
        tdsrrclcm_ul_rlc_lc_info[i].tx_queue = &tdsrrclcm_rb2_dcch_ul_wmq;
        break;
      case (TDSRRCLCM_UL_AM_DATA_START_IDX +1) :
        tdsrrclcm_ul_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_ul_rlc_lc_info[i].lc_type = UE_LOGCHAN_DCCH;
        tdsrrclcm_ul_rlc_lc_info[i].mode = UE_MODE_ACKNOWLEDGED_DATA;
        tdsrrclcm_ul_rlc_lc_info[i].rb_id = DCCH_DT_HIGH_PRI_RADIO_BEARER_ID;
        tdsrrclcm_ul_rlc_lc_info[i].tx_queue = &tdsrrclcm_rb3_dcch_ul_wmq;
        break;
      case (TDSRRCLCM_UL_AM_DATA_START_IDX+2) :
        tdsrrclcm_ul_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_ul_rlc_lc_info[i].lc_type = UE_LOGCHAN_DCCH;
        tdsrrclcm_ul_rlc_lc_info[i].mode = UE_MODE_ACKNOWLEDGED_DATA;
        tdsrrclcm_ul_rlc_lc_info[i].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
        tdsrrclcm_ul_rlc_lc_info[i].tx_queue = &tdsrrclcm_rb4_dcch_ul_wmq;
        break;
      default:
        tdsrrclcm_ul_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_ul_rlc_lc_info[i].lc_type = UE_LOGCHAN_DTCH;
        tdsrrclcm_ul_rlc_lc_info[i].mode = UE_MODE_ACKNOWLEDGED_DATA;
        tdsrrclcm_ul_rlc_lc_info[i].rb_id = TDSRRCLCM_RB_ID_NOT_ALLOCATED;
        tdsrrclcm_ul_rlc_lc_info[i].tx_queue = NULL;
        break;
    }
  }


} /* tdsrrclcm_init_ul_rlc_lc_info */


/*===========================================================================

FUNCTION tdsrrclcm_init_dl_rlc_lc_info

DESCRIPTION
  This function will initialize the Downlink RLC Logical Channel Id Info with 
  the default value. This function also maps the Watermark queues with the
  respective RLC Logical Channel IDs.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_init_dl_rlc_lc_info
(
 void
)
{  
  uint8 i;

 
  /* Intializes Transparent mode Downlink RLC Logical Entities. */

  for( i = TDSRRCLCM_DL_TM_START_IDX; i < UE_MAX_DL_TM_CHANNEL; i++ )
  {
    switch(i)
    {
      case (TDSRRCLCM_DL_TM_START_IDX): 
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_BCCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_TRANSPARENT;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = BCCH_S_RADIO_BEARER_ID;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = &tdsrrclcm_rb32_bcch_dl_wmq;
        break;
      case (TDSRRCLCM_DL_TM_START_IDX + 1): 
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_BCCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_TRANSPARENT;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = BCCH_N_RADIO_BEARER_ID;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = &tdsrrclcm_rb33_bcch_dl_wmq;
        break;
      case (TDSRRCLCM_DL_TM_START_IDX + 2): 
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_BCCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_TRANSPARENT;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = BCCH_FACH_RADIO_BEARER_ID;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = &tdsrrclcm_rb34_bcch_fach_dl_wmq;
        break;
      case (TDSRRCLCM_DL_TM_START_IDX + 3): 
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_PCCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_TRANSPARENT;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = PCCH_RADIO_BEARER_ID;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = &tdsrrclcm_rb35_pcch_dl_wmq;
        break;
      default:
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_DTCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_TRANSPARENT;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = TDSRRCLCM_RB_ID_NOT_ALLOCATED;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = NULL;
        break;
    }
  }

  /* Intializes Unacknowledged mode RLC Logical Entities. */

  for( i = TDSRRCLCM_DL_UM_START_IDX; i < (TDSRRCLCM_DL_UM_START_IDX + 
       TDSUE_MAX_DL_UM_CHANNEL); i++ )
  {     
    switch(i)
    {
      case (TDSRRCLCM_DL_UM_START_IDX) : 
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_CCCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_UNACKNOWLEDGED;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = CCCH_RADIO_BEARER_ID;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = &tdsrrclcm_rb0_ccch_dl_wmq;
        break;
      case (TDSRRCLCM_DL_UM_START_IDX + 1) : 
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_DCCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_UNACKNOWLEDGED;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = DCCH_UM_RADIO_BEARER_ID;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = &tdsrrclcm_rb1_dcch_dl_wmq;
        break;


      default :
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_DTCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_UNACKNOWLEDGED;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = TDSRRCLCM_RB_ID_NOT_ALLOCATED;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = NULL;
        break;
    }
  }
  /* Intializes Acknowledged mode RLC Logical Entities for Data. */

  for( i = TDSRRCLCM_DL_AM_DATA_START_IDX; 
       i < (TDSRRCLCM_DL_AM_DATA_START_IDX + TDSUE_MAX_DL_AM_DATA_CHANNEL) ;
       i++ ) 
  {
    switch(i)
    {
      case (TDSRRCLCM_DL_AM_DATA_START_IDX) :
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_DCCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_ACKNOWLEDGED_DATA;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = DCCH_AM_RADIO_BEARER_ID;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = &tdsrrclcm_rb2_dcch_dl_wmq;
        break;
      case (TDSRRCLCM_DL_AM_DATA_START_IDX + 1) :
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_DCCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_ACKNOWLEDGED_DATA;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = DCCH_DT_HIGH_PRI_RADIO_BEARER_ID;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = &tdsrrclcm_rb3_dcch_dl_wmq;
        break;
      case (TDSRRCLCM_DL_AM_DATA_START_IDX + 2) :
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_DCCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_ACKNOWLEDGED_DATA;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = &tdsrrclcm_rb4_dcch_dl_wmq;
        break;
      default:
        tdsrrclcm_dl_rlc_lc_info[i].lc_id = i;
        tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
        tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_DTCH;
        tdsrrclcm_dl_rlc_lc_info[i].mode = UE_MODE_ACKNOWLEDGED_DATA;
        tdsrrclcm_dl_rlc_lc_info[i].rb_id = TDSRRCLCM_RB_ID_NOT_ALLOCATED;
        tdsrrclcm_dl_rlc_lc_info[i].rx_queue = NULL;
    }
  }

} /* tdsrrclcm_init_dl_rlc_lc_info() */

/*===========================================================================

FUNCTION tdsrrclcm_clear_rlc_lc_info

DESCRIPTION
  This function clears and initializes the RLC Logical Channel Id
  info for both Uplink and Downlink.
  DSM items in all watermark queues are freed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_clear_rlc_lc_info
( 
 void
)
{
  rlc_lc_id_type ul_lc_id;
  rlc_lc_id_type dl_lc_id;
  dsm_item_type *dsm_item;                  /* Local variable for DSM Item */
  dsm_watermark_type *ul_wm_ptr;            /* uplink Water Mark pointer */
  dsm_watermark_type *dl_wm_ptr;            /* downlink Water Mark pointer */
  for(ul_lc_id = 0; ul_lc_id < TDSUE_MAX_UL_LOGICAL_CHANNEL; ul_lc_id ++)
  {
    /* Dequeue all DSM items in the Uplink watermark queue */
    ul_wm_ptr = tdsrrclcm_get_ul_watermark_ptr(ul_lc_id);

    if(ul_wm_ptr != NULL)
    {
      /* Flush out DSM items in the watermark queue */
      while((dsm_item = dsm_dequeue(ul_wm_ptr)) != NULL)
      {
        /* Free all packets in the chain */
        dsm_free_packet(&dsm_item);
      }
    }
  } /* for(TDSUE_MAX_UL_LOGICAL_CHANNEL) */

  for(dl_lc_id = 0; dl_lc_id < TDSUE_MAX_DL_LOGICAL_CHANNEL; dl_lc_id ++)
  {

    {
      /* Dequeue all DSM items in the Downlink watermark queue */
      dl_wm_ptr = tdsrrclcm_get_dl_watermark_ptr(dl_lc_id);

      if(dl_wm_ptr != NULL)
      {
        /* Flush out DSM items in the watermark queue */
        while((dsm_item = dsm_dequeue(dl_wm_ptr)) != NULL)
        {
          /* Free all packets in the chain */
          dsm_free_packet(&dsm_item);
        }
      }
    }
  } /* for(TDSUE_MAX_DL_LOGICAL_CHANNEL) */

  /* Initialize the UL RLC Logical channel info */
  tdsrrclcm_init_ul_rlc_lc_info();
  /* Initialize the DL RLC Logical channel info */
  tdsrrclcm_init_dl_rlc_lc_info(
    );
  /* Initialize the L2 Ack information */
  tdsrrclcm_init_l2_ack_info();
} /* tdsrrclcm_clear_rlc_lc_info */

/*===========================================================================

FUNCTION tdsrrclcm_allocate_ul_rlc_lc_id

DESCRIPTION
  This function will allocate Uplink RLC Logical Channel Id for the 
  requested RLC service entity. The RRC LLC module uses this function
  to allocate uplink RLC Logical Channel Id.

DEPENDENCIES
  None

RETURN VALUE
  The allocated RLC Logical Channel Id. If failed, it'll return a value 
  TDSRRCLCM_RLC_LC_ID_ERROR.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_allocate_ul_rlc_lc_id
(    
  uecomdef_logchan_e_type ul_lc_type,      /* Uplink Logical Channel Type. */
  tdsrrc_RB_Identity rb_id,                   /* Radio Bearer Id assoicated with
                                              the above Uplink Logical Channel
                                              Type. */
  uecomdef_logch_mode_e_type mode,         /* Mode of transmission for the RLC 
                                              service entity. */
  dsm_watermark_type **dsm_tx_queue        /* Pointer to the DSM Water Mark 
                                              associated with Uplink RLC Logical
                                              Channel Id. For Radio Bearers other
                                              than 0,1,2,3and 4 this will be 
                                              set to NULL pointer. */
)
{
  int i=0;
  boolean ul_lc_id_allocated = FALSE;

  switch(mode) 
  {
  /* Checks for Logical Channel Type and Radio Bearer Id match for Transparent
     mode. */
    case (UE_MODE_TRANSPARENT): 
      for( i = TDSRRCLCM_UL_TM_START_IDX ; i < UE_MAX_UL_TM_CHANNEL ; i++ )
      {
        if(ul_lc_type == tdsrrclcm_ul_rlc_lc_info[i].lc_type &&              
           tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_NOT_IN_USE)
        {
          if( tdsrrclcm_ul_rlc_lc_info[i].rb_id == rb_id ||
              tdsrrclcm_ul_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED )
              
          {
            if(tdsrrclcm_ul_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED )
            {
              tdsrrclcm_ul_rlc_lc_info[i].rb_id = rb_id;
            }
            *dsm_tx_queue = tdsrrclcm_ul_rlc_lc_info[i].tx_queue;
                                           /* Uplink DSM Watermark queue is 
                                              assigned */
            tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_IN_USE;
            ul_lc_id_allocated = TRUE;       /* RLC Logical Channel Id is 
                                                allocated. */
            break;                           /* Comes out of for loop */
          }
           
        }
      }
      break;
 /* Checks for Logical Channel Type and Radio Bearer Id match for 
    Unacknowledged mode. */
    case (UE_MODE_UNACKNOWLEDGED): 
      for( i = TDSRRCLCM_UL_UM_START_IDX ; i < (TDSRRCLCM_UL_UM_START_IDX +
               TDSUE_MAX_UL_UM_CHANNEL) ; i++ )
      {
        if( ul_lc_type == tdsrrclcm_ul_rlc_lc_info[i].lc_type && 
            tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_NOT_IN_USE)
        {
          if( tdsrrclcm_ul_rlc_lc_info[i].rb_id == rb_id ||
              tdsrrclcm_ul_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
          {
            if( tdsrrclcm_ul_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED )
            {  
              tdsrrclcm_ul_rlc_lc_info[i].rb_id = rb_id;
            }
            *dsm_tx_queue = tdsrrclcm_ul_rlc_lc_info[i].tx_queue;
                                             /* Uplink DSM Watermark queue is 
                                                 assigned */
            tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_IN_USE;
            ul_lc_id_allocated = TRUE;       /* RLC Logical Channel Id is 
                                                allocated. */
            break;                           /* Comes out of for loop */
          }

        }
      }
      break;  
 /* Checks for Logical Channel Type and Radio Bearer Id match for 
    Acknowledged Data mode. */
    case (UE_MODE_ACKNOWLEDGED_DATA): 
      for( i = (TDSRRCLCM_UL_AM_DATA_START_IDX); 
           i < (TDSRRCLCM_UL_AM_DATA_START_IDX + TDSUE_MAX_UL_AM_DATA_CHANNEL) ;
           i++ )
      {
        if( ul_lc_type == tdsrrclcm_ul_rlc_lc_info[i].lc_type && 
            tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_NOT_IN_USE)
        {
          if( tdsrrclcm_ul_rlc_lc_info[i].rb_id == rb_id ||
              tdsrrclcm_ul_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
          {
            if(tdsrrclcm_ul_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED )
            {
              tdsrrclcm_ul_rlc_lc_info[i].rb_id = rb_id;
            }
            *dsm_tx_queue = tdsrrclcm_ul_rlc_lc_info[i].tx_queue;
                                             /* Uplink DSM Watermark queue is 
                                                assigned. */
            tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_IN_USE;
            ul_lc_id_allocated = TRUE;       /* RLC Logical Channel Id is 
                                                allocated. */
            break;                           /* Comes out of for loop */
          }
        }
      }
      break;


    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"This should never happen !!");
      break;
  }
  if(ul_lc_id_allocated)
  {
    TDSRRC_MSG3(MSG_LEGACY_MED,"Allocated UL lc_id: %d, rb_id: %d, lc_type:%d",
             tdsrrclcm_ul_rlc_lc_info[i].lc_id,
             tdsrrclcm_ul_rlc_lc_info[i].rb_id,
             tdsrrclcm_ul_rlc_lc_info[i].lc_type);
    return (tdsrrclcm_ul_rlc_lc_info[i].lc_id);
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Failed to allocate UL rb_id:%d, lc_type:%d, mode:%d",rb_id,ul_lc_type,mode);
    return (TDSRRCLCM_RLC_LC_ID_ERROR);       /* RLC Logical Channel Id allocation
                                              failed. */
  }
}/*  tdsrrclcm_allocate_ul_rlc_lc_id */                                     

/*===========================================================================

FUNCTION tdsrrclcm_allocate_dl_rlc_lc_id

DESCRIPTION
  This function will allocate Downlink RLC Logical Channel Id for the 
  requested RLC service entity. The RRC LLC module uses this function
  to allocate Downlink RLC Logical Channel Id.

DEPENDENCIES
  None

RETURN VALUE
  The allocated RLC Logical Channel Id. If failed, it'll return a value 
  TDSRRCLCM_RLC_LC_ID_ERROR.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_allocate_dl_rlc_lc_id
(    
  uecomdef_logchan_e_type dl_lc_type,      /* Downlink Logical Channel
                                              Type. */
  tdsrrc_RB_Identity rb_id,                   /* Radio Bearer Id assoicated with
                                              the above Downlink Logical 
                                              Channel Type. */
  uecomdef_logch_mode_e_type mode,         /* Mode of transmission for the RLC 
                                              service entity. */
  dsm_watermark_type **dsm_rx_queue         /* Pointer to the DSM Water Mark 
                                              associated with Downlink RLC 
                                              Logical Channel Id. For Radio
                                              Bearers other than 0,1,2,3,4,32
                                              33 and 34,this will be set to 
                                              NULL pointer. */
)
{
  int i=0;
  boolean dl_lc_id_allocated = FALSE;

  switch (mode) 
  {
  /* Checks for Logical Channel Type and Radio Bearer Id match for Transparent
     mode. */
    case (UE_MODE_TRANSPARENT): 
      for( i = TDSRRCLCM_DL_TM_START_IDX ; i < UE_MAX_DL_TM_CHANNEL ; i++ )
      {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
        if(dl_lc_type == UE_LOGCHAN_TM_DCCH_DL && 
          tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_NOT_IN_USE)
        {
          if( tdsrrclcm_dl_rlc_lc_info[i].rb_id == rb_id ||
              tdsrrclcm_dl_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
          {
            if(tdsrrclcm_dl_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
            {
              tdsrrclcm_dl_rlc_lc_info[i].rb_id = rb_id;
            }
            
            tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_TM_DCCH_DL;

            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Allocating LC Id for SRB#5");

            /* Initially would be NULL;
               Is updated right before registering DL queue
               with RLC */
            *dsm_rx_queue = tdsrrclcm_dl_rlc_lc_info[i].rx_queue;
                                             /* Downlink DSM Watermark queue is 
                                                assigned */
            tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_IN_USE;
            dl_lc_id_allocated = TRUE;       /* RLC Logical Channel Id is 
                                                allocated. */
            break;                           /* Comes out of for loop */
          }
        }
        else
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
        if( dl_lc_type == tdsrrclcm_dl_rlc_lc_info[i].lc_type && 
            tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_NOT_IN_USE)
        {
          
          if( tdsrrclcm_dl_rlc_lc_info[i].rb_id == rb_id ||
              tdsrrclcm_dl_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
          {
            if(tdsrrclcm_dl_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
            {
              tdsrrclcm_dl_rlc_lc_info[i].rb_id = rb_id;
            }
            *dsm_rx_queue = tdsrrclcm_dl_rlc_lc_info[i].rx_queue;
                                             /* Downlink DSM Watermark queue is 
                                                assigned */
            tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_IN_USE;
            dl_lc_id_allocated = TRUE;       /* RLC Logical Channel Id is 
                                                allocated. */
            break;                           /* Comes out of for loop */
          }
        }
      }
      break;
  /* Checks for Logical Channel Type and Radio Bearer Id match for 
     Unacknowledged mode. */
    case (UE_MODE_UNACKNOWLEDGED): 
      for( i = TDSRRCLCM_DL_UM_START_IDX ; i < (TDSRRCLCM_DL_UM_START_IDX +
               TDSUE_MAX_DL_UM_CHANNEL) ; i++ )
      {
        if( dl_lc_type == tdsrrclcm_dl_rlc_lc_info[i].lc_type &&
            tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_NOT_IN_USE)
        {
          if( tdsrrclcm_dl_rlc_lc_info[i].rb_id == rb_id ||
              tdsrrclcm_dl_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
          {
            if(tdsrrclcm_dl_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
            {
              tdsrrclcm_dl_rlc_lc_info[i].rb_id = rb_id;
            }
            *dsm_rx_queue = tdsrrclcm_dl_rlc_lc_info[i].rx_queue;
                                             /* Downlink DSM Watermark queue is 
                                                assigned */
            tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_IN_USE;
            dl_lc_id_allocated = TRUE;       /* RLC Logical Channel Id is 
                                                allocated. */
            break;                           /* comes out of for loop */
          }
        }
      }
      break;
 /* Checks for Logical Channel Type and Radio Bearer Id match for 
    Acknowledged Data mode. */  
    case (UE_MODE_ACKNOWLEDGED_DATA): 
      for( i = (TDSRRCLCM_DL_AM_DATA_START_IDX); 
           i < (TDSRRCLCM_DL_AM_DATA_START_IDX + TDSUE_MAX_DL_AM_DATA_CHANNEL) ;
           i++ )
      {
        if( dl_lc_type == tdsrrclcm_dl_rlc_lc_info[i].lc_type &&
            tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_NOT_IN_USE)
        {
          
          if( tdsrrclcm_dl_rlc_lc_info[i].rb_id == rb_id ||
              tdsrrclcm_dl_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
          {
            if(tdsrrclcm_dl_rlc_lc_info[i].rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED)
            {
              tdsrrclcm_dl_rlc_lc_info[i].rb_id = rb_id;
            }
            *dsm_rx_queue = tdsrrclcm_dl_rlc_lc_info[i].rx_queue;
                                             /* Downlink DSM Watermark queue is 
                                                assigned */
            tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_IN_USE;
            dl_lc_id_allocated = TRUE;       /* RLC Logical Channel Id is 
                                                allocated. */
            break;                           /* Comes out of for loop */
          }
        }
      }
      break;

	  
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"This should never happen !!");
      break;
  }
  if(dl_lc_id_allocated)
  {
    TDSRRC_MSG3(MSG_LEGACY_MED,"Allocated DL lc_id: %d, rb_id: %d, lc_type:%d",
             tdsrrclcm_dl_rlc_lc_info[i].lc_id,
             tdsrrclcm_dl_rlc_lc_info[i].rb_id,
             tdsrrclcm_dl_rlc_lc_info[i].lc_type);
    return (tdsrrclcm_dl_rlc_lc_info[i].lc_id);
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Failed to allocate DL rb_id:%d, lc_type:%d, mode:%d", rb_id, dl_lc_type, mode);
    return (TDSRRCLCM_RLC_LC_ID_ERROR);       /* RLC Logical Channel Id allocation
                                              failed. */
  }
} /* tdsrrclcm_allocate_dl_rlc_lc_id */

/*===========================================================================

FUNCTION tdsrrclcm_check_ul_rlc_lc_id

DESCRIPTION
  This function will check whether Uplink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. Any RRC procedure can use this
  function to check whether a particular Logical channel entity is already 
  setup or not. If this function returns TDSRRCLCM_RLC_LC_ID_NOT_FOUND, then the
  RRC procedure will request RRC LLC module to setup Logical Channel entity
  and the corresponding physical channel for transmitting SDU on the uplink.

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If it is not alreday allocated,
   it'll return TDSRRCLCM_RLC_LC_ID_NOT_FOUND.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_check_ul_rlc_lc_id
(    
  uecomdef_logchan_e_type ul_lc_type,       /* Uplink Logical Channel Type. */
  tdsrrc_RB_Identity rb_id,                    /* Radio Bearer Id assoicated with
                                              the above Downlink Logical 
                                              Channel type. */
  uecomdef_logch_mode_e_type mode           /* Mode of transmission for the RLC 
                                              service entity. */
 
)
{
  int i=0;
  boolean ul_lc_id_found = FALSE;

  switch (mode) 
  {
  /* Checks for Logical Channel Type and Radio Bearer Id match for Transparent
     mode. */
    case (UE_MODE_TRANSPARENT): 
      for( i = TDSRRCLCM_UL_TM_START_IDX ; i < UE_MAX_UL_TM_CHANNEL ; i++ )
      {
        if( ul_lc_type == tdsrrclcm_ul_rlc_lc_info[i].lc_type && 
             rb_id == tdsrrclcm_ul_rlc_lc_info[i].rb_id &&
             tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
        {
          ul_lc_id_found = TRUE;           /* Match for the requested service 
                                              is found */
          break;
        }
      }
      break;
  /* Checks for Logical Channel Type and Radio Bearer Id match for 
     Unacknowledged mode. */
    case (UE_MODE_UNACKNOWLEDGED): 
      for( i = TDSRRCLCM_UL_UM_START_IDX ; i < (TDSRRCLCM_UL_UM_START_IDX +
               TDSUE_MAX_UL_UM_CHANNEL) ; i++ )
      {
        if( ul_lc_type == tdsrrclcm_ul_rlc_lc_info[i].lc_type && 
             rb_id == tdsrrclcm_ul_rlc_lc_info[i].rb_id &&
             tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
        {
          ul_lc_id_found = TRUE;           /* Match for the requested service 
                                              is found */
          break;
        }
      }
      break;  
  /* Checks for Logical Channel Type and Radio Bearer Id match for 
     Acknowledged Data mode. */
    case (UE_MODE_ACKNOWLEDGED_DATA): 
      for( i = (TDSRRCLCM_UL_AM_DATA_START_IDX); 
           i < (TDSRRCLCM_UL_AM_DATA_START_IDX + TDSUE_MAX_UL_AM_DATA_CHANNEL) ;
           i++ )
      {
        if( ul_lc_type == tdsrrclcm_ul_rlc_lc_info[i].lc_type && 
             rb_id == tdsrrclcm_ul_rlc_lc_info[i].rb_id &&
             tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
        {
          ul_lc_id_found = TRUE;           /* Match for the requested service 
                                              is found */
          break;
        }
      }
      break;              
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"This should never happen !!");
      break;
  }
  if(ul_lc_id_found)
  {
    return (tdsrrclcm_ul_rlc_lc_info[i].lc_id);
  }
  else
  {
    return (TDSRRCLCM_RLC_LC_ID_NOT_FOUND);   /* Match for the requested service 
                                              is not found. The RRC LLC needs 
                                              to request for allocation. */
  }
} /* tdsrrclcm_check_ul_rlc_lc_id */

/*===========================================================================

FUNCTION tdsrrclcm_get_ul_rlc_lc_id

DESCRIPTION
  This function will check whether Uplink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. Any RRC procedure can use this
  function to check whether a particular Logical channel entity is already 
  setup or not.

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If it is not alreday allocated,
   it'll return TDSRRCLCM_RLC_LC_ID_NOT_FOUND.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_get_ul_rlc_lc_id
(    
  tdsrrc_RB_Identity rb_id                     /* Radio Bearer Id assoicated with
                                               the above Downlink Logical 
                                               Channel type. */ 
)
{
  int i=0;
  
  /* Checks for Logical Channel Type and Radio Bearer Id match for all modes. */
  for( i = 0 ; i < (UE_MAX_UL_TM_CHANNEL + TDSUE_MAX_UL_UM_CHANNEL +
       TDSUE_MAX_UL_AM_DATA_CHANNEL); i++ )
  {
    if( rb_id == tdsrrclcm_ul_rlc_lc_info[i].rb_id &&
        tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
    {
      /* Return the lc id */
      return (tdsrrclcm_ul_rlc_lc_info[i].lc_id);
    }
  }

  /* If we have reached this point, we have not found the LC. Return Not Found */
  return (TDSRRCLCM_RLC_LC_ID_NOT_FOUND);   

} /* tdsrrclcm_get_ul_rlc_lc_id */

/*===========================================================================

FUNCTION tdsrrclcm_get_dl_rlc_lc_id

DESCRIPTION
  This function will check whether Downlink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. RRC LLC can use this
  function to check whether a particular Logical channel entity is already 
  setup or not. 

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If it is not alreday allocated,
   it'll return TDSRRCLCM_RLC_LC_ID_NOT_FOUND.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_get_dl_rlc_lc_id
(    
  tdsrrc_RB_Identity rb_id                     /* Radio Bearer Id assoicated with
                                               the above Downlink Logical 
                                               Channel type. */
 
)
{
  int i=0;

  /* Checks for Logical Channel Type and Radio Bearer Id match for all modes. */
  for( i = 0 ; i < (UE_MAX_DL_TM_CHANNEL + TDSUE_MAX_DL_UM_CHANNEL +
       TDSUE_MAX_DL_AM_DATA_CHANNEL); i++ )
  {
    if( rb_id == tdsrrclcm_dl_rlc_lc_info[i].rb_id &&
        tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
    {
      /* Return the lc id */
      return (tdsrrclcm_dl_rlc_lc_info[i].lc_id);  
    }
  }

  /* If we have reached this point, we have not found the LC. Return Not Found */
  return (TDSRRCLCM_RLC_LC_ID_NOT_FOUND);   

} /* tdsrrclcm_get_dl_rlc_lc_id */
/*===========================================================================

FUNCTION tdsrrclcm_deallocate_ul_rlc_lc_id

DESCRIPTION
  This function will deallocate Uplink RLC Logical Channel Id requested. It also 
  flushes out DSM items in the watermark queue before deallocating RLC Logical
  Channel Id.

DEPENDENCIES
  None

RETURN VALUE
   TRUE if deallocation is successful. Otherwise it returns FALSE.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_deallocate_ul_rlc_lc_id
(    
  rlc_lc_id_type ul_lc_id               /* Uplink  RLC Logical Channel ID */
)
{

  int i=0;
  dsm_item_type *dsm_item;                  /* Local variable for DSM Item */
  dsm_watermark_type *ul_wm_ptr;            /* uplink Water Mark pointer */

  tdsrrclcm_status_e_type rlc_lc_id_deallocated = TDSRRCLCM_FAILURE;

  for( i = 0; i < TDSUE_MAX_UL_LOGICAL_CHANNEL ; i++ )
  {
  /* Checks for RLC Logical Channel Id match for Uplink RLC Logiacal Channel 
     entities. */
    if( ul_lc_id == tdsrrclcm_ul_rlc_lc_info[i].lc_id &&
        tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
    { 
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Deallocating UL lc_id: %d, rb_id: %d, lc_type:%d", 
               tdsrrclcm_ul_rlc_lc_info[i].lc_id,
               tdsrrclcm_ul_rlc_lc_info[i].rb_id,
               tdsrrclcm_ul_rlc_lc_info[i].lc_type);
      ul_wm_ptr = tdsrrclcm_get_ul_watermark_ptr(ul_lc_id);
      if(ul_wm_ptr != NULL)
      {
        /* Flush out DSM items in the watermark queue */
        while((dsm_item = dsm_dequeue(ul_wm_ptr)) != NULL)
        {
          /* Free all packets in the chain */
          dsm_free_packet(&dsm_item);
        }
      }
      switch(tdsrrclcm_get_ul_rb_id(ul_lc_id))
      {
        case CCCH_RADIO_BEARER_ID:
        case DCCH_UM_RADIO_BEARER_ID:
        case DCCH_AM_RADIO_BEARER_ID:
        case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
        case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
          tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
          rlc_lc_id_deallocated = TDSRRCLCM_SUCCESS;           
          break;                            /* Match found and RLC Logical 
                                               Channel id is deallocated. */
        default:
          tdsrrclcm_ul_rlc_lc_info[i].rb_id = TDSRRCLCM_RB_ID_NOT_ALLOCATED;
          tdsrrclcm_ul_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
          tdsrrclcm_ul_rlc_lc_info[i].lc_type = UE_LOGCHAN_DTCH;
          rlc_lc_id_deallocated = TDSRRCLCM_SUCCESS;
          break;
       }
    }
  }
  if (rlc_lc_id_deallocated == TDSRRCLCM_FAILURE)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failed to deallocated lc_id_type:%d ",ul_lc_id);
  }
  return(rlc_lc_id_deallocated);

} /* tdsrrclcm_deallocate_ul_rlc_lc_id */

/*===========================================================================

FUNCTION tdsrrclcm_deallocate_dl_rlc_lc_id

DESCRIPTION
  This function will deallocate Downlink RLC Logical Channel Id requested. It 
  also flushes out DSM items in the watermark queue before deallocating RLC 
  Logical Channel Id.

DEPENDENCIES
  None

RETURN VALUE
   TRUE if deallocation is successful. Otherwise it returns FALSE.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_deallocate_dl_rlc_lc_id
(    
  rlc_lc_id_type dl_lc_id               /* Downlink  RLC Logical Channel ID */
)
{
  int i=0;
  dsm_item_type *dsm_item;                  /* Local variable for DSM Item */
  dsm_watermark_type *dl_wm_ptr;            /* Downlink Water Mark pointer */

  tdsrrclcm_status_e_type rlc_lc_id_deallocated = TDSRRCLCM_FAILURE;

  for( i = 0; i < TDSUE_MAX_DL_LOGICAL_CHANNEL ; i++ )
  {
  /* Checks for RLC Logical Channel Id match for Downlink RLC Logiacal Channel 
     entities. */
    if(dl_lc_id == tdsrrclcm_dl_rlc_lc_info[i].lc_id &&
        tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Deallocating DL lc_id: %d, rb_id: %d, lc_type: %d", 
               tdsrrclcm_dl_rlc_lc_info[i].lc_id,
               tdsrrclcm_dl_rlc_lc_info[i].rb_id,
               tdsrrclcm_dl_rlc_lc_info[i].lc_type);
      dl_wm_ptr = tdsrrclcm_get_dl_watermark_ptr(dl_lc_id);
      if(dl_wm_ptr != NULL)
      {
        /* Flush out DSM items in the watermark queue */
        while((dsm_item = dsm_dequeue(dl_wm_ptr)) != NULL)
        {
          /* Free all packets in the chain */
          dsm_free_packet(&dsm_item);
        }
      }
      switch(tdsrrclcm_get_dl_rb_id(dl_lc_id))
      {
        case CCCH_RADIO_BEARER_ID:
        case DCCH_UM_RADIO_BEARER_ID:
        case DCCH_AM_RADIO_BEARER_ID:
        case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
        case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
        case BCCH_S_RADIO_BEARER_ID:
        case BCCH_N_RADIO_BEARER_ID:
        case BCCH_FACH_RADIO_BEARER_ID:
        case PCCH_RADIO_BEARER_ID:
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
        case CTCH_RADIO_BEARER_ID:
#endif
          tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
          rlc_lc_id_deallocated = TDSRRCLCM_SUCCESS;
                                            /* Match found and RLC Logical 
                                               Channel id is deallocated. */


          break;

        default:
          tdsrrclcm_dl_rlc_lc_info[i].rb_id = TDSRRCLCM_RB_ID_NOT_ALLOCATED;
          tdsrrclcm_dl_rlc_lc_info[i].status = TDSRRCLCM_LC_NOT_IN_USE;
          tdsrrclcm_dl_rlc_lc_info[i].lc_type = UE_LOGCHAN_DTCH;
          rlc_lc_id_deallocated = TDSRRCLCM_SUCCESS;
          break;
      }    
    }
  }
  if (rlc_lc_id_deallocated == TDSRRCLCM_FAILURE)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failed to deallocated lc_id_type:%d ",dl_lc_id);
  }
  return (rlc_lc_id_deallocated); 
} /* tdsrrclcm_deallocate_dl_rlc_lc_id */

/*===========================================================================

FUNCTION tdsrrclcm_get_dl_lc_type

DESCRIPTION
  This function will identify the Logical Channel type to which the OTA message
  belongs. The RRC procedure uses this infomation for processing the received 
  message.

DEPENDENCIES
  None

RETURN VALUE
   TDSRRCLCM_SUCCESS if match was found. Otherwise returns TDSRRCLCM_FAILURE.


SIDE EFFECTS
  This function should normally never return TDSRRCLCM_FAILURE. 
  If it returns TDSRRCLCM_FAILURE, then there is a race condition between Logical
  Channel Manager and RRC LLC. 
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_dl_lc_type
(    
  rlc_lc_id_type dl_lc_id,                    /* The Downlink RLC Logical
                                                 Channel Id. */
  uecomdef_logchan_e_type *dl_lc_type_ptr     /* Pointer to the Logical Channel
                                                 type assocated with the above 
                                                 Id. */
)

{
  int i=0;
  tdsrrclcm_status_e_type match_found = TDSRRCLCM_FAILURE;

  for( i = 0; i < TDSUE_MAX_DL_LOGICAL_CHANNEL ; i++ )
  {
  /* Checks for mapping between RLC Logical Channel id and RLC Logical Channel
     Type. */
    if(dl_lc_id == tdsrrclcm_dl_rlc_lc_info[i].lc_id )
    {
      *dl_lc_type_ptr = tdsrrclcm_dl_rlc_lc_info[i].lc_type;
      match_found = TDSRRCLCM_SUCCESS;       /* The corresponding RLC logical
                                              channel type is identified. */
      break;                                        
    }
  }
  return(match_found);
} /* tdsrrclcm_get_dl_lc_type */
/*===========================================================================

FUNCTION tdsrrclcm_get_ul_lc_type

DESCRIPTION
  This function will identify the Logical Channel to which the OTA message
  belongs. The RRC procedure uses this infomation for sending the OTA 
  message.

DEPENDENCIES
  None

RETURN VALUE
   TDSRRCLCM_SUCCESS if match was found. Otherwise returns TDSRRCLCM_FAILURE.

SIDE EFFECTS
  This function should normally never return TDSRRCLCM_FAILURE. 
  If it returns TDSRRCLCM_FAILURE, then there is a race condition between Logical
  Channel Manager and RRC LLC. 
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_ul_lc_type
(    
  rlc_lc_id_type ul_lc_id,                    /* The uplink RLC Logical
                                                 Channel Id. */
  uecomdef_logchan_e_type *ul_lc_type_ptr     /* Pointer to the Logical Channel
                                                 type assocated with the above 
                                                 Id. */
)
{
  int i=0;
  tdsrrclcm_status_e_type match_found = TDSRRCLCM_FAILURE;

  for( i = 0; i < TDSUE_MAX_UL_LOGICAL_CHANNEL ; i++ )
  {
  /* Checks for mapping between RLC Logical Channel id and RLC Logical Channel
     Type. */
    if(ul_lc_id == tdsrrclcm_ul_rlc_lc_info[i].lc_id )
    {
      *ul_lc_type_ptr = tdsrrclcm_ul_rlc_lc_info[i].lc_type;
      match_found = TDSRRCLCM_SUCCESS;       /* The corresponding RLC logical
                                              channel type is identified. */
      break;
                                              
    }
  }
  return(match_found);
} /* tdsrrclcm_get_ul_lc_type */   
/*===========================================================================

FUNCTION tdsrrclcm_identify_proc

DESCRIPTION
  This function will processes the event from the Dispatcher and identifies 
  the procedure to which the L2 ACK belongs.

DEPENDENCIES
  None

RETURN VALUE
  TDSRRCLCM_SUCCESS if there is a match for MUI received. Other it returns
  TDSRRCLCM_FAILURE

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_identify_proc
(    
  tdsrrc_cmd_type *cmd_ptr,                    /* RRC Command Type */
  tdsrrc_RB_Identity rb_id,                    /* RB Id for which MUI need to
                                               be allocated. */  
  tdsrrc_proc_e_type *proc_name                /* RRC procedure    */
)
{
  uint8 i=0;
  tdsrrclcm_status_e_type match_found = TDSRRCLCM_FAILURE;
  uint8 rb_id_idx = 0;                           /* Local variable for RB Id 
                                              Index. */
  rb_id_idx = rb_id - 2;

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_L2ACK_IND)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rcvd MUI: %d for RB id: %d status %d [0:S, 1:F]",cmd_ptr->cmd.downlink_l2ack_ind.mui,
             rb_id, cmd_ptr->cmd.downlink_l2ack_ind.status);
 
    for(i = 0; i <  (uint8)TDSMAX_NUM_OF_PROCEDURES ; i++)
    {
      if(rb_id_idx < (uint8)(TDSUE_MAX_UL_AM_DATA_CHANNEL - 1))
      {    
        /* Searches for MUI match in the Look Up table*/
        if(tdsrrclcm_l2_ack_info[i][rb_id_idx].mui == 
          cmd_ptr->cmd.downlink_l2ack_ind.mui)
        {
          *proc_name = tdsrrclcm_l2_ack_info[i][rb_id_idx].rrc_proc;
          tdsrrclcm_l2_ack_info[i][rb_id_idx].rrc_proc = TDSRRC_PROCEDURE_NONE;
                                            /* Deallocates MUI in the LUT */
          match_found = TDSRRCLCM_SUCCESS;
          break;                              /* exit from for loop */
        }    
      }
    }
  }                                      
  else                                  /* Invalid Command Identifier*/
  {
    *proc_name = TDSRRC_PROCEDURE_NONE; 
     TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Command Id : %d",cmd_ptr->cmd_hdr.cmd_id);     
  }
  return (match_found);
} /* tdsrrclcm_identify_proc */

/*===========================================================================

FUNCTION tdsrrclcm_get_mui

DESCRIPTION
  This function builds the MUI for the RRC procedure and RRC procedure can 
  include the same in the SDU if it is transmitting SDU in Acknowledged mode.

DEPENDENCIES
  None

RETURN VALUE
  TDSRRCLCM_SUCCESS if allocation of MUI is successful. Other it returns
  TDSRRCLCM_FAILURE

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_mui
(    
  tdsrrc_proc_e_type rrc_proc,                /* The RRC Procedure type for 
                                              which MUI required. */
  tdsrrc_RB_Identity rb_id,                   /* RB Id for which MUI need to
                                              be allocated. */ 
  boolean l2ack_status,                    /* Indicates whether RRC needs
                                               L2 ACK or not */
  tdsrrclcm_mui_type *mui                     /* The MUI allocated for the RRC
                                              procedure. */ 
)
{
  static tdsrrclcm_mui_type rrclcm_current_mui[TDSUE_MAX_UL_AM_DATA_CHANNEL - 1];
  uint8 i=0;                                   /* Local varible for count */

  uint8 rb_id_idx = 0;                           /* Local variable for RB Id 
                                              Index. */
  tdsrrclcm_status_e_type temp_status = TDSRRCLCM_FAILURE;

  rb_id_idx = rb_id - 2;
  
  // Initialize only once at powerup.
  if(tdsrrclcm_init_current_mui_array_at_poweup == TRUE)
  { 
    for(i=0; i < TDSUE_MAX_UL_AM_DATA_CHANNEL-1; i++)
    {
      rrclcm_current_mui[i] = 0;
    }
    tdsrrclcm_init_current_mui_array_at_poweup = FALSE;
  }

  if(rb_id_idx < (TDSUE_MAX_UL_AM_DATA_CHANNEL - 1))
  {
    rrclcm_current_mui[rb_id_idx] = rrclcm_current_mui[rb_id_idx] + 1; 
                                          /* Increment the MUI */

    if(l2ack_status == TRUE)
    {
      for(i = 0; i <  (uint8)TDSMAX_NUM_OF_PROCEDURES ; i++)
      {
        /* Checks for the available index that is not allocated for any
           procedure */
        if(tdsrrclcm_l2_ack_info[i][rb_id_idx].rrc_proc == TDSRRC_PROCEDURE_NONE)
        {
          tdsrrclcm_l2_ack_info[i][rb_id_idx].mui = rrclcm_current_mui[rb_id_idx]; 
    
          *mui = rrclcm_current_mui[rb_id_idx];    /* Assigns the MUI to procedure.*/
    
          tdsrrclcm_l2_ack_info[i][rb_id_idx].rrc_proc = rrc_proc;
                                              /* Updates the LUT with the Procedure
                                                 type. */
          temp_status = TDSRRCLCM_SUCCESS;
          break;                              /* exit from for loop */
        }
      }
    }
    else
    {
      *mui = rrclcm_current_mui[rb_id_idx];    /* Assigns the MUI to procedure.*/
      temp_status = TDSRRCLCM_SUCCESS;
    }
    if ((rrc_proc != TDSRRC_PROCEDURE_MCMR) && (rrc_proc != TDSRRC_PROCEDURE_NONE) && 
        ((rb_id == DCCH_AM_RADIO_BEARER_ID) || (rb_id == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID)))
    {
      tdsrrc_set_l2_ack_req_mask(rrc_proc, *mui);
    }
  }
  return (temp_status);  
} /* tdsrrclcm_get_mui */

/*===========================================================================

FUNCTION tdsrrclcm_init_l2_ack_info

DESCRIPTION
  This function will initailize the L2 ACK info of all RRC procedures.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_init_l2_ack_info
(    
  void                  
)
{
  uint8 i,j;
  for(j = 0; j < (TDSUE_MAX_UL_AM_DATA_CHANNEL - 1); j++)
  {
    for(i = 0; i <  (uint8)TDSMAX_NUM_OF_PROCEDURES ; i++)
    {

      tdsrrclcm_l2_ack_info[i][j].rrc_proc = TDSRRC_PROCEDURE_NONE;
                                             /* Sets all RRC procedures to 
                                                NONE. */
      tdsrrclcm_l2_ack_info[i][j].mui = 0;      /* Set the Initial MUIs to 0 */    
    }
  }
} /* tdsrrclcm_init_l2_ack_info */

/*===========================================================================

 FUNCTION tdsrrclcm_post_event_for_dl_sdu

DESCRIPTION
  This function will post an internal event to RRC for the downlink SDU.

DEPENDENCIES
  None

RETURN VALUE
   None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrclcm_post_event_for_dl_sdu
(    
  rlc_lc_id_type dl_lc_id,                    /* The Downlink RLC Logical
                                                 Channel Id. */ 
  uint8 no_of_sdus                            /* No of SDUs that are
                                                 queued */                   
 , void *context
)
{
  uint32 dl_sdu_type;                          /* To store SDU type received*/

  tdsrrc_cmd_type *cmd_ptr;                      /* Pointer to RRC Command */

  tdsrrc_RB_Identity dl_rb_id;                   /* To store the channel on which
                                                 message was received. */
  uint8 count;                                /* Count to keep track of SDUs */

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrclcm_status_e_type    lcm_status = TDSRRCLCM_FAILURE;
  uecomdef_logchan_e_type dl_spl_sdu_type = UE_LOGCHAN_NONE;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

  count = no_of_sdus;

  if(context != NULL)
  {
    TDSRRC_MSG1(MSG_LEGACY_LOW,"0x%x",context);
  }

  while( count > 0)
  {
    /* Reset the command pointer to NULL */
    cmd_ptr = NULL;

    /* Get a command buffer from RRC task for internal command. */
    cmd_ptr = tdsrrc_get_int_cmd_buf();
    if(cmd_ptr != NULL)
    {
      cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DOWNLINK_SDU_IND;
                                              /* Fill in the Command Id */
      cmd_ptr->cmd.downlink_sdu_ind.lc_id= dl_lc_id; 
                                             /* Updates the RLC Loagical Channel
                                                Id. */
      cmd_ptr->cmd.downlink_sdu_ind.dl_sdu = NULL; 
      cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length = 0;
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT 
      lcm_status = tdsrrclcm_get_dl_lc_type(dl_lc_id, &dl_spl_sdu_type);

      /* Check if the logical channel ID passed by RLC exists.
         Also see if this is a special DCCH logical channel
         type. If both conditions are met, declare it to
         be of type "tdsrrc_DL_DCCH_Message_PDU"

         This condition is added because contrary to SRBs 0 to 4,
         there is no fixed relationship between SRB#5 RB Id & its logical channel ID 
      */
      if(lcm_status == TDSRRCLCM_SUCCESS &&
         dl_spl_sdu_type == UE_LOGCHAN_TM_DCCH_DL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_DOWNLINK_SDU_IND on DCCH for SRB#5");
        dl_sdu_type = TDSRRCLCM_DL_DCCH_SRB5_SDU_TYPE;
      }
      else      
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
      {
        /* Get SDU type of the OTA message */
        dl_rb_id = tdsrrclcm_get_dl_rb_id(dl_lc_id);
        switch(dl_rb_id)
        {
          case BCCH_S_RADIO_BEARER_ID: 
          case BCCH_N_RADIO_BEARER_ID:        /* Broadcast Control Channel.*/
       
            dl_sdu_type = tdsrrc_BCCH_BCH_Message_PDU;
            break;

          case BCCH_FACH_RADIO_BEARER_ID:    /* Broadcast Control Channel.*/

            dl_sdu_type = tdsrrc_BCCH_FACH_Message_PDU;
            break;

          case CCCH_RADIO_BEARER_ID:         /* Common Control Channel    */
            TDSRRC_MSG1(MSG_LEGACY_LOW,"TDSRRC_DOWNLINK_SDU_IND on CCCH RB ID %d",dl_rb_id);
            dl_sdu_type = tdsrrc_DL_CCCH_Message_PDU;
            break;

          case DCCH_UM_RADIO_BEARER_ID:       /* Dedicated Control Channel */
          case DCCH_AM_RADIO_BEARER_ID:
          case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
          case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
            TDSRRC_MSG1(MSG_LEGACY_LOW,"TDSRRC_DOWNLINK_SDU_IND on DCCH RB ID %d",dl_rb_id);
            dl_sdu_type = tdsrrc_DL_DCCH_Message_PDU;
            break;

          case PCCH_RADIO_BEARER_ID:         /* Paging Control Channel    */
            TDSRRC_MSG0(MSG_LEGACY_LOW,"TDSRRC_DOWNLINK_SDU_IND on PCCH");
            dl_sdu_type = tdsrrc_PCCH_Message_PDU;
            break;


          default:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"PDU from unknown RB Id: %d",dl_rb_id);
            dl_sdu_type = TDSRRCLCM_INVALID_SDU_TYPE;
            break;

        }/*end switch*/
      } 

      cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type = dl_sdu_type; 
                                     
      cmd_ptr->cmd.downlink_sdu_ind.decoded_msg = (void *) NULL;

      tdsrrc_put_int_cmd(cmd_ptr);

    }
    else                                     /* No free command buffers
                                                available */
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC Unable to allocate CMD buffer: %d",TDSRRC_DOWNLINK_SDU_IND);
    }
    /* Decrement the count */
    count--;
  }
} /* tdsrrclcm_post_event_for_dl_sdu */

/*===========================================================================

 FUNCTION tdsrrclcm_post_event_for_dl_l2ack_ind

DESCRIPTION
  This function will post an internal event to RRC for the downlink L2 ACK 
  confirm indication.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_post_event_for_dl_l2ack_ind
(    
  rlc_lc_id_type dl_lc_id,                      /* The Downlink RLC Logical
                                                   Channel Id. */  
  uecomdef_status_e_type  status,               /* Indicates the status of 
                                                   success or failure of SDU
                                                   transmission */
  uint8 mui,                                    /* Message Unit Id */
  rlc_sdu_tx_fail_reason_e_type failure_reason  /* Failure reason */
)
{
  tdsrrc_cmd_type *cmd_ptr;
  
  if(tdsrrcllc_curr_cmd_rel_all_req() == TRUE)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"No need of posting a cmd as it is CRLC REL ALL REQ mui %d, status %d lc id %d",
                   mui,status,dl_lc_id);
    return;
  }

  /* Get a command buffer from RRC task for internal command. */
  cmd_ptr = tdsrrc_get_int_cmd_buf();

  if(cmd_ptr != NULL)
  {
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DOWNLINK_L2ACK_IND;
                                           /* Fill in the Command Id */
    cmd_ptr->cmd.downlink_l2ack_ind.lc_id = dl_lc_id;     
                                           /* Updates the RLC Loagical Channel
                                              Id. */
    cmd_ptr->cmd.downlink_l2ack_ind.status = status;
                                           /* Updates the Success or Failure
                                              status SDU transmission. */
    cmd_ptr->cmd.downlink_l2ack_ind.mui = mui; 
                                           /* Updates the MUI */

    cmd_ptr->cmd.downlink_l2ack_ind.failure_reason = failure_reason;
                                           /* Failure reason */
  
    TDSRRC_MSG1(MSG_LEGACY_LOW,"TDSRRC_DOWNLINK_L2ACK_IND command is posted thru callback.Failure reason %d",failure_reason);

    tdsrrc_put_int_cmd(cmd_ptr);

  }
  else                                     /* No free command buffers
                                              available */
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC Unable to allocate CMD buffer: %d",TDSRRC_DOWNLINK_L2ACK_IND);
  }
  
} /* tdsrrclcm_post_event_for_dl_l2ack_ind */
/*===========================================================================

 FUNCTION rrclcm_get_watermark_ptr

DESCRIPTION
  This function will get watermark pointer corresponding to the RLC Logical
  channel Id.

DEPENDENCIES
  None

RETURN VALUE
   Pointer the DSM items' queue.

SIDE EFFECTS
  None
===========================================================================*/

dsm_watermark_type *tdsrrclcm_get_dl_watermark_ptr
(    
  rlc_lc_id_type dl_lc_id                  /* The Downlink RLC Logical
                                              Channel Id. */    
)
{
  return tdsrrclcm_dl_rlc_lc_info[dl_lc_id].rx_queue ;
} /* rrclcm_get_watermark_ptr */
/*===========================================================================

 FUNCTION tdsrrclcm_get_ul_watermark_ptr

DESCRIPTION
  This function will get watermark pointer corresponding to the RLC Logical
  channel Id.

DEPENDENCIES
  None

RETURN VALUE
   Pointer the DSM items' queue.

SIDE EFFECTS
  None
===========================================================================*/

dsm_watermark_type *tdsrrclcm_get_ul_watermark_ptr
(    
  rlc_lc_id_type ul_lc_id                  /* The uplink RLC Logical
                                              Channel Id. */    
)
{
  if(ul_lc_id >= TDSUE_MAX_UL_LOGICAL_CHANNEL)
  {
    ERR_FATAL("ul_lc_id out of bounds : 0x%x",ul_lc_id,0,0);
  }
  return tdsrrclcm_ul_rlc_lc_info[ul_lc_id].tx_queue ;
} /* tdsrrclcm_get_ul_watermark_ptr */
/*===========================================================================

FUNCTION tdsrrclcm_get_dl_rb_id

DESCRIPTION
  This function will get radio bearer Id  corresponding to the RLC Logical
  channel Id.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrc_RB_Identity.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrc_RB_Identity tdsrrclcm_get_dl_rb_id
(    
  rlc_lc_id_type dl_lc_id                  /* The Downlink RLC Logical
                                              Channel Id. */   
)
{
  return tdsrrclcm_dl_rlc_lc_info[dl_lc_id].rb_id ;
} /* rrclcm_get_rb_dl_id */
/*=========================================================================*/

/*===========================================================================

FUNCTION tdsrrclcm_get_ul_rlc_mode

DESCRIPTION
  This function will check whether Uplink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. If allocated then it returns
  assoctaed mode of transmission.

DEPENDENCIES
  None

RETURN VALUE
   Returns TDSRRCLCM_SUCCESS if RLC Logical Channel Id allocated. Otherwise returns
   TDSRRCLCM_FAILURE.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_ul_rlc_mode
(    
  rlc_lc_id_type ul_lc_id,                      /* RLC Logical Channel Id */
  uecomdef_logch_mode_e_type *tx_mode           /* Mode of transmission */
)
{
  tdsrrclcm_status_e_type status = TDSRRCLCM_FAILURE;
                                                /* Initialize status to 
                                                   Failure */
  uint32 i;                                     /* Local counter */

  for( i = 0; i < TDSUE_MAX_UL_LOGICAL_CHANNEL ; i++ )
  {
  /* Checks for RLC Logical Channel Id match for Uplink RLC Logiacal Channel 
     entities. */
    if( ul_lc_id == tdsrrclcm_ul_rlc_lc_info[i].lc_id &&
        tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
    {
      /* When match is found update the mode of transmission from Database */
      *tx_mode = tdsrrclcm_ul_rlc_lc_info[i].mode;
      status = TDSRRCLCM_SUCCESS;
      break;
    }
  }
  return(status);
}

/*===========================================================================

FUNCTION tdsrrclcm_get_dl_rlc_mode

DESCRIPTION
  This function will check whether Downlink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. If allocated then it returns 
  associated mode reception.

DEPENDENCIES
  None

RETURN VALUE
   Returns TDSRRCLCM_SUCCESS if RLC Logical Channel Id allocated. Otherwise returns
   TDSRRCLCM_FAILURE.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_dl_rlc_mode
(    
  rlc_lc_id_type dl_lc_id,                   /* RLC Logical Channel Id */
  uecomdef_logch_mode_e_type *rx_mode        /* Mode of reception */

 
)
{
  tdsrrclcm_status_e_type status = TDSRRCLCM_FAILURE;
                                                /* Initialize status to 
                                                   Failure */
  uint32 i;                                     /* Local counter */

  for( i = 0; i < TDSUE_MAX_DL_LOGICAL_CHANNEL ; i++ )
  {
  /* Checks for RLC Logical Channel Id match for Downlink RLC Logiacal Channel 
     entities. */
    if( dl_lc_id == tdsrrclcm_dl_rlc_lc_info[i].lc_id &&
        tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
    {
      /* When match is found update the mode of reception from Database */
      *rx_mode = tdsrrclcm_dl_rlc_lc_info[i].mode;
      status = TDSRRCLCM_SUCCESS;
      break;
    }
  }
  return(status);
}

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*===========================================================================

FUNCTION tdsrrclcm_update_db_with_srb5_wmq_info

DESCRIPTION
   This function ideally is expected to be called after a logical
   channel ID gets allocated to SRB#5 RB. During LCM initialization,
   watermark pointer is set to NULL. 

DEPENDENCIES
  None

RETURN VALUE
   Returns TRUE SRB#5 watermark is successfully updated.
           FALSE otherwise.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrclcm_update_db_with_srb5_wmq_info(rlc_lc_id_type srb5_lc_id)
{
  boolean wm_ptr_initialized = FALSE;

  if(srb5_lc_id < UE_MAX_DL_TM_CHANNEL &&
     tdsrrclcm_dl_rlc_lc_info[srb5_lc_id].status == TDSRRCLCM_LC_IN_USE &&
     tdsrrclcm_dl_rlc_lc_info[srb5_lc_id].lc_type == UE_LOGCHAN_TM_DCCH_DL)
  {
    tdsrrclcm_dl_rlc_lc_info[srb5_lc_id].rx_queue = &tdsrrclcm_srb5_dcch_dl_wmq;
    wm_ptr_initialized = TRUE;
  }

  return wm_ptr_initialized;
} /* tdsrrclcm_update_db_with_srb5_wmq_info */

/*===========================================================================

FUNCTION tdsrrclcm_get_rb_id_if_dl_lc_type_exists

DESCRIPTION
   This function checks whether Downlink RLC Logical Channel type exists in LCM
   database or not & returns the RB Id if present. See side effect.

DEPENDENCIES
  None

RETURN VALUE
   Returns TDSRRCLCM_SUCCESS if DL RLC Logical Channel type exists.
   Otherwise returns TDSRRCLCM_FAILURE.

SIDE EFFECTS
  Please note that for the given logical channel type, only the first 
  occurence of RB ID is returned.
===========================================================================*/
tdsrrclcm_status_e_type tdsrrclcm_get_rb_id_if_dl_lc_type_exists
(    
  uecomdef_logchan_e_type  lc_type,        /* Mode of reception */
  tdsrrc_RB_Identity         *rb_id           /* Radio bearer ID */
)
{
  tdsrrclcm_status_e_type status = TDSRRCLCM_FAILURE;
                                                /* Initialize status to 
                                                   Failure */
  uint32 i;                                     /* Local counter */

  for( i = 0; i < UE_MAX_DL_TM_CHANNEL ; i++ )
  {
  /* Checks for RLC Logical Channel Id match for Downlink RLC Logical Channel 
     entities. */
    if(tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE &&
       tdsrrclcm_dl_rlc_lc_info[i].lc_type == lc_type)
    {
      *rb_id = tdsrrclcm_dl_rlc_lc_info[i].rb_id;
      status = TDSRRCLCM_SUCCESS;
      break;
    }
  }
  return(status);
} /* tdsrrclcm_get_rb_id_if_dl_lc_type_exists */
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */


/*===========================================================================

FUNCTION tdsrrc_check_for_pending_ul_srb_data

DESCRIPTION
  This function checks whether UE is waiting for ul srb pending data
  for SRB

DEPENDENCIES
  None

RETURN VALUE
   TRUE : If any SRB is waiting for UL L2 srb data
   else returns  FALSE

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrc_check_for_pending_ul_srb_data()
{
  int i=0;
  
  /* Checks for Logical Channel Type and Radio Bearer Id match for AM modes. */
  for( i = ( UE_MAX_UL_TM_CHANNEL + UE_MAX_UL_UM_CHANNEL );
        i < (UE_MAX_UL_TM_CHANNEL + UE_MAX_UL_UM_CHANNEL + UE_MAX_UL_AM_DATA_CHANNEL); i++ )
  {
    if((tdsrrclcm_ul_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE) &&
        ((DCCH_AM_RADIO_BEARER_ID == tdsrrclcm_ul_rlc_lc_info[i].rb_id)||
         (DCCH_DT_HIGH_PRI_RADIO_BEARER_ID == tdsrrclcm_ul_rlc_lc_info[i].rb_id) ||
         (DCCH_DT_LOW_PRI_RADIO_BEARER_ID == tdsrrclcm_ul_rlc_lc_info[i].rb_id)
        )
      )
    {
      if(tdsrlc_check_pending_sdu(tdsrrclcm_ul_rlc_lc_info[i].lc_id) == TRUE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Pending UL srb data on rb %d",tdsrrclcm_ul_rlc_lc_info[i].rb_id);
        return (TRUE);
      }
    }
  }

  /* UE is not waiting for any for L2 ACK for SRB */
  return (FALSE);   

}

/*===========================================================================

FUNCTION tdsrrc_check_for_pending_dl_srb_data

DESCRIPTION
  This function checks whether UE is waiting for dl srb pending data
  for SRB

DEPENDENCIES
  None

RETURN VALUE
   TRUE : If SRB is waiting for dl srb pending data
   else returns  FALSE

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrc_check_for_pending_dl_srb_data(void)
{
  int i=0;
  
  /* Checks for Logical Channel Type and Radio Bearer Id match for AM modes. */
  for( i = ( UE_MAX_DL_TM_CHANNEL + UE_MAX_DL_UM_CHANNEL );
        i < (UE_MAX_DL_TM_CHANNEL + UE_MAX_DL_UM_CHANNEL + UE_MAX_DL_AM_DATA_CHANNEL); i++ )
  {
    if((tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE) &&
        ((DCCH_AM_RADIO_BEARER_ID == tdsrrclcm_dl_rlc_lc_info[i].rb_id)||
         (DCCH_DT_HIGH_PRI_RADIO_BEARER_ID == tdsrrclcm_dl_rlc_lc_info[i].rb_id) ||
         (DCCH_DT_LOW_PRI_RADIO_BEARER_ID == tdsrrclcm_dl_rlc_lc_info[i].rb_id)
        )
      )
    {
      if(tdsrlc_check_pending_dl_sdu(tdsrrclcm_dl_rlc_lc_info[i].lc_id) == TRUE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Pending DL srb data on rb %d",tdsrrclcm_dl_rlc_lc_info[i].rb_id);
        return (TRUE);
      }
    }
  }

  /* UE is not waiting for any for dl srb data */
  return (FALSE);   

}

/*===========================================================================

FUNCTION tdsrrclcm_get_nchan_dl_tm

DESCRIPTION
  This function will return number of DL TM channel that are used

DEPENDENCIES
  None

RETURN VALUE
   Returns number of CL TM channels that are used

SIDE EFFECTS
  None
===========================================================================*/

uint8 tdsrrclcm_get_nchan_dl_tm
(    
  void
)
{
  uint8 nchan_dl_tm =0;
  int i=0;
  for(i=0;i<UE_MAX_DL_TM_CHANNEL;i++)
  {
    if(tdsrrclcm_dl_rlc_lc_info[i].status == TDSRRCLCM_LC_IN_USE)
    {
      nchan_dl_tm++;
    }
  }
  return (nchan_dl_tm);
}
