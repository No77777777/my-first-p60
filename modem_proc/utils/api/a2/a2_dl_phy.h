#ifndef LTE_A2_DL_PHY_H
#define LTE_A2_DL_PHY_H

/*!
  @file a2_dl_phy.h

  @brief
   The interface to the downlink phy portion of the a2 driver for LTE.

  @ingroup a2_ext_interface
*/

/*===========================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/utils.mpss/5.4/api/a2/a2_dl_phy.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/20/20   av      CR 2606409: Data Path Optimization for HA 
08/01/17   am      CR1089189 L plus L changes
02/22/16   am      CR979909 support for 18 bit earfcn
08/20/14   ca      increamented the value of A2_DL_PHY_MAX_TR_BLK from 24 to 30
01/13/14   ca      DSDA feature is implemented.
11/08/13   mm      Increased A2_DL_PHY_MAX_TR_BLK for LTE cat6
10/09/13   ar      move internal api prototypes to src folder header files
07/02/12   ar      CR 375415 fix: set the dl phy interrupt back to immediate and 
                   stop the dl phy timer after tech is set to DEFAULT
10/10/11   st      Made eMBMS related changes
08/01/11   ar      Modified 64 bit FW-A2 driver interface to accomodate PMCH
                   parameters
06/07/11   ar      added a2_dl_phy_shut_down() and a2_dl_phy_hw_init() apis
03/28/11   bn      Added API to support IMMEDIATE or PERIODIC DL PHY timer INTR
11/02/09   ar      added a2_dl_phy_set_technology()
10/02/09   ar      Added LTE->HSPA hw bug workaround
07/15/09   sah     Remove a2 dl phy dsm dependancy.  Add definition of sfn
                   structure.
05/14/09   sah     Add in some new espresso initialization, featurization,
                   and split init.
04/18/09   sm      Added a2 specific typedefs to remove dependency on lte file
                   like lte_l1_types.h
03/09/09   sah     Add immediate interrupt bit to overlay structure.
01/05/09   yg      In structure a2_dl_phy_tb_info_t, changed data type of
                   rnti_type to uint8 from enum lte_rnti_type_e.
                   Size of enum is 4 bytes while we need to store it in 1 byte.
08/08/08   ahe     Added lte_l1_types.h
07/11/08   sah     Initial Checkin


===========================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include <a2_common.h>
#include <comdef.h>

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*===========================================================================

                             MACROS

===========================================================================*/

/*!
  Description Maximum number of transport blocks that can be processed by
  A2 driver in a given moment. In one shot, A2 driver will read only up to
  MAX_A2_DL_PHY_TR_BLK transport blocks.

  for 1 carrier, 2 TBs per msec.
  for 3 carriers, 6 TBs per msec.
  Budgeting for 5 msec delay in MAC DL processing (6 tb * 5 msec = 30)
  
  */
#define A2_DL_PHY_MAX_TR_BLK 30

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA chanegs,
  this macro can take an extra argument as sub_id.
  */
#define a2_dl_phy_set_dec_interrupt_type(imm_interrupt) \
a2_dl_phyi_set_dec_interrupt_type(A2_SUB_ID_DEFAULT, imm_interrupt)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA chanegs,
  this macro can take an extra argument as sub_id.
  */
#define a2_dl_phy_get_data(tb_arr) \
            a2_dl_phyi_get_data(A2_SUB_ID_DEFAULT, tb_arr)


/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA chanegs,
  this macro can take an extra argument as sub_id.
  */
#define a2_dl_phy_get_data_lted(tb_arr) \
            a2_dl_phyi_get_data_lted(A2_SUB_ID_DEFAULT, tb_arr)

/* WRAPPER Macros for L2 to access Meta Info*/
/* Word1 */
#define A2_DL_PHY_TB_META_CELL_ID(tb_ptr)     (((a2_dl_phy_tb_info_t*)(tb_ptr))->word1.info.cell_id)

#define A2_DL_PHY_TB_META_SCELL_ID(tb_ptr)    (((a2_dl_phy_tb_info_t*)(tb_ptr))->word1.info.scell_id)

#define A2_DL_PHY_TB_META_PMCH_ID(tb_ptr)     (((a2_dl_phy_tb_info_t*)(tb_ptr))->word1.pmch_info.pmch_id)

#define A2_DL_PHY_TB_META_SFN(tb_ptr)         (((a2_dl_phy_tb_info_t*)(tb_ptr))->word1.info.sfn)

