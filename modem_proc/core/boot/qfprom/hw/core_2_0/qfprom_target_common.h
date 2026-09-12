#ifndef QFPROM_TARGET_COMMON_H
#define QFPROM_TARGET_COMMON_H

/*===========================================================================

                        QFPROM  Driver Header  File

DESCRIPTION
 Contains target common definitions and APIs to be used to read and write
 qfprom values for sec ctrl 3.0.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright  2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/boot/qfprom/hw/core_2_0/qfprom_target_common.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/15/15   jz      Created (Moved out target specific defines to target specific file)
============================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "qfprom.h"
#include "msmhwio.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

typedef enum
{
  QFPROM_FEC_NONE = 0,
  QFPROM_FEC_15_10,
  QFPROM_FEC_63_56,

  /* Add above this */
  QFPROM_MAX_FEC
}QFPROM_FEC_SCHEME;

typedef enum
{
  QFPROM_CONV_FORMAT_RAW_TO_CORR = 0,
  QFPROM_CONV_FORMAT_CORR_TO_RAW = 1,
}QFPROM_CONV_FORMAT;

typedef enum
{
  QFPROM_ROW_LSB = 0,
  QFPROM_ROW_MSB,
}QFPROM_ROW;

/* Invalid entry */
#define QFPROM_INVALID_ENTRY 0xFFFFFFFF

/* Raw to Corrected Address region offset  */
#define QFPROM_RAW_TO_CORRECTED_ADDRESS_OFFSET   (0x4000)

typedef struct
{

    /* region name; added only for readability */  
    uint32                        region;

    /* how many rows the region takes */
    uint32                        size;

    /*  fec type of region */
    QFPROM_FEC_SCHEME   fec_type;

    /* raw address  of the region */
    uint32                        raw_base_addr;      

    /* corrected address of the region */
    uint32                        corr_base_addr;

    /* can we read this region    */
    uint32                        read_perm_mask;      

    /* can we write to this region    */
    uint32                        write_perm_mask;

    /* LSB or MSB  of the qfprom permission region */
    QFPROM_ROW  perm_reg_type;
    
    /* Region read-able/non-read-able */
    boolean                       read_allowed;	
    
    /* Region index */
    uint32                       qfprom_region_index;	
    
} QFPROM_REGION_INFO;

typedef struct
{
  uint32 sec_ctrl_base;
  uint32 sec_ctrl_base_phy;
  uint32 raw_lowest_offset;
  uint32 raw_highest_offset;
  uint32 read_perm_addr;
  uint32 write_perm_addr;
  uint32 fec_en_lsb_addr;
  uint32 fec_en_msb_addr;

} qfprom_fusemap_data;
    

extern QFPROM_REGION_INFO *qfprom_region_ptr;
extern uint32 qfprom_region_size;


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

**  Function :

** ==========================================================================
*/
/*!
*   qfprom_is_region_readable
* 
* @brief
*   This function shall return if the read permission for the region passed is available or not.
* 
* @par Dependencies
*    None
* 
* @param
*   region - The QFPROM region to be checked.
* 
* @retval boolean 
*                         - TRUE - permission is available 
*                         - FALSE - permission is not available 
* 
* @par Side Effects
* 
*/
boolean     qfprom_is_region_readable
(
    uint32  region
);

/*===========================================================================

**  Function :    qfprom_is_region_writeable

** ==========================================================================
*/
/*!
*   qfprom_is_region_writeable
* 
* @brief
*   This function shall return if the write permission for the region passed is available or not.
* 
* @par Dependencies
*    None
* 
* @param
*   region - The QFPROM region to be checked.
* 
* @retval boolean 
*                         - TRUE - permission is available 
*                         - FALSE - permission is not available 
* 
* @par Side Effects
* 
*/
boolean     qfprom_is_region_writeable
(
    uint32  region
);

/*===========================================================================

**  Function : qfprom_get_region_name

** ==========================================================================
*/
/*!
* 
* @brief
*   This function gets the region of the given row address which need to be a RAW
*   address.
*  
* @par Dependencies
*   None.
*     
* @param:
*   uint32  address - Address of the Row.
*   QFPROM_ADDR_SPACE addr_type - Raw or Corrected address,
*   uint32 *region_type - QFPROM Region of the Row address 
*                                                              passed.
* 
* @retval
* @retval: return - QFPROM_NO_ERR - if the operation is success.
*                           error_type           - if the operation is failure.                      
* @par Side Effects
*   None.
* 
*   
*/
uint32 qfprom_get_region_name
 (
    uint32 address,
    QFPROM_ADDR_SPACE addr_type,
    uint32 *region_type    
 );

