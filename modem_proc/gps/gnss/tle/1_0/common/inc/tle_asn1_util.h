#ifndef TLE_ASN1UTIL_H
#define TLE_ASN1UTIL_H
/*===========================================================================
                          TLE_ASN1_UTIL.H

DESCRIPTION
  This header file contains function prototypes for the functions in 
  tle_asn1_util.h.

  Copyright (c) 2014-2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 

  Copyright (c) 2015 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  
  Export of this technology or software is regulated
  by the U.S. Government. Diversion contrary to U.S. law prohibited.


  $Header: //components/rel/gnss8.mpss/11.1/gnss/tle/1_0/common/inc/tle_asn1_util.h#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $  
  
  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  09/03/2014  ah      Initial OSYS based version.
  
  
  
===========================================================================*/
/*==============================================================================

                                INCLUDE FILES

==============================================================================*/

#include "comdef.h"
#include "tle_common.h"
#include "tle_asn1.h"
#include "gnss_gdt.h"

/*=============================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

=============================================================================*/


/*=============================================================================

       Constants

=============================================================================*/


/*=============================================================================

       Macros

=============================================================================*/
#define C_TLE_ASN1_DATA_BITMASK0 (0X80)
#define C_TLE_ASN1_DATA_BITMASK1 (0X40)
#define C_TLE_ASN1_DATA_BITMASK2 (0X20)
#define C_TLE_ASN1_DATA_BITMASK3 (0X10)
#define C_TLE_ASN1_DATA_BITMASK4 (0X08)
#define C_TLE_ASN1_DATA_BITMASK5 (0X04)
#define C_TLE_ASN1_DATA_BITMASK6 (0X02)
#define C_TLE_ASN1_DATA_BITMASK7 (0X01)

#define TLE_OSYS_ASN1_MAX_LINE_LENGTH 40 /* max number of chars in a print line*/
#define TLE_OSYS_ASN1_TEMP_BUF_LENGTH 1024 /* Total number of chars in a print message*/

//#define C_TLE_GDT_RECORD_STATUS_MASK      0xFF000000
#define C_TLE_GDT_RECORD_STATUE_ENCRYPTED (0x01)

//#define C_TLE_GDT_RECORD_SERVICE_ID_MASK  0x00FF0000
#define C_TLE_GDT_RECORD_SERVICE_ID_TDP   (0x01)

#define C_TLE_GDT_RECORD_TDP_CLIENT_INFO   0x0001
#define C_TLE_GDT_RECORD_TDP_MOBILE_STATUS 0x0002
#define C_TLE_GDT_RECORD_TDP_CELL_ARRAY    0x0003

#define C_TDP_CDMA_CHIP_RATE_CHIPS_PER_MSEC  1228.8
#define C_TDP_NUM_MSEC_FROM_GREGORIAN_CALENDAR_START_TO_GPS_START    (2524953600000)

/* The following ranges are based on ASN.1 ICD 3.2.0 for ServingCellConfiguration_systemTime */
#define C_TDP_MAX_SYNCH_SYS_TIME_TEN_MSEC  (549755813887) /* In units of 10 msec */
#define C_TDP_MAX_UTC_SYS_TIME_TEN_MSEC    (549755813887) /* In units of 10 msec */
#define C_TDP_MAX_LOC_CONFIDENCE_PERCENTILE (100)
#define C_TDP_MAX_EXTENDED_EARFCN          (0x3FFFF)
#define C_TDP_MAX_LTE_FREQ_EARFCN          (39649)

#define C_TDP_EXTENDED_EARFCN_INVALID           (0xFFFF)
#define C_TDP_SYS_TIME_MASK_SYNCH_TIME_VALID    (0x01)
#define C_TDP_SYS_TIME_MASK_UTC_VALID           (0x02)

/*=============================================================================

       Structures and enumerated types

=============================================================================*/
/* This structure is used to get ASN.1 encoding status when there is any 
   encoding failure */
typedef struct
{
  int32  l_encode_status;      /* Encoding failure cause       */ 
  void   *unencoded_ptr;       /* Unencoded pdu ptr            */
} tle_OsysEncodeInfoType;

/* This structure is used to get ASN.1 decoding status when there is any
   decoding failure */
typedef struct
{
  int32  l_decode_status;      /* Decoding failure cause       */ 
  int32  l_pdu_size;           /* Size of the encoded string   */
  void   *encoded_pdu_ptr;     /* ptr to encoded string        */
  int32  l_decode_length;      /* Decoded length incase pre-allocated
                                  decode buffer is used        */
} tle_OsysDecodeInfoType;


