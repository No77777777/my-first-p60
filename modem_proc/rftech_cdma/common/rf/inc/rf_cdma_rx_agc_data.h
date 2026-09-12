#ifndef RF_CDMA_RX_AGC_DATA__H
#define RF_CDMA_RX_AGC_DATA__H

#include "rflm_rxagc_api.h"
/*----------------------------------------------------------------------------*/
typedef struct
{

  cell_id_t cell_id;

  /*! Void pointer to the tech specific scrathc area assigned by the common 
    RxAGC module, this is to be cast to the cdma specific data structure at
    MDSP layer. The data is used for jammer detection and other Rx related
    operations */
  void* rxagc_tech_data;
} rf_cdma_rxagc_data_t;


#endif /* RF_CDMA_RX_AGC_DATA__H */

