/*!
  @file
  vstmr.h

  @brief
  Virtual system timer interface.

  Generic functions used to emulate a tech-specific system timer using
  a shared common timer source.

*/

/*===========================================================================

  Copyright (c) 2019 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

$Header: //components/rel/mcs.mpss/5.2/api/vstmr.h#13 $

===========================================================================*/

#ifndef VSTMR_H
#define VSTMR_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include <comdef.h>
#include <atomic_ops.h>
#include <mcs_hwio.h>
#include <qurt.h>


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*! @brief VSTMR API version. Compare this to vstmr_get_api() to ensure
    that VSTMR API is in sync.
*/
#define VSTMR_API_VERSION                  (0x6)

/*! @brief Add alias for STMR register */
#ifndef HWIO_MSS_STMR_TIME_RD_IN
  #define MSS_STMR_TIME_RD                 STMR_TIME_RD
#endif

/*! @brief RTC identifiers.
*/
typedef enum
{
  VSTMR_RTC_WCDMA_SUB_START  = 0,
  VSTMR_RTC_WCDMA_SUB0       = VSTMR_RTC_WCDMA_SUB_START,   /*!< WCDMA subscription 0 */
  VSTMR_RTC_WCDMA_SUB1,                                     /*!< WCDMA subscription 1 */
  VSTMR_RTC_WCDMA_SUB_END    = VSTMR_RTC_WCDMA_SUB1,

  VSTMR_RTC_LTE_SUB_START,
  VSTMR_RTC_LTE_SUB0         = VSTMR_RTC_LTE_SUB_START,     /*!< LTE subscription 0 */
  VSTMR_RTC_LTE_SUB1,                                       /*!< LTE subscription 1 */
  VSTMR_RTC_LTE_SUB_END      = VSTMR_RTC_LTE_SUB1,

  VSTMR_RTC_GERAN_SUB_START,
  VSTMR_RTC_GERAN_SUB0       = VSTMR_RTC_GERAN_SUB_START,   /*!< GERAN subscription 0 */
  VSTMR_RTC_GERAN_SUB1,                                     /*!< GERAN subscription 1 */
  VSTMR_RTC_GERAN_SUB_END    = VSTMR_RTC_GERAN_SUB1,

  VSTMR_RTC_HDR_SUB_START,
  VSTMR_RTC_HDR_SUB0         = VSTMR_RTC_HDR_SUB_START,     /*!< HDR subscription 0 */
  VSTMR_RTC_HDR_SUB1,                                       /*!< HDR subscription 1 */
  VSTMR_RTC_HDR_SUB_END      = VSTMR_RTC_HDR_SUB1,

  VSTMR_RTC_1X_SUB_START,
  VSTMR_RTC_1X_SUB0          = VSTMR_RTC_1X_SUB_START,      /*!< 1x subscription 0 */
  VSTMR_RTC_1X_SUB1,                                        /*!< 1x subscription 1 */
  VSTMR_RTC_1X_SUB_END       = VSTMR_RTC_1X_SUB1,

  VSTMR_RTC_TDS_SUB_START,
  VSTMR_RTC_TDS_SUB0         = VSTMR_RTC_TDS_SUB_START,     /*!< TDS subscription 0 */
  VSTMR_RTC_TDS_SUB1,                                       /*!< TDS subscription 1 */
  VSTMR_RTC_TDS_SUB_END      = VSTMR_RTC_TDS_SUB1,

  VSTMR_NUM_RTC,
  VSTMR_RTC_INVALID          = VSTMR_NUM_RTC                /*!< Invalid RTC identifier */
} vstmr_rtc_id;

/*! @brief Bitmasks of comparators with different capabilities.
    STMR[5:0] support HIGH priority, connected to L1 also.
    STMR[31:30] support VFR strobes
    The remainder are NORMAL.
*/
#define VSTMR_INT_HIGH_MASK              (0x0000003F)
#define VSTMR_INT_VFR_MASK               (0xC0000000)
#define VSTMR_INT_VFR_PRI_MASK           (0x40000000)
#define VSTMR_INT_VFR_SEC_MASK           (0x80000000)
#define VSTMR_INT_NORMAL_MASK            (0xFFFFFFFF & ~(VSTMR_INT_HIGH_MASK | VSTMR_INT_VFR_MASK))

