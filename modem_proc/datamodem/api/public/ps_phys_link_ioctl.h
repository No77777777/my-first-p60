#ifndef PS_PHYS_LINK_IOCTL_H
#define PS_PHYS_LINK_IOCTL_H

/*===========================================================================

                          P S _ P H Y S _ L I N K _I O C T L. H

DESCRIPTION

  The Protocol Services interface file. This contains variables and
  definitions used by the various modules to access interface definitions.

Copyright (c) 2004-2008 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE


  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ps_phys_link_ioctl.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/14/08    pp     Common Modem Interface: Public/Private API split.
07/18/08    dm     Fixed compiler warnings
05/03/05    msr    Changed signature of ps_phys_link_ioctl() and moved all
                   phys link IOCTLs here
04/17/05    msr    Removed QOS definitions.
06/29/04    usb    Created file
===========================================================================*/

#include "comdef.h"

#include "ps_iface_defs.h"



/*===========================================================================

                         PHYS LINK IOCTL Definitions

===========================================================================*/
/*---------------------------------------------------------------------------
  Definition of various ps_phys_link control operations. Please do NOT change
  the values of the existing enumerated types when adding new ones (to ensure
  that object code released with previous versions of the code do not break).
  Each of the following control operations have associated tyepdef (with
  corresponding names) that are passed in or returned for get/set operations.

  The most significant bit of the enum value MUST be set for all these
  IOCTLs. This is used as a distinguishing factor between ps_iface, ps_flow
  and phys_link releated IOCTLs.

  Note:
    GET operations are assigned even numbers
    SET operations are assigned odd numbers.

    Also ensure that symetric operations are grouped.
---------------------------------------------------------------------------*/
#define PS_PHYS_LINK_IOCTL_MASK      0x20000000

typedef enum
{
  PS_PHYS_LINK_IOCTL_GO_ACTIVE              = 0x20000013, /* Go active from
                                                                  dormant    */
  PS_PHYS_LINK_IOCTL_GO_DORMANT             = 0x20000014, /* Go dormant      */
  PS_PHYS_LINK_IOCTL_GET_STATE              = 0x20000018, /* Get physical link
                                                           state             */
  PS_PHYS_LINK_IOCTL_GET_DORMANCY_INFO_CODE = 0x20000020, /* Get dormancy info
                                                 code when phys link is down */
  PS_PHYS_LINK_IOCTL_GET_CALL_ID            = 0x20000022,   /*Get call id */
  PS_PHYS_LINK_IOCTL_ABORT_GO_DORMANT       = 0x20000023,   /*Abort previously
                                                     issued dormancy request */ 
  PS_PHYS_LINK_IOCTL_RAN_ASST_MEDIA_ADAPT_REG       = 0x20000024,  
  /* Register for RAN assisted media adaptation information */
  PS_PHYS_LINK_IOCTL_RAN_ASST_MEDIA_ADAPT_DEREG     = 0x20000025,  
   /* Deregister for RAN assisted media adaptation information */
  PS_PHYS_LINK_IOCTL_RAN_ASST_MEDIA_ADAPT_UPDATE    = 0x20000026,  
   /* Update RAN assisted media adaptation information */
  PS_PHYS_LINK_IOCTL_GET_RAN_ASSIT_MEDIA_ADAPT_INFO = 0x20000027,
   /* Retrieve RAN assisted media adaptation information */
#ifdef FEATURE_DATA_FLOW_MGMT
  PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK                   = 0x20000028,
  /* Acknowledge flow control*/
  PS_PHYS_LINK_IOCTL_FLOW_CTL                       = 0x20000029, 
 /* Request to flow control physlink*/
  PS_PHYS_LINK_IOCTL_GET_NUM_ALLOWED_BYTES          = 0x20000030, 
 /* Request to get allowed number of bytes*/
#endif /* FEATURE_DATA_FLOW_MGMT */

} ps_phys_link_ioctl_type;

