/****************************************************************************
 * Copyright (c) 2014 Qualcomm Technologies, Inc. All Rights Reserved.
 *
 * FILE:            core/debugtrace/cti/dal/src/DalCTI.c
 *
 * DESCRIPTION:     DAL Implementation for CTI.
 *
 *
 * EDIT HISTORY FOR MODULE
 * --------------------------------------------------------------------------
 *
 * when         who         what, where, why
 * ----------   -------     -----------------
 * 2014-01-24   rpoddar     Initial revision.
 *
 *****************************************************************************/

/****************************************************************************
 * Include Files
 ****************************************************************************/

#include "stringl/stringl.h"
#include "DalDevice.h"
#include "DALFramework.h"

#include "qdss_throw.h"
#include "qdss_constants.h"

#include "DDICTI.h"
#include "halqdss_cti.h"
#include "cti_config.h"

/****************************************************************************
 * Preprocessor Definitions and Constants
 ****************************************************************************/

#define CTI_PAGE_SIZE 0x1000

/****************************************************************************
 * Typedefs & Function Prototypes
 ****************************************************************************/

/* Device, Driver, and Client contexts */
typedef struct CTIDevCtxt       CTIDevCtxt;
typedef struct CTIDrvCtxt       CTIDrvCtxt;
typedef struct CTIClientCtxt    CTIClientCtxt;

/* Declaring a private "CTI" Vtable */
typedef struct CTIDALVtbl CTIDALVtbl;

DALResult CTI_DeviceAttach(const char *pszArg,
                                DALDEVICEID DeviceId,
                                DalDeviceHandle **phDalDevice);

static void CTI_InitInterface(CTIClientCtxt *pClientCtxt);

/*
 * Helper function for CTI_Reset since CTI_Reset must handle the case
 * where the client wants to reset 'all' blocks at once.
 */
static DALResult _CTI_Reset_Block(DalDeviceHandle *h, const char *block);

/*
 * Helper function for CTI_Map and CTI_Unmap functions since the code
 * for both is essentially the same.
 */
static DALResult _CTI_MapUnmap(DalDeviceHandle *h,
                                const char *block,
                                uint8 type,
                                uint8 trigger,
                                uint8 channel,
                                uint8 enable);

/*
 * Sets up the base address in the HAL to be the virtual address of the
 * specified block. Must be called before any other HAL operations.
 */
static DALResult _CTI_SetBaseAddress(DalDeviceHandle *h, const char *block);

/* Returns the virtual address of the specified block. */
static DALResult _CTI_GetVirtAddr(DalDeviceHandle *h,
                                    const char *block,
                                    uint32 *virt_addr);

/****************************************************************************
 * Data Structures
 ****************************************************************************/

/* Memory map information for a single block */
struct CTIMemMap
{
    char            cti_name[CTI_BLOCK_SIZE];
    DALSYSMemHandle mem_handle;
};

/* CTI Vtable */
struct CTIDALVtbl
{
    int (*CTI_DriverInit)(CTIDrvCtxt *);
    int (*CTI_DriverDeInit)(CTIDrvCtxt *);
};

/* Device context */
struct CTIDevCtxt
{
    /* Base Members */
    uint32      dwRefs;
    DALDEVICEID DevId;
    uint32      dwDevCtxtRefIdx;
    CTIDrvCtxt  *pCTIDrvCtxt;
    DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
    uint32      Reserved[16];
    /* CTI device state can be added by developers here */
    DALSYSSyncHandle sync;
    /* Keep an array of memory maps for all CTI blocks in order to properly
     * handle memory allocation & mappings.
     */
    struct CTIMemMap *cti_mem_maps;
    uint8       cti_mem_maps_len;
    uint8       num_channels;
    uint8       num_triggers;
};

/* Driver context */
struct CTIDrvCtxt
{
    /* Base Members */
    CTIDALVtbl  CTIDALVtbl;
    uint32      dwNumDev;
    uint32      dwSizeDevCtxt;
    uint32      bInit;
    uint32      dwRefs;
    CTIDevCtxt  CTIDevCtxt[1];
    /* CTI driver state can be added by developers here */
};

/* CTI Client Context */
struct CTIClientCtxt
{
    /* Base Members */
    uint32          dwRefs;
    uint32          dwAccessMode;
    void            *pPortCtxt;
    CTIDevCtxt      *pCTIDevCtxt;
    DalCTIHandle    DalCTIHandle;
    /* CTI Client state can be added by developers here */
};