/*! @brief Comparator type identifiers.
    HW comparators have different capabilities. This enum allows selecting
    comparators of a particular type.
    Note that this does not directly affect the priority of the interrupt
    handler - it only selects comparators which could be used for the specified
    purpose.
    NORMAL: Normal comparator. Clients with no special preference should use this.
    HIGH:   Interrupt is connected to Hexagon L1 interrupt input, as well as L2.
            These can be registered as either an L1 or an L2 interrupt.
    VFR:    The comparator generates a VFR strobe when it expires.
            These should be used only when a VFR signal is needed, and only
            a single event should be pending on them at any time.
*/
typedef enum
{
  VSTMR_INT_NORMAL   =  VSTMR_INT_NORMAL_MASK,  /*!< Normal Priority Interrupt */
  VSTMR_INT_HIGH     =  VSTMR_INT_HIGH_MASK,    /*!< High Priority Interrupt. */
  VSTMR_INT_VFR_PRI  =  VSTMR_INT_VFR_PRI_MASK, /*!< Primary VFR interrupt */
  VSTMR_INT_VFR_SEC  =  VSTMR_INT_VFR_SEC_MASK, /*!< Secondary VFR interrupt */
  VSTMR_INT_VFR_AUTO =  VSTMR_INT_VFR_MASK      /*!< For auto-allocation of VFR */
} vstmr_int_types_e;


/*! @brief Macro to read current XO count.
    This returns the 24-bit MSS STMR count.
    User needs to add the appropriate HWIO #includes - doing it from this
    file will slow down compile time everywhere.
*/
#ifdef TEST_FRAMEWORK
#error code not present
#else
#define VSTMR_XO_READ()                 (vstmr_get_ustmr() & VSTMR_XO_MASK)
#define VSTMR_XO_READ_FULL()            (vstmr_get_ustmr_full())
#define VSTMR_XO_READ_FINE()            (VSTMR_XO_READ_FULL()<<VSTMR_NUM_XO_FRAC_BITS)
#endif /* TEST_FRAMEWORK */

/*! @brief Number of bits in XO count.
*/
#define VSTMR_NUM_XO_BITS               (24)
#define VSTMR_XO_MASK                   ((1<<VSTMR_NUM_XO_BITS)-1)
#define VSTMR_NUM_XO_FRAC_BITS          (8)

/*! @brief XO rate (19.2 MHz)
*/
#define VSTMR_XO_RATE_HZ                (19200000)

/*! @brief Maximum number of events per event table.
*/
#define VSTMR_MAX_EVENTS                (16)

/* Structures to optimize/ease dealing with words within a double/quad word */
typedef union
{
  uint64 unsigned_d;
  uint64 ud;
  int64  signed_d;
  int64  d;
  uint32 unsigned_w[2];
  uint32 uw[2];
  int32  signed_w[2];
  int32  w[2];
} vstmr_dword_u;

typedef union
{
  uint64 unsigned_d[2];
  uint64 ud[2];
  int64  signed_d[2];
  int64  d[2];
  uint32 unsigned_w[4];
  uint32 uw[4];
  int32  signed_w[4];
  int32  w[4];
} vstmr_qword_u;

/*! @brief Extern for opaque RTC handle.
*/
struct vstmr_rtc_s;

/*! @brief For storing the full resolution fine XO information.
*/
typedef union
{
  uint64         all;  /*!< To access the full value */
  vstmr_dword_u  val;  /*!< To access as a dword */

  struct {
    uint32 frac  : VSTMR_NUM_XO_FRAC_BITS;        /*!< Fractional part */
    uint32 lower : (32 - VSTMR_NUM_XO_FRAC_BITS); /*!< Lower bits (=VSTMR_NUM_XO_BITS) of the integer XO */
    uint32 upper;                                 /*!< Upper bits of the integer XO */
  };

} vstmr_fine_xo_t;

/*! @brief For storing the full resolution RTC information.
*/
typedef union
{
  vstmr_qword_u val;  /*!< To access as a qword */

  struct {
    uint64 frac;      /*!< Fractional */
    uint64 integer;   /*!< Integer */
  };

} vstmr_fine_rtc_t;

/*! @brief Event parameters.
*/
typedef union
{
  /* Note these are overlayed */
  atomic_word_t atomic;      /*!< For atomic access to all parameters */

  uint32 all;                /*!< Access to entire structure */

  uint32 xo_msb_aligned;     /*!< Access to MSb-aligned XO count, for wraparound diff */

  struct {
    uint32 enabled : 1;                  /*!< 1 = event enabled */
    uint32         : (32 - VSTMR_NUM_XO_BITS - 1);
    uint32 xo_cnt  : VSTMR_NUM_XO_BITS;  /*!< Expiration XO count */
  };

} vstmr_event_item_s;


