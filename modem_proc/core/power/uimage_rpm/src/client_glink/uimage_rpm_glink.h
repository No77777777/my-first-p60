#ifndef UIMAGE_RPM_GLINK_H
#define UIMAGE_RPM_GLINK_H

#include "DALStdDef.h"
#include "uimage_rpm_resource_data.h"

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

#define ACTIVE_SET   0

#define MSG_KEY  0x2367736d /* "msg#" */
#define ERR_KEY  0x00727265 /* "err" */
#define REQ_KEY  0x00716572 /* "req" */

/*******************************************************************************
 *
 * uImage_rpm_glink_init
 *
 * This function is used to set up GLink port to communicate with RPM FW.
 *
 */
void uImage_rpm_glink_init(void);

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

#endif /* UIMAGE_RPM_GLINK_H */

