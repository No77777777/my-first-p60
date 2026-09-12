/*===========================================================================
              R R C   S I B   D A T A B A S E   M O D U L E 

DESCRIPTION

  This module is part of the SIB processing procedure. This module containts
  the SIB database and utility functions related to the SIB database.
  
  The SIB database stores the SIBs from the last few cells that the UE
  has camped on. The number of cells for which the SIBs are stored is
  configurable. The SIB database is used to improve the efficiency of
  the UE since the UE does not have to read SIBs in a cell for which SIBs
  are already stored in the database, although it has to check if the
  SIBs are current.
  
  The SIB database is indexed using the combination of the Cell Id and the
  PLMN Id. This combination is unique for every cell in the UMTS system.
  When the UE moves to a new cell, it reads the MIB from the BCCH and
  retrieves the Cell Id and the PLMN Id. It then searches the database
  to see if there is a match. If so, it only has to check the value
  tag of the SIBs with the new value tags received in the MIB and make sure
  they are current.
  
  SIBs without value tags are read each time the UE camps on a new cell.

  Access functions are provided to read stored SIBs from the database.
  One function is provided to return a pointer to the desired SIB for
  the current active (serving) cell. Another function is provided to
  return a pointer to the desired SIB for any cell (cell-id and PLMN-id
  to be provided by the calling function).

  When SIBs in a serving cell change, this module can notify other
  procedures of the SIB change. This is done only if the modules
  register to be notified of the sib change.

  
  NOTE about the Value Tag Implementation:
  
  Note that currently both "cell value tags" and "PLMN value tags" are
  treated similarly - Only when there is a cell-id and plmn-id match for
  a cell in the database, it's value tags are checked. This is done for
  simplicity. This is functionally correct since plmn value tags are valid
  over the entire PLMN nd we match both the PLMN and the cell ids. 
  
  In the future an optimization could be done so that for
  SIBs having PLMN value tags, the value tags could be compared with any
  other cell in the same PLMN, and the SIB could be used for this cell
  also. Since only SIB 1 and SIB 16 have PLMN value tags,there may not 
  be much incentive to do this.
  
EXTERNALIZED FUNCTIONS

  tdsrrcsibdb_init  Initialize the database.
  
  tdsrrc_sibdb_return_sib_for_cell  Return a pointer to the requested SIB
                                 In the requested cell. 
  
  tdsrrcsibdb_return_sib_for_db_index  Return a pointer to the requested SIB
                                     for the requested database index. 
  
  tdsrrcsibdb_return_sib_for_srv_cell  Return a pointer to the requested
                                     SIB in the serving cell.

  tdsrrcsibdb_store_value_tags_for_new_cell  Store value_tags obtained for a new
                                     cell in the SIB database.

  tdsrrcsibdb_compare_value_tags_for_cell   Compare the value-tags stored in the
                                     SIB database with the newly received value-tags
                                     for a cell.
                                     
INITIALIZATION AND SEQUENCING REQUIREMENTS

  tdsrrcsibdb_init must be called once at power-up to initialize the
  SIB Database.
  
Copyright (c) 2010-2012 Qualcomm Technologies, Incorporated. 
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsibdb.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "assert.h"
#include "comdef.h"
#include "customer.h"
#include "err.h"
#include "msg.h"
#include "tdsrrcasn1.h"
#include "tdsrrcasn1util.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcintif.h"
#include "tdsrrcsibdb.h"
#include "tdsrrcsibdbi.h"
#include "tdsrrcumi.h"
#include "tdsrrcpg1.h"
#include "tdsrrccsp.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
#include "tdsl1rrcif.h"
#include "bit.h"
#endif

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsrrcmeas.h"
#include "tdsrrcnv.h"
#include "tdsrrcmcm.h"
#include "mm_umts.h"

#if defined(FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
#include "lte_rrc_ext_api.h"
#endif

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Total numer of SIB change notification registrations accepted.
It's 16 for now. If more than 16 registrations are needed, increase
this number. */
#define TDSRRC_TOT_SIB_CHG_N 16

#define TDSRRC_PSEUDO_SIB19_MAX_EARFCN 8

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* Structure to hold each sib change notification that has been
registered with this module */
typedef struct
{
  boolean index_used;           /* Indicate if this index is used.       */
  tdsrrc_proc_e_type  procedure;   /* Procedure to be notified              */
  tdsrrc_SIB_type sib;             /* SIB for which notification is needed. */
  uint8 priority;                 /*priority for reveived sib type which will be
                                           used for posting sib change to procedures*/
}tdsrrcsib_sib_change_notification_type;

/* Union used to copy Cell-id to a format used by GSTK. */
typedef union
{
  tdsrrc_int_cell_id_type int_cell_id; /* Internal type uint32 */
  uint8 ext_cell_id[4]; /* External type array of 4 uint8s  */
}tdsrrcsib_cell_id_convert_type;

/* To note if power-up initialization has been done already.
This is used in initializing variables for cases other than power-up
init such as FTM and LPM */
extern boolean tdsrrcsib_powerup_init_done;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

tdsrrc_sibdb_type tdssibdb;   /* The SIB database */

/* Array to store all the registered state change notifications */
tdsrrcsib_sib_change_notification_type tdsrrcsib_cn[TDSRRC_TOT_SIB_CHG_N];

uint8 tdsrrcsib_cn_max_priority = 0;/*Used to hold max number priority*/

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/*Look up table for holding the value tag and segments of the deferred SIBs.*/
extern tdsrrc_deferred_sib_info_type tdsdeferred_sib_info;
#endif

extern tds_nv_pseudo_sib19_type  tdsrrcnv_pseudo_sib19;

#ifdef FEATURE_TDSCDMA_SIB19
typedef struct {
   uint8                  cpid;
   uint16                 freq;
   uint32                 cell_id;
   tdsrrc_SysInfoType19 * sib19_ptr;
} tdsrrcsibdb_pseudo_sib19_type;

tdsrrcsibdb_pseudo_sib19_type pseudo_sib19 = {0, 0, 0, NULL};
#endif /*  FEATURE_TDSCDMA_SIB19 */

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */



/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ? 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */


/*===========================================================================

FUNCTION RRC_SIBDB_GET_SIB_PTR_FOR_CELL

DESCRIPTION

  This function returns a pointer to the requested SIB 
  for the requested cell. The pointer is typecast
  to void since different sibs have different type
  definitions. The calling function is reponsible for
  typecasting the returned pointer to the right type.
  
  The calling function MUST verify that the SIB database
  index passed to this function is a valid index
  created by this module.
  
DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  A void pointer that points to the requested SIB in the
  given SIB database index.
  
SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void * tdsrrcsibdb_get_sib_ptr_for_cell
(
  uint32 index,    /* Index for the requested cell */
  tdsrrc_SIB_type sib /* The SIB that's requested */
)
{
  void *ptr=NULL; /* Local ptr */

  /* Switch on the requested sib and copy the 
  corrosponding SIB pointer */
  switch( sib )
  {
    case tdsrrc_MIB :

      if(tdssibdb.cell[index].sibs.mib == NULL)
      {
        if(tdssibdb.cell[index].sibs.mib_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.mib =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.mib_encoded_ptr), /* Encoded SIB      */
          tdsrrc_MasterInformationBlock_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.mib_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.mib == NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from MIB",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.mib;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:mib_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.mib;
      }

      break;

    case tdsrrc_SIB1 :

      if(tdssibdb.cell[index].sibs.sib1 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib1_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib1 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib1_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType1_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib1_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib1== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB1",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib1;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib1_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib1;
      }

      break;

    case tdsrrc_SIB2 :

      if(tdssibdb.cell[index].sibs.sib2 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib2_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib2 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib2_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType2_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib2_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib2== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib2",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib2;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib2_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib2;
      }

      break;

    case tdsrrc_SIB3 :

      if(tdssibdb.cell[index].sibs.sib3 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib3_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib3 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib3_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType3_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib3_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib3== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib3",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib3;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib3_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib3;
      }

      break;

    case tdsrrc_SIB4 :

      if(tdssibdb.cell[index].sibs.sib4 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib4_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib4 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib4_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType4_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib4_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib4== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib4",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib4;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib4_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib4;
      }

      break;

    case tdsrrc_SIB5 :

      if(tdssibdb.cell[index].sibs.sib5 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib5_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib5 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib5_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType5bis_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib5_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib5 == NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib5",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib5;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib5_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib5;
      }

      break;

    case tdsrrc_SIB6 :

      if(tdssibdb.cell[index].sibs.sib6 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib6_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib6 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib6_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType6_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib6_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib6== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib6",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib6;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib6_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib6;
      }

      break;

    case tdsrrc_SIB7 :

      if(tdssibdb.cell[index].sibs.sib7 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib7_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib7 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib7_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType7_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib7_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib7== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib7",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib7;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib7_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib7;
      }

      break;

    case tdsrrc_SIB11 :

      if(tdssibdb.cell[index].sibs.sib11 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib11_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib11 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib11_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType11_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib11_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib11== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib11",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib11;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib11_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib11;
      }

      break;

    case tdsrrc_SIB12 :

      if(tdssibdb.cell[index].sibs.sib12 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib12_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib12 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib12_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType12_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib12_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib12== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib12",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib12;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib12_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib12;
      }

      break;

    case tdsrrc_SIB18 :

      if(tdssibdb.cell[index].sibs.sib18 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib18_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib18 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib18_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType18_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib18_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib18== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib18",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib18;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib18_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib18;
      }

      break;
  
    case tdsrrc_SB1 :

      if(tdssibdb.cell[index].sibs.sb1 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sb1_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sb1 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sb1_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoTypeSB1_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sb1_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sb1== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sb1",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sb1;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sb1_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sb1;
      }

      break;
  
    case tdsrrc_SB2 :

      if(tdssibdb.cell[index].sibs.sb2 == NULL)
      {
        if(tdssibdb.cell[index].sibs.sb2_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sb2 =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sb2_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoTypeSB2_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sb2_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sb2== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sb2",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sb2;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sb2_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sb2;
      }

      break;
  
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    case tdsrrc_SIB11bis :

      if(tdssibdb.cell[index].sibs.sib11bis == NULL)
      {
        if(tdssibdb.cell[index].sibs.sib11bis_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
        {
          /* ASN.1 decode the PDU */
          tdssibdb.cell[index].sibs.sib11bis =  tdsrrcasn1_decode_pdu( 
          (void *)(tdssibdb.cell[index].sibs.sib11bis_encoded_ptr), /* Encoded SIB      */
          tdsrrc_SysInfoType11bis_PDU,                                                       /* SIB's PDU number */
          tdssibdb.cell[index].sibs.sib11bis_encoded_pdu_length   /* SIB's Length in bytes */
          );
          if(tdssibdb.cell[index].sibs.sib11bis== NULL)
          {
            ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib11bis",0,0,0);
          }
          ptr = (void *) tdssibdb.cell[index].sibs.sib11bis;
        }
        else
        {
          ERR_FATAL("SIB_HEAP:sib11bis_encoded_ptr is NULL",0,0,0);
        }
      }
      else
      {
        ptr = (void *) tdssibdb.cell[index].sibs.sib11bis;
      }

      break;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
        case tdsrrc_SIB19 :

          if(tdssibdb.cell[index].sibs.sib19 == NULL)
          {
            if(tdssibdb.cell[index].sibs.sib19_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
            {
              /* ASN.1 decode the PDU */
              tdssibdb.cell[index].sibs.sib19 =  tdsrrcasn1_decode_pdu( 
              (void *)(tdssibdb.cell[index].sibs.sib19_encoded_ptr), /* Encoded SIB      */
              tdsrrc_SysInfoType19_PDU,                                                       /* SIB's PDU number */
              tdssibdb.cell[index].sibs.sib19_encoded_pdu_length   /* SIB's Length in bytes */
              );
              if(tdssibdb.cell[index].sibs.sib19== NULL)
              {
                ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib19",0,0,0);
              }
              ptr = (void *) tdssibdb.cell[index].sibs.sib19;
            }
            else
            {
              ERR_FATAL("SIB_HEAP:sib19_encoded_ptr is NULL",0,0,0);
            }
          }
          else
          {
            ptr = (void *) tdssibdb.cell[index].sibs.sib19;
          }

          break;
#endif

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown SIB%d",sib);
      break;
  }/* end switch */
  return(ptr);
}/* end function tdsrrcsibdb_get_sib_ptr_for_cell */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION RRC_SIBDB_RETURN_SIB_FOR_DB_INDEX

DESCRIPTION

  This function returns a pointer to the requested SIB for
  the requested cell index in the SIB database. If there is no cell
  for that particular index, or if this particular SIB has not been read 
  completely for that cell, this function returns
  NULL.
  
  The pointer is typecast to void since different sibs have
  different type definitions. The calling function is responsible
  for typecasting the pointer to the required SIB type.
   
  Caution: The calling function should read all the required
  data from the pointer as soon as the function returns since
  the SIB data could be overwritten if the serving cell changes
  or if a SIB change indication has been received.
  
DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  Pointer to the requested SIB. 
  "NULL" if the requested SIB has not been read or if there
  is no cell stored at the requested index.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void * tdsrrcsibdb_return_sib_for_db_index
(
  tdsrrc_SIB_type  req_sib,          /* The SIB that is required */
  uint32        rrc_sibdb_index   /* Index into the SIB database for
                                  the cell from which the SIB is required.
                                  Must be between 0 and RRC_MAX_SIBDB_INDEX */
)
{
  /* If the database is empty return NULL */
  if(tdssibdb.database_empty)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Database empty");
    return( NULL );
  }

  /* If the given sib is not in the valid range return NULL */
  /*lint -save -e685 */ 
  /* TRUE if the number is a valid sib.*/
  if(!(TDSSIBDB_VALID_SIB_RANGE(req_sib)))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid SIB %d",req_sib);
    return( NULL );
  }
  /*lint -restore */
  /* If the given index is not in the valid range return NULL */
  /* TRUE if the index is a valid SIB database index.*/
  if(!(TDSSIBDB_VALID_INDEX(rrc_sibdb_index)))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid tdssibdb index %d",rrc_sibdb_index);
    return( NULL );
  }

  /* Now check if the requested SIB has been received */
/*lint -e661 lint bug - incorrect out of bounds for ? operator */
  if(!(tdssibdb.cell[rrc_sibdb_index].sib_valid[req_sib]))
/*lint +e661 */
  {
    return( NULL );
  }

  /* Get the SIB pointer from the cell header and return the ptr */
  return( tdsrrcsibdb_get_sib_ptr_for_cell( rrc_sibdb_index, req_sib ));
} /* end function rrc_sbidb_return_sib_for_db_index */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

#ifdef FEATURE_TDSCDMA_SIB19
/*===========================================================================

FUNCTION tdsrrcsibdb_free_pseudo_sib19

DESCRIPTION

  This function frees pseudo SIB19 pointer
  
DEPENDENCIES

RETURN VALUE
  none.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrcsibdb_free_pseudo_sib19()
{
   if (pseudo_sib19.sib19_ptr)
   {
      if (pseudo_sib19.sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem)
         tdsrrc_free(pseudo_sib19.sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem);

      tdsrrc_free(pseudo_sib19.sib19_ptr);

      pseudo_sib19.sib19_ptr = NULL;
   }

   pseudo_sib19.cell_id = 0;
   pseudo_sib19.freq = 0;
   pseudo_sib19.cpid = 0;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/* CR664031 depends on NAS CR664032 which is not available for DPM */
#ifdef FEATURE_TDSCDMA_PSEUDO_SIB19
/*===========================================================================

FUNCTION tdsrrcsibdb_get_pseudo_sib19

DESCRIPTION

  This function returns a pointer to the SIB19 generated
  from NV and NAS EARFCN DB
  
DEPENDENCIES

RETURN VALUE

  Pointer to the requested SIB. 
  "NULL" if the requested SIB has not been read or if there
  is no serving cell selected.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_SysInfoType19 * tdsrrcsibdb_get_pseudo_sib19(inter_task_cell_id_type * nas_cell_id_ptr)
{
   uint8 i = 0;
   uint8  num_freq = 0;
   uint32 earfcn[TDSRRC_PSEUDO_SIB19_MAX_EARFCN];
   tdsrrc_SysInfoType19 * pseudo_sib19_ptr = NULL;
   earfcn_nas_type nas_earfcn;
#ifdef LTE_BAND_NUM
   sys_lte_band_mask_e_type supported_lte_band = tdsrrccsp_get_lte_band_pref();
#else
   sys_band_mask_type supported_lte_band = tdsrrccsp_get_lte_band_pref();
#endif

   ASSERT(nas_cell_id_ptr != NULL);
   
   if (!get_lte_earfcn_list_from_nas(nas_cell_id_ptr, &nas_earfcn))
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PSEUDO SIB19: NAS get_lte_earfcn_list_from_nas() returns FALSE");
      return NULL;
   }

   if ((nas_earfcn.count == 0) || (nas_earfcn.count > NAS_MAX_EARFCN_COUNT) || (nas_earfcn.count > TDSRRC_PSEUDO_SIB19_MAX_EARFCN))
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"PSEUDO SIB19: NAS get_lte_earfcn_list_from_nas() returns invalid num of freq. %d",nas_earfcn.count);
      return NULL;
   }
#if defined(LTE_BAND_NUM)&&(LTE_BAND_NUM == 256)//LTE_BAND_NUM is 256, only print low 64bit of LTE band cap*/
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"PSEUDO SIB19: NAS returns total %d EARFCNs, supported LTE band HI32=0x%x, LO32=0x%x",
                nas_earfcn.count,
                QWORD_HIGH(supported_lte_band.bits_1_64),
                QWORD_LOW(supported_lte_band.bits_1_64));
#else /*LTE_BAND_NUM is 64*/
   TDSRRC_MSG3(MSG_LEGACY_HIGH,"PSEUDO SIB19: NAS returns total %d EARFCNs, supported LTE band HI32=0x%x, LO32=0x%x",
               nas_earfcn.count,
               QWORD_HIGH(supported_lte_band),
               QWORD_LOW(supported_lte_band));
#endif             
   for (i = 0; i < nas_earfcn.count; i++)
   {
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
      if (lte_rrc_earfcn_is_supported(tdsrrc_get_as_id(), nas_earfcn.earfcn_nas[i], supported_lte_band))
#else
      if (lte_rrc_earfcn_is_supported(nas_earfcn.earfcn_nas[i], supported_lte_band))
#endif
      {
         earfcn[num_freq++] = nas_earfcn.earfcn_nas[i];
      }
      else
      {
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"PSEUDO SIB19: invalid EARFCN[%d] %d from NAS",i,nas_earfcn.earfcn_nas[i]);
      }
   }

   if (num_freq == 0)
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"PSEUDO SIB19: no valid EARFCN from NAS,total %d",nas_earfcn.count);
      return NULL;
   }
   
   pseudo_sib19_ptr = (tdsrrc_SysInfoType19 *) tdsrrc_malloc (sizeof(tdsrrc_SysInfoType19));

   if ( pseudo_sib19_ptr == NULL )
   {
      ERR_FATAL("tdsrrcsibdb_get_pseudo_sib19: Failed to allocate memory ",0,0,0);
   }

   pseudo_sib19_ptr->m.gsm_PriorityInfoListPresent = 0;
   pseudo_sib19_ptr->m.eutra_FrequencyAndPriorityInfoListPresent = 1;
   pseudo_sib19_ptr->m.v920NonCriticalExtensionsPresent = 0;

   pseudo_sib19_ptr->utra_PriorityInfoList.m.utran_FDD_FrequencyListPresent = 0;
   pseudo_sib19_ptr->utra_PriorityInfoList.m.utran_TDD_FrequencyListPresent = 0;

   pseudo_sib19_ptr->utra_PriorityInfoList.utra_ServingCell.m.s_PrioritySearch2Present = 0;
   
   pseudo_sib19_ptr->utra_PriorityInfoList.utra_ServingCell.priority = tdsrrcnv_pseudo_sib19.utra_priority;
   pseudo_sib19_ptr->utra_PriorityInfoList.utra_ServingCell.s_PrioritySearch1 = tdsrrcnv_pseudo_sib19.s_priority_search1;
   pseudo_sib19_ptr->utra_PriorityInfoList.utra_ServingCell.threshServingLow = tdsrrcnv_pseudo_sib19.thresh_serving_low;

   pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.n = num_freq;

   pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem = 
      (tdsrrc_EUTRA_FrequencyAndPriorityInfo *) tdsrrc_malloc (sizeof(tdsrrc_EUTRA_FrequencyAndPriorityInfo) * num_freq);
   
   if ( pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem == NULL )
   {
      ERR_FATAL("tdsrrcsibdb_get_pseudo_sib19: Failed to allocate memory for elem",0,0,0);
   }

   for (i = 0; i < num_freq; i++)
   {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"==> PSEUDO SIB19 EARFCN %d",earfcn[i]);

      /* Provide hard-coded bandwidth */
      pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem[i].m.measurementBandwidthPresent = 1;
      pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem[i].m.eutra_blackListedCellListPresent = 0;
      
      pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem[i].earfcn = (uint16) earfcn[i];
      pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem[i].measurementBandwidth = tdsrrcnv_pseudo_sib19.eutra_meas_bandwidth;
      pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem[i].priority = tdsrrcnv_pseudo_sib19.eutra_priority;
      pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem[i].threshXhigh = tdsrrcnv_pseudo_sib19.eutra_thresh_high;
      pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem[i].threshXlow = tdsrrcnv_pseudo_sib19.eutra_thresh_low;
      pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem[i].qRxLevMinEUTRA = tdsrrcnv_pseudo_sib19.eutra_qrxlevmin;
      pseudo_sib19_ptr->eutra_FrequencyAndPriorityInfoList.elem[i].eutraDetection = FALSE;
   }

   TDSRRC_MSG4(MSG_LEGACY_HIGH,"==> UtraPri %d, Sprisrch1 %d, ThrSrvLow %d, reserved 0x%x", 
               tdsrrcnv_pseudo_sib19.utra_priority,
               tdsrrcnv_pseudo_sib19.s_priority_search1,
               tdsrrcnv_pseudo_sib19.thresh_serving_low,
               tdsrrcnv_pseudo_sib19.reserved);
   TDSRRC_MSG5(MSG_LEGACY_HIGH,"==> EutraPri %d, EutraBW %d, ThrXHigh %d, ThrXLow %d, qRxLevMinEUTRA %d", 
               tdsrrcnv_pseudo_sib19.eutra_priority,
               tdsrrcnv_pseudo_sib19.eutra_meas_bandwidth,
               tdsrrcnv_pseudo_sib19.eutra_thresh_high,
               tdsrrcnv_pseudo_sib19.eutra_thresh_low,
               tdsrrcnv_pseudo_sib19.eutra_qrxlevmin);

   return pseudo_sib19_ptr;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif /* #ifdef FEATURE_TDSCDMA_PSEUDO_SIB19 */