typedef struct
{
  /*  Run-time context structure.  This structure is a container structure 
      that holds all working variables involved in encoding or decoding a
      message */
  OSCTXT z_Contxt; 

  /* Used to get ASN.1 encoding status in case of encoding failure */
  tle_OsysEncodeInfoType z_EncodeInfo;

  /* Used to get ASN.1 decoding status in case of decoding failure */
  tle_OsysDecodeInfoType z_DecodeInfo;
} tle_OsysDynamicDataType;

/* This structure holds the Static part of private data */

typedef struct
{
  uint8 u_Data;
} tle_OsysStaticDataType;


/* ASN1Util Private data sturctured definition */
typedef struct
{
  /* Static part of private data */
  //tle_OsysStaticDataType *pz_StaticData;
  
  /* Dynamic part of private data */
  tle_OsysDynamicDataType *pz_DynData;
} tle_OsysDataType;


/* ASN1 handle type, each caller to functions shall provide 
   a handle in its own TASK context */
typedef struct
{
  tle_OsysDataType  z_osys_Data;
  char print_buffer[TLE_OSYS_ASN1_TEMP_BUF_LENGTH]; 
  boolean b_HandleValid;
} tle_OsysHandleType;

/* enumurate data types that can be encoded/decoded */
typedef enum
{
  C_CLIENT_INFORMATION,
  C_MOBILE_STATUS_DATA,
  C_CELL_RECORD_ARRAY,
  C_CIENT_DOWNLOAD_DATA,
  C_TLE_ASN1_END
}e_TLE_Asn1DataType;

typedef struct
{
  OSINT32 length;
  OSOCTET * value;
} tle_RawAsn1DataBufType;

typedef struct
{
  unsigned char c_Status; /* BYTE1 = RECORD STATUS
                             Bit 0 – Encryption Status (0 – plaintext 1 – encrypted)
                             Bit 1 – 7 Reserved */

  uint8 u_ServiceId;      /* BYTE2 = GDT SERVICE ID
                             0x00 Reserved
                             0x01 GTP TDP
                             0x02 – 0xFF Reserved */

  uint16 w_MsgId;            /* BYTE3 and BYTE4 = Interpretation dependent on GDT SERVICE ID
                            For GDT SERVICE ID 0x01 (GDT TDP):
                            BYTE3 and BYTE4:  shall be interpreted as TDP FILE DATA ID
                            0x00 01 CLIENT INFO (MP to AP)
                            0x00 02 MOBILE STATUS DATA (MP to AP)
                            0x00 03 CELLDB RECORD (MP to AP) */
   
  uint32 q_Length;
  gdt_MemHandle data;
} tle_GdtMsgType;

/* This is a prototype call back function for ASN1 encoder in TLE 
   The encoder will pass the encoded data to this function to be processed */
typedef boolean (*tle_EncoderCbFnType)(const gdt_InterfaceType *, ... );


typedef union
{
  ClientInformation * pz_ClientInfo;
  MobileStatusData  * pz_MobileStatus;
  Cell_Record_Array * pz_CellArray;
  WwanClientDownloadData * pz_ClientDownloadedData;
} tle_OsysEncoderArgUnionType;

/* Generic paramaters that are required by tle ASN1 encoder */
typedef struct
{
  tle_OsysEncoderArgUnionType z_EncodeArg;
  e_TLE_Asn1DataType  e_DataType;
  gdt_InterfaceType *pz_GdtIface;
  tle_EncoderCbFnType CbFnc;
} tle_OsysEncoderArgType;

typedef struct
{
  char  sz_ModemModelId[C_TLE_PROT_MAX_MODEL_ID_LENGTH];
  char  sz_ModemOemId[C_TLE_PROT_MAX_OEM_ID_LENGTH];
  char  sz_ModemSwVersion[C_TLE_PROT_MAX_ASN1_SW_VERSION_LENGTH];
} tle_Asn1ClientInfoDynamicDataType;

typedef struct
{
  ClientInformation                 z_ClientInfoData;
  tle_Asn1ClientInfoDynamicDataType z_ClientInfoDynamicData;
} tle_Asn1ClientInfoDataType;

typedef union
{
  CdmaCellData   z_CdmaCell;
  GsmCellData    z_GsmCell;
  WcdmaCellData  z_WcdmaCell;
  LteCellData    z_LteCell;
}tle_Asn1CellDataUnionType;

