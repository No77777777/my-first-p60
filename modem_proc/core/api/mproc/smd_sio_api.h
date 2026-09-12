#ifndef SMD_SIO_API_H
#define SMD_SIO_API_H
/*===========================================================================

     Shared Memory Driver SIO Interface Public Header File
     *DEPRECATED*

     Copyright (c) 2013, 2018 QUALCOMM Technologies Incorporated.
     All rights reserved.
     Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/api/mproc/smd_sio_api.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/22/13   bm      Expose API to get SIO port ID given SMD port name.
===========================================================================*/

/*===========================================================================

                        INCLUDE FILES

===========================================================================*/
#include "glink_sio_api.h"

/*===========================================================================
FUNCTION      smd_sio_get_sio_id_from_name *DEPRECATED*

DESCRIPTION   Get SIO port ID for a given SMD port name
              Deprecated, use glink_sio_get_sio_id_from_name instead by 
              including glink_sio_api.h
  
DEPENDENCIES  None

ARGUMENTS     const char * smd_port_name
               SMD port name (string)

RETURN VALUE  SIO port ID for the given SMD port name
              SIO_PORT_NULL : Error
   
SIDE EFFECTS  None
===========================================================================*/
#define smd_sio_get_sio_id_from_name(name) glink_sio_get_sio_id_from_name(name)

#endif /* SMD_SIO_API_H */