#endif /* FEATURE_TDSCDMA_SIB19 */

/*===========================================================================

FUNCTION RRC_SIBDB_RETURN_SIB_FOR_SRV_CELL

DESCRIPTION

  This function returns a pointer to the requested SIB for
  the current serving cell. If there is no serving cell
  selected, or if this particular SIB has not been read 
  completely for the serving cell, this function returns
  NULL.
  
  The pointer is typecast to void since different sibs have
  different type definitions. The calling function is responsible
  for typecasting the pointer to the required SIB type.
  
  Caution: The calling function should read all the required
  data from the pointer as soon as the function returns since
  the SIB data could be overwritten if the serving cell changes
  or if a SIB change indication has been received.

DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  Pointer to the requested SIB. 
  "NULL" if the requested SIB has not been read or if there
  is no serving cell selected.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void * tdsrrcsibdb_return_sib_for_srv_cell
(
  tdsrrc_SIB_type  req_sib  /* The SIB that is required */
)
{
  void * ret_ptr = NULL;

  /* First check if we have an active cell selected */
  if(!(tdssibdb.active_cell_sibs_valid))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No active cell selected yet!");
    return(NULL);
  }
  /* Then use the generic function to return sibs for a cell */
  ret_ptr = tdsrrcsibdb_return_sib_for_db_index( req_sib, /* requested sib */
                                                 tdssibdb.active_cell_index /* For the active cell */
                                               );

#ifdef FEATURE_TDSCDMA_SIB19
  if ((ret_ptr == NULL) 
      && (req_sib == tdsrrc_SIB19) 
      && tdsrrcmcm_is_lte_mode_enabled()
      && (tdsrrcnv_pseudo_sib19.is_pseudo_sib19_enabled))
  {
/* CR664031 depends on NAS CR664032 which is not available for DPM */
#ifdef FEATURE_TDSCDMA_PSEUDO_SIB19
    /* This is for validating the SIB DB for serving cell index */
    tdsrrc_SysInfoType1 * sib1_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SIB1, tdssibdb.active_cell_index);
    tdsrrc_SysInfoType3 * sib3_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SIB3, tdssibdb.active_cell_index);

    if (sib1_ptr && sib3_ptr)
    {
      uint32 cell_id = tdsrrc_translate_cell_id(&sib3_ptr->cellIdentity);

      if ((pseudo_sib19.cpid == tdssibdb.cell[tdssibdb.active_cell_index].cpid)
          && (pseudo_sib19.freq == tdssibdb.cell[tdssibdb.active_cell_index].freq)
          && (pseudo_sib19.cell_id == cell_id)
          && (pseudo_sib19.sib19_ptr != NULL))
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"PSEUDO SIB19: No cell change,keep the same sib19");
      }
      else
      {
         inter_task_cell_id_type nas_cell_id;
         rrc_gsm_map_nas_sys_info_type rrc_lac;

         tdsrrcsibdb_free_pseudo_sib19();

         if (tdsrrccsp_fill_nas_common_info(sib1_ptr, &rrc_lac) == FAILURE)
         {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"PSEUDO SIB19: tdsrrccsp_fill_nas_common_info() failed");
         }
         else
         {
            rrc_plmn_identity_type rrc_plmn = tdssibdb.cell[tdssibdb.active_cell_index].plmn_id;
            
            pseudo_sib19.cpid = tdssibdb.cell[tdssibdb.active_cell_index].cpid;
            pseudo_sib19.freq = tdssibdb.cell[tdssibdb.active_cell_index].freq;
            pseudo_sib19.cell_id = cell_id;

            nas_cell_id.rat = SYS_RAT_TDS_RADIO_ACCESS;
            nas_cell_id.plmn = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(rrc_plmn);
            nas_cell_id.lac = b_unpackw(rrc_lac.value, 0, 16);
            nas_cell_id.cell_id = (uint16) (cell_id & 0x0000ffff); /* only take the last 16 bits */
               
            pseudo_sib19.sib19_ptr = tdsrrcsibdb_get_pseudo_sib19(&nas_cell_id);

            TDSRRC_MSG5(MSG_LEGACY_HIGH,"PSEUDO SIB19: new sib19 for freq %d, cpid %d, cell_id %d = 0x%x, sib19 0x%x", 
                        pseudo_sib19.freq, 
                        pseudo_sib19.cpid, 
                        pseudo_sib19.cell_id,
                        pseudo_sib19.cell_id,
                        pseudo_sib19.sib19_ptr);
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"PSEUDO SIB19: new sib19 RRC LAC len %d, lac[0] = 0x%x, lac[1] = 0x%x", 
                        rrc_lac.length, 
                        rrc_lac.value[0],
                        rrc_lac.value[1]);
            TDSRRC_MSG5(MSG_LEGACY_HIGH,"PSEUDO SIB19: new sib19 NAS data plmn %d-%d, lac 0x%x, short cell_id %d = 0x%x", 
                        TDSRRC_CSP_GET_INT_MCC(rrc_plmn), 
                        TDSRRC_CSP_GET_INT_MNC(rrc_plmn), 
                        nas_cell_id.lac, 
                        nas_cell_id.cell_id,
                        nas_cell_id.cell_id);
         }
      }

      ret_ptr = pseudo_sib19.sib19_ptr;

    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"PSEUDO SIB19: invalid SIB1 0x%d or SIB3 0x%d!",sib1_ptr,sib3_ptr);
    }
#else /* FEATURE_TDSCDMA_PSEUDO_SIB19 */
   TDSRRC_MSG0(MSG_LEGACY_HIGH,"PSEUDO SIB19: this feature is not for DPM 2.0");
#endif /* FEATURE_TDSCDMA_PSEUDO_SIB19 */
  }
#endif /* FEATURE_TDSCDMA_SIB19 */
  
  return ret_ptr;
} /* end function tdsrrcsibdb_return_sib_for_srv_cell */      
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/





/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/*===========================================================================

FUNCTION tdsrrcsibdb_get_sib_type_from_sib_type_and_tag

DESCRIPTION

  Helper function to get sib_type from sibsb type received in
  MIB
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_get_sib_type_from_sib_type_and_tag
(
  tdsrrc_SIB_TypeAndTag * sib_type_and_tag,
  tdsrrcsib_sib_type_vtag * sib_type_tag
 )
{ 
  //set the sib_type to invalid initially
  sib_type_tag->sib_type = tdsrrc_MAX_SIB;
  sib_type_tag->sib_tag = TDSINVALID_MIB_SB_VALUE_TAG;

  switch(TDSRRCGET_SIB_TYPE_AND_TAG(sib_type_and_tag))
  {                                                  
    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType1):   
      sib_type_tag->sib_type = tdsrrc_SIB1;
      sib_type_tag->sib_tag = sib_type_and_tag->u.sysInfoType1;
      break;                                         
      
    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType2):
      sib_type_tag->sib_type = tdsrrc_SIB2;
      sib_type_tag->sib_tag = sib_type_and_tag->u.sysInfoType2;
      break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType3):
      sib_type_tag->sib_type = tdsrrc_SIB3;
      sib_type_tag->sib_tag = sib_type_and_tag->u.sysInfoType3;
      break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType4):
      sib_type_tag->sib_type = tdsrrc_SIB4;
      sib_type_tag->sib_tag = sib_type_and_tag->u.sysInfoType4;
      break;

    /*lint -save -e616  Fallthrough intended */
    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType5bis):
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB5bis schedule present in MIB/SB.Typecasting it to SIB5");

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType5):
      sib_type_tag->sib_type = tdsrrc_SIB5;
      sib_type_tag->sib_tag = sib_type_and_tag->u.sysInfoType5;
      break;
    /*lint -restore */

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType6):
      sib_type_tag->sib_type = tdsrrc_SIB6;
      sib_type_tag->sib_tag = sib_type_and_tag->u.sysInfoType6;
      break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType11):
      sib_type_tag->sib_type = tdsrrc_SIB11;
      sib_type_tag->sib_tag = sib_type_and_tag->u.sysInfoType11;
      break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType12):
      sib_type_tag->sib_type = tdsrrc_SIB12;
      sib_type_tag->sib_tag = sib_type_and_tag->u.sysInfoType12;
      break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType18):
      sib_type_tag->sib_type = tdsrrc_SIB18;
      sib_type_tag->sib_tag = sib_type_and_tag->u.sysInfoType18;
      break;
    
    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType7):
      sib_type_tag->sib_type = tdsrrc_SIB7;
      break;
  
    default:                                           
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown/Unsup value tag %d",TDSRRCGET_SIB_TYPE_AND_TAG(sib_type_and_tag));  
      break;                                         
  } 
}                                                       

/*===========================================================================

FUNCTION tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag

DESCRIPTION

  Helper function to get sib_type from sibsb type received in
  SB1 and SB2 functions
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag
(
  tdsrrc_SIBSb_TypeAndTag * sib_sb_type_tag,
  tdsrrcsib_sib_type_vtag * sib_type_tag
 )   
{ 
  //set the sib_type to invalid initially
  sib_type_tag->sib_type = tdsrrc_MAX_SIB;
  sib_type_tag->sib_tag = TDSINVALID_MIB_SB_VALUE_TAG;

  //switch on the sibSb type and initialize sib_type_tag data-struct
  //to sib_type and get the sib_tag too
  switch(TDSRRCGET_SIB_TYPE_AND_TAG(sib_sb_type_tag))
  {                                                  
    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType1):     
      sib_type_tag->sib_type = tdsrrc_SIB1;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoType1;
      break;                                         
      
    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType2):
      sib_type_tag->sib_type = tdsrrc_SIB2;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoType2;
    break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType3):
      sib_type_tag->sib_type = tdsrrc_SIB3;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoType3;
      break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType4):
      sib_type_tag->sib_type = tdsrrc_SIB4;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoType4;
      break;

    /*lint -save -e616  Fallthrough intended */
    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType5bis):
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB5bis schedule present in MIB/SB.Typecasting it to SIB5");

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType5):
      sib_type_tag->sib_type = tdsrrc_SIB5;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoType5;
      break;
    /*lint -restore */

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType6):
      sib_type_tag->sib_type = tdsrrc_SIB6;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoType6;
      break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType11):
      sib_type_tag->sib_type = tdsrrc_SIB11;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoType11;
      break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType12):
      sib_type_tag->sib_type = tdsrrc_SIB12;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoType12;
      break;

    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType18):
      sib_type_tag->sib_type = tdsrrc_SIB18;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoType18;
      break;
  
    case TDSRRCGET_SIB_TYPE_AND_TAG_VALUE_SYS_INFO_TYPE(sysInfoTypeSB1):
      sib_type_tag->sib_type = tdsrrc_SB1;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoTypeSB1;
      break;
  
    case TDSRRCGET_SIB_TYPE_AND_TAG_VALUE_SYS_INFO_TYPE(sysInfoTypeSB2):
      sib_type_tag->sib_type = tdsrrc_SB2;
      sib_type_tag->sib_tag = sib_sb_type_tag->u.sysInfoTypeSB2;
      break;
  
    case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType7):
      sib_type_tag->sib_type = tdsrrc_SIB7;
      break;
  
    default:                                           
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown/Unsup value tag type %d",TDSRRCGET_SIB_TYPE_AND_TAG(sib_sb_type_tag));  
      break;                                         
  } 
}                                                       

/*===========================================================================

FUNCTION tdsrrcsibdb_clear_active_cell_status

DESCRIPTION

  This function reset's the sib-db active cell flags.
  This is done whenever we go OOS or whenever we go from WtoG
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_clear_active_cell_status( void )
{
  /* Clear SIB database of active cell selected yet. */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalidating Serving cell Index");

  {
    tdssibdb.active_cell_sibs_valid = FALSE;
    tdssibdb.active_cell_index = TDSSIBDB_INVALID_INDEX;
  }

  tdsrrcpg1_check_and_abort_bcch_mod_info_processing();
}
/*===========================================================================
FUNCTION RRC_SIBDB_INIT

DESCRIPTION

  This function initializes the SIB database. This is called
  once during power-up. Note that individual cell headers are
  not initialized at this time. They are initialized each time
  a new cell is selected and SIBs need to be stored in the cell
  headers.
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_init( void )
{
  uint32 index; /* Index to database */

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initializing the SIB-DB");

  /* Indicate that the database is empty */
  tdssibdb.database_empty = TRUE;

  /* Indicate that the SIB database does not have any active
  cell selected yet. */
  tdssibdb.active_cell_sibs_valid = FALSE;

  /* Clear active cell index */
  tdssibdb.active_cell_index = TDSSIBDB_INVALID_INDEX;

  tdssibdb.num_stored_cells = 0;

  /* Clear the Active Cells SIB Change indications.
  These are used when the SIBs change in the active
  cell to indicate the change to other RRC procedures. */
  for( index=0; index<TDSRRC_TOT_SIBS; index++ )
  {
    tdssibdb.active_cell_sib_changed[index] = FALSE;
  }

  /* Make sure all indices are unused */
  for( index=0; index<=TDSSIBDB_MAX_INDEX; index++ )
  {
    tdssibdb.cell[index].index_used = FALSE;
    /*Initalize six hr timer for each index to Zero*/
    tdssibdb.cell[index].six_hr_count =0;
  }

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  tdsdeferred_sib_info.val_tag.sib11_val_tag =0;
  tdsdeferred_sib_info.val_tag.sib11bis_val_tag =0;
  tdsdeferred_sib_info.val_tag.sib12_val_tag =0;
  tdsdeferred_sib_info.val_tag.sib18_val_tag =0;
#ifdef FEATURE_TDSCDMA_SIB19
  tdsdeferred_sib_info.val_tag.sib19_val_tag =0;
#endif
#endif

  /* Only at powerup should the state change notifications be
  initialized. Check for this before initializing */
  if (tdsrrcsib_powerup_init_done == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initializing SIB State Change Notifications");

    /* Make sure all the SIB change registration indices are
    unused */
    for( index=0; index<TDSRRC_TOT_SIB_CHG_N; index++ )
    {
      tdsrrcsib_cn[index].index_used = FALSE;
    }
  }

  /* Nothing else to do here. Individual cell headers can be initialized 
  before they need to be used */

} /* end function tdsrrcsibdb_init */


/*===========================================================================

FUNCTION RRC_SIBDB_FREE_SIB

DESCRIPTION

  This function frees the memory used by a System Information
  Block stored in the SIB database using the ASN.1 free pdu function.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_free_sib
(
  int pdu_num,    /* PDU number defined by ASN.1 for this SIB */
  void *sib_ptr   /* Pointer to the SIB typecast to void.     */
)
{
  /* Check if the given pointer is not NULL */
  if( sib_ptr != NULL )
  {
    /* Call the free pdu function. It returns non-zero if an error
    occured */
    if( tdsrrcasn1_free_pdu( pdu_num, sib_ptr ) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Error occured in ASN.1 free pdu function");
    }
  }
}




/*===========================================================================

FUNCTION RRC_SIBDB_CELL_HEADER_INIT

DESCRIPTION

  This function initializes a cell header within the SIB database.
  This function is called before storing SIBs for a new cell in the
  SIB database.
  
DEPENDENCIES

 None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_cell_header_init
(
  uint32 index /* Index of the cell header to be initialized */
)
{
  uint32 count; /* Counter */

  for( count = 0; count < TDSRRC_TOT_SIBS; count++ )
  {
    /* Initialize each SIB status for this cell index to invalid */
    tdssibdb.cell[index].sib_valid[count] = FALSE;
    /* Initialize each SIB's value tag to 0 which is not a valid
    value tag. This is to indicate that no value tag is stored
    in this index yet. */
    tdssibdb.cell[index].sib_value_tag[count] = 0; 
  }
  
  tdssibdb.cell[index].six_hr_count  = 0;

  /* Since the cells may have previously been used to store
  SIBs, we need to free the memory used by each SIB for this
  index. Since we have to use a different PDU type for each
  SIB, we have to free each SIB seperately. The rrc_sibdb_free
  _sib function is used to free them.  */

  /* Note that even though we don't support all these SIBs now,
  the code has been added here since additional SIBs will be supported
  in future releases */
  if( tdssibdb.cell[index].sibs.mib != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_MasterInformationBlock_PDU,
                       (void *)tdssibdb.cell[index].sibs.mib);
    tdssibdb.cell[index].sibs.mib = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib1 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType1_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib1);
    tdssibdb.cell[index].sibs.sib1 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib2 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType2_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib2);
    tdssibdb.cell[index].sibs.sib2 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib3 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType3_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib3);
    tdssibdb.cell[index].sibs.sib3 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib4 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType4_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib4);
    tdssibdb.cell[index].sibs.sib4 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib5 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType5bis_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib5);
    tdssibdb.cell[index].sibs.sib5 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib6 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType6_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib6);
    tdssibdb.cell[index].sibs.sib6 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib7 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType7_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib7);
    tdssibdb.cell[index].sibs.sib7 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib8 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType8_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib8);
    tdssibdb.cell[index].sibs.sib8 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib9 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType9_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib9);
    tdssibdb.cell[index].sibs.sib9 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib10 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType10_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib10);
    tdssibdb.cell[index].sibs.sib10 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib11 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType11_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib11);
    tdssibdb.cell[index].sibs.sib11 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib12 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType12_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib12);
    tdssibdb.cell[index].sibs.sib12 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib18 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType18_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib18);
    tdssibdb.cell[index].sibs.sib18 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sb1 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoTypeSB1_PDU,
                       (void *)tdssibdb.cell[index].sibs.sb1);
    tdssibdb.cell[index].sibs.sb1 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sb2 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoTypeSB2_PDU,
                       (void *)tdssibdb.cell[index].sibs.sb2);
    tdssibdb.cell[index].sibs.sb2 = NULL;
  }
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  if( tdssibdb.cell[index].sibs.sib11bis != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType11bis_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib11bis);
    tdssibdb.cell[index].sibs.sib11bis = NULL;
  }
#endif
#ifdef FEATURE_TDSCDMA_SIB19
  if( tdssibdb.cell[index].sibs.sib19 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType19_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib19);
    tdssibdb.cell[index].sibs.sib19 = NULL;
  }
