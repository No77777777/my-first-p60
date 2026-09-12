#ifndef MAC_QSH_H
#define MAC_QSH_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            WMAC QSH EXT . H                 

GENERAL DESCRIPTION
   Contains declarations for function which are QUALCOMM proprietary 
   and may or may not be shipped as source code.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:$
  $Header: 

when       who     what, where, why
--------   ---     ---------------------------------------------------------
06/01/15   scv     Code changes for QSH
06/01/15   scv     Initial version for wmac_qsh_ext.h

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "qsh.h"

#define WMAC_QSH_MAJOR_VER 0
#define WMAC_QSH_MINOR_VER 1

typedef enum
{
  WMAC_QSH_METRIC_R99_STAT_E,
  WMAC_QSH_METRIC_EUL_STAT_E,
  WMAC_QSH_METRIC_MAX_STAT_E
}wmac_qsh_metric_e;

typedef struct
{
  qsh_metric_hdr_s hdr; /* This header has the timestamp (uint16) */

  uint32 mac_r99ul_bo_bytes;
}wmac_qsh_metric_r99_stat_s;

typedef struct
{
  qsh_metric_hdr_s hdr; /* This header has the timestamp (uint16) */
  uint8  num_happy_bit_ind_pri;
  uint8  num_happy_bit_ind_sec;
  uint8  num_si_sent_pri;
  uint8  num_si_sent_sec;
  uint32 mac_eul_bo_bytes;
}wmac_qsh_metric_eul_stat_s;

#define MAX_ASC                         8
#define MAC_MAX_UE_CARRIER              2
#define MAC_RLC_MAX_NON_ZERO_BO_EXP     5
#define MAC_RLC_MAX_NUM_PDU_SIZE        5
#define MAC_RLC_MAX_TSN_VAL_PER_RLC_ID 10
#define MAC_MAX_FLOW_ID_EXP             3
#define MAC_MAX_DDI                     3
#define MAC_NUM_RLC_ID_PER_FLOW         5
#define MAC_UE_MAX_TB                   5
#define MAC_QSH_DL_HS_MAX_NUM_QUEUE     3
#define MAC_DL_MAX_PENDING_TSN          7
#define MAC_MAX_PARTIAL_PDU_PER_TSN     2
#define UE_REDUCED_MAX_CCTRCH           2
#define UE_REDUCED_MAX_TRCH             5
#define MAC_QSH_MAX_RACH_LCH            8
#define MAC_QSH_MAX_TRCH                5
#define MAC_QSH_UE_MAX_TFC              20
#define UE_REDUCED_MAX_DED_LOGCHAN_PER_TRCH    5


typedef enum
{
  WMAC_QSH_TFC_ALLOWED = 0x00,     /* TFC is allowed */
  WMAC_QSH_TFC_NOT_ALLOWED,        /* TFC is not allowed */
  WMAC_QSH_TFC_MANDATORY,          /* This TFCI is part of the minimum set and hence can't be
                             subjected to the elimination due to CM by HLS or Tx power
                             restriction */
  WMAC_QSH_TFC_REMOVED             /* TFC is removed */
} wmac_qsh_ul_tfc_control_e_type;


typedef enum {
  WMAC_QSH_COMPLETE_PDU = 0,
  WMAC_QSH_PARTIAL_END,
  WMAC_QSH_PARTIAL_START,
  WMAC_QSH_PARTIAL_MIDDLE_OR_START_END,
  WMAC_QSH_PARTIAL_INVALID
} wmac_qsh_i_seg_status;

typedef enum {
  WMAC_QSH_TTI_10MS, /**< 10 ms TTI */
  WMAC_QSH__TTI_20MS, /**< 20 ms TTI */
  WMAC_QSH_TTI_40MS, /**< 40 ms TTI */
  WMAC_QSH_TTI_80MS, /**< 80 ms TTI */
  WMAC_QSH_NUM_TTI   /**< Total number oif TTI types */
} wmac_qsh_tti_e;


typedef PACKED struct PACKED_POST 
{
 
   /* Maximum number of RACH preamble ramping cycles */
   uint8       mmax;
 
   /* Lower bound for back-off time specified as number of 10 msec */
   /* intervals */
   uint8       nbo1min;
 
   /* Upper bound for back-off time specified as number of 10 msec */
   /* intervals */
   uint8       nbo1max;
 
   /* Number of available access service classes */
   uint8       nASC;

   /* Parameters of each valid Access Service Class */
   uint32  pval_asc[MAX_ASC];
  
}wmac_qsh_ul_rach_info_s;

