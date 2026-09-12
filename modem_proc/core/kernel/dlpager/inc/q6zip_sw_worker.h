#pragma once

typedef void (*q6zip_sw_worker_fn_t)(q6zip_request_t * request);

/**
 * @brief Initialize Q6ZIP SW worker module
 * @param [in] work Function to work on a request. This is invoked by a worker
 * @return q6zip_error_t 
 */
extern q6zip_error_t q6zip_sw_worker_init (q6zip_sw_worker_fn_t work);

/**
 * @brief Submit a request to the Q6ZIP SW worker
 * @param [in] request The request to work on
 * @return q6zip_error_t 
 */
extern q6zip_error_t q6zip_sw_worker_submit (q6zip_request_t * request);

/**
 * @brief Cleanup Q6ZIP SW worker module
 */
extern void q6zip_sw_worker_fini (void);
