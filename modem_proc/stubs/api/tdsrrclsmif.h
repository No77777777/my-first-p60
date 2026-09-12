#ifndef TDSRRCLSMIF_H
#define TDSRRCLSMIF_H

#define  RRC_MAX_MCC_DIGITS                3
#define  RRC_MAX_GSMMAP_LAC_LENGTH         2   /* Length of LAC in bytes */

/* From wcdma\api\rrcmmif.h */
typedef struct
{
  /* Mobile Country Code - one digit per byte range 0..9)  */
  byte mcc[RRC_MAX_MCC_DIGITS];
  /* Number of MNC digits */
  byte num_mnc_digits;
  /* Mobile Network Code - one digit per byte (range 0..9) */
  byte mnc[RRC_MAX_MNC_DIGITS];
}rrc_plmn_identity_type;

typedef struct
{
  uint32 cell_id;
  uint8 cpid;
  uint16 freq;
  rrc_plmn_identity_type plmn_id;
  uint8 lac[RRC_MAX_GSMMAP_LAC_LENGTH];
} tdsrrcmeas_plmn_cell_info_struct_type;

#endif  /* TDSRRCLSMIF_H */


