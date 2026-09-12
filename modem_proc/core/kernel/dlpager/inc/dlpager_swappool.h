
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               DL PAGER TOP LEVEL HEADER FILE

GENERAL DESCRIPTION

  Copyright (c) 2010 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/09/14   rr      Header file for APIs related swap pool management
===========================================================================*/
#include <qurt_types.h>
#include <dlpager_swapmem.h>
#if defined( DLPAGER_UNIT_TEST )
#include <stdio.h>
#endif

typedef struct
{
  unsigned int va;
  unsigned int va_swap_rw;
  unsigned int va_swap_rx;
  unsigned int pa;
} dlpager_swappool_page_info_t; 

void dlpager_swappool_init( void );

void dlpager_swappool_lookup( unsigned int va, dlpager_swappool_page_info_t *info );

unsigned int dlpager_swappool_rw_evict_page( void ** token );
void dlpager_swappool_rw_insert_mapping( unsigned int va, unsigned int pa, void * token );
void dlpager_swappool_rw_remove_mapping( unsigned int va );
void dlpager_swappool_rw_get_pa( unsigned int va, unsigned int *pa );
void dlpager_swappool_rw_mru_page( unsigned int va );
void dlpager_swappool_rw_get_info( qurt_addr_t * const va, unsigned int * const size );
void dlpager_swappool_rw_decrement_clean_page_count( void );
void dlpager_swappool_rw_increment_clean_page_count( void );
void dlpager_swappool_rw_initiate_soft_clean( void );

unsigned int dlpager_swappool_rx_evict_page( void ** token );
void dlpager_swappool_rx_insert_mapping( unsigned int va, unsigned int pa, void * token );
void dlpager_swappool_rx_remove_mapping( unsigned int va );
void dlpager_swappool_rx_get_pa( unsigned int va, unsigned int *pa );
void dlpager_swappool_rx_mru_page( unsigned int va );
/*void dlpager_swappool_rx_get_info( qurt_addr_t * const va, unsigned int * const size );*/

/**
 * @brief Get swap-memories associated with RX swap-pool 
 *  
 * @param [out] swapmem Pointer to an array of swap-memories
 * @param [out] num_swapmem Number of swap-memories
 */
extern void dlpager_swappool_rx_get_swapmem (
    dlpager_swapmem_t const ** swapmem,
    unsigned int *             num_swapmem
);

/**
 * @brief Get swap-memories associated with RW swap-pool 
 *  
 * @param [out] swapmem Pointer to an array of swap-memories
 * @param [out] num_swapmem Number of swap-memories
 */
extern void dlpager_swappool_rw_get_swapmem (
    dlpager_swapmem_t const ** swapmem,
    unsigned int *             num_swapmem
);

void dlpager_swappool_perform_mru( void );

/**
 * @brief Reset RX swap-pool 
 * @warning Must be called from dlpager_reset_rx() only! 
 * 
 * @return unsigned int Number of pages reset
 */
extern unsigned int dlpager_swappool_rx_reset (void);

#if defined( DLPAGER_UNIT_TEST )
/**
 * @brief Pretty-print DL pager RX swap to file 
 *  
 * @param [in] ofile File to print to 
 */
extern void dlpager_swappool_rx_print (FILE * ofile);

/**
 * @brief Pretty-print DL pager RW swap to file 
 *  
 * @param [in] ofile File to print to 
 */
extern void dlpager_swappool_rw_print (FILE * ofile);

extern qurt_addr_t dlpager_swappool_rx_get_virtaddr (void);

#endif