#define A2_DL_PHY_TB_META_SYS_FN(tb_ptr)      (((a2_dl_phy_tb_info_t*)(tb_ptr))->word1.info.sfn.sys_fn)

#define A2_DL_PHY_TB_META_SUB_FN(tb_ptr)      (((a2_dl_phy_tb_info_t*)(tb_ptr))->word1.info.sfn.sub_fn)

#define A2_DL_PHY_TB_META_DISCARD_TB(tb_ptr)  (((a2_dl_phy_tb_info_t*)(tb_ptr))->word1.info.sfn.discard_tb)

#define A2_DL_PHY_TB_META_MBSFN_IND(tb_ptr)   (((a2_dl_phy_tb_info_t*)(tb_ptr))->word1.info.sfn.mbsfn_ind)

/* Word2 */
#define A2_DL_PHY_TB_META_HARQ_ID_OR_AREA_ID(tb_ptr) (((a2_dl_phy_tb_info_t*)(tb_ptr))->word2.info.harq_id_or_embms_area_id)

#define A2_DL_PHY_TB_META_RNTI(tb_ptr)        (((a2_dl_phy_tb_info_t*)(tb_ptr))->word2.info.rnti_type)

#define A2_DL_PHY_TB_META_FREQ(tb_ptr)        (((a2_dl_phy_tb_info_t*)(tb_ptr))->word2.info.earfcn_freq)

#define A2_DL_PHY_TB_VARIANT_ID(tb_ptr)        (((a2_dl_phy_tb_info_t*)(tb_ptr))->word2.info.as_id)

/*===========================================================================

                       STRUCTURES AND UNIONS

===========================================================================*/

struct dsm_item_s;

typedef uint16 a2_sfn_t;

/*! @brief
  The system/sub-frame number type
*/
typedef struct
{
  uint16 sub_fn:4;
  uint16 sys_fn:10;
  uint16 discard_tb :1; /*!< bit indicates if tb needs to be discarded */
  uint16 mbsfn_ind  :1; /*!< bit indicates if its an mbsfn indication */
} a2_sfn_s;

/*!
  Same as lte_phy_cell_id_t
  */
typedef uint16 a2_phy_cell_id_t;


/*!
  call back type that MAC regsters to get a notification after the data is 
  recieved in DL PHY. */

typedef void (*a2_dl_phy_tb_available_cb)(void);

/*!
   @brief
    Structure holds info passed from FW to A2 for tbs received on PDSCH
*/
typedef struct
{
  uint16 cell_id    :9; /*!< The cell Id of the cell on which
                               the transport block was received */
  uint16 scell_id   :3; /*!< The secondary cell Id of the cell on which
                               the transport block was received */
  uint16 reserved   :4; /*!< Reserved bits */

  a2_sfn_s sfn;          /*!< The system and subframe number
                           transport block was received */
} a2_dl_phy_tb_info_word1_s;

/*!
   @brief
    Structure holds info passed from FW to A2 for tbs received on PMCH
*/
typedef struct
{
  uint16 cell_id    :9; /*!< The cell Id of the cell on which
                             the transport block was received */
  uint16 scell_id   :3; /*!< The secondary cell Id of the cell on which
                             the transport block was received */
  uint16 pmch_id    :4; /*!< 4bits representing pmch_id for this tb */
  
  a2_sfn_s sfn;          /*!< The system and subframe number
                             transport block was received */
} a2_dl_phy_pmch_tb_info_word1_s;

/*!
   @brief
    Union holds word1 (32bit) tb related info passed from FW to A2
*/
typedef union
{
  a2_dl_phy_tb_info_word1_s       info;      /*!< PDSCH tb info */
  a2_dl_phy_pmch_tb_info_word1_s  pmch_info; /*!< PMCH tb info */
  uint32                          value;     /*!< 32bit union value */
} a2_dl_phy_tb_info_word1_u;

/*!
   @brief
    Structure holds info passed from FW to A2 for tbs received on PDSCH
*/
typedef struct
{
  uint32 harq_id_or_embms_area_id  :8; /*!< PDSCH harq_id on which tb was received*/
  uint32 rnti_type                 :4; /*!< The RNTI-type of the PDSCH data received */
  uint32 earfcn_freq               :18; /*!< The frequency of the cell on which the */
  /*!< Used as SUB ID, values: 0/1 variable name can be changed to sub_id*/
  uint32 as_id                :1; 
  uint32 immediate_irq             :1;
} a2_dl_phy_tb_info_word2_s;