typedef PACKED struct PACKED_POST
{
  uint8 e_tfci_min_set;
  uint8 harq_restriction;
  /* Holds PO for Non-sched mac_qsh-d flows when BOTH carriers are active */
  uint8 non_sched_harq_po;
}wmac_qsh_hs_ul_sec_carrier_info_type;

typedef PACKED struct PACKED_POST
{

  /* Bit 0 - Indicates include in sI
      Bit 1 -7 Priority */
  uint8 priority_si_info;
  /* number of valid fields in rlc_size_ddi_info[] */
  uint8  num_rlc_size_ddi_info; /* 0..32, 0 is valid only when action is NOOP or STOP */

  uint16 ul_flex_min_pdu_size; /* Minimum PDU size           */
  uint16 ul_flex_max_pdu_size; /* Minimum PDU size           */

  uint16 rlc_pdu_size_in_bits[MAC_MAX_DDI];

} mac_qsh_e_ul_logch_info_s_type ;

typedef PACKED struct PACKED_POST
{

  uint8 mac_flow_id;
  /* E-DCH MAC-d flow multiplexing list:*/
  uint8  mac_d_flow_mux_list; /* BITSTRING */

  /* Non-scheduled grant in bytes 0xFFFF indicates it is scheduled grant */
  uint8 mac_nsg;

  uint32 mac_hs_ul_rlc_id_bmp;

  /* number of fields valid in following array is equal to 'num_logch' */
  mac_qsh_e_ul_logch_info_s_type e_ul_logch_info[MAC_NUM_RLC_ID_PER_FLOW];

} wmac_qsh_ul_hs_mac_d_flow_info_s_type;

typedef PACKED struct PACKED_POST
{

  /* MAC logical channel identifier (1-15) */
  uint8 mac_id;

  /* MAC logical channel priority (1-8) */
  uint8 priority;

  uint8 pdu_size;

  uint32 rb_id;

}wmac_qsh_ul_ded_logch_config_s;

typedef PACKED struct PACKED_POST
{
  wmac_qsh_ul_ded_logch_config_s ul_ded_lch_cfg[MAC_QSH_MAX_RACH_LCH];
  wmac_qsh_ul_rach_info_s rach_info;
}wmac_qsh_ul_rach_cfg_s;

typedef PACKED struct PACKED_POST
{
  uint8 trch_id;

  wmac_qsh_tti_e tti_info;
  
  /* List of RLC-ID's configured in this Transport channel */
  uint32 rlc_id_bmp;
}wmac_qsh_r99_ul_trch_info_s;

typedef PACKED struct PACKED_POST
{
  uint8 num_trch;
  /*Audio codec mode */
  uint8 mvs_mode;
  
  wmac_qsh_r99_ul_trch_info_s mac_r99ul_trch[MAC_QSH_MAX_TRCH];

}wmac_qsh_ul_dch_cfg_info_type;

typedef union
{
  wmac_qsh_ul_rach_cfg_s rach_cfg_info;
  wmac_qsh_ul_dch_cfg_info_type  mac_ul_dch_cfg;
}wmac_qsh_r99_ul_cfg_type_s;


typedef PACKED struct PACKED_POST
{
 /* window size as configured by RRC */
  uint8  window_size;

  uint8 num_partials_pendng;

  uint8 num_pending_tsn;

  /* Next Expected TSN  */
  uint16 next_expected_tsn;

  uint16 t1_tsn;
  /* The lower edge of the window.*/
  uint16 lower_edge;
  
  /* The upper edge of the window.*/
  uint16 upper_edge;

  /* List of RLC-ID's that are configured to this Queue-id */
  uint32 rlc_id_bmp;
}wmac_qsh_dl_hs_queue_info_type_s;

typedef PACKED struct PACKED_POST
{
  boolean bch_enable;   /* TRUE             if transport channel is enabled */
  uint8   bcch_id;  /* RLC ID of the logical channel to which this TrCh is mapped */
}wmac_qsh_bch_config_info_type_s;


/*---------------------------------------------------------------------
PCH CONFIG TABLE

The CCTrCh config info received from RRC is validated and stored in
this structure. This information is used during header decoding.
----------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  boolean    pch_enable;
  uint8      pcch_id;    /* RLC ID of the logical channel to which this TrCh is mapped */
} wmac_qsh_pch_config_info_type_s;