#endif

  memset(tdssibdb.cell[index].sibs.mib_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.mib_encoded_pdu_length = TDSINVALID_PDU_LENGTH;
  
  memset(tdssibdb.cell[index].sibs.sib1_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib1_encoded_pdu_length = TDSINVALID_PDU_LENGTH;
  
  memset(tdssibdb.cell[index].sibs.sib2_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib2_encoded_pdu_length = TDSINVALID_PDU_LENGTH;
  
  memset(tdssibdb.cell[index].sibs.sib3_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib3_encoded_pdu_length = TDSINVALID_PDU_LENGTH;  
  
   memset(tdssibdb.cell[index].sibs.sib4_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib4_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

  //CRC:nickle, remove if check
  memset(tdssibdb.cell[index].sibs.sib5_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib5_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

  memset(tdssibdb.cell[index].sibs.sib6_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib6_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

  memset(tdssibdb.cell[index].sibs.sib7_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib7_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

  memset(tdssibdb.cell[index].sibs.sib11_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib11_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

  memset(tdssibdb.cell[index].sibs.sib12_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib12_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

  memset(tdssibdb.cell[index].sibs.sib18_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib18_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

  memset(tdssibdb.cell[index].sibs.sb1_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sb1_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

  memset(tdssibdb.cell[index].sibs.sb2_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sb2_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  memset(tdssibdb.cell[index].sibs.sib11bis_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib11bis_encoded_pdu_length = TDSINVALID_PDU_LENGTH;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
  memset(tdssibdb.cell[index].sibs.sib19_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  tdssibdb.cell[index].sibs.sib19_encoded_pdu_length = TDSINVALID_PDU_LENGTH;
#endif

} /* end function tdsrrcsibdb_cell_header_init */




/*===========================================================================

FUNCTION RRC_SIBDB_FIND_CELL_INDEX

DESCRIPTION

  This function tries to find the index in the sib
  database for the given cell id and PLMN id combination.
  If a match is found, the correct index is returned.
  
DEPENDENCIES

  SIB Database should be correctly initialized. The PLMN
  and Cell Ids given by the function should be in the
  database.

RETURN VALUE

  Index of the cell. If this cell is not present
  in the SIB database, TDSSIBDB_INVALID_INDEX is returned.

SIDE EFFECTS

  None.

===========================================================================*/
uint32 tdsrrcsibdb_find_cell_index
(
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell       /* The cell id for the cell */
)
{    
  uint32 index;                /* To find the matching index */
  boolean index_found = FALSE; /* Index has'nt been found yet */

  /* Try to match the PLMN and Cell Ids to one of the cells
  stored in the database. */
  for( index=0; index<=TDSSIBDB_MAX_INDEX; index++ )
  {
    /* If this particular index is being used */
    if( tdssibdb.cell[index].index_used == TRUE )
    {
      /* Match the PLMN id and cell id for each index. */
      if( (tdsrrc_bcch_plmns_match((plmn), (tdssibdb.cell[index].plmn_id))) &&
          (cell == tdssibdb.cell[index].cell_id)
        )
      {
        /* A Match has been found. Get out of the for loop. */
        index_found = TRUE;
        break; /* Break out of the for loop */
      }
    }
  }/* end for */

  if( index_found )
  {
    return( index );
  }
  else                                                       
  {
    return( TDSSIBDB_INVALID_INDEX );
  }
} /* end function tdsrrcsibdb_find_cell_index */

/*===========================================================================

FUNCTION tdsrrcsibdb_find_valid_cell_index

DESCRIPTION

  This function tries to find the index in the sib
  database for the given cell id and PLMN id combination.
  If a match is found, the correct index is returned.
  
DEPENDENCIES

  SIB Database should be correctly initialized. The PLMN
  and Cell Ids given by the function should be in the
  database.

RETURN VALUE

  Index of the cell. If this cell is not present
  in the SIB database, TDSSIBDB_INVALID_INDEX is returned.

SIDE EFFECTS

  None.

===========================================================================*/
uint32 tdsrrcsibdb_find_valid_cell_index
(
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell,       /* The cell id for the cell */
  tdssib_events_index_e_type event_index /*Current avtice sib event*/
)
{    
  uint32 index;                /* To find the matching index */
  boolean index_found = FALSE; /* Index has'nt been found yet */

  /* Try to match the PLMN and Cell Ids to one of the cells
  stored in the database. */
  for( index=0; index<=TDSSIBDB_MAX_INDEX; index++ )
  {
    /* If this particular index is being used */
    if( tdssibdb.cell[index].index_used == TRUE )
    {
      /* Match the PLMN id and cell id for each index. */
      if( (tdsrrc_bcch_plmns_match((plmn), (tdssibdb.cell[index].plmn_id))) &&
          (cell == tdssibdb.cell[index].cell_id)
        )
      {
        if((tdsrrcsib_is_need_to_cleanup_cell(event_index) == TRUE) &&
            (tdssibdb.cell[index].six_hr_count  >=  TDSMAX_CELL_INDEX_COUNT_SIXHR_CELL_SEL_RESEL))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"6hr sib: SIX hr Timer expired for sib db index=%d",index);
          if((tdssibdb.active_cell_sibs_valid == TRUE)&&
             (tdssibdb.active_cell_index == index))
          {//We want to skip clearing serving cell, Just to ensure that other procedures accessing
            //SIB DB should not get blocked. 
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"6hr sib: Skip Cleaning index=%d,as its serving cell ",index);
          }
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"6hr sib:Clean sib db Index =%d,six hr count =%d ",index,tdssibdb.cell[index].six_hr_count);
            tdsrrcsibdb_clear_a_cell_index(index);
            index=TDSSIBDB_INVALID_INDEX ;
          }
        }
        /* A Match has been found. Get out of the for loop. */
        index_found = TRUE;
        break; /* Break out of the for loop */
      }
    }
  }/* end for */

  if( index_found )
  {
    return( index );
  }
  else
  {
    return( TDSSIBDB_INVALID_INDEX );
  }
} /* end function tdsrrcsibdb_find_cell_index */


/*===========================================================================

FUNCTION RRC_SIBDB_ADJUST_AGE_INDEX

DESCRIPTION

  This function adjusts the "age_index" for the cells in the SIB
  database. The age_index is a number that indicates how long a cell
  has been stored in the database. The newest cell has the age
  index of 0, and each previous cell has an index one higher than
  the newest cell.
  
  If the database is full, the oldest cell has the age index of 
  TDSSIBDB_MAX_INDEX.  
    
  The index of the newest cell is passed to this function as soon as
  it is created. NOTE: Only tdsrrcsibdb_create_cell_index can call this
  function.
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_adjust_age_index
(
  uint32 new_cell_index  /* Index of the new cell that has just been created */
)
{
  uint32 count;  /* Counter */

  /* If this cell is being used for the first time, all 
  other cells are older than this cell and their age
  index needs to be adjusted. 
  
  If this cell is being re-used, only those cells that
  were previously newer than this cell need to be adjusted. */ 

  if(new_cell_index >= TDSSIBDB_MAX_CELLS)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib db idx = %d,cant adjust",new_cell_index);
    return;
  }

  /* First make sure the age index is not invalid */
  ASSERT( tdssibdb.cell[new_cell_index].age_index <= TDSSIBDB_MAX_INDEX );

  if( tdssibdb.cell[new_cell_index].index_used == FALSE )
  {
    /* This is the first time this cell is being used. All
    other used cells have to be adjusted. Force the age index
    of this cell to be the maximum. */
    tdssibdb.cell[new_cell_index].age_index = TDSSIBDB_MAX_INDEX;
  }

  for( count=0; count <=TDSSIBDB_MAX_INDEX; count++ )
  {
    if( /* If this is not the new cell's index              */ 
        (count != new_cell_index) &&        
        /* And if this cell index is used                   */
        (tdssibdb.cell[count].index_used == TRUE) &&  
        /* And if this cell is newer than the selected cell */
        (tdssibdb.cell[count].age_index < tdssibdb.cell[new_cell_index].age_index)
      )
    {
      /* If the above conditions are satisfied, the age index needs
      to be incremented by one. */
      tdssibdb.cell[count].age_index++;
    }

  }

  /* Finally make the age index of the given cell to be 0 since this
  is now the newest cell. */
  tdssibdb.cell[new_cell_index].age_index = 0;
} /* end function tdsrrcsibdb_adjust_age_index */                                      



/*===========================================================================

FUNCTION RRC_SIBDB_CREATE_CELL_INDEX

DESCRIPTION

  This function creates a new cell index for the given
  PLMN and CELL id if needed. The function first checks
  if the index for this PLMN and Cell id combination is
  already present in the database. If so, it adjusts the
  "age_index" of the cells accordingly and returns the
  existing index. If a match is not found a new cell index
  is created and returned to the calling function.
  If the database is full, the oldest cell is removed and
  that index is used for the new cell.
    
DEPENDENCIES

  SIB Database should be correctly initialized. The PLMN
  and Cell Ids given by the function should be in the
  database.

RETURN VALUE

  The newly created index for the given cell. This ranges
  from 0 to TDSSIBDB_MAX_INDEX.

SIDE EFFECTS

  None.

===========================================================================*/
uint32 tdsrrcsibdb_create_cell_index
(
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell,       /* The cell id for the cell */
  tdssib_events_index_e_type event_index /*Current avtice sib event*/
)
{    
  uint32 index;      /* The index to be returned           */
  uint32 l_index=0; /* Used to find the largest age Index */
  uint32 l_age=0;    /* Used to find largest age index     */

  /* Always set to FALSE the variable that indicates if the returned
  index has SIBs for a matching PLMN and Cell id. This will be set
  to TRUE later in this function if needed. */
  tdssibdb.last_plmn_and_cell_id_matched = FALSE;

  /* If the database is empty, just return the first index. */
  if(tdssibdb.database_empty)
  {
    tdssibdb.database_empty = FALSE;    /* Not empty anymore */
    tdssibdb.cell[0].index_used = TRUE; /* Index 0 is now used */
    tdssibdb.cell[0].sib_db_valid_for_quick_cell_resel = TRUE;
    tdssibdb.cell[0].age_index = 0;     /* This is the newest cell in the
                                     database. */
    tdssibdb.cell[0].plmn_id = plmn;    /* Copy the PLMN id */
    tdssibdb.cell[0].cell_id = cell;    /* Copy the Cell id */
    tdsrrcsibdb_cell_header_init(0);   /* Initialize the cell header for this cell */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB Heap: Empty SIB Idx %d createidx");
    tdssibdb.num_stored_cells++;   
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"New SIBDB Index created - %d,%d num_cells",0,tdssibdb.num_stored_cells);

    return(0); /* Return index 0 */
  }
  
  /* If the database is not empty, try to find a match
  for the given PLMN and Cell Id within the database. That
  means we are re-visiting a cell already stored in the
  database. */
  if( (index = tdsrrcsibdb_find_valid_cell_index(plmn, cell,event_index)) != TDSSIBDB_INVALID_INDEX )
  {
    /* We found a match. Adjust the age index for the cells in the
    database and return the index. Note that "index_used" will
    already be set to TRUE and the PLMN and Cell ids don't have
    to be written again. */
    tdsrrcsibdb_adjust_age_index( index );
    /* Since we already stored SIBs for the same PLMN and Cell ids, indicate
    this by setting the corrosponding variable to TRUE. */
    tdssibdb.last_plmn_and_cell_id_matched = TRUE;
    tdssibdb.cell[index].sib_db_valid_for_quick_cell_resel = TRUE;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIBDB Index found - %d,%d num_cells",index,tdssibdb.num_stored_cells);
    return( index );
  }

  /* If a match is not found then check for the next empty
  slot in the database */
  for( index=0; index<=TDSSIBDB_MAX_INDEX; index++ )
  {
    if(tdssibdb.cell[index].index_used == FALSE)
    {
      tdsrrcsibdb_adjust_age_index( index ); /* Adjust age index for cells */
      tdssibdb.cell[index].index_used = TRUE; /* This index is now used */
      tdssibdb.cell[index].plmn_id = plmn;    /* Copy the PLMN id */
      tdssibdb.cell[index].cell_id = cell;    /* Copy the Cell id */
      tdssibdb.cell[index].sib_db_valid_for_quick_cell_resel = TRUE;
      tdsrrcsibdb_cell_header_init(index);   /* Initialize the cell header */

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB Heap: Empty SIB Idx %d createidx",index);

      tdssibdb.num_stored_cells++;   
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"New SIBDB Index created - %d,%d num_cells",index,tdssibdb.num_stored_cells);

      return( index );
    }
  }

  /* If we get here, then the database is full. The oldest
  cell has to be removed to accomodate the new cell. Note that
  the oldest cell will have the age_index of TDSSIBDB_MAX_INDEX
  since we store only that many cells in the database.Also note 
  that "index_used" will already be set to TRUE. */
  for( index=0; index<=TDSSIBDB_MAX_INDEX; index++ )
  {
    if(tdssibdb.cell[index].age_index == TDSSIBDB_MAX_INDEX)
    {
      /* ZZTBD: This if block is added from new W code */
      if((tdssibdb.active_cell_sibs_valid == TRUE)&&
         (tdssibdb.active_cell_index == index))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"age_index is %d for active cell index, so continue to next entry"
                 ,TDSSIBDB_MAX_INDEX, 0, 0);
        continue;
      }
      /* This is the oldest cell. Adjust the age index
      of the cells accordingly and return this index. */
      tdsrrcsibdb_adjust_age_index( index );
      tdssibdb.cell[index].plmn_id = plmn;    /* Copy the PLMN id */
      tdssibdb.cell[index].cell_id = cell;    /* Copy the Cell id */
      tdsrrcsibdb_cell_header_init(index);   /* Initialize the cell header */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB Heap: Freeing SIB Idx %d createidx",index);
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIBDB Index created - %d,%d num_cells",index,tdssibdb.num_stored_cells);
      return( index );
    }
  }

  /* We can come here if the index with highest age_index is active cell's index
   and hence we can't delete this entry or due to an error situation since one of the cells must
  have an age index of SIBDB_MAX_INDEX. Note that the error situation should never
  happen in the field. */
  TDSRRC_MSG3(MSG_LEGACY_ERROR,"Highest age_index is %d and is active cell's index or Indexing problem in the SIB database!"
       ,TDSSIBDB_MAX_INDEX, 0, 0);

  /* We don't have to err-fatal here. We can recover from this
  error by just picking the largest age index and using that
  index for the new cell */
  for( index=0; index<=TDSSIBDB_MAX_INDEX; index++ )
  {
    if(tdssibdb.cell[index].age_index > l_age)
    {
      if((tdssibdb.active_cell_sibs_valid == TRUE)&&
         (tdssibdb.active_cell_index == index))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"age_index is %d for active cell index, so continue to next entry"
                 ,TDSSIBDB_MAX_INDEX, 0, 0);
        continue;
      }
       /* In order to get the next largest next time assign the 
           comparison parameter to present largest*/
      l_age = tdssibdb.cell[index].age_index;
      /* Replace current largest index with new largest index */
      l_index = index;
    }
  }
  /* Now, l_index is the index of the oldest cell */
  tdsrrcsibdb_adjust_age_index( l_index );
  tdssibdb.cell[l_index].plmn_id = plmn;  /* Copy the PLMN id */
  tdssibdb.cell[l_index].cell_id = cell;  /* Copy the Cell id */
  tdsrrcsibdb_cell_header_init(l_index); /* Initialize the cell header */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB Heap: Freeing SIB Idx %d ERRcreateidx",l_index);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIBDB Index created - %d",l_index);
  return( l_index );

} /* end function tdsrrcsibdb_create_cell_index */
/*===========================================================================

FUNCTION RRC_SIBDB_RETURN_SIB_FOR_CELL

DESCRIPTION

  This function returns a pointer to the requested SIB for
  For a requested Cell id and PLMN Id. If there are no sibs stored
  for that particular PLMN and Cell id combination, or if this particular 
  SIB has not been read completely for that cell, this function returns
  NULL.
  
  The pointer is typecast to void since different sibs have
  different type definitions. The calling function is responsible
  for typecasting the pointer to the required SIB type.
   
  Caution: The calling function should read all the required
  data from the pointer as soon as the function returns since
  the SIB data could be overwritten if the serving cell changes
  or if a SIB change indication has been received.
  
DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  Pointer to the requested SIB. 
  "NULL" if the requested SIB has not been read or if there
  is no cell stored at the requested index.

SIDE EFFECTS

  None.

===========================================================================*/
void * tdsrrc_sibdb_return_sib_for_cell
(
  tdsrrc_SIB_type  req_sib,          /* The SIB that is required */
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell       /* The cell id for the cell */
)
{                           
  uint32 index; /* To find the matching index */

  if( (index = tdsrrcsibdb_find_cell_index(plmn, cell)) != TDSSIBDB_INVALID_INDEX )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIBDB Index found - %d",index);
    /* Use the generic function to return sibs for a index */
    return( tdsrrcsibdb_return_sib_for_db_index( req_sib, index ));
  }
  else
  {
    /* The requested SIB is not stored in the database. */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Match not found for given PLMN and cell id");
    return( NULL );
  }
} /* end function tdsrrc_sibdb_return_sib_for_cell */

/*===========================================================================

FUNCTION tdsrrcsibdb_invalidate_mib_sb_value_tags_for_interrupted_sib_update

DESCRIPTION

  This function invalidates the MIB and SB's value-tags in case SIB_UPDATE_IND
  is interrupted by a higher priority n_bcch event.  Invalidation of the MIB
  and SB value tags, cause the SIB's in the reference-list and SIB's in SB's to
  be compared for value-tags afresh.  This is needed bcos, it could so happen that
  in the earlier run which was interrupted, SB and MIB were read but all the SIB's
  that were changed are not completely read.
  
DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_invalidate_mib_sb_value_tags_for_interrupted_sib_update
(
  uint32 dbindex     /* SIB Database Index for the cell for which the
                        new Value Tags have been received */
)
{  
  /* Make sure the index is a valid sib database index where
  SIBs are stored */
  /* TRUE if the index is a valid SIB database index.*/
  if(!(TDSSIBDB_VALID_INDEX(dbindex)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid database index given");
    return;
  }
/*lint -e661 lint bug - incorrect out of bounds for ? operator */
  if( tdssibdb.cell[dbindex].index_used == FALSE )
/*lint +e661 */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unused database index given");
    return;
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalidate MIB,SB v-tag");
  //invalidate value tags for mib,sb1 and sb2
  tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_MIB] = 
  tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SB1] = 
  tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SB2] = 
    TDSINVALID_MIB_SB_VALUE_TAG;

}


/*===========================================================================

FUNCTION RRC_SIBDB_COMPARE_VALUE_TAGS_FOR_CELL

DESCRIPTION

  This function compares the stored value tags for a particular
  cell with the newly received value tags in the MIB or the
  Scheduling blocks.
  
  If the new value tags don't match the stored value tags, 
  this function indicates this in the result_ptr so that the
  new SIBs can be read and stored in their place.

  NOTE: THE FOLLOWING SIBS DO NOT HAVE VALUE-TAGS
  
    SIB7, SIB9, SIB10, SIB14, SIB17
    
  These SIBs only have expiration timers.

DEPENDENCIES

  SIB Database should be correctly initialized.

  The result_ptr should be initialized by the calling function.
  The results of the value tag comparisons will be stored in
  the result_ptr.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_compare_value_tags_for_cell
(
  uint32 dbindex,     /* SIB Database Index for the cell for which the
                      new Value Tags have been received */
  tdsrrc_SIB_type sib,   /* Indicate which SIB the value-tags are from
                      This can be the MIB or SB1 or SB2. */
  void *block_ptr,    /* Pointer to the MIB, SB1 or SB2, typecast
                      to void */
  tdsrrc_sibdb_value_tag_results_type *result_ptr /* Pointer to store
                      the results of value tag comparisons */
,
tdsrrcsib_vtag_match_type  match_type
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
         ,tdssib_events_index_e_type index
#endif

)
{
  tdsrrc_MasterInformationBlock *mib_ptr; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr;         /* Local pointer to SB2       */

  tdsrrc_SIBSb_ReferenceList *sb_ptr; /* Local pointer to SIBSb ref list */
  tdsrrc_SIB_ReferenceList *ptr;      /* Local pointer to SIB ref list   */
  uint32 count, n_index=0;                   /* Local counter                   */

  tdsrrcsib_sib_type_vtag sib_type_tag;


#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List *ext_siblist_ptr = NULL;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List2 *ext_siblist2_ptr = NULL;
#endif

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/* Used to indicate whether the scheduling info for the 
   deferred SIBs are in SB1/SB2. */
  boolean deferred_sibs_in_sb1 = FALSE;
  boolean deferred_sibs_in_sb2 = FALSE;
  tdsrrc_SIB_type schedul_info_def_sibs;
#endif

  if( result_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Un-initialized result ptr!");
    return;
  }

  /* Fill the results ptr with the default value. The
  results for the SIBs present in the reference list will
  be updated later. */
  for( count=0; count < TDSRRC_TOT_SIBS; count++ )
  {
    result_ptr->result[count] = TDSVALUE_TAG_NOT_PRESENT;
  }

  /* Make sure the index is a valid sib database index where
  SIBs are stored */
  /* TRUE if the index is a valid SIB database index.*/
  if(!(TDSSIBDB_VALID_INDEX(dbindex)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid database index given");
    return;
  }
/*lint -e661 lint bug - incorrect out of bounds for ? operator */
  if( tdssibdb.cell[dbindex].index_used == FALSE )
/*lint +e661 */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unused database index given");
    return;
  }

  /* Now check whether the new value tags are from
  the MIB or one of the SBs. 
  
  If the new value tags are from the MIB, first we
  have to check the MIB value tag. If the MIB value
  tag does not match, save the new MIB value tag and
  free the space used by the old MIB for this cell. */
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/* Check whether the Deferred SIBs in Update SIBs event are scheduled 
in SB1/SB2 */

  if( (tdssib_events_ptr->event[index].tempholder.mib_decoded == TRUE) &&
      (tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS)&&
      ((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] ==TDSINVALID_MIB_SB_VALUE_TAG)||
       (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12] ==TDSINVALID_MIB_SB_VALUE_TAG)||
       (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] ==TDSINVALID_MIB_SB_VALUE_TAG) ||
#ifdef FEATURE_TDSCDMA_SIB19
       (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] ==TDSINVALID_MIB_SB_VALUE_TAG) || 
#endif
       (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18] ==TDSINVALID_MIB_SB_VALUE_TAG)
  )
     )
  {
    if(tdsrrcsib_is_sib_scheduled(tdsrrc_SIB11,dbindex, TRUE,&schedul_info_def_sibs) == TRUE)
    {
      if(schedul_info_def_sibs == tdsrrc_SB1)
      {
        deferred_sibs_in_sb1 = TRUE;
      }
      else if(schedul_info_def_sibs == tdsrrc_SB2)
      {
        deferred_sibs_in_sb2 = TRUE;
      }
    }
    if(tdsrrcsib_is_sib_scheduled(tdsrrc_SIB12,dbindex, TRUE,&schedul_info_def_sibs) == TRUE)
    {
      if(schedul_info_def_sibs == tdsrrc_SB1)
      {
        deferred_sibs_in_sb1 = TRUE;
      }
      else if(schedul_info_def_sibs == tdsrrc_SB2)
      {
        deferred_sibs_in_sb2 = TRUE;
      }
    }

    if(tdsrrcsib_is_sib_scheduled(tdsrrc_SIB18,dbindex, TRUE,&schedul_info_def_sibs) == TRUE)
    {
      if(schedul_info_def_sibs == tdsrrc_SB1)
      {
        deferred_sibs_in_sb1 = TRUE;
      }
      else if(schedul_info_def_sibs == tdsrrc_SB2)
      {
        deferred_sibs_in_sb2 = TRUE;
      }
    }

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT

    if(!deferred_sibs_in_sb1)
    {
     if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SB1] )
     {
       sb1_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB1,dbindex);
       if((sb1_ptr != NULL) && ((TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(sb1_ptr, tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1))
        || (TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(sb1_ptr, tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1))
        ))
       {
         deferred_sibs_in_sb1 = TRUE;
       }
      }
    }
    if(!deferred_sibs_in_sb2)
    {
      if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SB2] )
      {
        sb2_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB2,dbindex);
        if((sb2_ptr != NULL) && ((TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(sb2_ptr, tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2))
           || (TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(sb2_ptr, tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2))
         ))
        {
          deferred_sibs_in_sb2 = TRUE;
        }
      }
    }
