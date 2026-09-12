#ifndef PD_MON_QURT_H
#define PD_MON_QURT_H
/** vi: tw=128 ts=3 sw=3 et
@file pd_mon_qurt.h
@brief This file contains the API details for the Protection Domain Monitor, API 2.0
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2015 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Confidential and Proprietary
===============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/api/debugtools/pd_mon_qurt.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

#include "tms_dll_api.h"                                                         /**< MUST PRECEDE CLIENT DEFINITION OF DLL_API_EXPORT */

#if defined(DLL_API_BUILD_PD_MON)                                                /**< REQUIRED TO BE DECLARED IN TECH AREA SOURCE MODULES (SU/VU VISIBLE) */
#undef  DLL_API_EXPORT_PD_MON                                                    /**< REDEFINITION FOR THIS SPECIFIC MODULE */
#define DLL_API_EXPORT_PD_MON DLL_API_EXPORT_HELPER                              /**< CONTROL OVER DLL SYMBOL VISIBILITY BY EXPORT */
#else
#undef  DLL_API_EXPORT_PD_MON                                                    /**< REDEFINITION FOR THIS SPECIFIC MODULE */
#define DLL_API_EXPORT_PD_MON DLL_API_IMPORT_HELPER                              /**< CONTROL OVER DLL SYMBOL VISIBILITY BY EXPORT */
#endif

/**

Requirements

1. The Deliverable shall be partitioned as drop in Build Time components.

2. The Deliverable dependency shall be partitioned between ROOT PD and USER PD
   depending upon Deliverable technology requirements.

3. The Deliverable shall be launched by software supplied by the Deliverable
   Owner. CORE supplied launching mechanisms shall be used with the RCINIT Framework.

Policy

1. Policy shall be to Initialize CORE related User PD required ROOT Drivers
   in RCINIT Framework Groups by CORE [ (ROOT|USER)/RCINIT_GROUP_0 .. (ROOT|USER)/RCINIT_GROUP_1 ]

2. Policy shall be to Allow for User PD launching to occur from ROOT Domain no sooner
   than ROOT/RCINIT_GROUP_2, following CORE dependency. Recommended launching of User PD
   is from ROOT/RCINIT_GROUP_7.

3. Policy shall be to Allow for Operation of ROOT Protection Domain ROOT Drivers
   in RCINIT Framework Groups by Deliverable Owner [ ROOT/RCINIT_GROUP_2 .. ROOT/RCINIT_GROUP_7 ]

4. Policy shall be to Allow for Operation to USER Protection Domain USER Drivers
   in RCINIT Framework Groups by Deliverable Owner [ USER/RCINIT_GROUP_2 .. USER/RCINIT_GROUP_7 ]

USER PD LAUNCHING INSTRUCTIONS

The deliverable owner is responsible to deliver components to both ROOT PD and USER PD
as necessary. CORE software does not retain or track specific application requirements
of a USER PD -- the protection domain is logically a software partition mechanism that
allows the application owner to retain more control over their deliverable.

A User PD is started by an RCINIT Init Function supplied by the deliverable owner. The
following example shows the minimum PD_MON API use required to launch a specific USER PD.
Essentially, the software below is a "driver" init function that bootstraps the USER PD
deliverable image.

SCONS Source, Provided by AUDIO, pd_audio.scons, Launches Audio PD from ROOT PD

if 'USES_RCINIT' in env:
 env.AddRCInitFunc(
  ROOT_IMG, # Executes in ROOT to SPAWN audio
  {
   'sequence_group'             : 'RCINIT_GROUP_7', # Policy Based, No Earlier than RCINIT_GROUP_2
   'init_name'                  : 'pd_audio',
   'init_function'              : 'pd_mon_audio',
  })

C Source, Provided by AUDIO, pd_audio.c, Launches Audio PD from ROOT PD

#include "pd_mon_qurt.h"
void pd_mon_audio(void)
{
PD_MON_RESTART restart = PD_MON_RESTART_ALWAYS;
PD_MON_HANDLE hPDAudio = pd_mon_spawn(&restart, BUILD_TIME_DEFINED_AUDIO_IMAGE_NAME); }
}

In the case of this specific example, the AUDIO technology area will be supplying their
"driver" init function that launches their software in a USER PD. Other specific examples
where a software application can be partitioned and launched could include SENSORS, MPSS,
... any logical software application that is currently running in a Q6 ROOT PD.

*/

#if defined(__cplusplus)
extern "C" {
#endif

/** @addtogroup pd_mon_api */ /** @{ */

/**
API, Typedefs and Enumerations
*/

#define PD_MON_RESTART_NEVER  1           /**< Restart Never, Starts Once */
#define PD_MON_RESTART_CAPS   2           /**< Restart w/ Caps */
#define PD_MON_RESTART_ALWAYS 0x7fffffff  /**< Restart 2 Billion Times */

#define PD_MON_ERROR   ((PD_MON_HANDLE)0) /**< Error Indication (for Comparision) */

typedef char const* PD_MON_NAME;          /**< PD MON Name */

typedef int PD_MON_HANDLE;                /**< PD MON Handle */

typedef int PD_MON_RESTART;               /**< Restart Counter */

typedef enum { PD_MON_STATUS_SUCCESS = 0, PD_MON_STATUS_ERROR = -1 } PD_MON_STATUS;

typedef char const* PD_DUMP_NAME;         /**< PD DUMP Name */

typedef int PD_DUMP_HANDLE;               /**< PD DUMP Handle */

typedef enum { PD_DUMP_STATUS_SUCCESS = 0, PD_DUMP_STATUS_ERROR = -1 } PD_DUMP_STATUS;

typedef unsigned long PD_EXPIRY;
typedef unsigned long PD_RESTARTS;

typedef struct
{
   PD_EXPIRY r_expiry;
   PD_RESTARTS r_fails;

} PD_SPAWN_CAPS;

/**
API, INTERMEDIATE, Spawn a Protection Domain with Restart Monitoring
PD_MON_RESTART -- Default Restart Monitoring Counter
PD_MON_NAME -- Pointer to NULL terminated string, name of the Protection Domain
... -- Optional Args
@return
PD_MON_HANDLE -- Error Indication or Protection Domain Handle
*/
DLL_API_WARN_UNUSED_RESULT 
DLL_API_EXPORT_PD_MON PD_MON_HANDLE pd_mon_spawn(PD_MON_RESTART*, PD_MON_NAME const, ...);

/** @} end_addtogroup pd_mon_api */

#if defined(__cplusplus)
}
#endif

#endif
