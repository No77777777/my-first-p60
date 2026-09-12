/*====*====*===*====*====*====*====*====*====*====*====*====*====*====*====*
             C M    M U L T I - S T A C K    C O N T R O L L E R

                 I M P L E M E N T A T I O N    F I L E

GENERAL DESCRIPTION
  This file implements the CM multi-stack architecture to support multi-stacks
  requirements such as SVLTE, DualSim, etc.

FILES STRUCTURE

  *** PLEASE NOTE BEFORE MAKING MODIFICATIONS ***

  This file is organized in below structures:
  Section 1: Includes files
  Section 2: Static/Local Variables
  Section 3: Static Function Forward Declaration
  Section 4: SIM Coordinator Core-Functions (Init, get_ptr, find_routes, etc.)
  Section 5: SIM Coordinator Process Functions (subsc, oprt_mode, pref_sys_chgd)
  Section 6: Action Functions (route request, subsc_chgd, etc.)
  Section 7: Utilities/Helper Functions (including static functions, get/set)

  The section are arranged in the logical order, increased by frequency-of-change.
  Means the last section (section 7) are most likely to be updated/added.

  *** PLEASE NOTE BEFORE MAKING MODIFICATIONS ***


Copyright (c) 2013 - 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmsimcoord.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/7/2013   cl     DSDX/TSTX Migration
07/09/1413  cl     Initial Release for MultiSim Framework
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "mmcp_variation.h"
#include "customer.h" /* Customer configuration file */
#include "comdef.h"   /* Definition for basic types and macros */
#include "cmtaski.h"

#include "cmph.h"     /* Interface to CM ph functions */
#include "cmcsfbcall.h"
#include "cmsds.h"
#ifdef CM_GW_SUPPORTED
#include "cmwcall.h"
#endif
#include "cmmsc.h"
#include "cmmsc_auto.h"
#include "cmsimcoord.h"
#include "mmoc_v.h"
#include "modem_mem.h"
#include "sys.h"      /* Common types. */
#if defined(CM_GW_SUPPORTED) || defined(FEATURE_CM_LTE) || defined(FEATURE_TDSCDMA)
#include "cmregprxi.h"
#endif

#include "cmpmprx.h"
#include "cmdbg_qsh.h"

#ifdef CM_DEBUG
#error code not present
#endif


/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

static cmmsimc_state_s_type   sim_coord_state;

/* Routing Table
** EVENT,
** STATE for stack1, 2, ...
** ACTION for stack 1, 2, ... (order matters)
** NEW STATE for stack 1, 2, ...
*/
static cmmsimc_correlation_table_entry_s_type correlation_table[] =
{
  /* ------------------------------- SINGLE SIM -------------------------------- */
  {
    CMMSIMC_EVENT_ONLINE,                 CMMSIMC_COORDINATION_SINGLE_SIM,
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_INACTIVE,               CMMSIMC_SIM_STATE_INACTIVE} //NULL or INACTIVE?
  },

  {
    CMMSIMC_EVENT_OFFLINE,                CMMSIMC_COORDINATION_SINGLE_SIM,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_INACTIVE,               CMMSIMC_SIM_STATE_INACTIVE} //NULL or INACTIVE?
  },

  {
    CMMSIMC_EVENT_SUB,                   CMMSIMC_COORDINATION_SINGLE_SIM,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_INACTIVE,               CMMSIMC_SIM_STATE_INACTIVE} //NULL or INACTIVE?
  },

  /* Catch all - Single SIM*/
  {
    CMMSIMC_EVENT_ANY,                    CMMSIMC_COORDINATION_SINGLE_SIM,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                 CMMSIMC_SIM_STATE_NO_CHG}
  },
  /* ------------------------------ SINGLE SIM END ------------------------------ */

  /* -------------------------------- MULTI-SIM --------------------------------- */

  {
    CMMSIMC_EVENT_SUB,                    CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL}

  },
  /* 5 */
  {
    CMMSIMC_EVENT_SUB,                    CMMSIMC_COORDINATION_XSIM_SINGLE_SIM1,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_NO_CHG,                 CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SUB,                    CMMSIMC_COORDINATION_XSIM_SINGLE_SIM2,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SUB,                    CMMSIMC_COORDINATION_XSIM_SINGLE_SIM3,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                 CMMSIMC_SIM_STATE_ACTIVE}
  },

  {
    CMMSIMC_EVENT_SUB,                    CMMSIMC_COORDINATION_XSIM_DUAL_SIM1_SIM2,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SUB,                    CMMSIMC_COORDINATION_XSIM_DUAL_SIM1_SIM3,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_NO_CHG,                 CMMSIMC_SIM_STATE_ACTIVE}
  },

  /* 10 */

  {
    CMMSIMC_EVENT_SUB,                    CMMSIMC_COORDINATION_XSIM_DUAL_SIM2_SIM3,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_ACTIVE}
  },

  {
    CMMSIMC_EVENT_SUB,                    CMMSIMC_COORDINATION_XSIM_TRIPLE_STANDBY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_ACTIVE}
  },

  /* Catch all */
  {
    CMMSIMC_EVENT_SUB,                    CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_ACT_SUBS_CHANGE},         {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_NO_CHG,                 CMMSIMC_SIM_STATE_NO_CHG}
  },

  /* ----------------------------------------------------------------------------- */
  {
    CMMSIMC_EVENT_STANDBY_PREF_CHGD,      CMMSIMC_COORDINATION_XSIM_SINGLE_SIM1,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ON},            {1,CMMSIMC_SIM_ACT_OFF},                 {2,CMMSIMC_SIM_ACT_OFF}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_INACTIVE,               CMMSIMC_SIM_STATE_INACTIVE}
  },

  {
    CMMSIMC_EVENT_STANDBY_PREF_CHGD,      CMMSIMC_COORDINATION_XSIM_SINGLE_SIM2,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_OFF},            {1,CMMSIMC_SIM_ACT_ON},                 {2,CMMSIMC_SIM_ACT_OFF}},
    {CMMSIMC_SIM_STATE_INACTIVE,          CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_INACTIVE}
  },

  /* 15 */

  {
    CMMSIMC_EVENT_STANDBY_PREF_CHGD,      CMMSIMC_COORDINATION_XSIM_SINGLE_SIM3,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_OFF},            {1,CMMSIMC_SIM_ACT_OFF},                 {2,CMMSIMC_SIM_ACT_ON}},
    {CMMSIMC_SIM_STATE_INACTIVE,          CMMSIMC_SIM_STATE_INACTIVE,               CMMSIMC_SIM_STATE_ACTIVE}
  },

  {
    CMMSIMC_EVENT_STANDBY_PREF_CHGD,      CMMSIMC_COORDINATION_XSIM_DUAL_SIM1_SIM2,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ON},            {1,CMMSIMC_SIM_ACT_ON},                 {2,CMMSIMC_SIM_ACT_OFF}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_INACTIVE}
  },

  {
    CMMSIMC_EVENT_STANDBY_PREF_CHGD,      CMMSIMC_COORDINATION_XSIM_DUAL_SIM1_SIM3,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ON},            {1,CMMSIMC_SIM_ACT_OFF},                 {2,CMMSIMC_SIM_ACT_ON}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_INACTIVE,               CMMSIMC_SIM_STATE_ACTIVE}
  },

  {
    CMMSIMC_EVENT_STANDBY_PREF_CHGD,      CMMSIMC_COORDINATION_XSIM_DUAL_SIM2_SIM3,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_OFF},            {1,CMMSIMC_SIM_ACT_ON},                 {2,CMMSIMC_SIM_ACT_ON}},
    {CMMSIMC_SIM_STATE_INACTIVE,          CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_ACTIVE}
  },

  {
    CMMSIMC_EVENT_STANDBY_PREF_CHGD,      CMMSIMC_COORDINATION_XSIM_TRIPLE_STANDBY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                    CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ON},            {1,CMMSIMC_SIM_ACT_ON},                 {2,CMMSIMC_SIM_ACT_ON}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_ACTIVE}
  },

  /* ----------------------------------------------------------------------------- */

  /* 20 */
  {
    CMMSIMC_EVENT_ONLINE,                 CMMSIMC_COORDINATION_NONE,
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_NONE},                 {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_INACTIVE,            CMMSIMC_SIM_STATE_INACTIVE,               CMMSIMC_SIM_STATE_INACTIVE}
  },

  {
    CMMSIMC_EVENT_ONLINE,                 CMMSIMC_COORDINATION_XSIM_SINGLE_SIM1,
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_OFF},                  {2,CMMSIMC_SIM_ACT_OFF}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_INACTIVE,               CMMSIMC_SIM_STATE_INACTIVE}
  },

  {
    CMMSIMC_EVENT_ONLINE,                 CMMSIMC_COORDINATION_XSIM_SINGLE_SIM2,
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_SIM_ACT_OFF},             {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},        {2,CMMSIMC_SIM_ACT_OFF}},
    {CMMSIMC_SIM_STATE_INACTIVE,          CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_INACTIVE}
  },

  {
    CMMSIMC_EVENT_ONLINE,                 CMMSIMC_COORDINATION_XSIM_SINGLE_SIM3,
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_SIM_ACT_OFF},             {1,CMMSIMC_SIM_ACT_OFF},                  {2,CMMSIMC_SIM_ACT_ROUTE_REQUEST}},
    {CMMSIMC_SIM_STATE_INACTIVE,          CMMSIMC_SIM_STATE_INACTIVE,               CMMSIMC_SIM_STATE_ACTIVE}
  },

  {
    CMMSIMC_EVENT_ONLINE,                 CMMSIMC_COORDINATION_XSIM_DUAL_SIM1_SIM2,
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},        {2,CMMSIMC_SIM_ACT_OFF}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ACTIVE,                 CMMSIMC_SIM_STATE_INACTIVE}
  },

  /* 25 */
  {
    CMMSIMC_EVENT_ONLINE,                 CMMSIMC_COORDINATION_XSIM_TRIPLE_STANDBY,
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                  CMMSIMC_SIM_STATE_NULL},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},       {2,CMMSIMC_SIM_ACT_ROUTE_REQUEST}},
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ACTIVE,                CMMSIMC_SIM_STATE_ACTIVE}
  },

  /* ----------------------------------------------------------------------------- */

  {
    CMMSIMC_EVENT_OFFLINE,                CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},       {2,CMMSIMC_SIM_ACT_ROUTE_REQUEST}},
    {CMMSIMC_SIM_STATE_NULL,              CMMSIMC_SIM_STATE_NULL,                   CMMSIMC_SIM_STATE_NULL}
  },


  /* ----------------------------------------------------------------------------- */

  {
    CMMSIMC_EVENT_PREF_SYS_SIM1,          CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_NONE},                {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_PREF_SYS_SIM2,          CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_NONE},            {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},       {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_PREF_SYS_SIM3,          CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_NONE},            {1,CMMSIMC_SIM_ACT_NONE},                {2,CMMSIMC_SIM_ACT_ROUTE_REQUEST}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  /* ----------------------------------------------------------------------------- */
  /* 30 */
  {
    CMMSIMC_EVENT_SRV_SIM1,               CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_WAKE_UP},             {2,CMMSIMC_SIM_ACT_WAKE_UP}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SRV_SIM2,               CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_WAKE_UP},         {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},       {2,CMMSIMC_SIM_ACT_WAKE_UP}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SRV_SIM3,               CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_WAKE_UP},         {1,CMMSIMC_SIM_ACT_WAKE_UP},             {2,CMMSIMC_SIM_ACT_ROUTE_REQUEST}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  /* ----------------------------------------------------------------------------- */

  /* call end on sub2 (srlte+g), and action on sub1 */
  {
    CMMSIMC_EVENT_CALL_SIM2,              CMMSIMC_COORDINATION_XSIM_DUAL_SIM1_SIM2,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_PEER_SUBS_CALL},  {1,CMMSIMC_SIM_ACT_NONE},                {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  /* call end on sub1 (g+srlte), and action on sub2 */
  {
    CMMSIMC_EVENT_CALL_SIM1,              CMMSIMC_COORDINATION_XSIM_DUAL_SIM1_SIM2,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_NONE},            {1,CMMSIMC_SIM_ACT_PEER_SUBS_CALL},     {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  /* ----------------------------------------------------------------------------- */

  /* 35 */
  {
    CMMSIMC_EVENT_SUSPEND_RESUME,         CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ACTIVE,                CMMSIMC_SIM_STATE_ACTIVE},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},       {2,CMMSIMC_SIM_ACT_ROUTE_REQUEST}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SUSPEND_RESUME,         CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ACTIVE,                CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},       {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SUSPEND_RESUME,         CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ACTIVE,                CMMSIMC_SIM_STATE_ACTIVE},
    {{0,CMMSIMC_SIM_ACT_NONE},            {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},       {2,CMMSIMC_SIM_ACT_ROUTE_REQUEST}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SUSPEND_RESUME,         CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ACTIVE},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_NONE},                {2,CMMSIMC_SIM_ACT_ROUTE_REQUEST}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SUSPEND_RESUME,         CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ACTIVE,            CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_ROUTE_REQUEST},   {1,CMMSIMC_SIM_ACT_NONE},                {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  /* 40 */
  {
    CMMSIMC_EVENT_SUSPEND_RESUME,         CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ACTIVE,                CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_NONE},           {1,CMMSIMC_SIM_ACT_ROUTE_REQUEST},        {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SUSPEND_RESUME,         CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ACTIVE},
    {{0,CMMSIMC_SIM_ACT_NONE},             {1,CMMSIMC_SIM_ACT_NONE},               {2,CMMSIMC_SIM_ACT_ROUTE_REQUEST}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  },

  {
    CMMSIMC_EVENT_SUSPEND_RESUME,         CMMSIMC_COORDINATION_ANY,
    {CMMSIMC_SIM_STATE_ANY,               CMMSIMC_SIM_STATE_ANY,                   CMMSIMC_SIM_STATE_ANY},
    {{0,CMMSIMC_SIM_ACT_NONE},            {1,CMMSIMC_SIM_ACT_NONE},                {2,CMMSIMC_SIM_ACT_NONE}},
    {CMMSIMC_SIM_STATE_NO_CHG,            CMMSIMC_SIM_STATE_NO_CHG,                CMMSIMC_SIM_STATE_NO_CHG}
  }

  /* ------------------------------ MULTI-SIM END -------------------------------- */
};

