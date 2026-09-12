#ifndef __MCPM_L1EMULATOR_H__
#define __MCPM_L1EMULATOR_H__

/*=========================================================================


           M O D E M   C L O C K   A N D   P O W E R   M A N A G E R

                 U N I T  T E S T  H E A D E R  F I L E



GENERAL DESCRIPTION

  This file contains the MCPM state definitions.

PUBLIC EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  Invoke the MCPM_Init function to initialize the Modem Clock and Power Manager.


    Copyright (c) 2021 by QUALCOMM Technologies, Inc.  All Rights Reserved.

==========================================================================*/


/*==========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mpower.mpss/13.1/mcpm/test/stubs/inc/mcpm_l1emulator.h#8 $

when       who      what, where, why
--------   ---      --------------------------------------------------------
03/05/15   yz      UT removal support
05/05/14   cl      First Jolokia official release branch off Bolt tip
10/21/13   sr       MSG_ macro changes.
09/22/11   ps       Phase-1 scheduled NPA support
07/08/11   ps       Added changes for awake timeline optimization
                    after NPA scheduled API integration
6/17/11    ps       MCPM changes to support FW Power collapse
11/15/10   mg       Initial Revision

==========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "mcpm_sys_configs.h"
#include "mcpm_resrc_configs.h"
#include <rfm_resources.h>
#include <ULog.h>
#include <ULogFront.h>
#include <mcpm_npa.h>
#include <mcpm_drv.h>

/* ---------------  Defines -------------- */


#define IF_UT_ENABLED     if (0)
#define IS_UT_ENABLED     0
#define IS_SCENARIO_ENB   0
#define COMMIT_UT_EQ_CASE 0
#define MCPM_UT_ERR_FATAL MCPM_ERR_FATAL
#define MCPM_UT_SS_ERR_FATAL MCPM_ERR_FATAL


#define CFL_FLAG_NONE                0x0
#define CFL_ISMCVSREQ                0x2

typedef enum
{
  CLOCKPLAN_INVALID_STATE = 0,
  CLOCKPLAN_REJECTED_INPUT_SCENARIO = 1,
  CLOCKPLAN_ACCEPTED_INPUT_SCENARIO = 2,
  CLOCKPLAN_NUM_COMMIT_STATES = 3
}l1em_clockplan_states;

#define CFL_CLOCKPLAN_2_0_SCENARIO_TRIGGERED       0x20000000
#define CFL_CLOCKPLAN_2_1_SCENARIO_TRIGGERED       0x40000000

#define CFL_CLOCKPLAN_TRIGGERED (CFL_CLOCKPLAN_2_0_SCENARIO_TRIGGERED|CFL_CLOCKPLAN_2_1_SCENARIO_TRIGGERED)

#define NO_RESRC_DEP 0xffffffff /* Default state of all resources measured by L1EM
                                   NOTE: CLK_DIV_NOT_APPLICABLE is also mapped to NO_RESRC_DEP */
#define NO_RESRC_DEP_UINT64 0xffffffffffffffff

#define DO_NOT_CHECK_VALUE 0xfffffffd /* Explicitly tell L1EM to not check certain value -
                                         NOTE: using 0x..FD as 0x...FE is MCPM_DC */
#define DO_NOT_CHECK_VALUE_UINT64 0xfffffffffffffffd

/* map to some safe max value to avoid overlapping with other known L1EM controls */
#define NPA_MAPPED_MAX_VALUE 0xfffffffc

//value is valid - it's not a DC
#define IS_VALID_VAL(val) ( (sizeof(val) == sizeof(uint32)) ? \
                            (((val) != ((NO_RESRC_DEP))) && ((val) != ((MCPM_RESRC_CFG_VALUE_DC))) && \
                             ((val) != ((DO_NOT_CHECK_VALUE))) && ((val) != ((MCPM_LATENCY_MAX)))) : \
                            (((uint64)(val) != ((NO_RESRC_DEP_UINT64))) && \
                             ((uint64)(val) != ((DO_NOT_CHECK_VALUE_UINT64)))) \
                           )

#define IS_INVALID_VAL(val) (!(IS_VALID_VAL((val))))
#define IS_VALID_VAL_GT_0(val) (IS_VALID_VAL(val) && ((val)) > 0)


typedef struct
{
  uint32 utTechsToRun;

  //Values populated by resrc drivers to tell UT what values they are about to commit, to help
  //track freq error checks during randomized measurements and test when there a possibility
  //some resrc might be in a scheduled npa driver callback for some tech in sleep
  uint32 valToBeCommitted[MCPM_NUM_RESRC];

  //Maximum value of the resource
  uint32 maxResrcValue[MCPM_NUM_RESRC];
} ut_global_config;


typedef struct
{
  uint32 exectd;
  uint32 apitech;
} ut_apicolres;


/* ---------------  Exported variables -------------- */


/* ---------------  Exported Functions -------------- */

/* Saves which thread ID locked the 'Tech Lock' */
void L1EM_Set_TechLock_LastThreadId(mcpm_tech_type tech);
/* Update info related to when scheduled resources get applied etc. */
void L1EM_Update_Sched_Info(mcpm_npa_tech_data * schedData);
void L1EM_Update_Tech_ActiveAsNeighbor(mcpm_tech_type iratTech, mcpm_tech_type homeTech,
                                       boolean bIsTechActiveAsNeighbor);
void L1EM_Update_Req_Start(mcpm_tech_type eTech, mcpm_request_type eRequest, void * pmReqParms,
                           uint32 cntrlflag);
void L1EM_Update_Req_End(mcpm_tech_type eTech, mcpm_request_type eRequest, void * pmReqParms,
                           uint32 cntrlflag);
void MCPM_Update_ValToBeCommitted(MCPM_Resrc_IDType resrcId, uint32 resrcState);
void MCPM_UT_Update_RF_Info(rfm_resource_info * rfInfo, mcpm_tech_type tech);
void mcpm_unit_test_init(const MCPM_Drv_CtxtType *pDrvCtxt);
void clockplan_ut_mcvs_thread_sync_update_state_table(mcpm_tech_type eTech,uint32 cntrlflag,mcpm_state_type state);
/* --------------------------------------- */

#endif /* __MCPM_L1EMULATOR_H__ */