#endif
  }
#endif  
  
  if( sib == tdsrrc_MIB )
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Comparing value tags from MIB");
    /* Typecast to MIB pointer */
    mib_ptr = (tdsrrc_MasterInformationBlock *)block_ptr;

    /* Check the MIB value tag */
    if( (tdssibdb.cell[dbindex].sib_valid[tdsrrc_MIB]) &&
        (mib_ptr->mib_ValueTag == tdssibdb.cell[dbindex]
           .sib_value_tag[tdsrrc_MIB])
      )
    {
      if(match_type == TDSRRCSIB_READ_ALL_SIBS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Forcibly re-read all sibs");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIB value tag matched");
      }
      result_ptr->result[tdsrrc_MIB] = TDSVALUE_TAG_MATCHED;

      /* If the MIB value tag matched, the MIB has not changed so 
      we can assume all the SIBs stored for this cell are current.
      Update the result pointer accordingly. For loop starts from
      1 since we don't need to check the mib. */
      for( count=1; count < TDSRRC_TOT_SIBS; count++ )
      {
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
        if(((count == tdsrrc_SB1)&&(deferred_sibs_in_sb1 == TRUE)) ||
           ((count == tdsrrc_SB2)&&(deferred_sibs_in_sb2 == TRUE))
           )
        {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB:sib %d value tag didnot match",count);
          result_ptr->result[count] = TDSVALUE_TAG_DID_NOT_MATCH;
        }
        else if(((count == tdsrrc_SIB11)||
           (count == tdsrrc_SIB12)||
           (count == tdsrrc_SIB11bis) ||
#ifdef FEATURE_TDSCDMA_SIB19
           (count == tdsrrc_SIB19) ||
#endif
           (count == tdsrrc_SIB18)
          )&&
           (tdssibdb.cell[dbindex].sib_value_tag[count] ==TDSINVALID_MIB_SB_VALUE_TAG)
           )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB:sib %d value tag didnot match",count);
          result_ptr->result[count] = TDSDEFERRED_SIB_RCVD;
        }
        else
#endif
        if( tdssibdb.cell[dbindex].sib_valid[count] )
        {
          if(match_type == TDSRRCSIB_READ_ALL_SIBS)
          {
            result_ptr->result[count] = TDSVALUE_TAG_DID_NOT_MATCH;
          }
          else
          {
            /* If we have a valid SIB, indicate that the value tag is current */
            result_ptr->result[count] = TDSVALUE_TAG_MATCHED;
          }
        }
        else
        {
          /* The sib is not present */
          result_ptr->result[count] = TDSNO_SIB_FOR_THIS_VALUE_TAG;
        }
      }
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
      if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS)&&
         ((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] ==TDSINVALID_MIB_SB_VALUE_TAG)||
          (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12] ==TDSINVALID_MIB_SB_VALUE_TAG)||
          (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] ==TDSINVALID_MIB_SB_VALUE_TAG) ||
#ifdef FEATURE_TDSCDMA_SIB19
          (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] ==TDSINVALID_MIB_SB_VALUE_TAG) ||
#endif
          (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18] ==TDSINVALID_MIB_SB_VALUE_TAG)
          )
        )
      {
        tdsrrcsibdb_copy_deferred_sibdb_to_tsibdb(dbindex,index, mib_ptr, tdsrrc_MIB);
      }
#endif
      /* Nothing else to do here, just return */
      return;
    }
    else
    {
      if(match_type == TDSRRCSIB_READ_ALL_SIBS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Forcibly re-read all sibs");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIB value tag did not match");
      }
    }

    /* The MIB value tag did not match. So we have to go through the
    reference list and see which SIBs are current and which ones need
    to be read again. Compare all value-tags present in the MIB. */
    sb_ptr = &(mib_ptr->sibSb_ReferenceList);
    n_index = 0;
    while(sb_ptr->n > n_index)
    {
      /* Switch on the value tag's sib type and look
      at the corrosponding SIBs stored value tag for
      a match */
      tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag(&sb_ptr->elem[n_index].sibSb_Type,
                                                  &sib_type_tag);

          
      switch( sib_type_tag.sib_type )
      {
      case tdsrrc_SIB1:
      case tdsrrc_SIB2:
      case tdsrrc_SIB3:
      case tdsrrc_SIB4:
      case tdsrrc_SIB5:
      case tdsrrc_SIB6:
      case tdsrrc_SIB11:
      case tdsrrc_SIB12:
      case tdsrrc_SIB18:
      case tdsrrc_SB1:
      case tdsrrc_SB2:

        if(tdssibdb.cell[dbindex].sib_valid[sib_type_tag.sib_type])
        {
          if((sib_type_tag.sib_tag) == 
             (tdssibdb.cell[dbindex].sib_value_tag[sib_type_tag.sib_type]))
          {
            if(match_type == TDSRRCSIB_READ_ALL_SIBS)
            {
              result_ptr->result[sib_type_tag.sib_type] = TDSVALUE_TAG_DID_NOT_MATCH;
            }
            else
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            if(((sib_type_tag.sib_type == tdsrrc_SB1)&&(deferred_sibs_in_sb1 == TRUE)) ||
               ((sib_type_tag.sib_type == tdsrrc_SB2)&&(deferred_sibs_in_sb2 == TRUE))
               )
            {
/* If the deferred SIBs are scheduled in SB1/SB2, then read them again. */
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB:sib %d value tag didnot match",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(count));
              result_ptr->result[sib_type_tag.sib_type] = TDSVALUE_TAG_DID_NOT_MATCH;
            }
            else
#endif
            {
              /* SIB1 is present and the value tags match */
              TDSRRC_MSG1(MSG_LEGACY_LOW,"V-Tag matched for MIB SIB-%d",sib_type_tag.sib_type);
              result_ptr->result[sib_type_tag.sib_type] = TDSVALUE_TAG_MATCHED;
            }
          }
          else
          {
            /* Update result pointer */
            result_ptr->result[sib_type_tag.sib_type] = 
              TDSVALUE_TAG_DID_NOT_MATCH;
          }
        }
        else
        {
          /* The SIB is not present in the database. Update result pointer. */
          result_ptr->result[sib_type_tag.sib_type] = 
            TDSNO_SIB_FOR_THIS_VALUE_TAG;
        }

        break;

      case tdsrrc_SIB7:
        break;

      default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown/Unsup value tag type");
          //value_tag = TDSINVALID_MIB_SB_VALUE_TAG;
          break;
      }/* end switch */

      n_index++;
    }/* end while */

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    if(TDSEXTENSION_SIB_SCHED_PRESENT_IN_MIB(mib_ptr))
    {
       ext_siblist_ptr = &(mib_ptr->v690NonCriticalExtensions.
        v6b0NonCriticalExtensions.masterInformationBlock_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
    }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
    if((mib_ptr->m.v690NonCriticalExtensionsPresent) &&
       (mib_ptr->v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent) &&
       (mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
       (mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.v860NonCriticalExtensions.
          masterInformationBlock_v860ext.m.extSIBTypeInfoSchedulingInfo_ListPresent))
    {
      ext_siblist2_ptr = &(mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.v860NonCriticalExtensions.
        masterInformationBlock_v860ext.extSIBTypeInfoSchedulingInfo_List);
    }
#endif
  }/* end if value tags from MIB */
  else 
  {
    /* The value tags must be from one of the scheduling blocks. Find out which one. */
    if( sib == tdsrrc_SB1 )
    {
      /* Value tag is from Scheduling Block 1 
      Copy the pointer and compare the received
      value tags. */
      sb1_ptr = (tdsrrc_SysInfoTypeSB1 *)block_ptr;
      ptr = &(sb1_ptr->sib_ReferenceList);
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if((TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(
            sb1_ptr,tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1)))
      {
        ext_siblist_ptr = &(sb1_ptr->v6b0NonCriticalExtensions.
          sysInfoTypeSB1_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
      }
#endif
#ifdef FEATURE_TDSCDMA_SIB19
      if((sb1_ptr->m.v6b0NonCriticalExtensionsPresent) &&
         (sb1_ptr->v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
         (sb1_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.sysInfoTypeSB1_v860ext.
            m.extSIBTypeInfoSchedulingInfo_ListPresent))
      {
        ext_siblist2_ptr = &(sb1_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.
          sysInfoTypeSB1_v860ext.extSIBTypeInfoSchedulingInfo_List);
      }
#endif

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
      if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS)&&
         ((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] ==TDSINVALID_MIB_SB_VALUE_TAG)||
          (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12] ==TDSINVALID_MIB_SB_VALUE_TAG)||
          (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] ==TDSINVALID_MIB_SB_VALUE_TAG)
#ifdef FEATURE_TDSCDMA_SIB19
         || (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] ==TDSINVALID_MIB_SB_VALUE_TAG)
#endif
         || (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18] ==TDSINVALID_MIB_SB_VALUE_TAG)
        ))
      {
        tdsrrcsibdb_copy_deferred_sibdb_to_tsibdb(dbindex, index, sb1_ptr, tdsrrc_SB1);
      }
#endif
    }
    else if( sib == tdsrrc_SB2 ) 
    {
      /* Value tag is from Scheduling Block 2 
      Copy the pointer and compare the received
      value tags. */
      sb2_ptr = (tdsrrc_SysInfoTypeSB2 *)block_ptr;
      ptr = &(sb2_ptr->sib_ReferenceList);
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if((TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(
            sb2_ptr,tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2)))
      {
           ext_siblist_ptr = &(sb2_ptr->v6b0NonCriticalExtensions.
          sysInfoTypeSB2_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
      }
#endif
#ifdef FEATURE_TDSCDMA_SIB19
      if((sb2_ptr->m.v6b0NonCriticalExtensionsPresent) &&
         (sb2_ptr->v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
         (sb2_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.sysInfoTypeSB2_v860ext.
            m.extSIBTypeInfoSchedulingInfo_ListPresent))
      {
        ext_siblist2_ptr = &(sb2_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.
          sysInfoTypeSB2_v860ext.extSIBTypeInfoSchedulingInfo_List);
      }
#endif

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
      if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS)&&
         ((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] ==TDSINVALID_MIB_SB_VALUE_TAG)||
          (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12] ==TDSINVALID_MIB_SB_VALUE_TAG)||
          (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] ==TDSINVALID_MIB_SB_VALUE_TAG)
#ifdef FEATURE_TDSCDMA_SIB19
          || (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] ==TDSINVALID_MIB_SB_VALUE_TAG)
#endif
          || (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18] ==TDSINVALID_MIB_SB_VALUE_TAG)
          ))
      {
        tdsrrcsibdb_copy_deferred_sibdb_to_tsibdb(dbindex, index, sb2_ptr, tdsrrc_SB2);
      }
#endif
    }

    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid scheduling block %d",sib);
      ptr=NULL;
    }

    /* Now compare all the value tags given by this block */
    n_index = 0;

    if(NULL == ptr)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"INVALID pointer");
      return;
    }

    while( ptr->n > n_index )
    {
      /* Switch on the value tag's sib type and look
      at the corrosponding SIBs stored value tag for
      a match */
      tdsrrcsibdb_get_sib_type_from_sib_type_and_tag(&ptr->elem[n_index].sib_Type,
                                                &sib_type_tag);

          
      switch( sib_type_tag.sib_type )
      {
      case tdsrrc_SIB1:
      case tdsrrc_SIB2:
      case tdsrrc_SIB3:
      case tdsrrc_SIB4:
      case tdsrrc_SIB5:
      case tdsrrc_SIB6:
      case tdsrrc_SIB11:
      case tdsrrc_SIB12:
      case tdsrrc_SIB18:

        if(tdssibdb.cell[dbindex].sib_valid[sib_type_tag.sib_type])
        {
          if((sib_type_tag.sib_tag) == 
             (tdssibdb.cell[dbindex].sib_value_tag[sib_type_tag.sib_type]))
          {
            if(match_type == TDSRRCSIB_READ_ALL_SIBS)
            {
              result_ptr->result[sib_type_tag.sib_type] = TDSVALUE_TAG_DID_NOT_MATCH;
            }
            else
            {
              /* SIB1 is present and the value tags match */
              TDSRRC_MSG1(MSG_LEGACY_LOW,"V-Tag matched for SB SIB-%d",sib_type_tag.sib_type);
              result_ptr->result[sib_type_tag.sib_type] = TDSVALUE_TAG_MATCHED;
            }
          }
          else
          {
            /* Update result pointer */
            result_ptr->result[sib_type_tag.sib_type] = 
              TDSVALUE_TAG_DID_NOT_MATCH;
          }
        }
        else
        {
          /* The SIB is not present in the database. Update result pointer. */
          result_ptr->result[sib_type_tag.sib_type] = 
            TDSNO_SIB_FOR_THIS_VALUE_TAG;
        }

        break;

      case tdsrrc_SIB7:
        break;

      default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown/Unsup value tag type");
          //value_tag = TDSINVALID_MIB_SB_VALUE_TAG;
          break;

      }/* end switch */
      n_index++;
    }/* end while */
  }/* end if value tags from SB1 or SB2 */

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  if(ext_siblist_ptr != NULL)
  {
    n_index = 0;
    while(ext_siblist_ptr->n > n_index)
  {
    if( TDSRRCCHECK_SIB_TypeExt(
          (&(ext_siblist_ptr->elem[n_index].extensionSIB_Type)),
           systemInfoType11bis)
	    )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib11bis: vtag =%d for sib11bis",ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag);
   
      if(tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB11bis])
      {
        if((ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag) == 
           (tdssibdb.cell[dbindex].sib_value_tag[rrc_SIB11bis]))
        {
          if(match_type == TDSRRCSIB_READ_ALL_SIBS)
          {
            result_ptr->result[tdsrrc_SIB11bis] = TDSVALUE_TAG_DID_NOT_MATCH;
          }
          else
          {
            /* SIB1 is present and the value tags match */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib11bis:V-Tag matched for SIB11bis,Vtag-%d",ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag);
            result_ptr->result[tdsrrc_SIB11bis] = TDSVALUE_TAG_MATCHED;
          }
        }
        else
        {
          /* Update result pointer */
          result_ptr->result[tdsrrc_SIB11bis] = 
            TDSVALUE_TAG_DID_NOT_MATCH;
        }
      }
      else
      {
        /* The SIB is not present in the database. Update result pointer. */
        result_ptr->result[tdsrrc_SIB11bis] = 
          TDSNO_SIB_FOR_THIS_VALUE_TAG;
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:extensionSIB_Type =%x not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr->elem[n_index].extensionSIB_Type)));
      }
      n_index++;    
    }
  }
#endif
#ifdef FEATURE_TDSCDMA_SIB19
  if(ext_siblist2_ptr != NULL)
  {
    n_index = 0;
    while(ext_siblist2_ptr->n > n_index)
    {
      if(ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t ==
         T_tdsrrc_SIB_TypeExt2_systemInfoType19)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib19: vtag =%d for sib19",ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag);
   
        if(tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB19])
        {
          if((ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag) == 
             (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19]))
          {
            if(match_type == TDSRRCSIB_READ_ALL_SIBS)
            {
              result_ptr->result[tdsrrc_SIB19] = TDSVALUE_TAG_DID_NOT_MATCH;
            }
            else
            {
              /* SIB1 is present and the value tags match */
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib19:V-Tag matched for SIB19,Vtag-%d",ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag);
              result_ptr->result[tdsrrc_SIB19] = TDSVALUE_TAG_MATCHED;
            }
          }
          else
          {
            /* Update result pointer */
            result_ptr->result[tdsrrc_SIB19] = 
              TDSVALUE_TAG_DID_NOT_MATCH;
          }
        }
        else
        {
          /* The SIB is not present in the database. Update result pointer. */
          result_ptr->result[tdsrrc_SIB19] = 
            TDSNO_SIB_FOR_THIS_VALUE_TAG;
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:extensionSIB_Type =%x not supported",ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t);
      }
      n_index++; 
    }
  }
#endif

}/* end function tdsrrcsibdb_compare_value_tags_for_cell */


/*===========================================================================

FUNCTION RRC_SIBDB_STORE_VALUE_TAGS_FOR_NEW_CELL

DESCRIPTION

  This function stores value tags for a new cell for which
  a MIB or a Scheduling Block was received. When a cell is 
  acquired, and the PLMN and Cell id are known, the sib procedure
  will try to find a match in the sib database. If a match is
  found rrc_sibdb_process_value_tags_for_cell is called to
  compare value_tags. If a match is not found, this function
  should be called so that the value_tags can be copied into
  the database.
    
  NOTE: THE FOLLOWING SIBS DO NOT HAVE VALUE-TAGS
  
    SIB7, SIB9, SIB10, SIB14, SIB17
    
  These SIBs only have expiration timers.

DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_store_value_tags_for_new_cell
(
  uint32 dbindex      /* SIB Database Index for the cell for which the
                      new Value Tags have been received */
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  , tdssib_events_index_e_type index
#endif

)
{
  tdsrrc_SIBSb_ReferenceList *sb_ptr=NULL; /* Local pointer to SIBSb ref list   */
  tdsrrc_SIB_ReferenceList *ptr1=NULL;     /* To store Scheduling block1 ref ptr*/
  tdsrrc_SIB_ReferenceList *ptr2=NULL;     /* To store Scheduling block2 ref ptr*/
  tdsrrc_SIB_ReferenceList *ptr=NULL;      /* To store Scheduling block2 ref ptr*/
  boolean sb1_present=FALSE;           /* Indicate if Sched block 1 present */
  boolean sb2_present=FALSE;           /* Indicate if Sched block 2 present */
  uint32 count=0;                      /* Count to loop through Sched Blocks*/
  uint32 n_index=0;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List *ext_siblist_ptr = NULL;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List2 *ext_siblist2_ptr = NULL;
#endif

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
  tdsrrc_MasterInformationBlock *mib_ptr = NULL; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr = NULL;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr = NULL;         /* Local pointer to SB2       */
#endif
  
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  boolean deferred_meas_read_supported = FALSE;
   if(tdsrrcsib_can_event_support_deferred_sib(index) == TRUE)
   {
     deferred_meas_read_supported =
                 tdsrrcsib_can_nw_support_deferred_sib(dbindex);
   }
#endif
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Storing value tags from MIB");

  if( tdssibdb.cell[dbindex].sibs.mib != NULL )
  {
    /* Copy the MIB value tag into the database */
    tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_MIB] =
      tdssibdb.cell[dbindex].sibs.mib->mib_ValueTag;

    /* We have to go through the reference list and
    store all value-tags present in the MIB. */
    sb_ptr = &(tdssibdb.cell[dbindex].sibs.mib->sibSb_ReferenceList);
    n_index = 0;

    while( sb_ptr->n > n_index )
    {
      /* Switch on the value tag's sib type and look
      at the corrosponding SIBs stored value tag for
      a match */
      switch(TDSRRCGET_SIB_TYPE_AND_TAG((&(sb_ptr->elem[n_index].sibSb_Type))))
      {
        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType1):
          
          if( tdssibdb.cell[dbindex].sibs.sib1 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB1]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType1;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB1 value tag stored");
          }
          break;

        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType2):

          if( tdssibdb.cell[dbindex].sibs.sib2 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB2]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType2;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB2 value tag stored");
          }
          break;

        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType3):
  
          if( tdssibdb.cell[dbindex].sibs.sib3 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB3]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType3;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB3 value tag stored");
          }
          break;

        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType4):
          
          if( tdssibdb.cell[dbindex].sibs.sib4 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB4]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType4;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB4 value tag stored");
          }
          break;

        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType5):
          
          if( tdssibdb.cell[dbindex].sibs.sib5 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB5]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType5;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB5 value tag stored");
          }
          break;

        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType6):
          
          if( tdssibdb.cell[dbindex].sibs.sib6 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB6]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType6;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB6 value tag stored");
          }
          break;
         
        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType7):
          
          /* SIB7 does not have a value-tag, it only has an expiration timer */

          break;

        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType11):

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] == TDSINVALID_MIB_SB_VALUE_TAG)&&
             (deferred_meas_read_supported == TRUE)
             )
          {
            tdsdeferred_sib_info.val_tag.sib11_val_tag = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType11;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: SIB11 value tag stored %d",tdsdeferred_sib_info.val_tag.sib11_val_tag);
          }
          else
#endif
          if( tdssibdb.cell[dbindex].sibs.sib11 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType11;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB11 value tag stored");
          }
          break;

        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType12):
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12] == TDSINVALID_MIB_SB_VALUE_TAG)&&
             (deferred_meas_read_supported == TRUE)
             )
          {
            tdsdeferred_sib_info.val_tag.sib12_val_tag = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType12;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: SIB12 value tag stored %d",tdsdeferred_sib_info.val_tag.sib12_val_tag);
          }
          else
#endif
          if( tdssibdb.cell[dbindex].sibs.sib12 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType12;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB12 value tag stored");
          }
          break;

        case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType18):
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18] == TDSINVALID_MIB_SB_VALUE_TAG)&&
             (deferred_meas_read_supported == TRUE)
             )
          {
            tdsdeferred_sib_info.val_tag.sib18_val_tag = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType18;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: SIB18 value tag stored %d",tdsdeferred_sib_info.val_tag.sib18_val_tag);
          }
          else
