#ifndef _APM_CONTAINER_API_H_
#define _APM_CONTAINER_API_H_

/*========================================================================*/
/**
@file apm_container_api.h

This file contains APM container commands and events structures definitions.*/

/*===========================================================================
Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================ */
/*========================================================================
Edit History

$Header: //components/rel/avs.mpss/9.0.c51/api/apm/apm_container_api.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------
03/27/18    SJ    Created File
07/01/19    pbm   H2xml annotations
========================================================================== */

#include "mmdefs.h"
#include "apm_graph_properties.h"

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

/*====================================================================================================================*/
/*====================================================================================================================*/

#define APM_PARAM_ID_CONTAINER_CONFIG             0x08001000
/** h2xmlp_parameter   {"Container Configuration", APM_PARAM_ID_CONTAINER_CONFIG}
    h2xmlp_description {ID of the parameter to configure the container properties.\n }
    h2xmlp_toolPolicy  {RTC; Calibration}*/


/* Immediately following this structure is a variable length array of structure objects corresponding to each container
   configuration and properties. Length of the array is determined by number of containers being configured */ 
#include "gk_begin_pack.h"
struct apm_param_id_container_cfg_t
{
   uint32_t num_container;
   /**< h2xmle_description   {Number of containers being configured}
        h2xmle_range         {0..0xFFFFFFFF}
        h2xmle_default       {0} */
}
#include "gk_end_pack.h"
;
typedef struct apm_param_id_container_cfg_t apm_param_id_container_cfg_t;

/* Immediately following this structure is a variable length array of container property structure. Length of the array
   is determined by number of properties being configured */
#include "gk_begin_pack.h"
struct apm_container_cfg_t
{
   uint32_t container_id;
   /**< h2xmle_description   {Valid ID of the container}
        h2xmle_range         {0..0xFFFFFFFF}
        h2xmle_default       {0} */

   uint32_t num_prop;
   /**< h2xmle_description   {Number of properties for this sub-graph}
        h2xmle_range         {0..0xFFFFFFFF}
        h2xmle_default       {0} */
}
#include "gk_end_pack.h"
;
typedef struct apm_container_cfg_t apm_container_cfg_t;



#include "gk_begin_pack.h"
struct apm_cont_capability_t
{
   uint32_t capability_id;
   /**< @h2xmle_rangeList {"INVALID"=APM_PROP_ID_INVALID,
                           "PP"=APM_CONTAINER_CAP_ID_PP,
                           "CD"=APM_CONTAINER_CAP_ID_CD,
                           "EP"=APM_CONTAINER_CAP_ID_EP,
                           "OLC"=APM_CONTAINER_CAP_ID_OLC}
        @h2xmle_default {APM_PROP_ID_INVALID}
        @h2xmle_description {Capability ID for this container} */
}
#include "gk_end_pack.h"
;
typedef struct apm_cont_capability_t apm_cont_capability_t;

/** h2xmlgp_configType {"CONT_CFG"}
    @h2xmlgp_config {"CONT_CFG", "APM_PARAM_ID_CONTAINER_CONFIG", APM_PARAM_ID_CONTAINER_CONFIG}
    @h2xmlgp_description {ID of the parameter to configure the container properties}
    @{ <-- Start of container_cfg --> */
/*--------------------------------------------------------------------------------------------------------------------*/
#define APM_CONTAINER_PROP_ID_CAPABILITY_LIST     0x08001011
/** @h2xmlp_property {"Capability List", APM_CONTAINER_PROP_ID_CAPABILITY_LIST}
    @h2xmlp_description {Container property ID for Capability List} */

/**
    @h2xmlp_insertSubStruct{apm_cont_capability_t}
*/

/* Immediately following this structure is a variable length array of container capability ID's. Length of the array
   is determined by number of ID's being configured for this container instance. */
#include "gk_begin_pack.h"
struct apm_cont_prop_id_cap_list_t
{
   uint32_t num_capability_id;
   /**< @h2xmle_range {1..0xFFFFFFFF}
        @h2xmle_default {1}
        @h2xmle_description {Number of capabilities for this container} */

#ifdef __H2XML__
   apm_cont_capability_t cont_capability_id_list[0];
   /**< @h2xmle_range {0..0xFFFFFFFF}
        @h2xmle_default {APM_PROP_ID_INVALID}
        @h2xmle_description {Capability ID list for this container}
        @h2xmle_variableArraySize {num_capability_id}*/
#endif
}
#include "gk_end_pack.h"
;
typedef struct apm_cont_prop_id_cap_list_t apm_cont_prop_id_cap_list_t;

/*--------------------------------------------------------------------------------------------------------------------*/
#define APM_CONTAINER_PROP_ID_GRAPH_POS           0x08001012
/** @h2xmlp_property {"Graph Position", APM_CONTAINER_PROP_ID_GRAPH_POS}
    @h2xmlp_description {Container property ID for Graph Position} */
/**< Enumeration for container graph position Stream */
#define APM_CONT_GRAPH_POS_STREAM                 0x1

/**< Enumeration for container graph position Per Stream Per Device*/
#define APM_CONT_GRAPH_POS_PER_STR_PER_DEV        0x2

/**< Enumeration for container graph position Stream-Device */
#define APM_CONT_GRAPH_POS_STR_DEV                0x3

