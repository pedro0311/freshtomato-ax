/*****************************************************************************************
 * FILE NAME          : HostCfgLib.h
 *
 *
 * DATE CREATED       : Oct/11/2016
 * LAST MODIFIED      :
 *
 * DESCRIPTION        : Platform &ECB  API header file.
 *
 *
 *****************************************************************************************
 * MaxLinear, Inc. retains all right, title and interest (including all intellectual
 * property rights) in and to this computer program, which is protected by applicable
 * intellectual property laws.  Unless you have obtained a separate written license from
 * MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., you are not authorized
 * to utilize all or a part of this computer program for any purpose (including
 * reproduction, distribution, modification, and compilation into object code), and you
 * must immediately destroy or return all copies of this computer program.  If you are
 * licensed by MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., your rights
 * to utilize this computer program are limited by the terms of that license.
 *
 * This computer program contains trade secrets owned by MaxLinear, Inc. and, unless
 * authorized by MaxLinear, Inc. in writing, you agree to maintain the confidentiality
 * of this computer program and related information and to not disclose this computer
 * program and related information to any other person or entity.
 *
 * Misuse of this computer program or any information contained in it may results in
 * violations of applicable law.  MaxLinear, Inc. vigorously enforces its copyright,
 * trade secret, patent, contractual, and other legal rights.
 *
 * THIS COMPUTER PROGRAM IS PROVIDED "AS IS" WITHOUT ANY WARRANTIES, AND
 * MAXLINEAR, INC.EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE
 * WARRANTIES OF MERCHANTIBILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE, AND
 * NONINFRINGEMENT.
 *
 ****************************************************************************************
 * Copyright (c) 2016, MaxLinear, Inc.
 ****************************************************************************************/

#ifndef __ECBLIB_API_H__
#define __ECBLIB_API_H__


/*******************************************************************************
 *      Includes
 ******************************************************************************/
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "mxl_moca_config.h"
#include "ClnkLibApi.h"


/*******************************************************************************
 *      Constants
 ******************************************************************************/
#define  CFG_HOSTID                 "hostid"
#define  CFG_TESTMODE               "testmode"
#define  CFG_IPADDR                 "ipaddr"
#define  CFG_NETMASK                "netmask"
#define  CFG_DHCP                   "netcfgmode"
#define  CFG_GATEWAY                "gateway"
#define  CFG_ADMPSWD                "admpassword"
#define  CFG_MFR_VENDOR_ID          "mfrvendorid"
#define  CFG_MFR_HW_VER             "mfrhwver"
#define  CFG_MFR_SW_VER             "mfrswver"
#define  CFG_PERSONALITY            "personality"

// Configuration file name to be used

#define PLATFORM_CFG_FILE           "platform.conf"

#define MOCA_PASSWORD_MAX_LEN       18

#define MAX_RLAPM_MODE_STR_SIZE     (17 + 1)
#define RLAPM_CONF_FILENAME         "rlapm.conf"
#define RLAPM_BACKUP_FILENAME       "rlapm.backup"
#define RLAPM_PS_FILENAME           "INCF"

#define MAX_SAPM_MODE_STR_SIZE      (16 + 1)
#define SAPM_CONF_FILENAME          "sapm.conf"
#define SAPM_BACKUP_FILENAME        "sapm.backup"
#define SAPM_PS_FILENAME            "SSCF"

#define RSSI_CONF_FILENAME          "rssi.conf"
#define RSSI_BACKUP_FILENAME        "rssi.backup"
#define RSSI_PS_FILENAME            "RSCF"

#define ENDET_CONF_FILENAME         "endet.conf"
#define ENDET_BACKUP_FILENAME       "endet.backup"
#define ENDET_PS_FILENAME           "EDCF"

#define CLNK_CONF_FILENAME          "clink.conf"
#define CLNK_BACKUP_FILENAME        "clink.backup"
#define CLNK_PS_FILENAME            "CLNK"

#define MCAST_CONF_FILENAME         "mcast.conf"
#define MCAST_BACKUP_FILENAME       "mcast.backup"
#define MCAST_PS_FILENAME           "MCAS"


#define PSAL_RETURN_OK 0
#define PSAL_RETURN_FAIL -1

#define HW_MAC_ADDR_FMT(x,y) ((x) >>24), (((x)>>16)&0xff), (((x)>>8) &0xff), ((x)&0xff), ((y)>>24) , (((y)>>16) &0xff)
#define HW_MAC_ADDR_FMT_STR    "%02X:%02X:%02X:%02X:%02X:%02X"

#define INT4(a, b, c, d) ( ( (a) << 24) | ((b) << 16) | ( (c) << 8) | d)
#define IPSTR(d)  (uint8_t)((d) >> 24), (uint8_t)(((d) >>16)&0xff), (uint8_t)(((d)>>8) &0xff), (uint8_t)((d) &0xff)

enum
{
  MASK_BAND_ED =0,
  MASK_BAND_D_LO ,
  MASK_BAND_D_HI ,
  MASK_BAND_E ,
  MASK_BAND_FSAT,
  MASK_BAND_FCBL,
  MASK_BAND_H,
  MASK_BAND_CUSTOM,
};