/* Info File */
const DALREG_DriverInfo DALCTI_DriverInfo = { CTI_DeviceAttach, 0, NULL};

/****************************************************************************
 * CTI Function Definitions
 ****************************************************************************/

DALResult CTI_Map(uint32 ClientHandle, DalDeviceHandle *h,
                        const char *block,
                        uint8 type,
                        uint8 trigger,
                        uint8 channel)
{
    DALResult   err;
    CTIDevCtxt  *pDevCtxt;

    pDevCtxt = (CTIDevCtxt *) ((DALClientCtxt *) (h->pClientCtxt))->pDALDevCtxt;

    DALSYS_SyncEnter(pDevCtxt->sync);

    /* Note that a block of 'all' is not supported for 'map' */
    TRY(err, _CTI_MapUnmap(h, block, type, trigger, channel, TRUE));

    CATCH(err) {}

    DALSYS_SyncLeave(pDevCtxt->sync);

    return err;
}

DALResult CTI_Unmap(uint32 ClientHandle, DalDeviceHandle *h,
                        const char *block,
                        uint8 type,
                        uint8 trigger,
                        uint8 channel)
{
    DALResult   err;
    CTIDevCtxt  *pDevCtxt;

    pDevCtxt = (CTIDevCtxt *) ((DALClientCtxt *) (h->pClientCtxt))->pDALDevCtxt;

    DALSYS_SyncEnter(pDevCtxt->sync);

    /* Note that a block of 'all' is not supported for 'unmap' */
    TRY(err, _CTI_MapUnmap(h, block, type, trigger, channel, FALSE));

    CATCH(err) {}

    DALSYS_SyncLeave(pDevCtxt->sync);

    return err;
}

DALResult CTI_Reset(uint32 ClientHandle, DalDeviceHandle *h, const char *block)
{
    DALResult       err = DAL_CTI_SUCCESS;
    CTIDevCtxt      *pDevCtxt;
    uint32          i;

    pDevCtxt = (CTIDevCtxt *) ((DALClientCtxt *) (h->pClientCtxt))->pDALDevCtxt;

    DALSYS_SyncEnter(pDevCtxt->sync);

    /* User can reset all CTI blocks at once */
    if (0 == strcmp(block, "all")) {
        for (i = 0; i < pDevCtxt->cti_mem_maps_len; i++) {
            TRY(err, _CTI_Reset_Block(h, pDevCtxt->cti_mem_maps[i].cti_name));
        }
    } else {
        TRY(err, _CTI_Reset_Block(h, block));
    }

    CATCH(err) {}

    DALSYS_SyncLeave(pDevCtxt->sync);

    return err;
}

DALResult CTI_Query(uint32 ClientHandle, DalDeviceHandle *h,
                        const char *block,
                        uint8   type,
                        uint8   max_triggers,
                        uint8   *length,
                         uint32 *mappings)
{
    DALResult   err;
    CTIDevCtxt  *pDevCtxt;
    uint8       trigger;

    pDevCtxt = (CTIDevCtxt *) ((DALClientCtxt *) (h->pClientCtxt))->pDALDevCtxt;

    DALSYS_SyncEnter(pDevCtxt->sync);

    if (TRIGIN != type && TRIGOUT != type) {
        THROW(err, DAL_CTI_INVALID_TYPE);
    }

    /* Note that a block of 'all' is not supported for 'query' */
    TRY(err, _CTI_SetBaseAddress(h, block));

    if (pDevCtxt->num_triggers > max_triggers) {
        /* Exit since our the length of mappings[] is too small */
        THROW(err, DAL_CTI_ERROR);
    }

    *length = pDevCtxt->num_triggers;

    for(trigger = 0; trigger < pDevCtxt->num_triggers; trigger++) {
        if (TRIGIN == type) {
            mappings[trigger] = HAL_qdss_cti_Get_CTIINEN(trigger);
        } else {
            mappings[trigger] = HAL_qdss_cti_Get_CTIOUTEN(trigger);
        }
    }

    CATCH(err) {}

    DALSYS_SyncLeave(pDevCtxt->sync);

    return err;
}

