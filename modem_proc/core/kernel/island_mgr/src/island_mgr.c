/*======================================================================
                        island_mgr.c 

GENERAL DESCRIPTION
  Island Manager functions

 EXTERNALIZED FUNCTIONS
  none

 INITIALIZATION AND SEQUENCING REQUIREMENTS
  none

 Copyright (c) 2014  by Qualcomm Technologies, Inc.  All Rights Reserved.
 ======================================================================*/
#include <qurt.h>
#include <island_mgr.h>
#include "qdss.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "DALSys.h"
#include "DDIHWIO.h"
#include "msmhwiobase.h"

#define ISLAND_MGR_MAX_ATTRS 32 /* maximum number of attributes */

extern qurt_island_t g_island_spec_id;
extern int g_island_mgr_inited;

qurt_island_attr_resource_t g_uImageIslandData[20] = 
{
    /* Interrupt elements */
    {QURT_ISLAND_ATTR_INT, {{0}}   }, /* PMI */
    {QURT_ISLAND_ATTR_INT, {{2}}   }, /* q6ss_qtmr_irq[0] */
	{QURT_ISLAND_ATTR_INT, {{4}}   }, /* q6ss_qtmr_irq[2] */
	{QURT_ISLAND_ATTR_INT, {{84}}   }, /* rsc_core_comp_irq_0 */
	{QURT_ISLAND_ATTR_INT, {{85}}   }, /* rsc_core_comp_irq_1 */
	{QURT_ISLAND_ATTR_INT, {{86}}   }, /* rsc_rsc_error_irq */
	{QURT_ISLAND_ATTR_INT, {{288}}	}, /* q6ss_wdog_irq */
	{QURT_ISLAND_ATTR_INT, {{290}}	}, /* nav_irq */
	{QURT_ISLAND_ATTR_INT, {{291}}	}, /* nav_dm_irq */

    /* Memory range elements */
    {QURT_ISLAND_ATTR_MEMORY, 
    {{MSS_TOP_BASE, 0x00400000}} }, /* MODEM_TOP used for time tick, sleep, GPS and the MPSS RSC (MSS_RSCC_RSCC_RSC) */

    /* Indicate end of list */
    {QURT_ISLAND_ATTR_END_OF_LIST, {{0}} }
};

#if 0

If there are any physical addresses that need to be converted to virtual for an attr,
use the following code:

DalDeviceHandle *handle;
DALResult result;
uint8 *base_ptr;

      result = DAL_DeviceAttach(DALDEVICEID_HWIO, handle);
      if ( (DAL_SUCCESS != result) || (NULL == handle) ) {
         XXX ERROR XXX
      }
       if ( DAL_SUCCESS !=
             DalHWIO_MapRegion(handle,
                               "RPM_SS_MSG_RAM_START_ADDRESS_BASE",
                               &base_ptr) ) {
         XXX ERROR XXX
      }
   }


#endif

extern int __attribute__((weak)) __tcm_qurt_pa_load_start__;
extern int __attribute__((weak)) __tcm_qurt_pa_load_end__;

int island_mgr_init(void)
{
    int qurt_rc;
    qurt_island_attr_t *island_attr;
    DalDeviceHandle *handle;
    DALResult dal_result;
    unsigned int rpm_ss_msg_ram_base_ptr;

    dal_result = DAL_DeviceAttach(DALDEVICEID_HWIO, &handle);
    if((DAL_SUCCESS != dal_result) || (NULL == handle))
    {
		return ISLAND_MGR_EFAILED;
    }
    if(DAL_SUCCESS != DalHWIO_MapRegion(handle,"RPM_SS_MSG_RAM_START_ADDRESS",(uint8 **)&rpm_ss_msg_ram_base_ptr))
    {
		return ISLAND_MGR_EFAILED;
    }

    qurt_rc = qurt_island_attr_create(&island_attr, ISLAND_MGR_MAX_ATTRS);
    if (qurt_rc != QURT_EOK)
    {
      return ISLAND_MGR_EFAILED;
    }

    qurt_rc = qurt_island_attr_add(island_attr, g_uImageIslandData);
    if (qurt_rc != QURT_EOK)
    {
		qurt_island_attr_delete(island_attr);
		return ISLAND_MGR_EFAILED;
    }

    qurt_rc = qurt_island_attr_add_mem(island_attr, rpm_ss_msg_ram_base_ptr, 0x00007000);
    if (qurt_rc != QURT_EOK)
    {
		qurt_island_attr_delete(island_attr);
		return ISLAND_MGR_EFAILED;
    }

    qurt_rc = qurt_island_attr_add_pool (island_attr, "QURTOS_ISLAND_POOL");
    if (qurt_rc != QURT_EOK)
    {
		qurt_island_attr_delete(island_attr);
		return ISLAND_MGR_EFAILED;
    }

    /* Add only static TCM portion to GPS Island */
    if (&__tcm_qurt_pa_load_start__ && &__tcm_qurt_pa_load_end__) {
        qurt_rc = qurt_island_attr_add_mem(island_attr, (unsigned int)&__tcm_qurt_pa_load_start__, ((unsigned int)&__tcm_qurt_pa_load_end__ - (unsigned int)&__tcm_qurt_pa_load_start__));
        if (qurt_rc != QURT_EOK) {
            qurt_island_attr_delete(island_attr);
            return ISLAND_MGR_EFAILED;
        }
    } else {
        qurt_rc = qurt_island_attr_add_pool (island_attr, "TCM_POOL");
        if (qurt_rc != QURT_EOK)
        {
            qurt_island_attr_delete(island_attr);
            return ISLAND_MGR_EFAILED;
        }
    }

    qurt_rc = qurt_island_spec_create(&g_island_spec_id, island_attr);
    if (qurt_rc != QURT_EOK)
    {
	    qurt_island_attr_delete(island_attr);
		 return ISLAND_MGR_EFAILED;
    }
    qurt_island_attr_delete(island_attr);

    g_island_mgr_inited=1;

    return ISLAND_MGR_EOK;
}