#endif          
          if( tdssibdb.cell[dbindex].sibs.sib18 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoType18;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB18 value tag stored");
          }
          break;

        case TDSRRCGET_SIB_TYPE_AND_TAG_VALUE_SYS_INFO_TYPE(sysInfoTypeSB1):
     
          if( tdssibdb.cell[dbindex].sibs.sb1 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SB1]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoTypeSB1;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SB1 value tag stored");
  
            /* Note that Sched Block 1 is present */
            sb1_present=TRUE;
          }
          break;

        case TDSRRCGET_SIB_TYPE_AND_TAG_VALUE_SYS_INFO_TYPE(sysInfoTypeSB2):

          if( tdssibdb.cell[dbindex].sibs.sb2 != NULL )
          {
            tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SB2]
             = sb_ptr->elem[n_index].sibSb_Type.u.sysInfoTypeSB2;
            TDSRRC_MSG0(MSG_LEGACY_LOW,"SB2 value tag stored");
  
            /* Note that Sched Block 2 is present */
            sb2_present=TRUE;
          }
          break;

        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown value tag type");
          break;
      }/* end switch */
      n_index++;
    }/* end while */

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
    mib_ptr = tdssibdb.cell[dbindex].sibs.mib;
#endif

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    if(TDSEXTENSION_SIB_SCHED_PRESENT_IN_MIB(mib_ptr))
    {
      ext_siblist_ptr = &(tdssibdb.cell[dbindex].sibs.mib->v690NonCriticalExtensions.
        v6b0NonCriticalExtensions.masterInformationBlock_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
  
      n_index = 0;
      while( ext_siblist_ptr->n > n_index )
      {
        if( TDSRRCCHECK_SIB_TypeExt(
              (&(ext_siblist_ptr->elem[n_index].extensionSIB_Type)),
               systemInfoType11bis)
        )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib11bis:Updating vtag =%d for sib11bis",ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag);
          
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] == TDSINVALID_MIB_SB_VALUE_TAG)&&
             (deferred_meas_read_supported == TRUE)
             )
          {
            tdsdeferred_sib_info.val_tag.sib11bis_val_tag = ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
          }
          else
#endif
          {
            if(tdssibdb.cell[dbindex].sibs.sib11bis != NULL)
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] =
               ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
            }
            
          }
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:extensionSIB_Type =%x not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr->elem[n_index].extensionSIB_Type)));
        }
        n_index++;
      }
    }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
    if(TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_MIB(mib_ptr))
        {
          ext_siblist2_ptr = &(tdssibdb.cell[dbindex].sibs.mib->v690NonCriticalExtensions.v6b0NonCriticalExtensions.
          v860NonCriticalExtensions.masterInformationBlock_v860ext.extSIBTypeInfoSchedulingInfo_List);
      
          n_index = 0;
          while(ext_siblist2_ptr->n > n_index)
          {
            if(ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t & T_tdsrrc_SIB_TypeExt2_systemInfoType19)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib19:Updating vtag =%d for sib19",ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag);
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
              if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] == TDSINVALID_MIB_SB_VALUE_TAG)&&
                 (deferred_meas_read_supported == TRUE))
              {
                tdsdeferred_sib_info.val_tag.sib19_val_tag = ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
              }
              else
#endif 
              {
                if (tdssibdb.cell[dbindex].sibs.sib19 != NULL)
                {
                  tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] =
                  ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
                }
             
            }
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:extensionSIB_Type =%x not supported",ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t);
            }
            n_index++;
          }
        }
#endif

    /* If scheduling blocks are present, save value tags from 
    them also. */
    if( (sb1_present == TRUE) && (tdssibdb.cell[dbindex].sibs.sb1!=NULL) )
    {
      count++;
      ptr1 = &(tdssibdb.cell[dbindex].sibs.sb1->sib_ReferenceList);
#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
      sb1_ptr = tdssibdb.cell[dbindex].sibs.sb1;
#endif

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if((TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(
            sb1_ptr,tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1)))
      {
        ext_siblist_ptr = &(tdssibdb.cell[dbindex].sibs.sb1->v6b0NonCriticalExtensions.
            sysInfoTypeSB1_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
        
        n_index = 0;
        while( ext_siblist_ptr->n > n_index )
        {
          if(ext_siblist_ptr->elem[n_index].extensionSIB_Type.t == 
               T_rrc_SIB_TypeExt_systemInfoType11bis)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib11bis:Updating vtag =%d for sib11bis",ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag);

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] == TDSINVALID_MIB_SB_VALUE_TAG)&&
              (deferred_meas_read_supported == TRUE)
               )
            {
              tdsdeferred_sib_info.val_tag.sib11bis_val_tag = ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
            }
            else
#endif
            {
              if (tdssibdb.cell[dbindex].sibs.sib11bis != NULL)
              {
                tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] =
                ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
              }
              
            }
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:extensionSIB_Type =%d not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr->elem[n_index].extensionSIB_Type)));
          }
          n_index++;
        }
      }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
      if((TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(
            sb1_ptr,tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1)))
          {
            ext_siblist2_ptr = &(tdssibdb.cell[dbindex].sibs.sb1->v6b0NonCriticalExtensions.
             v860NonCriticalExtensions.sysInfoTypeSB1_v860ext.extSIBTypeInfoSchedulingInfo_List);
        
            n_index = 0;
            while(ext_siblist2_ptr->n > n_index)
            {
              if(ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t ==
                 T_tdsrrc_SIB_TypeExt2_systemInfoType19)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib19:Updating vtag =%d for sib19",ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag);
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
                if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] == TDSINVALID_MIB_SB_VALUE_TAG)&&
                   (deferred_meas_read_supported == TRUE))
                {
                  tdsdeferred_sib_info.val_tag.sib19_val_tag = ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
                }
                else
#endif
               {
                if ( tdssibdb.cell[dbindex].sibs.sib19 != NULL)
                {
                  tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] =
                  ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
                }
                
              }
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:extensionSIB_Type =%d not supported",ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t);
              }
              n_index++;
            }
          }
#endif
    }
    if( (sb2_present == TRUE) && (tdssibdb.cell[dbindex].sibs.sb2!=NULL) )
    {
      count++;
      ptr2 = &(tdssibdb.cell[dbindex].sibs.sb2->sib_ReferenceList);

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
      sb2_ptr = tdssibdb.cell[dbindex].sibs.sb2;
#endif

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if((TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(
            sb2_ptr,tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2)))
      {
        ext_siblist_ptr = &(tdssibdb.cell[dbindex].sibs.sb2->v6b0NonCriticalExtensions.
          sysInfoTypeSB2_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
    
        n_index = 0;
        while( ext_siblist_ptr->n > n_index )
        {
          if(ext_siblist_ptr->elem[n_index].extensionSIB_Type.t ==
               T_rrc_SIB_TypeExt_systemInfoType11bis)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib11bis:Updating vtag =%d for sib11bis",ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag);
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] == TDSINVALID_MIB_SB_VALUE_TAG)&&
               (deferred_meas_read_supported == TRUE)
               )
            {
              tdsdeferred_sib_info.val_tag.sib11bis_val_tag = ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
            }
            else
#endif
            {
              if ( tdssibdb.cell[dbindex].sibs.sib11bis != NULL)
              {
                tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] =
                ext_siblist_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
              }
              
            }
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:extensionSIB_Type =%d not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr->elem[n_index].extensionSIB_Type)));
          }
          n_index++;
        }
      }
#endif


#ifdef FEATURE_TDSCDMA_SIB19
      if((TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(
            sb2_ptr,tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2)))
      {
        ext_siblist2_ptr = &(tdssibdb.cell[dbindex].sibs.sb2->v6b0NonCriticalExtensions.
          v860NonCriticalExtensions.sysInfoTypeSB2_v860ext.extSIBTypeInfoSchedulingInfo_List);
    
        n_index = 0;
        while(ext_siblist2_ptr->n > n_index)
        {
          if(ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t &
             T_tdsrrc_SIB_TypeExt2_systemInfoType19)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib19:Updating vtag =%d for sib19",ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag);
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] == TDSINVALID_MIB_SB_VALUE_TAG)&&
               (deferred_meas_read_supported == TRUE)
               )
            {
              tdsdeferred_sib_info.val_tag.sib19_val_tag = ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
            }
            else
#endif
            {
              if (tdssibdb.cell[dbindex].sibs.sib19 != NULL)
              {
                tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] =
                ext_siblist2_ptr->elem[n_index].valueTagInfo.u.cellValueTag;
              }
            
          }
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:extensionSIB_Type =%d not supported",ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t);
          }
          n_index++;
        }
      }
#endif
    }

    while( count != 0 )
    {
      /* Sequentialy copy value-tags from SB1 and SB2 */
      count--;
      if(sb1_present)
      {
        sb1_present=FALSE;
        ptr=ptr1;
      }
      else if(sb2_present)
      {
        sb2_present=FALSE;
        ptr=ptr2;
      }

      n_index = 0;

      if(NULL == ptr)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"INVALID pointer");
        return;
      }

      while( ptr->n > n_index )
      {
        /* Switch on the value tag's sib type and store the value tag */
        switch(ptr->elem[n_index].sib_Type.t)
        {
          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType1):
            
            if( tdssibdb.cell[dbindex].sibs.sib1 != NULL )
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB1]
               = ptr->elem[n_index].sib_Type.u.sysInfoType1;
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB1 value tag stored");
            }
            break;
  
          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType2):
    
            if( tdssibdb.cell[dbindex].sibs.sib2 != NULL )
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB2]
               = ptr->elem[n_index].sib_Type.u.sysInfoType2;
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB2 value tag stored");
            }
            break;

          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType3):
    
            if( tdssibdb.cell[dbindex].sibs.sib3 != NULL )
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB3]
               = ptr->elem[n_index].sib_Type.u.sysInfoType3;
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB3 value tag stored");
            }
            break;

          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType4):
            
            if( tdssibdb.cell[dbindex].sibs.sib4 != NULL )
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB4]
               = ptr->elem[n_index].sib_Type.u.sysInfoType4;
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB4 value tag stored");
            }
            break;

          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType5):
            
            if( tdssibdb.cell[dbindex].sibs.sib5 != NULL )
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB5]
               = ptr->elem[n_index].sib_Type.u.sysInfoType5;
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB5 value tag stored");
            }
            break;

          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType6):
            
            if( tdssibdb.cell[dbindex].sibs.sib6 != NULL )
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB6]
               = ptr->elem[n_index].sib_Type.u.sysInfoType6;
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB6 value tag stored");
            }
            break;
         
          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType7):
            
            /* SIB7 does not have a value-tag, it only has an expiration
            timer */
  
            break;

          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType11):
            
            if( tdssibdb.cell[dbindex].sibs.sib11 != NULL )
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11]
               = ptr->elem[n_index].sib_Type.u.sysInfoType11;
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB11 value tag stored");
            }
            break;

          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType12):
    
        #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12] == TDSINVALID_MIB_SB_VALUE_TAG)&&
               (deferred_meas_read_supported == TRUE)
               )
            {
              tdsdeferred_sib_info.val_tag.sib12_val_tag = ptr->elem[n_index].sib_Type.u.sysInfoType12;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: SIB12 value tag stored %d",tdsdeferred_sib_info.val_tag.sib12_val_tag);
            }
            else
        #endif
            if( tdssibdb.cell[dbindex].sibs.sib12 != NULL )
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12]
               = ptr->elem[n_index].sib_Type.u.sysInfoType12;
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB12 value tag stored");
            }
            break;

          case TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType18):
            
            if( tdssibdb.cell[dbindex].sibs.sib18 != NULL )
            {
              tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18]
               = ptr->elem[n_index].sib_Type.u.sysInfoType18;
              TDSRRC_MSG0(MSG_LEGACY_LOW,"SIB18 value tag stored");
            }
            break;

          default:
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown value tag type");
            break;
        }/* end switch */
        n_index++;
      }/* end while ptr != NULL */
    }/* end while count != 0 */
  }/* end if MasterInformationblock is not null */
  else 
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"MIB ptr is NULL!");
  }
} /* tdsrrcsibdb_store_value_tags_for_new_cell */



/*===========================================================================

FUNCTION RRCSIB_REGISTER_FOR_SIB_CHANGE_NOTIFICATION

DESCRIPTION

  This function registers SIB change notification requests from 
  other procedures. When a procedure needs to be notified when a 
  particular SIB in the serving cell changes, that procedure must register
  using this function. 

  Only when the SIBs change in the serving cell the procedures
  will be notified. When the serving cell itself changes, the procedures
  are not notified - they must use a NEW_CELL_INDICATION notice for
  that.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_register_for_sib_change_notification
(
  tdsrrc_proc_e_type  procedure,   /* Procedure that's doing this registration*/
  tdsrrc_SIB_type sib              /* Sib for which notification is needed    */
)
{
  uint32 count; /* Local counter */
  boolean free_index_found = FALSE; /* Indicate if a space has
                been found to save this registration. */

  /* Check if the given SIB is valid. */
  /*lint -save -e685 */ 
  if( sib >= tdsrrc_ExtensionType )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid SIB registered %d",sib);
    return;
  }
  /*lint -restore */

  /* Register this in the first available space */
  for( count=0; count < TDSRRC_TOT_SIB_CHG_N; count++ )
  {
    if( tdsrrcsib_cn[count].index_used == FALSE )
    {
      free_index_found = TRUE;
      break; /* Break from for loop */
    }
  }

  if((free_index_found == TRUE ) && (count < TDSRRC_TOT_SIB_CHG_N))
  {
    /* Ok to register this. */
    tdsrrcsib_cn[count].procedure = procedure;   /* Copy procedure   */
    tdsrrcsib_cn[count].sib = sib;         /* Copy sib type          */
    tdsrrcsib_cn[count].index_used = TRUE; /* This index is now used */
    tdsrrcsibdb_set_priority(count,sib);    /*Set priority for this sibtype,which will
                                          be used for posting sib_change_ind */
  }
  else
  {
    /* All indices are taken up - 
    We probably need to increase the count */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No space to register SIB CN");
  }
}


/*===========================================================================

FUNCTION RRCSIB_DEREGISTER_SIB_CHANGE_NOTIFICATION

DESCRIPTION

  This function de-registers previously registered SIB change 
  notification requests. When a procedure no longer needs to be 
  notified of sib changes, it may de-register using this
  function.
   
  This is not mandatory but if the procedure does 
  not de-register, it will continue to receive sib change
  notifications.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsib_deregister_sib_change_notification
(
  tdsrrc_proc_e_type  procedure,   /* Procedure that's de-registering         */
  tdsrrc_SIB_type sib              /* Sib for which notification is not needed*/
)
{
  uint32 count; /* Local counter */

  /* Find the previous registration and remove it */
  for( count=0; count < TDSRRC_TOT_SIB_CHG_N; count++ )
  {
    if( tdsrrcsib_cn[count].index_used == TRUE )
    {
      if( (tdsrrcsib_cn[count].procedure == procedure) &&
          (tdsrrcsib_cn[count].sib == sib) )
      {
        /* Match found. This index is no longer used. */
        tdsrrcsib_cn[count].index_used = FALSE;
        break; /* Break from for loop */
      }
    }
  }
}



/*===========================================================================

FUNCTION RRC_SIBDB_NOTIFY_SIB_CHANGE_TO_PROCS

DESCRIPTION

  This function looks through the stored sib change registrations
  to see if the sib changes should be notified to any of
  the registered procedures. This is called by the sib procedure
  when the TDSRRCSIB_UPDATE_SIBS event is active and all changed
  sibs haave been decoded.

  If it finds procedures that need to be notified, it sends
  an internal command TDSRRC_SIB_CHANGE_IND to the internal command
  queue.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_notify_sib_change_to_procs( void )
{
  tdsrrc_cmd_type *cmd_ptr;
  tdsrrc_sib_change_mask_type sib_change_mask=0;
  uint8 procedure;
  uint8 priority; 

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending notification about sib change with tdsrrcsib_cn_max_priority=%d",tdsrrcsib_cn_max_priority);

  /* Check which SIB has changed by looking at the
  active cell SIB change indication */
  //  for( count1 =0; count1 < TDSRRC_TOT_SIBS; count1++ )
  /* Processing SIB in the reverse order - from sib 18 to sib 1 */
  for(priority = 1;priority<=tdsrrcsib_cn_max_priority;priority++)
  {
    for(procedure=(uint8)TDSMAX_NUM_OF_PROCEDURES;procedure>(uint8)0;procedure--)
    {
      if(tdsrrcsibdb_is_it_registered_procedure_for_sibchange((tdsrrc_proc_e_type)procedure) == TRUE)
      {
        if(tdsrrcsib_check_and_prepare_sib_change_mask(&sib_change_mask,(tdsrrc_proc_e_type)procedure,priority) == TRUE)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB change ind sent to proc %d with bit mask =%x",procedure,sib_change_mask);
          if( (cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
          {
            /* Fill up the command type, the sib, and the procedure
            name. */
            cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIB_CHANGE_IND;
            cmd_ptr->cmd.sib_change_ind.sib_change_mask= sib_change_mask;
            cmd_ptr->cmd.sib_change_ind.procedure = (tdsrrc_proc_e_type)procedure;
            /* Now, send the command. */
            tdsrrc_put_int_cmd( cmd_ptr );
          }
          else
          {
            /* Out of memory - fatal error */
            ERR_FATAL("No memory for SIB change ind!",0,0,0);
          }
        }
      }
    }
  }
  /* Whenever we get a new SIB1 we need to check if the
  connected mode timers and constants need to be updated.
  */
  if( tdssibdb.active_cell_sib_changed[tdsrrc_SIB1] == TRUE )
  {
    if( (tdsrrcumi_check_ue_timer_constants_update_status(FALSE))
        == TDSRRC_CONN_TIMER_AND_CONSTANTS_UPDATE_REQ )
    {

      (void)tdsrrc_update_ue_conn_timers_and_constants
        (
          (TDSRRCCHECK_SIB_MSG_IE_PRESENT(tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1,
            rrc_SysInfoType1_, 
            ue_ConnTimersAndConstants))? 
            &(tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1->ue_ConnTimersAndConstants):
            NULL,
          (TDSRRCCHECK_SIB_MSG_IE_PRESENT(tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1,
             rrc_SysInfoType1_, 
             v3a0NonCriticalExtensions))?
            &(tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1->v3a0NonCriticalExtensions.
            sysInfoType1_v3a0ext.ue_ConnTimersAndConstants_v3a0ext):
            NULL
#if defined(FEATURE_3GPP_FAST_DORMANCY)
          ,((tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1->m.v3a0NonCriticalExtensionsPresent) &&
            (tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1->v3a0NonCriticalExtensions.m.v860NonCriticalExtentionsPresent))? 
            &(tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1->v3a0NonCriticalExtensions.v860NonCriticalExtentions.
            sysInfoType1_v860ext.ue_ConnTimersAndConstants):
            NULL
#endif     
        );
    }
#if defined(FEATURE_3GPP_FAST_DORMANCY)
    else
    {
      (void)tdsrrc_update_ue_conn_timers_and_constants(NULL, NULL,
        ((tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1->m.v3a0NonCriticalExtensionsPresent) &&
          (tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1->v3a0NonCriticalExtensions.m.v860NonCriticalExtentionsPresent))? 
          &(tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib1->v3a0NonCriticalExtensions.v860NonCriticalExtentions.
          sysInfoType1_v860ext.ue_ConnTimersAndConstants):
          NULL
        );
    }
#endif
  }
}/* end function - rrcscmgr_notify_sib_change_to_procs */
/*===========================================================================

FUNCTION RRC_SIBDB_CHECK_IF_SB_PRESENT

DESCRIPTION

  This function checks if a Scheduling Block (1 or 2) is
  present in a particular cell. This is done by looking 
  through the MIB's scheduling information.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  TRUE if the SB is present. FALSE otherwise.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcsibdb_check_if_sb_present
(
  tdsrrc_SIB_type sib, /* Either SB1 or SB2 */ 
  uint32 dbindex    /* Database index    */
)
{
  tdsrrc_SIBSb_ReferenceList *sb_ptr; /* Local pointer to SIBSb ref list */
  boolean match_found = FALSE;    /* Indicate if match is found      */
  tdsrrc_MasterInformationBlock *mib = NULL;
  uint32 n_value=0, index=0;

  if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_MIB] )
  {
    /* If the MIB is valid */
    mib = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_MIB,dbindex);
    if(mib != NULL)
    {
      sb_ptr = &(mib->sibSb_ReferenceList);
    
      n_value = sb_ptr->n;
      index = 0;
    
      while( n_value != 0 )
      {
          /* Check if the given scheduling block matches
          this particular sib type */
        if( (TDSRRCCHECK_SIBSb_TypeAndTag((&sb_ptr->elem[index].sibSb_Type),sysInfoTypeSB1)) &&
              (sib == tdsrrc_SB1) )
        {
          /* We found it */
          match_found = TRUE;
          break; /* Out of the while loop */
        }
        else if( (TDSRRCCHECK_SIBSb_TypeAndTag((&sb_ptr->elem[index].sibSb_Type),sysInfoTypeSB2)) &&
                   (sib == tdsrrc_SB2) )
        {
          /* We found it */
          match_found = TRUE;
          break; /* Out of the while loop */
        }
        /* Now increment the index */
        n_value--;
        index++;
      }
    }                     
    else
    {
      /* If the MIB hasn't been read yet, we'll assume
      the scheduling block is present. */
      match_found = TRUE;
    }
  }
  return( match_found );
}


/*===========================================================================

FUNCTION RRC_SIBDB_CLEAR_ACTIVE_CELL_SIB_CHANGE

DESCRIPTION

  This function clears the variable that saves
  the active cell SIB change indications.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  TRUE if the SB is present. FALSE otherwise.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_clear_active_cell_sib_change( void )
{
  uint32 index;
  
  /* Clear the Active Cells SIB Change indications */
  for( index=0; index<TDSRRC_TOT_SIBS; index++ )
  {
    tdssibdb.active_cell_sib_changed[index] = FALSE;
  }
}