DALResult CTI_Trigger(uint32 ClientHandle, DalDeviceHandle *h,
                    const char *block,
                    uint8 channel,
                    uint8 trigger_type)
{
    DALResult   err;
    CTIDevCtxt  *pDevCtxt;

    pDevCtxt = (CTIDevCtxt *) ((DALClientCtxt *) (h->pClientCtxt))->pDALDevCtxt;

    if (channel >= pDevCtxt->num_channels) {
        return DAL_CTI_INVALID_CHANNEL;
    }

    DALSYS_SyncEnter(pDevCtxt->sync);

    TRY(err, _CTI_SetBaseAddress(h, block));
    HAL_qdss_cti_UnlockAccess();

    err = DAL_SUCCESS;
    switch(trigger_type) {
    case 1:
       HAL_qdss_cti_AppSet(channel);
       break;

    case 2:
       HAL_qdss_cti_AppClear(channel);
       break;

    case 3:
       HAL_qdss_cti_AppPulse(channel);
       break;

    default:
       err =  DAL_CTI_INVALID_TYPE;
    }
    

    HAL_qdss_cti_LockAccess();
    CATCH(err) {}

    DALSYS_SyncLeave(pDevCtxt->sync);

    return err;
}



static DALResult _CTI_MapUnmap(DalDeviceHandle *h,
                                const char *block,
                                uint8 type,
                                uint8 trigger,
                                uint8 channel,
                                uint8 enable)
{
    DALResult   err;
    CTIDevCtxt  *pDevCtxt;

    pDevCtxt = (CTIDevCtxt *) ((DALClientCtxt *) (h->pClientCtxt))->pDALDevCtxt;

    if (TRIGIN != type && TRIGOUT != type) {
        return DAL_CTI_INVALID_TYPE;
    } else if (trigger >= pDevCtxt->num_triggers) {
        return DAL_CTI_INVALID_TRIGGER;
    } else if (channel >= pDevCtxt->num_channels) {
        return DAL_CTI_INVALID_CHANNEL;
    }

    TRY(err, _CTI_SetBaseAddress(h, block));

    HAL_qdss_cti_UnlockAccess();

    if (TRIGIN == type) {
        HAL_qdss_cti_Set_CTIINEN(enable, trigger, channel);
    } else {
        HAL_qdss_cti_Set_CTIOUTEN(enable, trigger, channel);
    }

    if (enable) {
        HAL_qdss_cti_Enable();
    }

    /* Disabling CTI requires keeping state as to whether every single
     * input and output trigger has been unmapped. So, we will avoid
     * doing this and not disable CTI if user has manually unmapped every
     * single trigger.
     */

    HAL_qdss_cti_LockAccess();

    CATCH(err) {}

    return err;
}

static DALResult _CTI_Reset_Block(DalDeviceHandle *h, const char *block)
{
    CTIDevCtxt  *pDevCtxt;
    uint8       trigger;
    DALResult   err;

    pDevCtxt = (CTIDevCtxt *) ((DALClientCtxt *) (h->pClientCtxt))->pDALDevCtxt;

    TRY(err, _CTI_SetBaseAddress(h, block));

    HAL_qdss_cti_UnlockAccess();

    for(trigger = 0; trigger < pDevCtxt->num_triggers; trigger++) {
        HAL_qdss_cti_Reset_CTIINEN(trigger);
        HAL_qdss_cti_Reset_CTIOUTEN(trigger);
    }

    /* Note: Only disabling CTI on Reset(). If all triggers are unmapped
     * manually, we do not disable CTI.
     */
    HAL_qdss_cti_Disable();

    HAL_qdss_cti_LockAccess();

    CATCH(err) {}

    return err;
}

static DALResult _CTI_SetBaseAddress(DalDeviceHandle *h, const char *block)
{
    DALResult   err;
    uint32      virt_addr;

    TRY(err, _CTI_GetVirtAddr(h, block, &virt_addr));
    HAL_qdss_cti_SetBaseAddress(virt_addr);

    CATCH(err) {}

    return err;
}

static DALResult _CTI_GetVirtAddr(DalDeviceHandle *h,
                                  const char *block,
                                  uint32 *virt_addr)
{
    CTIDevCtxt          *pDevCtxt;
    uint32              i;
    DALSYSMemInfo       mem_info;

    pDevCtxt = (CTIDevCtxt *)
               ((DALClientCtxt *) (h->pClientCtxt))->pDALDevCtxt;

    for (i = 0; i < pDevCtxt->cti_mem_maps_len; i++) {
        if (!strncmp(block,
                     pDevCtxt->cti_mem_maps[i].cti_name,
                     CTI_BLOCK_SIZE)) {
            if (0 != DALSYS_MemInfo(pDevCtxt->cti_mem_maps[i].mem_handle,
                                    &mem_info)) {
                break;
            }
            *virt_addr = mem_info.VirtualAddr;
            return DAL_CTI_SUCCESS;
        }
    }

    /* Did not find block name in cti_mem_maps array */
    *virt_addr = 0;
    return DAL_CTI_INVALID_BLOCK;
}

