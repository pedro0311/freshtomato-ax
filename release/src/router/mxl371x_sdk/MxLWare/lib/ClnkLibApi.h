/*****************************************************************************************
 * FILE NAME          : ClnkLibApi.h
 *
 *
 * DATE CREATED       : Oct/11/2016
 * LAST MODIFIED      :
 *
 * DESCRIPTION        : c.LINK  API header file.
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

#ifndef __CLNK_API_H__
#define __CLNK_API_H__
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *      Includes
 ******************************************************************************/
///int32_t
#include <inttypes.h>
///FILE *
#include <stdbool.h>
#include <stdio.h>
#include <net/if.h>
#include "mxl_moca_config.h"
#include "mxl_data_types.h"
#include "mxl_moca_soc_cmd.h"
#include "mxl_moca_config.h"


/*******************************************************************************
 *      Constants
 ******************************************************************************/
  //      REGISTER MACRO NAME                            ADDRESS       BIT    NUM
  //                                                                   LOC    BITS

#define SRE_PRODUCT_FAMILY_ID                          0x08200000,     0,     32
#define SRE_DEVICE_ID                                  0x08200004,     0,     16
#define SRE_REVISION_ID                                0x08200004,    16,     16
#define SRE_INTERRUPT_MASK                             0x08200008,     0,     32
#define SRE_BOND_OPTION                                0x0820000c,    16,      4
#define SRE_BOND_OPTION_PCIE                           0x0820000c,    20,      1
#define SRE_BOND_OPTION_POR_BYPASS                     0x0820000c,    21,      1

#define SRE_CPU_SRC_SEL_CSR                            0x08200010,     0,      1
#define SRE_CPU_SRC_FORCE                              0x08200010,     3,      1
#define SRE_CPU_SRC_SELECTED                           0x08200014,     0,      2

#define SRE_SPI_DISABLE                                0x08200018,    18,      1

#define MRE_PULSEREMOVALFRACTIONLDPC                   0x0c1030e0,    16,      8
#define MRE_LDPCCLKDISABLE                             0x0c1030e4,     0,      1
#define MRE_CPCDATAFLOWCLKDISABLE                      0x0c1030e4,     3,      1
#define MRE_DICDATAFLOWCLKDISABLE                      0x0c1030e4,     4,      1
#define MRE_PHYRXCLK200CH0DISABLE                      0x0c1030e4,     6,      1
#define MRE_PHYTXCLK200CH0DISABLE                      0x0c1030e4,     7,      1
#define MRE_PHYTXCLK600DISABLE                         0x0c1030e4,     8,      1
#define MRE_PHYCLK200CH0DISABLE                        0x0c1030e4,     9,      1
#define MRE_PHYCLKSYSDISABLE                           0x0c1030e4,    10,      1
#define MRE_DATAFLOWCSRCAMDISABLE                      0x0c1030e4,    12,      1
#define MRE_PHYRFICCLK200DISABLE                       0x0c1030e4,    13,      1
#define MRE_PHYCLK200CH1DISABLE                        0x0c1030e4,    16,      1
#define MRE_PHYDBGCLK400DISABLE                        0x0c1030e4,    17,      1
#define MRE_PHYRXCLK200CH1DISABLE                      0x0c1030e4,    22,      1
#define MRE_PHYTXCLK200CH1DISABLE                      0x0c1030e4,    23,      1

#define DFC_DICTCCLOCKGAP                              0x0cd48050,     8,      8
#define DFC_CPCTCCLOCKGAP                              0x0cd48050,    24,      8

#define KEYGEN_ENABLE                                  0x0c105028,     0,      1

#define MRX_DFE_MOCA_RX_CHN0_EN                        0x0c140010,     0,      1
#define MRX_DFE_MOCA_RX_CHN1_EN                        0x0c140014,     0,      1
#define MRX_DFE_MOCA_RX_CHN0_CLK_EN                    0x0c140050,     0,      1
#define MRX_DFE_MOCA_RX_CHN1_CLK_EN                    0x0c140054,     0,      1
#define MRX_DFE_MOCA_RX_DCCAL_FREEZE                   0x0c140074,     0,      1
#define MRX_DFE_MOCA_RX_GA_INIT                        0x0c1400c0,     0,      7
#define MRX_DFE_MOCA_RX_GA_INIT_APPLY                  0x0c1400c4,     0,      1
#define MRX_DFE_MOCA_RX_WB_EN                          0x0c140270,     0,      1

#define MTX_DFE_MOCA_TX_DYN_EN0                        0x0c142008,     0,      1
#define MTX_DFE_MOCA_TX_DYN_EN1                        0x0c14200c,     0,      1
#define MTX_DFE_MOCA_TX_CLK_EN0                        0x0c142010,     0,      1
#define MTX_DFE_MOCA_TX_CLK_EN1                        0x0c142014,     0,      1

#define RADIO_D2A_MOCA_ADC_ADC_EN                      0x0c14c000,     0,      3
#define RADIO_D2A_MOCA_RFFE_MASTER_EN                  0x0c14c004,     0,      1
#define RADIO_D2A_MOCA_RFFE_SPARE2                     0x0c14c00c,     0,      4

#define RADIO_D2A_MOCA_TXDRVR_ENA                      0x0c14c014,     0,      1
#define RADIO_D2A_MOCA_TXDRVR_BO                       0x0c14c018,     0,      4
#define RADIO_D2A_MOCA_TXDRVR_AWAKE                    0x0c14c034,     0,      1

#define RADIO_D2A_MOCA_DAC_CLK_ENA                     0x0c14c048,     0,      1
#define RADIO_D2A_MOCA_DAC_RSTB                        0x0c14c044,     0,      1

#define RADIO_D2A_REFSX_MOCA_ADC_CLK_ENA               0x0c14c0b4,    14,      1
#define RADIO_D2A_REFSX_MOCA_DAC_CLK_ENA               0x0c14c0b4,    15,      1

#define RADIO_D2A_MOCA_DAC_REGD_ENA                    0x0c14c068,     0,      1
#define RADIO_D2A_MOCA_DAC_AWAKE                       0x0c14c078,     0,      1

#define RADIO_D2A_MOCA_ADC_BIAS_EN                     0x0c14c0e0,    20,      1
#define RADIO_D2A_MOCA_ADC_REPLICA_EN                  0x0c14c0e0,    21,      1
#define RADIO_D2A_MOCA_ADC_CML2CMOS_EN                 0x0c14c0e0,    31,      1
#define RADIO_D2A_MOCA_ADC_REG4_EN                     0x0c14c0e4,     5,      1
#define RADIO_D2A_MOCA_ADC_REG4_EN_LEAK1               0x0c14c0e4,     6,      1
#define RADIO_D2A_MOCA_ADC_REG1_EN                     0x0c14c0e4,    20,      1
#define RADIO_D2A_MOCA_ADC_REG1_EN_LEAK1               0x0c14c0e4,    21,      1
#define RADIO_D2A_MOCA_ADC_REG2_EN                     0x0c14c0e8,     0,      1
#define RADIO_D2A_MOCA_ADC_REG2_EN_LEAK1               0x0c14c0e8,     1,      1

#define PHY_CTRL_PRCM_ADC_RST_N                        0x0c14e000,    16,      1

