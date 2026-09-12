# --------------------------------------------------------------------------- #
# File Name :: cxm_victim_tbl_translation.py
# Purpose   :: Converts the .xlsx file containing victim table information into
# corresponding C data structures.  This eliminates the need to load a binary
# file into NV to configure CxM.
# --------------------------------------------------------------------------- #

# Copyright (c) 2015 Qualcomm Technologies Incorporated.
# All Rights Reserved.
# Qualcomm Confidential and Proprietary

# Export of this technology or software is regulated by the U.S. Government.
# Diversion contrary to U.S. law prohibited.

# All ideas, data and information contained in or disclosed by
# this document are confidential and proprietary information of
# Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
# By accepting this material the recipient agrees that this material
# and the information contained therein are held in confidence and in
# trust and will not be used, copied, reproduced in whole or in part,
# nor its contents revealed in any manner to others without the express
# written permission of Qualcomm Technologies Incorporated.
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
#                         RETURN CODES
# --------------------------------------------------------------------------- #
# This script returns the following exit codes:
#   0) executed successfully
#   1) general error
#   2) problem with command line args
#   3) problem writing output file(s)

#==============================================================================
#
#                        EDIT HISTORY FOR MODULE
#
# This section contains comments describing changes made to the module.
# Notice that changes are listed in reverse chronological order.
#
# $Header: //components/rel/mcs.mpss/5.2/cxm/tools/cxm_victim_tbl_translation.py#1 $
#
# when       who    what, where, why
# --------   ---    -----------------------------------------------------------
# 09/14/15   cdh    readability improved, sanity checks added, namedtuples removed
# 09/01/15   cdh    more than two modem carriers, additional reasons
# 07/20/15   cdh    initial rev
#
#==============================================================================


# --------------------------------------------------------------------------- #
#                       IMPORT MODULES & DEFINITONS                           #
# --------------------------------------------------------------------------- #

from openpyxl import Workbook, load_workbook
from collections import namedtuple
import os
import sys
import fileinput

# --------------------------------------------------------------------------- #
#                       GLOBAL VARIABLES
# --------------------------------------------------------------------------- #
FIRST_DATA_ROW = 4 # The first xl row with data
DEFAULTS_ROW = 2 # The row where default values are placed in the xl file
col_num = {} # Dictionary indexed by column letter that returns the index
wlan_chan = {'freq_start':[], 'freq_stop':[]} # Freqs for possible wlan channels
op_dim = {} # Dictionary indexed by band number that returns operating dimension
group_glob = 1 # The group number used for carrier aggregation
CXM_COEX_VT_MAX_NV_ENTRIES = 8 # The number of entries that can come from NV
CXM_COEX_VT_MAX_ENTRIES = 255 # Total size of victim table

# --------------------------------------------------------------------------- #
#                       TARGET DICTIONARIES
# --------------------------------------------------------------------------- #
# These dictionaries allow us to translate from xl strings to the proper
# target value
wcn_tech_dict = {u'WLAN':'COEX_WIFI_TECH_V01', None:'COEX_WCN_TECH_MIN_ENUM_VAL_V01'}

wcn_mode_dict = {u'TX':'COEX_WLAN_CONN_MODE_V01', u'RX':'COEX_WLAN_CONN_MODE_V01',
                 u'RX|TX':'COEX_WLAN_CONN_MODE_V01', u'RX&TX':'COEX_WLAN_CONN_MODE_V01', 
                 u'SCAN':'COEX_WLAN_SCAN_MODE_V01', None:'0'}

wwan_dir_dict = {u'TX':'COEX_UL_BAND_DIRECTION_V01',
                 u'RX':'COEX_DL_BAND_DIRECTION_V01',
                 u'RX|TX':'(COEX_UL_BAND_DIRECTION_V01 | COEX_DL_BAND_DIRECTION_V01)',
                 u'RX&TX':'COEX_UL_BAND_DIRECTION_V01',
                 None:'0'}

wwan_tech_dict = {u'LTE':'COEX_LTE_TECH_V01', u'TDSCDMA':'COEX_TDSCDMA_TECH_V01',
                  u'GSM':'COEX_GSM_TECH_V01', u'1X':'COEX_ONEX_TECH_V01',
                  u'HDR':'COEX_HDR_TECH_V01', u'WCDMA':'COEX_WCDMA_TECH_V01',
                  u'GSM2':'COEX_GSM2_TECH_V01', u'GSM3':'COEX_GSM3_TECH_V01',
                  u'WCDMA2':'COEX_WCDMA2_TECH_V01', None:'COEX_WWAN_TECH_MIN_ENUM_VAL_V01'}

conflict_reason_dict = {u'Adjacent Bands':'COEX_CONFLICT_ADJ_BANDS',
                        u'SAWless':'COEX_CONFLICT_SAWLESS',
                        u'WWAN TX 2nd Harmonic':'COEX_CONFLICT_WWAN_TX_2ND_HARMONIC',
                        u'WWAN TX 3rd Harmonic':'COEX_CONFLICT_WWAN_TX_3RD_HARMONIC',
                        u'WWAN TX 7th Harmonic':'COEX_CONFLICT_WWAN_TX_7TH_HARMONIC',
                        u'WLAN_TX - WWAN_TX':'COEX_CONFLICT_WLAN_TX_WWAN_TX',
                        u'2*WWAN_TX - WLAN_TX':'COEX_CONFLICT_2WWAN_TX_WLAN_TX',
                        u'WLAN_TX - 2*WWAN_TX':'COEX_CONFLICT_WLAN_TX_2WWAN_TX',
                        u'WAN RX 3rd LO Harmonic':'COEX_CONFLICT_WWAN_RX_3RD_LO_HARMONIC',
                        u'2*WWAN_TX2 + WWAN_TX1':'COEX_CONFLICT_2WWAN_TX2_PLUS_WWAN_TX1',
                        u'WWAN_TX1 + WWAN_TX2 - WLAN_TX':'COEX_CONFLICT_WWAN_TX1_PLUS_WWAN_TX2_WLAN_TX',
                        u'WLAN_TX - WWAN_TX1 - WWAN_TX2':'COEX_CONFLICT_WLAN_TX_WWAN_TX1_WWAN_TX2',
                        u'WWAN_TX1 + WWAN_TX2':'COEX_CONFLICT_WWAN_TX1_PLUS_WWAN_TX2',
                        u'2*WWAN_TX1 + WWAN_TX2':'COEX_CONFLICT_2WWAN_TX1_PLUS_WWAN_TX2',
                        u'2*WWAN_TX1 - WWAN_TX2':'COEX_CONFLICT_2WWAN_TX1_WWAN_TX2'}


