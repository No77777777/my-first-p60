#ifndef DIAGPKTI_H
#define DIAGPKTI_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   Diagnostics Packet Processing

General Description
  Diagnostic packet processing internal declarations.

Copyright (c) 2007-2016, 2018 by Qualcomm Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                           Edit History

$Header: //components/rel/core.mpss/10.0/services/diag/LSM/qurt/src/diagpkti.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/23/18   gn      Command registration enhancement
07/25/16   gn      Unused Public API removal
08/26/16   sp      Cleanup unused mutex. Append master_tbl_cs and related function with _dcm to be meaningful
04/11/16   ph      Added support for QShrink 4.0
07/10/15   rh      Moved command header structs to diagpkti.h
03/23/15   rh      Fixed compiler warnings.
02/11/15   rh      Added client_id to diagpkt_get_delayed_rsp_id_dcm()
07/30/14   is      Do cleanup to support PD Restart
10/10/12   sg      Changed the return type of diag_pkt_commit_dcm()
07/05/12   sg      Changes to bring up Diag MultiPD
04/20/10   sg      Moved diag_client_maskchange() to diag_v.h
07/07/09   mad     Removed inclusion of qw.h, included queue.h
07/07/09   mad     Moved structure definitions here from diagi.h, to 
                   maintain commonality of diagi.h
04/24/09   mad     Added declaration for diagpkt_get_delayed_rsp_id_dcm().
01/15/09   mad     Removed declaration of Mutex that protects global Rx buffer
12/02/08   mad     Introduced some function declarations etc.
                   which are shared between different C files.
===========================================================================*/
#include "comdef.h"
#include "diag.h"
#include "diagdiag_v.h"
#include "queue.h" /* for q_link_type */
#include "qurt_signal.h"

typedef struct
{
  uint8 command_code;
}
diagpkt_hdr_type;

typedef struct
{
  uint8 command_code;
  uint8 subsys_id;
  uint16 subsys_cmd_code;
}
diagpkt_subsys_hdr_type;

typedef PACK(struct)
{
  unsigned int pattern;     /*!< Pattern to check validity of committed pointers. */
  unsigned int size;        /*!< Size of usable buffer diagpkt_q_type->pkt */
  unsigned int length;      /*!< Size of packet */
  byte pad[3];              /*!< Padding for alignment */
  byte cmd_req_code;        /*!< DCI cmd req code */
  unsigned int cmd_tag;     /*!< Tag to identify the client that sent the request */
  byte pkt[1];           /*!< Sized by 'length' field. */
}
diagpkt_rsp_type;

typedef struct
{
  diag_cmd_rsp rsp_func; /* If !NULL, this is called in lieu of comm layer */
  void *rsp_func_param;

  diagpkt_rsp_type rsp; /* see diagi.h */
}
diagpkt_lsm_rsp_type;

typedef struct
{
   boolean pending;
   diag_cmd_rsp rsp_func;
   void *rsp_func_param;
} diagpkt_rsp_func_store_type;

typedef struct
{ 
  uint32 client_id;
  int qdi_handle;
  qurt_signal_t *LSM_obj;
} diagpkt_mask_client_tbl_type;

typedef struct
{
  q_link_type link;
  diagpkt_mask_client_tbl_type mask_client;
} diagpkt_mask_client_q_type;

q_type diagpkt_mask_client_list_q;

typedef struct
{ 
  uint16 cmd_code;
  word subsysid;
  uint16 proc_id;
  word count;
  word cmd_code_lo;
  word cmd_code_hi;
  qurt_signal_t * LSM_obj;
  uint32 client_id;
  int qdi_handle;
} diagpkt_dcm_master_table_type;

typedef struct
{
  q_link_type link;
  diagpkt_dcm_master_table_type* master_tbl_entry;
  uint16 fwd_status;
}diagpkt_master_table_q_type;

q_type diagpkt_master_table_q;


void diagpkt_table_q_init(void);

void diagpkt_fwd_cmd_dereg(void);

void diagpkt_delayrspid_mutex_init(void);

/* Functions used by windiag. */
int diagpkt_commit_dcm (int client_id,PACKED byte *ptr);
boolean diagpkt_get_delayed_rsp_id_dcm(int client_handle, byte* out_buf_ptr, unsigned long outbuf_max_bytes, unsigned long* pbytes_written);

/*===========================================================================

FUNCTION DIAGPKT_RSP_Q_INIT

DESCRIPTION
  This function initializes the response queue.

===========================================================================*/
void diagpkt_rsp_q_init (void);
/*!
@ingroup packet_service

   @brief
   Gets the response Id count.

   @param [in] wrap_flag Wrap Flag

   @dependencies
   none    

   @sideeffects
   None

   @return 
   <ul>
   <li>  Response ID count 
   </ul>
   @sa 
   None   

*/
uint32 getWrapcount(uint16 wrap_flag);


