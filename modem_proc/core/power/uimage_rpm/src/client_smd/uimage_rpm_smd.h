#ifndef UIMAGE_RPM_SMD_H
#define UIMAGE_RPM_SMD_H

#include "DALStdDef.h"
#include "uimage_rpm_resource_data.h"

#define SMD_PACKET_HEADER_SIZE      (5)
#define SMD_PACKET_HEADER_BYTE_SIZE (5 * sizeof(uint32))

#define ACTIVE_SET   0

#define MSG_KEY  0x2367736d /* "msg#" */
#define ERR_KEY  0x00727265 /* "err" */
#define REQ_KEY  0x00716572 /* "req" */


typedef enum
{
    SMD_SIG_ID_TX_DSR        = 0,   /* transmit side */
    SMD_SIG_ID_RX_DTR        = 0,   /* receive side */
    SMD_SIG_ID_TX_CTS        = 1,   /* transmit side */
    SMD_SIG_ID_RX_RTS        = 1,   /* receive side */
    SMD_SIG_ID_CD            = 2,
    SMD_SIG_ID_RI            = 3,
    SMD_SIG_ID_DATA_WRITTEN  = 4,
    SMD_SIG_ID_DATA_READ     = 5,
    SMD_SIG_ID_STATE_UPDATED = 6,

    SMD_NUM_IF_SIGS          = 8,   /* Must be multiple of 4 for alignment */
} smd_if_sig_ids_t;

/* New SMD channel structure created with all elements word aligned */
typedef struct
{
    uint32 stream_state;
    uint32 if_sigs[SMD_NUM_IF_SIGS];
    uint32 read_index;
    uint32 write_index;
} uImage_smd_shared_stream_info_algn_t;

typedef struct
{
    /*
     * The following data comes from SMD about the channel
     * Must be virtual address; otherwise, it causes a TLB exception
     */
    volatile uImage_smd_shared_stream_info_algn_t *tx_shared_info_ptr_algn;
    volatile uImage_smd_shared_stream_info_algn_t *rx_shared_info_ptr_algn;
    const uint32 fifo_sz;
    volatile uint32 *tx_shared_fifo;
    volatile uint32 *rx_shared_fifo;
} uImage_smd_shared_info_t;

/* 
 * Internal bits for messaging
 */
typedef struct
{
    uint32 service;
    uint32 len_with_header;
    uint32 msg_id;
    uint32 set;
    uint32 resource_type;
    uint32 resource_id;
    uint32 data_len;
    uint32 key;
    uint32 value_len;
    uint32 value;
} uImage_rpm_request_t;

typedef struct
{
    uint32 service;
    uint32 len_with_header;
    uint32 msg_id_key;
    uint32 msg_id_len;
    uint32 msg_id;
} uImage_rpm_ack_t;

/*******************************************************************************
 *
 * uImage_rpm_smd_init
 *
 * This function is used to initialize SMD shared info.
 *
 */
void uImage_rpm_smd_init(void);

/*******************************************************************************
 *
 * uImage_rpm_sendMessage
 *
 * This function is used to send a message to RPM FW by writing into the
 * message RAM, and signalling when done
 *
 *   msg_id:          Message ID
 *   resource_info:   Resource request info
 *
 */
void uImage_rpm_sendMessage(uint32 msg_id, const uImage_rpm_resource_info_t *resource_info);

/*******************************************************************************
 *
 * uImage_rpm_readAck
 *
 * This function is used to clear all DDR ACKs the first time we enter
 * uImage and then on to read Acks from RPM and ensure the contents of
 * the ack are as expected
 *
 *   msg_id:     Message ID
 *   clear_nas:  Clear out NAS message Acks from DDR
 *
 */
DALBOOL uImage_rpm_readAck(uint32 msg_id, DALBOOL clear_nas);

#endif /* UIMAGE_RPM_SMD_H */