///Table 7-2. c.LINK Configuration paramters
///System parameters
#define CFG_MOCA_VER                "mocaversion"
#define CFG_TEST_MODE               "testmode"
#define CFG_CLNKLED_MODE            "clnkledmode"
#define CFG_TEST_PARAM_1            "testparam1"
#define CFG_TEST_PARAM_2            "testparam2"
#define CFG_TEST_PARAM_3            "testparam3"
#define CFG_TEST_PARAM_4            "testparam4"
#define CFG_TURBO_MODE              "turbomode"
///ECL Parameters
#define CFG_MII_PAUSE_PRIO_LEVEL    "miipauseprilevel"
#define CFG_POLICING_MODE           "policingmode"
#define CFG_PQOS_CLASSI_MODE        "pqosclassifymode"
#define CFG_RETRANS_PRIO            "retxpriority"
#define CFG_TRANS_PRIO_PER          "txpriorityper"
//#define CFG_XMII_SAF                "xmii"
#define CFG_MII_SPEED               "miispeed"
///LC Parameters
#define CFG_FAILOVER_MODE           "failovermode"
#define CFG_HANDOFF_MODE            "handoffmode"
#define CFG_INITIAL_ROLE            "initialrole"
#define CFG_LMO_MODE                "lmomode"
#define CFG_MOCA_PASSWD             "mocapassword"
#define CFG_PREF_NC_MODE            "preferrednc"
#define CFG_SECU_MODE               "securitymode"
#define CFG_NETWORK_SEARCH_MODE     "networksearch"
#define CFG_FREQ_PLAN               "freqplan"
#define CFG_CHANNEL_MASK            "channelmask"
#define CFG_LAST_OPER_FREQ          "lof"
#define CFG_PRIMARY_CHANN_OFFSET    "primchnoffset"
#define CFG_SECOND_CHANN_OFFSET     "secchnoffset"
#define CFG_TABOO_MASK              "taboomask"
#define CFG_TABOO_OFFSET            "taboooffset"
#define CFG_TLP_MIN                 "tlpmin"
#define CFG_TLP_MAX                 "tlpmax"
///PHY PARAMETERS
#define CFG_BEACON_TX_POWER         "beacontxpower"
#define CFG_MAX_TX_POWER            "maxtxpower"
#define CFG_TARGET_P2P_RATE50       "tpcphyrate50"
#define CFG_TARGET_P2P_RATENPER     "tpcphyratenper"
#define CFG_TARGET_P2P_RATEVLPER    "tpcphyratevlper"
#define CFG_STRIP_VLAN_TAG          "stripvlantag"
#define CFG_ENCODING_WORD_LEN_NPER  "encwordlennper"
#define CFG_ENCODING_WORD_LEN_VLPER "encwordlenvlper"
#define CFG_BITLOAD_MARGIN50        "phymargin"
#define CFG_BITLOAD_MARGINNPER      "phymarginnper"
#define CFG_BITLOAD_MARGINVLPER     "phymarginvlper"
#define CFG_FEATURE_FLAGS           "featureflags"
#define CFG_TPC_MODE                "tpcmode"
#define CFG_BITLOAD_UCAST50         "phymoducast50"
#define CFG_BITLOAD_BCAST50         "phymodbcast50"
#define CFG_BITLOAD_UCAST100        "phymoducast100"
#define CFG_BITLOAD_BCAST100        "phymodbcast100"
#define CFG_MOCA_SEQ_NUM_MR         "mocaseqnummr"
///V2 CFGS
#define CFG_SCAN_OFFSET                       "scanoffset"
#define CFG_PRIM_CHN_ABOVE                    "primchnabove"
#define CFG_PRIM_CHN_BELOW                    "primchnbelow"
#define CFG_SEC_CHN_ABOVE                     "secchnabove"
#define CFG_SEC_CHN_BELOW                     "secchnbelow"
#define CFG_FREQ_PROFC_GAP_MODE               "custombandprofcgapmode"
#define CFG_FREQ_BAND_MASK                    "freqbandmask"
#define CFG_SCAN_MASK                         "scanmask"
#define CFG_SCAN_MASK_BAND_D                  "scanmaskbandd"
#define CFG_SCAN_MASK_BAND_D_LO               "scanmaskbanddlo"
#define CFG_SCAN_MASK_BAND_D_HI               "scanmaskbanddhi"
#define CFG_SCAN_MASK_BAND_E                  "scanmaskbande"
#define CFG_SCAN_MASK_BAND_FSAT               "scanmaskbandfsat"
#define CFG_SCAN_MASK_BAND_FCBL               "scanmaskbandfcbl"
#define CFG_SCAN_MASK_BAND_H                  "scanmaskbandh"
#define CFG_SCAN_MASK_CUSTOM                  "scanmaskcustom"
#define CFG_SCAN_OFFSET_BAND_D                "scanoffsetbandd"
#define CFG_SCAN_OFFSET_BAND_D_LO             "scanoffsetbanddlo"
#define CFG_SCAN_OFFSET_BAND_D_HI             "scanoffsetbanddhi"
#define CFG_SCAN_OFFSET_BAND_E                "scanoffsetbande"
#define CFG_SCAN_OFFSET_BAND_FSAT             "scanoffsetbandfsat"
#define CFG_SCAN_OFFSET_BAND_FCBL             "scanoffsetbandfcbl"
#define CFG_SCAN_OFFSET_BAND_H                "scanoffsetbandh"
#define CFG_SCAN_OFFSET_CUSTOM                "scanoffsetcustom"
#define CFG_LOF_UPDATE_MODE                   "lofupdmode"
#define CFG_PRIM_CHN_ABOVE_BAND_D             "primchnabovebandd"
#define CFG_PRIM_CHN_ABOVE_BAND_D_LO          "primchnabovebanddlo"
#define CFG_PRIM_CHN_ABOVE_BAND_D_HI          "primchnabovebanddhi"
#define CFG_PRIM_CHN_ABOVE_BAND_E             "primchnabovebande"
#define CFG_PRIM_CHN_ABOVE_BAND_FSAT          "primchnabovebandfsat"
#define CFG_PRIM_CHN_ABOVE_BAND_FCBL          "primchnabovebandfcbl"
#define CFG_PRIM_CHN_ABOVE_BAND_H             "primchnabovebandh"
#define CFG_PRIM_CHN_ABOVE_CUSTOM             "primchnabovecustom"
#define CFG_PRIM_CHN_BELOW_BAND_D             "primchnbelowbandd"
#define CFG_PRIM_CHN_BELOW_BAND_D_LO          "primchnbelowbanddlo"
#define CFG_PRIM_CHN_BELOW_BAND_D_HI          "primchnbelowbanddhi"
#define CFG_PRIM_CHN_BELOW_BAND_E             "primchnbelowbande"
#define CFG_PRIM_CHN_BELOW_BAND_FSAT          "primchnbelowbandfsat"
#define CFG_PRIM_CHN_BELOW_BAND_FCBL          "primchnbelowbandfcbl"
#define CFG_PRIM_CHN_BELOW_BAND_H             "primchnbelowbandh"
#define CFG_PRIM_CHN_BELOW_CUSTOM             "primchnbelowcustom"
#define CFG_SEC_CHN_ABOVE_BAND_D              "secchnabovebandd"
#define CFG_SEC_CHN_ABOVE_BAND_D_LO           "secchnabovebanddlo"
#define CFG_SEC_CHN_ABOVE_BAND_D_HI           "secchnabovebanddhi"
#define CFG_SEC_CHN_ABOVE_BAND_E              "secchnabovebande"
#define CFG_SEC_CHN_ABOVE_BAND_FSAT           "secchnabovebandfsat"
#define CFG_SEC_CHN_ABOVE_BAND_FCBL           "secchnabovebandfcbl"
#define CFG_SEC_CHN_ABOVE_BAND_H              "secchnabovebandh"
#define CFG_SEC_CHN_ABOVE_CUSTOM              "secchnabovecustom"
#define CFG_SEC_CHN_BELOW_BAND_D              "secchnbelowbandd"
#define CFG_SEC_CHN_BELOW_BAND_D_LO           "secchnbelowbanddlo"
#define CFG_SEC_CHN_BELOW_BAND_D_HI           "secchnbelowbanddhi"
#define CFG_SEC_CHN_BELOW_BAND_E              "secchnbelowbande"
#define CFG_SEC_CHN_BELOW_BAND_FSAT           "secchnbelowbandfsat"
#define CFG_SEC_CHN_BELOW_BAND_FCBL           "secchnbelowbandfcbl"
#define CFG_SEC_CHN_BELOW_BAND_H              "secchnbelowbandh"
#define CFG_SEC_CHN_BELOW_CUSTOM              "secchnbelowcustom"
#define CFG_TURBO_MODE_BAND_D                 "turbomodebandd"
#define CFG_TURBO_MODE_BAND_D_LO              "turbomodebanddlo"
#define CFG_TURBO_MODE_BAND_D_HI              "turbomodebanddhi"
#define CFG_TURBO_MODE_BAND_E                 "turbomodebande"
#define CFG_TURBO_MODE_BAND_FSAT              "turbomodebandfsat"
#define CFG_TURBO_MODE_BAND_FCBL              "turbomodebandfcbl"
#define CFG_TURBO_MODE_BAND_H                 "turbomodebandh"
#define CFG_TURBO_MODE_CUSTOM                 "turbomodecustom"
#define CFG_POLICING_MODE_BAND_D              "policingmodebandd"
#define CFG_POLICING_MODE_BAND_D_LO           "policingmodebanddlo"
#define CFG_POLICING_MODE_BAND_D_HI           "policingmodebanddhi"
#define CFG_POLICING_MODE_BAND_E              "policingmodebande"
#define CFG_POLICING_MODE_BAND_FSAT           "policingmodebandfsat"
#define CFG_POLICING_MODE_BAND_FCBL           "policingmodebandfcbl"
#define CFG_POLICING_MODE_BAND_H              "policingmodebandh"
#define CFG_POLICING_MODE_CUSTOM              "policingmodecustom"
#define CFG_RETXPRIORITY_BAND_D               "retxprioritybandd"
#define CFG_RETXPRIORITY_BAND_D_LO            "retxprioritybanddlo"
#define CFG_RETXPRIORITY_BAND_D_HI            "retxprioritybanddhi"
#define CFG_RETXPRIORITY_BAND_E               "retxprioritybande"
#define CFG_RETXPRIORITY_BAND_FSAT            "retxprioritybandfsat"
#define CFG_RETXPRIORITY_BAND_FCBL            "retxprioritybandfcbl"
#define CFG_RETXPRIORITY_BAND_H               "retxprioritybandh"
#define CFG_RETXPRIORITY_CUSTOM               "retxprioritycustom"
#define CFG_TXPRIORITYPER_BAND_D              "txpriorityperbandd"
#define CFG_TXPRIORITYPER_BAND_D_LO           "txpriorityperbanddlo"
#define CFG_TXPRIORITYPER_BAND_D_HI           "txpriorityperbanddhi"
#define CFG_TXPRIORITYPER_BAND_E              "txpriorityperbande"
#define CFG_TXPRIORITYPER_BAND_FSAT           "txpriorityperbandfsat"
#define CFG_TXPRIORITYPER_BAND_FCBL           "txpriorityperbandfcbl"
#define CFG_TXPRIORITYPER_BAND_H              "txpriorityperbandh"
#define CFG_TXPRIORITYPER_CUSTOM              "txprioritypercustom"
#define CFG_MOCA_PASSWD_BAND_D                "mocapasswordbandd"
#define CFG_MOCA_PASSWD_BAND_D_LO             "mocapasswordbanddlo"
#define CFG_MOCA_PASSWD_BAND_D_HI             "mocapasswordbanddhi"
#define CFG_MOCA_PASSWD_BAND_E                "mocapasswordbande"
#define CFG_MOCA_PASSWD_BAND_FSAT             "mocapasswordbandfsat"
#define CFG_MOCA_PASSWD_BAND_FCBL             "mocapasswordbandfcbl"
#define CFG_MOCA_PASSWD_BAND_H                "mocapasswordbandh"
#define CFG_MOCA_PASSWD_CUSTOM                "mocapasswordcustom"
#define CFG_MOCA_SEQNUMMR_BAND_D              "mocaseqnummrbandd"
#define CFG_MOCA_SEQNUMMR_BAND_D_LO           "mocaseqnummrbanddlo"
#define CFG_MOCA_SEQNUMMR_BAND_D_HI           "mocaseqnummrbanddhi"
#define CFG_MOCA_SEQNUMMR_BAND_E              "mocaseqnummrbande"
#define CFG_MOCA_SEQNUMMR_BAND_FSAT           "mocaseqnummrbandfsat"
#define CFG_MOCA_SEQNUMMR_BAND_FCBL           "mocaseqnummrbandfcbl"
#define CFG_MOCA_SEQNUMMR_BAND_H              "mocaseqnummrbandh"
#define CFG_MOCA_SEQNUMMR_CUSTOM              "mocaseqnummrcustom"
#define CFG_TLP_MIN_BAND_D                    "tlpminbandd"
#define CFG_TLP_MIN_BAND_D_LO                 "tlpminbanddlo"
#define CFG_TLP_MIN_BAND_D_HI                 "tlpminbanddhi"
#define CFG_TLP_MIN_BAND_E                    "tlpminbande"
#define CFG_TLP_MIN_BAND_FSAT                 "tlpminbandfsat"
#define CFG_TLP_MIN_BAND_FCBL                 "tlpminbandfcbl"
#define CFG_TLP_MIN_BAND_H                    "tlpminbandh"
#define CFG_TLP_MIN_CUSTOM                    "tlpmincustom"
#define CFG_TLP_MAX_BAND_D                    "tlpmaxbandd"
#define CFG_TLP_MAX_BAND_D_LO                 "tlpmaxbanddlo"
#define CFG_TLP_MAX_BAND_D_HI                 "tlpmaxbanddhi"
#define CFG_TLP_MAX_BAND_E                    "tlpmaxbande"
#define CFG_TLP_MAX_BAND_FSAT                 "tlpmaxbandfsat"
#define CFG_TLP_MAX_BAND_FCBL                 "tlpmaxbandfcbl"
#define CFG_TLP_MAX_BAND_H                    "tlpmaxbandh"
#define CFG_TLP_MAX_CUSTOM                    "tlpmaxcustom"
#define CFG_SECURITY_MODE_BAND_D              "securitymodebandd"
#define CFG_SECURITY_MODE_BAND_D_LO           "securitymodebanddlo"
#define CFG_SECURITY_MODE_BAND_D_HI           "securitymodebanddhi"
#define CFG_SECURITY_MODE_BAND_E              "securitymodebande"
#define CFG_SECURITY_MODE_BAND_FSAT           "securitymodebandfsat"
#define CFG_SECURITY_MODE_BAND_FCBL           "securitymodebandfcbl"
#define CFG_SECURITY_MODE_BAND_H              "securitymodebandh"
#define CFG_SECURITY_MODE_CUSTOM              "securitymodecustom"
#define CFG_TPC_MODE_BAND_D                   "tpcmodebandd"
#define CFG_TPC_MODE_BAND_D_LO                "tpcmodebanddlo"
#define CFG_TPC_MODE_BAND_D_HI                "tpcmodebanddhi"
#define CFG_TPC_MODE_BAND_E                   "tpcmodebande"
#define CFG_TPC_MODE_BAND_FSAT                "tpcmodebandfsat"
#define CFG_TPC_MODE_BAND_FCBL                "tpcmodebandfcbl"
#define CFG_TPC_MODE_BAND_H                   "tpcmodebandh"
#define CFG_TPC_MODE_CUSTOM                   "tpcmodecustom"
#define CFG_BITLOAD_MARGIN50_BAND_D             "phymarginbandd"
#define CFG_BITLOAD_MARGIN50_BAND_D_LO          "phymarginbanddlo"
#define CFG_BITLOAD_MARGIN50_BAND_D_HI          "phymarginbanddhi"
#define CFG_BITLOAD_MARGIN50_BAND_E             "phymarginbande"
#define CFG_BITLOAD_MARGIN50_BAND_FSAT          "phymarginbandfsat"
#define CFG_BITLOAD_MARGIN50_BAND_FCBL          "phymarginbandfcbl"
#define CFG_BITLOAD_MARGIN50_BAND_H             "phymarginbandh"
#define CFG_BITLOAD_MARGIN50_CUSTOM             "phymargincustom"
#define CFG_BITLOAD_MARGINNPER_BAND_D           "phymarginnperbandd"
#define CFG_BITLOAD_MARGINNPER_BAND_D_LO        "phymarginnperbanddlo"
#define CFG_BITLOAD_MARGINNPER_BAND_D_HI        "phymarginnperbanddhi"
#define CFG_BITLOAD_MARGINNPER_BAND_E           "phymarginnperbande"
#define CFG_BITLOAD_MARGINNPER_BAND_FSAT        "phymarginnperbandfsat"
#define CFG_BITLOAD_MARGINNPER_BAND_FCBL        "phymarginnperbandfcbl"
#define CFG_BITLOAD_MARGINNPER_BAND_H           "phymarginnperbandh"
#define CFG_BITLOAD_MARGINNPER_CUSTOM           "phymarginnpercustom"
#define CFG_BITLOAD_MARGINVLPER_BAND_D          "phymarginvlperbandd"
#define CFG_BITLOAD_MARGINVLPER_BAND_D_LO       "phymarginvlperbanddlo"
#define CFG_BITLOAD_MARGINVLPER_BAND_D_HI       "phymarginvlperbanddhi"
#define CFG_BITLOAD_MARGINVLPER_BAND_E          "phymarginvlperbande"
#define CFG_BITLOAD_MARGINVLPER_BAND_FSAT       "phymarginvlperbandfsat"
#define CFG_BITLOAD_MARGINVLPER_BAND_FCBL       "phymarginvlperbandfcbl"
#define CFG_BITLOAD_MARGINVLPER_BAND_H          "phymarginvlperbandh"
#define CFG_BITLOAD_MARGINVLPER_CUSTOM          "phymarginvlpercustom"
#define CFG_MPA_MAC_ADDR                        "mpadstmacaddr"
#define CFG_MPA_PKT_FILTER                      "mpamocapktfilter"
#define CFG_FIRST_CHANNEL_OFFSET_BAND_D         "firstchanneloffsetbandd"
#define CFG_FIRST_CHANNEL_OFFSET_BAND_D_LO      "firstchanneloffsetbanddlo"
#define CFG_FIRST_CHANNEL_OFFSET_BAND_D_HI      "firstchanneloffsetbanddhi"
#define CFG_FIRST_CHANNEL_OFFSET_BAND_E         "firstchanneloffsetbande"
#define CFG_FIRST_CHANNEL_OFFSET_BAND_FSAT      "firstchanneloffsetbandfsat"
#define CFG_FIRST_CHANNEL_OFFSET_BAND_FCBL      "firstchanneloffsetbandfcbl"
#define CFG_FIRST_CHANNEL_OFFSET_BAND_H         "firstchanneloffsetbandh"
#define CFG_FIRST_CHANNEL_OFFSET_CUSTOM         "firstchanneloffsetcustom"
#define CFG_NUMBER_OF_CHANNELS_BAND_D           "numberofchannelsbandd"
#define CFG_NUMBER_OF_CHANNELS_BAND_D_LO        "numberofchannelsbanddlo"
#define CFG_NUMBER_OF_CHANNELS_BAND_D_HI        "numberofchannelsbanddhi"
#define CFG_NUMBER_OF_CHANNELS_BAND_E           "numberofchannelsbande"
#define CFG_NUMBER_OF_CHANNELS_BAND_FSAT        "numberofchannelsbandfsat"
#define CFG_NUMBER_OF_CHANNELS_BAND_FCBL        "numberofchannelsbandfcbl"
#define CFG_NUMBER_OF_CHANNELS_BAND_H           "numberofchannelsbandh"
#define CFG_NUMBER_OF_CHANNELS_CUSTOM           "numberofchannelscustom"
#define CFG_LOADING_CAPS                        "loadingcaps"
#define CFG_MAC_ADDR_AGING_TIME                 "macaddragingtime"
#define CFG_BCN_PWR_ENABLE                      "beaconpowerenable"
#define CFG_BCN_PWR_DIST                        "beaconpowerdist"
#define CFG_BCN_PWR_LOCAL                       "beaconpowerlocal"
#define CFG_PRIVACY_SUPPORTED                   "privacysupported"
#define CFG_ENHANCED_PASSWORD                   "enhancedpassword"
#define CFG_NETWORK_NAME                        "networkname"
#define CFG_NETWORK_NAME_ADMIT_CFG              "networknameadmitcfg"
#define CFG_ME_NETWORK_IE_PAYLOAD_TX            "menetworkiepayloadtx"
#define CFG_ME_NETWORK_IE_RESP_TIMEOUT          "menetworkieresptimeout"
#define CFG_ME_NETWORK_JOIN_EN                  "menetworkjoinen"
#define CFG_MPS_PRIVACY_RECEIVE                 "mpsprivacyreceive"
#define CFG_MPS_PRIVACY_DOWN                    "mpsprivacydown"
#define CFG_MPS_UNPAIRED_TIME                   "mpsunpairedtime"
#define CFG_MPS_WALK_TIME                       "mpswalktime"
#define CFG_AUTH_LINK_ETHERTYPE                 "authlinkethtype"
#define CFG_AUTH_LINK_BITMASK_0                 "authlinkbitmask0"
#define CFG_AUTH_LINK_BITMASK_1                 "authlinkbitmask1"
#define CFG_AUTH_LINK_BITMASK_2                 "authlinkbitmask2"
#define CFG_AUTH_LINK_BITMASK_3                 "authlinkbitmask3"
#define CFG_AUTH_LINK_BITMASK_4                 "authlinkbitmask4"
#define CFG_AUTH_LINK_BITMASK_5                 "authlinkbitmask5"
#define CFG_AUTH_LINK_BITMASK_6                 "authlinkbitmask6"
#define CFG_AUTH_LINK_BITMASK_7                 "authlinkbitmask7"
#define CFG_AUTH_LINK_BITMASK_8                 "authlinkbitmask8"
#define CFG_AUTH_LINK_BITMASK_9                 "authlinkbitmask9"
#define CFG_AUTH_LINK_BITMASK_10                "authlinkbitmask10"
#define CFG_AUTH_LINK_BITMASK_11                "authlinkbitmask11"
#define CFG_AUTH_LINK_BITMASK_12                "authlinkbitmask12"
#define CFG_AUTH_LINK_BITMASK_13                "authlinkbitmask13"
#define CFG_AUTH_LINK_BITMASK_14                "authlinkbitmask14"
#define CFG_AUTH_LINK_BITMASK_15                "authlinkbitmask15"
#define CFG_TRACE_ENABLE                        "traceenable"
#define CFG_TRACE_LEVEL                         "tracelevel"
#define CFG_TRACE_MODULE                        "tracemodule"
#define CFG_TRACE_DEST                          "tracedest"
#define CFG_TRACE_HOST_CTL                      "tracehostctl"
#define CFG_HANDOFF_TO_LOWERMOCAVER             "handofftolowermocaver"