/*! @brief Event table management structure.
*/
typedef struct
{
  vstmr_event_item_s events[VSTMR_MAX_EVENTS];  /*!< Event information */
  uint16             irq_id;                    /*!< Interrupt ID for comparator.
                                                     When 0, also means this event table is deregistered. */
  uint8              comp_id;                   /*!< Comparator index associated with event table */

  qurt_mutex_t       *comp_mutex_ptr;           /*!< Mutex pointer to protect programming for the HW comparator
                                                     associated for this event table. By default, its
                                                     initialized for SW event tables and set to NULL for FW event
                                                     tables. FW has some fastint timelines which can't take
                                                     a mutex. However, it can be enabled using an API call
                                                     if needed. */

  vstmr_event_item_s next_event;                /*!< Next event time stored in comparator */

} vstmr_event_tbl_s;


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Versioning related API's */
extern uint32 vstmr_get_api(void);

/* Init/teardown related API's */
extern void vstmr_init(void);
extern void vstmr_teardown(void);
extern void vstmr_suspend(void);
extern void vstmr_resume(void);

/* RTC handle related API's */
extern struct vstmr_rtc_s * vstmr_get_rtc_handle
(
  vstmr_rtc_id id
);

/* XO<->RTC translation related API's - Legacy resolution */
extern uint64 vstmr_get_current_rtc
(
  struct vstmr_rtc_s *rtc
);

extern uint64 vstmr_xo_to_rtc
(
  struct vstmr_rtc_s *rtc,
  uint32              xo
);

extern uint32 vstmr_rtc_to_xo
(
  struct vstmr_rtc_s *rtc,
  uint64              rtc_cnt
);

/* XO<->RTC translation related API's - Extended resolution */
extern uint64 vstmr_fine_xo_to_rtc
(
  struct vstmr_rtc_s *rtc,
  vstmr_fine_xo_t     fine_xo
);

extern void vstmr_fine_xo_to_fine_rtc
(
  struct vstmr_rtc_s *rtc,
  vstmr_fine_xo_t     fine_xo,
  vstmr_fine_rtc_t   *fine_rtc
);

extern vstmr_fine_xo_t vstmr_rtc_to_fine_xo
(
  struct vstmr_rtc_s *rtc,
  uint64              rtc_cnt
);

extern vstmr_fine_xo_t vstmr_fine_rtc_to_fine_xo
(
  struct vstmr_rtc_s *rtc,
  vstmr_fine_rtc_t   *fine_rtc
);

/* Unwrap related API's */
extern vstmr_fine_xo_t vstmr_get_fine_xo_unwrap
(
  uint32              xo_cnt
);

extern uint64 vstmr_get_full_rtc_unwrap
(
  struct vstmr_rtc_s *rtc,
  uint32              rtc_cnt_int
);

/* FCW get/update related API's - Legacy resolution */
extern uint32 vstmr_get_rtc_fcw
(
  struct vstmr_rtc_s *rtc
);

extern void vstmr_update_rtc_fcw
(
  struct vstmr_rtc_s *rtc,
  uint32 xo_cnt,
  uint32 fcw
);

extern void vstmr_update_rtc_err_ratio_q45
(
  struct vstmr_rtc_s *rtc,
  uint32              xo_cnt,
  int32               err_ratio_q45
);

/* FCW get/update related API's - Extended resolution */
extern uint64 vstmr_get_rtc_fcw_ext
(
  struct vstmr_rtc_s *rtc
);

int32 vstmr_get_rtc_err_ratio_Q45
(
  struct vstmr_rtc_s *rtc
);

extern void vstmr_update_rtc_fcw_ext
(
  struct vstmr_rtc_s *rtc,
  vstmr_fine_xo_t     fine_xo,
  uint64              fcw
);

extern void vstmr_update_rtc_err_ratio_q45_ext
(
  struct vstmr_rtc_s *rtc,
  vstmr_fine_xo_t     fine_xo,
  int32               err_ratio_q45
);

/* FCW conversion related API's - Legacy resolution */
extern int32 vstmr_fcw_to_ppmQ10
(
  struct vstmr_rtc_s *rtc,
  uint32              fcw
);

extern uint32 vstmr_err_ratio_to_fcw
(
  struct vstmr_rtc_s *rtc,
  int32               err_ratio_Q45
);

extern uint32 vstmr_ppmQ10_to_fcw
(
  struct vstmr_rtc_s *rtc,
  int32               ppmQ10
);

/* FCW conversion related API's - Extended resolution */
extern int32 vstmr_fcw_ext_to_err_ratio_Q45
(
  struct vstmr_rtc_s *rtc,
  uint64              fcw_ext
);

