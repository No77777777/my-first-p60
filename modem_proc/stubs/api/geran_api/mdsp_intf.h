#ifndef MDSP_INTF_H
#define MDSP_INTF_H

/* Burst type enum.  This is used to select which SBI/non-SBI buffer to use.
   Note many types are overlayed to save host interface segment memory,
   thus care must be taken that these burst types are not issued in the same
   frame.  Be careful when changing any of these as the overlaps have been
   carefully chosen to avoid such conflicts. */
typedef enum
{
   RX_BURST             = 0,
   TX_BURST             = 1,
   MON_BURST_1          = 2,
   MON_BURST_2          = 3,
   MON_BURST_3          = 4,
   STOP_ACQ_BURST       = 5,
   MON_BURST_4          = TX_BURST,
   MON_BURST_5          = STOP_ACQ_BURST,
   MON_BURST_6          = RX_BURST,
   START_ACQ_BURST      = MON_BURST_1,
   ASYNC_RX_BURST       = MON_BURST_2,
   MON_BURST_7          = 6,
   CM_MON_BURST_1       = MON_BURST_1,
   CM_MON_BURST_2       = MON_BURST_2,
   CM_MON_BURST_3       = MON_BURST_3,
   CM_MON_BURST_4       = MON_BURST_4,
   CM_MON_BURST_5       = MON_BURST_7,
   CM_MON_BURST_6       = 7,
   CM_MON_BURST_7       = 8,
   CM_MON_BURST_8       = 9,
   CM_MON_BURST_9       = STOP_ACQ_BURST,
   CM_MON_BURST_10      = 10,
   CM_STARTUP_BURST     = RX_BURST,
   CM_CLEANUP_BURST     = 11,
   WCDMA_RF_ON_BURST    = 12,
   WCDMA_RF_OFF_BURST   = 13,
   /* In LCU we had to add a new dummy SBI buffer WCDMA_DPLL_BURST */
   /* to fix a HW  bug such that the  dummy SBI buffer  allows the */
   /* clock to be on for  the  mDSP  accessing  certain registers. */
   WCDMA_DPLL_BURST       = 14,
   CM_STARTUP_BURST2      = 15,
   CM_CLEANUP_BURST2      = 16,
   CM_STARTUP_BURST1      = 17,
   CM_CLEANUP_BURST1      = 18,
   G2X_STARTUP_BURST      = 19,
   G2X_CLEANUP_BURST      = 20,

/*######################################*/
/* WARNING: If you add a burst type you */
/* must update                          */
/* sema check in mdsp_check_semaphores. */
/*######################################*/

/* Define the number of burst types */
/* Defined regardless of FEATURE_GSM_GPRS_QDSP6 (in which case
   MDSP_NUM_BURST_TYPES is already defined) */
   API_MDSP_NUM_BURST_TYPES

} mdsp_burst_type;
#endif /* MDSP_INTF_H */