/****************************************************************************
 * DalDevice DAL Interface Functions Definitions.
 ****************************************************************************/

static uint32 CTI_AddRef(DalCTIHandle *h)
{
    return DALFW_AddRef((DALClientCtxt *) (h->pClientCtxt));
}

DALResult CTI_DriverInit(CTIDrvCtxt *pDrvCtxt)
{
    return DAL_CTI_SUCCESS;
}

DALResult CTI_DriverDeInit(CTIDrvCtxt *pDrvCtxt)
{
    return DAL_CTI_SUCCESS;
}

DALResult CTI_DeviceAttach(const char *pszArg,
                                DALDEVICEID DeviceId,
                                DalDeviceHandle **phDalDevice)
{
    DALResult err;
    static CTIDrvCtxt drvCtxt = {{CTI_DriverInit, CTI_DriverDeInit},
                                    1,
                                    sizeof(CTIDevCtxt)};
    static CTIClientCtxt clientCtxt;

    CTIClientCtxt *pClientCtxt = &clientCtxt;

    *phDalDevice = NULL;

    TRY(err, DALFW_AttachToStringDevice(pszArg,
                                        (DALDrvCtxt *) &drvCtxt,
                                        (DALClientCtxt *) pClientCtxt));

    CTI_InitInterface(pClientCtxt);
    CTI_AddRef(&(pClientCtxt->DalCTIHandle));
    *phDalDevice = (DalDeviceHandle *) &(pClientCtxt->DalCTIHandle);

    CATCH(err) {}

    return err;
}

static uint32 CTI_DeviceDetach(uint32 ClientHandle, DalDeviceHandle *h)
{
    uint32 dwRef = DALFW_Release((DALClientCtxt *) (h->pClientCtxt));
    return dwRef;
}

static DALResult CTI_DeviceInit(uint32 ClientHandle, DalDeviceHandle *h)
{
    DALResult           err;
    DALSYSPropertyVar   prop_var;
    DALClientCtxt       *pClientCtxt;
    CTIDevCtxt          *pDevCtxt;

    uint32                      i;
    struct CTIPhysAddrMap       *cti_phys_addrs;
    struct CTIPhysAddrMapLen    *cti_phys_addrs_len;

    pClientCtxt = (DALClientCtxt *) (h->pClientCtxt);
    pDevCtxt    = (CTIDevCtxt *) pClientCtxt->pDALDevCtxt;

    TRY(err, DALSYS_GetDALPropertyHandleStr(
                pClientCtxt->pDALDevCtxt->strDeviceName,
                pDevCtxt->hProp));

    TRY(err, DALSYS_GetPropertyValue(
                pDevCtxt->hProp,
                "num_channels",
                0,
                &prop_var));
    pDevCtxt->num_channels = prop_var.Val.dwVal;

    TRY(err, DALSYS_GetPropertyValue(
                pDevCtxt->hProp,
                "num_triggers",
                0,
                &prop_var));
    pDevCtxt->num_triggers = prop_var.Val.dwVal;

    /* Map physical addresses for all CTI blocks at initialization.
     * We could dynamically map addresses as they are used, but it would lead
     * to more complicated code. Possible optimization for future.
     */
    TRY(err, DALSYS_GetPropertyValue(
                pDevCtxt->hProp,
                "cti_phys_addrs_len",
                0,
                &prop_var));
    cti_phys_addrs_len = (struct CTIPhysAddrMapLen * ) prop_var.Val.pStruct;

    TRY(err, DALSYS_GetPropertyValue(
                pDevCtxt->hProp,
                "cti_phys_addrs",
                0,
                &prop_var));
    cti_phys_addrs = (struct CTIPhysAddrMap * ) prop_var.Val.pStruct;

    pDevCtxt->cti_mem_maps_len = cti_phys_addrs_len->len;

    TRY(err,
        DALSYS_Malloc(sizeof(struct CTIMemMap) * pDevCtxt->cti_mem_maps_len,
                        (void **) &pDevCtxt->cti_mem_maps));

    memset(pDevCtxt->cti_mem_maps,
            0x0,
            sizeof(struct CTIMemMap) * pDevCtxt->cti_mem_maps_len);

    for (i = 0; i < pDevCtxt->cti_mem_maps_len; i++) {
        if (strlcpy(pDevCtxt->cti_mem_maps[i].cti_name,
                    cti_phys_addrs[i].cti_name,
                    CTI_BLOCK_SIZE) >= CTI_BLOCK_SIZE)
        {
            THROW(err, DAL_CTI_ERROR);
        }

        TRY(err, DALSYS_MemRegionAlloc(
                    DALSYS_MEM_PROPS_HWIO,
                    DALSYS_MEM_ADDR_NOT_SPECIFIED,
                    cti_phys_addrs[i].phys_addr,
                    CTI_PAGE_SIZE,
                    &pDevCtxt->cti_mem_maps[i].mem_handle,
                    NULL));

    }

    TRY(err,
        DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE, &pDevCtxt->sync, NULL));

    /* Ensure our sync handle is not NULL */
    if (0 == pDevCtxt->sync) {
        THROW(err, DAL_CTI_ERROR);
    }

    CATCH(err) {}

    return err;
}

