#ifndef REMOTE_APDU_H
#define REMOTE_APDU_H

/*
 * $Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/remote_apdu.h#1 $
 *
 * This class represents the command structure between clients and the shim.  This
 * interface is implemented over TCP and the client is currently written in C.  The
 * structure here depends on the C program so any changes to either should be reflected
 * accordingly in the other ie - if you change it here change the C header file if you
 * change the C header file change it here.
 */

#include "jc_spcom_msg.h"

typedef enum {
  APDU_CMD,
  APDU_RSP,
  RST_CMD,
  RST_RSP,
  PPS_CMD,
  PPS_RSP,
  ERR
}remote_cmd_type;


typedef struct {
  char pps_bytes[6];
}pps_cmd_type;

typedef struct {
  char pps_bytes[6];
}pps_rsp_type;


typedef union {
  apdu_cmd_type  apdu_cmd;
  apdu_rsp_type  apdu_rsp;
  //reset_cmd_type reset_cmd;
  reset_rsp_type reset_rsp;
  pps_cmd_type   pps_cmd;
  pps_rsp_type   pps_rsp;
  error_type     error;
}remote_cmd_body;

typedef struct {
  remote_cmd_type header;
  int             length;
  remote_cmd_body body;
}remote_card_cmd_type;


#endif /* REMOTE_APDU_H */
