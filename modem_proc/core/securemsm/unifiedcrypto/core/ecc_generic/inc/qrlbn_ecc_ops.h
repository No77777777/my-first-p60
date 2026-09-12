#ifndef _QRLBN_ECC_OPS_H
#define _QRLBN_ECC_OPS_H

/*

  Copyright © 2014, 2019 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
  
*/

#include <stdint.h>
#include "qrlbn_arith.h"
#include "qrlbn_ecc_types.h"

#ifdef	__cplusplus
extern "C" {
#endif
#if 0
} /* keep emacs from indenting all the stuff below */
#endif


/*
 * p and z can alias.  q and z can alias. If z is NULL, the locations of some tempory variables are zeroed.
 * This is a hack and depens on the stack being the same for this and previous calls.  flags is ignored.
 */
int 
qrlbn_point_add_standard(qrlbn_ecc_point_t *z, const qrlbn_ecc_point_t *p, 
			 const qrlbn_ecc_point_t *q, 
			 const qrlbn_ecc_domain_t *dp, int flags);

/*
 * p, q, and z can alias.  Care is taken to never branch when p and q
 * denote the same point.  If flags contains QRLBN_NON_SENSITIVE, this
 * just calls qrlbn_point_add_standard.  If z is NULL, the locations of
 * some temporay variables are zeroed.  This is a hack and depens on
 * the stack being the same for this and previous calls.
 */
int  qrlbn_point_add_oblivious(qrlbn_ecc_point_t *z, const qrlbn_ecc_point_t *p, 
			       const qrlbn_ecc_point_t *q, 
			       const qrlbn_ecc_domain_t *dp, int flags);

/*
 * p must be non-negative.  p and tgt can alias
 */
int qrlbn_point_to_affine(qrlbn_ecc_point_t *tgt, const qrlbn_ecc_point_t *p, 
			  const qrlbn_ecc_domain_t *dp);



/*
 * coordinates of p must be non-negative
 * z and p can alias
 */

int qrlbn_point_dbl(qrlbn_ecc_point_t *z, const qrlbn_ecc_point_t *p, 
			const qrlbn_ecc_domain_t *dp);


/*
 * coordinates of p and q must be non-negative z, p, and q can alias.
 * The oblivious version never branches to convert a point add to a
 * point double.  The QRLBN_NON_SENSITIVE saves time at the expense of
 * leakage.
 */
int qrlbn_point_multiply(qrlbn_ecc_point_t * restrict z, 
			 const qrlbn_ecc_bigval_t *k, 
			 const qrlbn_ecc_point_t *base, 
			 const qrlbn_ecc_domain_t *dp, int flags);


int qrlbn_import_point(qrlbn_ecc_point_t * restrict tgt, 
		       const qrlbn_ecc_affine_point_t *p,
		       const qrlbn_ecc_domain_t *dp, int flags);

int qrlbn_export_point(qrlbn_ecc_affine_point_t * restrict tgt,
		       const qrlbn_ecc_point_t *p, 
		       const qrlbn_ecc_domain_t *dp);

boolean_t qrlbn_point_on_curve(const qrlbn_ecc_point_t *Q, 
		     const qrlbn_ecc_domain_t *dp);

void qrlbn_zeroize_point(qrlbn_ecc_point_t * restrict tgt,
			 const qrlbn_modulus_data_t *mdp);


#ifdef	__cplusplus
}
#endif

#endif
