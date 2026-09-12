/*==============================================================================

                    S S G C C S _ M S G R _ M S G . H

DESCRIPTION
  SSGCCS QMI service msgr messages file

Copyright (c) 2018, 2020 by QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.

==============================================================================*/
#ifndef SSGCCS_MSGR_MSG_H
#define SSGCCS_MSGR_MSG_H

#include <msgr.h>
#include <msgr_umid.h>
#include "qmi_csi.h"


/*==============================================================================
                           Module definitions
==============================================================================*/
#define MSGR_MODULE_SSGCCS 0x1c
#define MSGR_QMI_SSGCCS MSGR_TECH_MODULE(MSGR_TECH_QMI, MSGR_MODULE_SSGCCS)

#define SSGCCS_CELL_INFO_LEN_MAX 1024
#define SSGCCS_INTERNAL_CELL_INFO_LEN_MAX 8192
#define SSGCCS_LEGACY_SCORE_INFO_LEN_MAX 1024

// Message structure for cell info indication message
typedef struct
{
    msgr_hdr_s msg_hdr;
    uint32 cell_info_len;
    uint8 cell_info[SSGCCS_CELL_INFO_LEN_MAX];
} ssgccs_cell_info_ind_msg_type;

// Message structure for internal cell info indication message
typedef struct
{
    msgr_hdr_s msg_hdr;
    uint32 cell_info_len;
    int event;
    uint8 cell_info[SSGCCS_INTERNAL_CELL_INFO_LEN_MAX];
} ssgccs_internal_cell_info_ind_msg_type;

// message structure for Timer info indication
typedef struct
{
  msgr_hdr_s msg_hdr;
  unsigned long param;
}ssgcs_timer_expiry_type;

// Message structure for Secure Channel Requests
typedef struct
{
    msgr_hdr_s msg_hdr;
    qmi_client_handle client_handle;
    uint32 req_buff_len;
    uint8 req_buff[SSGCCS_CELL_INFO_LEN_MAX];
} ssgccs_sca_req_buff_msg_type;

// Message structure for msg from ssgsca
typedef struct
{
    msgr_hdr_s msg_hdr;
    uint32 sc_info_len;
    uint8 sc_info[SSGCCS_CELL_INFO_LEN_MAX];
} ssgccs_sc_info_ind_msg_type;

/*==============================================================================
                           Msg UMID definitions
==============================================================================*/
enum
{
    SSGCCS_IND_FIRST = MSGR_UMID_BASE(MSGR_QMI_SSGCCS, MSGR_TYPE_IND),

    /* QMI_SSGCCS_CELL_INFO_IND  - Interface to pass cell info */
    MSGR_DEFINE_UMID(QMI, SSGCCS, IND, CELL_INFO, 0x01, ssgccs_cell_info_ind_msg_type),

    // QMI_SSGCCS_INTERNAL_CELL_INFO_IND- Interface to pass internal cell info
    MSGR_DEFINE_UMID(QMI, SSGCCS, IND, INTERNAL_CELL_INFO, 0x02, ssgccs_internal_cell_info_ind_msg_type),

    // QMI_SSGCCS_TIMER_EXPIRY_IND- Interface to pass internal cell info
    MSGR_DEFINE_UMID(QMI, SSGCCS, IND, TIMER_EXPIRY, 0x03, ssgcs_timer_expiry_type),

    // QMI_SSGCCS_SCA_UPDATE_CONFIG_REQ
    MSGR_DEFINE_UMID(QMI, SSGCCS, REQ, SCA_UPDATE_CONFIG, 0x04, ssgccs_sca_req_buff_msg_type),

    // QMI_SSGCCS_SC_INFO_IND
    MSGR_DEFINE_UMID(QMI, SSGCCS, IND, SC_INFO, 0x05, ssgccs_sc_info_ind_msg_type),

    SSGCCS_IND_MAX,
    SSGCCS_IND_LAST = SSGCCS_IND_MAX - 1
};

#endif /* SSGCCS_MSGR_MSG_H */
