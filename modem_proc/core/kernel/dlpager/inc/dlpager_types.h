/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

   DL PAGER TOP LEVEL HEADER FILE

   GENERAL DESCRIPTION

   Copyright (c) 2016 Qualcomm Technologies Incorporated.
   All Rights Reserved. QUALCOMM Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

EDIT HISTORY FOR MODULE


===========================================================================*/
#pragma once

#define THREAD_NULL (0xffffffff)

/** @brief Hint to compiler that condition is likely */
#define DLPAGER_LIKELY( exp ) __builtin_expect((exp), 1)

/** @brief Hint to compiler that condition is unlikely */
#define DLPAGER_UNLIKELY( exp ) __builtin_expect((exp), 0)

/** @brief Handy macro to return minimum of two integers */
#define DLPAGER_MIN( a, b ) ( ( a ) <= ( b ) ? ( a ) : ( b ) )

/** @brief Handy macro to get the number of elements in an array */
#define DLPAGER_ARRAY_SIZE( array ) ( sizeof( ( array ) ) / sizeof( ( array )[ 0 ] ) )

typedef enum
{
   TLB_MISS_X,
   TLB_MISS_R,  
   TLB_MISS_W,
   EVICT_PAGE,
   DECOMPRESSION_COMPLETE,
   COMPRESSION_COMPLETE,
   SOFT_CLEAN_PAGE,
   HARD_CLEAN_PAGE,
   SOFT_CLEAN_FAILED,
   INVALID_EVENT,
   MAX_EVENTS = INVALID_EVENT,
} dlpager_event_t;


/* enum for page states */
typedef enum
{
   UNMAPPED_CLEAN,
   UNMAPPED_BSS,
   UNMAPPED_CLEAN_DECOMPRESSING,
   UNMAPPED_DIRTY_DECOMPRESSING,
   UNMAPPED_DIRTY_COMPRESSING_ALLOCATED,
   UNMAPPED_CLEAN_ALLOCATED,
   UNMAPPED_HARD_CLEAN_COMPRESSING,
   MAPPED_CLEAN,
   MAPPED_DIRTY,
   MAPPED_DIRTY_COMPRESSING,
   MAPPED_CLEAN_COMPRESSING,
   MAPPED_CLEAN_BSS,
   INVALID_STATE,
   MAX_STATES = INVALID_STATE,
} dlpager_page_state_t;

//typedef enum
//{
   //SOFT_CLEAN = SOFT_CLEAN_PAGE,
   //HARD_CLEAN = HARD_CLEAN_PAGE,
   //NO_CLEAN = INVALID_EVENT,
//} dlpager_clean_t;

typedef enum
{
   LOW_PRIORITY = SOFT_CLEAN_PAGE,
   HIGH_PRIORITY = HARD_CLEAN_PAGE,
} dlpager_priority_t;

/** @brief Type for an I/O vector - something that has address and length */
typedef struct
{
   /** @brief Address associated with this I/O vector */
   unsigned int addr;
   /** @brief Length (bytes) of this I/O vector */
   unsigned int len;
} dlpager_iovec_t;