extern uint64 vstmr_err_ratio_Q45_to_fcw_ext
(
  struct vstmr_rtc_s *rtc,
  int32               err_ratio_Q45
);

extern int64 vstmr_ppmQ10_to_err_ratio_Q45
(
  int32               ppmQ10
);

extern int32 vstmr_err_ratio_Q45_to_ppmQ10
(
  int64               err_ratio_Q45
);

/* Time slam related API's - Legacy resolution */
extern void vstmr_rtc_sync
(
  struct vstmr_rtc_s *rtc,
  uint32              xo_cnt,
  uint64              rtc_cnt
);

/* Time slam related API's - Extended resolution */
extern void vstmr_fine_rtc_sync
(
  struct vstmr_rtc_s *rtc,
  vstmr_fine_xo_t     fine_xo,
  vstmr_fine_rtc_t   *fine_rtc
);

/* Event handling related API's */
extern void vstmr_event_tbl_register
(
  vstmr_event_tbl_s *event_tbl,
  vstmr_int_types_e int_type
);

extern void vstmr_event_tbl_deregister
(
  vstmr_event_tbl_s *event_tbl
);

extern void vstmr_event_sched
(
  vstmr_event_tbl_s *event_tbl,
  uint32 event_idx,
  uint32 xo_count
);

extern void vstmr_event_cancel
(
  vstmr_event_tbl_s *event_tbl,
  uint32 event_idx
);

extern uint32 vstmr_event_service
(
  vstmr_event_tbl_s *event_tbl
);

extern void vstmr_event_tbl_set_mutex
(
  vstmr_event_tbl_s *event_tbl
);

/* USTMR related API's */
#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/*===========================================================================

  FUNCTION:  vstmr_get_ustmr

===========================================================================*/
/*!
    @brief
    Get USTMR count by reading UTIMERLO register (counts in XO resolution
    at 19.2MHz).

    NOTE: UTIMERLO is a Q6 Core register, available on Hexagon V61 (or
          greater).

    @return
    USTMR count (32 bits).
*/
/*=========================================================================*/
INLINE uint32 vstmr_get_ustmr
(
  void
)
{
  #ifdef TEST_FRAMEWORK
  #error code not present
#else
  uint32 utimerlo;
  asm volatile ("%[utimerlo] = UTIMERLO" : [utimerlo] "=r"(utimerlo));
  return utimerlo; /*lint !e530  "reg not initialized" */
  #endif /* TEST_FRAMEWORK */

} /* vstmr_get_ustmr() */

/*===========================================================================

  FUNCTION:  vstmr_get_ustmr_full

===========================================================================*/
/*!
    @brief
    Get full USTMR count by reading UTIMER register (counts in XO resolution
    at 19.2MHz).

    NOTE: UTIMER is a Q6 Core register, available on Hexagon V61 (or greater).

    @return
    Full USTMR count (56 bits).
*/
/*=========================================================================*/
INLINE uint64 vstmr_get_ustmr_full
(
  void
)
{
  #ifdef TEST_FRAMEWORK
  #error code not present
#else
  uint64 utimer;
  asm volatile ("%[utimer] = UTIMER" : [utimer] "=r"(utimer));
  return utimer; /*lint !e530  "reg not initialized" */
  #endif /* TEST_FRAMEWORK */

} /* vstmr_get_ustmr_full() */


/*===========================================================================

  FUNCTION:  vstmr_xo_read_full

===========================================================================*/
/*!
    @brief
    Return full range of XO/USTMR count. Same as vstmr_get_ustmr_full()/
    VSTMR_XO_READ_FULL(). Retained for backward compatibility.

    @return
    Full USTMR count (56 bits).
*/
/*=========================================================================*/
INLINE uint64 vstmr_xo_read_full(void)
{
  /* Return full XO count */
  return vstmr_get_ustmr_full();

} /* vstmr_xo_read_full() */

/* Timetick related API's */
/*! @brief The following functions give access to a 32-bit and 64-bit version of the
           current 'timetick' counter, which runs at a rate of XO/586. Note that
           this is actually 32764.5 Hz in XO-based targets, and not 32768 Hz, as it
           was in legacy targets with a standalone sleep xtal. */
extern uint64 vstmr_get_timetick_full
(
  void
);
#define vstmr_get_timetick()  ((uint32)vstmr_get_timetick_full())

extern uint64 vstmr_xo_to_timetick
(
  uint64 xo_cnt
);

extern uint64 vstmr_timetick_to_xo
(
  uint64 timetick
);

#ifdef __cplusplus
}  // extern "C"
#endif
#endif /* VSTMR_H */