/*---------------------------------------------------------------------
Config info of dedicated logical channel mapped to DL DCH
----------------------------------------------------------------------*/

typedef PACKED struct PACKED_POST
{
  boolean          enable;   /* TRUE if the DLC is enabled */
  uint8            mac_id;  
  uint8       rlc_id;   /* RAdio bearer ID */
}wmac_qsh_dlc_dch_config_info_type_s;

/*---------------------------------------------------------------------
DCH CONFIG TABLE

The Transport Channel config info received from RRC is validated and
stored in this structure.
----------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8 cctrch_id;  /* CCTrCh ID */
  uint8 trch_id;  /* Transport channel ID */
  uint8 mac_d_flow_id;
  uint8 num_dlc;    /* Number of DCCH/DTCHs */
  wmac_qsh_dlc_dch_config_info_type_s    dlcinfo[UE_REDUCED_MAX_DED_LOGCHAN_PER_TRCH];    /* Dedicated logical channel configuration */
  boolean    cs_or_ps_domain;    /* 0(False) indicates it is CS domain, 1(TRUE) indicates it is PS domain */

}wmac_qsh_dl_dch_config_info_type_s;


/*---------------------------------------------------------------------
Config info of dedicated logical channel mapped to FACH
----------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  
  boolean  enable;    /* TRUE if dedicated logical channel is enabled */
  uint8    rlc_id;    /* Dedicated logical channel ID */

}wmac_qsh_dlc_fach_config_info_type_s;


/*---------------------------------------------------------------------
FACH CONFIG TABLE

The Transport channel config info received from RRC is validated and
stored in this structure.
----------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
 
  uint8   cctrch_id;      /* ID of the CCTrCH to which this transport channel mapped */
  uint8   trch_id;        /* Transport channel ID */
  boolean bcch_enable;    /* TRUE if BCCH is enabled */
  uint8   bcch_id;        /* BCCH ID */
  boolean ccch_enable;    /* TRUE if CCCH is enabled */
  uint8   ccch_id;        /* CCCH ID */
  boolean ctch_enable;    /* TRUE if CTCH is enabled */
  uint8   ctch_id;        /* CTCH ID */
  boolean dlc_enable;     /* TRUE if there is at least one DCCH/DTCH */
  uint8   num_dlc;        /* Number of DCCH/DTCHs */
  wmac_qsh_dlc_fach_config_info_type_s     dlcinfo[UE_REDUCED_MAX_DED_LOGCHAN_PER_TRCH];    /* Dedicated logical channel info */
}wmac_qsh_fach_config_info_type_s;

typedef PACKED struct PACKED_POST
{
  wmac_qsh_bch_config_info_type_s    bch_cfg_info[UE_REDUCED_MAX_CCTRCH];
  wmac_qsh_pch_config_info_type_s    pch_cfg_info[UE_REDUCED_MAX_CCTRCH];
  wmac_qsh_dl_dch_config_info_type_s dl_dch_cfg_info[UE_REDUCED_MAX_TRCH];
  wmac_qsh_fach_config_info_type_s   fach_cfg_info[UE_REDUCED_MAX_TRCH];
}wmac_qsh_dl_non_hs_cfg_info_type_s;

typedef PACKED struct PACKED_POST
{
  /* For TM nbytes = num_pdu * pdu_size 
        For ctrl_data_bytes = nbytes - new - retx data */
  uint32   nbytes;

  uint32   new_data_bytes;

  uint32   retx_data_bytes;
}wmac_qsh_ul_non_zero_buf_status_type_s;

typedef PACKED struct PACKED_POST
{
  /* RLC ID Bitmap for which non zero BO was reported 
        Bit 0 - RLC-ID 0*/
  uint32 rlc_id_bmp;

  /* NEW uncomp bytes applicable for 1st user RLC-ID alone */
  uint32   new_data_uncomp_bytes;
  
  wmac_qsh_ul_non_zero_buf_status_type_s chan_info[MAC_RLC_MAX_NON_ZERO_BO_EXP];
}wmac_qsh_ul_lc_buf_status_type_s ;