#define MAX_ADMINPASSWORD_LEN         64
#define TR69_PARAM_NAME_MAX_SIZE      256
#define TR69_MAX_ACS_URL_SIZE         TR69_PARAM_NAME_MAX_SIZE
#define MAX_ACS_USR_NAME_IN_BYTES     64
#define MAX_ACS_PASSWD_IN_BYTES       64
#define MAX_TLS_COMMON_NAME_STR_SIZE_IN_BYTES     64
#define MAX_TLS_HOST_FILE_SERVER_IN_BYTES         64

/******************************************************************************
  *    Data types
  *****************************************************************************/
typedef struct
{
  uint32_t           hostId;//default val 0
  uint32_t           testmode;//Test mode: normal mode(0), Ethernet control port mode (1)
  uint32_t           netCfgMode;//default static(0) other val is dhcp 1 link local 2
  uint32_t           ipAddr;  //default val 192.168.144.20
  uint32_t           netMask; //default val 255.255.255.0
  uint32_t           gateway;//default val 0.0.0.0
  char               adminPasswd[MAX_ADMINPASSWORD_LEN];//max len is 64 , default val maxlinear
  uint32_t           mfrVendorId; //default val 0
  uint32_t           mfrSwVer;//default 0
  uint32_t           mfrHwVer;//default 0
  uint32_t           personality;//default val 0
  uint32_t           tr69Enable; 
  char               tr69AcsUrl[TR69_MAX_ACS_URL_SIZE];
  char               tr69AcsUsername[MAX_ACS_USR_NAME_IN_BYTES];
  char               tr69AcsPassword[MAX_ACS_PASSWD_IN_BYTES];
  uint32_t           dnsServer;//default val 0
  uint32_t           acsInformIntervalInSecs;  
  char               tlsCommonName[MAX_TLS_COMMON_NAME_STR_SIZE_IN_BYTES];
  char               tlsHostFileServer[MAX_TLS_HOST_FILE_SERVER_IN_BYTES];
} Lib_EcbCfg_t;

