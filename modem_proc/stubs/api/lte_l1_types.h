#ifndef LTE_L1_TYPES_H
#define LTE_L1_TYPES_H

/*! @brief
  Supported frame structures enum
  Defined in 36.201 Section 4.2.1
*/
typedef enum
{
  /*! Frame structure 1 (generic frame structure) */
  LTE_L1_FRAME_STRUCTURE_FS1 = 0,

  /*! Frame structure 2 (alternative frame structure) */
  LTE_L1_FRAME_STRUCTURE_FS2,

  /*! Maximum number of frame structure for LTE cells */
  LTE_L1_FRAME_MAX_NUM_STRUCTURES
} lte_l1_frame_struct_e;

/*! @brief 
    Indicates TDD DL/UL configurations where LTE_L1_TDD_UL_DL_CFG_0 points to Configuration 0,
    LTE_L1_TDD_UL_DL_CFG_1 points to Configuration 1,  etc. as specified in the 36.211, table 4.2.2.
    (ref: 36.331, 6.3.2)
 
*/
typedef enum
{
  LTE_L1_TDD_UL_DL_CFG_0 = 0,
  LTE_L1_TDD_UL_DL_CFG_1 = 1,
  LTE_L1_TDD_UL_DL_CFG_2 = 2,
  LTE_L1_TDD_UL_DL_CFG_3 = 3,
  LTE_L1_TDD_UL_DL_CFG_4 = 4,
  LTE_L1_TDD_UL_DL_CFG_5 = 5,
  LTE_L1_TDD_UL_DL_CFG_6 = 6,
  LTE_L1_TDD_UL_DL_CFG_UNKNOWN_WITH_UNKNOWN_10MS_FRAME = 7,
  LTE_L1_TDD_UL_DL_CFG_UNKONWN_WITH_KNOWN_10MS_FRAME   = 8 ,
  LTE_L1_TDD_UL_DL_CFG_NUM
} lte_l1_tdd_ul_dl_cfg_index_e;

typedef enum {
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_0 = 0,
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_1,
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_2,
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_3,
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_4,
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_5,
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_6,
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_7,
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_8,
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_9, /* Rel-11 SSF9 */
  LTE_L1_TDD_SPECIAL_SUBFRAME_PATTERN_MAX /* Invalid value */
} lte_l1_tdd_special_subframe_pattern_e;

/*! @brief
  Supported cyclix prefix modes
  Defined in 36.211 Table 6.12-1 for normal and extended mode.
  TBD: fill in the spec reference for the extended_msbfn mode.
*/
typedef enum
{
  /*! Normal CP */
  LTE_L1_CP_MODE_NORMAL = 0,

  /*! Extended CP */
  LTE_L1_CP_MODE_EXTENDED,

  /*! Long CP for MBSFN
     @todo Do we need list the CP_MODE_EXTENDED MBSFN in this enum? */
  LTE_L1_CP_MODE_EXTENDED_MBSFN,

  LTE_L1_MAX_NUM_CP_MODES
} lte_l1_cp_e;

/*! @brief
    UL Cyclic Prefix Length.  len1 corresponds to normal cyclic prefix and
    len2 corresponds to extended cyclic prefix.
*/
typedef enum
{
  LTE_CPHY_UL_CYCLIC_PREFIX_LENGTH_1,
  LTE_CPHY_UL_CYCLIC_PREFIX_LENGTH_2,
  LTE_CPHY_UL_CYCLIC_PREFIX_LENGTH_MAX /* Invalid Value */
}lte_cphy_ul_cyclic_prefix_length_e;
#endif /* LTE_L1_TYPES_H */