/*===========================================================================

            FUNCTION DECLARATIONS AND IMPLEMENTATION

This section contains function declaration and implementation for this
feature.

===========================================================================*/



/*===========================================================================

FUNCTION cmmsimc_print_simc_info

DESCRIPTION
This function prints the sim coordinator info.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void  cmmsimc_print_simc_info(void);

/*===========================================================================

FUNCTION cmmsimc_print_corr_table_info

DESCRIPTION
This function prints the correlation table information.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void  cmmsimc_print_corr_table_info(
  int                   correlation_tbl_idx,
  cmmsimc_event_e_type  event
);

/****************************************************************************
**    Function definitions
****************************************************************************/

/* Static function */
static void cmmsimc_sim_init( void )
{
  size_t i = 0;
  cmmsimc_state_s_type *coordinator = cmmsimc_state_ptr();

  for(i=0; i < cmpmprx_get_num_of_sims(); i++)
  {
    coordinator->sim[i].state = CMMSIMC_SIM_STATE_NULL;
    coordinator->sim[i].esr_state = CMSIMC_ESR_NONE;
  }
}

static void cmmsimc_msc_state_machine_init( void )
{
  cmmsimc_state_s_type  *coordinator = cmmsimc_state_ptr();
  size_t i = 0;

  for(i = 0; i < cmpmprx_get_num_of_sims(); i++)
  {
    /* Dynamically allocate state machine for each sub */
    coordinator->sim[i].msc_machine = (cmmsc_state_machine_s_type *)cm_mem_malloc(
                                        sizeof(cmmsc_state_machine_s_type));

    if(coordinator->sim[i].msc_machine)
    {
      memset(coordinator->sim[i].msc_machine, 0, sizeof(cmmsc_state_machine_s_type));
    }
    else
    {
      sys_err_fatal_null_ptr_exception();
    }

    /* initialize state machine */
    coordinator->sim[i].msc_machine->asubs_id = (sys_modem_as_id_e_type)(i);
    cmmsc_state_machine_init(coordinator->sim[i].msc_machine);
  }
}

/*===========================================================================

FUNCTION cmmsimc_proc_cmd_oprt_mode_per_state_multi_sim

DESCRIPTION
  This function processes multi-sim state upon online

DEPENDENCIES
  cmpref2_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void cmmsimc_proc_cmd_oprt_mode_per_state_multi_sim (void)
{
  cmph_s_type          *ph_ptr = cmph_ptr();
  cm_mm_ss_info_sub_prop_s_type *ss_info_prop_ptr;
  uint8 i, j;

  for(i=0; i<cmss_ptr()->count; i++)
  {
    ss_info_prop_ptr = cmss_get_info_sub_prop((sys_modem_as_id_e_type)i);
    if(ss_info_prop_ptr == NULL)
    {
      continue;
    }
    for(j=MM_STACK_0; j<MM_STACK_ALL; j++)
    {
      if(ss_info_prop_ptr->stack_info[j].gen_info != NULL)
      {
        ss_info_prop_ptr->stack_info[j].gen_info->is_stack_operational = FALSE;
      }
    }
  }

  for(i=0; i<cmss_ptr()->count; i++)
  {
    ss_info_prop_ptr = cmss_get_info_sub_prop((sys_modem_as_id_e_type)i);
    if(ss_info_prop_ptr == NULL)
    {
      continue;
    }

    if(ss_info_prop_ptr->stack_info[MM_STACK_0].gen_info != NULL &&
        (ph_ptr->device_prop.msim_prop.active_subs & BM(i)))
    {
      ss_info_prop_ptr->stack_info[MM_STACK_0].gen_info->is_stack_operational = TRUE;
    }
  }
}

/*===========================================================================

FUNCTION cmsimcoord_state_ptr

DESCRIPTION
  This function returns the SIM Coordinator pointer

DEPENDENCIES
  None.

RETURN VALUE
  cmmsc_state_machine_s_type*

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
cmmsimc_state_s_type *cmmsimc_state_ptr( void )
{
  return &sim_coord_state;
}

/*===========================================================================

FUNCTION cmmsimc_route_table_ptr

DESCRIPTION
  This function returns the a pointer to the entry of the Correlation Table

DEPENDENCIES
  None.

RETURN VALUE
  cmmsimc_correlation_table_entry_s_type*

===========================================================================*/
cmmsimc_correlation_table_entry_s_type *cmmsimc_correlation_table_ptr( void )
{
  return (cmmsimc_correlation_table_entry_s_type *)&correlation_table;
}

/*===========================================================================

FUNCTION route_table_entry_ptr

DESCRIPTION
  This function returns the pointer to an index of the correlation table

DEPENDENCIES
  None.

RETURN VALUE
  cmmsc_routing_table_entry_s_type*

===========================================================================*/
cmmsimc_correlation_table_entry_s_type *correlation_table_entry_ptr(
  int index
)
{
  cmmsimc_correlation_table_entry_s_type *correlation_table
    = cmmsimc_correlation_table_ptr();

  return &correlation_table[index];
}

/*===========================================================================

FUNCTION cmmsimc_correlation_table_size

DESCRIPTION
  This function returns the size of the correlation table

DEPENDENCIES
  None.

RETURN VALUE
  int

===========================================================================*/
int cmmsimc_correlation_table_size(void)
{
  return ARR_SIZE(correlation_table);
}

/*===========================================================================

FUNCTION cmmsimc_state_init

DESCRIPTION
  This function should be called to initialize the whole SIM Coordinator

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_state_init( void )
{
  cmmsimc_state_s_type  *coordinator = cmmsimc_state_ptr();
  sys_overall_feature_t  feature_mode = cmph_get_feature_mode();

  if(coordinator == NULL)
  {
    sys_err_fatal_null_ptr_exception();
  }

  /* ERR_FATAL if feature mode is SGLTE */
  if(feature_mode == SYS_OVERALL_FEATURE_MODE_SGLTE)
  {
    CM_ERR_FATAL_1("Feature mode SGLTE is not supported: fmode = %d", feature_mode);
  }

  /* initialize the coordinator */
  coordinator->dev_info.standby_pref = SYS_MODEM_DS_PREF_NONE;
  cmmsimc_sim_init();
  cmmsimc_msc_state_machine_init();
  coordinator->dev_info.active_subs = 0;
  coordinator->dev_info.device_mode = cmpmprx_get_device_mode();

  /* Initialize the coordination for single SIM */
  if(cmph_is_ssim())
  {
    coordinator->dev_info.coordination = CMMSIMC_COORDINATION_SINGLE_SIM;
    coordinator->dev_info.active_subs  = SYS_MODEM_AS_ID_1_MASK;
  }
}

cmmsc_state_machine_s_type     *cmmsimc_state_machine_ptr(
  sys_modem_as_id_e_type        as_id
)
{
  size_t i = 0;
  cmmsimc_state_s_type *coordinator = cmmsimc_state_ptr();
  if(coordinator == NULL)
  {
    sys_err_fatal_null_ptr_exception();
  }

  for(i = 0; i < cmpmprx_get_num_of_sims(); i++)
  {
    if(coordinator->sim[i].msc_machine->asubs_id == as_id)
    {
      return coordinator->sim[i].msc_machine;
    }
  }
  sys_err_fatal_invalid_value_exception((int)as_id);
  return NULL;
}

cmmsc_state_machine_s_type *cmmsimc_state_machine_ptr_per_sim_idx(
  size_t                        sim_idx
)
{
  cmmsimc_state_s_type *coordinator = cmmsimc_state_ptr();
  if(coordinator == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return NULL;
  }

  if(!(sim_idx < MAX_SIMS))
  {
    sys_err_fatal_invalid_value_exception((int)sim_idx);
  }

  if(coordinator->sim[sim_idx].msc_machine != NULL)
  {
    return coordinator->sim[sim_idx].msc_machine;
  }

  sys_err_fatal_invalid_value_exception((int)sim_idx);
  return NULL;
}