int32_t ClnkHostCfg_GetEcbCfg(int, Lib_EcbCfg_t *); /// 1 to read def, 0 to read current
int32_t ClnkHostCfg_SetEcbCfg(Lib_EcbCfg_t *);

void ClnkHostCfg_Parse_Number(FILE *fp, int *val);
void ClnkHostCfg_Parse_Number_Ull(FILE *fp, uint64_t *val);
void ClnkHostCfg_Parse_String(FILE *fp, char *str, size_t maxlen);
int ClnkHostCfg_Parse_Macstr(FILE *fp, uint32_t *hi, uint32_t *lo);
void ClnkHostCfg_Parse_Pwd(FILE *fp, char *str, uint32_t maxlen);
void ClnkHostCfg_Parse_Mpwd(FILE *fp, char *str, uint32_t maxlen);

void ClnkHostCfg_Plop_Number(FILE *fp, const char *variable, const int value);
void ClnkHostCfg_Plop_String(FILE *fp, const char *variable, const char *value);
void ClnkHostCfg_Plop_Ipaddr(FILE *fp, const char *variable, uint32_t value);
void ClnkHostCfg_Plop_Macaddr(FILE *fp, const char *variable, uint32_t hi, uint32_t lo);
void ClnkHostCfg_Plop_Hex(FILE *fp, const char *variable, const int value);
void ClnkHostCfg_Plop_Hex_U8(FILE *fp, const char *variable, const int value);
void ClnkHostCfg_Plop_Hex_Ull(FILE *fp, const char *variable, const uint64_t value);
void ClnkHostCfg_Plop_Pwd(FILE *fp, const char *variable, const char *value);
void ClnkHostCfg_Plop_Mpwd(FILE *fp, const char *variable, const char *value);


