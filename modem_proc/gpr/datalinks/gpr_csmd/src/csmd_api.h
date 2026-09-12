/*========================================================================*/
/**
@file csmd_api.h

@brief Shared Memory API: This file contains the shared MACRO (opcode) definitions
and structure declarations for the packet and payload across xport types.
*/
/*========================================================================
Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================== */
/*========================================================================
Edit History

$Header: L:\Projects\avs.adsp.2.9.Mar8_SIM_2_9_gpr\adsp_proc\avs\main\gpr\inc\csmd_api.h

when       who        what, where, why
--------   ---       --------------------------------------------------
03/19/18   akr      Created file.
========================================================================== */
#ifndef CSMD_API_H
#define CSMD_API_H
/* =======================================================================
INCLUDE FILES
========================================================================== */
#include "ar_osal_types.h"
/** @addtogroup proc_ids
@{ */
/** proc ID corresponding to LPASS */
#define CSMD_ID_LPASS_PROC 0x0000
/** proc ID corresponding to apps processor*/
#define CSMD_ID_APSS_PROC 0x0001
/** proc ID corresponding to modem processor*/
#define CSMD_ID_MPSS_PROC 0x0002

/** @} */ /* end_addtogroup proc_ids */
/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/
typedef struct csmd_pkt csmd_pkt_t;
/*General CSMD Packet Information header structure declaration*/
struct csmd_pkt
{
   uint32_t opcode;     /*<Command OPCODE supported values below*/
   uint32_t size;       /*< Size of the payload that follows this packet header */
   uint32_t channel_id; /*< Channel ID - the unique number that identifies the link between two processors
      @values
      - opcode supports all among the group @csmd_commands_and_events_opcodes
      - size size of the payload structure that follows the csmd_pkt_t header
      - channel_id supports the domain IDs defined in gpr_csmd_i.h (1-3)*/
};

/** @addtogroup csmd_commands_and_events_opcodes
@{ */
/*This group contains three data types
- command  - CSMD_CMD_xxxx
- response - CSMD_RSP_xxxx
- event    - CSMD_EVENT_xxxx*/
/*
Command to Register Host Information with IPC Manager
*/
#define CSMD_CMD_REG_HOST 0x101
/*
Command to De-Register Host Information with IPC Manager
*/
#define CSMD_CMD_DEREG_HOST 0x102
/* Structure for the Payload of the CSMD_CMD_REG_HOST and CSMD_CMD_DEREG_HOST,
   which informs the remote proc via the CSMD_COSIM about their state */
typedef struct csmd_cmd_reg_dereg_host csmd_cmd_reg_dereg_host_t;

struct csmd_cmd_reg_dereg_host
{
   uint64_t src_proc_id; /*< Native Proc's id */
   uint64_t dst_proc_id; /*< Remote Proc's id */
   /*@values

   - CSMD_ID_LPASS_PROC
   - CSMD_ID_APSS_PROC
   - CSMD_ID_MPSS_PROC*/
};
/* The reason the members of the above struct are chosen to be of 8 bytes each
The load size is 8 bytes - when something is written to the shared memory region,
the cosim callbacks are triggered for every 8 bytes (if the size is greater than or equal to
8 bytes) or every byte (if less than 8 bytes). since the registration is a one time thing,
we want the callback to be triggered in one shot for the hostcodes.*/
/*
Command sent by the CSMD_COSIM to hosts after registration,  indicating the client is active
*/
#define CSMD_EVENT_CLIENT_UP_NOTIFICATION 0x103
/*@values There is no payload*/
/*
Command sent to hosts after registration indicating the client is DOWN/Disconnected
*/
#define CSMD_EVENT_CLIENT_DOWN_NOTIFICATION 0x104
/*@values There is no payload*/
/*
Command to identify GPR data packets from host/remote proc
*/
#define CSMD_CMD_DATA_PACKET 0x105
/*@values The data buffer that follows the csmd_pkt_t header with this opcode
is the actual data payload of type gpr_packet_t*/
/*
Command Acknowledging reception of any data from the COSIM/IPC Manager
*/
#define CSMD_RSP_IBASIC_RESULT 0x106
/*@values There is no payload*/
/*
Command issued to the COSIM to bring in data from the OB chunk on the remote proc
*/
#define CSMD_CMD_INVALIDATE_REGION 0x107
/* @values payload of type csmd_cmd_invalidate_flush_region_t*/
/*
Command issued to the COSIM to update data on the OB chunk on the remote proc
*/
#define CSMD_CMD_FLUSH_REGION 0x108
/* @values payload of type csmd_cmd_invalidate_flush_region_t*/

/* Structure for the Payload of the CSMD_CMD_INVALIDATE_REGION and CSMD_CMD_FLUSH_REGION,
   which brings in a chunk of out of band memory or updates it, respectively*/
typedef struct csmd_cmd_invalidate_flush_region
{
   uint64_t src_phy_addr;  /*<physical address in the mirror mem on the src proc*/
   uint64_t dst_phy_addr;  /*<physical address on the destination proc - Not used Today*/
   uint64_t src_virt_addr; /*<virtual address in the src proc mirror memory - Not used Today*/
   uint64_t dst_virt_addr; /*<virtual address in the OB memory of the dst proc */
   uint32_t size;          /*<Size of the OB memory chunk to invalidate/flush*/
   uint32_t reserved;      /*alignment*/
} csmd_cmd_invalidate_flush_region_t;
/*
Command issued by the COSIM requesting data from the OB chunk on the remote proc
*/
#define CSMD_CMD_DATA_READ 0x109
/* @values payload of type csmd_data_read_write_t*/
/*
Command issued by the COSIM to write data into the OB chunk on the remote proc
*/
#define CSMD_CMD_DATA_WRITE 0x10A
/* @values payload of type csmd_data_read_write_t
   The actual data buffer (void*) is appended after the payload header
   of said type csmd_data_read_write_t for this opcode*/

/* Structure for the Payload of the CSMD_CMD_DATA_READ and CSMD_CMD_DATA_WRITE,
   which brings in a chunk of out of band memory or updates it, respectively
   from the CSMD_COSIM context*/
typedef struct csmd_data_read_write
{
   uint64_t address;  /*<Remote understandable address*/
   uint32_t size;     /*<Size of the requested/to be updated memory chunk*/
   uint32_t reserved; /*alignment*/
} csmd_data_read_write_t;
/** @} */ /* end_addtogroup csmd_commands_and_events_opcodes */
#endif    // CSMD_API_H