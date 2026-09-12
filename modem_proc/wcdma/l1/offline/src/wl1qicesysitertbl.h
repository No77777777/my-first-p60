#ifndef WMCVS_CLK_ITER_H
#define WMCVS_CLK_ITER_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         WCDMA MCVS Driver interface

GENERAL DESCRIPTION

  This file contains the auto generated QICE iterations tables for SC and DC


Copyright (c) 2000-2015 by Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/wl1qicesysitertbl.h#4 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when           who      what, where, why
--------       ----     ---------------------------------------------------------- 
11/20/15   svh    Update QICE tables taking into account new search mode..
04/02/15       pkj      Changes in max_qset_size and max_iter_size tables
12/02/14       svh      Externed tables.
11/26/14       rsr      Updated Table Based on System Recommendation.
11/18/14       rsr      Updated Table Based on System Recommendation.
11/6/14        sks      Initial version

 ===========================================================================*/

#include "rex.h"
#include "mcpm_api.h"

/*time 2014 10 28 16 38 5.189600e+01 */
/*time = 2014 11 18 22 56 1.993500e+01 */
/*time = 2014 11 25 23 37 5.963200e+01 */
/*time = 2015 4 2 12 27 4.256000e+00 */

/* max Qset for SC: Dimensions = SrchMode(1..3) Nrx(1..2) Ntx(1..2) Ncell(2:2:8) */
extern uint8 wl1_cme_max_qset_size_array[4][2][2][4];

/* Iter for SC: Dimensions = SrchMode(1..3) Nrx(1..2) Ntx(1..2) Nqset(1..3) Ncell(2:2:8) */
extern uint8 wl1_cme_qice_num_iter[4][2][2][3][4];
/* DC qset indexing for DC: Dimensions = Nqset0(1..3) Nqset1(1..3) */
extern uint8 wl1_cme_qice_qset_table_idx[3][3];


#endif /* ifndef WMCVS_CLK_ITER */