static DALResult CTI_DeviceDeInit(uint32 ClientHandle, DalDeviceHandle *h)
{
    DALResult   err;
    CTIDevCtxt  *pDevCtxt;
    uint32      i;

    pDevCtxt = (CTIDevCtxt *) ((DALClientCtxt *) (h->pClientCtxt))->pDALDevCtxt;

    if (NULL != pDevCtxt->sync) {
        TRY(err, DALSYS_DestroyObject(pDevCtxt->sync));
    }

    if (NULL == pDevCtxt->cti_mem_maps) {
        return DAL_CTI_SUCCESS;
    }

    for (i = 0; i < pDevCtxt->cti_mem_maps_len; i++) {
        if (NULL != pDevCtxt->cti_mem_maps[i].mem_handle) {
            TRY(err,
                DALSYS_DestroyObject(pDevCtxt->cti_mem_maps[i].mem_handle));
        }
    }

    TRY(err, DALSYS_Free(pDevCtxt->cti_mem_maps));

    CATCH(err) {}

    return err;
}

static DALResult CTI_DeviceOpen(uint32 ClientHandle, DalDeviceHandle* h, uint32 mode)
{
   return DAL_CTI_SUCCESS;
}

static DALResult CTI_DeviceClose(uint32 ClientHandle, DalDeviceHandle* h)
{
   return DAL_CTI_SUCCESS;
}

static DALResult CTI_DeviceInfo(uint32 ClientHandle, DalDeviceHandle* h,
                                    DalDeviceInfo* info,
                                    uint32 info_size)
{
   info->Version = DALCTI_STRING_INTERFACE_VERSION;
   return DAL_CTI_SUCCESS;
}

static DALResult CTI_DevicePowerEvent(uint32 ClientHandle, DalDeviceHandle *h,
                                        DalPowerCmd PowerCmd,
                                        DalPowerDomain PowerDomain)
{
   return DAL_CTI_SUCCESS;
}

static DALResult CTI_DeviceSysRequest(uint32 ClientHandle, DalDeviceHandle* h,
                                        DalSysReq ReqIdx,
                                        const void* SrcBuf,
                                        uint32 SrcBufLen,
                                        void* DestBuf,
                                        uint32 DestBufLen,
                                        uint32* DestBufLenReq)
{
   return DAL_CTI_SUCCESS;
}

static void CTI_InitInterface(CTIClientCtxt *clientCtxt)
{
    static const struct DalCTI vtbl =
    {
        {
            CTI_DeviceAttach,
            CTI_DeviceDetach,
            CTI_DeviceInit,
            CTI_DeviceDeInit,
            CTI_DeviceOpen,
            CTI_DeviceClose,
            CTI_DeviceInfo,
            CTI_DevicePowerEvent,
            CTI_DeviceSysRequest
        },

        CTI_Map,
        CTI_Unmap,
        CTI_Reset,
        CTI_Query,
        CTI_Trigger,
    };


    clientCtxt->DalCTIHandle.dwDalHandleId  = DALDEVICE_INTERFACE_HANDLE_ID;
    clientCtxt->DalCTIHandle.pVtbl          = &vtbl;
    clientCtxt->DalCTIHandle.pClientCtxt    = clientCtxt;
}