/*===========================================================================

FUNCTION RRC_SIBDB_SAVE_ACTIVE_CELL_SIB_CHANGE

DESCRIPTION

  If a SIB in the active cell is changed by the UTRAN this function
  is called to save the information about the SIB that has changed.
  Later when all the changed SIBs have been read, the change information
  is sent to other RRC procedures if needed.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_save_active_cell_sib_change
( 
  tdsrrc_SIB_type sib   /* The sib that has changed.*/
)
{

  /* If the given sib is not in the valid range indicate
  the error. */
  /*lint -save -e685 */ 
  /* TRUE if the number is a valid sib.*/
  if(!(TDSSIBDB_VALID_SIB_RANGE(sib)))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid SIB %d",sib);
  }
  /*lint -restore */
  else
  {
    /* Note that this particular SIB has changed */
/*lint -e661 lint bug - incorrect out of bounds for ? operator */
    tdssibdb.active_cell_sib_changed[sib] = TRUE;
/*lint +e661 */
  }
}


/*===========================================================================

FUNCTION RRC_GET_SERVING_CELL_ID

DESCRIPTION

  This function gets the current serving cell's id in the
  format of an unsigned char array of 2 elements with the
  LSB 8 bits of the cell-id in the first element and the 
  2nd LSB 8 bits of the cell-id in the second element.
  Only the first 16 bits of the cell-id are used - the rest
  if any, are ignored.

  It will also fill in the length in bits of the cell id.

  At present this function is meant to be used only by
  GSTK module.

DEPENDENCIES

  The calling function is responsible for allocating 
  the appropriate amount of space to the 2 pointers that are
  input to this function.
  
RETURN VALUE
  
  TRUE if cell-id has been copied, FALSE if it's not.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_get_serving_cell_id
( 
  uint8* cell_id, /* Cell Id to be copied */
  uint8* length   /* Length of the Cell Id to be copied */
)
{
  tdsrrcsib_cell_id_convert_type temp;
  uint32 dbindex;

  if( (cell_id == NULL ) || (length == NULL) )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Uninitialized pointers!");
    return(FALSE);
  }

  if( tdssibdb.active_cell_sibs_valid == FALSE )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No active cell in tdssibdb!");
    return(FALSE);
  }

  /* Note the current active index */
  dbindex = tdssibdb.active_cell_index;

  /* Copy the cell-id length directly from SIB3 */
  if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB3] )
  {
    /* Ok to typecast to uint8 since longest length is only 28 */
    *length = (uint8)tdssibdb.cell[dbindex].sibs.sib3->cellIdentity.numbits;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB3 not present in serving cell!");
    return(FALSE);
  }

  /* Copy cell-id from database into union */
  temp.int_cell_id = tdssibdb.cell[dbindex].cell_id;

  /* Copy first 2 bytes into received pointer. */
  *cell_id = temp.ext_cell_id[0];
  *(cell_id+1) = temp.ext_cell_id[1];

  //purpously changging order here between 3 and 2, since GSTK want's it in the reverse order
  *(cell_id+2) = temp.ext_cell_id[3];
  *(cell_id+3) = temp.ext_cell_id[2];
  
  return(TRUE);
  
}

/*===========================================================================

FUNCTION RRC_GET_SERVING_CELL_MIB_VALUE_TAG

DESCRIPTION

  This function gets the current serving cell's 
  MIB Value tag to the calling function 

DEPENDENCIES
  None
  
RETURN VALUE
  
  Mib Value tag of serving cell index.

SIDE EFFECTS

  None.

===========================================================================*/
  uint32 tdsrrcsibdb_get_serving_cell_mib_value_tag(void)
  {
    uint32 dbindex;
    if(!(tdssibdb.active_cell_sibs_valid))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No active cell selected yet!");
      return(TDSINVALID_MIB_SB_VALUE_TAG);
    }
    dbindex = tdssibdb.active_cell_index;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Stored MIB V-tag=%d",tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_MIB]);
    return (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_MIB]);
  }

/*===========================================================================

FUNCTION RRC_SIBDB_FIND_CELL_INDEX_FOR_PLMN_FREQ_PSC

DESCRIPTION

  This function tries to find the index in the sib
  database for the given plmn-id, frequency and primary scrambling
  code combination.
  
  If a match is found, the correct index is returned.
  
DEPENDENCIES

  SIB Database should be correctly initialized. The PLMN
  and Cell Ids given by the function should be in the
  database.

RETURN VALUE

  Index of the cell. If this cell is not present
  in the SIB database, TDSSIBDB_INVALID_INDEX is returned.

SIDE EFFECTS

  None.

===========================================================================*/
uint32 tdsrrcsibdb_find_cell_index_for_plmn_freq_cpid
(
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */
  uint32 freq,    /* Frequency of the cell */
  uint8 cpid, /* cpid       */
  tdssib_events_index_e_type  event_index /*Current sib event index*/
)
{    
  uint32 index=TDSSIBDB_INVALID_INDEX ;  /* To find the matching index */

  /* Try to match the PLMN and Cell Ids to one of the cells
  stored in the database. */
  for( index=0; index<=TDSSIBDB_MAX_INDEX; index++ )
  {
    /* If this particular index is being used */
    if(( tdssibdb.cell[index].index_used == TRUE )
       &&( tdssibdb.cell[index].sib_db_valid_for_quick_cell_resel == TRUE ))
    {
      /* Match the freq and scr_code. */
      if( (freq == tdssibdb.cell[index].freq) &&
          (cpid == tdssibdb.cell[index].cpid) &&
          (tdsrrc_bcch_plmns_match((plmn), (tdssibdb.cell[index].plmn_id))))
      {
        if((tdsrrcsib_is_need_to_cleanup_cell(event_index) == TRUE) &&
            (tdssibdb.cell[index].six_hr_count  >=  TDSMAX_CELL_INDEX_COUNT_SIXHR_CELL_SEL_RESEL))
        {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"6hr sib:Clean sib db Index =%d,six hr count =%d ",index,tdssibdb.cell[index].six_hr_count);
            tdsrrcsibdb_clear_a_cell_index(index);
            index=TDSSIBDB_INVALID_INDEX ;
        }
        /* A Match has been found. Get out of the for loop. */
        break; /* Break out of the for loop */
      }
    }
    else if(( tdssibdb.cell[index].index_used == TRUE )
            &&( tdssibdb.cell[index].sib_db_valid_for_quick_cell_resel == FALSE ))
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"The SIB DB of (freq = %d, cpid = %d, cell_id = 0X%x ) is invalid for quick cell reselection ",
                  tdssibdb.cell[index].freq,
                  tdssibdb.cell[index].cpid,
                  tdssibdb.cell[index].cell_id);
    }
  }/* end for */

  return( index );
 
} /* end function rrc_sibdb_find_cell_index_for_freq_psc */

/*===========================================================================

FUNCTION tdsrrcsibdb_invalid_sibdb_for_quick_cell_resel

DESCRIPTION

  This function is to invalid the sibdb for quick cell reselection if TDS stop happens or LTA is longer thant 5 min.
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_invalid_sibdb_for_quick_cell_resel
(
 void
)
{    
  uint32 index=TDSSIBDB_INVALID_INDEX; 

  for( index=0; index<=TDSSIBDB_MAX_INDEX; index++ )
  {
    /* If this particular index is being used */
    if(tdssibdb.cell[index].index_used == TRUE)
    {
      tdssibdb.cell[index].sib_db_valid_for_quick_cell_resel = FALSE;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid SIB DB for quick cell reselection, freq =%d, cpid=%d, cell_id=0X%x",
                  tdssibdb.cell[index].freq,
                  tdssibdb.cell[index].cpid,
                  tdssibdb.cell[index].cell_id);
    }
  }/* end for */

  return;
 
} /* end function tdsrrcsibdb_invalid_sibdb_for_quick_cell_resel */

/*===========================================================================

FUNCTION TDSRRCSIBDB_COMPARE_LAC_RAC_WITH_SERVING_CELL

DESCRIPTION

  This function compares the serving LAC/RAC with the SIBdb cell's.
  If LAC/RAC mismatch, return TRUE.
  If match, return FALSE.
  
DEPENDENCIES



RETURN VALUE

  If LAC/RAC mismatch, return TRUE.
  If match, return FALSE.
  
SIDE EFFECTS

  None.

===========================================================================*/

boolean tdsrrcsibdb_compare_lac_rac_with_serving_cell(tdsrrc_SysInfoType1 *SIBdb_sib1_ptr)
{
  boolean ret = FALSE; /* default value: normal cell */

  rrc_lai_type serving_lai;
  
  tdsrrc_SysInfoType1 *sib1_ptr;
  rrc_rai_type serving_rai;
  serving_rai.rac[0] = 0;
  serving_lai.lac[0] = 0;
  serving_lai.lac[1] = 0;
  
  sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1);
  /* get serving LAC*/
  (void)tdsrrc_get_lac_id(&serving_lai);
  TDSRRC_MSG2(MSG_LEGACY_MED,"Serving LAC[0]: %d,LAC[1] %d",serving_lai.lac[0],serving_lai.lac[1]);
  /* get serving RAC*/
  if((sib1_ptr != NULL) &&
      (tdsrrc_extract_rac_from_sib_info(&serving_rai, &sib1_ptr->cn_DomainSysInfoList)))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED,"Serving RAC: %d",serving_rai.rac[0]);
  }
 
  
  if (SIBdb_sib1_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrcsibdb_high_cost_cell got null SIB1 pointer");
  }
  else
  {
    rrc_gsm_map_nas_sys_info_type   nas_common_info;
      
    nas_common_info.length = (uint32) SIBdb_sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
    if (nas_common_info.length <= RRC_MAX_GSM_INFO_OCTET_COUNT)
    {
      uint32 i;
      unsigned char  *tdsdata_ptr;
      rrc_lai_type  lai_new;

      tdsdata_ptr = SIBdb_sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
      for( i = 0; i < SIBdb_sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
      {
        nas_common_info.value[i] = *tdsdata_ptr;
        tdsdata_ptr ++;
      }
      /* get SIBdb cell LAC */
      if(mm_extract_lac_from_sib_info(&lai_new, &nas_common_info) == FALSE)
      {
       	TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't get LAC for SIBdb cell");
      }
	  else /* LAC is valid */
      {
        uint8 j;
        /* Begin to match LAC */
        TDSRRC_MSG4(MSG_LEGACY_MED, "Compare LAC serving(%d, %d) SIBdb cell(%d, %d)", 
                              serving_lai.lac[0], serving_lai.lac[1], lai_new.lac[0], lai_new.lac[1]);

        for (j = 0; j < RRC_MAX_GSMMAP_LAC_LENGTH; j++)
        {
          if (serving_lai.lac[j] != lai_new.lac[j])
          {
            break;
          }
        }
        if (j != RRC_MAX_GSMMAP_LAC_LENGTH)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH, "LAC mismatch,HIGH COST cell!");
		  ret = TRUE;
        }
			  
        else /* j == RRC_MAX_GSMMAP_LAC_LENGTH */
        {

		  /* get SIBdb cell RAC */
		  rrc_rai_type rai_new;
          rai_new.rac[0] = 0;
          TDSRRC_MSG0(MSG_LEGACY_MED, "LAC match,will compare RAC");//CRMQTF
		  if (tdsrrc_extract_rac_from_sib_info(&rai_new, &SIBdb_sib1_ptr->cn_DomainSysInfoList) == FALSE)
	      {
			TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't get RAC for SIBdb cell");
		  }
		  else
	      {
		    /* Begin to match RAC */
			TDSRRC_MSG2(MSG_LEGACY_MED, "Compare RAC serving %d,SIBdb cell %d",serving_rai.rac[0],rai_new.rac[0]);
			if (serving_rai.rac[0] != rai_new.rac[0])
		    {
			  TDSRRC_MSG0(MSG_LEGACY_HIGH, "RAC mismatch,HIGH COST cell!");
			  ret = TRUE;
			}
			else
			{
		      TDSRRC_MSG0(MSG_LEGACY_MED, "LAC/RAC match,normal cell");
			  ret = FALSE;
			}
		  }
        }
       }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid nas_common_info.length=%d",nas_common_info.length);
      }
   }

   return ret;
   
}

/*===========================================================================

FUNCTION TDSRRCSIBDB_FIND_CELL_INDEX_FOR_FREQ_CPID

DESCRIPTION

  This function tries to find the index in the sib
  database for the given cell info and frequency.
    
  If a match is found,
  compares the serving LAC/RAC with the SIBdb cell's.
     if LAC/RAC mismatch, high cost indicator will be high-cost cell.
     otherwise, it will be normal cell.

  If not found,
  the high cost indicator will be normal cell.
  
DEPENDENCIES


RETURN VALUE

 None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcsibdb_find_cell_index_for_freq_cpid
(
	tdsl1_cell_info_struct_type  *cell_info,    /* The cell info */ 
	uint32 freq    /* Frequency of the cell */
)
{    
  uint32 index = TDSSIBDB_INVALID_INDEX ;  /* To find the matching index */

  /* Try to match the PLMN and Cell Ids to one of the cells stored in the database. */
  for( index = 0; index <= TDSSIBDB_MAX_INDEX; index++ )
  {
    /* If this particular index is being used */
    if( tdssibdb.cell[index].index_used == TRUE )
    {
      /* Match the freq and cpid. */
      if( (freq == tdssibdb.cell[index].freq) &&
          (cell_info->cpi== tdssibdb.cell[index].cpid) )
      {
        /* A Match has been found. Now decide the high-cost indicator. */
		/* First, check whether sib1 is null in sibdb, if null, decode it from sib1_encoded_ptr */
		if(tdssibdb.cell[index].sibs.sib1 == NULL)
		{
		  tdsrrc_SysInfoType1 *sib1_tmp_ptr = NULL;/* Local ptr */
		  if(tdssibdb.cell[index].sibs.sib1_encoded_pdu_length != TDSINVALID_PDU_LENGTH)
		  {
			/* ASN.1 decode the PDU */
			sib1_tmp_ptr =  tdsrrcasn1_decode_pdu( 
			(void *)(tdssibdb.cell[index].sibs.sib1_encoded_ptr), /* Encoded SIB	   */
			tdsrrc_SysInfoType1_PDU,													   /* SIB's PDU number */
			tdssibdb.cell[index].sibs.sib1_encoded_pdu_length	 /* SIB's Length in bytes */
			);
			if(sib1_tmp_ptr == NULL)
			{
			  TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcsibdb_find_cell_index_for_freq_cpid:Asn.1 decode failed from SIB1");
			}
			else /* sib1 decoded */
			{
			  /* use the decoded sib1: sib1_tmp_ptr */
			  if( tdsrrcsibdb_compare_lac_rac_with_serving_cell(sib1_tmp_ptr) == TRUE )
			  {
			    cell_info->high_cost_cell = TRUE;
		        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB1 decoded, SIBDB index for Freq and CPID found,HIGH COST cell");
		      }
		      else
		      {
		        cell_info->high_cost_cell = FALSE;
		        TDSRRC_MSG0(MSG_LEGACY_MED,"SIB1 decoded, SIBDB index for Freq and CPID found,normal cell");
			    }
			    tdsrrcsibdb_free_sib( tdsrrc_SysInfoType1_PDU,(void *)sib1_tmp_ptr );
			    break; /* Break out of the for loop */
			  }
		  }
		  else
		  {
			TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcsibdb_find_cell_index_for_freq_cpid:sib1_encoded_ptr is NULL");
		  }
		}
		else /* sib1 is not null */
		{
		  if( tdsrrcsibdb_compare_lac_rac_with_serving_cell(tdssibdb.cell[index].sibs.sib1) == TRUE )
		  {
		    cell_info->high_cost_cell = TRUE;
		    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIBDB index for Freq and CPID found,HIGH COST cell");
		  }
		  else
		  {
		    cell_info->high_cost_cell = FALSE;
		    TDSRRC_MSG0(MSG_LEGACY_MED,"SIBDB index for Freq and CPID found,normal cell");
		  }
		  break; /* Break out of the for loop */
		}
      }
    }
	}/* end for */

  if( index == TDSSIBDB_INVALID_INDEX )
  {
     TDSRRC_MSG0(MSG_LEGACY_MED,"SIBDB index for Freq and CPID not found,normal cell");
  }
  
} /* end function tdsrrcsibdb_find_cell_index_for_freq_cpid */

#ifdef FEATURE_TDSCDMA_SPECIFIC_SIB_READING	
/*===========================================================================

FUNCTION tdsrrcsibdb_find_cell_index_for_freq_cpid_for_special_reading

DESCRIPTION

  This function tries to find the index in the sib
  database for the given CPID and frequency.
    
  
DEPENDENCIES


RETURN VALUE

 boolean.

SIDE EFFECTS

  None.

===========================================================================*/

uint32 tdsrrcsibdb_find_cell_index_for_freq_cpid_for_special_reading
(
	uint32       freq,    /* Frequency of the cell */
	uint8        cpi    /* The CPID */ 
)
{    
  uint32 index = TDSSIBDB_INVALID_INDEX ;  /* To find the matching index */
  boolean found = FALSE; /* default value */

  /* Try to match the Freq and CPID to one of the cells stored in the database. */
  for( index = 0; index <= TDSSIBDB_MAX_INDEX; index++ )
  {
    /* If this particular index is being used */
    if( tdssibdb.cell[index].index_used == TRUE )
    {
      /* Match the freq and cpid. */
      if( (freq == tdssibdb.cell[index].freq) &&
          (cpi == tdssibdb.cell[index].cpid) )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIBDB index for Freq and CPID found");
        found = TRUE;
        break; /* Break out of the for loop */
      }
    }
	}/* end for */

  if( found == FALSE )
  {
     index = TDSSIBDB_INVALID_INDEX;
     TDSRRC_MSG0(MSG_LEGACY_MED,"SIBDB index for Freq and CPID not found,normal cell");
  }

	return index;
} /* end function tdsrrcsibdb_find_cell_index_for_freq_cpid */
#endif/* FEATURE_TDSCDMA_SPECIFIC_SIB_READING */


/*===========================================================================

FUNCTION tdsrrcsibdb_update_six_hr_timer

DESCRIPTION

  This function will Implement the required six hours 
  Timer for each of the valid sib db index.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
     
===========================================================================*/

void  tdsrrcsibdb_update_six_hr_timer(void)
{
  uint8     count;

  {
    for( count=0; count<=TDSSIBDB_MAX_INDEX; count++ )
    {
      if(tdssibdb.cell[count].index_used == TRUE)
      {
        if(tdssibdb.cell[count].six_hr_count !=TDSMAX_CELL_INDEX_COUNT_SIXHR)
        {
          tdssibdb.cell[count].six_hr_count ++;
        }
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"6hr sib:Count value for index = %d is %d",count,tdssibdb.cell[count].six_hr_count);
      }
    }
  }
  //Start 10min timer for getting six hours timer
  tdsrrctmr_start_timer( TDSRRCTMR_SIXHR_SIB_UPDATE_TIMER,
                                   TDSRRCSIB_SIX_HR_SIB_UPDATE_TIMER );
}

/*===========================================================================

FUNCTION rrcsib_clear_a_cell_index

DESCRIPTION

  This function will clear all the SIB's/MIB/SB's for the given cell inedx
  for the SIB db.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsibdb_clear_a_cell_index(uint32 dbindex)
{
  if (dbindex != TDSSIBDB_INVALID_INDEX)
  {
    tdssibdb.cell[dbindex].index_used = FALSE; /* This index is now used */
    if(tdssibdb.num_stored_cells)
    {
      tdssibdb.num_stored_cells--;
    }
          
    tdsrrcsibdb_adjust_age_index( dbindex ); /* Adjust age index for cells */
    tdsrrcsibdb_cell_header_init(dbindex);

  
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Clearing the sibdb_var's associated with sibdb_index %d",dbindex);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"InValid SIB-DB Index=%d",dbindex);
  }
}