/*===========================================================================

FUNCTION cmmsimc_compare_sim_states

DESCRIPTION
  This function compares two sim states. SIM States are enums and
  not always the same to be 'equal'.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if equals
===========================================================================*/
boolean cmmsimc_compare_sim_states(
  cmmsimc_sim_state_e_type   state1,
  cmmsimc_sim_state_e_type   state2
)
{
  if(state1 == state2)
  {
    return TRUE;
  }

  /* If either of mode says "ANY", then they're matched */
  if(state1 == CMMSIMC_SIM_STATE_ANY ||
      state2 == CMMSIMC_SIM_STATE_ANY )
  {
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION cmmsimc_compare_sim_events

DESCRIPTION
  This function compares two sim events. SIM events are enums and
  not always the same to be 'equal'.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if equals
===========================================================================*/
boolean cmmsc_compare_sim_events(
  cmmsimc_event_e_type   event1,
  cmmsimc_event_e_type   event2
)
{
  if(event1 == event2)
  {
    return TRUE;
  }

  /* If either of mode says "ANY", then they're matched */
  if(event1 == CMMSIMC_EVENT_ANY ||
      event2 == CMMSIMC_EVENT_ANY )
  {
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION cmmsc_compare_coordinations

DESCRIPTION
  This function compares two sim coordinations setups.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if equals
===========================================================================*/
boolean cmmsc_compare_coordinations(
  cmmsimc_coordination_e_type   coordination1,
  cmmsimc_coordination_e_type   coordination2
)
{
  if(coordination1 == coordination2)
  {
    return TRUE;
  }

  /* If either of mode says "ANY", then they're matched */
  if(coordination1 == CMMSIMC_COORDINATION_ANY ||
      coordination2 == CMMSIMC_COORDINATION_ANY )
  {
    return TRUE;
  }
  return FALSE;
}

/* Find correlation index in the correlation table based on current states & events
*/
int cmmsimc_find_correlation_idx(
  cmmsimc_event_e_type   event
)
{
  cmmsimc_state_s_type                   *cmmsimc_ptr = cmmsimc_state_ptr();
  cmmsimc_correlation_table_entry_s_type *corr_table  =
                                              cmmsimc_correlation_table_ptr();
  int max_entry = cmmsimc_correlation_table_size();
  size_t                             col_idx;
  int                                corr_tbl_idx;
  boolean                            is_match      = FALSE;

  /* Validate input event */
  if(event == CMMSIMC_EVENT_NONE)
  {
    return CMMSIMC_CORRELATION_NONE;
  }

  /* Take the event and check for corresponding action */
  for(corr_tbl_idx = 0; corr_tbl_idx < max_entry; corr_tbl_idx++)
  {
    /* Check for event && current mode */
    if(cmmsc_compare_sim_events(corr_table[corr_tbl_idx].event, event) &&
        cmmsc_compare_coordinations(cmmsimc_ptr->dev_info.coordination,
                                    corr_table[corr_tbl_idx].coordination)
      )
    {
      /* If event matched, then check if each individual state matches */
      for(col_idx = 0; col_idx < cmpmprx_get_num_of_sims(); col_idx++)
      {
        if(!cmmsimc_compare_sim_states(cmmsimc_ptr->sim[col_idx].state,
                                       corr_table[corr_tbl_idx].state[col_idx]))
        {
          break;
        }
      }
      /* All states matched */
      if(col_idx == cmpmprx_get_num_of_sims())
      {
        is_match = TRUE;
        break;
      }
    }
  }

  /* If match is found, take action for each stack */
  if(is_match)
  {
    return corr_tbl_idx;
  }

  return CMMSIMC_CORRELATION_NONE;
}

/*
** Entry point for Multi-Sim Controller. It will find:
** 1. Routing index in the Correlation Table
** 2. Take action upon that routing index
*/
void cmmsimc_sim_coordinate(
  cmmsc_param_base_s_type *event_package
)
{
  int correlation_tbl_idx     = CMMSIMC_CORRELATION_NONE;
  cmmsimc_event_e_type  event = event_package->event;

  correlation_tbl_idx = cmmsimc_find_correlation_idx(event);

  /* Print correlation table info */
  cmmsimc_print_corr_table_info(correlation_tbl_idx, event);

#ifdef FEATURE_CM_DEBUG_BUFFER
  //cmdbg_add_to_dbg_buffer(CM_BUFF_SIMC_ROUTING_ENTRY_TYPE, 0, ((void*)&correlation_tbl_idx));
#endif

  /* Execute sim action */
  if (correlation_tbl_idx != CMMSIMC_CORRELATION_NONE)
  {
    /* Perform the action */
    cmmsimc_execute_action(correlation_tbl_idx, event_package);

    /* Update the state */
    cmmsimc_update_state(correlation_tbl_idx);
  }
}

/*===========================================================================

FUNCTION cmmsimc_execute_action

DESCRIPTION
  This function takes action based on the matched index action in
  Correlation Table.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_execute_action(
  int                        correlation_tbl_idx,
  cmmsc_param_base_s_type   *event_package
)
{
  size_t act_idx;
  cmmsimc_correlation_table_entry_s_type  *corr_table_entry =
    correlation_table_entry_ptr(correlation_tbl_idx);

  /* Step through each act_idx of the corr_table_entry and perform the relative actions */
  for(act_idx=0; act_idx < cmpmprx_get_num_of_sims() && act_idx < MAX_SIMS; act_idx++)
  {
    int sim_idx = corr_table_entry->action[act_idx].sim_idx;
    event_package->state_machine = cmmsimc_state_machine_ptr_per_sim_idx(sim_idx);

    switch( corr_table_entry->action[act_idx].action)
    {
      case CMMSIMC_SIM_ACT_ROUTE_REQUEST:
        cmmsimc_act_route_request(event_package);
        break;

      case CMMSIMC_SIM_ACT_POWER_DOWN:
        cmmsimc_act_power_down((cmmsc_param_oprt_mode_s_type *)event_package);
        break;

      case CMMSIMC_ACT_SUBS_CHANGE:
      {
        /* Send sub command to MSC and then process sub-command
             - CMMSIMC: 1. set subscription capability per stack (cmmsc_proc_cmd_subsc_chgd_set_capability)
             - CMMSIMC: 2. send subscription on MAIN (SIM-1) to MMOC
             - CMMSIMC: 3. Trigger CMMSC.
           Key note: we don't want to trigger CMMSC Routing Table before CMMSIMC because
           we might take action on CMMSC, which we don't want to take before subscription
           are sent.
        */
        cmmsc_param_subs_chgd_s_type *subs_param =
          (cmmsc_param_subs_chgd_s_type *)event_package;
        cmmsc_proc_cmd_subsc_chgd_set_capability(subs_param);
        cmmsimc_send_mmoc_subsc_chgd(event_package);
        cmmsc_proc_cmd_subsc_chgd(subs_param);
        break;
      }

      case CMMSIMC_SIM_ACT_WAKE_UP:
        cmmsimc_act_wake_up(sim_idx, (cmmsc_param_srv_ind_s_type *)event_package);
        break;

      case CMMSIMC_SIM_ACT_ON:
      case CMMSIMC_SIM_ACT_OFF:
      {
        cmmsimc_act_on_off(corr_table_entry->action[act_idx].action, event_package);
        break;
      }

      case CMMSIMC_SIM_ACT_PEER_SUBS_CALL:
        cmsimc_act_peer_subs_call((cmmsc_param_call_ind_s_type *)event_package, sim_idx);
        break;

      case CMMSIMC_SIM_ACT_MAX:
      default:
        break;
    }
  }
}

/*===========================================================================

FUNCTION cmmsimc_update_state

DESCRIPTION
  This function update the current states to a new states in given
  correlation table index.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_update_state(
  int   corr_table_idx
)
{
  size_t                                   sim_idx;
  cmmsimc_correlation_table_entry_s_type  *corr_table_entry =
    correlation_table_entry_ptr(corr_table_idx);
  cmmsimc_state_s_type                    *cmmsimc_ptr  = cmmsimc_state_ptr();

  for(sim_idx = 0; sim_idx < cmpmprx_get_num_of_sims(); sim_idx++)
  {
    if( corr_table_entry->new_state[sim_idx] != CMMSIMC_SIM_STATE_NO_CHG )
    {
      if(cmmsimc_ptr->sim[sim_idx].state != corr_table_entry->new_state[sim_idx] )
      {
        CM_MSG_HIGH_3("MSIMC: updating sim %d, from state %d to %d",
                      sim_idx,
                      cmmsimc_ptr->sim[sim_idx].state,
                      corr_table_entry->new_state[sim_idx]);
      }
      /* Update sim state */
      cmmsimc_ptr->sim[sim_idx].state = corr_table_entry->new_state[sim_idx];
    }
  }
}

/*===========================================================================

FUNCTION cmmsimc_print_simc_info

DESCRIPTION
This function prints the sim coordinator info.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void  cmmsimc_print_simc_info(void)
{
  cmmsimc_state_s_type *cmmsimc_ptr = cmmsimc_state_ptr();
  size_t sim_idx = 0;

  /* Print the current state for each sim */
#ifdef FEATURE_MMODE_DUAL_SIM
  if (cmph_is_dsda() || cmph_is_dsds())
  {
    CM_MSG_HIGH_4("MSIMC: info DSDX coordination=%d, standby_pref=%d, sim1_state=%d, sim2_state=%d",
                  cmmsimc_ptr->dev_info.coordination,
                  cmmsimc_ptr->dev_info.standby_pref,
                  cmmsimc_ptr->sim[0].state,
                  cmmsimc_ptr->sim[1].state);
  }
  else
#endif
#ifdef FEATURE_MMODE_TRIPLE_SIM
  if (cmph_is_tsts())
  {
    CM_MSG_HIGH_5("MSIMC: info DSDX coordination=%d, standby_pref=%d, sim1_state=%d, sim2_state=%d, sim3_state=%d",
                  cmmsimc_ptr->dev_info.coordination,
                  cmmsimc_ptr->dev_info.standby_pref,
                  cmmsimc_ptr->sim[0].state,
                  cmmsimc_ptr->sim[1].state,
                  cmmsimc_ptr->sim[2].state);
  }
  else
#endif
  {
    CM_MSG_HIGH_3("MSIMC: info SSIM coordination=%d, standby_pref=%d, sim_state=%d",
                  cmmsimc_ptr->dev_info.coordination,
                  cmmsimc_ptr->dev_info.standby_pref,
                  cmmsimc_ptr->sim[0].state);
  }
}

/*===========================================================================

FUNCTION cmmsimc_print_corr_table_info

DESCRIPTION
This function prints the correlation table information.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void  cmmsimc_print_corr_table_info(
  int                   correlation_tbl_idx,
  cmmsimc_event_e_type  event
)
{
  if (correlation_tbl_idx != CMMSIMC_CORRELATION_NONE)
  {
    cmmsimc_correlation_table_entry_s_type *corr_table_entry =
      correlation_table_entry_ptr(correlation_tbl_idx);
    size_t sim_idx = 0;

    /* Print the state, action, new state per sim */
    for(sim_idx=0; sim_idx < cmpmprx_get_num_of_sims(); sim_idx++)
    {

      if(corr_table_entry->action[sim_idx].action != CMMSIMC_SIM_ACT_NONE
          && corr_table_entry->action[sim_idx].action != CMMSIMC_SIM_ACT_ROUTE_REQUEST)
      {

        CM_MSG_HIGH_6_ASID("MSIMC_TABLE: state=%d->%d, action=%d, event=%d, idx=%d",
                           sim_idx,
                           corr_table_entry->state[sim_idx],
                           corr_table_entry->new_state[sim_idx],
                           corr_table_entry->action[sim_idx].action,
                           event, correlation_tbl_idx);
      }
    }
  }
}


/*===========================================================================

  DESCRIPTION:
   active_subs has bits set for corresponding enum of sys_modem_as_id_e_type.
   Based on bits sets in it, standby pref is decided and set in SIMC.

===========================================================================*/
static void cmsimc_set_coordination(
  uint8 active_subs
)
{
  cmmsimc_state_s_type *msimc = cmmsimc_state_ptr();

  switch(active_subs)
  {
    case SYS_MODEM_AS_ID_1_MASK:
      msimc->dev_info.coordination = CMMSIMC_COORDINATION_XSIM_SINGLE_SIM1;
      break;

    case SYS_MODEM_AS_ID_2_MASK:
      msimc->dev_info.coordination = CMMSIMC_COORDINATION_XSIM_SINGLE_SIM2;
      break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
    case SYS_MODEM_AS_ID_3_MASK:
      msimc->dev_info.coordination = CMMSIMC_COORDINATION_XSIM_SINGLE_SIM3;
      break;
#endif

    case (SYS_MODEM_AS_ID_1_MASK | SYS_MODEM_AS_ID_2_MASK):
      msimc->dev_info.coordination = CMMSIMC_COORDINATION_XSIM_DUAL_SIM1_SIM2;
      break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
    case (SYS_MODEM_AS_ID_1_MASK | SYS_MODEM_AS_ID_3_MASK):
      msimc->dev_info.coordination = CMMSIMC_COORDINATION_XSIM_DUAL_SIM1_SIM3;
      break;

    case (SYS_MODEM_AS_ID_2_MASK | SYS_MODEM_AS_ID_3_MASK):
      msimc->dev_info.coordination = CMMSIMC_COORDINATION_XSIM_DUAL_SIM2_SIM3;
      break;

    case (SYS_MODEM_AS_ID_1_MASK | SYS_MODEM_AS_ID_2_MASK | SYS_MODEM_AS_ID_3_MASK):
      msimc->dev_info.coordination = CMMSIMC_COORDINATION_XSIM_TRIPLE_STANDBY;
      break;
#endif

    default:
      msimc->dev_info.coordination = CMMSIMC_COORDINATION_NONE;
      break;
  }

}