enum
{
    OPT_MOCA_VER                                 = 1 ,//1
    OPT_TEST_MODE                                    ,//
    OPT_CLNKLED_MODE                                 ,//
    OPT_TESTPARAM1                                   ,//
    OPT_TESTPARAM2                                   ,//
    OPT_TESTPARAM3                                   ,//
    OPT_TESTPARAM4                                   ,//
    OPT_TURBO_MODE                                   ,//
    OPT_TURBO_MODE_D                                 ,//
    OPT_TURBO_MODE_D_LO                              ,//
    OPT_TURBO_MODE_D_HI                              ,//10
    OPT_TURBO_MODE_E                                 ,//
    OPT_TURBO_MODE_F_SAT                             ,//
    OPT_TURBO_MODE_F_CBL                             ,//
    OPT_TURBO_MODE_H                                 ,//
    OPT_TURBO_MODE_CUSTOM                            ,//
    OPT_MII_PPRIO_LEVEL                              ,//
    OPT_MII_SPEED                                    ,//
    OPT_POLICING_MODE                                ,//
    OPT_POLICING_MODE_D                              ,//
    OPT_POLICING_MODE_D_LO                           ,//20
    OPT_POLICING_MODE_D_HI                           ,//
    OPT_POLICING_MODE_E                              ,//
    OPT_POLICING_MODE_F_SAT                          ,//
    OPT_POLICING_MODE_F_CBL                          ,//
    OPT_POLICING_MODE_H                              ,//
    OPT_POLICING_MODE_CUSTOM                         ,//
    OPT_PQOS_CLASS_MODE                              ,//
    OPT_RETRANS_PRIO                                 ,//
    OPT_RETRANS_PRIO_D                               ,//
    OPT_RETRANS_PRIO_D_LO                            ,//30
    OPT_RETRANS_PRIO_D_HI                            ,//
    OPT_RETRANS_PRIO_E                               ,//
    OPT_RETRANS_PRIO_F_SAT                           ,//
    OPT_RETRANS_PRIO_F_CBL                           ,//
    OPT_RETRANS_PRIO_H                               ,//
    OPT_RETRANS_PRIO_CUSTOM                          ,//
    OPT_TRANS_PRIO_PER                               ,//
    OPT_TRANS_PRIO_PER_D                             ,//
    OPT_TRANS_PRIO_PER_D_LO                          ,//
    OPT_TRANS_PRIO_PER_D_HI                          ,//40
    OPT_TRANS_PRIO_PER_E                             ,//
    OPT_TRANS_PRIO_PER_F_SAT                         ,//
    OPT_TRANS_PRIO_PER_F_CBL                         ,//
    OPT_TRANS_PRIO_PER_H                             ,//
    OPT_TRANS_PRIO_PER_CUSTOM                        ,//
    OPT_FAILOVER_MODE                                ,//
    OPT_HANDOFF_MODE                                 ,//
    OPT_INITIAL_ROLE                                 ,//
    OPT_LMO_MODE                                     ,//
    OPT_MOCA_PASSWD                                  ,//50
    OPT_MOCA_PASSWD_D                                ,//
    OPT_MOCA_PASSWD_D_LO                             ,//
    OPT_MOCA_PASSWD_D_HI                             ,//
    OPT_MOCA_PASSWD_E                                ,//
    OPT_MOCA_PASSWD_F_SAT                            ,//
    OPT_MOCA_PASSWD_F_CBL                            ,//
    OPT_MOCA_PASSWD_H                                ,//
    OPT_MOCA_PASSWD_CUSTOM                           ,//
    OPT_MOCASEQNUMMR                                 ,//
    OPT_MOCASEQNUMMR_D                               ,//60
    OPT_MOCASEQNUMMR_D_LO                            ,//
    OPT_MOCASEQNUMMR_D_HI                            ,//
    OPT_MOCASEQNUMMR_E                               ,//
    OPT_MOCASEQNUMMR_F_SAT                           ,//
    OPT_MOCASEQNUMMR_F_CBL                           ,//
    OPT_MOCASEQNUMMR_H                               ,//
    OPT_MOCASEQNUMMR_CUSTOM                          ,//
    OPT_PREFER_NC_MODE                               ,//
    OPT_SECURITY_MODE                                ,//
    OPT_SECURITY_MODE_D                              ,//70
    OPT_SECURITY_MODE_D_LO                           ,//
    OPT_SECURITY_MODE_D_HI                           ,//
    OPT_SECURITY_MODE_E                              ,//
    OPT_SECURITY_MODE_F_SAT                          ,//
    OPT_SECURITY_MODE_F_CBL                          ,//
    OPT_SECURITY_MODE_H                              ,//
    OPT_SECURITY_MODE_CUSTOM                         ,//
    OPT_NET_SEARCH_MODE                              ,//
    OPT_LOF                                          ,//
    OPT_LOF_UPDATE_MODE                              ,//80
    OPT_PROF_C_GAP_MODE                              ,//
    OPT_FREQ_BAND_MASK                               ,//    
    OPT_SCAN_MASK                                    ,//
    OPT_SCAN_MASK_D                                  ,//
    OPT_SCAN_MASK_D_LO                               ,//
    OPT_SCAN_MASK_D_HI                               ,//
    OPT_SCAN_MASK_E                                  ,//
    OPT_SCAN_MASK_F_SAT                              ,//
    OPT_SCAN_MASK_F_CBL                              ,//
    OPT_SCAN_MASK_H                                  ,//
    OPT_SCAN_MASK_CUSTOM                             ,//90
    OPT_SCAN_OFFSET                                  ,//
    OPT_SCAN_OFFSET_D                                ,//
    OPT_SCAN_OFFSET_D_LO                             ,//
    OPT_SCAN_OFFSET_D_HI                             ,//
    OPT_SCAN_OFFSET_E                                ,//
    OPT_SCAN_OFFSET_F_SAT                            ,//
    OPT_SCAN_OFFSET_F_CBL                            ,//
    OPT_SCAN_OFFSET_H                                ,//
    OPT_SCAN_OFFSET_CUSTOM                           ,//
    OPT_PRIM_CHAN_ABOVE                              ,//100
    OPT_PRIM_CHAN_ABOVE_D                            ,//
    OPT_PRIM_CHAN_ABOVE_D_LO                         ,//
    OPT_PRIM_CHAN_ABOVE_D_HI                         ,//
    OPT_PRIM_CHAN_ABOVE_E                            ,//
    OPT_PRIM_CHAN_ABOVE_F_SAT                        ,//
    OPT_PRIM_CHAN_ABOVE_F_CBL                        ,//
    OPT_PRIM_CHAN_ABOVE_H                            ,//
    OPT_PRIM_CHAN_ABOVE_CUSTOM                       ,//
    OPT_PRIM_CHAN_BELOW                              ,//
    OPT_PRIM_CHAN_BELOW_D                            ,//110
    OPT_PRIM_CHAN_BELOW_D_LO                         ,//
    OPT_PRIM_CHAN_BELOW_D_HI                         ,//
    OPT_PRIM_CHAN_BELOW_E                            ,//
    OPT_PRIM_CHAN_BELOW_F_SAT                        ,//
    OPT_PRIM_CHAN_BELOW_F_CBL                        ,//
    OPT_PRIM_CHAN_BELOW_H                            ,//
    OPT_PRIM_CHAN_BELOW_CUSTOM                       ,//
    OPT_SECO_CHAN_ABOVE                              ,//
    OPT_SECO_CHAN_ABOVE_D                            ,//
    OPT_SECO_CHAN_ABOVE_D_LO                         ,//120
    OPT_SECO_CHAN_ABOVE_D_HI                         ,//
    OPT_SECO_CHAN_ABOVE_E                            ,//
    OPT_SECO_CHAN_ABOVE_F_SAT                        ,//
    OPT_SECO_CHAN_ABOVE_F_CBL                        ,//
    OPT_SECO_CHAN_ABOVE_H                            ,//
    OPT_SECO_CHAN_ABOVE_CUSTOM                       ,//
    OPT_SECO_CHAN_BELOW                              ,//
    OPT_SECO_CHAN_BELOW_D                            ,//
    OPT_SECO_CHAN_BELOW_D_LO                         ,//
    OPT_SECO_CHAN_BELOW_D_HI                         ,//130
    OPT_SECO_CHAN_BELOW_E                            ,//
    OPT_SECO_CHAN_BELOW_F_SAT                        ,//
    OPT_SECO_CHAN_BELOW_F_CBL                        ,//
    OPT_SECO_CHAN_BELOW_H                            ,//
    OPT_SECO_CHAN_BELOW_CUSTOM                       ,//
    OPT_TABOO_MASK                                   ,//
    OPT_TABOO_OFFSET                                 ,//
    OPT_TLP_MIN                                      ,//
    OPT_TLP_MIN_D                                    ,//
    OPT_TLP_MIN_D_LO                                 ,//140
    OPT_TLP_MIN_D_HI                                 ,//
    OPT_TLP_MIN_E                                    ,//
    OPT_TLP_MIN_F_SAT                                ,//
    OPT_TLP_MIN_F_CBL                                ,//
    OPT_TLP_MIN_H                                    ,//
    OPT_TLP_MIN_CUSTOM                               ,//
    OPT_TLP_MAX                                      ,//
    OPT_TLP_MAX_D                                    ,//
    OPT_TLP_MAX_D_LO                                 ,//
    OPT_TLP_MAX_D_HI                                 ,//150
    OPT_TLP_MAX_E                                    ,//
    OPT_TLP_MAX_F_SAT                                ,//
    OPT_TLP_MAX_F_CBL                                ,//
    OPT_TLP_MAX_H                                    ,//
    OPT_TLP_MAX_CUSTOM                               ,//
    OPT_BEACON_TX_POW                                ,//
    OPT_ENCODINGWORDLENNPER                          ,//
    OPT_ENCODINGWORDLENVLPER                         ,//
    OPT_FEATUREFLAGS                                 ,//
    OPT_MAX_TX_POW                                   ,//160
    OPT_BITLOADMARGIN50                              ,//
    OPT_BITLOADMARGIN50_D                            ,//
    OPT_BITLOADMARGIN50_D_LO                         ,//
    OPT_BITLOADMARGIN50_D_HI                         ,//
    OPT_BITLOADMARGIN50_E                            ,//
    OPT_BITLOADMARGIN50_F_SAT                        ,//
    OPT_BITLOADMARGIN50_F_CBL                        ,//
    OPT_BITLOADMARGIN50_H                            ,//
    OPT_BITLOADMARGIN50_CUSTOM                       ,//
    OPT_BITLOADMARGINNPER                            ,//170
    OPT_BITLOADMARGINNPER_D                          ,//
    OPT_BITLOADMARGINNPER_D_LO                       ,//
    OPT_BITLOADMARGINNPER_D_HI                       ,//
    OPT_BITLOADMARGINNPER_E                          ,//
    OPT_BITLOADMARGINNPER_F_SAT                      ,//
    OPT_BITLOADMARGINNPER_F_CBL                      ,//
    OPT_BITLOADMARGINNPER_H                          ,//
    OPT_BITLOADMARGINNPER_CUSTOM                     ,//
    OPT_BITLOADMARGINVLPER                           ,//
    OPT_BITLOADMARGINVLPER_D                         ,//180
    OPT_BITLOADMARGINVLPER_D_LO                      ,//
    OPT_BITLOADMARGINVLPER_D_HI                      ,//
    OPT_BITLOADMARGINVLPER_E                         ,//
    OPT_BITLOADMARGINVLPER_F_SAT                     ,//
    OPT_BITLOADMARGINVLPER_F_CBL                     ,//
    OPT_BITLOADMARGINVLPER_H                         ,//
    OPT_BITLOADMARGINVLPER_CUSTOM                    ,//
    OPT_TPCMODE                                      ,//
    OPT_TPCMODE_D                                    ,//
    OPT_TPCMODE_D_LO                                 ,//190
    OPT_TPCMODE_D_HI                                 ,//
    OPT_TPCMODE_E                                    ,//
    OPT_TPCMODE_F_SAT                                ,//
    OPT_TPCMODE_F_CBL                                ,//
    OPT_TPCMODE_H                                    ,//
    OPT_TPCMODE_CUSTOM                               ,//
    OPT_TARGETP2PRATE50                              ,//
    OPT_TARGETP2PRATENPER                            ,//
    OPT_TARGETP2PRATEVLPER                           ,//
    OPT_STRIPVLANTAG                                 ,//200
    OPT_MPAENABLE                                    ,//
    OPT_ENHANCEDENABLE                               ,//
    OPT_TIMEMISMATCHCAL                              ,//
    OPT_MPA_MACADDR                                  ,//
    OPT_MPA_PKTFILTER                                ,//
    OPT_FIRSTCHANNELOFFSET                           ,//
    OPT_FIRSTCHANNELOFFSET_D                         ,//
    OPT_FIRSTCHANNELOFFSET_D_LO                      ,//
    OPT_FIRSTCHANNELOFFSET_D_HI                      ,//
    OPT_FIRSTCHANNELOFFSET_E                         ,//210
    OPT_FIRSTCHANNELOFFSET_F_SAT                     ,//
    OPT_FIRSTCHANNELOFFSET_F_CBL                     ,//
    OPT_FIRSTCHANNELOFFSET_H                         ,//
    OPT_FIRSTCHANNELOFFSET_CUSTOM                    ,//
    OPT_NUMBEROFCHANNELS                             ,//
    OPT_NUMBEROFCHANNELS_D                           ,//
    OPT_NUMBEROFCHANNELS_D_LO                        ,//
    OPT_NUMBEROFCHANNELS_D_HI                        ,//
    OPT_NUMBEROFCHANNELS_E                           ,//
    OPT_NUMBEROFCHANNELS_F_SAT                       ,//220
    OPT_NUMBEROFCHANNELS_F_CBL                       ,//
    OPT_NUMBEROFCHANNELS_H                           ,//
    OPT_NUMBEROFCHANNELS_CUSTOM                      ,//
    OPT_LOADING_CAPS                                 ,//
    OPT_MAC_ADDR_AGING_TIME                          ,//
    OPT_BCN_PWR_ENABLE                               ,//
    OPT_BCN_PWR_DIST                                 ,//
    OPT_BCN_PWR_LOCAL                                ,//
    OPT_PRIVACY_SUPPORTED                            ,//
    OPT_ENHANCED_PASSWORD                            ,//230
    OPT_NETWORK_NAME                                 ,// NETWORK_NAME/NC_NN 
    OPT_NETWORK_NAME_ADMIT_CFG                       ,// NETWORK_NAME/ADMIT
    OPT_ME_NETWORK_IE_PAYLOAD_TX                     ,// ME_NETWORK_IE/PAYLOAD_TX
    OPT_ME_NETWORK_IE_RESP_TIMEOUT                   ,// ME_NETWORK_IE/RESP_TIMEOUT   
    OPT_ME_NETWORK_JOIN_EN                           ,// ME_NETWORK/JOIN Enable
    OPT_MPS_PRIVACY_DOWN                             ,//
    OPT_MPS_PRIVACY_RECEIVE                          ,//
    OPT_MPS_UNPAIRED_TIME                            ,//
    OPT_MPS_WALK_TIME                                ,//
    OPT_AUTH_LINK_ETHERTYPE                          ,//
    OPT_AUTH_LINK_BITMASK                            ,//240
    OPT_AUTH_LINK_BITMASK_0                          ,//
    OPT_AUTH_LINK_BITMASK_1                          ,//
    OPT_AUTH_LINK_BITMASK_2                          ,//
    OPT_AUTH_LINK_BITMASK_3                          ,//
    OPT_AUTH_LINK_BITMASK_4                          ,//
    OPT_AUTH_LINK_BITMASK_5                          ,//
    OPT_AUTH_LINK_BITMASK_6                          ,//
    OPT_AUTH_LINK_BITMASK_7                          ,//
    OPT_AUTH_LINK_BITMASK_8                          ,//
    OPT_AUTH_LINK_BITMASK_9                          ,//250
    OPT_AUTH_LINK_BITMASK_10                         ,//
    OPT_AUTH_LINK_BITMASK_11                         ,//
    OPT_AUTH_LINK_BITMASK_12                         ,//
    OPT_AUTH_LINK_BITMASK_13                         ,//
    OPT_AUTH_LINK_BITMASK_14                         ,//
    OPT_AUTH_LINK_BITMASK_15                         ,//
    OPT_SGMII0_CONFIG                                ,//
    OPT_SGMII1_CONFIG                                ,//
    OPT_TWOKPE                                       ,//
    OPT_PORT0_DISABLE                                ,//260
    OPT_PORT1_DISABLE                                ,//
    OPT_PORT_MOCA_DISABLE                            ,//    
    OPT_TRACE_ENABLE                                 ,//
    OPT_TRACE_LEVEL                                  ,//
    OPT_TRACE_MODULE                                 ,//
    OPT_TRACE_DEST                                   ,//
    OPT_TRACE_HOST_CTL                               ,//
    OPT_HANDOFF_TO_LOWERMOCAVER                      ,//    
    OPT_CLNK_MAX_OPTIONS                              //
};

#define OPT_CLNK_WORDS  ((OPT_CLNK_MAX_OPTIONS+31)/32)

typedef struct
{
  uint32_t words[OPT_CLNK_WORDS];
}
clnk_param_mask_t;

#define CLEAR_PARAM_BITS(mask) do               \
{                                               \
  memset(mask, 0, sizeof(*mask));               \
} while (0)
#define SET_ALL_PARAM_BITS(mask) do             \
{                                               \
  memset(mask, 0xff, sizeof(*mask));            \
} while (0)
#define GET_PARAM_BIT(mask, opt) (((mask)->words[(opt)/32] & (1 << ((opt)%32))) != 0)
#define SET_PARAM_BIT(mask, opt) do             \
{                                               \
  (mask)->words[(opt)/32] |= (1 << ((opt)%32)); \
} while (0)

/*******************************************************************************
 *      Macros
 ******************************************************************************/
#define MXL_MOCA_LSB_MASK(bitWidth)     ((bitWidth) < (uint8_t) 32 ? (((uint32_t) 1 << (bitWidth)) - (uint32_t) 1) : 0xFFFFFFFF)
#define ClnkLib_UpdateRegBitField(a,b,c)  ClnkLib_UpdateRegisterField(a,b,c)

/*
 * USER CONFIGURABLES
 */

#define  CLNKLIB_MAX_DEV_NUM    29

///using in printf("%02x%02x:%02x:%02x:%02x:%02x",mac0,mac1,mac2,mac3,mac4,mac5);
#define GET_FLAGS(f, p)  ((p < 33) ? (f.lo&(1<<(p-1))):(f.hi&(1<<(p-33))))
#define SET_FLAGS(f, p)  ((p < 33) ? (f.lo |= (1<<(p-1))):(f.hi |= (1<<(p-33))))
#define FLAGS_IS_EMPTY(f) ((f).lo == 0 && (f).hi == 0)

// Count elements in an array at compile time
#define APPS_ARRAY_LENGTH(a)         (sizeof(a) / sizeof(*(a)))

#define GET_ARRAY_LEN(_array) (sizeof(_array)/sizeof(*(_array)))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

#define SWAPLONG(x) (((x & 0x00000000ffffffff) << 32) | ((x & 0xffffffff00000000) >> 32))

/** Created to mark simplificant modifications done duriung Jaws
 *  Pre Alpha    BRADMBX */