/*===========================================================================

FUNCTION rrcsib_encode_sib_change_mask

DESCRIPTION

  This function prepares sib change indication bit mask.
  which will be sent to all the procedures which have registered 
  with sib for a notification
  
DEPENDENCIES
  None.

RETURN VALUE
TRUE : If Sibs have changed for this registered procedure
FALSE: Otherwise

SIDE EFFECTS
  None.
     
===========================================================================*/
uint8 tdsrrcsib_check_and_prepare_sib_change_mask
(
tdsrrc_sib_change_mask_type *sib_mask,
tdsrrc_proc_e_type procedure,
uint8 priority
)
{
  uint8 i,count;
  uint8 sib_change_status = FALSE;
  tdsrrc_sib_change_mask_type  gen_mask;
  *sib_mask = 0;

  for( count=0; count < TDSRRC_TOT_SIB_CHG_N; count++ )
  {
    if((tdsrrcsib_cn[count].procedure == procedure)&&
        (tdsrrcsib_cn[count].index_used == TRUE)&&
        (tdsrrcsib_cn[count].priority== priority)
      )
    {
      TDSRRC_MSG2(MSG_LEGACY_MED,"prepare_sib_change_mask for procedure=%d & sibType=%d ",tdsrrcsib_cn[count].procedure,tdsrrcsib_cn[count].sib);
      /* Indicate which SIBs are changed */
      gen_mask = TDSMIB_BIT;
      for(i = 0;i < TDSRRC_TOT_SIBS;i++, gen_mask <<= 1)
      {
        if((tdssibdb.active_cell_sib_changed[i] == TRUE)&&
           (tdsrrcsib_cn[count].sib ==(tdsrrc_SIB_type)i)
          )
        {
          *sib_mask |= gen_mask;
          TDSRRC_MSG2(MSG_LEGACY_MED,"prepare_sib_change_mask = %x, gen mask=%x", *sib_mask,gen_mask);
          if(sib_change_status == FALSE)
          {
            sib_change_status = TRUE;
          }
          break;
        }
      }
    }
  }
  return sib_change_status;
}
/*===========================================================================

FUNCTION tdsrrcsibdb_is_it_registered_procedure_for_sibchange

DESCRIPTION

  This function will loop through regestered procedure 
  data base for sib change to look whether the passed 
  procedure has regestered for sib change
  
DEPENDENCIES
  None.

RETURN VALUE
TRUE : If Sibs have changed for this registered procedure
FALSE: Otherwise

SIDE EFFECTS
  None.
     
===========================================================================*/
uint8 tdsrrcsibdb_is_it_registered_procedure_for_sibchange(tdsrrc_proc_e_type procedure)
{
  uint8 count;
  uint8 status=FALSE;
  
  for( count=0; count < TDSRRC_TOT_SIB_CHG_N; count++ )
  {
    if((tdsrrcsib_cn[count].procedure == procedure)&&
        (tdsrrcsib_cn[count].index_used == TRUE)
      )
    {
      status = TRUE;
      break;
    }
  }
  return status;
}
/*===========================================================================

FUNCTION tdsrrcsibdb_set_priority

DESCRIPTION

  This function will set a priority for reveived sib type
  for registration which will be used for posting sib change ind
  to procedures when ever these sibs are changed.
DEPENDENCIES
  Boundary check of the index should be done by the caller of this function

RETURN VALUE
none
SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsibdb_set_priority(uint32 index,tdsrrc_SIB_type sib)
{
  /*These case hard codes priorities for the sib type which are being registered
    by procedures. If  a sib type change needs to be intimated to procedures ahead
    of other, then the priorities needs to be changed. Current sequence should not 
    be altered.*/

  switch(sib)
  {
    case tdsrrc_SIB5:
    case tdsrrc_SIB6:
      tdsrrcsib_cn[index].priority = 1;
      break;
      
    case tdsrrc_SIB11:
    case tdsrrc_SIB12:
    case tdsrrc_SIB18:
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    case tdsrrc_SIB11bis:
#endif

#ifdef FEATURE_TDSCDMA_SIB19
    case tdsrrc_SIB19:
#endif

      tdsrrcsib_cn[index].priority = 2;
      break;
     
    case tdsrrc_SIB1:
    case tdsrrc_SIB3:
    case tdsrrc_SIB4:
      tdsrrcsib_cn[index].priority = 3;
      break;
      
    case tdsrrc_SIB2:
      tdsrrcsib_cn[index].priority = 4;
      break;
      
    default:
      tdsrrcsib_cn[index].priority = 5;
      break;
  }
  
  if(tdsrrcsib_cn_max_priority < tdsrrcsib_cn[index].priority)
  {
    tdsrrcsib_cn_max_priority = tdsrrcsib_cn[index].priority; 
  }
}
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
/*===========================================================================

FUNCTION tdsrrcsibdb_get_vtag_of_a_sib_for_serving_cell

DESCRIPTION

  This function will return the value tag for the SIB type
  passed as input to this function for serving cell. 
  If serving cell sib db index is valid then the corresponding 
  value tag is returned else an invalid value tag is returned
  
DEPENDENCIES
  None.

RETURN VALUE
  If serving cell sib db index is valid then the corresponding 
  value tag is returned 
  else an invalid value tag is returned

SIDE EFFECTS
  None.
     
===========================================================================*/
uint8 tdsrrcsibdb_get_vtag_of_a_sib_for_serving_cell(tdsrrc_SIB_type sibtype)
{
  uint8 vtag;

  if(!tdssibdb.active_cell_sibs_valid)
  {
    vtag = TDSINVALID_SIB_VALUE_TAG;
  }
  else
  {
    vtag = (uint8)tdssibdb.cell[tdssibdb.active_cell_index].sib_value_tag[sibtype];
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"sibtype = %d has Vtag = %d, tdssibdb.active_cell_sibs_valid = %d",
                                                                           sibtype,
                                                                           vtag,
                                                                           tdssibdb.active_cell_sibs_valid);

  return vtag;
}
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

/*===========================================================================

FUNCTION tdsrrcsibdb_is_serving_cell_sib_vaild

DESCRIPTION

  This function will return the status of serving cell SIB information. 
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE:If SIB db index for serving cell is valid
  FALSE:otherwise
SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsibdb_is_serving_cell_sib_vaild
(
  void
)
{
  return(tdssibdb.active_cell_sibs_valid);
}
/*===========================================================================

FUNCTION tdsrrcsibdb_print_cellid_psc_plmnid_from_sibdb

DESCRIPTION

  This function will prints cell id, plmn id & CPID from sib db
  
DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsibdb_print_cellid_psc_plmnid_from_sibdb
(
  void
)
{
  uint32 count;  /* Counter */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"******************************");
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Print tdssibdb contents");

  for( count=0; count <=TDSSIBDB_MAX_INDEX; count++ )
  {
    if( tdssibdb.cell[count].index_used == TRUE)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell I.d =%d, fre=%d,cpid=%d",tdssibdb.cell[count].cell_id,
                                                    tdssibdb.cell[count].freq,
                                                    tdssibdb.cell[count].cpid);
      tdsrrc_print_plmn_id(tdssibdb.cell[count].plmn_id);
    }
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"******************************");
}
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
/*===========================================================================

FUNCTION tdsrrcsibdb_is_sib11bis_present_in_srv_cell

DESCRIPTION

  This function returns TRUE is SIB11bis is present in serving cell.
  other wise it returns FALSE
DEPENDENCIES
none
RETURN VALUE

TRUE is SIB11bis is present in serving cell.
FALSE: other wise 

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcsibdb_is_sib11bis_present_in_srv_cell
(
  void
)
{
  boolean status = FALSE;
  if(!tdssibdb.active_cell_sibs_valid)
  {
    return FALSE;
  }
  /*Check if SIB11bis present for serving cell*/
  if(tdssibdb.cell[tdssibdb.active_cell_index].sib_valid[tdsrrc_SIB11bis] &&
     (tdsrrcsib_check_if_extension_sib_present(tdssibdb.active_cell_index) == TRUE)
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis:SIB11bis present for serving cell");
    status = TRUE;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis:SIB11bis Not present for serving cell");
  }
  return status;
}
#endif

/*===========================================================================

FUNCTION tdsrrcsibdb_decode_all_sibs_for_a_cell

DESCRIPTION

  This function ASN.1 decode all sibs for a cell. 
  It uses ASN.1 encoded messages which are stored in sibdb 
  
DEPENDENCIES

 None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_decode_all_sibs_for_a_cell
(
  uint32 index /* Index of the cell header to be initialized */
)
{
  uint32 pdu_length = 0;
  
  if( tdssibdb.cell[index].sibs.mib == NULL )
  {
    /* Get the PDU length in bytes */
    //CRC:nickle
    if(tdssibdb.cell[index].sibs.mib_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
    {
      ERR_FATAL("SIB_HEAP:Zero mib_encoded_pdu_length ",0,0,0);
    }
    pdu_length = tdssibdb.cell[index].sibs.mib_encoded_pdu_length;
    /* ASN.1 decode the PDU */
    tdssibdb.cell[index].sibs.mib =  tdsrrcasn1_decode_pdu( 
    (void *)(tdssibdb.cell[index].sibs.mib_encoded_ptr), /* Encoded SIB      */
    tdsrrc_MasterInformationBlock_PDU,                                                    /* SIB's PDU number */
    pdu_length   /* SIB's Length in bytes */
    );
    if(tdssibdb.cell[index].sibs.mib == NULL)
    {
      ERR_FATAL("SIB_HEAP:Asn.1 decode failed from MIB",0,0,0);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:Mib already decoded,mib_ptr=%d",tdssibdb.cell[index].sibs.mib);
  }
  if( tdssibdb.cell[index].sibs.sb1== NULL )
  {
    if( (tdsrrcsib_is_sib_scheduled( tdsrrc_SB1,
              index 
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
             , FALSE
             , NULL
#endif
        )) == TRUE
          )
    {
      if(tdssibdb.cell[index].sibs.sb1_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
      {
        ERR_FATAL("SIB_HEAP:Zero sb1_encoded_pdu_length ",0,0,0);
      }
      /* Get the PDU length in bytes */
      pdu_length = tdssibdb.cell[index].sibs.sb1_encoded_pdu_length;
      /* ASN.1 decode the PDU */
      tdssibdb.cell[index].sibs.sb1=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[index].sibs.sb1_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoTypeSB1_PDU,                                                    /* SIB's PDU number */
      pdu_length   /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[index].sibs.sib1== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SB1",0,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:SB1 not present in this cell");
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SB1 already decoded,sb1_ptr=%d",tdssibdb.cell[index].sibs.sb1);
  }
  if( tdssibdb.cell[index].sibs.sb2== NULL )
  {
    if( (tdsrrcsib_is_sib_scheduled( tdsrrc_SB2,
              index
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
             , FALSE
             , NULL
#endif
        )) == TRUE
          )
    {
      if(tdssibdb.cell[index].sibs.sb2_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
      {
        ERR_FATAL("SIB_HEAP:Zero sb2_encoded_pdu_length ",0,0,0);
      }
      /* Get the PDU length in bytes */
      pdu_length = tdssibdb.cell[index].sibs.sb2_encoded_pdu_length;
      /* ASN.1 decode the PDU */
      tdssibdb.cell[index].sibs.sb2=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[index].sibs.sb2_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoTypeSB2_PDU,                                                    /* SIB's PDU number */
      pdu_length   /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[index].sibs.sib2== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SB2",0,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:SB2 not present in this cell");
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SB2 already decoded,sb1_ptr=%d",tdssibdb.cell[index].sibs.sb2);
  }  
  if( tdssibdb.cell[index].sibs.sib1 == NULL )
  {
    if(tdssibdb.cell[index].sibs.sib1_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
    {
      ERR_FATAL("SIB_HEAP:Zero sib1_encoded_pdu_length ",0,0,0);
    }
    /* Get the PDU length in bytes */
    pdu_length = tdssibdb.cell[index].sibs.sib1_encoded_pdu_length;
    /* ASN.1 decode the PDU */
    tdssibdb.cell[index].sibs.sib1=  tdsrrcasn1_decode_pdu( 
    (void *)(tdssibdb.cell[index].sibs.sib1_encoded_ptr), /* Encoded SIB      */
    tdsrrc_SysInfoType1_PDU,                                                    /* SIB's PDU number */
    pdu_length   /* SIB's Length in bytes */
    );
    if(tdssibdb.cell[index].sibs.sib1== NULL)
    {
      ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB1",0,0,0);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB1 already decoded,sib1_ptr=%d",tdssibdb.cell[index].sibs.sib1);
  }
  if( tdssibdb.cell[index].sibs.sib2 == NULL )
  {
    if( (tdsrrcsib_is_sib_scheduled( tdsrrc_SIB2,
              index
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            , FALSE 
            , NULL
#endif
        )) == TRUE
          )
    {
      if(tdssibdb.cell[index].sibs.sib2_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
      {
        ERR_FATAL("SIB_HEAP:Zero sib2_encoded_pdu_length ",0,0,0);
      }
      /* Get the PDU length in bytes */
      pdu_length = tdssibdb.cell[index].sibs.sib2_encoded_pdu_length;
      /* ASN.1 decode the PDU */
      tdssibdb.cell[index].sibs.sib2=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[index].sibs.sib2_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType2_PDU,                                                       /* SIB's PDU number */
      pdu_length   /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[index].sibs.sib2== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB2",0,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:SIB2 not present in this cell");
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB2 already decoded,sib2_ptr=%d",tdssibdb.cell[index].sibs.sib2);
  }

  if( tdssibdb.cell[index].sibs.sib3== NULL )
  {
    if(tdssibdb.cell[index].sibs.sib3_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
    {
      ERR_FATAL("SIB_HEAP:Zero sib3_encoded_pdu_length ",0,0,0);
    }
    /* Get the PDU length in bytes */
    pdu_length = tdssibdb.cell[index].sibs.sib3_encoded_pdu_length;
    /* ASN.1 decode the PDU */
    tdssibdb.cell[index].sibs.sib3=  tdsrrcasn1_decode_pdu( 
    (void *)(tdssibdb.cell[index].sibs.sib3_encoded_ptr), /* Encoded SIB      */
    tdsrrc_SysInfoType3_PDU,                                                    /* SIB's PDU number */
    pdu_length   /* SIB's Length in bytes */
    );
    if(tdssibdb.cell[index].sibs.sib3== NULL)
    {
      ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB3",0,0,0);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB2 already decoded,sib3_ptr=%d",tdssibdb.cell[index].sibs.sib3);
  }

  if( tdssibdb.cell[index].sibs.sib4== NULL )
  {
    if(tdssibdb.cell[index].sibs.sib3->sib4indicator == TRUE)
    {
      if(tdssibdb.cell[index].sibs.sib4_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
      {
        ERR_FATAL("SIB_HEAP:Zero sib4_encoded_pdu_length ",0,0,0);
      }
      /* Get the PDU length in bytes */
      pdu_length = tdssibdb.cell[index].sibs.sib4_encoded_pdu_length;
      /* ASN.1 decode the PDU */
      tdssibdb.cell[index].sibs.sib4=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[index].sibs.sib4_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType4_PDU,                                                    /* SIB's PDU number */
      pdu_length   /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[index].sibs.sib4== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB4",0,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:SIB4 not present in this cell");
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB2 already decoded,sib4_ptr=%d",tdssibdb.cell[index].sibs.sib4);
  }

  if( tdssibdb.cell[index].sibs.sib5== NULL )
  {
    if(tdssibdb.cell[index].sibs.sib5_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
    {
      ERR_FATAL("SIB_HEAP:Zero sib5_encoded_pdu_length ",0,0,0);
    }
    /* Get the PDU length in bytes */
    pdu_length = tdssibdb.cell[index].sibs.sib5_encoded_pdu_length;
    /* ASN.1 decode the PDU */
    tdssibdb.cell[index].sibs.sib5=  tdsrrcasn1_decode_pdu( 
    (void *)(tdssibdb.cell[index].sibs.sib5_encoded_ptr), /* Encoded SIB      */
    tdsrrc_SysInfoType5bis_PDU,                                                    /* SIB's PDU number */
    pdu_length   /* SIB's Length in bytes */
    );
    if(tdssibdb.cell[index].sibs.sib5== NULL)
    {
      ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB5",0,0,0);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB2 already decoded,sib5_ptr=%d",tdssibdb.cell[index].sibs.sib5);
  }

  if( tdssibdb.cell[index].sibs.sib6== NULL )
  {
    if(tdssibdb.cell[index].sibs.sib5->sib6indicator == TRUE)
    {
      if(tdssibdb.cell[index].sibs.sib6_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
      {
        ERR_FATAL("SIB_HEAP:Zero sib6_encoded_pdu_length ",0,0,0);
      }
      /* Get the PDU length in bytes */
      pdu_length = tdssibdb.cell[index].sibs.sib6_encoded_pdu_length;
      /* ASN.1 decode the PDU */
      tdssibdb.cell[index].sibs.sib6=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[index].sibs.sib6_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType6_PDU,                                                     /* SIB's PDU number */
      pdu_length   /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[index].sibs.sib6== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB6",0,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:SIB6 not present in this cell");
    }
      
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB2 already decoded,sib6_ptr=%d",tdssibdb.cell[index].sibs.sib6);
  }

  if( tdssibdb.cell[index].sibs.sib7== NULL )
  {
    if(tdssibdb.cell[index].sibs.sib7_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
    {
      ERR_FATAL("SIB_HEAP:Zero sib7_encoded_pdu_length ",0,0,0);
    }
    /* Get the PDU length in bytes */
    pdu_length = tdssibdb.cell[index].sibs.sib7_encoded_pdu_length;
    /* ASN.1 decode the PDU */
    tdssibdb.cell[index].sibs.sib7=  tdsrrcasn1_decode_pdu( 
    (void *)(tdssibdb.cell[index].sibs.sib7_encoded_ptr), /* Encoded SIB      */
    tdsrrc_SysInfoType7_PDU,                                                    /* SIB's PDU number */
    pdu_length   /* SIB's Length in bytes */
    );
    if(tdssibdb.cell[index].sibs.sib7== NULL)
    {
      ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB7",0,0,0);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB2 already decoded,sib7_ptr=%d",tdssibdb.cell[index].sibs.sib7);
  }

  if( tdssibdb.cell[index].sibs.sib11== NULL )
  {
    if(tdssibdb.cell[index].sibs.sib11_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
    {
      ERR_FATAL("SIB_HEAP:Zero sib11_encoded_pdu_length ",0,0,0);
    }
    /* Get the PDU length in bytes */
    pdu_length = tdssibdb.cell[index].sibs.sib11_encoded_pdu_length;
    /* ASN.1 decode the PDU */
    tdssibdb.cell[index].sibs.sib11=  tdsrrcasn1_decode_pdu( 
    (void *)(tdssibdb.cell[index].sibs.sib11_encoded_ptr), /* Encoded SIB      */
    tdsrrc_SysInfoType11_PDU,                                                    /* SIB's PDU number */
    pdu_length  /* SIB's Length in bytes */
    );
    if(tdssibdb.cell[index].sibs.sib11== NULL)
    {
      ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB11",0,0,0);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB11 already decoded,sib11_ptr=%d",tdssibdb.cell[index].sibs.sib11);
  }


  if( tdssibdb.cell[index].sibs.sib12== NULL )
  {
    if(tdssibdb.cell[index].sibs.sib11->sib12indicator == TRUE)
    {
      if(tdssibdb.cell[index].sibs.sib12_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
      {
        ERR_FATAL("SIB_HEAP:Zero sib12_encoded_pdu_length ",0,0,0);
      }
      /* Get the PDU length in bytes */
      pdu_length = tdssibdb.cell[index].sibs.sib12_encoded_pdu_length;
      /* ASN.1 decode the PDU */
      tdssibdb.cell[index].sibs.sib12=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[index].sibs.sib12_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType12_PDU,                                                    /* SIB's PDU number */
      pdu_length   /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[index].sibs.sib12== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB12",0,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:SIB12 not present in this cell");
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB12 already decoded,sib12_ptr=%d",tdssibdb.cell[index].sibs.sib12);
  }
  
  if( tdssibdb.cell[index].sibs.sib18== NULL )
  {
    if( (tdsrrcsib_is_sib_scheduled( tdsrrc_SIB18,
              index 
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            , FALSE
            , NULL
#endif
        )) == TRUE
          )
    {
      if(tdssibdb.cell[index].sibs.sib18_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
      {
        ERR_FATAL("SIB_HEAP:Zero sib18_encoded_pdu_length ",0,0,0);
      }
      /* Get the PDU length in bytes */
      pdu_length = tdssibdb.cell[index].sibs.sib18_encoded_pdu_length;
      /* ASN.1 decode the PDU */
      tdssibdb.cell[index].sibs.sib18=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[index].sibs.sib18_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType18_PDU,                                                    /* SIB's PDU number */
      pdu_length  /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[index].sibs.sib18== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB18",0,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:SIB18 not present in this cell");
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB18 already decoded,sib18_ptr=%d",tdssibdb.cell[index].sibs.sib18);
  }
  
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  if( tdssibdb.cell[index].sibs.sib11bis== NULL )
  {
    if( (tdsrrcsib_is_sib_scheduled( tdsrrc_SIB11bis,
              index 
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            , FALSE
            , NULL
#endif
        )) == TRUE
          )
    {
      if(tdssibdb.cell[index].sibs.sib11bis_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
      {
        ERR_FATAL("SIB_HEAP:Zero sib11bis_encoded_pdu_length ",0,0,0);
      }
      /* Get the PDU length in bytes */
      pdu_length = tdssibdb.cell[index].sibs.sib11bis_encoded_pdu_length;
      /* ASN.1 decode the PDU */
      tdssibdb.cell[index].sibs.sib11bis=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[index].sibs.sib11bis_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType11bis_PDU,                                                    /* SIB's PDU number */
      pdu_length   /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[index].sibs.sib11bis== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib11bis",0,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:SIB11bis not present in this cell");
    }

  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:sib11bis already decoded,sib11bis_ptr=%d",tdssibdb.cell[index].sibs.sib11bis);
  }
#endif

#ifdef FEATURE_TDSCDMA_TO_LTE
  if( tdssibdb.cell[index].sibs.sib19== NULL )
  {
    if( (tdsrrcsib_is_sib_scheduled( tdsrrc_SIB19,
              index 
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
                 , FALSE
                 , NULL
#endif
              )) == TRUE
          )
    {
      if(tdssibdb.cell[index].sibs.sib19_encoded_pdu_length == TDSINVALID_PDU_LENGTH)
      {
        ERR_FATAL("SIB_HEAP:Zero sib19_encoded_pdu_length ",0,0,0);
      }
      /* Get the PDU length in bytes */
      pdu_length = tdssibdb.cell[index].sibs.sib19_encoded_pdu_length;
      /* ASN.1 decode the PDU */
      tdssibdb.cell[index].sibs.sib19=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[index].sibs.sib19_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType19_PDU,                                                    /* SIB's PDU number */
      pdu_length   /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[index].sibs.sib19== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from sib19",0,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:SIB19 not present in this cell");
    }

  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:sib19 already decoded,sib19_ptr=%d",tdssibdb.cell[index].sibs.sib19);
  }
#endif

} /* end function tdsrrcsibdb_decode_all_sibs_for_a_cell */
/*===========================================================================

FUNCTION rrc_sibdb_free_all_encodedsibs_for_a_cell

DESCRIPTION

  This function will free all ASN.1 decode sibs for a cell. 
  
DEPENDENCIES

 None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_free_all_decodedsibs_for_a_cell
(
  uint32 index /* Index of the cell header to be initialized */
)
{
  if(index >= TDSSIBDB_INVALID_INDEX)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:Bad sibbd idx=%d",index );
    return;
  }
  /* Note that even though we don't support all these SIBs now,
  the code has been added here since additional SIBs will be supported
  in future releases */
  if( tdssibdb.cell[index].sibs.mib != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_MasterInformationBlock_PDU,
                       (void *)tdssibdb.cell[index].sibs.mib);
    tdssibdb.cell[index].sibs.mib = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib1 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType1_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib1);
    tdssibdb.cell[index].sibs.sib1 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib2 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType2_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib2);
    tdssibdb.cell[index].sibs.sib2 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib3 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType3_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib3);
    tdssibdb.cell[index].sibs.sib3 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib4 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType4_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib4);
    tdssibdb.cell[index].sibs.sib4 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib5 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType5bis_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib5);
    tdssibdb.cell[index].sibs.sib5 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib6 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType6_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib6);
    tdssibdb.cell[index].sibs.sib6 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib7 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType7_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib7);
    tdssibdb.cell[index].sibs.sib7 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib11 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType11_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib11);
    tdssibdb.cell[index].sibs.sib11 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib12 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType12_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib12);
    tdssibdb.cell[index].sibs.sib12 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sib18 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType18_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib18);
    tdssibdb.cell[index].sibs.sib18 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sb1 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoTypeSB1_PDU,
                       (void *)tdssibdb.cell[index].sibs.sb1);
    tdssibdb.cell[index].sibs.sb1 = NULL;
  }
  if( tdssibdb.cell[index].sibs.sb2 != NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoTypeSB2_PDU,
                       (void *)tdssibdb.cell[index].sibs.sb2);
    tdssibdb.cell[index].sibs.sb2 = NULL;
  }
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  if( tdssibdb.cell[index].sibs.sib11bis!= NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType11bis_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib11bis);
    tdssibdb.cell[index].sibs.sib11bis= NULL;
  }