int ClnkHostCfg_PsWriteClnk(ClnkLib_Handle_t *pHandle, ClnkLib_StaticCfg_t *setup, const char *file_path, const char *fwdir);
int ClnkHostCfg_PsReadClnk(ClnkLib_Handle_t *pHandle, ClnkLib_StaticCfg_t *setup, int flag, const char *file_path);
int ClnkHostCfg_PsWriteClnkV2(ClnkLib_Handle_t *pHandle, ClnkLib_ClnkCfgV2_t *setup, const char *file_path, const char *fwdir);
int ClnkHostCfg_PsReadClnkV2(ClnkLib_Handle_t *pHandle, ClnkLib_ClnkCfgV2_t *setup, int flag, const char *file_path);
int ClnkHostCfg_PsWriteClnkV2Params(ClnkLib_Handle_t *pHandle, clnk_param_mask_t *mask, ClnkLib_ClnkCfgV2_t *setup);
int ClnkHostCfg_PsReadClnkV2Params(ClnkLib_Handle_t *pHandle, clnk_param_mask_t *mask, ClnkLib_ClnkCfgV2_t *setup);
int ClnkHostCfg_PsRestoreClnkV2(ClnkLib_Handle_t *pHandle);
int ClnkHostCfg_PsWriteMcast(ClnkLib_Handle_t *pHandle, ClnkLib_McastCfg_t *pCfg, const char *file_path, const char *fwdir);
int ClnkHostCfg_PsReadMcast(ClnkLib_Handle_t *pHandle, ClnkLib_McastCfg_t *pCfg, int flag, const char *file_path);
int ClnkHostCfg_PsSetMcastFilter(ClnkLib_Handle_t *pHandle, uint32_t macAddrHi, uint32_t macAddrLo, uint32_t mode);
int ClnkHostCfg_PsSetMcastFilterMode(ClnkLib_Handle_t *pHandle, uint32_t mode);
int ClnkHostCfg_PsRestoreMcast(ClnkLib_Handle_t *pHandle);
int ClnkHostCfg_PsReadRlapmV2(ClnkLib_Handle_t *pHandle, ClnkLib_RlapmCfgV2_t *pRlapmCfgV2, int flag, const char *pFilePath);
int ClnkHostCfg_PsReadStaticRlapmV2(ClnkLib_Handle_t *pHandle, ClnkLib_RlapmStaticCfgV2_t *pRlapmStaticCfgV2, int flag, const char *pFilePath);
int ClnkHostCfg_PsSaveStaticRlapmV2(ClnkLib_Handle_t *pHandle, const ClnkLib_RlapmStaticCfgV2_t *pRlapmStaticCfgV2, const char *pFilePath, const char *fwdir);
int ClnkHostCfg_PsReplaceRlapmV2(ClnkLib_Handle_t *pHandle, const char *pNewPathFileName, const char *pFilePath, const char *fwdir);
int ClnkHostCfg_PsGetRlapmFilenameV2(ClnkLib_Handle_t *pHandle, char *pDstPathFile, const char *pFilePath);
int ClnkHostCfg_PsSetRlapmProfileId(ClnkLib_Handle_t *pHandle, int32_t profileId);
int ClnkHostCfg_PsSetRlapmMode(ClnkLib_Handle_t *pHandle, uint32_t band, uint32_t mode);
int ClnkHostCfg_PsSetRlapmSimpleCfg(ClnkLib_Handle_t *pHandle, int32_t freq, int32_t rxLevel, int32_t margin);
int ClnkHostCfg_PsReadSapmV2(ClnkLib_Handle_t *pHandle, ClnkLib_SapmCfgV2_t *pSapmCfgV2, int flag, const char *pFilePath);
int ClnkHostCfg_PsReadStaticSapmV2(ClnkLib_Handle_t *pHandle, ClnkLib_SapmStaticCfgV2_t *pSapmStaticCfgV2, int flag, const char *pFilePath);
int ClnkHostCfg_PsSaveStaticSapmV2(ClnkLib_Handle_t *pHandle, const ClnkLib_SapmStaticCfgV2_t *pSapmStaticCfgV2, const char *pFilePath, const char *fwdir);
int ClnkHostCfg_PsReplaceSapmV2(ClnkLib_Handle_t *pHandle, const char *pNewPathFileName, const char *pFilePath, const char *fwdir);
int ClnkHostCfg_PsGetSapmFilenameV2(ClnkLib_Handle_t *pHandle, char *pDstPathFile, const char *pFilePath);
int ClnkHostCfg_PsSetSapmProfileId(ClnkLib_Handle_t *pHandle, int32_t profileId);
int ClnkHostCfg_PsSetSapmMode(ClnkLib_Handle_t *pHandle, uint32_t band, uint32_t mode);
int ClnkHostCfg_PsSetSapmSimpleCfg(ClnkLib_Handle_t *pHandle, int32_t freq, int32_t threshold, int32_t scStart, int32_t carrier, int32_t margin);
int ClnkHostCfg_PsWriteRssiV2(ClnkLib_Handle_t *pHandle, ClnkLib_RssiCfgV2_t *pRssiCfgV2, const char *pFilePath, const char *fwdir);
int ClnkHostCfg_PsReadRssiV2(ClnkLib_Handle_t *pHandle, ClnkLib_RssiCfgV2_t *pRssiCfgV2, int flag, const char *pFilePath);
int ClnkHostCfg_PsReplaceRssiV2(ClnkLib_Handle_t *pHandle, const char *pNewPathFileName, const char *pFilePath, const char *fwdir);
int ClnkHostCfg_PsWriteEndet(ClnkLib_Handle_t *pHandle, const ClnkLib_EndetCfg_t *pEndetCfg, const char *pFilePath, const char *fwdir);
int ClnkHostCfg_PsReadEndet(ClnkLib_Handle_t *pHandle, ClnkLib_EndetCfg_t *pEndetCfg, int flag, const char *pFilePath);
int ClnkHostCfg_PsReadPlatform(ClnkLib_Handle_t *pHandle, ClnkLib_PlatformCfg_t *pPlatformCfg, int flag, const char *pFilePath);
int ClnkHostCfg_FromV2ToV1(ClnkLib_ClnkCfgV2_t *v2, ClnkLib_StaticCfg_t *v1);
int ClnkHostCfg_FromV1ToV2(ClnkLib_StaticCfg_t *v1, ClnkLib_ClnkCfgV2_t *v2);
uint64_t ConvertPrimChnOffset2PrimChnABMask(uint32_t primChnOffset, uint8_t scanOffset, uint64_t *above, uint64_t *below, int freqplan);
uint32_t ConvertPrimChnAB2PrimChnOffset(uint64_t above, uint64_t below, int scanOffset, int freq, int32_t lof);
uint64_t ConvertSecChnOffset2SecChnABMask(uint32_t secChnOffset, uint8_t scanOffset, uint64_t *above, uint64_t *below, int freqplan);
uint32_t ConvertSecChnAB2SecChnOffset(uint64_t above, uint64_t below, int  scanOffset, int freq, int32_t lof);
uint64_t ConvertChannelMask2ScanMask(uint32_t channelMask, int freqplan);
int32_t ConvertChannelMask2ScanOffset(uint32_t channelMask, int freqplan);
uint32_t ConvertScanInfo2ChannelMask(uint64_t scanMask, uint8_t scanOffset, int freq);
int ClnkHostCfg_GetHwDefVal(ClnkLib_Handle_t *pHandle, char *modnum, uint32_t *diplexCal, uint32_t *miiClkDelay, 
                            uint32_t *hwCfgWord, uint32_t *freq, const char *pFilePath);