/*===========================================================================

FUNCTION cmmsimc_proc_cmd_oprt_mode_chgd

DESCRIPTION
This function handle Oprt_mode Change command at the SIM level

DEPENDENCIES
  cmpref_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_cmd_oprt_mode_chgd(
  sys_oprt_mode_e_type                  oprt_mode
  /* New operating mode of the phone.*/
)
{
  cmmsimc_state_s_type             *sim_coord_state = cmmsimc_state_ptr();
  cmmsc_state_machine_s_type       *state_machine   = NULL;
  cmmsimc_event_e_type              event           = CMMSIMC_EVENT_NONE;
  cmmsc_param_oprt_mode_s_type      event_oprt_mode;
  cmph_s_type                      *ph_ptr          = cmph_ptr();
  cm_mm_ss_info_sub_prop_s_type    *ss_info_prop_ptr;
  uint8 i;

  /*1. Setting event & coordination */
  switch(oprt_mode)
  {
    case SYS_OPRT_MODE_ONLINE:
    case SYS_OPRT_MODE_PSEUDO_ONLINE:
    {
      cmsimc_set_coordination(sim_coord_state->dev_info.active_subs);

      /* Generate event */
      event = CMMSIMC_EVENT_ONLINE;
      break;
    }

    /* going offline */
    default:
      cmsimc_set_coordination(sim_coord_state->dev_info.active_subs);

      event = CMMSIMC_EVENT_OFFLINE;
      break;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Print oprt mode change */
  CM_MSG_HIGH_6("oprt_chgd: oprt_mode: %d->%d, device_mode %d, rf_cap = 0x%x, dds sub %d, active_subs 0x%x",
                ph_ptr->device_prop.ph_oprt_mode.oprt_mode, oprt_mode,
                cm_get_device_mode(),
                ph_ptr->device_prop.hw_capability.mode_capability,
                ph_ptr->device_prop.msim_prop.default_data_subs,
                ph_ptr->device_prop.msim_prop.active_subs);

  for(i = 0; i < cmpmprx_get_num_of_sims(); i++)
  {
    if (ph_ptr->sub_info[i])
    {
      CM_MSG_HIGH_6_ASID_EXT("oprt_chgd: cdma %d, gwl %d, cm sfmode %d, pm sfmode %d, msc maxcap 0x%x",
                             i,
                             ph_ptr->sub_info[i]->is_3gpp2_subs_avail,
                             ph_ptr->sub_info[i]->is_3gpp_subs_avail,
                             cmph_get_subs_feature_mode(i),
                             cmpmprx_get_pm_sfmode(i),
                             cmmsc_get_curr_msc_max_cap(i),
                             (i+1));
      CM_MSG_HIGH_6_EXT("oprt_chgd: rtre_config %d, rtre_control %d, mode_pref %d, srv_domain_pref %d, hybr_pref %d, msc domain_cap %d",
                        ph_ptr->sub_info[i]->rtre_config,
                        ph_ptr->sub_info[i]->rtre_control,
                        ph_ptr->sub_info[i]->pref_info.mode_pref,
                        ph_ptr->sub_info[i]->pref_info.srv_domain_pref,
                        ph_ptr->sub_info[i]->pref_info.hybr_pref,
                        cmmsc_get_msc_domain_cap(i),
                        (i+1));

    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Setting "is_operational_flag"
  ** If the phone is moving to any operating mode other than ONLINE, then
  ** set the operational status accordingly
  */
  if( oprt_mode == SYS_OPRT_MODE_ONLINE ||
      oprt_mode == SYS_OPRT_MODE_PSEUDO_ONLINE
    )
  {
    ss_info_prop_ptr = cmss_get_info_sub_prop(SYS_MODEM_AS_ID_1);
    if (cmph_is_msim())
    {
      cmmsimc_proc_cmd_oprt_mode_per_state_multi_sim();
    }
    else if(ss_info_prop_ptr != NULL &&
            ss_info_prop_ptr->stack_info[MM_STACK_0].gen_info != NULL)
    {
      ss_info_prop_ptr->stack_info[MM_STACK_0].gen_info->is_stack_operational = TRUE;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Package the event
  ** Multi-Sim coordinator needs to pass in:
  ** 1. the state_machine pointer
  ** 2. parameters of the command
  ** 3. AS_ID
  */
  event_oprt_mode.event = event;
  event_oprt_mode.state_machine = state_machine;
  event_oprt_mode.oprt_mode = oprt_mode;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Print sim coordinator info */
  cmmsimc_print_simc_info();

  /* Call coordination function */
  cmmsimc_sim_coordinate( (cmmsc_param_base_s_type *)&event_oprt_mode );
}

/*===========================================================================

FUNCTION cmmsimc_proc_cmd_pref_sys_chgd

DESCRIPTION
This function handle preference Change command at the SIM level

DEPENDENCIES
  cmpref_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_cmd_pref_sys_chgd(
  cmmsc_proc_cmd_pref_sys_chgd_s_type  *cmd
)
{
  cmmsc_state_machine_s_type       *state_machine = NULL;
  cmmsimc_event_e_type              event = CMMSIMC_EVENT_PREF_SYS_SIM1;
  cmmsc_param_pref_sys_s_type       event_pref_sys;
  cm_call_id_type                   call_id = CM_CALL_ID_INVALID;

  /* Set event for multi sim case */
  if (cmd->mm_id.asubs_id == SYS_MODEM_AS_ID_2)
  {
    event = CMMSIMC_EVENT_PREF_SYS_SIM2;
  }
#ifdef FEATURE_MMODE_TRIPLE_SIM
  else if (cmd->mm_id.asubs_id == SYS_MODEM_AS_ID_3)
  {
    event = CMMSIMC_EVENT_PREF_SYS_SIM3;
  }
#endif
  else
  {
    event = CMMSIMC_EVENT_PREF_SYS_SIM1;
  }

  /*Package event */
  event_pref_sys.event = event;
  event_pref_sys.state_machine = state_machine;
  event_pref_sys.cmd = cmd;

  /* Trigger Coordinator */
  cmmsimc_sim_coordinate( (cmmsc_param_base_s_type *)&event_pref_sys );

  call_id = cmcall_find_active_cs_call_per_sub(cmd->mm_id.asubs_id);

  /* Generate additional events to sim coord if needed if
  ** 1.no active call pending
  ** 2.The CS call on this sub is the last one pending
  */
  if((call_id == CM_CALL_ID_INVALID) ||
      cmcall_is_last_voice_call_ending(call_id))
  {
    cmmsimc_proc_call_event(cmd->mm_id.asubs_id,
                            CM_CALL_TYPE_UNKNOWN,
                            CM_CALL_EVENT_END);
  }
  /* to make sure that for all error conditions also pref_info memory gets free */
  if(cmd->pref_info != NULL)
  {
    cm_mem_free(cmd->pref_info);
  }
}

/*===========================================================================

FUNCTION cmmsimc_proc_cmd_subsc_chgd

DESCRIPTION
  This function should be called when the subscrption availability has
  changed.This function will pack the info and in turn call the function
  cmmsc_proc_cmd_subsc_chgd during processing based on which subscription.

DEPENDENCIES
  cmmsc_init_multi_stacks() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_cmd_subsc_chgd(

  mmoc_subsc_chg_e_type           chg_type,
  /* Subscription change type.
  */

  byte                            nam,
  /* Current NAM selection (NAM-1, NAM-2, etc.).
  */

  cm_pref_s_type                 **sub_pref_ptr,
  /* array of preference info for subs.
  */

  uint8                           *sub_avail,
  /* array of subscription available mask.
  */

  prot_subsc_chg_e_type           *sub_chg,
  /* array of subscription change mask
  */

  uint8                            size
)
{

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Variable used for Multi-Sim Framework processing */
  cmmsimc_event_e_type              event;
  cmmsc_param_subs_chgd_s_type      event_subs;
  cmph_s_type                      *ph_ptr = cmph_ptr();
  cmmsimc_state_s_type             *sim_coord_state = cmmsimc_state_ptr();
  uint8                             i;

  /* Packed subscription info into array format */
  mmoc_subs_data_s_type            *sub_data[MAX_SIMS];
  uint8                             allocated_sub_data_cnt = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(size == 0)
  {
    return;
  }

  /* Validate params
   ** sub_pref_ptr[1] should be null if num_of_sim = 1
   ** sub_pref_ptr[2] should be null if num_of_sim = 2.
   */
  for (i = 0; i < size; i ++)
  {
    if (i >= cmpmprx_get_num_of_sims() && sub_pref_ptr[i] != NULL)
    {
      CM_ERR_FATAL_3("MSIMC: sub_pref_ptr[%d]=%d, nsim=%d,",
                     i, sub_pref_ptr[i], cmpmprx_get_num_of_sims());
    }

    if (sub_chg[i] != PROT_SUBSC_CHG_NONE)
    {
      break;
    }
  }

  /* Nothing has changed */
  if (i == size)
  {
    CM_MSG_HIGH_0("Ignore due to no change in subscription status");
    return;
  }

  /* Subscription change shouldn't be sent to lower layers other than in ONLINE/PSEUDO_ONLINE,
  LPM,POWER OFF */
  if (!cm_is_valid_state_for_sub_proc(cmph_ptr()->device_prop.ph_oprt_mode.true_oprt_mode))
  {
    CM_MSG_HIGH_1("Ignore subs change due to oprt_mode %d",
                  cmph_ptr()->device_prop.ph_oprt_mode.true_oprt_mode);
    return;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Remove mode as per NV item when moving to triple standby */
#if defined(FEATURE_MMODE_TRIPLE_SIM)
  if( ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY )
  {
    cmph_remove_mode_in_TSTS(size, sub_chg);

    for (i = 0; i< size; i ++)
    {
      if(cmph_is_lte_capable_on_sub((sys_modem_as_id_e_type)i) &&
          sub_pref_ptr[i] != NULL &&
          ph_ptr->sub_info[i])
      {
        CM_MSG_HIGH_3("sub%d:ph_ptr->mode_pref:%d pref mode_pref:%d ",
                      i, ph_ptr->sub_info[i]->pref_info.mode_pref, sub_pref_ptr[i]->mode_pref);
        sub_pref_ptr[i]->mode_pref = ph_ptr->sub_info[i]->pref_info.mode_pref;
      }
    }
  }
#endif

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Pack all sub information to mmoc_data type (subs_data)*/
  for (i = 0; i < MIN(size, ARR_SIZE(sub_data)); i++)
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    sub_data[i] = NULL;
    if(ph_ptr->sub_info[i] == NULL) continue;
    /* NOTIFY CMTASK/CMSS to activate new stacks if needed. */
    if(sub_chg[i] != PROT_SUBSC_CHG_NONE)
    {
      cmtask_update_new_prio_q((sys_modem_as_id_e_type)i, TRUE,
                               SYS_MODEM_AS_ID_ALL_ACTIVE_MASK);
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    if (sub_pref_ptr[i])
    {
      sub_data[i] = (mmoc_subs_data_s_type *)cm_mem_malloc(
                      sizeof(mmoc_subs_data_s_type));
      allocated_sub_data_cnt ++;
      if (sub_data[i])
      {
        memset(sub_data[i], 0, sizeof(mmoc_subs_data_s_type));
      }
      else
      {
        sys_err_fatal_null_ptr_exception();
      }

      cmmsc_form_sub_data_simx( sub_avail[i],
                                sub_pref_ptr[i],
                                sub_data[i],
                                i,
                                chg_type);
    }

    CM_MSG_HIGH_6_ASID_EXT("sub_chgd: cdma %d, gw %d, new sub_avail mask %d, mode_pref %d, domain pref %d",
                           i,
                           ph_ptr->sub_info[i]->is_3gpp2_subs_avail,
                           ph_ptr->sub_info[i]->is_3gpp_subs_avail,
                           sub_avail[i],
                           ph_ptr->sub_info[i]->pref_info.mode_pref,
                           ph_ptr->sub_info[i]->pref_info.srv_domain_pref,
                           (i+1));
  }


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Adjust the coordination */
  cmsimc_set_coordination(sim_coord_state->dev_info.active_subs);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*2. Set Event */
  event = CMMSIMC_EVENT_SUB;

  /*3. Package event */
  event_subs.event    = event;
  event_subs.nam      = nam;
  event_subs.chg_type = chg_type;

#ifdef FEATURE_CM_LTE
  if (chg_type == MMOC_SUBSC_CHG_MODE_CHNG &&
     !cmpmprx_is_ok_to_enforce_device_policy_not_check_mt_page())
  {
    CM_MSG_HIGH_4("maxcap change during active call %d %d %d %d",
                  cmph_is_mt_page_in_prog(),
                  cmph_is_in_emergency_cb(),
                  sim_coord_state->dev_info.active_subs,
                  sim_coord_state->dev_info.standby_pref_chg);
  }
#endif

  /* Pass active_subs to msc so that only sub event is only generated for active ss */
  event_subs.active_subs = sim_coord_state->dev_info.active_subs;


  /* Send previously changed standby pref due to this subs & reset in SIMC */
  event_subs.apply_standby_pref_chg = sim_coord_state->dev_info.standby_pref_chg;
  sim_coord_state->dev_info.standby_pref_chg = FALSE;

  memscpy( event_subs.prot_sub_chg,
           sizeof(prot_subsc_chg_e_type)*size,
           sub_chg,
           sizeof(prot_subsc_chg_e_type)*size );
  memscpy( event_subs.sub_data,
           sizeof(mmoc_subs_data_s_type *)*size,
           sub_data,
           sizeof(mmoc_subs_data_s_type *)*size );

  /* Print sim coordinator info */
  cmmsimc_print_simc_info();

  /*3. Trigger Coordinator */
  cmmsimc_sim_coordinate( (cmmsc_param_base_s_type *)&event_subs );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Free memory for sub_data */
  for (i = 0; i < allocated_sub_data_cnt; i ++)
  {
    if(sub_data[i] != NULL)
    {
      cm_mem_free((void *) sub_data[i]);
      sub_data[i] = NULL;
    }
  }
}

/*===========================================================================

FUNCTION  cmmsimc_proc_rpt_lte_do_irat

DESCRIPTION
This function process the reselection reports from SD20/MMOC at the SIM level.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_rpt_lte_do_irat(
  sys_modem_as_id_e_type   asubs_id
)
{
  cmmsc_param_base_s_type  param;

  /* Update param base on ss */
  param.state_machine = cmmsimc_state_machine_ptr(asubs_id);

  /* Multi-stack process */
  cmmsc_auto_proc_rpt_lte_do_irat(&param);
}

/*===========================================================================

FUNCTION  cmmsimc_proc_rpt_redial_hdr_fail

DESCRIPTION
This function process the faked ACQ_FAIL report from SD20 to trigger 1s BSR at the SIM level.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_rpt_redial_hdr_fail(
  sys_modem_as_id_e_type   asubs_id
)
{
  cmmsc_param_base_s_type  param;

  /* Update param base on ss */
  param.state_machine = cmmsimc_state_machine_ptr(asubs_id);

  /* Multi-stack process */
  cmmsc_auto_proc_rpt_redial_hdr_fail(&param);
}

/*===========================================================================

FUNCTION  cmmsimc_proc_disable_hybr_hdr

DESCRIPTION
This function process disable hybr hdr at the SIM level.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_disable_hybr_hdr(
  sys_modem_as_id_e_type   asubs_id
)
{
  cmmsc_param_base_s_type  param;

  /* Update param base on ss */
  param.state_machine = cmmsimc_state_machine_ptr(asubs_id);

  /* Multi-stack process */
  cmmsc_auto_proc_disable_hybr_hdr(&param);
}

/*===========================================================================

FUNCTION  cmmsimc_proc_enable_hybr_hdr

DESCRIPTION
This function process enable hybr hdr at the SIM level.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void  cmmsimc_proc_enable_hybr_hdr (
  sys_modem_as_id_e_type   asubs_id,
  boolean                        send_pref_sys_cmd
  /* Flag to indicate if pref sys changed command has to be sent
      */
)
{
  cmmsc_param_proc_enable_hybr_hdr_s_type  param;

  /* Update param base on ss */
  param.state_machine = cmmsimc_state_machine_ptr(asubs_id);

  /* Package additional params */
  param.send_pref_sys_cmd = send_pref_sys_cmd;

  /* Multi-stack process */
  cmmsc_auto_proc_enable_hybr_hdr(&param );
}