typedef union
{
  GsmWcdmaHomeCarrierID z_GsmWcdmaCarrId;
  char sz_CdmaRealm[C_TLE_PROT_MAX_REALM_LENGTH];
}tle_Asn1HomeCarrierIdUnionType;

typedef struct
{
  tle_Asn1CellDataUnionType       z_CellData;
  tle_Asn1HomeCarrierIdUnionType  z_HomeCarrierId;
} tle_Asn1MobileStatusDataDynamicType;

typedef struct
{
  MobileStatusData                    z_MobileStatusData;
  tle_Asn1MobileStatusDataDynamicType z_MobileStatusDynamicData;
  /* Latest position used for logging */
  tle_InputPosStructType              z_LatestPosition; 
} tle_Asn1MobileStatusDataType;

typedef struct
{
  /* SIB8 to be uploaded is in units of 10 msec */
  uint64 t_SynchronousSystemTime;

  /* SIB16 is also in units of 10 msec */
  uint64 t_TimeInfoUtc;
  uint8  u_StatusMask; /* C_TDP_SYS_TIME_MASK_SYNCH_TIME_VALID
                          C_TDP_SYS_TIME_MASK_UTC_VALID */
} tle_TdpSib8Sib16SysTimeType;

/*=============================================================================

       Functions

=============================================================================*/

/*===========================================================================
FUNCTION 
  tle_osys_malloc

DESCRIPTION
  This function is called by the OSS ASN.1 libraries in order to allocate 
  dynamic memory. 

PARAMETERS
 
  size     - The size (in bytes) to be allocated
 (input)
 
DEPENDENCIES
  None.

RETURN VALUE
  A pointer to the allocated memory if the memory was successfully 
  reallocated, otherwise NULL.
 
SIDE EFFECTS
  None.
===========================================================================*/
void *(tle_osys_malloc)(size_t size);

/*===========================================================================
FUNCTION 
  tle_osys_free

DESCRIPTION
  This function is called by the OSS ASN.1 libraries in order to free 
  dynamic memory. 

PARAMETERS
 
  ptr      - Points to the block previously allocated from the heap or NULL pointer
  (input)
 
DEPENDENCIES
  None.

RETURN VALUE
 
SIDE EFFECTS
  None.
===========================================================================*/
void (tle_osys_free)(void *ptr);

/*===========================================================================

  FUNCTION:  tle_asn1_osys_printf

===========================================================================*/
/*!
    @brief
    This is the printf() function for objective systems use.

    @return
    None
*/
/*=========================================================================*/
void tle_asn1_osys_printf(void *stream, const char *format, va_list args);


/*===========================================================================
FUNCTION 
  tle_asn1_ConvTleCell2Asn1Cell

DESCRIPTION
  This function converts a celldb record from TLE format into the ASN.1 format.

PARAMETERS
 
  cpz_XtaUpldCell  - Points to the uploaded celldb record
  (input)
 
  pz_Asn1UpldCell  - Points to the celldb in ASN.1 format
 (output)
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Encode succeed.
  FALSE - Encode failed.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tle_asn1_ConvTleCell2Asn1Cell(const xtt_XtaUpldGenericRecordType * cpz_XtaUpldCell,
                                      CellDB_element * pz_Asn1UpldCell);

/*==============================================================================
  FUNCTION      tle_osys_asn1_free_encode_buf

  DESCRIPTION 	
    This function frees memory that was allocated by tle_osys_asn1_encode_pdu.

  DEPENDENCIES
    None

  PARAMETERS
 
   pz_Handle - Points to the handle
   (input)
 
   pz_buf -   Points to the buffer to be freed
   (input)
 

  RETURN VALUE  

  SIDE EFFECTS  

==============================================================================*/
void tle_osys_asn1_free_encode_buf(tle_OsysHandleType *pz_Handle, tle_RawAsn1DataBufType *pz_buf);


/*==============================================================================
  FUNCTION      tle_osys_asn1_EncodeData

  DESCRIPTION 	
    This function encodes an ASN.1 data type to the raw TLV format
    (tle_RawAsn1DataBufType) and sends that to the corresponding call back
    function. 

  DEPENDENCIES
    None

  PARAMETERS
 
    pz_Handle - Points to the handle for encoder
    (input)
 
    z_Arg    - Points to the unencoded ASN.1 data, and relevant info
    (input)
 
    b_ReportRawData - If TRUE, raw data will be reported in F3 message
    (input)
 
  RETURN VALUE  
    TRUE  - If the encoding is successful
    FALSE - Otherwise

  SIDE EFFECTS  

==============================================================================*/
boolean tle_osys_asn1_EncodeData(tle_OsysHandleType *pz_Handle,
                                 tle_OsysEncoderArgType z_Arg,
                                 boolean b_ReportRawData);

