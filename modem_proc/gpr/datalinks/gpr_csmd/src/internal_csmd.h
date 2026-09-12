/*========================================================================*/
/**
@file internal_csmd.h

@brief This file contains the shared MACRO (opcode) definitions
and structure declarations for csmd operations.
This exposes APIs to the GPR layer.
*/
/*========================================================================
Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================== */
/*========================================================================
Edit History

$Header: L:\Projects\avs.adsp.2.9.Mar8_SIM_2_9_gpr\adsp_proc\avs\main\gpr\datalink\gpr_csmd\inc\internal_csmd.h

when       who        what, where, why
--------   ---       --------------------------------------------------
03/19/18   akr        Created file.
========================================================================== */
#ifndef INTERNAL_CSMD_H
#define INTERNAL_CSMD_H
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/* =======================================================================
INCLUDE FILES
========================================================================== */
#include "ar_osal_types.h"
#include "ar_osal_error.h"
#include "ar_osal_thread.h"
#include "ar_osal_signal.h"
#include "ar_osal_mem_op.h"
#include "ar_osal_string.h"
#include "stdlib.h"
#include "gpr_csmd_i.h"
#include <stdio.h>
/*===========================================================================
MACRO DEFINITIONS
===========================================================================*/
// Transport Mode
#if defined(__qdsp6__)
#define COSIM_TRANSPORT
#else
#define WINSOCK_TRANSPORT
#endif

#ifdef WINSOCK_TRANSPORT
#include "winsock.h"
#define WIN32_LEAN_AND_MEAN
//// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "advapi32.lib")
#endif // WINSOCK_TRANSPORT

/* Uncomment to enable Low Level Logs*/
#define CSMD_DEBUG_LOW

/** Macro to initialize the link identifier structure with default values.
 * It memsets the header to 0 and initializes the header field */
#define CSMD_LINK_ID_STRUCT_INIT(link_id)                                                                              \
   (link_id).xport         = 0;                                                                                        \
   (link_id).remote_ss     = 0;                                                                                        \
   (link_id).link_notifier = 0;

/* Flag for no options */
#define CSMD_TX_NO_OPTIONS (0)
/* Character Length for the xport name */
#define CSMD_XPORT_NAME_LEN 32
/* Character length fo the remote proc */
#define CSMD_REMOTE_SS_NAME_LEN 32
/* Flags indicating the readiness of datalink*/
#define CSMD_NOT_INITIALIZED 0
#define CSMD_INITIALIZED 1

/* Start address of SM where ADSP can write data*/
#define SHARED_MEM_REG_START_ADDR 0x00700000
/* Start address of SM where ADSP can write packets*/
#define SM_DSP2COSIM_PKT_SEND_ADDR 0x0070FC18
/* Start address of SM where ADSP can read from */
#define SM_COSIM2DSP_RANGE_START 0x00710000
/* Size of Shared Memory to create */
#define SHARED_MEM_REG_SIZE 131072 // hex 20000
/* Interrupt Number used*/
#define COSIM2DSP_INTERRUPT_NUM 11
/* Stack size for the thread*/
#define CSMD_STACK_SIZE 1024

#define GPR_CSMD_128_BYTES_BUFFER (128)
#define GPR_CSMD_512_BYTES_BUFFER (512)
#define GPR_CSMD_1024_BYTES_BUFFER (1024)
#define GPR_CSMD_4096_BYTES_BUFFER (4096)

#define GPR_CSMD_NUM_OF_128_BYTES_INTENTS (64)
#define GPR_CSMD_NUM_OF_512_BYTES_INTENTS (32)
#define GPR_CSMD_NUM_OF_1024_BYTES_INTENTS (16)
#define GPR_CSMD_NUM_OF_4096_BYTES_INTENTS (04)

/*----------------------------------------------------------------------------
* Type Declarations
* -------------------------------------------------------------------------*/
/** Enums to identify link state */
typedef enum {
   CSMD_LINK_STATE_UP,  /* Called after specified link is online */
   CSMD_LINK_STATE_DOWN /* Called after SSR (before REMOTE_DISCONNECT) */
} csmd_link_state_type;

/** Data structure holding the link state information */
typedef struct csmd_link_info_t
{
   char *               remote_ss;  /* Remote subsystem name */
   csmd_link_state_type link_state; /* Link state */
} csmd_link_info_type;

/*----------------------------------------------------------------------------
 * Function Declarations
 * -------------------------------------------------------------------------*/
/* Callback function invoked when link goes up/down */
typedef uint32_t (*csmd_link_state_notif_cb)(csmd_link_info_type *link_info); /* Ptr to link information strcuture */

/* Regsiters a client specified callback to be invoked when the specified
 * transport (link) is up/down.*/
uint32_t csmd_init_and_register(uint32_t src_domain, uint32_t channel_id);
/*API to deinit CSMD*/
uint32_t csmd_deinit_and_deregister(uint32_t src_domain, uint32_t channel_id);
/*CB called when Client is up and host gets notification*/
uint32_t gpr_ipc_csmd_link_up_cb(csmd_link_info_type *link_info);
/*Opens the CSMD channel between host and the remote host*/
uint32_t gpr_csmd_open(uint32_t channel_id);
/*Opens the CSMD channel between host and the remote host*/
uint32_t gpr_csmd_close(uint32_t channel_id);
/*Transmit the provided buffer over csmd.*/
uint32_t csmd_tx(uint32_t channel_id, const void *data, uint32_t size);
/*Receive data transmitted over csmd.*/
uint32_t csmd_rx(uint32_t channel_id, void *buf, uint32_t size);