/*===========================================================================

FUNCTION  cmmsimc_proc_call_end

DESCRIPTION
This function process call end at the SIM level.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_call_end(
  sys_modem_as_id_e_type   asubs_id
)
{
  cmmsc_param_base_s_type  param;

  /* Update param base on ss */
  param.state_machine = cmmsimc_state_machine_ptr(asubs_id);

  /* Multi-stack process */
  cmmsc_auto_proc_call_end(&param);
}

/*===========================================================================

  DESCRIPTION:

  This is called when any type of call event occurs in CMCALL module.
  Based on the ss and mapping as_id, event is converted to SIM based.
  This event is sent to SIMC statemachine, where depending current
  requirements, either the call event is ignored or routing to msc.

===========================================================================*/
void cmmsimc_proc_call_event(
  sys_modem_as_id_e_type   asubs_id,
  cm_call_type_e_type call_type,
  cm_call_event_e_type call_event
)
{
  cmmsc_param_call_ind_s_type call_ind;

  if (asubs_id == SYS_MODEM_AS_ID_2)
  {
    call_ind.event = CMMSIMC_EVENT_CALL_SIM2;
  }
#ifdef FEATURE_MMODE_TRIPLE_SIM
  else if (asubs_id == SYS_MODEM_AS_ID_3)
  {
    call_ind.event = CMMSIMC_EVENT_CALL_SIM3;
  }
#endif
  else
  {
    call_ind.event = CMMSIMC_EVENT_CALL_SIM1;
  }

  call_ind.call_event = call_event;
  call_ind.call_type = call_type;
  call_ind.state_machine = cmmsimc_state_machine_ptr(asubs_id);

  cmmsimc_sim_coordinate((cmmsc_param_base_s_type *)&call_ind);
}

/*===========================================================================

FUNCTION  cmmsimc_proc_timer

DESCRIPTION
This function process timer expiration at the SIM level.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_timer(void)
{
  size_t i = 0;
  cmmsc_param_base_s_type   param;

  for(i = 0; i < cmpmprx_get_num_of_sims(); i++)
  {
    /* Update param base on ss */
    param.state_machine = cmmsimc_state_machine_ptr((sys_modem_as_id_e_type)i);

    /* Multi-stack process */
    cmmsc_auto_proc_timer(&param);
  }
}

/*===========================================================================

FUNCTION  cmmsimc_set_esr_state

DESCRIPTION
This function updates the esr_state of given sub

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_set_esr_state(
  sys_modem_as_id_e_type    esr_sub,
  cmsimc_esr_status_e_type  esr_state
)
{
  cmmsimc_state_s_type  *simc = cmmsimc_state_ptr();
  
  if(((esr_sub == SYS_MODEM_AS_ID_1) ||
      (esr_sub == SYS_MODEM_AS_ID_2))
      && (simc != NULL)
    )

  {
    simc->sim[esr_sub].esr_state = esr_state;
  }
}

/*===========================================================================

FUNCTION  cmmsimc_get_esr_state

DESCRIPTION
This function fetches the esr_state of given sub

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  ESR state for a give sub.

SIDE EFFECTS
  None.
===========================================================================*/
cmsimc_esr_status_e_type cmmsimc_get_esr_state(
  sys_modem_as_id_e_type    esr_sub
)
{
  cmmsimc_state_s_type  *simc = cmmsimc_state_ptr();
  cmsimc_esr_status_e_type esr_state = CMSIMC_ESR_NONE;
  
  if(((esr_sub == SYS_MODEM_AS_ID_1) ||
      (esr_sub == SYS_MODEM_AS_ID_2))
      && (simc != NULL)
    )
  {
    esr_state = simc->sim[esr_sub].esr_state;
  }

  return esr_state;
}

/*===========================================================================

FUNCTION  cmmsimc_is_tau_pending

DESCRIPTION
This function checks, if UE sent ESR and waiting for TAU to happen.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  returns pending TAU status.

SIDE EFFECTS
  None.
===========================================================================*/
boolean cmmsimc_is_tau_pending()
{
  cmmsimc_state_s_type  *simc = cmmsimc_state_ptr();
  cmsimc_esr_status_e_type esr_state = CMSIMC_ESR_NONE;
  uint8 i = 0;

  if (simc == NULL)
  {
   return FALSE;
  }
  
  for(i=0; i<MAX_SIMS; i++)
  {
    if (cmmsimc_get_esr_state(i) == CMSIMC_ESR_TAU_PENDING)
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*===========================================================================

FUNCTION  cmmsimc_proc_rpt_hybr_bsr_to_hdr

DESCRIPTION
This function process CM_HYBR_BSR_TO_HDR_F rpt at the SIM level.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void  cmmsimc_proc_rpt_hybr_bsr_to_hdr (
  sys_modem_as_id_e_type     asubs_id,
  sd_hybr_bsr_to_hdr_s_type  hybr_bsr_to_hdr_status
)
{
  cmmsc_param_proc_rpt_hybr_bsr_to_hdr_s_type param;

  /* Update param base on ss */
  param.state_machine = cmmsimc_state_machine_ptr(asubs_id);

  /* Package additional params */
  param.hybr_bsr_to_hdr_status = hybr_bsr_to_hdr_status;

  /* Multi-stack process */
  cmmsc_auto_proc_rpt_hybr_bsr_to_hdr(&param );
}

/*===========================================================================

FUNCTION  cmmsimc_proc_rpt_kick_hybr2

DESCRIPTION
This function process CM_KICK_HYBR2_F rpt at the SIM level.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_rpt_kick_hybr2(
  sys_modem_as_id_e_type   asubs_id
)
{
  cmmsc_param_base_s_type     param;

  /* Update param base on ss */
  param.state_machine = cmmsimc_state_machine_ptr(asubs_id);

  /* Multi-stack process */
  cmmsc_auto_proc_rpt_kick_hybr2(&param );
}

/*===========================================================================
FUNCTION cmmsimc_proc_cmd_standby_pref_chgd

DESCRIPTION
  This function should be called when the operating mode of the phone has
  changed.This function should be called just before sending the operating
  mode changed command to MMOC.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_cmd_standby_pref_chgd(
  sys_modem_dual_standby_pref_e_type    standby_pref,
  /**< New standby preference of the phone */

  uint8                                 active_subs_id,
  /**< New active subscription when the standby preference is single */

  uint8                                *subsc_status,
  /**< Array of 3gpp & 3gpp2 subscription bitmask for each sub */

  uint8                                 subsc_status_size,
  /**< Size of subsc_status array */

  boolean                               force_pref_standby_change_to_mmoc

)
{
  cmmsimc_state_s_type             *cmmsimc_state = cmmsimc_state_ptr();
  cmph_s_type                      *ph_ptr = cmph_ptr();
  cmss_s_type                      *ss_ptr = cmss_ptr();
  cmmsc_param_base_s_type           param;

  boolean                           is_subs_chgd = FALSE;
#ifdef FEATURE_MMODE_TRIPLE_SIM
  prot_subsc_chg_e_type             sub_chg[MAX_SIMS];
  sys_modem_dual_standby_pref_e_type prev_standby_pref = cmmsimc_state->dev_info.standby_pref;
  sys_modem_dual_standby_pref_e_type new_standby_pref = ph_ptr->device_prop.msim_prop.internal_standby_pref;
#endif
  uint8                               i = 0;

  for(i = 0; i < subsc_status_size; i++)
  {
    CM_MSG_HIGH_2("MSIMC: standby_chgd: asubs_id %d, subsc avail mask %d",
                  i,
                  subsc_status[i]);
  }

  CM_MSG_HIGH_6("MSIMC: standby_chgd: oprt_mode %d, oprt_mode_send_time %d, standby pref %d -> %d, active_subs %d, refresh sub mask %d",
                ph_ptr->device_prop.ph_oprt_mode.oprt_mode,
                ph_ptr->device_prop.ph_oprt_mode.oprt_mode_send_time,
                cmmsimc_state->dev_info.standby_pref,
                standby_pref,
                active_subs_id,
                cmph_get_subs_with_mcfg_refresh_ongoing());

  /* If subscription has changed, do not send device mode from standby pref proc */
  for(i = 0; i < subsc_status_size && ph_ptr->sub_info[i]; i++)
  {
    if((subsc_status[i] & MASK_GPP2) != ph_ptr->sub_info[i]->is_3gpp2_subs_avail ||
        (subsc_status[i] & MASK_GPP) != ph_ptr->sub_info[i]->is_3gpp_subs_avail)
    {
      is_subs_chgd = TRUE;
    }
  }

  /* send the standby preferences to MMOC
  ** In case if subscription changed and standby pref was buffered during a call,
  ** no need to send explicit standby change command to mmoc as subscription change
  ** will be able to handle
  */
  if((((ph_ptr->device_prop.msim_prop.user_standby_pref != SYS_MODEM_DS_PREF_AUTO &&
        ph_ptr->device_prop.msim_prop.user_standby_pref != SYS_MODEM_DS_PREF_AUTO_NO_TUNEAWAY) ||
      ((ph_ptr->device_prop.msim_prop.is_standby_pref_buffered || force_pref_standby_change_to_mmoc)) ||
      (cmmsimc_state->dev_info.device_mode != cmpmprx_get_device_mode() &&
        is_subs_chgd == FALSE)))&&
     !cmpmprx_is_device_mask_set(PM_CHG_MAX_MODE_CAP,NULL)
     && !ph_ptr->device_prop.msim_prop.is_mmoc_standby_pref_not_req)
  {
    /* If we are moving to triple standby, remove modes from multimode sub */
#ifdef FEATURE_MMODE_TRIPLE_SIM
    if(new_standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY)
    {
      cmph_remove_mode_in_TSTS(ARR_SIZE(sub_chg), sub_chg);
    }
#endif

    CM_MSG_HIGH_5( "MSIMC: standby_chgd: CM->MMOC, user_standby_pref %d is_standby_pref_buffered %d oprt_mode %d force to mmoc %d is_subs_chgd %d",
                   ph_ptr->device_prop.msim_prop.user_standby_pref,
                   ph_ptr->device_prop.msim_prop.is_standby_pref_buffered,
                   cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode,
                   force_pref_standby_change_to_mmoc,
                   is_subs_chgd);

#ifdef FEATURE_CM_DEBUG_BUFFER
    {
      CMDBG_MEM_ALLOC(cm_mmoc_cmd_buff, cm_mmoc_dual_standby_cmd_entry_s_type)

      cm_mmoc_cmd_buff->standby_pref   = standby_pref;
      cm_mmoc_cmd_buff->active_subs = active_subs_id;
      cm_mmoc_cmd_buff->device_mode   = cmpmprx_get_device_mode();
      cm_mmoc_cmd_buff->is_mcfg_deact = FALSE;
      cm_mmoc_cmd_buff->current_active_subs = cmmsimc_state->dev_info.active_subs;
      cm_mmoc_cmd_buff->current_internal_standby_pref = cmmsimc_state->dev_info.standby_pref;
      cm_mmoc_cmd_buff->refresh_sub_mask = cmph_get_subs_with_mcfg_refresh_ongoing();
      cm_mmoc_cmd_buff->is_standby_pref_buffered = ph_ptr->device_prop.msim_prop.is_standby_pref_buffered;
      cmdbg_add_to_dbg_buffer(CM_BUFF_MMOC_CMD_TYPE, MMOC_CMD_DUAL_STANDBY_CHGD, ((void *)cm_mmoc_cmd_buff));
    }
#endif

    mmoc_cmd_dual_standby_chgd(standby_pref, active_subs_id, cmpmprx_get_device_mode());

    /* If we are moving from triple standby to dual/single standby,
    ** add modes to multimode sub */
#ifdef FEATURE_MMODE_TRIPLE_SIM
    if(prev_standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY)
    {
      cmph_add_mode_in_DSDX();
    }
#endif

    ph_ptr->device_prop.msim_prop.is_standby_pref_buffered = FALSE;
  }

  cmmsimc_state->dev_info.device_mode = cmpmprx_get_device_mode();
  if(cmmsimc_state->dev_info.standby_pref == standby_pref &&
      cmmsimc_state->dev_info.active_subs == active_subs_id)
  {
    CM_MSG_HIGH_0("MSIMC: standby_chgd: No change");
    return;
  }

  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  cmdbg_qsh_send_standby_pref_change_evts();
  #endif
  
  /* Update the standby preferences to be used in subscription handling */
  cmmsimc_state->dev_info.standby_pref = standby_pref;
  cmmsimc_state->dev_info.active_subs = active_subs_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If ph is not Online no need to update state machine
  */
  if (!( ph_ptr->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_ONLINE ||
         ph_ptr->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_PSEUDO_ONLINE )||
      ph_ptr->device_prop.ph_oprt_mode.oprt_mode_send_time != CMPH_MAX_UPTIME )
  {
    return;
  }

  /* Even if we are not sending corresponding cmd to MMOC, active subs will get updated via Max Cap switch,
  ** thus num standby req processing need to be incremented */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* This part setting the standby preference for STANDBY_PREF CHANGE */
  cmsimc_set_coordination(cmmsimc_state->dev_info.active_subs);
  cmmsimc_state->dev_info.standby_pref_chg = TRUE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  param.event = CMMSIMC_EVENT_STANDBY_PREF_CHGD;

  /* Print sim coordinator info */
  cmmsimc_print_simc_info();

  /* Trigger Coordinator */
  cmmsimc_sim_coordinate(&param );

  /* Update the current priority sub if required */
#if defined FEATURE_MMODE_TRIPLE_SIM
  if(cmpmprx_get_device_mode() == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY)
  {
    cmph_update_curr_priority_sub();
  }
#endif
}

