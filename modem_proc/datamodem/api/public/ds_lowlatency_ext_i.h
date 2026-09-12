/*===========================================================================

                       DS_LOWLATENCY_EXT_I.H
DESCRIPTION


EXTERNALIZED FUNCTIONS

 Copyright (c) 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE
 
  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ds_lowlatency_ext_i.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who          what, where, why
--------   ------     ----------------------------------------------------------
07/25/17   youjunf      Initial version
===========================================================================*/

#ifndef DS_LOWLATENCY_EXT_I_H
#define DS_LOWLATENCY_EXT_I_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "sys.h"


/*==============================================================================
FUNCTION      DS_LOW_LATENCY_IPA_REDUCE_AGGR_CB_FCN_TYPE

DESCRIPTION   Callback function to notify ipa about reducing downlink aggregation timer or not.

PARAMETERS 
              subs_id               - Subscription ID     
              reduce_aggr_info_ptr  - detailed info 


DEPENDENCIES  None
==============================================================================*/
typedef void (*ds_low_latency_ipa_reduce_aggr_cb_fcn_type)
(                                          
  sys_modem_as_id_e_type subs_id,
  boolean                invoke_reduce
);

/*===========================================================================
                   EXTERNAL FUNCTIONS
===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif


/*===========================================================================
FUNCTION      DS_LOW_LATENCY_IPA_REG_REDUCE_AGGR_EV

DESCRIPTION   Called by ipa to register to get downlink aggregation timer reduce notification.

DEPENDENCIES  None.

RETURN VALUE  TRUE: if registration successful

              FALSE: if registration unsuccessful

SIDE EFFECTS                                                                    
===========================================================================*/
boolean ds_low_latency_ipa_reg_reduce_aggr_ev
(
  ds_low_latency_ipa_reduce_aggr_cb_fcn_type  cb_fcn_ptr
);


#ifdef __cplusplus
}
#endif
#endif /* DS_LOWLATENCY_EXT_I_H */





