#ifndef _APM_PRV_API_
#define _APM_PRV_API_

/*==============================================================================
  @file apm_prv_api.h
  @brief This file contains private apis for APM module

  Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/

/*============================================================================
  Edit History

  when       who       what, where, why
  --------   -------   -------------------------------------------------------
============================================================================== */
#include "mmdefs.h"

/**
  @ingroup apm_cmd_get_memory_stats
  This API is used by client to request information on Gecko memory consumption

  @gpr_hdr_fields
  Opcode -- APM_CMD_GET_MEMORY_STATS
  
  @msg_payload
  None

  @return
  Opcode -- APM_CMD_RSP_GET_MEMORY_STATS
  
  @dependencies
  None
 */
#define APM_CMD_GET_MEMORY_STATS 0x01001011

/**
  @ingroup apm_cmd_rsp_get_memory_stats
  This API is sent in response to APM_CMD_GET_MEMORY_STATS command
  The response payload returns statistics related to Gecko memory
  allocation use

  @gpr_hdr_fields
  Opcode -- APM_CMD_RSP_GET_MEMORY_STATS

  @msg_payload
  apm_cmd_rsp_get_memory_stats_t

  @return
  None
  
  @dependencies
  None
 */
#define APM_CMD_RSP_GET_MEMORY_STATS 0x02001004

/**
  This structure is the payload structure used by APM_CMD_RSP_GET_MEMORY_STATS command.
  Contains the statistics related to Gecko memory allocation use. This can be used to
  detect how fully the Gecko heap is being used and to track memory allocation/free
  counts for memory leaks
 */
#include "gk_begin_pack.h"

struct apm_cmd_rsp_get_memory_stats_t
{
   uint32_t num_mallocs;
   /**< Number of memory allocations that have occurred since bootup or
        since statistics were reset. */

   uint32_t num_frees;
   /**< Number of times memory was freed since bootup or since
        statistics were reset. */

   uint32_t current_heap_use;
   /**< Current number of bytes allocated from the heap since statistics were
        reset for . */

   uint32_t peak_heap_use;
   /**< Peak number of bytes allocated from the heap since bootup or since
        statistics were reset for .*/

   uint32_t num_non_mallocs;
   /**< Number of non- memory allocations that have occurred since bootup
        or since statistics were reset. */

   uint32_t num_non_frees;
   /**< Number of times non- memory was freed since bootup or since
        statistics were reset. */

   uint32_t current_non_heap_use;
   /**< Current number of bytes allocated from the heap since statistics were
        reset for non-. */

   uint32_t peak_non_heap_use;
   /**< Peak number of bytes allocated from the heap since bootup or since
        statistics were reset for non-. */
   uint32_t num_nondefault_mallocs;
   /**< Number of  lpa and lpm heap memory allocations that have occurred since bootup or
            since statistics were reset. */

   uint32_t num_nondefault_frees;
   /**< Number of times  lpa  and lpm heap memory was freed since bootup or since
            statistics were reset. */

   uint32_t current_nondefault_heap_use;
   /**< Current number of bytes allocated from the lpa and lpm heap since statistics were
            reset for . */

   uint32_t peak_nondefault_heap_use;
   /**< Peak number of bytes allocated from the lpa and lpm heap since bootup or since
            statistics were reset for .*/
}

#include "gk_end_pack.h"
;
typedef struct apm_cmd_rsp_get_memory_stats_t apm_cmd_rsp_get_memory_stats_t;

/**
  @ingroup apm_cmd_reset_peak_heap_use
  Requests that the Gecko reset the peak heap usage to zero, as reported in
  profiling events and acknowledgments.

  @gpr_hdr_fields
  Opcode -- APM_CMD_RESET_PEAK_HEAP_USE

  @msg_payload
  None

  @return
  None
  
  @dependencies
  None
 */
#define APM_CMD_RESET_PEAK_HEAP_USE 0x01001012


#include "gk_begin_pack.h"

struct apm_imcl_peer_domain_info_t
{
   uint32_t module_iid;
   /**< Module instance ID of the remote peer module */

   uint32_t domain_id;
   /**< Domain ID of remote peer module */
}

#include "gk_end_pack.h"
;
typedef struct apm_imcl_peer_domain_info_t apm_imcl_peer_domain_info_t;


/**
  @ingroup apm_param_id_imcl_peer_domain_info_t
  This API is sent in addition to the OPEN payload by the OLC 
  to the SATELLITE APM. This gives the information about the 
  domain IDs of the IMCL peers.

  @gpr_hdr_fields
  Opcode -- APM_PARAM_ID_IMCL_PEER_DOMAIN_INFO

  @msg_payload
  apm_param_id_imcl_peer_domain_info_t

  @return
  None
  
  @dependencies
  None
 */
#define APM_PARAM_ID_IMCL_PEER_DOMAIN_INFO 0x08001153

/**
  This structure is the payload structure used by APM_PARAM_ID_IMCL_PEER_DOMAIN_INFO param
  at the end of the container payload of the APM_CMD_GRAPH_OPEN. This is used to communicate
  to the APM on the remote processors about the domain IDs of the Inter-processor IMCL peers.
 */

#include "gk_begin_pack.h"
#include "gk_begin_pragma.h"

struct apm_param_id_imcl_peer_domain_info_t
{
   uint32_t num_imcl_peer_cfg;
   /**< Number of IMCL peer configuration objects
        @values  */

   apm_imcl_peer_domain_info_t imcl_peer_cfg[0];
   /**< Number of these objects is determined
        by num_imcl_peer_cfg.
        @values  */

}
#include "gk_end_pragma.h"
#include "gk_end_pack.h"
;

typedef struct apm_param_id_imcl_peer_domain_info_t apm_param_id_imcl_peer_domain_info_t;

#endif //_APM_PRV_API_