/*!
   @brief
    Union holds word2 (32bit) tb related info passed from FW to A2
*/
typedef union
{
  a2_dl_phy_tb_info_word2_s      info;      /*!< PDSCH tb info */
  uint32                         value;     /*!< 32bit union value */
} a2_dl_phy_tb_info_word2_u;

/*!
  @brief
  The meta-information passed with the transport block.

  @note  This may be a common structure and not defined by the a2.
  */
typedef struct
{
  a2_dl_phy_tb_info_word1_u word1;
  a2_dl_phy_tb_info_word2_u word2;
} a2_dl_phy_tb_info_t;

/*!
  The downlink transport block information
*/
typedef struct
{
  struct dsm_item_s *tb_dsm_ptr;  /*!< Pointer to the dsm chain containing the
                                       transport block.

                             @todo find the actual type for that */
  uint16       tb_len;      /*!< The length in bytes of the transport block */
  a2_dl_phy_tb_info_t meta_info; /*!< The meta-information contained in the
                                   transport block. */
} a2_dl_phy_transport_block_t;


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

  FUNCTION:  a2_dl_phyi_get_data

===========================================================================*/
/*!
  @brief
  Used to get already received buffered downlink data sitting in the downlink
  phy status queue.

  This function will pack the already recieved data and meta information
  into the provided tb_array.

  The expected caller of this funciton is the MAC DL task.

  @return
    The number of transport blocks received
*/
/*=========================================================================*/
uint8 a2_dl_phyi_get_data
(
  /*! system subscriber id */
  a2_sub_id_t                 sub_id,
 
 /*! out: The array to be filled with pointer to all of the buffered downlink
  transport blocks up to MAX_A2_DL_PHY_TR_BLK.  This array also includes
  the meta-information associated with the transport blocks.  This
  array must contain MAX_A2_DL_PHY_TR_BLK entries */
  a2_dl_phy_transport_block_t tb_array[A2_DL_PHY_MAX_TR_BLK]
);

/*===========================================================================

  FUNCTION:  a2_dl_phyi_get_data_lted

===========================================================================*/
/*!
  @brief
  Used to get already received buffered downlink data sitting in the downlink
  phy status queue for LTE-D optimized packing of TB

  This function will pack the already recieved data and meta information
  into the provided tb_array.

  The expected caller of this funciton is the MAC DL task.

  @return
    The number of transport blocks received
*/
/*=========================================================================*/

uint8 a2_dl_phyi_get_data_lted
(
 /*! out: The array to be filled with pointer to all of the buffered downlink
  transport blocks up to MAX_A2_DL_PHY_TR_BLK.  This array also includes
  the meta-information associated with the transport blocks.  This
  array must contain MAX_A2_DL_PHY_TR_BLK entries */
  a2_sub_id_t sub_id,

  a2_dl_phy_transport_block_t tb_array[A2_DL_PHY_MAX_TR_BLK]
);
/*===========================================================================

  FUNCTION:  a2_dl_phy_register_tb_cb

===========================================================================*/
/*!
  @brief
  Registers a callback which is called upon transport block reception.

  This callback can be used to send a signal to MAC.  This was formerly
  performed via the A2_DL_PHY_DATA_IND signal.

  After receiving this signal data should be available to fetch via the
  a2_dl_phyi_get_data() function.

*/
/*=========================================================================*/
void a2_dl_phy_register_tb_cb
(
  /*! The callback to be called whenever a transport block is available. */
  a2_dl_phy_tb_available_cb cb
);

/*==============================================================================

  FUNCTION:  a2_dl_phyi_set_dec_interrupt_type

==============================================================================*/
/*!
  @brief
  Set DL PHY data interrupt type: IMMEDIATE or PERIODIC and Data available

  @detail
  Note that this function is used by LTE MAC Control Task when LTE is active and
  a2 task when LTE is getting deactivated. 
  Mutex protection is needed to protect the common data which is used by 
  a2_dl_phy_timer_setup_config_init()

*/
/*============================================================================*/
void  a2_dl_phyi_set_dec_interrupt_type
(
  a2_sub_id_t  sub_id,   /*! System subscriber id */
  boolean      imm_interrupt /* immedite interrupt value */
);

/*===========================================================================

  FUNCTION:  a2_dl_phy_flush_recycle_pool

===========================================================================*/
/*!
  @brief
  API to flush the recycle dsm pool back to dsm lib
*/
/*=========================================================================*/
void a2_dl_phy_flush_recycle_pool
(
  void
);

#endif /* LTE_A2_DL_PHY_H */