/** Data receive notification callback type*/
typedef void (*csmd_rx_notification_cb)(const void *ptr, /* pointer to the received buffer */
                                        uint32_t    size);
/*Callback that notifies the host that the remote host has disconnected*/
typedef uint32_t (*csmd_notify_link_close_cb)(uint32_t channel_id);
/*Interrupt Context Structure : stores pointers to CB funtions*/
typedef struct isr_ctx
{
   csmd_link_state_notif_cb  ls_cb; /*Link State Open CB*/
   csmd_rx_notification_cb   rx_cb; /*Receive Data CB*/
   csmd_rx_notification_cb   tx_done_cb;
   csmd_notify_link_close_cb link_close_cb; /*Link Close CB*/
   // TODO: populate as we move.
} isr_ctx_t;

/*----------------------------------------------------------------------------
* XPORT Defines
* -------------------------------------------------------------------------*/
#define CSMD_XPORT_DEFAULT_BUFLEN (512000) // winsock receive size
#define CSMD_RX_THREAD_STACK_SIZE 4096
#define CH_RX_QUEUE_BIT 2
#define CH_KILL_SIG_BIT 0
#define CSMD_TX_PORT "22224"
#define CSMD_RX_PORT "22222"
#define CSMD_OB_TX_PORT "22228"
#define CSMD_OB_RX_PORT "22226"
#define CSMD_COSIM_SERVER_ADDRESS "127.0.0.1"
/*----------------------------------------------------------------------------
* XPORT Type Declarations
* -------------------------------------------------------------------------*/

typedef struct xport_global xport_global_t;
/* Transport info global structure declaration*/
#ifdef WINSOCK_TRANSPORT
struct xport_global
{
   ar_osal_thread_t rcv_tid;            /*< Receive Thread ID*/
   ar_osal_thread_t ob_rcv_tid;         /*< OB Receive Thread ID*/
   SOCKET             tx_connect_sock;    /*< Transmit SOCKET*/
   SOCKET             rx_connect_sock;    /*< Receive SOCKET*/
   SOCKET             ob_tx_connect_sock; /*<OB Transmit SOCKET*/
   SOCKET             ob_rx_connect_sock; /*<OB Receive SOCKET*/
   const char *       xport_port_tx;      /*< Transmit Port*/
   const char *       xport_port_rx;      /*< Receive Port*/
   const char *       ob_xport_port_tx;   /*< OB Transmit Port*/
   const char *       ob_xport_port_rx;   /*< OB Receive Port*/
   const char *       xport_port_address; /*< Port IP Address*/
};
#endif // WINSOCK_TRANSPORT

#ifdef COSIM_TRANSPORT
struct xport_global
{
   ar_osal_signal_t p_rx_signal; /*< Receive Signal*/
   uint32_t           signal_mask;
   ar_osal_thread_t rcv_tid; /*< Receive Thread ID*/
};
#endif // COSIM_TRANSPORT

/*----------------------------------------------------------------------------
* Function Prototypes
* ------------------------------------------------------------------------- */
/**
Creates and initializes the Mode of transport (Shared Memory or Sockets and ports)
Also initializes signals, channels and spawns RX thread
*/
uint32_t xport_init(void);
/**
Deregisters Mode of transport (Shared Memory)
Also destroys signals, channels RX thread
*/
uint32_t xport_deinit(void);
/**
Transmit data via Shared memory to the other host/IPC Manager
*/
uint32_t xport_send(void *pkt_ptr, uint32_t pkt_size, void *data_ptr, uint32_t data_size);
/**
 Receive data via Shared memory into RX intent from the client/IPC Manager
*/
uint32_t xport_receive(uint32_t channel_id, void *buf, uint32_t size);

/*----------------------------------------------------------------------------
* CSMD Context Structure Type Definition
* -------------------------------------------------------------------------*/
typedef uint32_t csmd_state_t;
typedef struct csmd_ctx
{
   volatile csmd_state_t state[GPR_CSMD_MAX_PORTS];             /*< State of the channel*/
   isr_ctx_t             global_ipc_cb_table;                   /*< Table with stored CBs*/
   csmd_link_info_type   link_notif_params[GPR_CSMD_MAX_PORTS]; /*< Holds link state info*/
   xport_global_t        transport_info_t;                      /*< Transport info structure*/
   volatile bool_t       open_cb_done[GPR_CSMD_MAX_PORTS];      /*< boolean to denote the channel open*/
   bool_t                induced_crash;                         /*< boolean to denote assert(0) - need to debug*/
} csmd_ctx_t;

/*----------------------------------------------------------------------------
 * LPM Function Declarations
 * -------------------------------------------------------------------------*/
#ifdef COSIM_TRANSPORT
/*Registers and initializes physical shared mem with the ADSP*/
uint32_t csmd_lpm_sm_register(void);
/*Deregisters a portion of Physical memory with the ADSP*/
uint32_t csmd_lpm_sm_deregister(void);
/*Makes a write to l2vic registers to clear the level interrupt*/
void clear_level_interrupt(void);
#endif // COSIM_TRANSPORT

#ifdef __cplusplus
}
#endif //__cplusplus
#endif /* INTERNAL_CSMD_H */