# --------------------------------------------------------------------------- #
#                       HELPER FUNCTIONS
# --------------------------------------------------------------------------- #
# function to translate column label to number
def col_to_num(char_code):
    nchars = len(char_code)
    ASCII_A = 65
    result = 0
    for i in range(0, nchars):
        result += (26 ** (nchars - 1 - i)) * (ord(char_code[i]) - ASCII_A + 1)

    return result

# function to determine the number of columns in a spreadsheet
def ncols_worksheet(wrksht):
    char_code = wrksht.max_col
    return col_to_num(char_code)

# function to help translate from band number to operating dimension
def init_operating_dim_dict(bands_ws):
    print 'Mapping bands to operating dimensions...'
    op_dim[u'LTE'] = {}
    op_dim[u'WCDMA'] = {}
    op_dim[u'1X'] = {}
    op_dim[u'HDR'] = {}
    op_dim[u'TDSCDMA'] = {}
    op_dim[u'GSM'] = {}
    b0_row = 3
    b41_row = 44
    for i in range(b0_row, b41_row + 1):
        if (bands_ws.cell(row=i,column=col_num['D'])).value == 'x':
            if (bands_ws.cell(row=i,column=col_num['C'])).value == u'FDD':
                op_dim[u'LTE'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_FDD_V01'
            elif (bands_ws.cell(row=i,column=col_num['C'])).value == u'TDD':
                op_dim[u'LTE'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_TDD_V01'
        if (bands_ws.cell(row=i,column=col_num['E'])).value == 'x':
            if (bands_ws.cell(row=i,column=col_num['C'])).value == u'FDD':
                op_dim[u'WCDMA'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_FDD_V01'
            elif (bands_ws.cell(row=i,column=col_num['C'])).value == u'TDD':
                op_dim[u'WCDMA'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_TDD_V01'
        if (bands_ws.cell(row=i,column=col_num['F'])).value == 'x':
            if (bands_ws.cell(row=i,column=col_num['C'])).value == u'FDD':
                op_dim[u'1X'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_FDD_V01'
            elif (bands_ws.cell(row=i,column=col_num['C'])).value == u'TDD':
                op_dim[u'1X'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_TDD_V01' 
        if (bands_ws.cell(row=i,column=col_num['G'])).value == 'x':
            if (bands_ws.cell(row=i,column=col_num['C'])).value == u'FDD':
                op_dim[u'HDR'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_FDD_V01'
            elif (bands_ws.cell(row=i,column=col_num['C'])).value == u'TDD':
                op_dim[u'HDR'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_TDD_V01'
        if (bands_ws.cell(row=i,column=col_num['H'])).value == 'x':
            if (bands_ws.cell(row=i,column=col_num['C'])).value == u'FDD':
                op_dim[u'TDSCDMA'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_FDD_V01'
            elif (bands_ws.cell(row=i,column=col_num['C'])).value == u'TDD':
                op_dim[u'TDSCDMA'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_TDD_V01'
        if (bands_ws.cell(row=i,column=col_num['I'])).value == 'x':
            if (bands_ws.cell(row=i,column=col_num['C'])).value == u'FDD':
                op_dim[u'GSM'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_FDD_V01'
            elif (bands_ws.cell(row=i,column=col_num['C'])).value == u'TDD':
                op_dim[u'GSM'][u'B'+str(i-b0_row)] = 'COEX_TECH_OPERATING_DIMENSION_TDD_V01'
            
                   
# function to add a new victim table entry
def add_vt_entry(coex_victim_tbl, ncols):
    coex_victim_tbl.append({})
    index = len(coex_victim_tbl) - 1
    coex_victim_tbl[index]['wwan'] = {
        'direction':0,
        'operating_dim':0,
        'tech':0,
        'freq_start':0,
        'freq_stop':0
    }
    coex_victim_tbl[index]['wcn'] = {
        'mode':0,
        'tech':0,
        'freq_start':0,
        'freq_stop':0
    }
    coex_victim_tbl[index]['mdm_policy'] = 0
    coex_victim_tbl[index]['wcn_policy'] = 0
    coex_victim_tbl[index]['wcn_params'] = {
        'imd_protection_params':
        {
            'block_tx':0,
            'static_backoff_tx_pwr':0
        },
        'bt_afh_update_len':0,
        'bt_afh_update_arr':
        {
            'bt_afh_update[0]':
            {
                'start_bt_channel':0,
                'end_bt_channel':0
            },
            'bt_afh_update[1]':
            {
                 'start_bt_channel':0,
                'end_bt_channel':0
            },
            'bt_afh_update[2]':
            {
                'start_bt_channel':0,
                'end_bt_channel':0
            },
            'bt_afh_update[3]':
            {
                'start_bt_channel':0,
                'end_bt_channel':0
            }
        },
    }
    coex_victim_tbl[index]['mdm_params'] = {
        'wcn_prio':
        {
            'tx_thlds':
            {
                'tx_pwr_thld':0,
                'tx_bw_thld':0
            },
            'frame_denials':
            {
                'duty_cycle_params':
                {
                    'max_allowed_frame_denials':0,
                    'frame_denial_window':0
                },
                'max_cont_denials':0
            }
        },
        'tx_adv':
        {
            'rb_thresh_len':0,
            'rb_thresh_arr':
            {
                'rb_thresh[0]':
                {
                    'start':0,
                    'end':0
                },
                'rb_thresh[1]':
                {
                    'start':0,
                    'end':0
                },
                'rb_thresh[2]':
                {
                    'start':0,
                    'end':0
                },
                'rb_thresh[3]':
                {
                    'start':0,
                    'end':0
                },
                'rb_thresh[4]':
                {
                    'start':0,
                    'end':0
                },
                'rb_thresh[5]':
                {
                    'start':0,
                    'end':0
                }
            },
            'tx_power_thresh':0
        },
        'pwr_lmt':
        {
            'power':0
        }
    }
    coex_victim_tbl[index]['group'] = 0
    coex_victim_tbl[index]['xl_row_num'] = 0
    coex_victim_tbl[index]['reason'] = 0

# Function to translate all interesting column labels to numbers in a dictionary
def init_col_num_tbl():
    col_num['A'] = col_to_num('A')
    col_num['B'] = col_to_num('B')
    col_num['C'] = col_to_num('C')
    col_num['D'] = col_to_num('D')
    col_num['E'] = col_to_num('E')
    col_num['F'] = col_to_num('F')
    col_num['G'] = col_to_num('G')
    col_num['H'] = col_to_num('H')
    col_num['I'] = col_to_num('I')
    col_num['J'] = col_to_num('J')
    col_num['K'] = col_to_num('K')
    col_num['L'] = col_to_num('L')
    col_num['M'] = col_to_num('M')
    col_num['N'] = col_to_num('N')
    col_num['P'] = col_to_num('P')
    col_num['Q'] = col_to_num('Q')
    col_num['R'] = col_to_num('R')
    col_num['S'] = col_to_num('S')
    col_num['T'] = col_to_num('T')
    col_num['V'] = col_to_num('V')
    col_num['W'] = col_to_num('W')
    col_num['X'] = col_to_num('X')
    col_num['Y'] = col_to_num('Y')
    col_num['Z'] = col_to_num('Z')
    col_num['AO'] = col_to_num('AO')
    col_num['AP'] = col_to_num('AP')
    col_num['AQ'] = col_to_num('AQ')
    col_num['AR'] = col_to_num('AR')
    col_num['AS'] = col_to_num('AS')
    col_num['AT'] = col_to_num('AT')
    col_num['AU'] = col_to_num('AU')
    col_num['AV'] = col_to_num('AV')
    col_num['AW'] = col_to_num('AW')
    col_num['AX'] = col_to_num('AX')
    col_num['AY'] = col_to_num('AY')
    col_num['AZ'] = col_to_num('AZ')
    col_num['BA'] = col_to_num('BA')
    col_num['BB'] = col_to_num('BB')
    col_num['BC'] = col_to_num('BC')
    col_num['BD'] = col_to_num('BD')
    col_num['BE'] = col_to_num('BE')
    col_num['BF'] = col_to_num('BF')
    col_num['BG'] = col_to_num('BG')
    col_num['BH'] = col_to_num('BH')
    col_num['BI'] = col_to_num('BI')
    col_num['BJ'] = col_to_num('BJ')
    col_num['BK'] = col_to_num('BK')
    col_num['BL'] = col_to_num('BL')
    col_num['BM'] = col_to_num('BM')
    col_num['BN'] = col_to_num('BN')
    col_num['BO'] = col_to_num('BO')
    col_num['BP'] = col_to_num('BP')
    col_num['BQ'] = col_to_num('BQ')
    col_num['BR'] = col_to_num('BR')
    col_num['BS'] = col_to_num('BS')
    col_num['BT'] = col_to_num('BT')
    col_num['BU'] = col_to_num('BU')    
    
# Function to initialize a table of all possible frequencies associated with WLAN channels.
# The table is used as a sanity check to help catch typos in the excel spreadsheet.
def init_wlan_chan_tbl(wlan_chan_ws):
    first_row = 3
    last_row = 40
    for i in range(first_row, last_row + 1):
        wlan_chan['freq_start'].append((str.split(str((wlan_chan_ws.cell(row=i,column=col_num['C'])).value), ' '))[0])
        wlan_chan['freq_stop'].append((str.split(str((wlan_chan_ws.cell(row=i,column=col_num['E'])).value), ' '))[0])
    
# Function to populate a victim table entry
# ws is the main victim table worksheet
# coex_victim_tbl is the victim table dictionary
# index is the victim table entry number (indexed from 0)
# irow is the row in ws that we want to parse
# WAN_indx is 2 if we want to use the R2 entry, 3 for the R3 entry, 4 for the R4 entry, etc.
# WAN_dir_option is 0 for single-frequency entry, 1 for first range in UL/DL pair, and 2 for second range in UL/DL pair
def populate_vt_entry(ws, coex_victim_tbl, index, irow, WAN_indx, WAN_dir_option):
    coex_victim_tbl[index]['wcn']['tech'] = wcn_tech_dict[(ws.cell(row=irow,column=col_num['B'])).value]
    # Sanity check for WLAN frequencies
    if 'COEX_WIFI_TECH_V01' == coex_victim_tbl[index]['wcn']['tech']:
        if str((ws.cell(row=irow,column=col_num['F'])).value) not in wlan_chan['freq_start']:
            print [x for x in wlan_chan['freq_start']]
            sys.exit("Error: ChL WLAN freq does not match any WLAN channel definition on row " + str(irow))
        if str((ws.cell(row=irow,column=col_num['G'])).value) not in wlan_chan['freq_stop']:
            sys.exit("Error: ChH WLAN freq does not match any WLAN channel definition on row " + str(irow))
    # Convert wcn frequencies from MHz to kHz
    coex_victim_tbl[index]['wcn']['freq_start'] = (ws.cell(row=irow,column=col_num['F'])).value * 1000
    coex_victim_tbl[index]['wcn']['freq_stop'] = (ws.cell(row=irow,column=col_num['G'])).value * 1000
    # Sanity check: starting freq should be lower than ending freq
    if coex_victim_tbl[index]['wcn']['freq_start'] >= coex_victim_tbl[index]['wcn']['freq_stop']:
        sys.exit("Error: Starting WCN freq >= Ending WCN freq for row " + str(irow))
    coex_victim_tbl[index]['wcn']['mode'] = wcn_mode_dict[(ws.cell(row=irow,column=col_num['H'])).value]
    # Depending on WAN_indx, use the appropriate columns
    WAN_indx_offset = (WAN_indx - 2) * 6
    coex_victim_tbl[index]['wwan']['tech'] = wwan_tech_dict[(ws.cell(row=irow,column=col_num['J']+WAN_indx_offset)).value]
    coex_victim_tbl[index]['wwan']['operating_dim'] = op_dim[(ws.cell(row=irow,column=col_num['J']+WAN_indx_offset)).value]\
                                                        [(ws.cell(row=irow,column=col_num['K']+WAN_indx_offset)).value]
    if WAN_dir_option == 0:
        # Entry with one frequency range (direction is treated as is)
        coex_victim_tbl[index]['wwan']['freq_start'] = (ws.cell(row=irow,column=col_num['L']+WAN_indx_offset)).value * 1000
        coex_victim_tbl[index]['wwan']['freq_stop'] = (ws.cell(row=irow,column=col_num['M']+WAN_indx_offset)).value * 1000
        # Sanity check: starting freq should be lower than ending freq
        if coex_victim_tbl[index]['wwan']['freq_start'] >= coex_victim_tbl[index]['wwan']['freq_stop']:
            sys.exit("Error: Starting WWAN freq >= Ending WWAN freq for row " + str(irow))
        coex_victim_tbl[index]['wwan']['direction'] = wwan_dir_dict[(ws.cell(row=irow,column=col_num['N']+WAN_indx_offset)).value]
    elif WAN_dir_option == 1:
        # Entry with two frequency ranges, one for DL and one for UL, where we are now handling the first frequency range
        # Convert wwan frequencies from MHz to kHz
        coex_victim_tbl[index]['wwan']['freq_start'] = (str((ws.cell(row=irow,column=col_num['L']+WAN_indx_offset)).value)).split('/')[0] + '000'
        coex_victim_tbl[index]['wwan']['freq_stop'] = (str((ws.cell(row=irow,column=col_num['M']+WAN_indx_offset)).value)).split('/')[0] + '000'
        if '|' in str((ws.cell(row=irow,column=col_num['N'])).value):
          coex_victim_tbl[index]['wwan']['direction'] = wwan_dir_dict[(str((ws.cell(row=irow,column=col_num['N']+WAN_indx_offset)).value)).split('|')[0]]
        else:
          coex_victim_tbl[index]['wwan']['direction'] = wwan_dir_dict[(str((ws.cell(row=irow,column=col_num['N']+WAN_indx_offset)).value)).split('&')[0]]
        # Sanity check: starting freq should be lower than ending freq
        if (ws.cell(row=irow,column=col_num['L']+WAN_indx_offset)).value >= (ws.cell(row=irow,column=col_num['M']+WAN_indx_offset)).value:
            sys.exit("Error: Starting WWAN freq >= Ending WWAN freq for RX in row " + str(irow))
    elif WAN_dir_option == 2:
        # Entry with two frequency ranges, one for DL and one for UL, where we are now handling the second frequency range
        # Convert wwan frequencies from MHz to kHz
        coex_victim_tbl[index]['wwan']['freq_start'] = (str((ws.cell(row=irow,column=col_num['L']+WAN_indx_offset)).value)).split('/')[1] + '000'
        coex_victim_tbl[index]['wwan']['freq_stop'] = (str((ws.cell(row=irow,column=col_num['M']+WAN_indx_offset)).value)).split('/')[1] + '000'
        if '|' in str((ws.cell(row=irow,column=col_num['N'])).value):
          coex_victim_tbl[index]['wwan']['direction'] = wwan_dir_dict[(str((ws.cell(row=irow,column=col_num['N']+WAN_indx_offset)).value)).split('|')[1]]
        else:
          coex_victim_tbl[index]['wwan']['direction'] = wwan_dir_dict[(str((ws.cell(row=irow,column=col_num['N']+WAN_indx_offset)).value)).split('&')[1]]
        # Sanity check: starting freq should be lower than ending freq
        if (ws.cell(row=irow,column=col_num['L']+WAN_indx_offset)).value >= (ws.cell(row=irow,column=col_num['M']+WAN_indx_offset)).value:
            sys.exit("Error: Starting WWAN freq >= Ending WWAN freq for TX in row " + str(irow))

    coex_victim_tbl[index]['reason'] = conflict_reason_dict[(ws.cell(row=irow,column=col_num['AO'])).value]
    nonzero = False
    mdm_policy = '('
    if (ws.cell(row=irow,column=col_num['AP'])).value == 'x':
        nonzero = True
        mdm_policy += 'COEX_PCM_TOGGLE_FRAME_SYNC_V01'
    if (ws.cell(row=irow,column=col_num['AQ'])).value == 'x':
        if nonzero:
            mdm_policy += '|'
        mdm_policy += 'COEX_PCM_TOGGLE_TX_ACTIVE_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['AR'])).value == 'x':
        if nonzero:
            mdm_policy += '|'
        mdm_policy += 'COEX_PCM_TOGGLE_RX_PRIORITY_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['AS'])).value == 'x':
        if nonzero:
            mdm_policy += '|'
        mdm_policy += 'COEX_PCM_REACT_TO_WCN_PRIORITY_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['AT'])).value == 'x':
        if nonzero:
            mdm_policy += '|'
        mdm_policy += 'COEX_PCM_SEND_WCI2_TYPE3_INACT_DURN_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['AU'])).value == 'x':
        if nonzero:
            mdm_policy += '|'
        mdm_policy += 'COEX_PCM_SEND_WCI2_TYPE6_TX_ADV_NOTICE_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['AV'])).value == 'x':
        if nonzero:
            mdm_policy += '|'
        mdm_policy += 'COEX_PCM_ENFORCE_CONTROLLER_TX_POWER_LIMIT_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['AW'])).value == 'x':
        if nonzero:
            mdm_policy += '|'
        mdm_policy += 'COEX_PCM_REACT_TO_WCI2_TYPE6_TX_POWER_LIMIT_V01'
        nonzero = True
    if nonzero:
        mdm_policy += ')'
    else:
        mdm_policy = '0'
    coex_victim_tbl[index]['mdm_policy'] = mdm_policy    
        
    nonzero = False
    wcn_policy = '('
    if (ws.cell(row=irow,column=col_num['AX'])).value == 'x':
        nonzero = True
        wcn_policy += 'COEX_PCM_ENFORCE_WWAN_RX_IMD_PROTECTION_V01'
    if (ws.cell(row=irow,column=col_num['AY'])).value == 'x':
        if nonzero:
            wcn_policy += '|'
        wcn_policy += 'COEX_PCM_ENFORCE_DYNAMIC_WLAN_POWER_BACKOFF_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['AZ'])).value == 'x':
        if nonzero:
            wcn_policy += '|'
        wcn_policy += 'COEX_PCM_ENFORCE_BT_AFH_UPDATE_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['BA'])).value == 'x':
        if nonzero:
            wcn_policy += '|'
        wcn_policy += 'COEX_PCM_ENFORCE_WLAN_CHANNEL_AVOIDANCE_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['BB'])).value == 'x':
        if nonzero:
            wcn_policy += '|'
        wcn_policy += 'COEX_PCM_ENFORCE_TDM_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['BC'])).value == 'x':
        if nonzero:
            wcn_policy += '|'
        wcn_policy += 'COEX_PCM_ENFORCE_MAX_WWAN_PROTECTION_V01'
        nonzero = True
    if (ws.cell(row=irow,column=col_num['BD'])).value == 'x':
        if nonzero:
            wcn_policy += '|'
        wcn_policy += 'COEX_PCM_ENFORCE_TDM_STICKY_V01'
        nonzero = True
    if nonzero:
        wcn_policy += ')'
    else:
        wcn_policy = '0'
    coex_victim_tbl[index]['wcn_policy'] = wcn_policy
    
    if (ws.cell(row=irow,column=col_num['BE'])).value == None:
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['tx_thlds']['tx_pwr_thld'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BE'])).value).replace(' dBm','')
    else:        
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['tx_thlds']['tx_pwr_thld'] = ((ws.cell(row=irow,column=col_num['BE'])).value).replace(' dBm','')
    if (ws.cell(row=irow,column=col_num['BF'])).value == None:
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['tx_thlds']['tx_bw_thld'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BF'])).value).replace(' RB','')
    else:
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['tx_thlds']['tx_bw_thld'] = ((ws.cell(row=irow,column=col_num['BF'])).value).replace(' RB','')
    if (ws.cell(row=irow,column=col_num['BG'])).value == None:
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['frame_denials']['max_cont_denials'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BG'])).value).replace(' sf','')
    else:
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['frame_denials']['max_cont_denials'] = ((ws.cell(row=irow,column=col_num['BG'])).value).replace(' sf','')
    if (ws.cell(row=irow, column=col_num['BH'])).value == None:
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['frame_denials']['duty_cycle_params']['max_allowed_frame_denials'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BH'])).value).replace(' sf','')
    else:
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['frame_denials']['duty_cycle_params']['max_allowed_frame_denials'] = ((ws.cell(row=irow,column=col_num['BH'])).value).replace(' sf','')
    if (ws.cell(row=irow,column=col_num['BI'])).value == None:
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['frame_denials']['duty_cycle_params']['frame_denial_window'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BI'])).value).replace(' sf','')
    else:
        coex_victim_tbl[index]['mdm_params']['wcn_prio']['frame_denials']['duty_cycle_params']['frame_denial_window'] = ((ws.cell(row=irow,column=col_num['BI'])).value).replace(' sf','')
    if (ws.cell(row=irow,column=col_num['BJ'])).value == None:
        coex_victim_tbl[index]['mdm_params']['tx_adv']['tx_power_thresh'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BJ'])).value).replace(' dBm','')
    else:
        coex_victim_tbl[index]['mdm_params']['tx_adv']['tx_power_thresh'] = ((ws.cell(row=irow,column=col_num['BJ'])).value).replace(' dBm','')
    # Assumption here is that both the min and max will be filled out in pairs, starting
    # with the first TX Adv RB min and max entries
    if (ws.cell(row=irow,column=col_num['BK'])).value == None:
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_len'] = 1
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[0]']['start'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BK'])).value).replace(' RB','')
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[0]']['end'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BL'])).value).replace(' RB','')
    elif (ws.cell(row=irow,column=col_num['BM'])).value == None:
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_len'] = 1
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[0]']['start'] = ((ws.cell(row=irow,column=col_num['BK'])).value).replace(' RB','')
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[0]']['end'] = ((ws.cell(row=irow,column=col_num['BL'])).value).replace(' RB','')
    else:
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_len'] = 2
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[0]']['start'] = ((ws.cell(row=irow,column=col_num['BK'])).value).replace(' RB','')
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[0]']['end'] = ((ws.cell(row=irow,column=col_num['BL'])).value).replace(' RB','')
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[1]']['start'] = ((ws.cell(row=irow,column=col_num['BM'])).value).replace(' RB','')
        coex_victim_tbl[index]['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[1]']['end'] = ((ws.cell(row=irow,column=col_num['BN'])).value).replace(' RB','')
    if (ws.cell(row=irow,column=col_num['BO'])).value == None:
        coex_victim_tbl[index]['mdm_params']['pwr_lmt']['power'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BO'])).value).replace(' dBm','')
    else:
        coex_victim_tbl[index]['mdm_params']['pwr_lmt']['power'] = ((ws.cell(row=irow,column=col_num['BO'])).value).replace(' dBm','')
    # Assumption here is that both the start and stop will be filled out in pairs, starting
    # with the first BT AFH Start and Stop entries
    if (ws.cell(row=irow,column=col_num['BP'])).value == None:
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_len'] = 1
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_arr']['bt_afh_update[0]']['start_bt_channel'] = (ws.cell(row=DEFAULTS_ROW,column=col_num['BP'])).value
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_arr']['bt_afh_update[0]']['end_bt_channel'] = (ws.cell(row=DEFAULTS_ROW,column=col_num['BQ'])).value
    elif (ws.cell(row=irow,column=col_num['BR'])).value == None:
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_len'] = 1
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_arr']['bt_afh_update[0]']['start_bt_channel'] = (ws.cell(row=irow,column=col_num['BP'])).value
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_arr']['bt_afh_update[0]']['end_bt_channel'] = (ws.cell(row=irow,column=col_num['BQ'])).value
    else:
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_len'] = 2 
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_arr']['bt_afh_update[0]']['start_bt_channel'] = (ws.cell(row=irow,column=col_num['BP'])).value
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_arr']['bt_afh_update[0]']['end_bt_channel'] = (ws.cell(row=irow,column=col_num['BQ'])).value
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_arr']['bt_afh_update[1]']['start_bt_channel'] = (ws.cell(row=irow,column=col_num['BR'])).value
        coex_victim_tbl[index]['wcn_params']['bt_afh_update_arr']['bt_afh_update[1]']['end_bt_channel'] = (ws.cell(row=irow,column=col_num['BS'])).value
    if (ws.cell(row=irow,column=col_num['BT'])).value == None:
        coex_victim_tbl[index]['wcn_params']['imd_protection_params']['block_tx'] = (ws.cell(row=DEFAULTS_ROW,column=col_num['BT'])).value
    else:
        coex_victim_tbl[index]['wcn_params']['imd_protection_params']['block_tx'] = (ws.cell(row=irow,column=col_num['BT'])).value
    if (ws.cell(row=irow,column=col_num['BU'])).value == None:
        coex_victim_tbl[index]['wcn_params']['imd_protection_params']['static_backoff_tx_pwr'] = ((ws.cell(row=DEFAULTS_ROW,column=col_num['BU'])).value).replace(' dBm','')
    else:
        coex_victim_tbl[index]['wcn_params']['imd_protection_params']['static_backoff_tx_pwr'] = ((ws.cell(row=irow,column=col_num['BU'])).value).replace(' dBm','')
    coex_victim_tbl[index]['xl_row_num'] = irow
	
    return 0

# Function to create victim table dictionary from spreadsheet
def parse_WS(ws, ncols):
    global group_glob

    print "Parsing xl worksheet..."
    coex_victim_tbl = []

    # Find the last data row in the xl spreadsheet
    final_row = FIRST_DATA_ROW
    ROW_LIMIT = ws.get_highest_row() # Certainly no data past the highest row
    while (ws.cell(row = final_row, column = col_num['B'])).value != None or\
          (ws.cell(row = final_row, column = col_num['A'])).value != None:
        final_row += 1
        if final_row > ROW_LIMIT:
            break
    print "Rows " + str(FIRST_DATA_ROW) + " through " + str(final_row - 1) + " may contain victim table entries."
    
    # Iterate through worksheet
    for i in range(FIRST_DATA_ROW, final_row):
        if len(coex_victim_tbl) >= (CXM_COEX_VT_MAX_ENTRIES - CXM_COEX_VT_MAX_NV_ENTRIES):
            break
        if (ws.cell(row = i, column = col_num['A'])).value == 'x' or\
           (ws.cell(row = i, column = col_num['A'])).value == 'X':
            continue
        else:            
            # Figure out how many entries to generate from spreadsheet row i
            # If R2 Rx/Tx frequencies are both listed, add another entry to the same group
            # If R3 is not blank, add at least one other entry to the same group
            # If R3 Rx/Tx frequencies are both listed, add two entries, otherwise add one, etc.

            # Keep track of whether or not group number needs to be incremented
            incr_group = False
            
            if '/' in str((ws.cell(row = i, column = col_num['L'])).value) or '&' in str((ws.cell(row = i, column = col_num['L'])).value):
                # R2 Rx entry
                add_vt_entry(coex_victim_tbl, ncols)
                index = len(coex_victim_tbl) - 1
                populate_vt_entry(ws, coex_victim_tbl, index, i, 2, 1)					
                coex_victim_tbl[index]['group'] = group_glob
                # R2 Tx entry
                add_vt_entry(coex_victim_tbl, ncols)
                index = len(coex_victim_tbl) - 1
                populate_vt_entry(ws, coex_victim_tbl, index, i, 2, 2)
                coex_victim_tbl[index]['group'] = group_glob
                incr_group = True
            else:
                # R2 entry (may be Rx or Tx)
                add_vt_entry(coex_victim_tbl, ncols)
                index = len(coex_victim_tbl) - 1
                populate_vt_entry(ws, coex_victim_tbl, index, i, 2, 0)
            
            if (ws.cell(row = i, column = col_num['R'])).value != None:
                # Make sure the previous R2 entry group number is correct
                coex_victim_tbl[index]['group'] = group_glob
                incr_group = True
                
                if '/' in str((ws.cell(row = i, column = col_num['R'])).value) or '&' in str((ws.cell(row = i, column = col_num['R'])).value):
                    # R3 Rx entry
                    add_vt_entry(coex_victim_tbl, ncols)
                    index = len(coex_victim_tbl) - 1
                    populate_vt_entry(ws, coex_victim_tbl, index, i, 3, 1)
                    coex_victim_tbl[index]['group'] = group_glob
                    # R3 Tx entry
                    add_vt_entry(coex_victim_tbl, ncols)
                    index = len(coex_victim_tbl) - 1
                    populate_vt_entry(ws, coex_victim_tbl, index, i, 3, 2)
                    coex_victim_tbl[index]['group'] = group_glob
                else:
                    # R3 entry (may be Rx or Tx)
                    add_vt_entry(coex_victim_tbl, ncols)
                    index = len(coex_victim_tbl) - 1
                    populate_vt_entry(ws, coex_victim_tbl, index, i, 3, 0)
                    coex_victim_tbl[index]['group'] = group_glob

            if (ws.cell(row = i, column = col_num['X'])).value != None:
                if '/' in str((ws.cell(row = i, column = col_num['X'])).value) or '&' in str((ws.cell(row = i, column = col_num['X'])).value):
                    # R4 Rx entry
                    add_vt_entry(coex_victim_tbl, ncols)
                    index = len(coex_victim_tbl) - 1
                    populate_vt_entry(ws, coex_victim_tbl, index, i, 4, 1)
                    coex_victim_tbl[index]['group'] = group_glob
                    # R4 Tx entry
                    add_vt_entry(coex_victim_tbl, ncols)
                    index = len(coex_victim_tbl) - 1
                    populate_vt_entry(ws, coex_victim_tbl, index, i, 4, 2)
                    coex_victim_tbl[index]['group'] = group_glob
                else:
                    # R4 entry (may be Rx or Tx)
                    add_vt_entry(coex_victim_tbl, ncols)
                    index = len(coex_victim_tbl) - 1
                    populate_vt_entry(ws, coex_victim_tbl, index, i, 4, 0)
                    coex_victim_tbl[index]['group'] = group_glob
                    
                        
            # Increment the group ID
            if incr_group:
                group_glob += 1

    return coex_victim_tbl


# Function to format the .c and .h file strings
def format_file_strings(vict_tbl, CXM_COEX_VT_VALID_STATIC_SIZE):
    print "Building formatted string..."
    
    h_file_preamble = '''#ifndef __COEX_VICTIM_TABLE_H__
#define __COEX_VICTIM_TABLE_H__
/*!
  @file
  coex_victim_table.h

  @brief
  Coex service victim table and configuration

*/

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              C O E X _ V I C T I M _ T A B L E . H

GENERAL DESCRIPTION
  This file defines coex service configuration data structures.

  Copyright (c) 2015 Qualcomm Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


  
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with tool victim_tbl_translation.py */

'''
    h_file_content = '#define CXM_COEX_VT_MAX_NV_ENTRIES     ((uint32)' + str(CXM_COEX_VT_MAX_NV_ENTRIES) +\
                     ')\n#define CXM_COEX_VT_MAX_ENTRIES        ((uint32)' + str(CXM_COEX_VT_MAX_ENTRIES) +\
                     ')\n#define CXM_COEX_VT_VALID_STATIC_SIZE  ((uint32)' + str(CXM_COEX_VT_VALID_STATIC_SIZE) +\
                     ')\n\nextern coex_conflict_type coex_victim_tbl[CXM_COEX_VT_MAX_ENTRIES];'

    c_file_preamble = '''/*!
  @file
  coex_victim_table.c

  @brief
  Coex service victim table and configuration

*/

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              C O E X _ V I C T I M _ T A B L E . C

GENERAL DESCRIPTION
  This file defines coex service configuration data structures.

  Copyright (c) 2015 Qualcomm Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


  
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with tool victim_tbl_translation.py */

'''
    
    includes = '''#include <coexistence_service_v01.h>
#include "coex_algos.h"
#include "cxm_intf_types.h"
#include "coex_victim_table.h"

'''
    pre_struct_str = '\n\ncoex_conflict_type coex_victim_tbl[CXM_COEX_VT_MAX_ENTRIES] =\n{'

    struct_str = ""
    i = 0
    for c in vict_tbl:
        struct_str += '''
  {{ /*coex_victim_table[{0}]*/
    /* wwan */
    {{
      /* direction */ {1},
      /* operating_dim */ {2},
      /* tech */ {3},
      /* freq_start */ {4},
      /* freq_stop */ {5},
    }},
    /* wcn */
    {{
      /* mode */ {6},
      /* tech */ {7},
      /* freq_start */ {8},
      /* freq_stop */ {9},
    }},
    /* mdm_policy */ {10},
    /* wcn_policy */ {11},
    /* wcn_params */
    {{
      /* imd_protection_params */
      {{
        /* block_tx */ {12},
        /* static_backoff_tx_pwr */ {13},
      }},
      /* bt_afh_update_len */ {14},
      /* bt_afh_update[COEX_MAX_BT_AFH_CHANNEL_RANGE_V01] */
      {{
        /* bt_afh_update[0] */
        {{
          /* start_bt_channel */ {15},
          /* end_bt_channel */ {16},
        }},
        /* bt_afh_update[1] */
        {{
          /* start_bt_channel */ {17},
          /* end_bt_channel */ {18},
        }},
        /* bt_afh_update[2] */
        {{
          /* start_bt_channel */ {19},
          /* end_bt_channel */ {20},
        }},
        /* bt_afh_update[3] */
        {{
          /* start_bt_channel */ {21},
          /* end_bt_channel */ {22},
        }},
      }},
    }},
    /* mdm_params */
    {{
      /* wcn_prio */
      {{
        /* tx_thlds */
        {{
          /* tx_pwr_thld */ {23},
          /* tx_bw_thld */ {24},
        }},
        /* frame_denials */
        {{
          /* duty_cycle_params */
          {{
            /* max_allowed_frame_denials */ {25},
            /* frame_denial_window */ {26},
          }},
          /* max_cont_denials */ {27},
        }},
      }},
      /* tx_adv */
      {{
        /* rb_thresh_len */ {28},
        /* rb_thresh[COEX_MAX_RB_THRES_PER_CONFLICT_V01] */
        {{
          /* rb_thresh[0] */
          {{
            /* start */ {29},
            /* end */ {30},
          }},
          /* rb_thresh[1] */
          {{
            /* start */ {31},
            /* end */ {32},
          }},
          /* rb_thresh[2] */
          {{
            /* start */ {33},
            /* end */ {34},
          }},
          /* rb_thresh[3] */
          {{
            /* start */ {35},
            /* end */ {36},
          }},
          /* rb_thresh[4] */
          {{
            /* start */ {37},
            /* end */ {38},
          }},
          /* rb_thresh[5] */
          {{
            /* start */ {39},
            /* end */ {40},
          }},
        }},
        /* tx_power_thresh */ {41},
      }},
      /* pwr_lmt */
      {{
        /* power */ {42},
      }},
    }},
    /* group */ {43},
    /* xl_row_num */ {44},
    /* reason */ {45},
  }},'''.format(i, c['wwan']['direction'], c['wwan']['operating_dim'], c['wwan']['tech'],
               c['wwan']['freq_start'], c['wwan']['freq_stop'], c['wcn']['mode'],
               c['wcn']['tech'], c['wcn']['freq_start'], c['wcn']['freq_stop'],
               c['mdm_policy'], c['wcn_policy'], c['wcn_params']['imd_protection_params']['block_tx'],
               c['wcn_params']['imd_protection_params']['static_backoff_tx_pwr'],
               c['wcn_params']['bt_afh_update_len'],
               c['wcn_params']['bt_afh_update_arr']['bt_afh_update[0]']['start_bt_channel'],
               c['wcn_params']['bt_afh_update_arr']['bt_afh_update[0]']['end_bt_channel'],
               c['wcn_params']['bt_afh_update_arr']['bt_afh_update[1]']['start_bt_channel'],
               c['wcn_params']['bt_afh_update_arr']['bt_afh_update[1]']['end_bt_channel'],
               c['wcn_params']['bt_afh_update_arr']['bt_afh_update[2]']['start_bt_channel'],
               c['wcn_params']['bt_afh_update_arr']['bt_afh_update[2]']['end_bt_channel'],
               c['wcn_params']['bt_afh_update_arr']['bt_afh_update[3]']['start_bt_channel'],
               c['wcn_params']['bt_afh_update_arr']['bt_afh_update[3]']['end_bt_channel'],
               c['mdm_params']['wcn_prio']['tx_thlds']['tx_pwr_thld'],
               c['mdm_params']['wcn_prio']['tx_thlds']['tx_bw_thld'],
               c['mdm_params']['wcn_prio']['frame_denials']['duty_cycle_params']['max_allowed_frame_denials'],
               c['mdm_params']['wcn_prio']['frame_denials']['duty_cycle_params']['frame_denial_window'],
               c['mdm_params']['wcn_prio']['frame_denials']['max_cont_denials'],
               c['mdm_params']['tx_adv']['rb_thresh_len'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[0]']['start'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[0]']['end'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[1]']['start'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[1]']['end'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[2]']['start'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[2]']['end'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[3]']['start'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[3]']['end'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[4]']['start'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[4]']['end'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[5]']['start'],
               c['mdm_params']['tx_adv']['rb_thresh_arr']['rb_thresh[5]']['end'],
               c['mdm_params']['tx_adv']['tx_power_thresh'],
               c['mdm_params']['pwr_lmt']['power'],
               c['group'],
               c['xl_row_num'],
               c['reason'])
        i += 1
			   
    # Get rid of "None"
    struct_str = struct_str.replace('None', '0')
        
    h_file_end_str = '\n\n#endif /* __COEX_VICTIM_TABLE_H__ */'
    c_file_end_str = '\n};'
    c_file_str = c_file_preamble + includes +\
                 pre_struct_str + struct_str + c_file_end_str
    h_file_str = h_file_preamble + h_file_content + h_file_end_str

    return c_file_str, h_file_str

# --------------------------------------------------------------------------- #
#                       MAIN
# --------------------------------------------------------------------------- #
def main(argv):
    if CXM_COEX_VT_MAX_NV_ENTRIES > CXM_COEX_VT_MAX_ENTRIES:
        print "Error: NV LIMITED TO " + str(CXM_COEX_VT_MAX_ENTRIES) + " entry maximum!"
        return 1
    
    # 1. Parse the command line arguments
    if len(argv) != 3:
        print "Number of cmd line arguments: ", len(sys.argv)
        return 2
    xl_fname = argv[0]
    c_fname = argv[1]
    h_fname = argv[2]
    
    # Set up the xl workbook/worksheet(s)
    wb = load_workbook(xl_fname, use_iterators=True)
    ws = wb.get_sheet_by_name('table')
    bands_ws = wb.get_sheet_by_name('bands')
    wlan_chan_ws = wb.get_sheet_by_name('wlan_chan')
    ncols = ncols_worksheet(ws)
    init_col_num_tbl()
    init_wlan_chan_tbl(wlan_chan_ws)
    init_operating_dim_dict(bands_ws)

    # 2. Parse the xl worksheet
    coex_victim_tbl = parse_WS(ws, ncols)
                    
    # Append extra entries for purposes of adjusting victim table using NV
    if len(coex_victim_tbl) > CXM_COEX_VT_MAX_ENTRIES:
        print "Error, aborting: somehow added too many entries to victim table!"
        return 1
    CXM_COEX_VT_VALID_STATIC_SIZE = len(coex_victim_tbl)
    for i in range(CXM_COEX_VT_VALID_STATIC_SIZE, CXM_COEX_VT_MAX_ENTRIES):
        add_vt_entry(coex_victim_tbl, ncols)
        
    # 3. Fix formatting
    c_file_str, h_file_str = format_file_strings(coex_victim_tbl, CXM_COEX_VT_VALID_STATIC_SIZE)
    
    # 4. Generate file(s)
    print "Generating file(s)..."
    
    out_file = open(h_fname, 'w')
    out_file.write(h_file_str)  
    out_file.close()
    out_file = open(c_fname, 'w')
    out_file.write(c_file_str)  
    out_file.close()
            
    # Success
    return 0

if __name__ == "__main__":
    # Run the program with the three file args
    main(sys.argv[1:])