#define APPS_LOG(_fmt, _args...) do                               \
{                                                                 \
    printf("%s(%d): " _fmt "\n", __FILE__, __LINE__, ##_args );   \
    /*sleep(1);*/                                                 \
} while (0)

#define APPS_COMMENT(_fmt, _args...)

#ifdef DBG_APPS
#define APPS_FATAL_IF(_cond, _fmt, _args...) do                       \
{                                                                     \
    if (_cond)                                                        \
    {                                                                 \
        printf(" %s(%d): FATAL: " _fmt " (condition " #_cond ")\n",   \
               __FILE__, __LINE__, ##_args );                         \
        exit(EXIT_FAILURE);                                           \
    }                                                                 \
} while (0)
#else
#define APPS_FATAL_IF(_cond, _fmt, _args...) do                       \
{                                                                     \
    if (_cond)                                                        \
    {                                                                 \
        printf(_fmt "\n", ##_args );                                  \
        exit(EXIT_FAILURE);                                           \
    }                                                                 \
} while (0)
#endif

#define APPS_FATAL_UNLESS(_cond, _fmt, _args...) APPS_FATAL_IF(!(_cond), _fmt, ##_args)
#define APPS_FATAL(_fmt, _args...) APPS_FATAL_IF(1, _fmt, ##_args)
#define APPS_FATAL_PTRNULL(_ptr) APPS_FATAL_IF(NULL == (_ptr), "null pointer")

/* APPS_ERROR_... same as APPS_FATAL_ except return CLNK_ERR or CLNK_OK instead of
                    exiting. This allows the app to exit in orderly fashion */
#ifdef DBG_APPS
#define APPS_ERROR_IF(_cond, _fmt, _args...) (                        \
{                                                                     \
    int32_t e = 0;                                                      \
    if (_cond)                                                        \
    {                                                                 \
        printf(" %s(%d): FATAL: " _fmt " (condition " #_cond ")\n",   \
               __FILE__, __LINE__, ##_args );                         \
        e = -1;                                                       \
    }                                                                 \
     e;                                                               \
} )
#else
#define APPS_ERROR_IF(_cond, _fmt, _args...) (                        \
{                                                                     \
    int32_t e = 0;                                                      \
    if (_cond)                                                        \
    {                                                                 \
        printf(_fmt "\n", ##_args );                                  \
        e = -1;                                                       \
    }                                                                 \
     e;                                                               \
} )
#endif

#define APPS_ERROR_UNLESS(_cond, _fmt, _args...) APPS_ERROR_IF(!(_cond), _fmt, ##_args)
#define APPS_ERROR(_fmt, _args...) APPS_ERROR_IF(1, _fmt, ##_args)
#define APPS_ERROR_PTRNULL(_ptr) APPS_ERROR_IF(NULL == (_ptr), "null pointer")


#ifndef MXL_ENABLE_DEBUG_PRINT
#define MXL_ENABLE_DEBUG_PRINT                0
#endif

#ifndef MXL_ENABLE_DEBUG_ENTER_EXIT_FUNCTION
#define MXL_ENABLE_DEBUG_ENTER_EXIT_FUNCTION  0
#endif

#if MXL_ENABLE_DEBUG_PRINT
#define DEBUG_PRINT(lev, fmt, args...) ClnkOsal_PrintLog(lev,"(%s)%d " fmt " \n", __func__, __LINE__, ##args)
#else
#define DEBUG_PRINT(lev, fmt, args...)
#endif

#if MXL_ENABLE_DEBUG_ENTER_EXIT_FUNCTION
#define MXL_ENTER_FUNCTION(fmt, args...) ClnkOsal_PrintLog(L_INFO, "+++ (%s), line: %d " fmt "\n",__func__, __LINE__, ##args)
#define MXL_EXIT_FUNCTION(fmt, args...) ClnkOsal_PrintLog(L_INFO, "--- (%s), line: %d " fmt "\n",__func__, __LINE__, ##args)
#else
#define MXL_ENTER_FUNCTION(mod, fmt, args...)
#define MXL_EXIT_FUNCTION(mod, fmt, args...)
#endif

#define LIBAPI_COMMENT(_fmt, args...)
#define DEFINE_PREFIX(x)

#if MXL_ENABLE_DEBUG_PRINT
#define LIBAPI_LOG(_fmt, args...) DEBUG_PRINT(L_INFO, _fmt, ##args)
#define LIBAPI_VERBOSE(_fmt, args...) DEBUG_PRINT(L_VERBOSE, _fmt, ##args)
#else
#define LIBAPI_LOG(_fmt, args...) printf("%s(%d): LIBJPA_LOG: " _fmt "\n", __FILE__, __LINE__, ##args )
#define LIBAPI_VERBOSE(_fmt, args...)
#endif

/** During the Pre Alpha release of Jaws, c.LINK Daemon was
 *  heavily simplified in a temporary fashion to quickly
 *  enable all headline applcations 'clnkstat', 'clnkmem',
 *  'clnkcfg', etc to function as per external requirements.
 *  This macro marks places boundaries between jaws temporary
 *  development and the existing daemon design.  BRADMBX */
#define LIBAPI_OFFICIAL_DESIGN  0
typedef enum
{
    CCPU_IDX=0,
    CLINK_IDX,
    MCAST_IDX,
    RLAPM_IDX,
    ENDET_IDX,
    SAPM_IDX,
    RSSI_IDX,
    PLATFORM_IDX,
    MAX_FW_IDX,
}firmware_idx_t;

typedef struct
{
    uint32_t size;
    uint8_t *data;
    void   **pages;
} clnk_fw_entry_t;

typedef struct
{
    clnk_fw_entry_t *fw_entry[MAX_FW_IDX];
    uint32_t        ret;
} clnk_firmware_t;

/*****************************************************************************
 *      Types
 ******************************************************************************/

typedef struct
{
    uint32_t hi;
    uint32_t lo;
} flags_t;


typedef struct
{
    int mcastUseBackupOnly; //mcast
} comm_cfg_msg_struct;

typedef struct
{
    uint32_t diplexer;
    uint32_t miiClkDelay;
    uint32_t freq;
    uint32_t platHwCfg;
}ClnkLib_BoardInfo_t;

typedef enum
{
    //API Return Codes
    CLNK_OK                  = 0,
    CLNK_ERR                 = -1,
    CLNK_NODEV               = -2,
    CLNK_WHICHDEV            = -3,
    CLNK_TIMEOUT             = -4,
    CLNK_WRONGVER            = -5,
    CLNK_PERM                = -6,
    //c.LINK Common Return Codes for CD, driver
    CLNK_MEM_ALLOC_ERR       = -7,
    CLNK_RESET_ERR           = -8,
    CLNK_NOT_OPEN_ERR        = -9,
    CLNK_LINK_DOWN_ERR       = -10,
    CLNK_NO_HOST_DESC_ERR    = -11,
    CLNK_UCAST_FLOOD_ERR     = -12,
    CLNK_NO_KEY_ERROR        = -13,

    CLNK_RET_CODE_MAX        = -14   // This must always be last
} CLNK_RET_CODES;


enum
{
    NO_DATA_MAXLINEAR, //'MAXLINEAR' ends conflict with linux networking 'NO_DATA' sym
    IQ_RX_I_COEF,
    IQ_RX_Q_COEF,
    IQ_RX_DELAY,
    IQ_RX_DBINS,
    IQ_RX_DSLOPE,
    IQ_TX_I_COEF,
    IQ_TX_Q_COEF,
    IQ_TX_DBINS,
    IQ_TX_DSLOPE,
    FREQ_OFFSET,
    RANGE_VALID,
    NODE_DELAY,
    RX_CP_LEN,
    RX_CP_LEN2,
    RX_CHN_PRECUR_BIAS,
    TX_CP_LEN,
    AGC_ANALOG_GAIN,
    AGC_FRONTEND_GAIN,
    AGC_DIGI_GAIN,
    RX_BITLOADING,
    RX_EVM,
    RX_NUM_SYMBOLS,
    RX_BITS_PER_SYMBOL,
    RX_CHN_USABLE,
    RX_PREAMBLE,
    RX_TPC_BACKOFF,
    TX_BITLOADING,
    TX_NUM_SYMBOLS,
    TX_BITS_PER_SYAMBOL,
    TX_CHN_USABLE,
    TX_PREAMBLE,
    TX_TPC_BACKOFF,
};

enum
{
    //normal operation
    JAWS_NORMAL_OPERATION            = 0,

    //continuous transmit mode
    JAWS_HARDWARE_TEST_MODE          = 1,

    //continuous wave mode
    JAWS_CONTINUOUS_WAVE_MODE        = 2,

    /* no SoC download mode
     * freqIdx = testparam1[6:0] with freqIdx = (freq(MHz)-450)/12.5
     * vcoMode = testparam1[9:8]
     * 0x0 = normal mode (no override)
     * 0x1 = synth1
     * 0x2 = synth2
     * 0x3 = TBD
     */
    JAWS_NO_SOC_DOWNLOAD_MODE        = 3,

    //no health monitoring mode
    JAWS_NO_HEALTH_MONITOR_MODE      = 4,
    
    //test harness mode
    JAWS_TWOTONE_MODE                = 5,
    JAWS_DIPLEXER_CAL_MODE           = 6,
    JAWS_CONSTANT_TRANSMIT_50        = 7,

    /* new test mode, formally testmode=0 testparam1=0x8000
     * tpcBackoff = testparam1[5:0] 
     */
    JAWS_CONSTANT_TRANSMIT_100       = 8,

    /* new test mode, formally testmode=0 testparam1=0x11000
     * tpcBackoff = testparam1[5:0]
     */
    JAWS_CONSTANT_RECEIVE_100        = 9,

    /* testparam1 = log_gain (0 - 72)
     * testparam2 = test_mux_select (0 - 7)
     * if both testparam1 and testparam2 are == 0, default mode
     */
    JAWS_MAC_LOOPBACK_MODE           = 10,
    JAWS_TEST_TX_SNR                 = 11,

    /* tpcBackoff = testparam1[5:0]
     *numCarriers = testparam1[17:8] - default=4
     * toneMode = testparam1[20] - default=0, notchMode
     * carrier0 = testparam2[31:0] 
     */
    JAWS_TEST_PWRSUP_TRANSIENTS      = 12,

    JAWS_DIPLEXER_BHR_LAN_TX_MODE    = 13,
    JAWS_CONSTANT_RECEIVE_50         = 15,
    JAWS_CONSTANT_TRANSMIT_SEC_100   = 16,
    JAWS_CONSTANT_TRANSMIT_BOND_100  = 17, 
    SOC_CONSTANT_TRANSMIT_CHN2_100   = 18,  
    SOC_CONSTANT_TRANSMIT_CHN3_100   = 19,
    SOC_CONSTANT_TRANSMIT_CHN4_100   = 20,
    SOC_CONSTANT_TRANSMIT_5CHNS_100  = 21,
    SOC_CONSTANT_TRANSMIT_3CHNS_100  = 22,
    SOC_CONSTANT_TRANSMIT_4CHNS_100  = 23,
    SOC_CONSTANT_TRANSMIT_CHN1_100   = 25,
    SOC_CONSTANT_TRANSMIT_CHN0_100   = 26,
    MAX_TEST_MODE, 
};

#define MAX_NODES 16
#define MAX_OFDMA_TABLES 8
#define MAX_OFDMA_PROFILES 3
#ifndef IFNAMSIZ
//#define    IFNAMSIZ    16
#endif
#define MAX_BRIDGE_ENTRIES  192
// QOSTODO: shrink this for memory savings?
#define MAX_FLOWS_IN_LIST 32
#define MAX_FILE_NAME               256
#define DEFAULT_CLINK_CONFIG_PATH  "/etc/clink/"
#define DEFAULT_FW_PATH            "/lib/firmware/"

#if FEATURE_SAPM
#define SS_CARRIER_DISABLED     0xff  // force bit loading of 0
#define SS_THRESHOLD_MIN        0     // valid min threshold value
#define SS_THRESHOLD_MAX        65    // valid max threshold value
#endif

///model for read configuration file
#ifndef     USE_BACKUP_ONLY
///read backup configuration file only
#define     USE_BACKUP_ONLY    1
#endif
#ifndef     USE_BACKUP_FIRST
///first read backup configuration file, then override with current configuration file
#define     USE_BACKUP_FIRST   0
#endif

///Table 7.9.6 Multicast Filter Configuration
//
#define CFG_MCAST_FILTER_MODE       "filtermode"
///there could be 1-48 entries prefiexed with string  below
#define CFG_MCAST_MAC_PREFIX        "mcastmacaddr"


// Energy detect array dimensions
#define MAX_ATSC_CATV_FREQS     4
#define MAX_ATSC_CATV_GAPS      8
#define MAX_WEIGHTS             4
#define MAX_BTS_CSS_FREQS       6
#define MAX_BTS_CSS_VALUES      5

// Energy detection related constant
#define CATV_DETECTED 		1<<0
#define OTA_ATSC_DETECTED 	1<<1
#define CSS_DETECTED 		1<<2
#define BTS_DETECTED 		1<<3
#define NUM_BAND_E_100MHZ_RANK      5
#define NUM_BAND_F_100MHZ_RANK      6
#define NUM_BAND_D_100MHZ_RANK      19

#define NUM_BAND_E_50MHZ_RANK      7
#define NUM_BAND_F_50MHZ_RANK      8
#define NUM_BAND_D_50MHZ_RANK      21

// Spectrum Analyzer Definitions
#define MIN_SPECTRUM_FREQUENCY    400
#define MAX_SPECTRUM_FREQUENCY    1750

// Production and BondOption register Definitions, only for API function usage
#define SYS_RES_PRODUCT_FAMILY_ID 0x08200000
#define SYS_RES_EXT_CONFIG_STATUS 0x0820000c
#define SYS_RES_BOND_OPT_MASK     0x000f0000
#define SYS_RES_BOND_OPT_SKU_MASK 0x003f0000
#define SYS_RES_BOND_OPT_OFFSET   16

typedef unsigned char ClnkLib_NodeId_t;
typedef unsigned int ClnkLib_NodeMask_t;


typedef struct
{
    uint32_t           mocaVer;//default 20
    //    uint32_t           clinkGuidHi;                ///TODO  API document no such field
    //    uint32_t           clinkGuidLo;                ///TODO  API document no such field
    uint32_t           testMode;//def val 0
    uint32_t           clnkLedMode;//def val 1
    uint32_t           turboMode;//def val 0
    uint32_t           testParam1;
    uint32_t           testParam2;
    uint32_t           testParam3;
    uint32_t           testParam4;

    int32_t            miiPausePriLevel; //def val -1
    uint32_t           miiSpeed;                   ///i.e. XmiiSaf;
    uint32_t           policingMode;// defal disabled 0, enabled 1 using only 1bit
    uint32_t           pqosClassifyMode; //defval is moca1.1 classification(0), other val is pre upnp classification(1)
    /*
     *bit 3: high priority enable
     *bit 2: medium priority enable
     *bit 1: low priority enable
     *bit 0: background priority enable
     */
    uint32_t           retxPriority;//defal 0
    /*
     *bit 3: high priority (NPER(0) or VLPER(1))
     *bit 2: medium priority(NPER(0) or VLPER(1))
     *bit 1: low    priority(NPER(0) or VLPER(1))
     *bit 0: background priority(NPER(0) or VLPER(1))
     *
     */
    uint32_t           txPriorityPer;
    //    uint32_t datapathMode;             ///TODO API document no such field Jaws SoC Hi document has
    //    uint32_t xmiiSaf;                  ///TODO API document no such field Jaws SoC Hi document has


    //control parameters
    /**
     * this field is used only when the node is NC.
     *
     * disabled(0),
     * enabled(1)
     */
    uint32_t failoverMode;//defval 1
    /**
     * this field is used only when the node is NC.
     *
     * disabled(0),
     * enabled(1)
     */
    uint32_t handoffMode;//defval 1
    /**
     * this field is used only when the node is NC.
     *
     * disabled(0),
     * enabled(1)
     */
    uint32_t initialRole;//defval normal(0), NC only(1) client only(2)

    uint32_t lmoMode;//defval 1
    uint8_t mocaPasswd[18];//defval "99999999988888888"
    /**
     * the preferred NC mode for the c.Link device.
     * the values for preferred NC mode are follows:
     * disabled(0), enabled(1);
     */
    uint32_t  preferredNcMode;//defval disabled(0), enabled(1)
    /**
     * the security mode for c.Link device.
     * the values for security mode are follows:
     * disabled(0), enabled(1)
     */
    uint32_t securityMode;//defval disabled(0), enabled(1)
    /**
     * the network search mode for the c.Link device.
     * the values for network search mode are as follows:
     * disabled(0), enabled(1)
     */

    uint32_t networkSearchMode;//defval 1
    /**
     * the frequency plan for the c.Link device.
     * the values for freq plan are defined as below:
     * Band D(0), Extended Band D(1),
     * Sub-Band D-Low(2), Sub-Band D-High (3)
     * Band M(4)
     */
    uint32_t freqPlan; ///defval 1
    /**
     * the channel mask defines the "active" channels that user
     * has selected. the channel represents the center freq of
     * the beacon.
     *
     * For Extended Band D, the bits for channel mask are defined
     * as follows: bit 32(1150 MHz), bit 30(1175MHz), ... and bit
     * 12(1625 MHz).
     *
     * For Sub-Band D-Low, the bits for channel mask are defined as
     * follow: bit 31 (1175 MHz).
     *
     * For Sub-Band D-High, the bits for channel mask are define as
     * follow: bit 31 (1400 MHz), bit 30 (1425 MHz),...and bit 22
     * (1625 MHz)
     *
     * For Band M, the bits for channel mask are defined as follows:
     * bit 31 (500 MHz), bit 30 (525 MHz),...and bit 16 (875 MHz).
     *
     */
    uint32_t channelMask;///defval 0xfffff000

    /**
     * the last operating  freq(lof) for the c.Link device.
     *
     * the values for lof are:
     * none(-1) or the freq value(e.g. 1150 MHz)
     *
     * the default value for lof is different based on the freq plan.
     * the default values are as follows:
     *-----------------------------
     * |Frequency Plan  | Default |
     * ----------------------------
     * |Band D          |   -1    |
     * ----------------------------
     * |Extended Band D | 1150 MHz|
     * ----------------------------
     * |Sub-Band D-Low  | 1150 MHz|
     * ----------------------------
     * |Sub-Band D-High | 1400 MHz|
     * ----------------------------
     * |Band M          | 525 MHz |
     * ----------------------------
     */
    int32_t  lof;///defval is 1150
    /**
     * the primary channel offset is the center freq of the primary
     * channel relative to the channel for the beacon.
     * this field is used only when the Node is NC.
     */
    uint32_t       primChnOffset;/// Mask of primary channel offsets
    /**
     * the secondary channel offset is the center freq of the secondary
     * channel relative to the channel for the beacon.
     * this field is used only when the Node is NC.
     */
    uint32_t       secChnOffset;/// Mask of secondary channel offsets

    uint32_t tabooMask; //defval 0xee0000
    uint32_t tabooOffset; //defval 3
    uint32_t tlpMin;      //defval 0
    uint32_t tlpMax;      //defval 95

    uint32_t tpcMode;
    uint32_t tpcPhyRate50;
    uint32_t tpcPhyRateNper;
    uint32_t tpcPhyRateVlper;
    uint32_t encWordLenNper;
    uint32_t encWordLenVlper;
    /**
     * The beacon transmit power for the c.LINK device
     * when TPC is enabled.  The values for transmit power
     * are the same as defined for the Maximum TX Power
     * parameter
     */
    uint32_t beaconTxPower;///defval 10
    /**
     * The beacon transmit power for the c.LINK device
     * when TPC is enabled.  The values for transmit power
     * are the same as defined for the
     * Maximum TX Power parameter
     * Value    ECB (dBM)   Mini Card (dBM)
     * 0            -28         -31
     * 1            -25         -28
     * 2            -22         -25
     * 3            -19         -22
     * 4            -16         -19
     * 5            -13         -16
     * 6            -10         -13
     * 7            -7          -10
     * 8            -4          -7
     * 9            -1          -3
     * 10           +2           0
     *
     */
    uint32_t maxTxPower;///defval 10
    uint32_t phyMargin50;
    uint32_t phyMarginNper;
    uint32_t phyMarginVlper;
    uint32_t featureFlags;

    //added to indicate whether write to flash
    //int32_t          update;

    uint32_t mocaSeqNumMr;
} ClnkLib_StaticCfg_t;


///c.LINK Configuration Parameters
typedef MXL_MOCA_CFG_V2_T ClnkLib_ClnkCfgV2_t;

/***************************************************
 *Gets the current device status.
 *
 **************************************************/
typedef struct
{
    uint32_t status; /// Booting or active
    uint32_t resetCount;
} ClnkLib_DevStatus_t;

typedef struct
{
    uint32_t cdUpTimeInSecs;
    uint32_t linkUpTimeInSecs;
    uint32_t resetCount;
    uint32_t resetHistory;
    uint32_t resetLine;
    uint32_t resetEpc;
    uint32_t linkLostCount;  // link lost count record
    uint32_t drvVersionStr[MAX_HOST_VERSION_STR_SIZE / 4];
    uint32_t pkgVersionStr[MAX_HOST_VERSION_STR_SIZE / 4];
}
ClnkLib_CdInfo_t;

typedef MXL_MOCA_MCAST_CFG_T ClnkLib_McastCfg_t;

typedef struct
{
    uint32_t pktsTx2Node[MAX_NUM_NODES];
} ClnkLib_MacDataStats_t;

typedef struct
{
    //uint32_t   maxPktsPerAggr;
    uint32_t txAggr[MAX_AGGR_FRAMES_V25];
    uint32_t rxAggr[MAX_AGGR_FRAMES_V25];
} ClnkLib_AggrStats_t;

typedef struct
{
    uint32_t ucastDstEntries;
    uint32_t mcastDstEntries;
    uint32_t pqosDstEntries;
    //uint32_t totalSourceEntries;  changed to ucastSrcEntries and mcastSrcEntries
    uint32_t ucastSrcEntries;
    uint32_t mcastSrcEntries;
    uint32_t totalHits;
    uint32_t totalMisses;
} ClnkLib_BridgeStats_t;

typedef struct
{
    uint32_t macAddrHi;
    uint32_t macAddrLoIds;
} ClnkLib_BridgeEntry_t;

typedef struct
{
    //int32_t  numEntries;
    //char macAddr[16];
    ClnkLib_BridgeEntry_t   entry[MAX_BRIDGE_ENTRIES];
    //the two below do not need according to
    //table 16 Bridge Entry Element in
    //JawsHostCcpuInterface.028-TO_KEN.DOCX
    //clink_node_id_t  destNodeId;
    //flow_id_t destFlowId;
} ClnkLib_BridgeTable_t;


typedef struct
{
    uint32_t sgmii0Rx;
    uint32_t sgmii0Tx;
    uint32_t pcieRx;
    uint32_t pcieTx;    
    uint32_t sgmii1Rx;
    uint32_t sgmii1Tx;
    uint32_t mocaRx;
    uint32_t mocaTx;

    uint32_t debug0;
    uint32_t debug1;
    uint32_t debug2;
    uint32_t debug3;
    uint32_t debug4;
    uint32_t debug5;
    uint32_t debug6;
    uint32_t debug7;
    uint32_t debug8;
    uint32_t debug9;
    uint32_t debug10;
    uint32_t debug11;
    uint32_t debug12;
    uint32_t debug13;
    uint32_t debug14;
    uint32_t debug15;
    uint32_t debug16;
    
} ClnkLib_EcbStats_t;

typedef struct
{
    uint32_t miiPausePriLvl;
    uint32_t miiSpeed;
    uint32_t rxGood[2];
    uint32_t rxDrop[2];
    uint32_t rxError[2];
    uint32_t txGood[2];
    uint32_t txDrop[2];
    uint32_t txError[2];
} ClnkLib_EphyStats_t;

typedef struct
{
    uint32_t rxGood[4];
    uint32_t rxDrop[4];
    uint32_t rxError[4];
    uint32_t txGood[4];
    uint32_t txDrop[4];
    uint32_t txError[4];
} ClnkLib_SwitchStats_t;

typedef struct
{
    uint32_t macHi;
    uint32_t macLo;
    uint32_t tmo;
    uint32_t vlanCheck[2];
    uint32_t vlanStrip[2];
    uint32_t vlanPvid[2];
    uint32_t vlanId[2][SWITCH_PORT_MAX_VLAN_IDS];
} ClnkLib_SwitchConfig_t;


/* eSwitch config:
 * ---------------
 * eSwitch port control eSwitchPortConfig[2:0]
 *  A value of 1 in a given bit position indicates disable. Default port setting is 000'b
 *  Bit0 EPHY0(RGMII)
 *  Bit1 EPHY1(SGMII)
 *  Bit2 MoCA ifc.
 *  Other bits are reserved
 */
typedef struct

{  uint32_t eSwitchPortConfig;
} ClnkLib_SwitchPortConfig_t;  /* Hi_TxnSetSwitchPortConfigCmd_t */

typedef struct
{
    uint32_t retxFrames[MAX_NODES];
    uint32_t txTimeout[MAX_NODES];
} ClnkLib_RetxStats_t;

typedef struct
{
    uint64_t txTotalPkts;
    uint64_t txTotalBytes;
    uint64_t txDroppedPkts;
    uint64_t txBcastPkts;
    uint64_t txBcastBytes;
    uint64_t txMcastPkts;
    uint64_t txMcastBytes;
    uint64_t txUcastPkts[MAX_NUM_NODES];
    uint64_t txUcastBytes[MAX_NUM_NODES];
    uint64_t txGoodPktsInLast60s[MAX_NUM_NODES];
    uint64_t txDroppedPktsInLast60s[MAX_NUM_NODES];
    uint64_t rxTotalPkts;
    uint64_t rxTotalBytes;
    uint64_t rxDroppedPkts;
    uint64_t rxBcastPkts[MAX_NUM_NODES];
    uint64_t rxBcastBytes[MAX_NUM_NODES];
    uint64_t rxMcastPkts[MAX_NUM_NODES];
    uint64_t rxMcastBytes[MAX_NUM_NODES];
    uint64_t rxUcastPkts[MAX_NUM_NODES];
    uint64_t rxUcastBytes[MAX_NUM_NODES];
    uint64_t rxGoodPktsInLast60s[MAX_NUM_NODES];
    uint64_t rxBadPktsInLast60s[MAX_NUM_NODES];
} ClnkLib_DataStats_t;

typedef struct
{
    uint32_t rxFragFrames;
    uint32_t txFragFrames;
} ClnkLib_FragStats_t;

typedef struct
{
    uint32_t l2meIssuedTxns;
    uint32_t l2meSuccessfulTxns;
    uint32_t l2meTimedOutCycles;
    uint32_t l2meFailedTxns;
} ClnkLib_L2meStats_t;

typedef struct
{
    uint64_t txMapGood;
    uint64_t txMapError;
    uint64_t txMapDropped;
    uint64_t rxMapGood;
    uint64_t rxMapError;
    uint64_t rxMapDropped;
    uint64_t txRrGood;
    uint64_t txRrError;
    uint64_t txRrDropped;
    uint64_t rxRrGood;
    uint64_t rxRrError;
    uint64_t rxRrDropped;
    uint64_t txLcfGood;
    uint64_t txLcfError;
    uint64_t txLcfDropped;
    uint64_t rxLcfGood;
    uint64_t rxLcfError;
    uint64_t rxLcfDropped;
    uint64_t txBcnGood;
    uint64_t txBcnError;
    uint64_t txBcnDropped;
    uint64_t rxBcnGood;
    uint64_t rxBcnError;
    uint64_t rxBcnDropped;
    uint64_t txAcfGood;
    uint64_t txAcfError;
    uint64_t txAcfDropped;
    uint64_t rxAcfGood;
    uint64_t rxAcfError;
    uint64_t rxAcfDropped;
    uint64_t txProbeGood;
    uint64_t txProbeError;
    uint64_t txProbeDropped;
    uint64_t rxProbeGood;
    uint64_t rxProbeError;
    uint64_t rxProbeDropped;
    uint64_t txEthGood;
    uint64_t txEthError;
    uint64_t txEthDropped;
    uint64_t rxEthGood;
    uint64_t rxEthError;
    uint64_t rxEthDropped;

    uint64_t txOfdmaRrGood;
    uint64_t txOfdmaRrError;
    uint64_t txOfdmaRrDropped;
    uint64_t rxOfdmaRrGood;
    uint64_t rxOfdmaRrError;
    uint64_t rxOfdmaRrDropped;

    uint64_t txOfdmaEvmGood;
    uint64_t txOfdmaEvmError;
    uint64_t txOfdmaEvmDropped;
    uint64_t rxOfdmaEvmGood;
    uint64_t rxOfdmaEvmError;
    uint64_t rxOfdmaEvmDropped;
} ClnkLib_FrameStats_t;

typedef struct
{
    uint32_t nodeId;
    uint32_t ncNodeId;
    uint32_t nextNcNodeId;
    uint32_t backupNodeId;
    uint32_t lmoNodeId;
    uint32_t linkStatus;
    uint32_t lastAdmStatus;
    uint32_t beaconRxPwrLevel;  /* Beacon Rx power level (dBm) */
    uint32_t role;
    uint32_t networkState;
    uint32_t resetCnt;
    uint32_t networkMocaVer;
    uint32_t activeNodeBitmask;
    uint32_t sleepM1NodeBitmask;
    uint32_t sleepM2NodeBitmask;
    uint32_t sleepM3NodeBitmask;
    uint32_t tabooMask;
    uint32_t tabooOffset;
    int32_t  priChnOffset;
    int32_t  secChnOffset;
    uint32_t pqosFlowPolicingFlag;
    uint32_t socVersionStr[MAX_VERSION_STR_SIZE / 4];
    uint32_t reqStatus;
    uint32_t customBandProfCGapMode;
} ClnkLib_LocalInfo_t;

typedef struct
{
    //clink_guid_t  guid;
    uint32_t guidHi;
    uint32_t guidLo;
    uint32_t state;  // Inactive (0), assigned (1), MAP active (2), admitted (3),
    // active (4), low power idle (5), standby (6), sleep (7)
    uint32_t protoSupport;
    uint32_t mocaVer;
    uint64_t txFrameGood;
    uint64_t txFrameError;
    uint64_t txFrameDropped;
    uint64_t rxFrameGood;
    uint64_t rxFrameError;
    uint64_t rxFrameDropped;
    uint64_t txDataFrameGood;
    uint64_t txDataFrameDropped;
    uint64_t rxDataFrameGood;
    uint64_t rxDataFrameBad;
} ClnkLib_NetInfo_t;

typedef struct
{
    uint32_t status;
    uint32_t fatalCode;
    uint32_t state;
    uint32_t dbg;
    uint32_t bbTemp;
    uint32_t rfTemp;
} ClnkLib_SocStatus_t;


typedef struct
{
    uint8_t gapNper;
    uint8_t gapVLper;
    uint16_t ofdmbNper;
    uint16_t ofdmbVLper;
} ClnkLib_NodeFmrInfo_t;

typedef struct
{
    uint32_t numOfNodes;
    uint32_t nodeMask;
    struct
    {
        uint32_t validInfo;
        struct
        {
            uint8_t gapNper;
            uint8_t gapVLper;
            uint16_t ofdmbNper;
            uint16_t ofdmbVLper;
            uint16_t rateNper;
            uint16_t rateVLper;
        } p2p[MAX_NODES];
        uint8_t gapGcd;
        uint16_t ofdmbGcd;
        uint16_t rateGcd;
        uint8_t defTableNum;
        struct
        {
            uint32_t nodeBitmask;
            uint8_t subchannelNum;
            uint8_t gapOfdma;
            uint16_t ofdmab;
            uint16_t rateOfdma;
        } ofdma[3];
    } nodeFmr[MAX_NODES];
} ClnkLib_FmrInfo_t;

typedef enum
{
    CLNK_LIB_DD_DEV_ATTR_PQOS,
    CLNK_LIB_DD_DEV_ATTR_AGGR,
    CLNK_LIB_DD_DEV_ATTR_MAP,
    CLNK_LIB_DD_DEV_ATTR_BND_AGGR,
    CLNK_LIB_DD_DEV_ATTR_MOCA25_AGGR,
    CLNK_LIB_DD_DEV_ATTR_MAX,
    MAX_DD_IES = CLNK_LIB_DD_DEV_ATTR_MAX    /* # of Device discovery attrib */
}ClnkLib_DDdevAttrID_t;

typedef struct
{
    uint32_t numOfNodes;
    struct
    {
        uint32_t nodeId;
        uint32_t numOfAttr;
        struct
        {
            uint8_t attrId;
            uint8_t attrLen;
            union
            {
                struct
                {
                    uint8_t supportPqosIngressFlows;
                    uint8_t supportPqosEgressFlows;
                } ClnkLib_NodeDdIePqos;

                struct
                {
                    uint8_t pduNum;
                    uint8_t aggrSize;
                } ClnkLib_NodeDdIeRcvAggr;

                struct
                {
                    uint8_t aeNum;
                    uint8_t reserved2;
                } ClnkLib_NodeDdIeMapProc;
            } ddIe;
        } AttrData[MAX_DD_IES];
    } nodeDd[MAX_NODES];
   uint32_t nodeOffset; 
} ClnkLib_DdInfo_t;

typedef enum
{
    CLNK_LIB_ACA_TYPE_QUIET,  //!< Quiet line
    CLNK_LIB_ACA_TYPE_EVM     //!< EVM type
}
ClnkLib_AcaType_t;

//! ACA status codes
typedef enum
{
    CLNK_LIB_ACA_STATUS_SUCCESS,      //!< Operation successful
    CLNK_LIB_ACA_STATUS_TUNE_FAILED,  //!< Failed: Unable to tune to selected channel
    CLNK_LIB_ACA_STATUS_NO_PROBES,    //!< Failed: No EVM probe was detected
    CLNK_LIB_ACA_STATUS_GEN_ERROR     //!< Failed: Other reason
}
ClnkLib_AcaStatus_t;

//! Handoff request information
typedef struct
{
    uint32_t          nodeId;    //!< Source node ID
    uint32_t          isBnc;
}
ClnkLib_HandoffRequest_t;

//! LMO type
typedef enum
{
    CLNK_LIB_LMO_TYPE_OOO_LMO,         //!< Out of order LMO
    CLNK_LIB_LMO_TYPE_RX_DET_PROBE,    //!< Receiver determined probe
    CLNK_LIB_LMO_TYPE_UNSOL_PROBE_RPT, //!< Unsolicited probe report
    CLNK_LIB_LMO_TYPE_SILENT_PROBE,    //!< Silent probe report
    CLNK_LIB_LMO_TYPE_LB_PROBE,         //!< Loopback probe report
    CLNK_LIB_LMO_TYPE_ACA              //!< Alternate Channel Assessment
}
ClnkLib_LmoType_t;

//! LMO request information
typedef struct
{
    ClnkLib_LmoType_t type;    //!< LMO type
    union
    {

        struct
        {
            ClnkLib_AcaType_t type;         //!< Assessment type
            uint32_t          channel;      //!< Alternate channel
            uint32_t          srcNodeId;    //!< Source node ID
            uint32_t          dstNodeMask;  //!< Destination node bit mask
        } aca; // prev was ClnkLib_AcaRequest_t

        struct
        {
            uint32_t          nodeId;  //!< OOO lmo node ID
        } ooo;

        struct
        {
            uint32_t          type;     //!< 0 - Unicast, 1 OFDMA
            uint32_t          nodeMask; //!< Mask of nodes invloved
        } upr;

        struct
        {
            uint32_t          nodeId;  //!< Tx node Id
            uint32_t          probeId; //!< Pre-Defined Probe identifier
        } rdp;

        struct
        {
            uint32_t         timeSlots;
        } silent;

        struct
        {
            uint32_t         timeSlots;
        } loopBack;

    } data;
}
ClnkLib_LmoRequest_t;

//! LMO report information
typedef struct
{
    uint32_t          type;    //!< LMO type
    uint32_t          nodeId;  //!< LMO node ID
    uint32_t          result;

    union
    {
        struct
        {
            ClnkLib_AcaType_t   type;                //!< Assessment type
            ClnkLib_AcaStatus_t txStatus;            //!< TX Operation status
            ClnkLib_AcaStatus_t status;              //!< Operation status
            uint32_t            power;               //!< Total power
            uint32_t            relativePower;       //!< Total relative power
            uint8_t             powerProfile[LMO_ACA_RPT_PWR_PRFL_SZ];   //!< Received spectral power profile
        } aca;
    } data;

}
ClnkLib_LmoReport_t;



//! Topology report information
typedef struct
{
    uint32_t nodeId;         //!< Node ID for topology information
    uint32_t nperGap[16];
    uint32_t nperBitsPerSym[16];
    uint32_t vlperGap;
    uint32_t vlperBitsPerSym[16];
    uint32_t gcd50Gap;
    uint32_t gcd50BitsPerSym;
    uint32_t ofdmaNumTables;
    uint32_t ofdmaNodeBitmask[MAX_OFDMA_TABLES];
    uint32_t ofdmaNumSubChannels[MAX_OFDMA_TABLES];
    uint32_t ofdmaGap[MAX_OFDMA_TABLES];
    uint32_t ofdmaBitsPerSym[MAX_OFDMA_TABLES];
}
ClnkLib_TopoReport_t;


//! MoCA Reset request structure
typedef struct
{
    uint32_t   lof;
    uint32_t   nodemask;
    uint32_t   seconds;
    /*  Hi_TxnMocaResetRequest_t*/
} ClnkLib_MocaResetRequest_t;

//! MoCA Reset Code- result of operation attempt
typedef enum
{
    CLNKLIB_MRCODE_LIB_ERROR,     //!< No Reset Issued: library error
    CLNKLIB_MRCODE_SUCCESS_PART,  //!< Partial network reset issued and scheduled
    CLNKLIB_MRCODE_SUCCESS_NW,    //!< Entire network reset issued and scheduled
    CLNKLIB_MRCODE_FAIL_CLEAN,    //!< Nodes refused to reset, transaction completed
    CLNKLIB_MRCODE_FAIL_UNKNOWN1, //!< L2ME transaction did not complete correctly
    CLNKLIB_MRCODE_FAIL_UNKNOWN2, //!< L2ME transaction did not complete correctly
    CLNKLIB_MRCODE_FAIL_TIMEOUT,  //!< L2ME transaction did not take place
    CLNKLIB_MRCODE_FAIL_NO_NODES, //!< no nodes selected, no L2ME transaction
    /*     Hi_MRCode_t*/
} ClnkLib_MRCode_t;

//! MoCA Reset response structure
typedef struct
{
    uint32_t   mrCode;
    uint32_t   issuedNodemask;
    uint32_t   topLastResetStatus;
    uint32_t   nodeLastResetStatus[MAX_NODES];
    uint32_t   nodeLastResponseCode[MAX_NODES];
    /*  Hi_TxnMocaResetResponse_t*/
} ClnkLib_MocaResetResponse_t;


//! Maximum number of bytes for NMS response
//
//! NMS operation type
typedef enum
{
    CLNK_LIB_NMS_TYPE_REQUESTED
}
ClnkLib_NmsType_t;

//! NMS request information
typedef struct
{
    uint32_t reserved;  //!< Reserved
}
ClnkLib_NmsRequest_t;

//! NMS response information
typedef struct
{
    uint32_t result;
}
ClnkLib_NmsReport_t;

typedef struct
{
    uint32_t active;
    uint32_t assignmentTable[64];
    uint32_t bitloading[64];
    uint32_t maxDelay;
    uint32_t delayOffset;
    uint32_t ni;
    uint32_t k;
    uint32_t tpc;
    uint32_t cpLen;
}
ClnkLib_PhyOfdma_t;


typedef struct
{
    uint32_t txProfileValid;
    uint32_t iqTxCoef;
    uint32_t iqTxDelay;
    uint32_t iqTxDslope_AB;
    uint32_t iqTxDslope_CD;
    uint32_t txCpLen;
    uint32_t txNumSymbols;
    uint32_t txBitsPerSymbol;
    uint32_t txChnUsable;
    uint32_t txPreamble;
    uint32_t txTpcBackoff;
    uint32_t txKLdpcMax;
    uint32_t txPhyrate;
    uint32_t seqNum;
    uint32_t nodeMask;
    uint32_t txBitloading[64];
    /*      Hi_TxnGetPhyTxProfileRsp_t*/
} ClnkLib_PhyTxProfile_t;

typedef struct
{
    uint32_t rxProfileValid;
    uint32_t iqRxCoef;
    uint32_t iqRxDelay;
    uint32_t iqRxDslope_AB;
    uint32_t iqRxDslope_CD;
    uint32_t rxCpLen;
    uint32_t rxChnPrecurBias;
    uint32_t agc_b8_f8_digi16;
    uint32_t agc_b8_f8_digi3_16;
    uint32_t rxNumSymbols;
    uint32_t rxBitsPerSymbol;
    uint32_t rxChnUsable;
    uint32_t rxPreamble;
    uint32_t rxTpcBackoff;
    uint32_t rxKLdpcMax;
    uint32_t rxPhyrate;
    uint32_t rxPwrLevel;
    uint32_t seqNum;
    uint32_t rxBitloading[64];
    uint32_t rxChnSnr;
    /*      Hi_TxnGetPhyRxProfileRsp_t*/
} ClnkLib_PhyRxProfile_t;


typedef struct
{
    uint32_t valid;
    uint32_t freqOffset;
    uint32_t rangeValid;
    uint32_t nodeDelay;
    uint32_t validTxProf;
    /*      Hi_TxnGetPhyNodeRsp_t*/
} ClnkLib_PhyNode_t;

typedef struct
{
    uint32_t rxProfileValid;
    uint32_t rxEvm[128];
    /*      Hi_TxnGetPhyEvmRsp_t*/
} ClnkLib_PhyEvm_t;

typedef struct
{
    uint32_t temperature;
    uint32_t currentOperatingFrequency;
    /*      Hi_TxnGetPhyMiscRsp_t*/
} ClnkLib_PhyMisc_t;

typedef struct
{
    uint32_t temperature;
    uint32_t currentOperatingFrequency;
    uint32_t firstChannelFrequency;
    uint32_t numberM25Channels;
    /*      Hi_TxnGetM25PhyMiscRsp_t*/
} ClnkLib_M25PhyMisc_t;

typedef struct
{
    struct
    {
        uint32_t	subchanDefID;
        uint32_t	nodeBitmask;
        uint32_t	bitsPerSubchanelPerSym;
        uint32_t	gapOfdma;
        uint32_t	numSubchan;
    } profile[MAX_OFDMA_PROFILES];
    /*		Hi_TxnGetPhyOfdmaDefRsp_t*/
} ClnkLib_OfdmaDef_t;

typedef struct
{
    struct
    {
        uint32_t	frameID;
        uint32_t	subchanDefID;
        uint32_t	numSubchan;
        uint32_t	nodeBitmask;
        uint32_t	cpLen;
    } profile[MAX_OFDMA_PROFILES];
} ClnkLib_OfdmaAssign_t;
/*      Hi_TxnGetPhyOfdmaAssignRsp_t*/

typedef struct
{
    int32_t result;
    uint32_t ofdmaGrps;
    uint32_t m2NodeMask;
    uint32_t ofdmaNodeMask;
    /*Hi_TxnGetOfdmaParamsRsp_t*/
}  ClnkLib_OfdmaParams_t;

typedef struct
{
    int32_t result;
    uint32_t usedProfileTypeMask;
    uint32_t numAsgnTbls;
    uint32_t blt[64];
    uint32_t phyFrameId;
    uint32_t preambleType;
    uint32_t cpLen;
    uint32_t kLdpcMax;
    uint32_t rrUnitLength;
    uint32_t rateMultNi;
    uint32_t bitsPerSymbol;
    int32_t TPC;
    uint32_t propDelay;
    int32_t propDelayOffset;
    uint32_t propDelayMax;
    int32_t freqOffset;
    int32_t txIq[2];
    /*Hi_TxnGetOfdmaTxProfileRsp_t*/
}  ClnkLib_OfdmaTxProfile_t;

typedef struct
{
    int32_t result;
    uint32_t numAsgnTbls;
    uint32_t sumNi;
    uint32_t blt[64];
    uint32_t contextId[64];
    uint32_t phyFrameId;
    uint32_t clNodeMask;
    uint32_t preambleType;
    uint32_t cpLen;
    uint32_t kLdpcMax;
    uint32_t rrUnitLength;
    uint32_t rateMultNi[8];
    uint32_t bitsPerSymbolPerNi;
    int32_t TPC[8];
    uint32_t digitalGain;
    uint32_t analogGain;
    uint32_t feGain;
    uint32_t beGain;
    int32_t powerLevel;
    uint32_t propDelayMax;
    int32_t rxIq[2];
    int32_t rxIdDslope[4];
    /*Hi_TxnGetOfdmaRxProfileRsp_t*/
}  ClnkLib_OfdmaRxProfile_t;

typedef struct
{
    uint32_t powerState;
    uint32_t lastHbIvSecs;
    int32_t  ncHbIvExpiresInSecs;
    uint32_t clStartedHbIvTimerCount;
/*      Hi_TxnQueryPmStateRsp_t*/
} ClnkLib_QueryPmStateRsp_t;

typedef struct
{
    uint32_t nodeId;
    uint32_t isLocal;
    uint32_t state;
/*         Hi_PmSleepEventData_t*/
} ClnkLib_PmSleepEventData_t;

typedef struct
{
    uint32_t nodeId;
    uint32_t isLocal;
/*         Hi_PmWakeEventData_t*/
} ClnkLib_PmWakeEventData_t;

typedef struct
{
    uint32_t nodeId;
    uint32_t isLocal;
/*         Hi_PmWomEventData_t*/
} ClnkLib_PmWomEventData_t;

typedef union
{
    ClnkLib_PmSleepEventData_t   sleep;
    ClnkLib_PmWakeEventData_t    wake;
    ClnkLib_PmWomEventData_t     wom;
/*         Hi_PmEventData_t*/
} ClnkLib_PmEventData_t;

// QOSTODO: Scrub list of error codes
typedef enum
{
    /** The operation succeeded. */
    CLNKLIB_FLOWCODE_SUCCESS /*HI_FLOWCODE_SUCCESS*/ =                   0x01,

    /** The operation did not happen due to a network timeout during
     *  concurrent operations; in a typical network the application
     *  may retry the operation and expect the operation to happen. */
    CLNKLIB_FLOWCODE_TIMEOUT /*HI_FLOWCODE_TIMEOUT*/ =                   0x08,

    /** QoS services are not available since the network currently
     *  has nodes that are not QoS capable. */
    CLNKLIB_FLOWCODE_QOS_UNAVAIL /*HI_FLOWCODE_QOS_UNAVAIL*/ =           0x0c,

    /** A very infrequent error that must be trapped: a transaction
     *  requiring multiple node synchronization may fail in the event
     *  of a node failure at a critical time.  This can be due to
     *  rare transient events (a node leaves the network during a
     *  transaction) or the more sinister case of wire
     *  interferance (during which there can be no QOS assurances in
     *  the network).
     *
     *  If a caller sees this during a qos creation operation, the
     *  caller should attempt to delete the flow until certain that
     *  the flow has been deleted.  Then the creation may be
     *  reattempted.
     *
     *  If a caller sees this during an update operation, the caller
     *  should repeat the operation.   */
    CLNKLIB_FLOWCODE_UNKNOWN /*HI_FLOWCODE_UNKNOWN*/ =                   0x10,

    /** An internal error occurred and the operation should be
     *  reattempted. */
    CLNKLIB_FLOWCODE_INTERNAL_ERROR /*HI_FLOWCODE_INTERNAL_ERROR*/ =     0x14,

    /** Flow creation was unsuccessful due to the network already
     *  hosting a flow with the specified flow_id. */
    CLNKLIB_FLOWCODE_FLOW_EXISTS /*HI_FLOWCODE_FLOW_EXISTS*/ =           0x1C,

    /** Updating a flow failed since the flow could not be found. */
    CLNKLIB_FLOWCODE_NO_SUCH_FLOW /*HI_FLOWCODE_NO_SUCH_FLOW*/ =         0x20,

    /** Flow creation was unsuccessful due to a node involved in the
     *  flow not having enough resources to allocate one more flow. */
    CLNKLIB_FLOWCODE_TOO_MANY_FLOWS /*HI_FLOWCODE_TOO_MANY_FLOWS*/ =     0x24,

    /** Flow creation was unsuccessful due to ingress node not
     *  having enough resources to allocate one more multicast
     *  flow. */
    CLNKLIB_FLOWCODE_TOO_MANY_MCAST_F /*HI_FLOWCODE_TOO_MANY_MCAST_F*/ = 0x25,

    /** Flow creation unsuccessful due to violation of
     *  restriction of flow classification rules at ingress. */
    CLNKLIB_FLOWCODE_BAD_INGRESS_CFY /*HI_FLOWCODE_BAD_INGRESS_CFY*/ =   0x26,

    /** The specified flow destination address is not usable.
     *  Either the caller did not precisely match a multicast form
     *  PACKET_DA with the flow id or the caller passed a unicast
     *  PACKET_DA that was not tied to the node with the egress
     *  guid. */
    CLNKLIB_FLOWCODE_UNUSABLE_DA /*HI_FLOWCODE_UNUSABLE_DA*/ =           0x28,

    /** A node in the network rejected the flow creation due to
     *  MoCA specification dicated issues about the particular DA
     *  used: see the MoCA decision code definition. */
    CLNKLIB_FLOWCODE_INVALID_DA /*HI_FLOWCODE_INVALID_DA*/ =             0x29,

    /** The proposed ingress node cannot support the flow due to
     *  being in M1 sleep. */
    CLNKLIB_FLOWCODE_INGRESS_M1 /*HI_FLOWCODE_INGRESS_M1*/ =             0x2a,

    /** The ingress guid specified in the tspec is not currently on
     *  the network. */
    CLNKLIB_FLOWCODE_BAD_INGR /*HI_FLOWCODE_BAD_INGR*/ =                 0x2c,

    /** An egress guid specified in the tspec is not currently on
     *  the network. */
    CLNKLIB_FLOWCODE_BAD_EGR /*HI_FLOWCODE_BAD_EGR*/ =                   0x30,

    /** The operation was not successful due to a bandwidth
     *  constraint at the ingress node for the flow.  See the other
     *  field in the response structure to determine for what
     *  bandwidth the operation would have succeeded. */
    CLNKLIB_FLOWCODE_INGR_BW_UNAVAIL /*HI_FLOWCODE_INGR_BW_UNAVAIL*/ =   0x34,

    /** The operation was not successful due to a bandwidth
     *  constraint at an egress node for the flow.  See the other
     *  field in the response structure to determine for what
     *  bandwidth the operation would have succeeded. */
    CLNKLIB_FLOWCODE_EGR_BW_UNAVAIL /*HI_FLOWCODE_EGR_BW_UNAVAIL*/ =     0x38,

    /** The operation was not successful due to a network bandwidth
     *  constraint (not enough PQoS slot times per second available
     *  for proposed flow).  See the other field in the response
     *  structure to determine for what bandwidth the operation
     *  would have succeeded. */
    CLNKLIB_FLOWCODE_NW_STPS_UNAVAIL /*HI_FLOWCODE_NW_STPS_UNAVAIL*/ =   0x3c,

    /** The operation was not successful due to a network bandwidth
     *  constraint (not enough PQoS transmissions per second
     *  available for proposed flow).  See the other field in the
     *  response structure to determine for what bandwidth the
     *  operation would have succeeded. */
    CLNKLIB_FLOWCODE_NW_TXPS_UNAVAIL /*HI_FLOWCODE_NW_TXPS_UNAVAIL*/ =   0x40,

    /** Requested maximum retransmission number cannot be
     *  supported. */
    CLNKLIB_FLOWCODE_BAD_RETRANS /*HI_FLOWCODE_BAD_RETRANS*/ =           0x42,

    /** The operation was not successful since the lease expiration
     *  has become imminent; the flow will soon disappear on its own
     *  accord. */
    CLNKLIB_FLOWCODE_LEASE_EXPR_SOON /*HI_FLOWCODE_LEASE_EXPR_SOON*/ =   0x44,

    /** The operation was not successful due to an invalid peak
     *  bandwidth. */
    CLNKLIB_FLOWCODE_BAD_PEAK_BW /*HI_FLOWCODE_BAD_PEAK_BW*/ =           0x50,

    /** The operation failed due to an invalid burst size. */
    CLNKLIB_FLOWCODE_BAD_BURST_SIZE /*HI_FLOWCODE_BAD_BURST_SIZE*/ =     0x54,

    /** The operation failed due to a node in the network
     *  rejecting the tspec during the L2ME operation.  This is
     *  named as 'MoCA 2.0' but can be seen from a 'MoCA 1.1'
     *  node as well. */
    CLNKLIB_FLOWCODE_BAD_M20_TSPEC /*HI_FLOWCODE_BAD_M20_TSPEC*/ =       0x58,

    /** The operation failed since flow id is not multicast */
    CLNKLIB_FLOWCODE_INVALID_FLOW_ID /*HI_FLOWCODE_INVALID_FLOW_ID*/ =   0x5c,

    /** The operation was not successful due to a driver error; this
     *  is typically an Maxlinear internal error */
    CLNKLIB_FLOWCODE_DRIVER_ERROR /*HI_FLOWCODE_DRIVER_ERROR*/ =         0x60,

    /** The operation failed during an attempt to create a MoCA
     *  1.1 type flow with illegal TSPEC values.  The operation
     *  was not attempted since it would expose a MoCA 1.1 node
     *  to fields it is unprepared to interpret. */
    CLNKLIB_FLOWCODE_BAD_M11_TSPEC /*HI_FLOWCODE_BAD_M11_TSPEC*/ =       0x64,

    /** The operation failed due to an illegal DSCP_MOCA value. */
    CLNKLIB_FLOWCODE_INVALID_DSCP /*HI_FLOWCODE_INVALID_DSCP*/ =         0x68,

    /** The operation failed due to an illegal VLAN_TAG value. */
    CLNKLIB_FLOWCODE_INVALID_VLAN /*HI_FLOWCODE_INVALID_VLAN*/  =        0x6c,

    /** The operation failed due to an illegal
     *  INGRESS_CLASSIFICATION_RULE value. */
    CLNKLIB_FLOWCODE_INVALID_ICR /*HI_FLOWCODE_INVALID_ICR*/ =           0x70,

    /** The operation failed due to an unusable link. */
    CLNKLIB_FLOWCODE_UNUSABLE_LINK /*HI_FLOWCODE_UNUSABLE_LINK*/ =       0x74,
}                ClnkLib_FlowCode_e;
typedef uint32_t ClnkLib_FlowCode_t
/* Hi_FlowCode_t */;

/**
 * MAC Address, used for Ethernet, PQoS FlowID, and MoCA
 * GUID. NOT used for DFID. By convention, the most
 * significant MAC Address byte is the most significant byte
 * of field 'hi' and the least significant two MAC Address
 * bytes are found in the upper two bytes of field 'lo'.
 */
typedef struct
{
    uint32_t   hi;
    uint32_t   lo;
}  ClnkLib_MacAddr_t;


/**
 * PQoS Flow Parameter (PFP) information, fields used in
 * creation, modification (i.e. update), and query of MoCA PQoS
 * 1.1 and 2.0 Flows; fields match those in the Submit L2ME
 * frame for Create and Update as well as those in Response to
 * Wave 0 of the PQoS Query.  Some fields are only meaningful
 * when Ingress, Egress, and NC are MoCA 2.0 (M2).
 *
 * NOTE: at the time of this writing, the working standard does
 * not provide for PQOS query transaction to access
 * INORDER_DELIVERY_RECOMMENDED and TRAFFIC_PROTOCOL.  It is
 * possible this will be adjusted in a future rev so we'll keep
 * the same structure for PFP at this time.
 */
typedef struct
{
    ClnkLib_MacAddr_t  flowId;             //!< PQoS flow ID
    uint32_t           tPktSize;           //!< Traffic packet size
    ClnkLib_MacAddr_t  ingressGuid;        //!< Ingress GUID
    ClnkLib_MacAddr_t  egressGuid;         //!< Egress GUID
    uint32_t           flowTag;            //!< Flow tag
    ClnkLib_MacAddr_t  packetDa;           //!< Packet destination address
    uint32_t           tPeakDataRateKbps;  //!< Traffic peak data rate
    uint32_t           tLeaseTimeSec;      //!< Traffic lease time
    uint32_t           tBurstSize;         //!< Traffic burst size

    uint32_t           maxLatencyMs;       //!< M2: Maximum latency
    uint32_t           stAvgRatio;         //!< M2: Short term average ratio
    uint32_t           maxNumRetry;        //!< M2: Maximum number of retx attempts
    uint32_t           flowPer;            //!< M2: Flow PER (NPER or VLPER)
    uint32_t           inOrderDeliver;     //!< M2, no query: In order delivery recommendation
    uint32_t           tProtocol;          //!< M2, no query: Traffic protocol
    uint32_t           ingressClass;       //!< M2: Ingress classification rule
    uint32_t           vlanTag;            //!< M2: VLAN tag
    uint32_t           dscpMoca;           //!< M2: DSCP MoCA value
}  ClnkLib_PFP_t
/*Hi_PFP_t*/;


/** Results of attempted PQoS Create/Update.  Fields marked with
 *  W1REQ are copied from specific fields or bits in the PQoS
 *  Create/Update Wave 1 Request.
 */
typedef struct
{
    ClnkLib_FlowCode_t flowCode;           //!< Response code, encodes DECISION
    uint32_t           postStps;           //!< W1REQ: POST_STPS
    uint32_t           postTxps;           //!< W1REQ: POST_TXPS
    uint32_t           dfid;               //!< W1REQ: DFID
    uint32_t           maxRetryNum;        //!< W1REQ: Maximum allowed retry number
    uint32_t           maxShortTermAvRatio;//!< W1REQ: Maximum allowed short term avg ratio
    uint32_t           remainKbps;         //!< W1REQ: MAX_PEAK_DATA_RATE
    uint32_t           remainBurst;        //!< W1REQ: MAX_BURST_SIZE
    uint32_t           decision;           //!< W1REQ: DECISION
    uint32_t           bwLimitInfo;        //!< W1REQ: BW_LIMIT_INFO
    uint32_t           isIngressLimited;   //!< W1REQ: BW_LIMIT_INFO(3)
    uint32_t           isEgressLimited;    //!< W1REQ: BW_LIMIT_INFO(3)
    uint32_t           isAggrStpsLimited;  //!< W1REQ: BW_LIMIT_INFO(1)
    uint32_t           isAggrTxpsLimited;  //!< W1REQ: BW_LIMIT_INFO(0)
    uint32_t           currMaxStps;        //!< Estimate of current max MOCA STPS
    uint32_t           currMaxTxps;        //!< Estimate of current max MOCA TXPS
} ClnkLib_FlowCURsp_t
/*Hi_FlowCURsp_t*/;


//! Flow delete response
typedef struct
{
    ClnkLib_FlowCode_t flowCode;        //!< Response
    uint32_t           numNodesDelRsrc; //!< Number of nodes that deleted something
} ClnkLib_FlowDeleteRsp_t
/*Hi_FlowDeleteRsp_t*/;


//! Flow list request.  See MoCA 1.1 Specification Table 4-14 for precision.
typedef struct
{
    ClnkLib_MacAddr_t ingressGuid;  //!< Queried node
    uint32_t flowStartIndex;        //!< First flow index to return in logical table
    uint32_t flowMaxReturn;         //!< Maximum number of flows to return
} ClnkLib_FlowListReq_t
/*Hi_FlowListReq_t*/;


//! Flow list response.  See the MoCA 1.1 Specification Table 4-15 for precision.
typedef struct
{
    ClnkLib_FlowCode_t flowCode; //!< Response
    uint32_t flowUpdateCount;    //!< Event for each flow change at node
    uint32_t totalFlowIdCount;   //!< Flows for which node is Ingress
    uint32_t numReturnedFlowIds; //!< Num flows populated in table below
    ClnkLib_MacAddr_t returnedFlowId[MAX_FLOWS_IN_LIST];
} ClnkLib_FlowListRsp_t
/*Hi_FlowListRsp_t*/;


/** Flow query information
 *
 * NOTE: at the time of this writing, the working standard does
 * not provide for PQOS query transaction to access
 * INORDER_DELIVERY_RECOMMENDED and TRAFFIC_PROTOCOL.  It is
 * possible this will be adjusted in a future rev so we'll keep
 * the same structure for PFP at this time.
 *   */
typedef struct
{
    ClnkLib_FlowCode_t flowCode;      //!< Response code, encodes W1REQ DECISION
    uint32_t           leaseTimeLeft; //!< Lease time left (seconds)
    ClnkLib_PFP_t      pfp;           //!< Parameters for PQoS Flow
    uint32_t           dfid;          //!< DFID, 8 bit routing number
} ClnkLib_FlowQueryRsp_t
/*Hi_FlowQueryRsp_t*/;

//! Flow statistics
typedef struct
{
    /** Number of times that this node has seen other nodes admit and drop */
    uint32_t countTopologyChanged;
    /** Number of times that this node has been the ingress for a creation,
     *   update, or deletion of a Flow. */
    uint32_t countIngressFlowChanged;
    /** Number of times a PQoS maintenance cycle has reported that Flows
     *   in the network are at risk due to changing topology or bandwidth
     *   availability. */
    uint32_t countBwExceeded;
    /** Number of times this node has detected that a change in the network
     *   version has occurred.  This directly corresponds to events that cause
     *   network-wide PQoS guarantee changes.  */
    uint32_t countNwVersionChange;
    /** Node's estimate of maximum STPS possible in the network, may differ from
     *   the NC's version. */
    uint32_t currentMaxStps;
    /** Node's estimate of maximum TXPS possible in the network, may differ from
     *   the NC's version. */
    uint32_t currentMaxTxps;
    /** Flow Maintenance: several fields update every PQoS maintenance cycle- this
     *    field increments every maintenance. */
    uint32_t fmUpdateCount;
    /** Flow Maintenance: cache of the IOC_NODEMASK value during the last
     *    maintenance.   Represents which nodeIds are over ingress commitment */
    uint32_t fmIocNodemask;
    /** Flow Maintenance: cache of the ALLOCATED_STPS value during the last
     *    maintenance.   Represents networkwide allocation of slot times. */
    uint32_t fmAllocatedStps;
    /** Flow Maintenance: cache of the ALLOCATED_TXPS value during the last
     *    maintenance.   Represents networkwide allocation of transmissions. */
    uint32_t fmAllocatedTxps;
} ClnkLib_FlowStats_t
/*Hi_FlowStats_t*/;

//! Flow statistics
typedef struct
{
    ClnkLib_FlowCode_t flowCode; //!< Response
    uint32_t allocatedStps;      //!< if flowCode _SUCCESS, current alloc of STPS
    uint32_t allocatedTxps;      //!< if flowCode _SUCCESS, current alloc of TXPS
} ClnkLib_FlowCapacityRsp_t;



//! DES key
typedef uint32_t MocaDesKey_t[2];

//! AES key
typedef uint32_t MocaAesKey_t[4];

//! Security information
typedef struct
{
    uint32_t     state;         //!< Security state (enabled or disabled)
    MocaDesKey_t mmk;           //!< DES MAC management key
    MocaDesKey_t pmkInit;       //!< DES initial privacy management key
    MocaDesKey_t pmk[2];        //!< DES privacy management keys
    uint32_t     activePmk;     //!< Active DES PMK (even or odd)
    MocaDesKey_t tek[2];        //!< DES traffic encryption keys
    uint32_t     activeTek;     //!< Active DES TEK (even or odd)
    MocaAesKey_t ammk;          //!< AES MAC management key
    MocaAesKey_t apmkInit;      //!< AES initial privacy management key
    MocaAesKey_t apmk[2];       //!< AES privacy management keys
    uint32_t     activeApmk;    //!< Active AES PMK (even or odd)
    MocaAesKey_t atek[2];       //!< AES traffic encryption keys
    uint32_t     activeAtek;    //!< Active AES TEK (even or odd)
    uint32_t     permSalt[3];   //!< Permanent Salt
}
ClnkLib_SecInfo_t;

//! Security statistics
typedef struct
{
    uint32_t txNoSec;    //!< Frames txed with no security
    uint32_t txPmkInit;  //!< Frames txed with PMKInit
    uint32_t txMmk;      //!< Frames txed with MMK
    uint32_t txPmk;      //!< Frames txed with PMK
    uint32_t txTek;      //!< Frames txed with TEK
    uint32_t txApmkInit; //!< Frames txed with APMKInit
    uint32_t txAmmk;     //!< Frames txed with AMMK
    uint32_t txApmk;     //!< Frames txed with APMK
    uint32_t txAtek;     //!< Frames txed with ATEK
}
ClnkLib_SecStats_t;

#if FEATURE_SAPM
typedef struct
{
    uint32_t        threshold;
    uint32_t        sapmTbl[FFT_LENGTH / 4];
} ClnkLib_SapmSocProfile_t;

typedef struct
{
    int32_t         sapmProfileId;  // only used to pass pid through API calls
    uint32_t        threshold;
    uint32_t        sapmTbl[FFT_LENGTH / 4];
} ClnkLib_SapmProfile_t;

typedef struct
{
    int32_t         sapmProfileId;
    int32_t         profile;
    uint32_t        sapmThreshold[MAX_SAPM_PROFILES];
    uint32_t        sapmTbl[MAX_SAPM_PROFILES][FFT_LENGTH / 4];
} ClnkLib_SapmCfg_t;
#endif

#if FEATURE_RLAPM
typedef struct
{
    int32_t         rlapmProfileId;
    int32_t         profile;
    uint32_t        rlapmTbl[MAX_RLAPM_PROFILES][MAX_RX_LEVELS_PADDED / 4];
} ClnkLib_RlapmCfg_t;

//! SAPM table
typedef MXL_MOCA_SAPM_TABLE_V2_T ClnkLib_SapmTableV2_t;

//! SAPM configuration
typedef MXL_MOCA_SAPM_CFG_V2_T ClnkLib_SapmCfgV2_t;

typedef struct
{
    uint32_t    sapmmode;
    int32_t     sapmProfileId;
    // we refer the ClnkLib_SapmCfgV2_t type to reuse the underlying APIs,
    // but note the rlapmmode field in profiles is meaningless
    ClnkLib_SapmCfgV2_t profiles[MAX_SAPM_PROFILES];
} ClnkLib_SapmStaticCfgV2_t;

//! RLAPM table
typedef MXL_MOCA_RLAPM_TABLE_V2_T ClnkLib_RlapmTableV2_t;

//! RLAPM configuration
typedef MXL_MOCA_RLAPM_CFG_V2_T ClnkLib_RlapmCfgV2_t;

typedef struct
{
    //! RLAPM mode
    uint32_t rlapmmode;

    //! RLAPM tables
    ClnkLib_RlapmTableV2_t tbl;
} ClnkLib_RlapmActiveCfgV2_t;

typedef struct
{
    uint32_t    rlapmmode;
    int32_t     rlapmProfileId;
    // we refer the ClnkLib_RlapmCfgV2_t type to reuse the underlying APIs,
    // but note the rlapmmode field in profiles is meaningless
    ClnkLib_RlapmCfgV2_t profiles[MAX_RLAPM_PROFILES];
} ClnkLib_RlapmStaticCfgV2_t;

#endif

typedef enum
{
  CLNKLIB_ICR_VLAN_TAG_4_5                  = 0,
  CLNKLIB_ICR_IGNORE_VLAN_TAG_AND_DSCP_MOCA = 4,
  CLNKLIB_ICR_USE_DSCP_MOCA                 = 5,
  CLNKLIB_ICR_USE_VLAN                      = 6,
  CLNKLIB_ICR_USE_VLAN_OR_DSCP_MOCA         = 7,
} ClnkLib_Icr_t;

typedef struct
{
    uint32_t rssiWordTbl[MAX_RSSI_LEN / 4];
} ClnkLib_RssiCfg_t;

typedef MXL_MOCA_RSSI_CFG_V2_T ClnkLib_RssiCfgV2_t;

typedef MXL_MOCA_PLATFORM_CFG_T ClnkLib_PlatformCfg_t;

// Summing over 25 subcarriers for Ui and Li, each one separate by 31. i.e. U2 = U1+31
// ATSC and CATV table
// E2, EE3, F3, F7
typedef MXL_MOCA_BAND_E_TABLE_T BandEFTbl_t;

// CATV and ATSC
typedef MXL_MOCA_ATSC_CATV_WEIGHT_T AtscCatvWeight_t;

// D2, D4, D6, D8, D10, 1100
typedef MXL_MOCA_BAND_D_TABLE_T BandDTbl_t;

// BTS constants
typedef MXL_MOCA_BTS_T Bts_t;

// CSS constants
typedef MXL_MOCA_CSS_T Css_t;

// Energy and usability related thresholds
typedef MXL_MOCA_BAND_DEF_THRESHOLDS_T BandDEFThresholds_t;

// ClnkLib energy detection configuration parameters
typedef MXL_MOCA_ENERGY_DETECT_CFG_T ClnkLib_EndetCfg_t;

typedef struct
{
    uint32_t rawDetectedServiceBitmask;
    uint8_t channelInfo50MhzBandE[NUM_BAND_E_50MHZ_RANK];  // NUM_BAND_E_50MHZ_RANK
    uint8_t channelInfo50MhzBandF[NUM_BAND_F_50MHZ_RANK];  // NUM_BAND_F_50MHZ_RANK
    uint8_t channelInfo50MhzBandD[NUM_BAND_D_50MHZ_RANK];  // NUM_BAND_D_50MHZ_RANK
    uint8_t channelInfo100MhzBandE[NUM_BAND_E_100MHZ_RANK];  // NUM_BAND_E_100MHZ_RANK
    uint8_t channelInfo100MhzBandF[NUM_BAND_F_100MHZ_RANK];  // NUM_BAND_F_100MHZ_RANK
    uint8_t channelInfo100MhzBandD[NUM_BAND_D_100MHZ_RANK];  // NUM_BAND_D_100MHZ_RANK
    uint8_t pad[2];

    uint32_t bandEF50MhzEnergyBitmask;
    uint32_t bandD50MhzEnergyBitmask;

    uint32_t bandEF100MhzEnergyBitmask;
    uint32_t bandD100MhzEnergyBitmask;
}ClnkLib_EndetStatus_t;

//! Active SAPM
typedef struct
{
    //! SAPM mode
    uint32_t sapmmode;

    //! Added PHY margin table
    uint8_t phymargin[FFT_LENGTH];

    //! Threshold and frequency
    uint16_t frequency;
    uint8_t threshold;
    uint8_t pad;
} ClnkLib_ActiveSapm_t;

//Tx beacon power for local or distribute, range -10~7db
typedef MXL_MOCA_SOC_SET_BCN_POWER_RSP_T ClnkLib_SetBcnPowerRsp_t;

//Tx beacon power for local or distribute, range -10~7db
typedef MXL_MOCA_SOC_SET_BCN_POWER_CMD_T ClnkLib_SetBcnPowerCmd_t;

//Tx beacon power for local or distribute, range -10~7db
typedef MXL_MOCA_SOC_GET_BCN_POWER_RSP_T ClnkLib_GetBcnPowerRsp_t;

/*******************************************************************************
 *      Public/Private Data
 ******************************************************************************/

/*!
 * \brief c.LINK Interface Handle Structure
 *
 * Handle (or context) for the c.LINK interface.  This structure is used to
 * store the context for c.LINK interface.
 */
typedef struct
{
    char    ifname[IFNAME_MAX + 1];
    char    conf_path[256];
    char    fwdir[256];
    char    drvname[256];
    int32_t drv_fd;
    int32_t phy_id;
    int32_t pcie;
    int32_t testParam4;
    int32_t stopped;
    int32_t force_reset;
    int32_t retCodes;
    int32_t testMode;
    int32_t socChipType;
} ClnkLib_Handle_t;


/*!
 * \brief c.LINK Interface Name Structure
 *
 * A structure to store clink interface names. (for acch add/delete)
 */
typedef struct ClinkIfName_s
{
    int8_t ifName[IFNAMSIZ];
} ClnkLib_Name_t;


#include "HostCfgLib.h"

/*******************************************************************************
 *      Public/Private Functions
 ******************************************************************************/
///c.LINK library apis
//
//Interface management
int32_t ClnkLib_Open(ClnkLib_Handle_t *pHandle);
int32_t ClnkLib_OpenByName(ClnkLib_Handle_t *pHandle, char *ifNameString);
int32_t ClnkLib_ListNames(ClnkLib_Name_t *ifNamePtr, int32_t max);
int32_t ClnkLib_GetName(ClnkLib_Handle_t *pHandle, ClnkLib_Name_t *pName);
int32_t ClnkLib_DisplayInterfaces();
void ClnkLib_Close(ClnkLib_Handle_t *pHandle);
int32_t ClnkLib_Start(ClnkLib_Handle_t *pHandle);
int32_t ClnkLib_Stop(ClnkLib_Handle_t *pHandle);
int32_t ClnkLib_Reset(ClnkLib_Handle_t *pHandle);

//Device Configuration
int32_t ClnkLib_GetStaticCfg(ClnkLib_Handle_t *pHandle, int flags, ClnkLib_StaticCfg_t *setup);
int32_t ClnkLib_SetStaticCfg(ClnkLib_Handle_t *pHandle, ClnkLib_StaticCfg_t *setup);
int32_t ClnkLib_GetClnkCfgV2(ClnkLib_Handle_t *pHandle, int flag, ClnkLib_ClnkCfgV2_t *setup);
int32_t ClnkLib_SetClnkCfgV2(ClnkLib_Handle_t *pHandle, ClnkLib_ClnkCfgV2_t *setup);
int32_t ClnkLib_GetClnkCfgV2Params(ClnkLib_Handle_t *pHandle, clnk_param_mask_t *mask, ClnkLib_ClnkCfgV2_t *setup);
int32_t ClnkLib_SetClnkCfgV2Params(ClnkLib_Handle_t *pHandle, clnk_param_mask_t *mask, ClnkLib_ClnkCfgV2_t *setup);
int32_t ClnkLib_RestoreClnkCfgV2(ClnkLib_Handle_t *pHandle);
#if FEATURE_SAPM
// The following functions can be used to manage the Sapm profile config
int ClnkLib_SetSapmCfg(ClnkLib_Handle_t *pHandle, const ClnkLib_SapmStaticCfgV2_t *pCfg);
int ClnkLib_GetSapmCfg(ClnkLib_Handle_t *pHandle, ClnkLib_SapmStaticCfgV2_t *pCfg);
int ClnkLib_ReplaceSapmCfg(ClnkLib_Handle_t *pHandle, const char *newFile);
int ClnkLib_GetActiveSapm(ClnkLib_Handle_t *pHandle, ClnkLib_ActiveSapm_t *pActiveSapm);
int ClnkLib_GetActiveSapmByChIndex(ClnkLib_Handle_t *pHandle, uint32_t index, ClnkLib_ActiveSapm_t *pActiveSapm);
int ClnkLib_SetSapmProfileId(ClnkLib_Handle_t *pHandle, int32_t profileId);
int ClnkLib_SetSapmMode(ClnkLib_Handle_t *pHandle, uint32_t band, uint32_t mode);
int ClnkLib_SetSapmSimpleCfg(ClnkLib_Handle_t *pHandle, int32_t freq, int32_t threshold, int32_t scStart, int32_t carrier, int32_t margin);
#endif
#if FEATURE_RLAPM
int ClnkLib_SetRlapmCfg(ClnkLib_Handle_t *pHandle, const ClnkLib_RlapmStaticCfgV2_t *pRlapmStaticCfg);
int ClnkLib_GetRlapmCfg(ClnkLib_Handle_t *pHandle, ClnkLib_RlapmStaticCfgV2_t *pRlapmStaticCfg);
int ClnkLib_ReplaceRlapmCfg(ClnkLib_Handle_t *pHandle, const char *newFile);
int ClnkLib_GetActiveRlapm(ClnkLib_Handle_t *pHandle, ClnkLib_RlapmActiveCfgV2_t *pRlapmCfg);
int ClnkLib_GetActiveRlapmByChIndex(ClnkLib_Handle_t *pHandle, uint32_t index, ClnkLib_RlapmActiveCfgV2_t *pRlapmCfg);
int ClnkLib_SetRlapmProfileId(ClnkLib_Handle_t *pHandle, int32_t profileId);
int ClnkLib_SetRlapmMode(ClnkLib_Handle_t *pHandle, uint32_t band, uint32_t mode);
int ClnkLib_SetRlapmSimpleCfg(ClnkLib_Handle_t *pHandle, int32_t freq, int32_t rxLevel, int32_t margin);
#endif
int ClnkLib_GetBoardInfo(ClnkLib_Handle_t *pHandle, ClnkLib_BoardInfo_t *binfo);
int ClnkLib_GetChipName(ClnkLib_Handle_t *pHandle, char **chipname);
int ClnkLib_GetChipType(ClnkLib_Handle_t *pHandle, int32_t *pSocChipType);

//Device Management:switch
int32_t ClnkLib_GetSwitchTable(ClnkLib_Handle_t *pHandle, uint32_t type, uint32_t startIndex, uint32_t *pMaxEntries, ClnkLib_BridgeTable_t *brg_table);
int32_t ClnkLib_GetSwitchStats(ClnkLib_Handle_t *pHandle, int32_t clear, ClnkLib_SwitchStats_t *pSwitchStats);
int32_t ClnkLib_GetSwitchConfig(ClnkLib_Handle_t *pHandle, ClnkLib_SwitchConfig_t *pSwitchConfig);
int32_t ClnkLib_SetSwitchPortConfig(ClnkLib_Handle_t *pHandle, ClnkLib_SwitchPortConfig_t *pSwitchPortConfig, uint32_t* pRsp);
int32_t ClnkLib_ResetSwitchConfig(ClnkLib_Handle_t *pHandle);
int32_t ClnkLib_SetSwitchTmo(ClnkLib_Handle_t *pHandle, uint32_t tmo);
int32_t ClnkLib_EnableSwitchVlanCheck(ClnkLib_Handle_t *pHandle, int32_t port);
int32_t ClnkLib_DisableSwitchVlanCheck(ClnkLib_Handle_t *pHandle, int32_t port);
int32_t ClnkLib_EnableSwitchVlanStrip(ClnkLib_Handle_t *pHandle, int32_t port);
int32_t ClnkLib_DisableSwitchVlanStrip(ClnkLib_Handle_t *pHandle, int32_t port);
int32_t ClnkLib_SetSwitchVlanPvid(ClnkLib_Handle_t *pHandle, int32_t port, uint32_t pvid);
int32_t ClnkLib_AddSwitchVlanId(ClnkLib_Handle_t *pHandle, int32_t port, uint32_t vid);
int32_t ClnkLib_DelSwitchVlanId(ClnkLib_Handle_t *pHandle, int32_t port, uint32_t vid);
int32_t ClnkLib_SetMgmtMac(ClnkLib_Handle_t *pHandle, uint32_t macHi, uint32_t macLo);
int32_t ClnkLib_SetSwitchMgmtMac(ClnkLib_Handle_t *pHandle, uint32_t macHi, uint32_t macLo);

//Device Management:ethernet
int32_t ClnkLib_GetAggrStats(ClnkLib_Handle_t *pHandle, int clear, ClnkLib_AggrStats_t *);
int32_t ClnkLib_GetBridgeStats(ClnkLib_Handle_t *pHandle, int32_t clear, ClnkLib_BridgeStats_t *pBridgeStats);
int32_t ClnkLib_GetBridgeTable(ClnkLib_Handle_t *pHandle, uint32_t type, uint32_t startIndex, uint32_t *pMaxEntries, ClnkLib_BridgeTable_t *brg_table);
int32_t ClnkLib_GetEphyStats(ClnkLib_Handle_t *, int32_t clear, ClnkLib_EphyStats_t *pEphyStats);
int32_t ClnkLib_GetEcbStats(ClnkLib_Handle_t *, int32_t clear, ClnkLib_EcbStats_t *pEphyStats);
int ClnkLib_SetMcastFilter(ClnkLib_Handle_t *pHandle, uint32_t macAddrHi, uint32_t macAddrLo, uint32_t flag);
int ClnkLib_GetMcastFilterMode(ClnkLib_Handle_t *pHandle, uint32_t *pMode);
int ClnkLib_SetMcastFilterMode(ClnkLib_Handle_t *pHandle, uint32_t mode);
int ClnkLib_GetMcastCfg(ClnkLib_Handle_t *pHandle, uint32_t flag, ClnkLib_McastCfg_t *pCfg, int *numEntries);
int ClnkLib_SetMcastCfg(ClnkLib_Handle_t *pHandle, ClnkLib_McastCfg_t *pCfg);
int ClnkLib_RestoreMcastCfg(ClnkLib_Handle_t *pHandle);
int32_t ClnkLib_GetMacDataStats(ClnkLib_Handle_t *pHandle, int clear, ClnkLib_MacDataStats_t *pMacData);
int32_t ClnkLib_GetRetxStats(ClnkLib_Handle_t *pHandle, uint32_t clear, ClnkLib_RetxStats_t *pStats);

//Device Management:link
int32_t ClnkLib_GetCdInfo(ClnkLib_Handle_t *pHandle, uint32_t clear, ClnkLib_CdInfo_t *pInfo);
int32_t ClnkLib_GetFrameStats(ClnkLib_Handle_t *pHandle, int32_t clear, ClnkLib_FrameStats_t *pFrameStats);
int32_t ClnkLib_GetDataStats(ClnkLib_Handle_t *pHandle, uint32_t clear, ClnkLib_DataStats_t *pStats);
int32_t ClnkLib_GetFragStats(ClnkLib_Handle_t *pHandle, uint32_t clear, ClnkLib_FragStats_t *pStats);
int32_t ClnkLib_GetL2meStats(ClnkLib_Handle_t *pHandle, uint32_t clear, ClnkLib_L2meStats_t *pL2meStats);
int32_t ClnkLib_GetSocStatus(ClnkLib_Handle_t *pHandle, ClnkLib_SocStatus_t *pSocStatus);
int32_t ClnkLib_GetLocalInfo(ClnkLib_Handle_t *pHandle, ClnkLib_LocalInfo_t *pLocalInfo);
int32_t ClnkLib_GetFmrInfo(ClnkLib_Handle_t *pHandle, uint32_t fmrMask, uint32_t version, ClnkLib_FmrInfo_t *pFmrInfo);
int32_t ClnkLib_GetDdInfo(ClnkLib_Handle_t *pHandle, uint32_t ddMask, ClnkLib_DdInfo_t *pDdInfo);
int32_t ClnkLib_GetNetInfo(ClnkLib_Handle_t *pHandle, uint32_t nodeId, ClnkLib_NetInfo_t *pNetInfo);
int ClnkLib_RequestHandoff(ClnkLib_Handle_t *pHandle, ClnkLib_HandoffRequest_t *pReq);  ///Debug
int ClnkLib_RequestLmo(ClnkLib_Handle_t *pHandle, ClnkLib_LmoRequest_t *pReq, ClnkLib_LmoReport_t *pRpt);

//Device Management:NMS
int ClnkLib_IssueMocaReset(ClnkLib_Handle_t *pHandle, ClnkLib_MocaResetRequest_t *pRequest, ClnkLib_MocaResetResponse_t *pResponse);
int ClnkLib_RequestNms(ClnkLib_Handle_t *pHandle, ClnkLib_NmsRequest_t *pReq, ClnkLib_NmsReport_t *pRpt);
int32_t ClnkLib_SendNetProxyPacket(ClnkLib_Handle_t *pHandle, uint8_t* pPkt, uint32_t length, uint32_t* pRsp);
int32_t ClnkLib_WaitForNetProxyResponse(ClnkLib_Handle_t *pHandle, uint8_t* pPkt, uint32_t* pLength, uint32_t* pRsp);
int32_t ClnkLib_WaitForNetProxyRcvReq(ClnkLib_Handle_t *pHandle, uint8_t* pPkt, uint32_t* pLength, uint32_t* pRsp);
int32_t ClnkLib_SendNetProxyResponse(ClnkLib_Handle_t *pHandle, uint8_t* pPkt, uint32_t length, uint32_t* pRsp);
int32_t ClnkLib_SendNetProxyTestModeCtrl(ClnkLib_Handle_t *pHandle, uint32_t enable);

//Device management:PHY
int32_t ClnkLib_GetPhyTxProfile(ClnkLib_Handle_t *pHandle, uint32_t nodeId, uint32_t profileId, uint32_t getBitloading, ClnkLib_PhyTxProfile_t *pRsp);
int32_t ClnkLib_GetPhyRxProfile(ClnkLib_Handle_t *pHandle, uint32_t nodeId, uint32_t profileId, uint32_t getBitloading, ClnkLib_PhyRxProfile_t *pRsp);
int32_t ClnkLib_GetPhyNode(ClnkLib_Handle_t *pHandle, uint32_t nodeId, ClnkLib_PhyNode_t *pRsp);
int32_t ClnkLib_GetPhyEvm(ClnkLib_Handle_t *pHandle, uint32_t nodeId, uint32_t profileId, ClnkLib_PhyEvm_t *pRsp);
int32_t ClnkLib_GetPhyMisc(ClnkLib_Handle_t *pHandle, ClnkLib_PhyMisc_t *pRsp);
int32_t ClnkLib_GetM25PhyMisc(ClnkLib_Handle_t *pHandle, ClnkLib_M25PhyMisc_t *pRsp);
int ClnkLib_GetOfdmaParams(ClnkLib_Handle_t *pHandle, ClnkLib_OfdmaParams_t *pOfdmaParams);
int ClnkLib_GetOfdmaTxProfile(ClnkLib_Handle_t *pHandle, ClnkLib_OfdmaTxProfile_t *pOfdmaTxProfile, uint32_t role, uint32_t asgnTblIdx);
int ClnkLib_GetOfdmaRxProfile(ClnkLib_Handle_t *pHandle, ClnkLib_OfdmaRxProfile_t *pOfdmaRxProfile, uint32_t profileNum, uint32_t asgnTblIdx);
int ClnkLib_SetPhyModBitmask(ClnkLib_Handle_t *pHandle, uint32_t nodeId, uint32_t type, uint32_t value);
int ClnkLib_GetEndetStatus(ClnkLib_Handle_t *pHandle, ClnkLib_EndetStatus_t *pEndetStatus);
int ClnkLib_SetGetTxPwrVar(ClnkLib_Handle_t *pHandle, uint32_t set, uint32_t nodeId, uint32_t *txPwrVar);
int ClnkLib_GetTxPwr(ClnkLib_Handle_t *pHandle, float *txPwr, uint32_t nodeId);

//Device Management:PM
int ClnkLib_Sleep(ClnkLib_Handle_t *pHandle, uint32_t state);
int ClnkLib_Wake(ClnkLib_Handle_t *pHandle);
int ClnkLib_GetPmCaps(ClnkLib_Handle_t *pHandle, uint32_t *pPmCapsMask);
int ClnkLib_QueryPmState(ClnkLib_Handle_t *pHandle, uint32_t nodeId, ClnkLib_QueryPmStateRsp_t *pRsp);
int ClnkLib_GetPmEvent(ClnkLib_Handle_t *pHandle, uint32_t *pEvent, ClnkLib_PmEventData_t *pEventData, uint32_t clearEvent);
int ClnkLib_HostIfReady(ClnkLib_Handle_t *pHandle);
int32_t ClnkLib_StandaloneLowPowerMode(ClnkLib_Handle_t *pHandle, int8_t powerDown);

//Device Management:QOS
int ClnkLib_CreateFlow(ClnkLib_Handle_t *pHandle, ClnkLib_PFP_t *pReq, ClnkLib_FlowCURsp_t *pRsp);
int ClnkLib_UpdateFlow(ClnkLib_Handle_t *pHandle, ClnkLib_PFP_t *pReq, ClnkLib_FlowCURsp_t *pRsp);
int ClnkLib_DeleteFlow(ClnkLib_Handle_t *pHandle, ClnkLib_MacAddr_t *pFlowId, ClnkLib_FlowDeleteRsp_t *pRsp);
int ClnkLib_ListFlows(ClnkLib_Handle_t *pHandle, ClnkLib_FlowListReq_t *pReq, ClnkLib_FlowListRsp_t *pRsp);
int ClnkLib_QueryFlow(ClnkLib_Handle_t *pHandle, ClnkLib_MacAddr_t *pFlowId, ClnkLib_FlowQueryRsp_t *pRsp);
int ClnkLib_GetFlowStats(ClnkLib_Handle_t *pHandle, uint32_t clear, ClnkLib_FlowStats_t *pStats);
int ClnkLib_QueryFlowCapacity(ClnkLib_Handle_t *pHandle, ClnkLib_FlowCapacityRsp_t *pCapacity);

//Device Management:Security
int ClnkLib_GetSecPassword(ClnkLib_Handle_t *pHandle, uint8_t *pass);
int ClnkLib_GetSecInfo(ClnkLib_Handle_t *pHandle, ClnkLib_SecInfo_t *pInfo);
int ClnkLib_GetSecStats(ClnkLib_Handle_t *pHandle, uint32_t clear, ClnkLib_SecStats_t *pStats);

//Device Management:Spectrum Analyzer
uint32_t ClnkLib_GetSpectrumSize(uint32_t startFreq, uint32_t endFreq);
int32_t ClnkLib_GetSpectrum(ClnkLib_Handle_t *pHandle, int startFreq, int endFreq,
                            bool maxHold, ClnkLib_MacAddr_t *txMacAddr, bool postProcess,
                            int8_t *buf, int *bufSize);

//Device Management:Bridge Control
int32_t ClnkLib_BridgeCtrlGetNwNamePayload(ClnkLib_Handle_t *pHandle, uint8_t* pRsp);
int32_t ClnkLib_BridgeCtrlGetNwNameRec(ClnkLib_Handle_t *pHandle, uint32_t reset, int32_t* pRsp);
int32_t ClnkLib_BridgeCtrlNwNamePayloadTx(ClnkLib_Handle_t *pHandle, uint8_t set, uint8_t* pSetStr, uint8_t* pGetStr);
int32_t ClnkLib_BridgeCtrlNwNamePayloadRx(ClnkLib_Handle_t *pHandle, uint8_t set, uint8_t* pSetStr, uint8_t* pGetStr);
int32_t ClnkLib_BridgeCtrlNetworkJoinCfg(ClnkLib_Handle_t *pHandle, uint32_t enable, uint32_t reset, int32_t* pRsp);
int32_t ClnkLib_BridgeCtrlNetworkJoin(ClnkLib_Handle_t *pHandle, int32_t* pRsp);

//Device Management:Debug
int32_t ClnkLib_ReadMem(ClnkLib_Handle_t *pHandle, uint32_t addr, uint8_t *pBuf, int len);
int32_t ClnkLib_WriteMem(ClnkLib_Handle_t *pHandle, uint32_t addr, uint8_t *pBuf, int len);
#if LIBAPI_OFFICIAL_DESIGN
#else
// Test
int32_t ClnkLib_DoMailboxTest(ClnkLib_Handle_t *);
#endif
int32_t ClnkLib_DumpBin(ClnkLib_Handle_t *pHandle, char *iface, const char *confdir, const char *fwdir, int flags);
#ifdef SET_CD_FW_SUPPORT
int32_t ClnkLib_SetCdFirmwares(ClnkLib_Handle_t *pctx, const char *ifname);
#endif
int32_t ClnkLib_StartDaemon(ClnkLib_Handle_t *pctx, MXL_MOCA_DRV_START_DAEMON_CMD_T *pStartOption);
int32_t ClnkLib_SetApplicationTrace(ClnkLib_Handle_t *pHandle, uint32_t flag);

// MoCA2.5 Features
// Beacon Power
int32_t ClnkLib_SetBeaconPower(ClnkLib_Handle_t *pHandle, MXL_BCN_PWR_TYPE_T type, int32_t bcnPower);
int32_t ClnkLib_GetBeaconPower(ClnkLib_Handle_t *pHandle, ClnkLib_GetBcnPowerRsp_t *pRsp);
int32_t ClnkLib_SendBeaconPowerPIE(ClnkLib_Handle_t *pHandle, int32_t bcnPower, uint32_t bcnType, uint32_t nodeMask);

// MPS
int32_t ClnkLib_SetMpsTrigger(ClnkLib_Handle_t *pHandle, MXL_MOCA_SOC_SET_MPS_TRIGGER_RSP_T *pMpsTrigger);
int32_t ClnkLib_GetMpsState(ClnkLib_Handle_t *pHandle, MXL_MOCA_SOC_GET_MPS_STATE_RSP_T *pMpsState);
int32_t ClnkLib_GetMpsReport(ClnkLib_Handle_t *pHandle, MXL_MOCA_SOC_GET_MPS_REPORT_RSP_T *pMpsReport);

// Enhanced Privacy
int32_t ClnkLib_GetEnhancedSecInfo(ClnkLib_Handle_t *pHandle, MXL_MOCA_SOC_GET_EP_SEC_INFO_RSP_T *pEpSecInfo);
int32_t ClnkLib_GetTrafficPermit(ClnkLib_Handle_t *pHandle, MXL_MOCA_SOC_GET_TRAFFIC_PERMIT_STS_RSP_T *pTrafficPermitRsp);
int32_t ClnkLib_SetTrafficPermit(ClnkLib_Handle_t *pHandle, MXL_MOCA_SOC_SET_TRAFFIC_PERMIT_CFG_CMD_T *pTrafficPermitCfg);
int32_t ClnkLib_GetEtherTypeInfo(ClnkLib_Handle_t *pHandle, MXL_MOCA_SOC_GET_ETHER_TYPE_INFO_RSP_T *pEtherTypeInfoRsp);

#ifdef __cplusplus
}
#endif
#endif /* __CLNK_API_H__ */