/*===========================================================================
FUNCTION cmmsimmsc_proc_mcfg_deact

DESCRIPTION
  This function should be called when a Sub need to be de-activated for
  MCFG Refresh

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimmsc_proc_mcfg_deact(
  uint8                                 deact_subs_mask
  /**< Subs which need to be deactivated */
)
{
  cmph_s_type                      *ph_ptr = cmph_ptr();
  uint8                 active_subs_4_mmoc = 0;

  active_subs_4_mmoc = ph_ptr->device_prop.msim_prop.active_subs;
  if (cmph_is_mcfg_refresh_ongoing(SYS_MODEM_AS_ID_1))
  {
    active_subs_4_mmoc &= ~SYS_MODEM_AS_ID_1_MASK;
  }
  if (cmph_is_mcfg_refresh_ongoing(SYS_MODEM_AS_ID_2))
  {
    active_subs_4_mmoc &= ~SYS_MODEM_AS_ID_2_MASK;
  }

  CM_MSG_HIGH_6("MCFG_Refresh: deact_subs %d, active_subs %d, user_standby_pref %d, internal_standby %d, is_standby_buff %d, mmoc_active_subs %d",
                cmph_get_subs_with_mcfg_refresh_ongoing(),
                ph_ptr->device_prop.msim_prop.active_subs,
                ph_ptr->device_prop.msim_prop.user_standby_pref,
                ph_ptr->device_prop.msim_prop.internal_standby_pref,
                ph_ptr->device_prop.msim_prop.is_standby_pref_buffered,
                active_subs_4_mmoc);

#ifdef FEATURE_CM_DEBUG_BUFFER
  {
    CMDBG_MEM_ALLOC(cm_mmoc_cmd_buff, cm_mmoc_dual_standby_cmd_entry_s_type)

    cm_mmoc_cmd_buff->standby_pref = ph_ptr->device_prop.msim_prop.internal_standby_pref;
    cm_mmoc_cmd_buff->active_subs = active_subs_4_mmoc;
    cm_mmoc_cmd_buff->device_mode = cmpmprx_get_device_mode();
    cm_mmoc_cmd_buff->is_mcfg_deact = TRUE;
    cm_mmoc_cmd_buff->current_active_subs = ph_ptr->device_prop.msim_prop.active_subs;
    cm_mmoc_cmd_buff->current_internal_standby_pref = ph_ptr->device_prop.msim_prop.internal_standby_pref;
    cm_mmoc_cmd_buff->refresh_sub_mask = cmph_get_subs_with_mcfg_refresh_ongoing();
    cm_mmoc_cmd_buff->is_standby_pref_buffered = ph_ptr->device_prop.msim_prop.is_standby_pref_buffered;
    cmdbg_add_to_dbg_buffer(CM_BUFF_MMOC_CMD_TYPE, MMOC_CMD_DUAL_STANDBY_CHGD, ((void *)cm_mmoc_cmd_buff));
  }
#endif

  mmoc_cmd_dual_standby_chgd(ph_ptr->device_prop.msim_prop.internal_standby_pref, active_subs_4_mmoc, cmpmprx_get_device_mode());

}


/*===========================================================================

DESCRIPTION
  Process acquisition fail report from system selection layer. After reporting
  this indication, system selection either continues with second round of
  acquisition or put access layers in pwr save.

  This function is going to decide which subscription should consume the rpt.
===========================================================================*/
void cmmsimmsc_proc_rpt_acq_fail(
  mm_sub_stk_id_s_type     mm_id,
  sd_ss_mode_pref_e_type   mode_pref
)
{
  cmmsc_param_base_s_type  param;

  param.state_machine = cmmsimc_state_machine_ptr(mm_id.asubs_id);


  cmmsc_auto_proc_acq_fail_rpt(param.state_machine, mm_id.stk_id, mode_pref);
}

/*===========================================================================

DESCRIPTION
  Identify subscription
  invoke corresponding MSC

===========================================================================*/
void cmmsimmsc_proc_rpt_srv_ind(
  cmmsc_param_srv_ind_s_type *srv_ind
)
{
  cmmsc_auto_proc_srv_ind_rpt(srv_ind->state_machine,
                              srv_ind->rpt_ptr,
                              srv_ind->prev_srv_status);
}

/*===========================================================================

FUNCTION cmmsimmsc_proc_rpt_3gpp_stopped

DESCRIPTION
  This function should be called to process stop mode cnf.

DEPENDENCIES
  cmmsimc_state_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void cmmsimmsc_proc_rpt_3gpp_stopped(
  mm_sub_stk_id_s_type     mm_id
)
{
  cmmsc_param_base_s_type  param;

  param.state_machine = cmmsimc_state_machine_ptr(mm_id.asubs_id);
  cmmsc_auto_proc_3gpp_stopped_rpt(param.state_machine, mm_id.stk_id);
}

/*===========================================================================

FUNCTION cmmsimc_proc_rpt_srv_ind

DESCRIPTION
  This function should be called to process the CM_SRV_IND_INFO_F report from SD20

DEPENDENCIES
  cmpref2_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsimc_proc_rpt_srv_ind(
  const cm_sd_rpt_u_type         *rpt_ptr,
  /* Pointer to the CM_SRV_IND_INFO_F report */

  sys_srv_status_e_type           prev_srv_status
)
{
  cmmsimc_event_e_type           event = CMMSIMC_EVENT_NONE;
  cmmsc_param_srv_ind_s_type     event_srv_ind;

  CM_ASSERT( rpt_ptr != NULL);
  CM_ASSERT( rpt_ptr->hdr.cmd == CM_SRV_IND_INFO_F );

  CM_MSG_HIGH_6("RPT->MSIMC: srv_ind: ss %d, srv %d, mode %d, is_pref_srv_acq %d, realign_hdr %d, realign_lte %d",
                rpt_ptr->srv_ind_info.stack_id,
                rpt_ptr->srv_ind_info.si_info.srv_status,
                rpt_ptr->srv_ind_info.si_info.mode,
                rpt_ptr->srv_ind_info.si_info.is_pref_srv_acq,
                rpt_ptr->srv_ind_info.si_info.realign_hdr,
                rpt_ptr->srv_ind_info.si_info.realign_lte);

  /* Do not process service ind if oprt mode is not ONLINE */
  if(cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode != SYS_OPRT_MODE_ONLINE &&
      cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode != SYS_OPRT_MODE_PSEUDO_ONLINE)
  {
    CM_MSG_HIGH_1("RPT->MSIMC: srv_ind: Not processed due to oprt_mode= %d",
                  cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode);
    return;
  }

  /* Decide event */
  if (rpt_ptr->hdr.asubs_id == SYS_MODEM_AS_ID_2)
  {
    event = CMMSIMC_EVENT_SRV_SIM2;
  }
#ifdef FEATURE_MMODE_TRIPLE_SIM
  else if (rpt_ptr->hdr.asubs_id == SYS_MODEM_AS_ID_3)
  {
    event = CMMSIMC_EVENT_SRV_SIM3;
  }
#endif
  else
  {
    event = CMMSIMC_EVENT_SRV_SIM1;
  }

  /* Pack the event */
  event_srv_ind.event = event;
  event_srv_ind.state_machine = NULL;
  event_srv_ind.rpt_ptr = rpt_ptr;
  event_srv_ind.prev_srv_status = prev_srv_status;

  /* Trigger Coordinator */
  cmmsimc_sim_coordinate((cmmsc_param_base_s_type *)&event_srv_ind );
}