/*===========================================================================

**  Function : qfprom_is_fec_enabled

** ==========================================================================
*/
/*!
* 
* @brief
*   This function shall check the FEC status of the given Raw region row address.
*  
* @par Dependencies
*   None.
*     
* @param:
*   uint32 row_address - Address of the Row.
*   boolean *fec_status - TRUE , FEC is enabled. 
*                                   FALSE , FEC is disabled..
* 
* @retval
* @retval: return - QFPROM_NO_ERR - if the operation is success.
*                           error_type           - if the operation is failure.                      
* @par Side Effects
*   None.
* 
*/

uint32
qfprom_is_fec_enabled
(
    uint32    region,
    boolean *fec_status
);


/*===========================================================================

FUNCTION  qfprom_check_convert_to_active_address

DESCRIPTION
    This function shall check the address the appropriate voltage dependent settings for the qfprom block.

PARAMETERS
    uint32   row_address  - Pointer to the row address to be checked 
    uint32   *active_row_address  - Pointer to the active row address after conversion 

DEPENDENCIES
  None.
 
RETURN VALUE
  uint32  Any Errors while setting up the system before blowing the fuses.

SIDE EFFECTS
  Disables the vreg being used.

===========================================================================*/

uint32
qfprom_check_convert_to_active_address
(
    uint32 row_address,
    uint32*active_row_address
);


/*===========================================================================

FUNCTION  qfprom_check_boundary_condition_for_row_address

DESCRIPTION
    This function shall check the boundary condition for the row address to be operated.

PARAMETERS
    uint32   row_address  - Row address to be checked 

DEPENDENCIES
  None.
 
RETURN VALUE
  uint32  Any Errors while setting up the system before blowing the fuses.

SIDE EFFECTS
  Disables the vreg being used.

===========================================================================*/

uint32
qfprom_check_boundary_condition_for_row_address
(
    uint32 row_address
);


/*===========================================================================

FUNCTION  QFPROM_WRITE_FUSE_INIT

DESCRIPTION
  Here we might need to enable appropriate volatge regulator for each target.
  Currently MDM9x00 have volatge regulators ON by default.  
PARAMETERS
  None.

DEPENDENCIES
  None.
 
RETURN VALUE
  uint32  Any Errors while setting up the system before blowing the fuses.

SIDE EFFECTS
  Turns on qfprom clock and enables VREG_GP5.

===========================================================================*/
uint32 qfprom_write_fuse_init( void );


/*===========================================================================

FUNCTION  QFPROM_WRITE_FUSE_CLEANUP

DESCRIPTION
  This function is going to use PMIC library API to trun OFF the LDO.
  Currently though for MDM9x00 we do not need to do anything.
 
DEPENDENCIES
  None.

PARAMETERS
  None.
 
RETURN VALUE
  uint32   Any Errors while doing the cleanup.

SIDE EFFECTS
  Turns off qfprom clock and disables VREG_GP5.

===========================================================================*/
uint32 qfprom_write_fuse_cleanup( void );

/*===========================================================================

FUNCTION  QFPROM_IS_WRITE_OPERATION_ALLOWED

DESCRIPTION
  This function returns whether the clocks and voltage regulators have been
  setup correctly for writing fuses. 

DEPENDENCIES
  None.

PARAMETERS
  None.
 
RETURN VALUE
  boolean  Whether writing of fuses has been enabled.

SIDE EFFECTS
  None.

===========================================================================*/
boolean qfprom_is_write_operation_allowed ( void );

