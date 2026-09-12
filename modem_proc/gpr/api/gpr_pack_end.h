
/**
 * \file gpr_pack_end.h
 * \brief 
 *  	This file contains GPR pack end
 *  
 * 
 * \copyright
 *  Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
// clang-format off
/*
$Header: //commercial/MPSS.HA.1.1/Main/modem_proc/gpr/api/gpr_pack_end.h#1 $
*/
// clang-format on


#if defined( __qdsp6__ )
/* No packing atrributes for Q6 compiler; all structs manually packed */
#elif defined( __GNUC__ )
  __attribute__((packed));
#elif defined( __arm__ )
#elif defined( _MSC_VER )&& !defined(_KERNEL_MODE)
  #pragma pack( pop )
#elif defined (__H2XML__)
  __attribute__((packed))
#elif defined( _KERNEL_MODE)
#else
  #error "Unsupported compiler."
#endif /* __GNUC__ */

