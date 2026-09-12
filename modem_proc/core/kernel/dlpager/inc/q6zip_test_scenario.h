#pragma once
#include "comdef.h"
#include "q6zip_iface.h"
#include "q6zip_test.h"

typedef struct q6zip_test_scenario_s q6zip_test_scenario_t;

typedef struct
{
    /** @brief Maximum number of copies at high priority */
    unsigned int max_high_copies;

    /** @brief Interval between two high priority copies */
    unsigned int interval;

} q6zip_test_low_flood_knobs_t;

typedef struct
{
    /** @brief Maximum (approximate) number of copies to perform. Actual would be
               a bit higher */
    unsigned int max_copies;

    /** @brief Maximum number of Q6ZIP_ALGO_MEMCPY requests in a burst */
    unsigned int max_requests_per_burst;

    /** @brief Time (microseconds) between bursts */
    unsigned int inter_burst_interval;
} q6zip_test_interleaved_knobs_t;

/** @brief Various knobs for this test */
typedef struct
{
    
    /** @brief Knobs for testing response on high priority requests while keeping
               low priority queue flooded */
    q6zip_test_low_flood_knobs_t low_flood;

    q6zip_test_interleaved_knobs_t interleaved;

} q6zip_test_copyd_knobs_t;

typedef void (*q6zip_test_scenario_init_fn_t)(q6zip_test_scenario_t * scenario);
typedef void (*q6zip_test_scenario_conf_fn_t)(q6zip_test_scenario_t * scenario, q6zip_test_copyd_knobs_t const * knobs, uint8 const * msg, uint16 len);
typedef void (*q6zip_test_scenario_start_fn_t)(q6zip_test_scenario_t * scenario);
typedef void (*q6zip_test_scenario_response_fn_t)(q6zip_test_scenario_t * scenario, q6zip_request_t const * request, q6zip_response_t const * response);
typedef void (*q6zip_test_scenario_stop_fn_t)(q6zip_test_scenario_t * scenario);

typedef struct
{
    q6zip_test_scenario_init_fn_t init;
    q6zip_test_scenario_conf_fn_t conf;
    q6zip_test_scenario_start_fn_t start;
    q6zip_test_scenario_response_fn_t response;
    q6zip_test_scenario_stop_fn_t  stop;

} q6zip_test_scenario_operations_t;

typedef struct
{
    /** @brief Configuration for this scenario */
    q6zip_test_copyd_knobs_t knobs;

    /** @brief Has this scenario been stopped? */
    volatile boolean stopped;

    /** @brief Global (ie across test scenarios) data */
    q6zip_test_scenario_global_t * global;

    /** @brief Pointer to custom stuff specific to the scenario */
    void * custom;

} q6zip_test_scenario_data_t;

struct q6zip_test_scenario_s
{
    q6zip_test_scenario_operations_t ops;
    q6zip_test_scenario_data_t data;
};