/**< Enumeration for container graph position Global Device */
#define APM_CONT_GRAPH_POS_GLOBAL_DEV             0x4

#include "gk_begin_pack.h"
struct apm_cont_prop_id_graph_pos_t
{
   uint32_t graph_pos;
   /**< @h2xmle_rangeList {"DONT_CARE"=0xFFFFFFFF,
                           "STREAM"=APM_CONT_GRAPH_POS_STREAM,
                           "PSPD"=APM_CONT_GRAPH_POS_PER_STR_PER_DEV,
                           "STR_DEV"=APM_CONT_GRAPH_POS_STR_DEV,
                           "GLOBAL DEV"=APM_CONT_GRAPH_POS_GLOBAL_DEV}
        @h2xmle_default {0xFFFFFFFF}
        @h2xmle_description {Container position in the graph.} */
}
#include "gk_end_pack.h"
;
typedef struct apm_cont_prop_id_graph_pos_t apm_cont_prop_id_graph_pos_t;

/*--------------------------------------------------------------------------------------------------------------------*/
#define APM_CONTAINER_PROP_ID_STACK_SIZE          0x08001013
/** @h2xmlp_property {"Stack Size", APM_CONTAINER_PROP_ID_STACK_SIZE}
    @h2xmlp_description {Container property ID for Stack Size } */

#include "gk_begin_pack.h"
struct apm_cont_prop_id_stack_size_t
{
   uint32_t stack_size;
   /**< @h2xmle_range {0..0xFFFFFFFF}
        @h2xmle_default {0xFFFFFFFF}
        @h2xmle_description {Stack size for this container.} */
}
#include "gk_end_pack.h"
;
typedef struct apm_cont_prop_id_stack_size_t apm_cont_prop_id_stack_size_t;


/*--------------------------------------------------------------------------------------------------------------------*/
/**< Enumeration for container property ID Processor Domain */
#define APM_CONTAINER_PROP_ID_PROC_DOMAIN         0x08001014
/** @h2xmlp_property {"Proc Domain", APM_CONTAINER_PROP_ID_PROC_DOMAIN}
    @h2xmlp_description {Container property ID for Processor Domain } */

#include "gk_begin_pack.h"
struct apm_cont_prop_id_proc_domain_t
{
   //TODO:pbm, use the def from another file
   uint32_t proc_domain;
   /**< @h2xmle_rangeList {"DONT_CARE"=0xFFFFFFFF,
                           "MDSP"=APM_PROC_DOMAIN_ID_MDSP,
                           "ADSP"=APM_PROC_DOMAIN_ID_ADSP,
                           "SDSP"=APM_PROC_DOMAIN_ID_SDSP,
                           "CDSP"=APM_PROC_DOMAIN_ID_CDSP}
        @h2xmle_default {0xFFFFFFFF}
        @h2xmle_description {Container processor domain.} */
}
#include "gk_end_pack.h"
;
typedef struct apm_cont_prop_id_proc_domain_t apm_cont_prop_id_proc_domain_t;
/*--------------------------------------------------------------------------------------------------------------------*/
/**< Enumeration for parent container property ID  */
#define APM_CONTAINER_PROP_ID_PARENT_CONTAINER_ID    0x080010CB
/** @h2xmlp_property {"Parent Container ID", APM_CONTAINER_PROP_ID_PARENT_CONTAINER_ID}
    @h2xmlp_description {Container property ID for parent container property ID} */

#include "gk_begin_pack.h"
struct apm_cont_prop_id_parent_container_t
{
   uint32_t parent_container_id;
   /**< @h2xmle_range {0..0xFFFFFFFF}
        @h2xmle_default {0xFFFFFFFF}
        @h2xmle_description {ID of the Offload Container in Master DSP to which
                             the containers in the Satellite DSP are associated.} */
}
#include "gk_end_pack.h"
;
typedef struct apm_cont_prop_id_parent_container_t apm_cont_prop_id_parent_container_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/**< Enumeration for container property Heap ID  */
#define APM_CONTAINER_PROP_ID_HEAP_ID                  0x08001174
/** @h2xmlp_property {"Container Heap", APM_CONTAINER_PROP_ID_HEAP_ID}
    @h2xmlp_description {Container property ID for the Heap ID} */
/**< Enumeration for container Default Heap ID */
#define APM_CONT_HEAP_DEFAULT                     0x1

/**< Enumeration for container Low Power Island (LPI) Heap ID */
#define APM_CONT_HEAP_LOW_POWER                   0x2

#include "gk_begin_pack.h"
struct apm_cont_prop_id_heap_id_t
{
   uint32_t heap_id;
   /**< @h2xmle_rangeList {"Default"=APM_CONT_HEAP_DEFAULT,
                           "Low Power"=APM_CONT_HEAP_LOW_POWER}
        @h2xmle_default {APM_CONT_HEAP_DEFAULT}
        @h2xmle_description {Informs the container about the heap it needs to use.
                             The container re-maps this id to the actaul heap id} */
}
#include "gk_end_pack.h"
;
typedef struct apm_cont_prop_id_heap_id_t apm_cont_prop_id_heap_id_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/** @} <-- End of container_cfg -->*/
/*====================================================================================================================*/
/*====================================================================================================================*/

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* _APM_CONTAINER_API_H_ */