/*==============================================================================
  FUNCTION      tle_osys_asn1_DecodeWwanClientData

  DESCRIPTION 	
    This function decodes an ASN1 encoded WClientDownloadedData.

  DEPENDENCIES
    None

  PARAMETERS
 
    pz_Handle - Points to the handle for the ASN1 decoder
    (input)
 
    pc_EncodedWwanClientData - Points to the WwanClientDownloaded to be injected
    (input)                    in a raw format
 
    encoded_size  - Number of bytes in the raw data
    (input)
 
    pz_DecodedClientInfo - Points to the decoded data
    (output)

  RETURN VALUE  
    TRUE:  Decoding success
    FALSE: Otherwise 

  SIDE EFFECTS  

==============================================================================*/
boolean tle_osys_asn1_DecodeWwanClientData(tle_OsysHandleType *pz_Handle,
                                           OSOCTET * pc_EncodedWwanClientData,
                                           size_t encoded_size,
                                           tle_TdpClientDownloadData *pz_DecodedClientInfo);

/*===========================================================================
FUNCTION 
  tle_osys_init

DESCRIPTION
  This function initializes the ASN.1 OSYS encoder/decoder

PARAMETERS 
  pz_Handle - Points to the context handler
  (input/output)

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  If the Osys encoder/decoder is initialized successfuly
  FALSE: Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean tle_osys_init(tle_OsysHandleType * pz_Handle);

/*===========================================================================
FUNCTION 
  tle_osys_deinit

DESCRIPTION
  This function de-initializes the ASN.1 OSYS encoder/decoder

PARAMETERS 
  pz_Handle - Points to the context handler
  (input/output)

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  If the Osys encoder/decoder is de-initialized successfuly
  FALSE: Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean tle_osys_deinit(tle_OsysHandleType * pz_Handle);

/*==============================================================================
FUNCTION
  tle_asn1_ConvTleWwanClient2Asn1WwanClient

DESCRIPTION
  Converts a WwanDownloadedData to the equivalent ASN1 based data
  
PARAMETERS
  cpz_TdpClient  - Points to the WwanClientDownloadedData to be converted 
  (input)
 
  cpz_Asn1Client  - Points to the converted ASN1 data
  (output)
 

DEPENDENCIES
  None

RETURN VALUE 
  TRUE:  If successful
  FALSE: Otherwise

SIDE EFFECTS
   None

==============================================================================*/
boolean tle_asn1_ConvTleWwanClient2Asn1WwanClient(const tle_TdpClientDownloadData * cpz_TdpClient,
                                                  WwanClientDownloadData * pz_Asn1Client);

/*===========================================================================
FUNCTION 
  tle_asn1_ReportRawData

DESCRIPTION
  This function reports a raw ASN.1 string in F3 message.

PARAMETERS
 
  pz_Handle      - Points to the handler
  (input/output)
 
  e_DataType       - The type of record
  (input)
 
  cpz_RawData  - Points to the raw ASN.1 data to report
 (input)
 
  q_Len        - Number of valid bytes in the raw data 
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE  - Report succeed.
  FALSE - Otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tle_asn1_ReportRawData(tle_OsysHandleType *pz_Handle,
                               e_TLE_Asn1DataType e_DataType, 
                               const OSOCTET * cpz_RawData,
                               uint32 q_Len);

/*===========================================================================
FUNCTION 
  tle_asn1_ConvTdpCellDb2Asn1CellDb

DESCRIPTION
  This function converts a celldb record from TLE format into the ASN.1 format.

PARAMETERS
 
  cpz_TleUpldCell - Points to the uploaded celldb record
  (input)
 
  pz_Asn1UpldCell  - Points to the celldb in ASN.1 format
 (output)
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Encode succeed.
  FALSE - Encode failed.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tle_asn1_ConvTdpCellDb2Asn1CellDb(const tle_TdpCellDbUploadData * const cpz_TdpUpldCell,
                                          CellDB_Record * const pz_Asn1UpldCell);

/*==============================================================================
  FUNCTION      tle_osys_asn1_DecodeClientInfo

  DESCRIPTION 	
    This function decodes a byte stream into an ASN1 encoded client information 

  DEPENDENCIES
    None

  PARAMETERS
 
    pz_Handle - Points to the handle for the ASN1 decoder
    (input)
 
    pc_EncodedClientInfo - Points to the client information to be decoded
    (input)                    in a raw format
 
    encoded_size  - Number of bytes in the raw data
    (input)
 
    pz_DecodedClientInfo - Points to the decoded data
    (output)

  RETURN VALUE  
    TRUE:  Decoding success
    FALSE: Otherwise 

  SIDE EFFECTS  

==============================================================================*/
boolean tle_osys_asn1_DecodeClientInfo(tle_OsysHandleType *pz_Handle,
                                       OSOCTET *pc_EncodedClientInfo,
                                       size_t encoded_size,
                                       ClientInformation *pz_DecodedClientInfo);