typedef PACKED struct PACKED_POST
{

  uint8 as_id;
  /* bit0 - 0- SC, 1- DC
  	Bit1 - 0 - 10ms or 1 - 2ms TTI
  	Bit2 - 0- HS-RACH/1 - EUL
  	Bit3-4 - ETFCI table used 
  */
  uint8 mac_hs_bmp;
  
  uint8 mac_hs_flow_id_act_bmp;
  /* Bit-0 primary_e_rnti_present;
        bit-1 mac_qsh_hsrach_edch_rsrc_rel_cause_valid;
        bit-2  ernti_hdr_included;
        bit-3 tebs0_si_only_triggered;
        bit-4 hsrach_dxch_tebs0_flag;
        bit-5 lc_include_in_si_current_tti;
        bit-6 collision phase resolved
   */
  uint8 hs_rach_info_bmp;

  uint8 hs_rach_ccch_asc;

  uint8  t2_timer_count;

  uint8 harq_pwr_offst;
  
  wmac_qsh_ul_rach_info_s rach_cfg_info;
  
  wmac_qsh_hs_ul_sec_carrier_info_type mac_hs_ul_sec_carrier_info;
  
  wmac_qsh_ul_hs_mac_d_flow_info_s_type mac_ul_hs_mac_d_flow_info[MAC_MAX_FLOW_ID_EXP];

  wmac_qsh_ul_lc_buf_status_type_s mac_ul_lc_buf_status;
}wmac_qsh_eul_mini_dump_s;

typedef PACKED struct PACKED_POST
{
  uint8 as_id;
  boolean crnti_valid;

  wmac_qsh_r99_ul_cfg_type_s  mac_ul_cfg;

  wmac_qsh_ul_lc_buf_status_type_s mac_ul_lc_buf_status; 
}wmac_qsh_r99_ul_mini_dump_s;

typedef PACKED struct PACKED_POST
{
  uint8 as_id;
  /* Queue -id's that are configured with more than 1 RLC-ID's Bit 0 - queue 0 and so on */
  uint8 queue_id_cfg_bmp;

  boolean mac_hs_cfg_pendig;

  /* First 3 Queue id config */
  wmac_qsh_dl_hs_queue_info_type_s queue_id_info[MAC_QSH_DL_HS_MAX_NUM_QUEUE];
}wmac_qsh_dl_hs_mini_dump_s; 

typedef PACKED struct PACKED_POST
{
  uint8 as_id;
  /* Number of active FACHs */
  uint8    num_active_fachs;
  /* Number of active DSCHs */
  uint8    num_active_dschs;
	
  /* Number of active DL DCHs */
  uint8    num_active_dl_dchs;

  /* Bit 0 - URNTI valid Bit 1 - CRNTI Valid */
  uint8 rnti_valid;

  boolean mac_cfg_pendig;
  wmac_qsh_dl_non_hs_cfg_info_type_s  mac_non_hs_cfg;

}wmac_qsh_r99_dl_mini_dump_s;

typedef enum
{
  WMAC_QSH_DUMP_TAG_R99_DL,
  WMAC_QSH_DUMP_TAG_R99_UL,
  WMAC_QSH_DUMP_TAG_DL_HS,
  WMAC_QSH_DUMP_TAG_EUL,
  WMAC_QSH_DUMP_TAG_MAX
}wmac_qsh_dump_tag_e;

typedef PACKED struct PACKED_POST
{
  /* this header will have a tag and length */
  qsh_dump_tag_hdr_s hdr;
  wmac_qsh_eul_mini_dump_s wmac_eul;
} wmac_qsh_dump_tag_eul_s;  

typedef PACKED struct PACKED_POST
{
  /* this header will have a tag and length */
  qsh_dump_tag_hdr_s hdr;
  wmac_qsh_dl_hs_mini_dump_s wmac_dl_hs;
} wmac_qsh_dump_tag_dl_hs_s;  

typedef PACKED struct PACKED_POST
{
  /* this header will have a tag and length */
  qsh_dump_tag_hdr_s hdr;
  wmac_qsh_r99_ul_mini_dump_s wmac_r99_ul;
} wmac_qsh_dump_tag_r99_ul_s;  

typedef PACKED struct PACKED_POST
{
  /* this header will have a tag and length */
  qsh_dump_tag_hdr_s hdr;
  wmac_qsh_r99_dl_mini_dump_s wmac_r99_dl;
} wmac_qsh_dump_tag_r99_dl_s;  

#endif

