/*
===========================================================================

FILE:         ipa_emu_mgr.c

$Header:

===========================================================================

===========================================================================
Copyright © 2017-2018 Qualcomm Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
===========================================================================
*/

/* -----------------------------------------------------------------------
**                           INCLUDES
** ----------------------------------------------------------------------- */
#include "ipa_emu_mgr.h"
#include "qtf.h"
#include "rcinit.h"

extern "C"{
	ipa_emu_mgr_s ipa_emu_mgr;
}

void ipa_emu_mgr_timer_task_main
(
	dword dummy
	)
{
	(void)dummy;

	/* Synchronize thread with tmc/Rcinit startup */
	rcinit_handshake_startup();

	while (!ipa_emu_mgr.ipa_emu_mgr_kill_timer)
	{
		TF_SLEEP(1);
	}
}