/*==============================================================================
  FUNCTION      tle_InitClientInfoStruct

  DESCRIPTION 	
    This function initializes the static and dynamic parts of an ASN.1
    client informaton data structure that can be used for ASN.1 decoding or encoding

  DEPENDENCIES
    None

  PARAMETERS
 
    pz_Asn1Client - Points to the client information data structure to be initialized
    (input)
 
 
  RETURN VALUE  
    TRUE  - If successful
    FALSE - Otherwise

  SIDE EFFECTS

==============================================================================*/
boolean tle_InitClientInfoStruct(tle_Asn1ClientInfoDataType *pz_Asn1Client);

/*==============================================================================
  FUNCTION      tle_InitMobileStausStruct

  DESCRIPTION 	
    This function initializes the static and dynamic parts of an ASN.1
    mobile status data structure that can be used for ASN.1 decoding or encoding

  DEPENDENCIES
    None

  PARAMETERS
 
    pz_Asn1Mobile - Points to the mobile status data structure to be initialized
    (input)
 
    q_Rat - RAT as defined in ASN.1 serving cell information
 
 
  RETURN VALUE  
    TRUE  - If successful
    FALSE - Otherwise

  SIDE EFFECTS

==============================================================================*/
boolean tle_InitMobileStausStruct(tle_Asn1MobileStatusDataType *pz_Asn1Mobile,
                                  OSUINT32 q_Rat);

/*==============================================================================
  FUNCTION      tle_osys_asn1_DecodeMobileStatus

  DESCRIPTION 	
    This function decodes a byte stream into an ASN1 encoded mobile status data 

  DEPENDENCIES
    None

  PARAMETERS
 
    pz_Handle - Points to the handle for the ASN1 decoder
    (input)
 
    pc_EncodedMobileStatus - Points to the mobile status data to be decoded
    (input)                  in a raw format
 
    encoded_size  - Number of bytes in the raw data
    (input)
 
    pz_DecodedMobileStatus - Points to the decoded mobile status data
    (output)                 Note: Caller shall initialize and allocate
                                   The dynamic portion prior of calling
                                   this function

  RETURN VALUE  
    TRUE:  Decoding success
    FALSE: Otherwise 

  SIDE EFFECTS  

==============================================================================*/
boolean tle_osys_asn1_DecodeMobileStatus(tle_OsysHandleType *pz_Handle,
                                         OSOCTET *pc_EncodedMobileStatus,
                                         size_t encoded_size,
                                         MobileStatusData *pz_DecodedMobileStatus);

/*==============================================================================
  FUNCTION      tle_osys_asn1_DecodeCellArray

  DESCRIPTION 	
    This function decodes a byte stream into an ASN1 encoded cell array. 

  DEPENDENCIES
    None

  PARAMETERS
 
    pz_Handle - Points to the handle for the ASN1 decoder
    (input)
 
    pc_EncodedCellArray - Points to the cell array to be decoded
    (input)               in a raw format
 
    encoded_size  - Number of bytes in the raw data
    (input)
 
    pz_DecodedCellArray - Points to the decoded cell array
    (output)                 Note: Caller shall deallocate
                                   The dynamic portion of the data
                                   after consuming the data

  RETURN VALUE  
    TRUE:  Decoding success
    FALSE: Otherwise 

  SIDE EFFECTS  

==============================================================================*/
boolean tle_osys_asn1_DecodeCellArray(tle_OsysHandleType *pz_Handle,
                                      OSOCTET *pc_EncodedCellArray,
                                      size_t encoded_size,
                                      Cell_Record_Array *pz_DecodedCellArray);