typedef ps_phys_link_bit_rate_info_type ps_phys_link_ioctl_ran_asst_media_adapt_update_type;

#ifdef FEATURE_DATA_FLOW_MGMT
/*---------------------------------------------------------------------------
  Phy Link flow enable/disable enum
---------------------------------------------------------------------------*/
typedef enum
{
  
  PS_PHYS_LINK_IOCTL_FLOW_ENABLE  = 0x1,
  PS_PHYS_LINK_IOCTL_FLOW_DISABLE = 0x2

}ps_phy_link_ioctl_flow_ctl_enum_type;

/*---------------------------------------------------------------------------
  Phy Link flow enable/disable structure
---------------------------------------------------------------------------*/
typedef struct 
{
  ps_phy_link_ioctl_flow_ctl_enum_type         flow_ctl_req;
  uint64                                       flow_mask;
} ps_phys_link_ioctl_flow_ctl_type;

/*---------------------------------------------------------------------------
  Number of bytes are allowed to transmit 
---------------------------------------------------------------------------*/
typedef struct 
{
  uint32                                  num_of_bytes;
  ps_phys_link_data_traffic_enum_type     traffic_type;
} ps_phys_link_ioctl_allowed_num_bytes_type;

/*---------------------------------------------------------------------------
  Phy Link flow control ACK enum
---------------------------------------------------------------------------*/
typedef enum
{
  
  PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK_LAST_BYTE   = 0x1,
  PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK_THRESHOLD   = 0x2,
  PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK_RESERVED_1  = 0x4,
  PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK_RESERVED_2  = 0x8,
  PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK_RESERVED_3  = 0x10,
  PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK_RESERVED_4  = 0x20,
  PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK_RESERVED_5  = 0x40,
  PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK_MAX         = 0x80
}ps_phy_link_ioctl_flow_ctl_ack_enum_type;


/*---------------------------------------------------------------------------
  Flow control ACK confirmation (It is 1 byte field)
---------------------------------------------------------------------------*/
typedef struct 
{
  uint8                                        ack;
  
} ps_phys_link_ioctl_flow_ctl_ack_type;



#endif /* FEATURE_DATA_FLOW_MGMT */

#ifdef __cplusplus
extern "C" {
#endif
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                           EXTERNALIZED FUNCTIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*===========================================================================
FUNCTION PS_PHYS_LINK_IOCTL()

DESCRIPTION
  This function performs various operations on the given phys link.
  Typically, these operations are to get or set a value.

DEPENDENCIES
  None.

PARAMETERS

  phys_link_ptr             - Phys link on which the specified operations
                              is to be performed

  ioctl_name                - The operation name

  argval_ptr                - Pointer to operation specific structure

  ps_errno                   - Error code returned in case of failure (Error
                               values are those defined in dserrno.h)

                              DS_EINVAL - Returned when the specified IOCTL
                              does not belong to the common set of IOCTLs
                              and there is no IOCTL mode handler registered
                              for the specified interface.

                              DS_EOPNOTSUPP - Returned by the lower level
                              IOCTL mode handler when specified IOCTL is not
                              supported by the interface. For instance, this
                              would be returned by interfaces that do not
                              support a certain "iface specific common IOCTL"
                              (i.e. these are common IOCTLs, but the
                              implementation is mode specific)

                              DS_EFAULT - This error code is returned if the
                              specified arguments for the IOCTL are correct
                              but an error is encountered while executing
                              the IOCTL.

                              DS_NOMEMORY - This error code is returned if we
                              run out of mempory buffers during execution.

RETURN VALUE
  0 - on success
  -1 - on failure

SIDE EFFECTS
  None.

===========================================================================*/
int ps_phys_link_ioctl
(
  ps_phys_link_type        *phys_link_ptr,
  ps_phys_link_ioctl_type   ioctl_name,
  void                     *argval_ptr,
  sint15                   *ps_errno
);

#ifdef __cplusplus
}
#endif

#endif /* PS_PHYS_LINK_IOCTL_H */
