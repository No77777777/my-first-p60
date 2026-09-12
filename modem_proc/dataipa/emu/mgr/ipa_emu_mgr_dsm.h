#ifndef IPA_EMU_MGR_DSM_H
#define IPA_EMU_MGR_DSM_H

/*
===========================================================================

FILE:         ipa_emu_mgr_dsm.h

$Header:

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/27/17   nl      Added MOB and off-target IPA emulation support to DSM
===========================================================================

===========================================================================
Copyright © 2017 Qualcomm Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
===========================================================================
*/

/* -----------------------------------------------------------------------
**                           INCLUDES
** ----------------------------------------------------------------------- */

#include "VVDRV_deo_mempool.h"

/* -----------------------------------------------------------------------
**                           Functions
** ----------------------------------------------------------------------- */
/*
* C++ wrapper
*/
#ifdef __cplusplus
extern "C" {
#endif

	INLINE void* ipa_emu_mgr_dsm_alloc(size_t size)
	{
		return VVDRV_deo_alloc(size, M_ZERO);
	}

#ifdef __cplusplus
}
#endif

#endif /* IPA_EMU_MGR_H */