/*===========================================================================
FUNCTION tle_osys_asn1_ReportCell

DESCRIPTION
  This function Reports the content of a ASN.1 cell via F3 message
 
PARAMETERS 
 
  pz_Handle -     Points to the context handler for which cell will be
  (input/output)  reported              
 
  pz_Cell - Points to the ASN.1 cell to be reported
  (input)                   
 
DEPENDENCIES

RETURN VALUE 
  TRUE:  If successful
  FALSE: Otherwise

SIDE EFFECTS

===========================================================================*/
boolean tle_osys_asn1_ReportCell(tle_OsysHandleType *pz_Handle,
                                 CellDB_Record *pz_Cell);

/*===========================================================================
FUNCTION tle_osys_asn1_ReportCellArray

DESCRIPTION
  This function Reports the content of a ASN.1 cell array via F3 message
 
PARAMETERS 
 
  pz_Handle -     Points to the context handler for which cell array will be
  (input/output)  reported              
 
  pz_CellArray - Points to the ASN.1 cell array to be reported
  (input)                   
 
DEPENDENCIES

RETURN VALUE 
  TRUE:  If successful
  FALSE: Otherwise

SIDE EFFECTS

===========================================================================*/
boolean tle_osys_asn1_ReportCellArray(tle_OsysHandleType *pz_Handle,
                                      Cell_Record_Array *pz_CellArray);

/*===========================================================================
FUNCTION tle_osys_asn1_ReportClientInfo

DESCRIPTION
  This function Reports the content of a ASN.1 client information via F3 message
 
PARAMETERS 
 
  pz_Handle -     Points to the context handler for which cell will be
  (input/output)  reported              
 
  pz_Client - Points to the ASN.1 client information to be reported
  (input)                   
 
DEPENDENCIES

RETURN VALUE 
  TRUE:  If successful
  FALSE: Otherwise

SIDE EFFECTS

===========================================================================*/
boolean tle_osys_asn1_ReportClientInfo(tle_OsysHandleType *pz_Handle,
                                       ClientInformation *pz_Client);

/*===========================================================================
FUNCTION tle_osys_asn1_ReportMobileStatusData

DESCRIPTION
  This function Reports the content of a ASN.1 mobile status header via F3 message
 
PARAMETERS 
 
  pz_Handle -     Points to the context handler for which cell will be
  (input/output)  reported              
 
  pz_MobileStatus - Points to the ASN.1 mobile status data to be reported
  (input)                   
 
DEPENDENCIES

RETURN VALUE 
  TRUE:  If successful
  FALSE: Otherwise

SIDE EFFECTS

===========================================================================*/
boolean tle_osys_asn1_ReportMobileStatusData(tle_OsysHandleType *pz_Handle,
                                             MobileStatusData *pz_MobileStatus);

/*=============================================================================

  FUNCTION      tle_asn1_ReverseBits

  DESCRIPTION
    This function changes the bit orders in a byte.
 
  PARAMETERS
                
  DEPENDENCIES  None

  PARAMETERS
    u_in - The value which needs to be reordered
    (input)

  RETURN VALUE
    The input interpreted in reverse bit order

  SIDE EFFECTS  

=============================================================================*/
OSUINT8 tle_asn1_ReverseBits(OSUINT8 u_in);

/*==============================================================================
FUNCTION 
  tle_asn1_CheckAndAssignExtendedEarfcn

DESCRIPTION
  This function checks the input value for extended earfcn and assign to the
  output address.

  If the input value is out of range, the output address will be set to the
  invalid value and return TRUE.

PARAMETERS
 
  cq_InExtendedEarfcn - The input value of the extended earfcn
  (input)
 
  pq_OutExtendedEarfcn - Points to the output extended earfcn value
  (output)
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Succeed.
  FALSE - NULL parameter.

SIDE EFFECTS
  None.

==============================================================================*/
boolean tle_asn1_CheckAndAssignExtendedEarfcn(const uint32 cq_InExtendedEarfcn,
                                              OSUINT32 * const pq_OutExtendedEarfcn);

/*===========================================================================
FUNCTION 
  tle_asn1_RangeCheckVelocityV2

DESCRIPTION
  This function checks the range of the parameters against the values defined
  in ASN.1 ICD

PARAMETERS
 
  cpz_VelocityV2  - Points to the velocity data to be checked
  (input)
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If all the parameters are in range
  FALSE - Otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tle_asn1_RangeCheckVelocityV2(const VelocityV2 *cpz_VelocityV2);

#endif /* TLE_ASN1UTIL_H */

