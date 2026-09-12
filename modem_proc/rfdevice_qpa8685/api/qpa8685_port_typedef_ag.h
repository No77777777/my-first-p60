#ifndef QPA8685_TYPEDEF_AG_H
#define QPA8685_TYPEDEF_AG_H


#include "comdef.h"
/*----------------------------------------------------------------------------*/
/*!
  It defines the QPA8685 pa_port enum.
*/
typedef enum 
{
  QPA8685_GSM_BAND850_PORT_PA_2G_850, 
  QPA8685_GSM_BAND900_PORT_PA_2G_EGSM, 
  QPA8685_GSM_BAND1800_PORT_PA_2G_1800, 
  QPA8685_GSM_BAND1900_PORT_PA_2G_1900, 
  QPA8685_PORT_NUM, 
  QPA8685_PORT_INVALID, 
} qpa8685_pa_port_data_type;


#endif