ssize_t ClnkHostCfg_SafeWrite(int fd, const void *buf, size_t count);
ssize_t ClnkHostCfg_FullWrite(int fd, const void *buf, size_t len);
int32_t ClnkHostCfg_DumpBin(ClnkLib_Handle_t *pHandle, char *iface, const char *confdir, const char *fwdir, int flags);


int32_t clnk_parse_configV2(ClnkLib_ClnkCfgV2_t *setup, char *fname);
int32_t mcast_parse_cfg(ClnkLib_McastCfg_t *pCfg, char *fname);
int rlapm_parse_rlapmmode_lineV2(FILE *fp, ClnkLib_RlapmCfgV2_t *pRlapmCfgV2, int bandIndex, const char *pFileName);
int rlapm_parse_rlapmprofileid_lineV2(FILE *fp, int *pRlapmProfileId, const char *pFileName);
int rlapm_parse_profile_lineV2(FILE *fp, int *pProfile, const char *pFileName);
int rlapm_parse_freq_lineV2(FILE *fp, ClnkLib_RlapmCfgV2_t *pRlapmCfgV2, int freqIndex, const char *pFileName);
int rlapm_parse_rxlevel_lineV2(FILE *fp, ClnkLib_RlapmCfgV2_t *pRlapmCfgV2, int freqIndex, const char *pFileName);
int rlapm_parse_cfgV2(ClnkLib_RlapmCfgV2_t *pRlapmCfgV2, const char *pFileName);
int rlapm_parse_static_cfgV2(ClnkLib_RlapmStaticCfgV2_t *pRlapmStaticCfgV2, const char *pFileName);
int sapm_parse_sapmmode_lineV2(FILE *fp, ClnkLib_SapmCfgV2_t *pSapmCfgV2, int bandIndex, const char *pFileName);
int sapm_parse_sapmprofileid_lineV2(FILE *fp, int *pSapmProfileId, const char *pFileName);
int sapm_parse_profile_lineV2(FILE *fp, int *pProfile, const char *pFileName);
int sapm_parse_freq_lineV2(FILE *fp, ClnkLib_SapmCfgV2_t *pSapmCfgV2, int freqIndex, const char *pFileName);
int sapm_parse_threshold_lineV2(FILE *fp, ClnkLib_SapmCfgV2_t *pSapmCfgV2, int freqIndex, const char *pFileName);
int sapm_parse_scstart_lineV2(FILE *fp, ClnkLib_SapmCfgV2_t *pSapmCfgV2, int freqIndex, const char *pFileName);
int sapm_parse_scoffset_lineV2(FILE *fp, ClnkLib_SapmCfgV2_t *pSapmCfgV2, int freqIndex, const char *pFileName);
int sapm_parse_cfgV2(ClnkLib_SapmCfgV2_t *pSapmCfgV2, const char *pFileName);
int sapm_parse_static_cfgV2(ClnkLib_SapmStaticCfgV2_t *pSapmStaticCfgV2, const char *pFileName);
int rssi_parse_tbl_lineV2(FILE *fp, ClnkLib_RssiCfgV2_t *pRssiCfgV2, const char *pFileName);
int rssi_parse_sfi_lineV2(FILE *fp, ClnkLib_RssiCfgV2_t *pRssiCfgV2, const char *pFileName);
int rssi_parse_cs_lineV2(FILE *fp, ClnkLib_RssiCfgV2_t *pRssiCfgV2, const char *pFileName);
int rssi_parse_lgo_lineV2(FILE *fp, ClnkLib_RssiCfgV2_t *pRssiCfgV2, const char *pFileName);
int32_t rssi_parse_cfgV2(ClnkLib_RssiCfgV2_t *pRssiCfgV2, const char *pFileName);
int endet_parse_1_arg_str_line(FILE *fp, const char *pParamName,
                                      unsigned int *pReturnValueU32,
                                      const unsigned int maxValue, const char *pFileName);
int endet_parse_1_arg_num_line(FILE *fp, const char *pParamName,
                                      unsigned int *pReturnValueU32,
                                      const unsigned int maxValue, const char *pFileName);
int endet_parse_4_arg_num_line(FILE *fp, const char *pParamName,
                                      unsigned int *pReturnValuesU32,
                                      const unsigned int maxValue, const char *pFileName);

int endet_parse_5_arg_num_line(FILE *fp, const char *pParamName,
                                      unsigned int *pReturnValuesU32,
                                      const unsigned int maxValue, const char *pFileName);
int endet_parse_8_arg_num_line(FILE *fp, const char *pParamName,
                                      unsigned int *pReturnValuesU32,
                                      const unsigned int maxValue, const char *pFileName);
int32_t endet_parse_cfg(ClnkLib_EndetCfg_t *pEndetCfg, const char *pFileName);

#endif