#endif
#ifdef FEATURE_TDSCDMA_SIB19
  if( tdssibdb.cell[index].sibs.sib19!= NULL )
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoType19_PDU,
                       (void *)tdssibdb.cell[index].sibs.sib19);
    tdssibdb.cell[index].sibs.sib19= NULL;
  }
#endif

}

/*===========================================================================

FUNCTION tdsrrc_sibdb_clear_cell_header

DESCRIPTION

  Clear all cell header in tdssibdb
  
DEPENDENCIES

 None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  This will make SIB to reread all sib's.

===========================================================================*/
void tdsrrc_sibdb_clear_cell_header
(
  void
)
{
  uint32 count,index; /* Counter */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"clear all cell header in tdssibdb");

	{
	  for(index = 0; index < TDSSIBDB_MAX_CELLS; index++)
	  {
	    for( count = 0; count < TDSRRC_TOT_SIBS; count++ )
	    {
	      /* Initialize each SIB's value tag to 0 which is not a valid
	      value tag. This is to indicate that no value tag is stored
	      in this index yet. */
	      tdssibdb.cell[index].sib_value_tag[count] = 0; 
	    }
	    tdssibdb.cell[index].six_hr_count  = 0;
	  }
  }
}
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING

/*===========================================================================

FUNCTION tdsrrcsibdb_return_intra_freq_meas_quan_for_deferred_sib

DESCRIPTION

  This function returns the Intra Freq Meas Quantity to be used during measurement reporting in RACH
  for Deferred SIB 11/11bis/12reading. This IE is fetched from SIB3.

DEPENDENCIES
none

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrrcsibdb_return_intra_freq_meas_quan_for_deferred_sib
(
  tdsl1_intra_freq_meas_quan_struct_type *intra_freq_meas_quan
)
{
  uint8 i;
  tdsrrc_SysInfoType3 *sib3_ptr = NULL;         /* Local pointer to SIB3       */

  sib3_ptr = tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB3);
    
  if(sib3_ptr!=NULL)
  {
    if(INTRA_FREQ_MEAS_QUAN_FOR_DEFERRED_SIB(sib3_ptr))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB:Fill Meas Quanity from SIB3 DMCR intraFreqMeasQuantity_TDDList.");
	  tdsrrcmeas_set_intra_meas_quan_info(intra_freq_meas_quan, 
	  	  ( tdsrrc_IntraFreqMeasQuantity_TDDList *)&sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v5c0NoncriticalExtension.v670NonCriticalExtension.
	  	  v770NonCriticalExtension.sysInfoType3_v770ext.deferredMeasurementControlReadingSupport.modeSpecificInfo.u.tdd->intraFreqMeasQuantity_TDDList);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Deferred SIB:Meas Quanity not indicated in SIB3. Set TDSL1_MEAS_NONE");
	  intra_freq_meas_quan->meas_quan_list_no = 0;
      for (i = 0; i < L1_MAX_MEAS_LIST_NO; i++ )
      {
        intra_freq_meas_quan->meas_quan[i]= TDSL1_MEAS_NONE;
      }
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Deferred SIB:Asn.1 decode failed from SIB3");
  }
}

/*===========================================================================

FUNCTION tdsrrcsibdb_copy_deferred_sibdb_to_tsibdb

DESCRIPTION

  This function copies SIB segments from Deferred SIB optimization DB to TSIB.

DEPENDENCIES
none

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void  tdsrrcsibdb_copy_deferred_sibdb_to_tsibdb
(
  uint32 dbindex,     /* SIB Database Index for the cell for which the
                      new Value Tags have been received */
  tdssib_events_index_e_type index,
  void *block_ptr,    /* Pointer to the MIB, SB1 or SB2, typecast
                      to void */
  tdsrrc_SIB_type sib   /* Indicate which SIB the value-tags are from
                      This can be the MIB or SB1 or SB2. */
)
{

  tdsrrc_MasterInformationBlock *mib_ptr; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr;         /* Local pointer to SB2       */

  tdsrrc_SIBSb_ReferenceList *sb_ptr = NULL; /* Local pointer to SIBSb ref list */
  tdsrrc_SIB_ReferenceList *ptr = NULL;      /* Local pointer to SIB ref list   */
  uint32 idx;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List *ext_siblist_ptr = NULL;
#endif
#ifdef FEATURE_TDSCDMA_SIB19
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List2 *ext_siblist2_ptr = NULL;
#endif

  uint8 i;                   /* Local counter                   */
  tdsrrcsib_sib_type_vtag sib_type_tag;
  memset((void *)&sib_type_tag, 0, sizeof(tdsrrcsib_sib_type_vtag));

  if((tdsdeferred_sib_info.cell_id != tdssibdb.cell[dbindex].cell_id) ||
      !(tdsrrc_bcch_plmns_match(tdsdeferred_sib_info.plmn_id, tdssibdb.cell[dbindex].plmn_id))
     )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB:Cell id and PLMN id dont match");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: Copying Deferred SIB DB to TSIB");
    if( sib == tdsrrc_MIB )
    {
      /* Typecast to MIB pointer */
      mib_ptr = (tdsrrc_MasterInformationBlock *)block_ptr;
      sb_ptr = &(mib_ptr->sibSb_ReferenceList);

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if(TDSEXTENSION_SIB_SCHED_PRESENT_IN_MIB(mib_ptr))
      {
        ext_siblist_ptr = &(mib_ptr->v690NonCriticalExtensions.
          v6b0NonCriticalExtensions.masterInformationBlock_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
      }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
      if((TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_MIB(mib_ptr)))
      {
        ext_siblist2_ptr = &(mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.v860NonCriticalExtensions.
          masterInformationBlock_v860ext.extSIBTypeInfoSchedulingInfo_List);
      }
#endif

      idx = 0;
      while( sb_ptr->n > idx )
      {
          /* Switch on the value tag's sib type and look
          at the corrosponding SIBs stored value tag for
          a match */
        tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag(&sb_ptr->elem[idx].sibSb_Type,
                                                    &sib_type_tag);
        if(((sib_type_tag.sib_type == tdsrrc_SIB11) &&
            (sib_type_tag.sib_tag == tdsdeferred_sib_info.val_tag.sib11_val_tag))
            ||
            ((sib_type_tag.sib_type == tdsrrc_SIB12) &&
            (sib_type_tag.sib_tag == tdsdeferred_sib_info.val_tag.sib12_val_tag))
            || ((sib_type_tag.sib_type == tdsrrc_SIB18) &&
                 (sib_type_tag.sib_tag == tdsdeferred_sib_info.val_tag.sib18_val_tag))
          )
        {
          if((sib_type_tag.sib_type == tdsrrc_SIB11)&&
             (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].seg_index < 
               tdsdeferred_sib_info.dsib.sib11.seg_index)
             )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: Copying %d segments of SIB11 from DSIB",tdsdeferred_sib_info.dsib.sib11.seg_index);
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].seg_index = 
              tdsdeferred_sib_info.dsib.sib11.seg_index;
  
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_tot_length = 
              tdsdeferred_sib_info.dsib.sib11.sib_tot_length;

            b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_data_ptr,  /* Source Pointer */
                0,    /* Source Offset is always 0 since we copy all the received bits */
               tdsdeferred_sib_info.dsib.sib11.sib_data_ptr, /* Destination Pointer */
               tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_tot_length,  /* Destination Offset */
               TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
              );
          }
          else if((sib_type_tag.sib_type == tdsrrc_SIB12)&&
                  (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].seg_index < 
                    tdsdeferred_sib_info.dsib.sib12.seg_index)
                 )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: Copying %d segments of SIB12 from DSIB",tdsdeferred_sib_info.dsib.sib12.seg_index);
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].seg_index = 
              tdsdeferred_sib_info.dsib.sib12.seg_index;
  
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_tot_length = 
              tdsdeferred_sib_info.dsib.sib12.sib_tot_length;
  
            b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_data_ptr,  /* Source Pointer */
                0,    /* Source Offset is always 0 since we copy all the received bits */
               tdsdeferred_sib_info.dsib.sib12.sib_data_ptr, /* Destination Pointer */
               tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_tot_length,  /* Destination Offset */
               TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
              );
          }
          else if((sib_type_tag.sib_type == tdsrrc_SIB18)&&
                  (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].seg_index < 
                   tdsdeferred_sib_info.dsib.sib18.seg_index)
             )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: Copying %d segments of SIB18 from DSIB",tdsdeferred_sib_info.dsib.sib18.seg_index);
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].seg_index = 
            tdsdeferred_sib_info.dsib.sib18.seg_index;
          
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_tot_length = 
            tdsdeferred_sib_info.dsib.sib18.sib_tot_length;
          
            b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_data_ptr,  /* Source Pointer */
              0,/* Source Offset is always 0 since we copy all the received bits */
              tdsdeferred_sib_info.dsib.sib18.sib_data_ptr, /* Destination Pointer */
              tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_tot_length,  /* Destination Offset */
              TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
            );
          }

          for( i = 0 ;i<TDSMAX_DEFERRED_SIB_SEGMENTS_TO_HOLD;i ++)
          {
            if(tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_type == sib_type_tag.sib_type)
            {
              (void) tdsrrcsib_store_sib_seg_in_tsib_optim_db(tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_type,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].sfn_prime,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_index,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_seg_type,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_length,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_ptr,
                                                   index);
  
            }
          }
          (void) tdsrrcsib_process_sib_seg_in_tsib_optim_db(sib_type_tag.sib_type,index);
        }
        idx++;
      }
    }
    else if( sib == tdsrrc_SB1 || sib == tdsrrc_SB2 )
    {
      if(sib == tdsrrc_SB1)
      {
        /* Value tag is from Scheduling Block 1 
        Copy the pointer and compare the received
        value tags. */
        sb1_ptr = (tdsrrc_SysInfoTypeSB1 *)block_ptr;
        ptr = &(sb1_ptr->sib_ReferenceList);
  #ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if((TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(
              sb1_ptr,tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1)))
        {
          ext_siblist_ptr = &(sb1_ptr->v6b0NonCriticalExtensions.
            sysInfoTypeSB1_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
        }
  #ifdef FEATURE_TDSCDMA_SIB19
       if((TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(sb1_ptr,tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1)))
       {
         ext_siblist2_ptr = &(sb1_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.sysInfoTypeSB1_v860ext.extSIBTypeInfoSchedulingInfo_List);
       }
  #endif
  #endif
      }

      else if( sib == tdsrrc_SB2 ) 
      {
        /* Value tag is from Scheduling Block 2 
        Copy the pointer and compare the received
        value tags. */
        sb2_ptr = (tdsrrc_SysInfoTypeSB2 *)block_ptr;
        ptr = &(sb2_ptr->sib_ReferenceList);
  #ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if((TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(
              sb2_ptr,tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2)))
        {
          ext_siblist_ptr = &(sb2_ptr->v6b0NonCriticalExtensions.
            sysInfoTypeSB2_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
        }
  #ifdef FEATURE_TDSCDMA_SIB19
       if((TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(sb2_ptr,tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2)))
      {
        ext_siblist2_ptr = &(sb2_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.
        sysInfoTypeSB2_v860ext.extSIBTypeInfoSchedulingInfo_List);
      }
  #endif
  #endif
      }
      idx = 0;
      while( ptr->n > idx)
      {
          /* Switch on the value tag's sib type and look
          at the corrosponding SIBs stored value tag for
          a match */
        tdsrrcsibdb_get_sib_type_from_sib_type_and_tag(&ptr->elem[idx].sib_Type,
                                                    &sib_type_tag);
        if(((sib_type_tag.sib_type == tdsrrc_SIB11) &&
            (sib_type_tag.sib_tag == tdsdeferred_sib_info.val_tag.sib11_val_tag))
            ||
            ((sib_type_tag.sib_type == tdsrrc_SIB12) &&
            (sib_type_tag.sib_tag == tdsdeferred_sib_info.val_tag.sib12_val_tag))
            || ((sib_type_tag.sib_type == tdsrrc_SIB18) &&
                 (sib_type_tag.sib_tag == tdsdeferred_sib_info.val_tag.sib18_val_tag))
          )
        {
        
        if((sib_type_tag.sib_type == tdsrrc_SIB11)&&
           (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].seg_index < 
             tdsdeferred_sib_info.dsib.sib11.seg_index)
           )
        {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: Copying %d segments of SIB11 from DSIB",tdsdeferred_sib_info.dsib.sib11.seg_index);

            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].seg_index = 
              tdsdeferred_sib_info.dsib.sib11.seg_index;
          
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_tot_length = 
              tdsdeferred_sib_info.dsib.sib11.sib_tot_length;

            b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_data_ptr,  /* Source Pointer */
                0,    /* Source Offset is always 0 since we copy all the received bits */
               tdsdeferred_sib_info.dsib.sib11.sib_data_ptr, /* Destination Pointer */
               tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_tot_length,  /* Destination Offset */
               TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
              );
          }
          else if((sib_type_tag.sib_type == tdsrrc_SIB12)&&
                  (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].seg_index < 
                    tdsdeferred_sib_info.dsib.sib12.seg_index)
                 )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: Copying %d segments of SIB12 from DSIB",tdsdeferred_sib_info.dsib.sib12.seg_index);
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].seg_index = 
              tdsdeferred_sib_info.dsib.sib12.seg_index;
          
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_tot_length = 
              tdsdeferred_sib_info.dsib.sib12.sib_tot_length;
          
            b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_data_ptr,  /* Source Pointer */
                0,    /* Source Offset is always 0 since we copy all the received bits */
               tdsdeferred_sib_info.dsib.sib12.sib_data_ptr, /* Destination Pointer */
               tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_tot_length,  /* Destination Offset */
               TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
              );
          }
          else if((sib_type_tag.sib_type == tdsrrc_SIB18)&&
                  (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].seg_index < 
                    tdsdeferred_sib_info.dsib.sib18.seg_index)
                 )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: Copying %d segments of SIB18 from DSIB",tdsdeferred_sib_info.dsib.sib18.seg_index);
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].seg_index = 
              tdsdeferred_sib_info.dsib.sib18.seg_index;
          
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_tot_length = 
              tdsdeferred_sib_info.dsib.sib18.sib_tot_length;
          
            b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_data_ptr,  /* Source Pointer */
                0,    /* Source Offset is always 0 since we copy all the received bits */
               tdsdeferred_sib_info.dsib.sib18.sib_data_ptr, /* Destination Pointer */
               tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_tot_length,  /* Destination Offset */
               TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
              );
          }

          for( i = 0 ;i<TDSMAX_DEFERRED_SIB_SEGMENTS_TO_HOLD;i ++)
          {
            if(tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_type == sib_type_tag.sib_type)
            {
              (void) tdsrrcsib_store_sib_seg_in_tsib_optim_db(tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_type,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].sfn_prime,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_index,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_seg_type,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_length,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_ptr,
                                                   index);
            }
          }
          (void) tdsrrcsib_process_sib_seg_in_tsib_optim_db(sib_type_tag.sib_type,index);
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Deferred SIB: Deferred SIB val tag %d,SIB val tag %d",tdsdeferred_sib_info.val_tag.sib11_val_tag ,sib_type_tag.sib_tag);
        }
        idx++;
      }
    }

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    idx = 0;
    while((ext_siblist_ptr != NULL) && (ext_siblist_ptr->n > idx))
    {
      if(ext_siblist_ptr->elem[idx].extensionSIB_Type.t == T_rrc_SIB_TypeExt_systemInfoType11bis)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: Copying %d segments of SIB11bis from DSIB",tdsdeferred_sib_info.dsib.sib11bis.seg_index);

        if(ext_siblist_ptr->elem[idx].valueTagInfo.u.cellValueTag == 
            tdsdeferred_sib_info.val_tag.sib11bis_val_tag)
        {
          if(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].seg_index < 
               tdsdeferred_sib_info.dsib.sib11bis.seg_index)
          {
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].seg_index = 
              tdsdeferred_sib_info.dsib.sib11bis.seg_index;
          
            tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_tot_length = 
              tdsdeferred_sib_info.dsib.sib11bis.sib_tot_length;

            b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_data_ptr,  /* Source Pointer */
                0,    /* Source Offset is always 0 since we copy all the received bits */
               tdsdeferred_sib_info.dsib.sib11bis.sib_data_ptr, /* Destination Pointer */
               tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_tot_length,  /* Destination Offset */
               TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
              );
          }
          for( i = 0 ;i<TDSMAX_DEFERRED_SIB_SEGMENTS_TO_HOLD;i ++)
          {
            if(tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_type == sib_type_tag.sib_type)
            {
              (void) tdsrrcsib_store_sib_seg_in_tsib_optim_db(tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_type,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].sfn_prime,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_index,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_seg_type,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_length,
                                                   tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_ptr,
                                                   index);
          
            }
          }
          (void) tdsrrcsib_process_sib_seg_in_tsib_optim_db(sib_type_tag.sib_type,index);
        }
      }
    idx++;
    }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
  idx = 0;
  while((ext_siblist2_ptr != NULL) && (ext_siblist2_ptr->n > idx))
  {
    if(ext_siblist2_ptr->elem[idx].extensionSIB_Type2.t == T_tdsrrc_SIB_TypeExt2_systemInfoType19)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB: Copying %d segments of SIB19 from DSIB",tdsdeferred_sib_info.dsib.sib19.seg_index);
  
      if(ext_siblist2_ptr->elem[idx].valueTagInfo.u.cellValueTag == 
         tdsdeferred_sib_info.val_tag.sib19_val_tag)
      {
        if(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].seg_index < 
           tdsdeferred_sib_info.dsib.sib19.seg_index)
        {
          tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].seg_index = 
            tdsdeferred_sib_info.dsib.sib19.seg_index;
         
          tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_tot_length = 
            tdsdeferred_sib_info.dsib.sib19.sib_tot_length;
  
          b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_data_ptr,  /* Source Pointer */
             0,	/* Source Offset is always 0 since we copy all the received bits */
             tdsdeferred_sib_info.dsib.sib19.sib_data_ptr, /* Destination Pointer */
             tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_tot_length,  /* Destination Offset */
             TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
            );
        }
        for( i = 0 ;i<TDSMAX_DEFERRED_SIB_SEGMENTS_TO_HOLD;i ++)
        {
          if(tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_type == sib_type_tag.sib_type)
          {
           (void) tdsrrcsib_store_sib_seg_in_tsib_optim_db(tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_type,
              tdsdeferred_sib_info.deferred_sib_seg_db[i].sfn_prime,
              tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_index,
              tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_seg_type,
              tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_length,
              tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_ptr,
              index);
          }
        }
       (void) tdsrrcsib_process_sib_seg_in_tsib_optim_db(sib_type_tag.sib_type,index);
      }
    }
    idx++;
    }
#endif

  }
return;
}
#endif

#ifdef FEATURE_TDSCDMA_SIB19
/*===========================================================================

FUNCTION tdsrrcsibdb_serv_ota_sib19_available

DESCRIPTION

  This function determines if serving cell OTA SIB19 has been received.
  This is to vs pseudo SIB19.
 
DEPENDENCIES


RETURN VALUE

 Boolean

SIDE EFFECTS

  None.

===========================================================================*/

tdsrrc_SysInfoType19 * tdsrrcsibdb_get_serv_ota_sib19()
{
   tdsrrc_SysInfoType19 * ret_ptr = NULL;

   /* First check if we have an active cell selected */
   if(!(tdssibdb.active_cell_sibs_valid))
   {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcsibdb_get_serv_ota_sib19: No active cell selected yet!");
      return ret_ptr;
   }
   
   /* Then use the generic function to return sibs for a cell */
   ret_ptr = tdsrrcsibdb_return_sib_for_db_index( tdsrrc_SIB19, /* requested sib */
                                                  tdssibdb.active_cell_index /* For the active cell */
                                                );

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrcsibdb_get_serv_ota_sib19: OTA SIB19 available 0x%x", ret_ptr);

   return ret_ptr;
}
#endif /* FEATURE_TDSCDMA_SIB19 */