/*! 
   This macro deregisters the client's dispatch table with the diagnostics packet dispatching service 
*/
  #define DIAGPKT_DISPATCH_TABLE_DEREGISTER(xx_subsysid, xx_entry) \
    do { \
      static diagpkt_master_table_type xx_entry##_table = { \
       0, 0xFF, xx_subsysid, sizeof (xx_entry) / sizeof (xx_entry[0]), DIAG_NO_PROC, 0, DIAGCOMM_CTRL_NO_PORT, xx_entry \
      }; \
       /*lint -save -e717 */ \
      diagpkt_tbl_dereg (&xx_entry##_table); \
    } while (0)
     /*lint -restore */

/*! 
   This macro deregisters the client's dispatch table with the packet dispatching service for subsystem
   dispatch version 2 (cmd code 128) 
*/
    #define DIAGPKT_DISPATCH_TABLE_DEREGISTER_V2(xx_cmdcode, xx_subsysid, xx_entry) \
    do { \
      static diagpkt_master_table_type xx_entry##_table = { \
        0, xx_cmdcode, xx_subsysid, sizeof (xx_entry) / sizeof (xx_entry[0]), DIAG_NO_PROC, 0, DIAGCOMM_CTRL_NO_PORT, xx_entry \
      }; \
       /*lint -save -e717 */ \
      diagpkt_tbl_dereg (&xx_entry##_table); \
    } while (0)
     /*lint -restore */ 

/*!
   This macro deregisters the client's dispatch table with the packet dispatching service for subsystem
   dispatch version 2 (cmd code 128) with delayed response 
*/
  #define DIAGPKT_DISPATCH_TABLE_DEREGISTER_V2_DELAY(xx_cmdcode, xx_subsysid, xx_entry) \
    do { \
      static diagpkt_master_table_type xx_entry##_table = { \
        1, xx_cmdcode, xx_subsysid, sizeof (xx_entry) / sizeof (xx_entry[0]), DIAG_NO_PROC, 0, DIAGCOMM_CTRL_NO_PORT, xx_entry \
      }; \
       /*lint -save -e717 */ \
      diagpkt_tbl_dereg (&xx_entry##_table); \
    } while (0)
     /*lint -restore */ 

/*! 
   Use this macro to deregister client's dispatch table with the diagnostics
   packet dispatching service on a particular processor
*/ 
  #define DIAGPKT_DISPATCH_TABLE_DEREGISTER_PROC(xx_proc_id, xx_subsysid, xx_entry) \
    do { \
      static const diagpkt_master_table_type xx_entry##_table = { \
       0, 0xFF, xx_subsysid, sizeof (xx_entry) / sizeof (xx_entry[0]), xx_proc_id, 0, DIAGCOMM_CTRL_NO_PORT, xx_entry \
      }; \
       /*lint -save -e717 */ \
      diagpkt_tbl_dereg (&xx_entry##_table); \
    } while (0)
     /*lint -restore */ 

/*! 
   Use this macro to deregister your dispatch table with the diagnostics
   packet dispatching service version 2 on a particular processor
*/
  #define DIAGPKT_DISPATCH_TABLE_DEREGISTER_V2_PROC(xx_proc_id, xx_cmdcode, xx_subsysid, xx_entry) \
    do { \
      static const diagpkt_master_table_type xx_entry##_table = { \
        0, xx_cmdcode, xx_subsysid, sizeof (xx_entry) / sizeof (xx_entry[0]), xx_proc_id, 0, DIAGCOMM_CTRL_NO_PORT, xx_entry \
      }; \
       /*lint -save -e717 */ \
      diagpkt_tbl_dereg (&xx_entry##_table); \
    } while (0)
     /*lint -restore */ 

/*! 
   Use this macro to deregister your dispatch table with the diagnostics
   packet dispatching service version 2 on a particular processor
   This macro is used if the diag packet has delayed response 
*/
  #define DIAGPKT_DISPATCH_TABLE_DEREGISTER_V2_DELAY_PROC(xx_proc_id, xx_cmdcode, xx_subsysid, xx_entry) \
    do { \
      static const diagpkt_master_table_type xx_entry##_table = { \
        1, xx_cmdcode, xx_subsysid, sizeof (xx_entry) / sizeof (xx_entry[0]), xx_proc_id, 0, DIAGCOMM_CTRL_NO_PORT, xx_entry \
      }; \
       /*lint -save -e717 */ \
      diagpkt_tbl_dereg (&xx_entry##_table); \
    } while (0)
     /*lint -restore */ 

#endif /* DIAGPKTI_H */