/* ============================== ACTION FUNCTIONS ============================ */
void cmmsimc_act_route_request(
  cmmsc_param_base_s_type   *event_package
)
{
  cmmsimc_event_e_type event;

  if(event_package == NULL)
  {
    sys_err_fatal_null_ptr_exception();
  }

  event = event_package->event;

  switch (event)
  {
    case CMMSIMC_EVENT_ONLINE:
    case CMMSIMC_EVENT_OFFLINE:
    {
      cmmsc_param_oprt_mode_s_type *oprt_mode_param =
        (cmmsc_param_oprt_mode_s_type *)event_package;
      cmmsc_proc_cmd_oprt_mode_chgd(oprt_mode_param);
      break;
    }

    case CMMSIMC_EVENT_SUB:
    {
      cmmsc_param_subs_chgd_s_type *subs_param =
        (cmmsc_param_subs_chgd_s_type *)event_package;
      cmmsc_proc_cmd_subsc_chgd(subs_param);
      break;
    }

    case CMMSIMC_EVENT_PREF_SYS_SIM1:
    case CMMSIMC_EVENT_PREF_SYS_SIM2:
    case CMMSIMC_EVENT_PREF_SYS_SIM3:
    {
      cmmsc_param_pref_sys_s_type *pref_sys_param =
        (cmmsc_param_pref_sys_s_type *)event_package;
      cmmsc_proc_cmd_pref_sys_chgd(pref_sys_param);
      break;
    }

    case CMMSIMC_EVENT_SRV_SIM1:
    case CMMSIMC_EVENT_SRV_SIM2:
    case CMMSIMC_EVENT_SRV_SIM3:
    {
      cmmsc_param_srv_ind_s_type *srv_ind =
        (cmmsc_param_srv_ind_s_type *)event_package;
      cmmsimmsc_proc_rpt_srv_ind(srv_ind);
      break;
    }

    case CMMSIMC_EVENT_SUSPEND_RESUME:
    {
      cmmsc_param_suspend_resume_subs_s_type *suspend_resume_param =
        (cmmsc_param_suspend_resume_subs_s_type *)event_package;
      cmmsc_proc_cmd_suspend_resume(suspend_resume_param);
      break;
    }

    default:
      break;
  }

  return;
}

void cmmsimc_act_power_down(
  cmmsc_param_oprt_mode_s_type   *event_package
)
{
  cmmsimc_state_s_type          *cmmsimc_ptr = cmmsimc_state_ptr();

  if(event_package == NULL )
  {
    sys_err_fatal_null_ptr_exception();
  }

  cmmsimc_ptr->dev_info.active_subs = 0;
  cmmsimc_ptr->dev_info.coordination = CMMSIMC_COORDINATION_NONE;

  /* We don't re-initialize these:
  ** 1. cmmsimc_ptr->ss_with_mm
  ** 2. cmmsimc_ptr->standby_pref
  ** 3. cmmsimc_ptr->msc_machine
  **    -MSC will clear the state_machine
  **    -This avoids deallocation and reallocation of memory
  */

  /* Initialize all SIM */
  cmmsimc_sim_init();

  /* Forward the command to cmmsc */
  cmmsc_proc_cmd_oprt_mode_chgd(event_package);
}

void cmmsimc_act_on_off(
  cmmsimc_action_e_type     action,
  cmmsc_param_base_s_type *event_package
)
{
  if(event_package == NULL )
  {
    sys_err_fatal_null_ptr_exception();
  }

  if (action == CMMSIMC_SIM_ACT_ON)
  {
    cmmsc_proc_cmd_on_off(FALSE, event_package);
  }
  else if (action == CMMSIMC_SIM_ACT_OFF)
  {
    cmmsc_proc_cmd_on_off(TRUE, event_package);
  }
}

void cmmsimc_send_mmoc_subsc_chgd(
  cmmsc_param_base_s_type   *event_package
)
{
  cmmsc_param_subs_chgd_s_type *subs_change =
    (cmmsc_param_subs_chgd_s_type *)event_package;
  size_t                        sim_idx;
  uint8                         size = ARR_SIZE(subs_change->sub_data);
  mmoc_subs_data_s_type        *sub_data_arr[3] = {NULL, NULL, NULL};
  prot_subsc_chg_e_type         sub_chg_mask_arr[3]
    = {PROT_SUBSC_CHG_NONE, PROT_SUBSC_CHG_NONE, PROT_SUBSC_CHG_NONE};
  cmmsc_state_machine_op_mode_e_type  op_mode;
  uint8                         active_subs = cmph_remove_mcfg_refresh_subs(subs_change->active_subs);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Apply filter on SIM information - As this filter is not acting  */
  for (sim_idx = 0; sim_idx < MIN(size, (cmpmprx_get_num_of_sims())); sim_idx++ )
  {
    cmmsc_state_machine_s_type *simc = cmmsimc_state_machine_ptr_per_sim_idx(sim_idx);

    if(subs_change->sub_data[sim_idx] == NULL || simc == NULL)
    {
      continue;
    }

    cmmsc_update_sub_data_per_cap(subs_change->sub_data[sim_idx], sim_idx);
    op_mode = cmmsimc_state_machine_ptr_per_sim_idx(sim_idx)->op_mode;

    /* Update ue_mode */
    subs_change->sub_data[sim_idx]->ue_mode = simc->stack_common_info.ue_mode;
    subs_change->sub_data[sim_idx]->rat_capability = simc->stack_common_info.ue_mode_capability;
    if (cmph_is_subs_feature_mode_srlte((sys_modem_as_id_e_type)sim_idx))
    {
      subs_change->sub_data[sim_idx]->is_ue_mode_substate_srlte =
        (op_mode == CMMSC_OP_MODE_AUTO_MAIN_HYBR1_HYBR2);
    }
    else
    {
      subs_change->sub_data[sim_idx]->is_ue_mode_substate_srlte = FALSE;
    }

    /* set hybr pref to none if non 1x sub  or hybr not allowed */
    if( !cmmsc_is_msc_multi_stacks((sys_modem_as_id_e_type)sim_idx) )
    {
      subs_change->sub_data[sim_idx]->hybr_pref = SD_SS_HYBR_PREF_NONE;
    }

    subs_change->sub_data[sim_idx]->is_perso_locked = FALSE;

    sub_data_arr[sim_idx] = subs_change->sub_data[sim_idx];
    sub_chg_mask_arr[sim_idx] = subs_change->prot_sub_chg[sim_idx];

    CM_MSG_HIGH_6("CM_SUB_DATA: sim_idx %d, op_mode %d, hybr_pref %d, ue_mode %d, is_ue_mode_substate_srlte %d, pm rat_cap 0x%x",
                  sim_idx,
                  op_mode,
                  subs_change->sub_data[sim_idx]->hybr_pref,
                  subs_change->sub_data[sim_idx]->ue_mode,
                  subs_change->sub_data[sim_idx]->is_ue_mode_substate_srlte,
                  simc->stack_common_info.ue_mode_capability);

    CM_MSG_HIGH_6_EXT("CM_SUB_DATA: mode_pref=0x%x, domain %d, msc domain_cap %d, SUBCAP 0x%x, prot_subsc_chg %d, active_stacks %d",
                      subs_change->sub_data[sim_idx]->mode_pref,
                      subs_change->sub_data[sim_idx]->srv_domain_pref,
                      cmmsc_get_msc_domain_cap(sim_idx),
                      subs_change->sub_data[sim_idx]->subs_capability,
                      subs_change->sub_data[sim_idx]->prot_subsc_chg,
                      subs_change->sub_data[sim_idx]->active_stacks,
                      (subs_change->sub_data[sim_idx]->as_id+1));
  }

#ifdef FEATURE_CM_DEBUG_BUFFER
  cmdbg_add_subs_cmd_to_buffer(subs_change->chg_type,
                               subs_change->nam,
                               active_subs,
                               cmpmprx_get_device_mode(),
                               sub_chg_mask_arr[0],
                               sub_chg_mask_arr[1],
                               sub_chg_mask_arr[2],
                               sub_data_arr[0],
                               sub_data_arr[1],
                               sub_data_arr[2]
                              );

#endif

  /* Send sub chg command to mmoc */
  mmoc_cmd_subscription_chgd_new(subs_change->chg_type,
                                 subs_change->nam,
                                 active_subs,
                                 cmmsimc_state_ptr()->dev_info.standby_pref,
                                 cmpmprx_get_device_mode(),
                                 sub_chg_mask_arr[0],
                                 sub_chg_mask_arr[1],
                                 sub_chg_mask_arr[2],
                                 sub_data_arr[0],
                                 sub_data_arr[1],
                                 sub_data_arr[2]
                                );

}

void cmmsimc_act_wake_up(
  int                          sim_idx,
  cmmsc_param_srv_ind_s_type  *event_package
)
{
  cmmsimc_state_s_type           *cmmsimc_ptr = cmmsimc_state_ptr();
  sys_srv_status_e_type           srv_status = event_package->rpt_ptr->srv_ind_info.si_info.srv_status;
  sys_srv_status_e_type           curr_srv_status = SYS_SRV_STATUS_NO_SRV;
  mm_sub_stk_id_s_type            mm_id = cmph_map_sub_stk_to_id((sys_modem_as_id_e_type)sim_idx,
                                          MM_STACK_0);
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop((sys_modem_as_id_e_type)sim_idx);
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_ptr;

  /* Only wake up a stack if it's ACTIVE */
  if (cmmsimc_ptr->sim[sim_idx].state != CMMSIMC_SIM_STATE_ACTIVE)
  {
    return;
  }
  if(ss_info_sub_prop_ptr == NULL ||
      ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info == NULL)
  {
    return;
  }
  ss_info_all_stack_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info;

  /* Also, only wake up if we found service.
  ** If NOT(Full Service) || Is Still Acquiring More Preferred Service)
  */
  if(!sys_srv_status_is_full_srv( srv_status ) ||
      event_package->rpt_ptr->srv_ind_info.si_info.is_pref_srv_acq)
  {
    return;
  }

  curr_srv_status = ss_info_all_stack_ptr->srv_status;

  /* Conditions to wake up a SIM:
  ** - If the current stack is not in pwr save
  */
  if((curr_srv_status == SYS_SRV_STATUS_PWR_SAVE ) &&
      (event_package->prev_srv_status == SYS_SRV_STATUS_PWR_SAVE ||
       event_package->prev_srv_status == SYS_SRV_STATUS_PWR_SAVE_INTERNAL ))
  {
    CM_MSG_HIGH_1("MSIMC: Wake up simx_idx=%d", sim_idx);
  }
  else
  {
    return;
  }

  mmoc_cmd_wakeup_from_pwr_save(mm_id);
}

/*===========================================================================

DESCRIPTION:

  Action function to handle when call events occur in peer subs.
  For e.g. is a call is ended on SUB2 and SIMC needs to fwd the evt to SUB0
  then this action/call used.

===========================================================================*/
void cmsimc_act_peer_subs_call(
  cmmsc_param_call_ind_s_type  *call_ind,
  size_t                        sim_idx
)
{
  cmmsc_auto_proc_peer_subs_call(call_ind->state_machine, call_ind);

  /* If TAU is due, then perform TAU on SIM2 call end */
  if(cmmsimc_get_esr_state(sim_idx) == CMSIMC_ESR_TAU_PENDING &&
     call_ind->call_event == CM_CALL_EVENT_END)
  {
#ifdef FEATURE_LTE_TO_1X
    cmcsfbcall_send_mm_dual_rx_1xcsfb_tau_req(sim_idx);
#endif
    cmmsimc_set_esr_state(sim_idx, CMSIMC_ESR_NONE);
  }
}

