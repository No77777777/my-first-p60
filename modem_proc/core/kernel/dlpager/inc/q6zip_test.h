#pragma once
#include "comdef.h"
#include "fs_errno.h"
#include "q6zip_iface.h"
#include "q6zip_request.h"
#include "q6zip_waiter.h"
#include "qurt.h"

typedef enum
{
    Q6ZIP_TEST_SIGNAL_QUIT  = (1 << 0),
    Q6ZIP_TEST_SIGNAL_STOP  = (1 << 1),
    Q6ZIP_TEST_SIGNAL_SCENARIO_START = (1 << 2),
    Q6ZIP_TEST_SIGNAL_TMO   = (1 << 3),

} q6zip_test_signal_t;

#define Q6ZIP_TEST_SIGNALS \
    ( Q6ZIP_TEST_SIGNAL_QUIT | \
      Q6ZIP_TEST_SIGNAL_SCENARIO_START )

#define Q6ZIP_TEST_SIGNALS_SCENARIO_ONLY \
    ( Q6ZIP_TEST_SIGNAL_STOP | \
      Q6ZIP_TEST_SIGNAL_TMO )

typedef struct 
{
    unsigned char const * unzipped;
    unsigned int unzipped_size;
    unsigned char const * zipped;
    unsigned int zipped_size;

} q6zip_test_vectors_index_t;

typedef struct q6zip_test_scenario_global_s q6zip_test_scenario_global_t;

extern q6zip_test_scenario_global_t * q6zip_test_global;

extern void q6zip_test_init (void);
extern void q6zip_test_adapter_response (q6zip_request_t * request, q6zip_response_t const * response);
extern boolean q6zip_test_handle_diag_msg (uint8 const * msg, uint16 len);

/**
 * @brief Get Q6ZIP implementation 
 *  
 * @return q6zip_iface_t* Pointer to current implementation
 */
q6zip_iface_t const * q6zip_test_get_implementation (void);

/**
 * @brief Load contents of file into a buffer 
 *  
 * @param [in] filename Path/name of file to load
 * @param [in] buffer Buffer to load contents into 
 *  
 * @return 0 if no error, 0 > on error 
 */
extern int q6zip_test_load_file (char const * filename, q6zip_buffer_t * buffer);

extern void q6zip_test_load_vector (
    char const * directory,
    q6zip_buffer_t * compressed,
    q6zip_buffer_t * uncompressed
);

extern void q6zip_test_unzip_ro (
    void *           dst,
    unsigned int     dst_len,
    void *           src,
    unsigned int     src_len,
    void *           context,
    q6zip_waiter_t * waiter
);

/**
 * @brief Submit RW unzip request and wait (if waiter provided)
 * 
 * @param [in] dst 
 * @param [in] dst_len 
 * @param [in] src 
 * @param [in] src_len 
 * @param [in] waiter 
 */
extern void q6zip_test_unzip_rw (
    void *           dst,
    unsigned int     dst_len,
    void *           src,
    unsigned int     src_len,
    void *           context,
    q6zip_waiter_t * waiter
);

/**
 * @brief Submit zip RW request and (optionally) wait
 * 
 * @param dst Pointer to destination buffer
 * @param dst_len Size of destination
 * @param src Pointer to source (uncompressed) buffer
 * @param src_len Size of source
 * @param priority Priority to be used
 * @param context User specified context
 * @param waiter Waiter object to wait on (if non-NULL)
 */
extern void q6zip_test_zip_rw (
    void *           dst,
    unsigned int     dst_len,
    void *           src,
    unsigned int     src_len,
    q6zip_priority_t priority,
    void *           context,
    q6zip_waiter_t * waiter
);

extern void q6zip_test_submit_dma (
    q6zip_iovec_t const * dst,
    q6zip_iovec_t const * src,
    q6zip_waiter_t *      waiter 
);

extern boolean q6zip_test_dma_verify (
    q6zip_iovec_t const * dst,
    q6zip_iovec_t const * src,
    q6zip_iovec_t const * expected,
    q6zip_waiter_t *      waiter 
);

/**
 * @brief Wait for some time
 * 
 * @param [in] duration Duration (microseconds) to wait for
 */
extern void q6zip_test_wait (qurt_timer_duration_t duration);

extern void q6zip_test_fini (void);