/*===========================================================================

FUNCTION  qfprom_write_set_clock_dependent_settings

DESCRIPTION
    This function shall set the appropriate clock dependent settings for the qfprom block.

PARAMETERS
  None.

DEPENDENCIES
  None.
 
RETURN VALUE
  uint32  Any Errors while setting up the system before blowing the fuses.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 qfprom_write_set_clock_dependent_settings( void );

/*===========================================================================

FUNCTION  qfprom_write_reset_clock_dependent_settings

DESCRIPTION
    This function shall reset the appropriate clock dependent settings for the qfprom block.

PARAMETERS
  None.

DEPENDENCIES
  None.
 
RETURN VALUE
  uint32  Any Errors while setting up the system before blowing the fuses.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 qfprom_write_reset_clock_dependent_settings( void );

/*===========================================================================

**  Function : qfprom_get_region_index

** ==========================================================================
*/
/*!
* 
* @brief
*  This function gets the index of the region to which the given row address
*  belongs
*  
* @par Dependencies
*  None.
*     
* @param:
*  uint32 address - Address of the Row.
*  QFPROM_ADDR_SPACE addr_type - Raw or Corrected address,
*  uint32 *region_index - pointer to address location which will
*                          be filled in with index value by api.
* 
* @retval
* @retval: return - QFPROM_NO_ERR - if the operation is success.
*                   error_type    - if the operation is failure.                      
* @par Side Effects
*  None.
* 
*   
*/
uint32 qfprom_get_region_index
(
    uint32 address,
    QFPROM_ADDR_SPACE addr_type,
    uint32 *region_index
);


/*===========================================================================
**  Function :    : qfprom_write

** ==========================================================================
*/
/*!
*
* @brief :  This function writes the actual raw data to the row
*
*
* @param  :
*                 uint32   raw_row_address,
*                 uint32   *raw_data_to_write     - Pointer to data.
* @par Dependencies:
*
* @retval: return - QFPROM_NO_ERR - if the operation is success.
*                    error_type           - if the operation is failure.
*
* @par Side Effects:
* algo: Write the lower 32 bits and wait for the fuse write status to be ok.
*       Then write the upper 32 bits
*       If the write operation goes ok return QFPROM_NO_ERR else the error value.
*/
uint32 qfprom_write
(
    uint32   raw_row_address,
    uint32   *raw_data_to_write
);


/*===========================================================================
**  Function :    : qfprom_write_check_row_pre_conditions

** ==========================================================================
*/
/*!
*
* @brief :  This function shall check if the row address is part of supported QFPROM Region
*            returns the status.
*
* @param  :
*                 uint32   raw_row_address,
*                 uint32   *row_data,           - Pointer to data.
*                 uint32   *region_type
* @par Dependencies:
*
* @retval: return - QFPROM_NO_ERR - if the operation is success.
*                   error_type           - if the operation is failure.
*
*/
uint32 qfprom_write_check_row_pre_conditions
(
    uint32   raw_row_address,
    uint32   *row_data,
    uint32   *region_type
);


/*===========================================================================

**  Function : qfprom_find_index

** ==========================================================================
*/
/*!
*
* @brief
*   This function finds the index of the fuse in the chain.
*
* @par Dependencies
*   None.
*
* @param:
*   uint32  This tells the region ID for which we want to find index.
*
* @retval
*   uint32  Index in qfprom table that matches qfpromId;
*           return QFPROM_INVALID_ENTRY in no match found.
*
* @par Side Effects
*   None.
*
*
*/
uint32 qfprom_find_index(uint32 qfpromRegion);

/*===========================================================================
**  Function :    : qfprom_verify_write

** ==========================================================================
*/
/*!
*
* @brief :  If read is allowed for the region, then perform the verification to check the data written
*
*
* @param  :
*                 uint32   * region_type
*                 uint32   *raw_data_to_write     - Pointer to the write data
*                 uint32   *read_row_data          - Pointer to the data read
* @par Dependencies:
*   qfprom_init_fusemap has been called to initialize qfprom_region_data_ptr
*
* @retval: return - QFPROM_NO_ERR - if the operation is success.
*                     QFPROM_WRITE_ERR - if verification failed.
*
*/
uint32 qfprom_verify_write
(
  uint32  region_type,
  uint32  *raw_data_to_write,
  uint32  *read_row_data
);

/*===========================================================================
**  Function :    : qfprom_init_fusemap

** ==========================================================================
*/
/*!
*
* @brief :  This function initializes the qfprom_region_data table pointer and other variables.
*
* @param  :
*   None

* @par Dependencies:
*
* @retval: None
*
*/
void qfprom_init_fusemap
( 
  void 
);

/*===========================================================================
**  Function :    : qfprom_update_fusemap

** ==========================================================================
*/
/*!
*
* @brief :  This function initializes the qfprom_region_data table pointer and other variables.
*
* @param  :
*   qfprom_fusemap_data *fusemap_data_ptr  - pointer to access the fusemap related variables

* @par Dependencies:
*
* @retval: None
*
*/
void qfprom_update_fusemap
( 
  qfprom_fusemap_data *fusemap_data_ptr
);

#endif /* QFPROM_TARGET_COMMON_H */