/*===========================================================================

FUNCTION cmmsimc_send_subsc_chgd_current_pref

DESCRIPTION
  This function should be called when the subscrptions need to be refreshed
  using the current preferences on both the stacks.This function should be
  called just before sending the subsc_chgd command to MMOC.

DEPENDENCIES
  cmpref2_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void    cmmsimc_send_subsc_chgd_current_pref(
  uint8                    size,
  prot_subsc_chg_e_type   *sub_chg
)
{
  cmph_s_type *ph_ptr = cmph_ptr();
  cm_pref_s_type       *sub_pref_ptr[MAX_SIMS];
  uint8                 sub_avail[MAX_SIMS];
  uint8 i = 0;
  uint8                 alloc_cnt = 0;

  if (!cmph_is_msim())
  {
    CM_ERR_0("not multi-sim");
    return;
  }

  if(size > MAX_SIMS)
  {
    return;
  }

  /* Get phone sub availability array*/
  cmph_get_ph_sub_avail_info(size, sub_avail);


  /* Get the top preference array */
  alloc_cnt = cmph_get_top_sub_pref(size, sub_pref_ptr);

  /* Inform CM SIM cordinator & MMOC that subscription changed */
  cmmsimc_proc_cmd_subsc_chgd( MMOC_SUBSC_CHG_MODE_CHNG,
                               (byte)ph_ptr->device_prop.curr_nam,
                               sub_pref_ptr,
                               sub_avail,
                               sub_chg,
                               size);

  /* Free allocated buffer */
  if (alloc_cnt != 0)
  {
    for (i = 0; i< size && i < alloc_cnt; i ++)
    {
      if (sub_pref_ptr[i] != NULL) cm_mem_free (sub_pref_ptr[i]);
    }
  }
} /* cmmsimc_send_subsc_chgd_current_pref */

#ifdef FEATURE_MMODE_DUAL_SIM

/*===========================================================================

FUNCTION cmmsimc_suspend_or_resume_other_subs
DESCRIPTION
  Function to suspend or resume other SS

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmmsimc_suspend_or_resume_other_subs(
  sys_modem_as_id_e_type  asubs_id,
  boolean                 suspend
)
{
  sys_modem_as_id_e_type other_subs;
  cmmsimc_event_e_type                   event;
  cmmsc_param_suspend_resume_subs_s_type      event_suspend_resume;

  CM_MSG_HIGH_3("SUSPEND_RESUME: E911: subs_id %d, int_standby_pref %d, suspend %d",
                   asubs_id,
                   cmph_ptr()->device_prop.msim_prop.internal_standby_pref,
                   suspend);

  if (!IS_VALID_PH_SUB_PTR(asubs_id))
  {
    return;
  }

  if( cmph_ptr()->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY)
  {
    return;
  }

  other_subs = (asubs_id == SYS_MODEM_AS_ID_2)?SYS_MODEM_AS_ID_1:SYS_MODEM_AS_ID_2;

  if (!IS_VALID_PH_SUB_PTR(other_subs))
  {
    return;
  }

  /* Condition is needed for case when CM has yet to receive suspend CNF from SD */
  if(cmph_ptr()->sub_info[other_subs]->ss_susp_info[MM_STACK_0].is_suspend == suspend &&
      !cmph_ptr()->sub_info[other_subs]->ss_susp_info[MM_STACK_0].num_requests)
  {
    CM_MSG_HIGH_2("SUSPEND_RESUME: sub %d Stack 0 already in the expected suspend state %d", asubs_id, suspend);
    return;
  }

  /* Reset policyman acq fail when resuming a sub as we start scans from powerup */
  if(cmph_ptr()->sub_info[other_subs]->ss_susp_info[MM_STACK_0].is_suspend &&
     !suspend)
  {
    policyman_reset_acq_fail(other_subs, BM(SYS_MODEM_STACK_ID_1) | BM(SYS_MODEM_STACK_ID_2));
  }

  event = CMMSIMC_EVENT_SUSPEND_RESUME;

  event_suspend_resume.event       = event;
  event_suspend_resume.asubs_id    = other_subs;
  event_suspend_resume.suspend     = suspend;

  CM_MSG_HIGH_3("SUSPEND_RESUME: E911: asubs_id %d, suspend %d sub %d",
                  asubs_id, suspend, other_subs);

  cmmsimc_sim_coordinate( (cmmsc_param_base_s_type *)&event_suspend_resume );
}/* cmmsimc_suspend_or_resume_other_subs */
#endif /* FEATURE_MMODE_DUAL_SIM */

/*===========================================================================

FUNCTION cmmsimc_get_msc_stack_state

DESCRIPTION
  Get state of a stack from MSC

===========================================================================*/
cmmsc_stack_state_e_type  cmmsimc_get_msc_stack_state(
  mm_sub_stk_id_s_type  mm_id
)
{
  cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(mm_id.asubs_id);

  if (state_machine)
  {
    return state_machine->stack[mm_id.stk_id].state;
  }
  else
  {
    return CMMSC_STACK_STATE_NULL;
  }
}

/*===========================================================================

FUNCTION cmmsc_is_in_srv_state

DESCRIPTION
  The function returns if the stack is in service.

DEPENDENCIES
  None

RETURN VALUE
  boolean.

SIDE EFFECTS
  None

===========================================================================*/
boolean cmmsimc_is_in_srv(
  mm_sub_stk_id_s_type  mm_id
)
{
  return (cmmsimc_get_msc_stack_state(mm_id) == CMMSC_STACK_STATE_SRV);
}

/*===========================================================================

FUNCTION cmmsimc_is_esr_on_call_required

DESCRIPTION
  The function determines if the est is needed for 1xsxlte sub when GSM on the other sub
  is originating a call.

DEPENDENCIES
  None

RETURN VALUE
  boolean.

SIDE EFFECTS
  None

===========================================================================*/
boolean cmmsimc_is_esr_on_call_required(
  const cmcall_s_type     *call_ptr,
  sys_modem_as_id_e_type  *esr_sub
)
{
  sd_si_info_s_type *ss_true_stk_info_ptr = NULL;
  cmmsc_state_machine_s_type *state_machine = NULL;

  /* mm_id for lte sub and stack */
  mm_sub_stk_id_s_type    srlte_mm_id;

  /* Necessity of sending ESR is controlled by NV. If device supports
     dedicated RF between LTE and voice RAT of another subscription then
     this NV should be disabled */
  if(!cmph_is_msim_standby_esr_enabled())
  {
    CM_MSG_HIGH_0("ESR not enabled");
    return FALSE;
  }

  srlte_mm_id.asubs_id = cmmsc_get_1x_sub();

  if(srlte_mm_id.asubs_id != cmph_ptr()->device_prop.msim_prop.default_data_subs)
  {
    return FALSE;
  }

#ifdef FEATURE_LTE_TO_1X
  if(!cmss_lte_srv_on_sub(&srlte_mm_id))
#endif
  {
    return FALSE;
  }

  state_machine = cmmsimc_state_machine_ptr(srlte_mm_id.asubs_id);

  /* As of now, ESR to be sent in SRLTE+G and VOLTE+G mode
  */
  if(!(cmph_is_subs_feature_mode_1x_sxlte(srlte_mm_id.asubs_id) &&
       (state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED ||
        state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_ONLY) &&
          (cmss_is_stack2_operational(srlte_mm_id.asubs_id) ||
            (cmph_is_msim_standby_volte_esr_enabled(srlte_mm_id.asubs_id) && 
             (cmsds_get_csfb_type_allowed(srlte_mm_id.asubs_id) == CM_CALL_CSFB_TYPE_NONE))) )
    )
  {
    CM_MSG_HIGH_0("ESR not required, sub1 not in SRLTE/VOLTE mode");
    return FALSE;
  }
  #if defined(FEATURE_DOMAIN_SELECTION)
  if(!(cmsds_cm_supported_rat(srlte_mm_id.asubs_id) & SD_SS_MODE_PREF_ANY_BUT_HDR_LTE))
  {
    CM_MSG_HIGH_0("ESR not required, sub1 effective mode pref does not support cs rats");
    return FALSE;
  }
  #endif
  /* call being sent on LTE stack , ESR not needed here.
  ** since this ESR is to suspend LTE for voice on different stack
  ** this check is to make sure that CSFB calls on SUB1 do not trigger
  ** ESR procedure for msim case
  */
  if (call_ptr->mm_id.asubs_id == srlte_mm_id.asubs_id)
  {
    return FALSE;
  }

#if defined (FEATURE_3GPP_CSFB) || defined (FEATURE_LTE_TO_1X) || defined FEATURE_IP_CALL
  /* for CSFB E911 calls we do not need to do ESR from here */
  if(call_ptr->orig_mode == SD_SS_ORIG_MODE_PPCSFB_EMERG_ORIG ||
      call_ptr->orig_mode == SD_SS_ORIG_MODE_1XCSFB_EMERG_ORIG)
  {
    return FALSE;
  }
#endif

  ss_true_stk_info_ptr = cmss_true_stack_info_ptr(srlte_mm_id);
  if (ss_true_stk_info_ptr != NULL &&
      ss_true_stk_info_ptr->srv_status != SYS_SRV_STATUS_SRV)
  {
    return FALSE;
  }

#ifdef FEATURE_CM_LTE
  /* if lte service found ss is not active, do not do ESR */
  if(cmregprx_get_state(srlte_mm_id) != CMREGPRX_STATE_ACTIVE)
#endif
  {
    return FALSE;
  }

  if(!cmph_is_subs_feature_mode_srlte(srlte_mm_id.asubs_id))
  {
    return FALSE;
  }

  if(cmmsimc_get_esr_state(srlte_mm_id.asubs_id) != CMSIMC_ESR_NONE)
  {
    CM_MSG_HIGH_2("skip ESR, state %d, call id %d",
                  cmmsimc_get_esr_state(srlte_mm_id.asubs_id), call_ptr->call_id);
    return FALSE;
  }


  if (esr_sub != NULL)
  { 
  *esr_sub = srlte_mm_id.asubs_id;
    CM_MSG_HIGH_2("MSIM decides to do ESR, call id %d esr_sub %d", call_ptr->call_id, *esr_sub);
  }

  return TRUE;
}

/*===========================================================================
  Sends ESR request when RF used by LTE is going to be occupied by a higher
  priority voice RAT from another subscription due to voice call orig.
===========================================================================*/
void cmmsimc_send_esr_req(
  const cmcall_s_type     *call_ptr,
  sys_modem_as_id_e_type   esr_sub
)
{
#ifdef FEATURE_LTE_TO_1X
  cmcsfbcall_send_esr_req(call_ptr, esr_sub, TRUE);
  cmmsimc_set_esr_state(esr_sub, CMSIMC_ESR_RSP_PENDING);
#endif
}

/*===========================================================================
  This is called on recving MM_CM_1XCSFB_CALL_RSP. If call was waiting to
  due to ESR response, then MSIM call orig is resumed.
===========================================================================*/
void cmmsimc_proc_rpt_esr_rsp(
  uint8 context_id
)
{
  cmcall_s_type *call_ptr;
  sys_modem_as_id_e_type    esr_sub;
  
  CM_MSG_HIGH_1("MSIM: esr rsp recvd call-id %d", context_id);

  call_ptr = cmcall_ptr ( context_id );

  if(call_ptr == NULL) return;

  if(!call_ptr->is_waiting_on_esr_rsp || call_ptr->direction != CM_CALL_DIRECTION_MO) return;

  call_ptr->is_waiting_on_esr_rsp = FALSE;

  if((call_ptr->sr.is_ok_to_orig) &&
     (call_ptr->sr.orig_sent_on_mode == SYS_SYS_MODE_NONE))
  {
    /* Handle for IP call */
    if(call_ptr->cmcall_mode_info.info_type == CM_CALL_MODE_INFO_IP)
    {
      cmcall_send_ipcall_orig(call_ptr);
    }
    #if defined (FEATURE_MMODE_CDMA_800)|| defined (FEATURE_MMODE_CDMA_1900)
    /* Handle for CDMA call */
    else if(call_ptr->cmcall_mode_info.info_type == CM_CALL_MODE_INFO_CDMA)
    {
      cmxcall_send_orig(call_ptr);
    }
    #endif
    /* Handle for GW call */
    else
    {
#ifdef CM_GW_SUPPORTED
      cmwcall_send_orig( call_ptr );    
#else
      CM_MSG_HIGH_1("Call %d dropped, mSIM without GW", call_ptr->call_id);
#endif
    }
  }
  
  if(call_ptr->mm_id.asubs_id == SYS_MODEM_AS_ID_1)
  {
    esr_sub = SYS_MODEM_AS_ID_2;
  }
  else
  {
    esr_sub = SYS_MODEM_AS_ID_1;
  }
  cmmsimc_set_esr_state(esr_sub, CMSIMC_ESR_TAU_PENDING);
}

/* ============================================================================ */
/* ==========================END ULTILITIES FUNCTIONS ========================= */
/* ============================================================================ */
