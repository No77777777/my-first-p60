#ifndef SECSSLDCH_H
#define SECSSLDCH_H

#include "sec_variation.h"
/*==========================================================================
               SSL Dynamic certificate handler (DCH)

GENERAL DESCRIPTION
	Deals with retrieval of certificates from AP if match was not found on EFS
==========================================================================*/

/*==========================================================================

when       who     what, where, why
--------   ---     ---------------------------------------------------------
01/23/18   ggrewal  Initial check-in
==========================================================================*/


#include <stdlib.h>
#include "stdio.h"
#include "secdiag.h"
#include "secerrno.h"
#include "UxCommon.h"

#include "rex.h"
#include "ds_sys_ioctl.h"

#include "secssltask.h"

#define SECSSLDCH_RETRIEVAL_TIMER_SECS	5

void secssl_dch_ds_sys_ca_cert_callback
(
  uint8                                   cert_dn[DS_SYS_CERT_DN_MAX],
  uint8                                   cert_dn_length,
  ds_sys_ca_cert_result_enum_type         result,
  q_type                                  *ca_cert_q_ptr,
  void                                   *user_data_ptr
);

secerrno_enum_type secssl_dch_trigger_cert_fetch_from_ap(
   uint8* cert_dn,
   uint8  cert_dn_length
);

secerrno_enum_type secssl_dch_pop_next_cert_from_cert_q(uint8** cert_buffer, uint16* cert_buffer_size);

void secssl_dch_clear_certs_from_cert_q();

#endif /* SECSSLDCH_H */