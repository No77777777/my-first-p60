/*==============================================================================
     Copyright (c) 2015 QUALCOMM Technologies Incorporated.
     All rights reserved.
     Qualcomm Confidential and Proprietary
==============================================================================*/

/*===========================================================================
                           EDIT HISTORY FOR FILE
$Header: //components/rel/core.mpss/10.0/api/spss/spcom.h#1 $ $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/12/15   as      Initial version. 
===========================================================================*/

#ifndef _SPCOM_H_
#define _SPCOM_H_

#include <stdint.h> /* uint32_t */

/**
 * @brief - Secure Processor Communication API
 *
 * The API is based on Client/Server model.
 * The API resemble the trustzone QSEECOM API.
 * Currently, expecting the Secure Processor side to have servers and the HLOS
 * side to have clients. Request is initiated by the client and responded by the
 * server.
 * This API should be used by Android (HLOS) User Space Application.
 * Message/Response are expected to be synchronous.
 */

/*===========================================================================*/
/*                           defines, enums , types                          */
/*===========================================================================*/

/* Maximum size (including null) for channel names - match GLINK_NAME_SIZE */
#define SPCOM_CHANNEL_NAME_SIZE        32

/**
 * Request buffer size.
 * Any large data (multiply of 4KB) is provided by temp buffer in DDR.
 * Request shall provide the temp buffer physical address (align to 4KB).
 * Maximum request/response size of 268 is used to accommodate APDU size.
 * From kernel spcom driver perspective a PAGE_SIZE of 4K
 * is the actual maximum size for a single read/write file operation.
 */
#define SPCOM_MAX_REQUEST_SIZE         268
#define SPCOM_MAX_RESPONSE_SIZE        268

/**
 * Abstract spcom handle.
 * The actual struct definition is internal to the spcom driver.
 */
struct spcom_client; /* Forward declaration */
struct spcom_server; /* Forward declaration */

/**
 * Client registration info
 *
 * @ch_name:   glink logical channel name
 * @notify_ssr_cb: callback when the remote SP side reset (power down).
 *      This is likely to happen due to remote subsystem restart (SSR).
 *      NULL callback means no notification required.
 *      Upon ssr callback, the user should unregister,
 *      Poll for link up and then register again.
 */
struct spcom_client_info {
       const char *ch_name;
       void (*notify_ssr_cb)(void);
};

/**
 * Server registration info
 *
 * @ch_name:   glink logical channel name
 * @notify_ssr_cb: callback when the remote SP side reset (power down).
 *      This is likely to happen due to remote subsystem restart (SSR).
 *      NULL callback means no notification required.
 *      Upon ssr callback, the user should unregister,
 *      Poll for link up and then register again.
 */
struct spcom_service_info {
       const char *ch_name;
       void (*notify_ssr_cb)(void);
};

/*===========================================================================*/
/*                         IS LINK UP                                        */
/*===========================================================================*/

/**
 * spcom_is_sp_subsystem_link_up() - check if SPSS link is up.
 *
 * return: true if link is up, false if link is down.
 */
boolean spcom_is_sp_subsystem_link_up(void);

/*===========================================================================*/
/*                           Client Send Message                             */
/*===========================================================================*/
/**
 * spcom_register_client() - register client for channel
 *
 * Only one client/Server can register on each side of a channel.
 * Server on remote side is expected to be running and connected,
 * therefore connection expected within the provided timeout.
 * Handle is returned even if timeout expired.
 * use spcom_client_is_server_connected() to check fully connected.
 *
 * @info:      Client configuration info (input).
 *
 * return: client handle on sucess, NULL on failure.
 */
struct spcom_client *spcom_register_client(struct spcom_client_info *info);

/**
 * spcom_unregister_client() - unregister client for channel
 *
 * @client:    Client Handle.
 *
 * return: 0 on success, negative error code on failure (see errno.h)
 */
int spcom_unregister_client(struct spcom_client *client);

/**
 * spcom_client_send_message_sync() - Send a synchronous request and response
 *
 * @client:    a pointer to spcom client
 * @req_ptr:   a pointer to the request C struct representation
 * @req_size:  size of the request C struct
 * @resp_ptr:  a pointer to the response C struct representation
 * @resp_size:  size of the response C struct
 * @timeout_msec: Timeout in msec between command and response, 0=no timeout.
 *
 * return: num of rx bytes on success, negative error code on failure (see errno.h)
 */
int spcom_client_send_message_sync(struct spcom_client *client,
                                  void                 *req_ptr,
                                  uint32_t             req_size,
                                  void                 *resp_ptr,
                                  uint32_t             resp_size,
                                  uint32_t             timeout_msec);

/**
 * spcom_client_is_server_connected() - Check if remote server connected.
 *
 * This API checks that the logical channel is fully connected between
 * the client and the server.
 * Normally, the server should be up first and connect first.
 *
 * @client:    a pointer to spcom client
 *
 * return: true if server connected, false otherwise.
 */
boolean spcom_client_is_server_connected(struct spcom_client *client);

/*===========================================================================*/
/*                           Service                                         */
/*===========================================================================*/

/**
 * spcom_register_service() - register server for channel
 *
 * Only one client/Server can register on each side of a channel.
 *
 * @info:      Server configuration info (input).
 *
 * return: server handle on sucess, NULL on failure.
 */
struct spcom_server *spcom_register_service(struct spcom_service_info *info);

/**
 * spcom_unregister_service() - unregister server for channel
 *
 * @server:    server Handle.
 *
 * return: 0 on success, negative error code on failure (see errno.h)
 */
int spcom_unregister_service(struct spcom_server *server);

/**
 * spcom_server_get_next_request_size() - get the size of the
 * next request
 *
 * This API MUST be called before calling spcom_server_wait_for_request().
 * The server should allocate the relevant buffer size.
 *
 * @server:    a pointer to spcom server
 *
 * return: size of request in bytes on success, negative value on failure (see errno.h)
 */
int spcom_server_get_next_request_size(struct spcom_server *server);

/**
 * spcom_server_wait_for_request() - server wait for request
 *
 * @server:     a pointer to spcom server
 * @req_ptr:   a pointer to the request buffer
 * @req_size:  size of the buffer provided.
 * The server should provide a buffer of atleast the size
 * returned by spcom_server_get_next_request_size() and up to
 * SPCOM_MAX_REQUEST_SIZE.
 *
 * return: size of request on success, negative value on failure (see errno.h)
 */
int spcom_server_wait_for_request(struct spcom_server  *server,
                                 void                  *req_ptr,
                                 uint32_t              req_size);

/**
 * spcom_server_send_response() - Send a the response to request
 *
 * @server:    a pointer to spcom server
 * @resp_ptr:  a pointer to the response C struct representation
 * @resp_size:  size of the response C struct
 *
 * return: sent data size on success, negative value on failure (see errno.h)
 */
int spcom_server_send_response(struct spcom_server     *server,
                              void                     *resp_ptr,
                              uint32_t                 resp_size);

#endif /* _SPCOM_H_ */
