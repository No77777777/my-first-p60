#include <stdlib.h>
#include <string.h>
#include "uimage_rpm_glink.h"
#include "uimage_rpm_assert.h"
#include "uimage_rpm_log.h"
#include "HAL_uimage_rpm_hwio.h"
#include "HALhwio.h"
#include "qurt_atomic_ops.h"
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "DDIHWIO.h"
#include "glink_urpm.h"

static uImage_rpm_request_t uImage_rpm_req = 
{
    .service              = REQ_KEY,
    .len_with_header      = 32,
    .set                  = ACTIVE_SET,
    .data_len             = 12,
    .value_len            = 4,
};

/*
 * used for receiving ACK message from RPM
 */
static uImage_rpm_ack_t uImage_rpm_ack_msg;
static volatile uImage_rpm_ack_t uImage_rpm_invalid_ack_msg; /* used to store last invalid ack msg for debugging purpose
                                                              * since abort causes uSleep to attempt to exit again
                                                              * and it overwrote the state when an issue happened.
                                                              */

/*******************************************************************************
 *
 * uImage_rpm_sendMessage
 *
 * This function packages up the request and sends it via GLink
 */
void uImage_rpm_sendMessage(uint32 msg_id, const uImage_rpm_resource_info_t *resource_info)
{
    uImage_rpm_req.msg_id        = msg_id;
    uImage_rpm_req.resource_type = resource_info->resource_type;
    uImage_rpm_req.resource_id   = resource_info->resource_id;
    uImage_rpm_req.key           = resource_info->resource_key;
    uImage_rpm_req.value         = resource_info->resource_value;

    UIMAGE_RPM_LOG_DEBUG("send_msg (msg_id:%d) (type:0x%08X) (id:%d) (value:%d)",
                         uImage_rpm_req.msg_id,
                         uImage_rpm_req.resource_type,
                         uImage_rpm_req.resource_id,
                         uImage_rpm_req.value);


    //send request via glink. Retry if not enough space
    while(!glink_rpm_write((void*)(&uImage_rpm_req), (int)sizeof(uImage_rpm_request_t))){}

    UIMAGE_RPM_LOG_DEBUG("send_msg_done");
}

#define MAX_ACK_SIZE    100
/*******************************************************************************
 *
 * uImage_rpm_readAck
 *
 * This function retrieves an ACK
 */
static volatile int send_val = 0;
static volatile uint8 gbuf[MAX_ACK_SIZE]; /* CR 1092413: moved to global as stack overflowed */
DALBOOL uImage_rpm_readAck(uint32 msg_id, DALBOOL clear_nas)
{
    /*
     * @param[in]   buffer      pointer to the client buffer
     * @param[in]   size        size of the client buffer
     *
     * @return      positive value equal to the size of the data copied into
     client buffer, 0 if there is no incoming data,
     negative value equal to -size of incoming packet
     if the client buffer is too small
     *
     * @sideeffects  None
     */
    //try to read an ACK
    do
    {
        send_val = glink_rpm_read((void*)gbuf, MAX_ACK_SIZE);

        //send_val = glink_rpm_read((void*)&uImage_rpm_ack_msg, (int)sizeof(uImage_rpm_ack_t));

        UIMAGE_RPM_ASSERT(send_val >= 0, "buffer too small: %d (%d)", send_val, MAX_ACK_SIZE);

        memcpy((void*)&uImage_rpm_ack_msg, (void*)gbuf, sizeof(uImage_rpm_ack_msg));

        if((!send_val) && clear_nas)
        {
            //if we read nothing and we are clearing NAS ACKs, bail early and return false
            return FALSE;
        }
    }
    while(0 == send_val);

    if(FALSE == clear_nas)
    {
        if(uImage_rpm_ack_msg.service != REQ_KEY)
        {
            memcpy((void*)&uImage_rpm_invalid_ack_msg, (void*)&uImage_rpm_ack_msg, sizeof(uImage_rpm_ack_msg));
            UIMAGE_RPM_LOG("unknown service: 0x%08x", uImage_rpm_ack_msg.service);
            /*
             * Return FALSE rather than abort when the service validation failed
             * in order to discard V1 protocol ACK.
             *
             * There was a short window that RPM sent V1 protocol ACK after uimage RPM driver
             * had cleared the existing ACKs, and caused the validation failure.
             */
            return FALSE;
        }

        if(uImage_rpm_ack_msg.msg_id_key != MSG_KEY)
        {
            memcpy((void*)&uImage_rpm_invalid_ack_msg, (void*)&uImage_rpm_ack_msg, sizeof(uImage_rpm_ack_msg));
            UIMAGE_RPM_ASSERT(0, "invalid key: 0x%08x", uImage_rpm_ack_msg.msg_id_key);
        }

        if(uImage_rpm_ack_msg.msg_id_len != sizeof(uint32))
        {
            memcpy((void*)&uImage_rpm_invalid_ack_msg, (void*)&uImage_rpm_ack_msg, sizeof(uImage_rpm_ack_msg));
            UIMAGE_RPM_ASSERT(0, "invalid len: 0x%08x", uImage_rpm_ack_msg.msg_id_len);
        }

        UIMAGE_RPM_LOG_DEBUG("got_ack (msg_id:%d)", uImage_rpm_ack_msg.msg_id);

        if(uImage_rpm_ack_msg.msg_id != msg_id)
        {
            /*
             * Return FALSE rather than abort when the msg ID is not the one we are waiting for.
             *
             * During bringup, we fixed an issue of matching wrong ACKs by using different message IDs
             * for DDR-ON/DDR-OFF requests.
             * But we observed the case that ADSP received DDR-OFF ACKs just after sending DDR-ON requests,
             * causing message ID mismatch and making ADSP aborted.
             * To resolve the race condition, allow ACKs with unexpected msg ID and return FALSE
             * so that we can robustly receive the ACK with the expected one.
             */
            return FALSE;
        }
    }
    return TRUE;
}

