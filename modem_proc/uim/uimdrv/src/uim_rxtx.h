#ifndef UIM_RXTX_H
#define UIM_RXTX_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  UIM RX/TX HEADER

GENERAL DESCRIPTION
  This header defines the external interface for the uim_rxtx module.

EXTERNALIZED FUNCTIONS

  uim_tx
    Sends outgoing bytes to the UIM HW interface.

Copyright (c) 2001-2016 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_rxtx.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/14   ak      Created

===========================================================================*/

/**
*  This function sends outgoing bytes to the UIM interface.
*
*  
*  @param buffer A pointer to an array of bytes to send.
*  @param size The number of bytes to send
*  @param uim_ptr Pointer to the current instance of the UIM
*                 global structure 
*/
void uim_tx(unsigned char *buffer,
            int size,
            uim_instance_global_type *uim_ptr);

#endif  /* UIM_RXTX_H */
