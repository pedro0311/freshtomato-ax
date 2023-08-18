/*****************************************************************************************
 *
 * FILE NAME          : mxl_moca_soc_cmd.h
 *
 *
 *
 * DATE CREATED       : Sep/14/2016
 *
 * LAST MODIFIED      :
 *
 * DESCRIPTION        : This file contains all MoCA SOC commands and the corresponding 
 *                      response data that shall be used in communication with MxL Host 
 *                      driver 
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
 * Copyright (c) 2021, MaxLinear, Inc.
 ****************************************************************************************/

#ifndef __MXL_MOCA_SOC_CMD_H__
#define __MXL_MOCA_SOC_CMD_H__


/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
 ****************************************************************************************/
#ifndef __KERNEL__
#include <stdio.h>
#include <stdlib.h>
#endif

#include "mxl_data_types.h"
#include "mxl_moca_ioctl_cmd.h"
#include "mxl_moca_config.h"


/*****************************************************************************************
    Macros
 ****************************************************************************************/
#define CONFIG_PARAM_VERSION                0x20 /* Change this to the actual version when feature complete */

#define MXL_MOCA_SOC_CMD                    0x0010000
#define MXL_MOCA_DRV_CMD                    0x0020000
#define MXL_MOCA_DBG_CMD                    0x0040000
#define CMD_RESPONSE                        0x1000000
#define CMD_NO_RESPONSE                     0x2000000 

#define CLNK_CMD_DST_MASK                   0x03f0000
#define CLNK_CMD_RESPONSE_MASK              0xf000000 // private bitmask

#define CLNK_CMD_BYTE_MASK                  (0x000000ff)
#define CLNK_CTL_VERSION                    0x01

#define CCPU_MAGIC_NUM0_REG                 0x0c107080
#define RESET_CNT_MASK                      0x0000ffff
#define RESET_EC_OFFSET                     24
#define RESET_LINE_MASK                     0xffffff

/*
 * MxL MoCA SoC commands
 */

#define MXL_MOCA_SOC_GET_SOC_STATUS         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0001) /* HI_TXNTYPE_GET_SOC_STATUS     */
#define MXL_MOCA_SOC_GET_IMAGE_INFO         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0002) /* HI_TXNTYPE_GET_IMAGE_INFO     */
#define MXL_MOCA_SOC_SET_INIT_PARAM_V2      (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0004) /* HI_TXNTYPE_SET_INIT_PARAM_V2  */
#define MXL_MOCA_SOC_SET_INIT_RLAPM         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0005) /* HI_TXNTYPE_SET_INIT_RLAPM     */
#define MXL_MOCA_SOC_SET_INIT_SAPM          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0006) /* HI_TXNTYPE_SET_INIT_SAPM      */
#define MXL_MOCA_SOC_SET_INIT_START         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0007) /* HI_TXNTYPE_SET_INIT_START     */
#define MXL_MOCA_SOC_SET_BCN_POWER          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0008) /* HI_TXNTYPE_SET_BCN_POWER      */
#define MXL_MOCA_SOC_GET_BCN_POWER          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0009) /* HI_TXNTYPE_GET_BCN_POWER      */
#define MXL_MOCA_SOC_SET_INIT_PARAM_V25     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x000A) /* HI_TXNTYPE_SET_INIT_PARAM_V25 */
#define MXL_MOCA_SOC_GET_DATA_STATS         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x000B) /* HI_TXNTYPE_GET_DATA_STATS     */
#define MXL_MOCA_SOC_GET_AGGR_STATS         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0010) /* HI_TXNTYPE_GET_AGGR_STATS     */
#define MXL_MOCA_SOC_GET_BRIDGE_STATS       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0011) /* HI_TXNTYPE_GET_BRIDGE_STATS   */
#define MXL_MOCA_SOC_GET_BRIDGE_TABLE       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0012) /* HI_TXNTYPE_GET_BRIDGE_TABLE   */
#define MXL_MOCA_SOC_GET_EPHY_STATS         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0013) /* HI_TXNTYPE_GET_EPHY_STATS     */
#define MXL_MOCA_SOC_GET_FRAME_STATS        (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0014) /* HI_TXNTYPE_GET_FRAME_STATS    */
#define MXL_MOCA_SOC_GET_LOCAL_INFO         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0015) /* HI_TXNTYPE_GET_LOCAL_INFO     */
#define MXL_MOCA_SOC_GET_NET_INFO           (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0016) /* HI_TXNTYPE_GET_NET_INFO       */
#define MXL_MOCA_SOC_SET_MULTICAST_INFO     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0018) /* HI_TXNTYPE_SET_MULTICAST_INFO */
#define MXL_MOCA_SOC_REQUEST_HANDOFF        (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0019) /* HI_TXNTYPE_REQUEST_HANDOFF    */
#define MXL_MOCA_SOC_REQUEST_LMO            (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x001A) /* HI_TXNTYPE_REQUEST_LMO        */
#define MXL_MOCA_SOC_REQUEST_NMS            (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x001B) /* HI_TXNTYPE_REQUEST_NMS        */
#define MXL_MOCA_SOC_REQUEST_SPECTRUM       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x001C) /* HI_TXNTYPE_REQUEST_SPECTRUM   */
#define MXL_MOCA_SOC_GET_FMR_INFO           (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x001D) /* HI_TXNTYPE_REQUEST_FMR    	*/
#define MXL_MOCA_SOC_GET_DD_INFO            (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x001E) /* HI_TXNTYPE_GET_DD_INFO        */
#define MXL_MOCA_SOC_REQUEST_BCNPWR_PIE     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x001F) /* HI_TXNTYPE_REQUEST_ BCNPWR_PIE*/
#define MXL_MOCA_SOC_GET_PHY_TX_PROFILE     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0020) /* HI_TXNTYPE_GET_PHY_TX_PROFILE */
#define MXL_MOCA_SOC_GET_PHY_RX_PROFILE     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0021) /* HI_TXNTYPE_GET_PHY_RX_PROFILE */
#define MXL_MOCA_SOC_GET_PHY_NODE           (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0022) /* HI_TXNTYPE_GET_PHY_NODE       */
#define MXL_MOCA_SOC_GET_PHY_EVM            (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0023) /* HI_TXNTYPE_GET_PHY_EVM        */
#define MXL_MOCA_SOC_GET_PHY_MISC           (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0024) /* HI_TXNTYPE_GET_PHY_MISC       */
#define MXL_MOCA_SOC_GET_PHY_OFDMA          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0025) /* HI_TXNTYPE_GET_PHY_OFDMA      */
#define MXL_MOCA_SOC_GET_RETX_STATS         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0026) /* HI_TXNTYPE_GET_RETX_STATS     */
#define MXL_MOCA_SOC_GET_MULTICAST_MODE     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0027) /* HI_TXNTYPE_GET_MULTICAST_MODE */
#define MXL_MOCA_SOC_SET_MULTICAST_MODE     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0028) /* HI_TXNTYPE_SET_MULTICAST_MODE */
#define MXL_MOCA_SOC_SET_PHY_MOD_BITMASK    (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0029) /* HI_TXNTYPE_SET_PHY_MOD_BITMASK*/
#define MXL_MOCA_SOC_SLEEP                  (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0030) /* HI_TXNTYPE_SLEEP              */
#define MXL_MOCA_SOC_WAKE                   (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0031) /* HI_TXNTYPE_WAKE               */
#define MXL_MOCA_SOC_GET_PM_CAPS            (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0032) /* HI_TXNTYPE_GET_PM_CAPS        */
#define MXL_MOCA_SOC_QUERY_PM_STATE         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0033) /* HI_TXNTYPE_QUERY_PM_STATE     */
#define MXL_MOCA_SOC_GET_PM_EVENTS          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0034) /* HI_TXNTYPE_GET_PM_EVENTS      */
#define MXL_MOCA_SOC_HOST_IF_READY          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0035) /* HI_TXNTYPE_HOST_IF_READY      */
#define MXL_MOCA_SOC_CREATE_FLOW            (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0040) /* HI_TXNTYPE_CREATE_FLOW        */
#define MXL_MOCA_SOC_UPDATE_FLOW            (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0041) /* HI_TXNTYPE_UPDATE_FLOW        */
#define MXL_MOCA_SOC_DELETE_FLOW            (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0042) /* HI_TXNTYPE_DELETE_FLOW        */
#define MXL_MOCA_SOC_LIST_FLOW              (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0043) /* HI_TXNTYPE_LIST_FLOW          */
#define MXL_MOCA_SOC_QUERY_FLOW             (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0044) /* HI_TXNTYPE_QUERY_FLOW         */
#define MXL_MOCA_SOC_GET_FLOW_STATS         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0045) /* HI_TXNTYPE_GET_FLOW_STATS     */
#define MXL_MOCA_SOC_GET_SEC_PASSWORD       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0050) /* HI_TXNTYPE_GET_SEC_PASSWORD   */
#define MXL_MOCA_SOC_GET_SEC_INFO           (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0051) /* HI_TXNTYPE_GET_SEC_INFO       */
#define MXL_MOCA_SOC_GET_SEC_STATS          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0052) /* HI_TXNTYPE_GET_SEC_STATS      */
#define MXL_MOCA_SOC_GET_FRAG_STATS         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0054) /* HI_TXNTYPE_GET_FRAG_STATS     */
#define MXL_MOCA_SOC_MOCA_RESET             (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0055) /* HI_TXNTYPE_MOCA_RESET         */
#define MXL_MOCA_SOC_GET_L2ME_STATS         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0056) /* HI_TXNTYPE_GET_L2ME_STATS     */
#define MXL_MOCA_SOC_GET_ACTIVE_RLAPM_V2    (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0057) /* HI_TXNTYPE_GET_ACTIVE_RLAPM_V2*/
#define MXL_MOCA_SOC_GET_ACTIVE_SAPM_V2     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0058) /* HI_TXNTYPE_GET_ACTIVE_SAPM_V2 */
#define MXL_MOCA_SOC_GET_ENDET_STATUS       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0059) /* HI_TXNTYPE_GET_ENDET_STATUS   */
#define MXL_MOCA_SOC_GET_SEC_EP_PASSWORD    (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x005E) /* HI_TXNTYPE_GET_SEC_EP_PASSWORD*/
#define MXL_MOCA_SOC_GET_EP_SEC_INFO        (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x005F) /* HI_TXNTYPE_GET_EP_SEC_INFO    */
#define MXL_MOCA_SOC_SET_MCAST_PARAM        (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0061) /* HI_TXNTYPE_SET_MCAST_PARAM    */
#define MXL_MOCA_SOC_GET_OFDMA_SC_DEF       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0062) /* HI_TXNTYPE_GET_OFDMA_SC_DEF   */
#define MXL_MOCA_SOC_GET_OFDMA_SC_ASSIGN    (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0063) /* HI_TXNTYPE_GET_OFDMA_SC_ASSIGN*/
#define MXL_MOCA_SOC_GET_OFDMA_PARAMS       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0064) /* HI_TXNTYPE_GET_OFDMA_PARAMS   */
#define MXL_MOCA_SOC_GET_OFDMA_TX_PROFILE   (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0065) /* HI_TXNTYPE_GET_OFDMA_TX_PROFILE*/
#define MXL_MOCA_SOC_GET_OFDMA_RX_PROFILE   (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0066) /* HI_TXNTYPE_GET_OFDMA_RX_PROFILE*/
#define MXL_MOCA_SOC_SET_GET_TX_PWR_VAR     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0067) /* HI_TXNTYPE_SET_GET_TX_PWR_VAR  */
#define MXL_MOCA_SOC_DO_TEST_CMD            (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0070) /* HI_TXNTYPE_DO_TEST_CMD         */
#define MXL_MOCA_SOC_GET_SWITCH_TABLE       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0071) /* HI_TXNTYPE_GET_SWITCH_TABLE      */
#define MXL_MOCA_SOC_GET_SWITCH_STATS       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0072) /* HI_TXNTYPE_GET_SWITCH_STATS      */
#define MXL_MOCA_SOC_GET_SWITCH_CONFIG      (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0073) /* HI_TXNTYPE_GET_SWITCH_CONFIG     */
#define MXL_MOCA_SOC_RESET_SWITCH_CONFIG    (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0074) /* HI_TXNTYPE_RESET_SWITCH_CONFIG   */
#define MXL_MOCA_SOC_SET_SWITCH_TMO         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0075) /* HI_TXNTYPE_SET_SWITCH_TMO        */
#define MXL_MOCA_SOC_ENA_SWTICH_VLAN_CHECK  (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0076) /* HI_TXNTYPE_ENA_SWTICH_VLAN_CHECK */
#define MXL_MOCA_SOC_DIS_SWTICH_VLAN_CHECK  (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0077) /* HI_TXNTYPE_DIS_SWTICH_VLAN_CHECK */
#define MXL_MOCA_SOC_ENA_SWTICH_VLAN_STRIP  (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0078) /* HI_TXNTYPE_ENA_SWTICH_VLAN_STRIP */
#define MXL_MOCA_SOC_DIS_SWTICH_VLAN_STRIP  (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0079) /* HI_TXNTYPE_DIS_SWTICH_VLAN_STRIP */
#define MXL_MOCA_SOC_SET_SWTICH_VLAN_PVID   (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x007A) /* HI_TXNTYPE_SET_SWTICH_VLAN_PVID  */
#define MXL_MOCA_SOC_ADD_SWTICH_VLAN_ID     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x007B) /* HI_TXNTYPE_ADD_SWTICH_VLAN_ID    */
#define MXL_MOCA_SOC_DEL_SWTICH_VLAN_ID     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x007C) /* HI_TXNTYPE_DEL_SWTICH_VLAN_ID    */
#define MXL_MOCA_SOC_SET_SWITCH_MGMT_MAC    (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x007D) /* HI_TXNTYPE_SET_SWTICH_MGMT_MAC   */
#define MXL_MOCA_SOC_GET_MAC_DATA_STATS     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x007E) /* HI_TXNTYPE_GET_MAC_DATA_STATS    */
#define MXL_MOCA_SOC_GET_M25_PHY_MISC       (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x007F) /* HI_TXNTYPE_GET_M25_PHY_MISC      */
// Network Proxy
#define MXL_MOCA_SOC_NET_PROXY_PKT_SEND_REQ (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0081) /* HI_TXNTYPE_NET_PROXY_PKT_SEND_REQ */
#define MXL_MOCA_SOC_NET_PROXY_PKT_RCV_RSP  (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0082) /* HI_TXNTYPE_NET_PROXY_PKT_RCV_RSP  */
#define MXL_MOCA_SOC_NET_PROXY_PKT_RCV_REQ  (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0083) /* HI_TXNTYPE_NET_PROXY_PKT_RCV_REQ  */
#define MXL_MOCA_SOC_NET_PROXY_PKT_SEND_RSP (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0084) /* HI_TXNTYPE_NET_PROXY_PKT_SEND_RSP */
// Bridge detection
#define MXL_MOCA_SOC_GET_NETWORK_NAME_REC     (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0085) /* HI_TXNTYPE_GET_NETWORK_NAME_REC     */
#define MXL_MOCA_SOC_GET_NETWORK_NAME_PAYLOAD (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0086) /* HI_TXNTYPE_GET_NETWORK_NAME_PAYLOAD */
#define MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_TX (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0087) /* HI_TXNTYPE_ME_NETWORK_IE_PAYLOAD_TX */
#define MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_RX (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0088) /* HI_TXNTYPE_ME_NETWORK_IE_PAYLOAD_RX */
#define MXL_MOCA_SOC_ME_NETWORK_JOIN          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0089) /* HI_TXNTYPE_ME_NETWORK_JOIN          */
#define MXL_MOCA_SOC_ME_NETWORK_JOIN_CFG      (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x008A) /* HI_TXNTYPE_ME_NETWORK_JOIN_CFG      */
// Traffic Permission
#define MXL_MOCA_SOC_SET_TRAFFIC_PERMIT_CFG (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x008B) /* HI_TXNTYPE_SET_TRAFFIC_PERMIT_CFG  */
#define MXL_MOCA_SOC_GET_TRAFFIC_PERMIT_STS (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x008C) /* HI_TXNTYPE_GET_TRAFFIC_PERMIT_STS  */
#define MXL_MOCA_SOC_GET_ETHER_TYPE_INFO    (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x008D) /* HI_TXNTYPE_GET_ETHER_TYPE_INFO  */

// Network Proxy Response Enable - controls the response to a NetProxy SET or GET
// If the DCAP test mode is turned ON, the node in response to receiving a MIB Management GET or SET request message replies 
// with a MIB Management GET or SET response message respectively.
#define MXL_MOCA_SOC_NET_PROXY_TEST_MODE_CFG (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x008F) /* HI_TXNTYPE_NET_PROXY_TEST_MODE_CFG  */

// MPS
#define MXL_MOCA_SOC_SET_MPS_TRIGGER        (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0090) /* HI_TXNTYPE_SET_MPS_TRIGGER */
#define MXL_MOCA_SOC_GET_MPS_REPORT         (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0091) /* HI_TXNTYPE_GET_MPS_REPORT */
#define MXL_MOCA_SOC_GET_MPS_STATE          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0092) /* HI_TXNTYPE_GET_MPS_REPORT */

#define MXL_MOCA_SOC_GET_ECB_STATS          (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0093) /* HI_TXNTYPE_GET_ECB_STATS */
// Port Configure
#define MXL_MOCA_SOC_SET_SWITCH_PORT_CONFIG (CMD_RESPONSE | MXL_MOCA_SOC_CMD | 0x0094) /* HI_TXNTYPE_SET_SWITCH_PORT_CONFIG */

/*
 * MxL MoCA Driver commands
 */

#define MXL_MOCA_DRV_SET_OTP                        (MXL_MOCA_DRV_CMD | 0x01)  // Set OTP parameter 
#define MXL_MOCA_DRV_GET_CFG_AND_FW_DIR             (MXL_MOCA_DRV_CMD | 0x02)  // Get config dir and fw dir
#define MXL_MOCA_DRV_RESET_DEVICE                   (MXL_MOCA_DRV_CMD | 0x04)  // Reset the SoC. 
#define MXL_MOCA_DRV_STOP_DEVICE                    (MXL_MOCA_DRV_CMD | 0x05)  // Stop the SoC. 
#define MXL_MOCA_DRV_GET_DAEMON_INFO                (MXL_MOCA_DRV_CMD | 0x06)  // Get Daemon Information 
#define MXL_MOCA_DRV_SET_BOOT_SOC                   (MXL_MOCA_DRV_CMD | 0x07)  // Set socBooted and open Eth interface.
#define MXL_MOCA_DRV_SET_MAC_ADDR                   (MXL_MOCA_DRV_CMD | 0x08)  // Sets MAC address in the Driver subsystem.
#ifdef SET_CD_FW_SUPPORT
#define MXL_MOCA_DRV_SET_CD_FIRMWARE                (MXL_MOCA_DRV_CMD | 0x09)  // Set the MXL_MOCA_FIRMWARE_T in Clink Daemon 
#endif
#define MXL_MOCA_DRV_SET_DATA_PLANE_VARS            (MXL_MOCA_DRV_CMD | 0x0A)  // Set Data Plane Variables.
#define MXL_MOCA_DRV_GET_CHIP_TYPE                  (MXL_MOCA_DRV_CMD | 0x0B)  // Gets the current Chip type of Soc. 
#define MXL_MOCA_DRV_START_DAEMON                   (MXL_MOCA_DRV_CMD | 0x0C)  // Start daemon thread. 
#define MXL_MOCA_DRV_HW_RESET                       (MXL_MOCA_DRV_CMD | 0x0D)  // Hardware reset
#define MXL_MOCA_DRV_SW_RESET                       (MXL_MOCA_DRV_CMD | 0x0E)  // Software reset
#define MXL_MOCA_DRV_DEV_LOW_PWR_WITH_ESWITCH_ON    (MXL_MOCA_DRV_CMD | 0x0F)  // Standalone low power mode with eSwitching active

#define MXL_MOCA_DRV_SET_APPLICATION_TRACE          (MXL_MOCA_DRV_CMD | 0x10)  // Send the Application clnktrace notification on/off to driver

/*
 * MxL MoCA Debug commands
 */

#define MXL_MOCA_DBG_GET_MEM               (MXL_MOCA_DBG_CMD | 0x01) // Perform extended SoC memory read operation.
#define MXL_MOCA_DBG_SET_MEM               (MXL_MOCA_DBG_CMD | 0x02) // Perform extended SoC memory write operation.

/*****************************************************************************************
    User-Defined Types (Typedefs)
 ****************************************************************************************/

typedef struct
{
  uint32_t result;
  int8_t   str[MAX_PASSWORD_LENGTH_PADDED];
} MXL_MOCA_SEC_PASSWORD_T; //SecPassword_t;

/** Represents an address in the MaxLinear SoC FW */
typedef unsigned int MXL_SOCFW_ADDR_T; //En2710_CcpuAddr_t;

typedef struct
{
  uint32_t address; // MoCA SOC internal memory address
  uint32_t *pBuf;   // Pointer to user buffer
  uint32_t bufsize;
} MXL_MOCA_SOC_MEMORY_INFO_T;

typedef struct
{
  uint32_t      cmdBytes;
  uint32_t      cmdArray[1000];    // must adjust MAX_MBX_MSG to match this
  uint32_t      opId;
  uint32_t      resId;
  uint32_t      rspBytes;
  uint32_t      rspArray[1000];    // must adjust MAX_MBX_MSG to match this
} MXL_MOCA_SOC_IOCTL_SEND_RCV_MBX;

typedef struct
{
  uint32_t trapInfoSize;      // Trap info area size in bytes
  uint32_t errCode;           // Error code
  uint32_t errFileId;         // Error file ID (see Diag_FileId.h)
  uint32_t errLine;           // Error line number
  uint32_t bootStatus;        // Boot status
  uint32_t bootFileId;        // Boot file ID (see Diag_FileId.h)
  uint32_t bootLine;          // Boot line number
  uint32_t epc;               // Exception program counter
  uint32_t sp;                // Stack pointer (CPU register)
  uint32_t lr;                // Link register (CPU register)
  uint32_t threadName;        // Thread's name
  uint32_t threadRunCount;    // Thread's run counter
  uint32_t threadStackPtr;    // Thread's stack pointer
  uint32_t threadStackStart;  // Stack starting address
  uint32_t threadStackEnd;    // Stack ending address
  uint32_t threadStackSize;   // Stack size
  uint32_t threadPriority;    // Priority of thread (0-1023)
  uint32_t threadState;       // Thread's execution state
  uint32_t stackDump[16];     // Stack dump
} MXL_MOCA_SOC_TRAP_INFO_T;

/****************************************************************************************
    Try to list all data types documented in SoC HI.docx
 ****************************************************************************************/

// MXL_MOCA_SOC_GET_SOC_STATUS / HI_TXNTYPE_GET_SOC_STATUS

typedef struct
{
  uint32_t status;
  uint32_t fatalCode;
  uint32_t state;
  uint32_t dbg;
  uint32_t bbTemp;
  uint32_t rfTemp;
} MXL_MOCA_SOC_GET_SOC_STATUS_RSP_T;   /* Hi_TxnGetSocStatusRsp_t*/

// MXL_MOCA_SOC_GET_IMAGE_INFO / HI_TXNTYPE_GET_IMAGE_INFO

typedef struct
{
  uint32_t messageVersion;
  uint32_t hi;
  uint32_t ecl;
  uint32_t lc;
  uint32_t mac;
  uint32_t phyCtl;
  uint32_t sws;
  uint32_t hal;
  uint32_t reserved0;
  uint32_t reserved1;
  uint32_t tcDic;
  uint32_t tcCpc;
  uint32_t tcHif;
  uint32_t compile;
  uint32_t socVerStr[MAX_VERSION_STR_SIZE / 4];
} MXL_MOCA_SOC_GET_IMAGE_INFO_RSP_T;   /* Hi_TxnGetImageInfoRsp_t */

// MXL_MOCA_SOC_SET_INIT_START / HI_TXNTYPE_SET_INIT_START

typedef struct
{
  uint32_t accepted;
  uint32_t initStatusWord;
  uint32_t initExceptionWord;
  uint32_t initDataWord;
} MXL_MOCA_SOC_INIT_SYSTEM_START_RSP_T;   /* Hi_TxnSetInitSystemStartRsp_t */

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_AGGR_STATS_CMD_T;     /* Hi_TxnGetAggrStatsCmd_t */

// MXL_MOCA_SOC_GET_AGGR_STATS / HI_TXNTYPE_GET_AGGR_STATS

typedef struct
{
  uint32_t txAggNone;
  uint32_t txAggP2[MAX_AGGR_FRAMES_V25-1];
  uint32_t rxAggNone;
  uint32_t rxAggP2[MAX_AGGR_FRAMES_V25-1];
} MXL_MOCA_SOC_GET_AGGR_STATS_RSP_T;   /* Hi_TxnGetAggrStatsRsp_t */

// MXL_MOCA_SOC_GET_BRIDGE_STATS / HI_TXNTYPE_GET_BRIDGE_STATS

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_BRIDGE_STATS_CMD_T;   /* Hi_TxnGetBridgeStatsCmd_t */

typedef struct
{
  uint32_t ucastDestEntries;
  uint32_t mcastDestEntries;
  uint32_t pqosDestEntries;
  uint32_t ucastSrcEntries;
  uint32_t mcastSrcEntries;
  uint32_t totalHits;
  uint32_t totalMisses;
} MXL_MOCA_SOC_GET_BRIDGE_STATS_RSP_T;   /* Hi_TxnGetBridgeStatsRsp_t */

// MXL_MOCA_SOC_GET_BRIDGE_TABLE / HI_TXNTYPE_GET_BRIDGE_TABLE

typedef struct
{
  uint32_t type;
  uint32_t startIndex;
} MXL_MOCA_SOC_GET_BRIDGE_TABLE_CMD_T;   /* Hi_TxnGetBridgeTableCmd_t*/

typedef struct
{
  uint32_t macAddrHi;
  uint32_t macAddrLoIds;
} MXL_MOCA_SOC_BRIDGE_ENTRY_T;          /* Hi_TxnBridgeEntry_t */

typedef struct
{
  uint32_t                    maxEntries;
  MXL_MOCA_SOC_BRIDGE_ENTRY_T entry[32];
} MXL_MOCA_SOC_GET_BRIDGE_TABLE_RSP_T;   /* Hi_TxnGetBridgeTableRsp_t */

// MXL_MOCA_SOC_GET_EPHY_STATS / HI_TXNTYPE_GET_EPHY_STATS

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_EPHY_STATS_CMD_T;   /* Hi_TxnGetEphyStatsCmd_t */

// MXL_MOCA_SOC_GET_ECB_STATS / HI_TXNTYPE_GET_ECB_STATS
typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_ECB_STATS_CMD_T;   /* Hi_TxnGetEcbStatsCmd_t */

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
} MXL_MOCA_SOC_GET_EPHY_STATS_RSP_T;   /* Hi_TxnGetEphyStatsRsp_t */

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
    
} MXL_MOCA_SOC_GET_ECB_STATS_RSP_T;   /* Hi_TxnGetEphyStatsRsp_t */

// MXL_MOCA_SOC_GET_RETX_STATS / HI_TXNTYPE_GET_RETX_STATS 

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_RETRANS_STATS_CMD_T; /* Hi_TxnGetRetxStatsCmd_t */

typedef struct
{
  uint32_t retxFrames[16];
  uint32_t txTimeout[16];
} MXL_MOCA_SOC_GET_RETRANS_STATS_RSP_T;

// MXL_MOCA_SOC_GET_FRAG_STATS / HI_TXNTYPE_GET_FRAG_STATS 

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_FRAG_STATS_CMD_T;

typedef struct
{
  uint32_t rxFragFrames;
  uint32_t txFragFrames;
} MXL_MOCA_SOC_GET_FRAG_STATS_RSP_T;

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_DATA_STATS_CMD_T;

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
} MXL_MOCA_SOC_GET_DATA_STATS_RSP_T;

// MXL_MOCA_SOC_GET_L2ME_STATS / HI_TXNTYPE_GET_L2ME_STATS 

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_L2ME_STATS_CMD_T;

typedef struct
{
  uint32_t l2meIssuedTxns;
  uint32_t l2meSuccessfulTxns;
  uint32_t l2meTimedOutCycles;
  uint32_t l2meFailedTxns;
} MXL_MOCA_SOC_GET_L2ME_STATS_RSP_T;

// MXL_MOCA_SOC_GET_MULTICAST_MODE / HI_TXNTYPE_SET_MULTICAST_MODE

typedef struct
{
  uint32_t mode;
} MXL_MOCA_SOC_GET_MCAST_MODE_RSP_T;      /* Hi_TxnGetMcastModeRsp_t */

// MXL_MOCA_SOC_SET_MULTICAST_MODE / HI_TXNTYPE_SET_MULTICAST_MODE

typedef struct
{
  uint32_t mode;
} MXL_MOCA_SOC_SET_MCAST_MODE_CMD_T;      /* Hi_TxnSetMcastModeCmd_t */

typedef struct
{
  uint32_t result;
} MXL_MOCA_SOC_SET_MCAST_MODE_RSP_T;      /* Hi_TxnSetMcastModeRsp_t */

// MXL_MOCA_SOC_SET_GET_TX_PWR_VAR / HI_TXNTYPE_SET_GET_TX_PWR_VAR 

typedef struct
{
  uint32_t   set;                 // If true, set to new value before get
  uint32_t   nodeId;
  uint32_t   txPwrVariation;      // New value if 'set' is true
} MXL_MOCA_SOC_SET_GET_TXPWR_VAR_CMD_T;   /* Hi_TxnSetGetTxPwrVarCmd_t */

typedef struct
{
  uint32_t   txPwrVariation;      // Current Tx power variation value
} MXL_MOCA_SOC_SET_GET_TXPWR_VAR_RSP_T;   /* Hi_TxnSetGetTxPwrVarRsp_t */

// MXL_MOCA_SOC_SET_PHY_MOD_BITMASK / HI_TXNTYPE_SET_PHY_MOD_BITMASK

typedef struct
{
  uint32_t nodeId;
  uint32_t type;
  uint32_t value;
} MXL_MOCA_SOC_SET_PHYMODE_BITMASK_CMD_T;   /* Hi_TxnSetPhyModBitmaskCmd_t */

typedef struct
{
  uint32_t result;
} MXL_MOCA_SOC_SET_PHYMODE_BITMASK_RSP_T;   /* Hi_TxnSetPhyModBitmaskRsp_t */

// MXL_MOCA_SOC_SET_MULTICAST_INFO  / HI_TXNTYPE_SET_MULTICAST_INFO

typedef struct
{
  uint32_t macAddrHi;
  uint32_t macAddrLo;
  uint32_t mode;
} MXL_MOCA_SOC_SET_MULTICAST_FILTER_CMD_T;

typedef struct
{
  uint32_t result;
} MXL_MOCA_SOC_SET_MULTICAST_FILTER_RSP_T;

typedef struct
{
  /**
   * The exclusive set of multicast MAC address allowed to pass
   * from MoCA to host when filter mode is set to Enabled. When
   * filter mode is set to DfidOnly, then the following list of
   * MAC addresses are only filtered by MAP DFID and not by
   * precise MAC Address.  This is only useful for internal
   * testing.
   *
   * The first array entry with a 0 in macAddrHi indicates that it
   * and all following array entries do not contain a valid entry.
   */
  uint32_t macAddrHi[48];
  uint32_t macAddrLo[48];
  /**
   * The filter mode enables or disables multicast filtering
   * for the MoCA device.
   *
   * The values for filter mode are: Disabled (0), Enabled (1),
   *  and DfidOnly (2).
   */
  uint32_t mode;
} MXL_MOCA_SOC_SET_MULTICAST_CFG_CMD_T;  //Hi_TxnSetMultiCastCfgCmd_t

typedef struct
{
  uint32_t result;
} MXL_MOCA_SOC_SET_MULTICAST_CFG_RSP_T;

///3.5 Link

// MXL_MOCA_SOC_REQUEST_HANDOFF / HI_TXNTYPE_REQUEST_HANDOFF

typedef struct
{
  uint32_t nodeId;
  uint32_t isBnc;
} MXL_MOCA_SOC_REQ_HANDOFF_CMD_T;

typedef struct
{
  uint32_t status;
} MXL_MOCA_SOC_REQ_HANDOFF_RSP_T;

// MXL_MOCA_SOC_REQUEST_LMO / HI_TXNTYPE_REQUEST_LMO

typedef struct
{
  uint32_t type;    //!< LMO type

  union
  {
    struct
    {
      uint32_t type;          //!< Assessment type 0 - Quiet , 1 - EVM
      uint32_t channel;
      uint32_t srcNodeId;
      uint32_t dstNodeMask;
    } aca;

    struct
    {
      uint32_t nodeId;  //!< OOO lmo node ID
    } ooo;

    struct
    {
      uint32_t type;     //!< 0 - Unicast, 1 OFDMA
      uint32_t nodeMask; //!< Mask of nodes involved
    } upr;

    struct
    {
      uint32_t nodeId;  //!< Tx node Id
      uint32_t probeId; //!< Pre-Defined Probe identifier
    } rdp;

    struct
    {
      uint32_t timeSlots;
    } silent;

    struct
    {
      uint32_t timeSlots;
    } loopBack;

  } data;
} MXL_MOCA_SOC_REQ_LMO_CMD_T;

typedef struct
{
  uint32_t type;
  uint32_t nodeId;
  uint32_t result;

  union
  {
    struct
    {
      uint32_t type;          //!< Assessment type 0 - Quiet , 1 - EVM
      uint32_t txStatus;
      uint32_t status;
      uint32_t power;
      uint32_t relativePower;
      uint8_t  powerProfile[LMO_ACA_RPT_PWR_PRFL_SZ];
    } aca;
  } data;
} MXL_MOCA_SOC_REQ_LMO_RSP_T;

// MXL_MOCA_SOC_MOCA_RESET / HI_TXNTYPE_MOCA_RESET

typedef struct
{
  uint32_t   lof;
  uint32_t   nodemask;
  uint32_t   seconds;
} MXL_MOCA_SOC_MOCA_RESET_CMD_T;   /* Hi_TxnMocaResetRequest_t */

typedef struct
{
  uint32_t   mrCode;
  uint32_t   issuedNodemask;
  uint32_t   topLastResetStatus;
  uint32_t   nodeLastResetStatus[MAX_NODES];
  uint32_t   nodeLastResponseCode[MAX_NODES];
} MXL_MOCA_SOC_MOCA_RESET_RSP_T;    /* Hi_TxnMocaResetResponse_t */

// MXL_MOCA_SOC_GET_FRAME_STATS / HI_TXNTYPE_GET_FRAME_STATS

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_FRAME_STATS_CMD_T;   /* Hi_TxnGetFrameStatsCmd_t */

typedef struct
{
  uint64_t totalMap;
  uint64_t totalRr;
  uint64_t totalLc;
  uint64_t totalBcn;
  uint64_t totalAcf;
  uint64_t totalProbe;
  uint64_t totalEth;
  uint64_t totalOfdmaRr;
  uint64_t totalOfdmaEvm;
} MXL_MOCA_SOC_CATEGORY_T;   /*  Hi_Category_t */

typedef struct
{
  MXL_MOCA_SOC_CATEGORY_T txGood;
  MXL_MOCA_SOC_CATEGORY_T txError;
  MXL_MOCA_SOC_CATEGORY_T txDropped;
  MXL_MOCA_SOC_CATEGORY_T rxGood;
  MXL_MOCA_SOC_CATEGORY_T rxError;
  MXL_MOCA_SOC_CATEGORY_T rxDropped;
} MXL_MOCA_SOC_GET_FRAME_STATS_RSP_T;   /* Hi_TxnGetFrameStatsRsp_t */


// MXL_MOCA_SOC_GET_LOCAL_INFO / HI_TXNTYPE_GET_LOCAL_INFO

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
  uint32_t netState;
  uint32_t resetCnt;
  uint32_t mocaVer;
  uint32_t activeMask;
  uint32_t sleepM1NodeBitmask;
  uint32_t sleepM2NodeBitmask;
  uint32_t sleepM3NodeBitmask;
  uint32_t tabooMask;
  uint32_t tabooOffset;
  uint32_t priChnOffset;
  uint32_t secChnOffset;
  uint32_t pqosFlowPolicingFlag;
  uint32_t socVersionStr[MAX_VERSION_STR_SIZE / 4];
  uint32_t reqStatus;
  uint32_t customBandProfCGapMode;
} MXL_MOCA_SOC_GET_LOCAL_INFO_RSP_T;    /* Hi_TxnGetLocalInfoRsp_t */

// MXL_MOCA_SOC_GET_NET_INFO / HI_TXNTYPE_GET_NET_INFO

typedef struct
{
  uint32_t nodeId;
} MXL_MOCA_SOC_GET_NET_INFO_CMD_T;   /* Hi_TxnGetNetInfoCmd_t */

typedef struct
{
  uint32_t guidHi;
  uint32_t guidLo;
  uint32_t state;
  uint32_t protoSupport;
  uint32_t mocaVer;
  uint64_t txCtlFrameGood;          // Number of MoCA Ctl Frames / Probes successfully TXed
  uint64_t txCtlFrameError;         // Number of MoCA Ctl Frames / Probes that errored out (Tx Late) during TX
  uint64_t txCtlFrameDropped;       // Number of MoCA Ctl Frames / Probes that were dropped
  uint64_t rxCtlFrameGood;          // Number of MoCA Ctl Frames / Probes successfully RXed
  uint64_t rxCtlFrameError;         // Number of MoCA Ctl Frames / Probes that errored out (Rx Late, CRC Err) during RX
  uint64_t rxCtlFrameDropped;       // Number of MoCA Ctl Frames / Probes that were dropped
  uint64_t txDataFrameGood;         // Number of MoCA Data Frames successfully TXed
  uint64_t txDataFrameDropped;      // Number of MoCA Data Frames dropped (TX Data Late)
  uint64_t rxDataFrameGood;         // Number of MoCA Data Frames successfully RXed
  uint64_t rxDataFrameBad;          // Number of MoCA Data Frames with errors / drops during RX (RX Data Late, MPDU HDR CRC Err, APDU HDR CRC Err, BD Failure, PDU CRC Err)
} MXL_MOCA_SOC_GET_NET_INFO_RSP_T;   /* Hi_TxnGetNetInfoRsp_t */

// MXL_MOCA_SOC_GET_FMR_INFO / HI_TXNTYPE_REQUEST_FMR

typedef struct
{
  uint32_t nodeMask;
  uint32_t fmrVer;
} MXL_MOCA_SOC_GET_FMR_INFO_CMD_T;

typedef struct
{
  uint32_t rsp[104];
} MXL_MOCA_SOC_GET_FMR_INFO_RSP_T;

// MXL_MOCA_SOC_GET_DD_INFO / HI_TXNTYPE_GET_DD_INFO

typedef struct
{
  uint32_t nodeMask;
} MXL_MOCA_SOC_GET_DD_INFO_CMD_T;  // Device Discovery Info

typedef struct
{
  uint32_t rsp[100];
} MXL_MOCA_SOC_GET_DD_INFO_RSP_T;

// MXL_MOCA_SOC_REQUEST_NMS / HI_TXNTYPE_REQUEST_NMS

typedef struct
{
  uint32_t reserved;  //!< Reserved
} MXL_MOCA_SOC_REQ_NMS_CMD_T;

//! NMS response information
typedef struct
{
  uint32_t result;
} MXL_MOCA_SOC_REQ_NMS_RSP_T;

// MXL_MOCA_SOC_REQUEST_SPECTRUM / HI_TXNTYPE_REQUEST_SPECTRUM

typedef struct
{
  uint32_t request;
} MXL_MOCA_SOC_REQ_SPECTRUM_CMD_T;

typedef struct
{
  uint32_t result;
} MXL_MOCA_SOC_REQ_SPECTRUM_RSP_T;

// MXL_MOCA_SOC_GET_PHY_TX_PROFILE / HI_TXNTYPE_GET_PHY_TX_PROFILE

typedef struct
{
  uint32_t nodeId;
  uint32_t profileId;
  uint32_t getBitloading;
} MXL_MOCA_SOC_GET_PHY_TX_PROFILE_CMD_T;   /* Hi_TxnGetPhyTxProfileCmd_t */

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
} MXL_MOCA_SOC_GET_PHY_TX_PROFILE_RSP_T;   /* Hi_TxnGetPhyTxProfileRsp_t */

// MXL_MOCA_SOC_GET_PHY_RX_PROFILE / HI_TXNTYPE_GET_PHY_RX_PROFILE

typedef struct
{
  uint32_t nodeId;
  uint32_t profileId;
  uint32_t getBitloading;
} MXL_MOCA_SOC_GET_PHY_RX_PROFILE_CMD_T;   /* Hi_TxnGetPhyRxProfileCmd_t */

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
} MXL_MOCA_SOC_GET_PHY_RX_PROFILE_RSP_T;   /* Hi_TxnGetPhyRxProfileRsp_t */

// MXL_MOCA_SOC_GET_PHY_NODE / HI_TXNTYPE_GET_PHY_NODE

typedef struct
{
  uint32_t nodeId;
} MXL_MOCA_SOC_GET_PHY_NODE_CMD_T;   /* Hi_TxnGetPhyNodeCmd_t */

typedef struct
{
  uint32_t valid;
  uint32_t freqOffset;
  uint32_t rangeValid;
  uint32_t nodeDelay;
  uint32_t validTxProf;
} MXL_MOCA_SOC_GET_PHY_NODE_RSP_T;   /* Hi_TxnGetPhyNodeRsp_t */

// MXL_MOCA_SOC_GET_PHY_EVM / HI_TXNTYPE_GET_PHY_EVM

typedef struct
{
  uint32_t nodeId;
  uint32_t profileId;
} MXL_MOCA_SOC_GET_PHY_EVM_CMD_T;   /* Hi_TxnGetPhyEvmCmd_t */

typedef struct
{
  uint32_t rxProfileValid;
  uint32_t rxEvm[128];
} MXL_MOCA_SOC_GET_PHY_EVM_RSP_T;   /* Hi_TxnGetPhyEvmRsp_t */

// MXL_MOCA_SOC_GET_PHY_MISC / HI_TXNTYPE_GET_PHY_MISC

typedef struct
{
  uint32_t temperature;
  uint32_t currentOperatingFrequency;
} MXL_MOCA_SOC_GET_PHY_MISC_RSP_T;   /* Hi_TxnGetPhyMiscRsp_t */

// MXL_MOCA_SOC_GET_M25_PHY_MISC / HI_TXNTYPE_GET_M25_PHY_MISC

typedef struct
{
  uint32_t temperature;
  uint32_t currentOperatingFrequency;
  uint32_t firstChannelFrequency;
  uint32_t numberM25Channels;
} MXL_MOCA_SOC_GET_M25_PHY_MISC_RSP_T; /* Hi_TxnGetM25PhyMiscRsp_t*/

// MXL_MOCA_SOC_GET_OFDMA_SC_DEF / HI_TXNTYPE_GET_OFDMA_SC_DEF

typedef struct
{
  struct
  {
    uint32_t subchanDefID;
    uint32_t nodeBitmask;
    uint32_t bitsPerSubchanelPerSym;
    uint32_t gapOfdma;
    uint32_t numSubchan;
  } profile[MAX_OFDMA_PROFILES];
} MXL_MOCA_SOC_GET_PHY_OFDMA_DEF_RSP_T;   /* Hi_TxnGetPhyOfdmaDefRsp_t */

// MXL_MOCA_SOC_GET_OFDMA_SC_ASSIGN / HI_TXNTYPE_GET_OFDMA_SC_ASSIGN

typedef struct
{
  struct
  {
    uint32_t frameID;
    uint32_t subchanDefID;
    uint32_t numSubchan;
    uint32_t nodeBitmask;
    uint32_t cpLen;
  } profile[MAX_OFDMA_PROFILES];
} MXL_MOCA_SOC_GET_PHY_OFDMA_ASSIGN_RSP_T;   /* Hi_TxnGetPhyOfdmaAssignRsp_t */

// MXL_MOCA_SOC_GET_OFDMA_PARAMS / HI_TXNTYPE_GET_OFDMA_PARAMS

typedef struct
{
  int32_t  result;
  uint32_t ofdmaGrps;
  uint32_t m2NodeMask;
  uint32_t ofdmaNodeMask;
} MXL_MOCA_SOC_GET_OFDMA_PARAMS_RSP_T;   /* Hi_TxnGetOfdmaParamsRsp_t */

// MXL_MOCA_SOC_GET_OFDMA_TX_PROFILE / HI_TXNTYPE_GET_OFDMA_TX_PROFILE

typedef struct
{
  int32_t  result;
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
  int32_t  TPC;
  uint32_t propDelay;
  int32_t  propDelayOffset;
  uint32_t propDelayMax;
  int32_t  freqOffset;
  int32_t  txIq[2];
} MXL_MOCA_SOC_GET_OFDMA_TX_PROFILE_RSP_T;   /* Hi_TxnGetOfdmaTxProfileRsp_t */

// MXL_MOCA_SOC_GET_OFDMA_RX_PROFILE / HI_TXNTYPE_GET_OFDMA_RX_PROFILE

typedef struct
{
  int32_t  result;
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
  int32_t  TPC[8];
  uint32_t digitalGain;
  uint32_t analogGain;
  uint32_t feGain;
  uint32_t beGain;
  int32_t  powerLevel;
  uint32_t propDelayMax;
  int32_t  rxIq[2];
  int32_t  rxIdDslope[4];
} MXL_MOCA_SOC_GET_OFDMA_RX_PROFILE_RSP_T;   /* Hi_TxnGetOfdmaRxProfileRsp_t */

// MXL_MOCA_SOC_GET_OFDMA_TX_PROFILE / HI_TXNTYPE_GET_OFDMA_TX_PROFILE

typedef struct
{
  uint32_t role;          // Either NC or BNC
  uint32_t asgnTblIdx;    // Index of assignment table that you want access
} MXL_MOCA_SOC_GET_OFDMA_TX_PROFILE_CMD_T;   /* Hi_TxnGetOfdmaTxProfileCmd_t*/

// MXL_MOCA_SOC_GET_OFDMA_RX_PROFILE / HI_TXNTYPE_GET_OFDMA_RX_PROFILE
typedef struct
{
  uint32_t profileNum;
  uint32_t asgnTblIdx;    // Index of assignment table that you want access
} MXL_MOCA_SOC_GET_OFDMA_RX_PROFILE_CMD_T;    /* Hi_TxnGetOfdmaRxProfileCmd_t */

// Power Management

// MXL_MOCA_SOC_SLEEP / HI_TXNTYPE_SLEEP

typedef struct
{
  uint32_t requestedState;
} MXL_MOCA_SOC_POWER_SLEEP_CMD_T;   /* Hi_TxnPowerSleepCmd_t */

typedef struct
{
  uint32_t actualState;
  uint32_t result;
} MXL_MOCA_SOC_POWER_SLEEP_RSP_T;   /* Hi_TxnPowerSleepRsp_t */

// MXL_MOCA_SOC_WAKE / HI_TXNTYPE_WAKE

typedef struct
{
  uint32_t nodeId;
} MXL_MOCA_SOC_POWER_WAKE_CMD_T;   /* Hi_TxnPowerWakeCmd_t */


typedef struct
{
  uint32_t actualState;
  uint32_t result;
} MXL_MOCA_SOC_POWER_WAKE_RSP_T;    /* Hi_TxnPowerWakeRsp_t */

// MXL_MOCA_SOC_GET_PM_CAPS / HI_TXNTYPE_GET_PM_CAPS

typedef struct
{
  uint32_t capsMask;
  uint32_t result;
} MXL_MOCA_SOC_GET_PM_CAPS_RSP_T;   /* Hi_TxnGetPmCapsRsp_t */

// MXL_MOCA_SOC_QUERY_PM_STATE / HI_TXNTYPE_QUERY_PM_STATE

typedef struct
{
  uint32_t nodeId;
} MXL_MOCA_SOC_QUERY_PM_STATE_CMD_T;   /* Hi_TxnQueryPmStateCmd_t */

typedef struct
{
  uint32_t powerState;
  uint32_t lastHbIvSecs;
  int32_t  ncHbIvExpiresInSecs;
  uint32_t clStartedHbIvTimerCount;
} MXL_MOCA_SOC_QUERY_PM_STATE_RSP_T;    /* Hi_TxnQueryPmStateRsp_t */

// MXL_MOCA_SOC_GET_PM_EVENTS / HI_TXNTYPE_GET_PM_EVENTS

typedef struct
{
  uint32_t clearEvent;
} MXL_MOCA_SOC_GET_PM_EVENT_CMD_T;    /* Hi_TxnGetPmEventCmd_t */

typedef struct
{
  uint32_t nodeId;
  uint32_t isLocal;
  uint32_t state;
} MXL_MOCA_SOC_PM_SLEEP_EVENT_DATA_T;   /* Hi_PmSleepEventData_t */

typedef struct
{
  uint32_t nodeId;
  uint32_t isLocal;
} MXL_MOCA_SOC_PM_WAKE_EVENT_DATA_T;    /* Hi_PmWakeEventData_t */

typedef struct
{
  uint32_t nodeId;
  uint32_t isLocal;
} MXL_MOCA_SOC_PM_WOM_EVENT_DATA_T;    /* Hi_PmWomEventData_t */


typedef union
{
  MXL_MOCA_SOC_PM_SLEEP_EVENT_DATA_T   sleep;
  MXL_MOCA_SOC_PM_WAKE_EVENT_DATA_T    wake;
  MXL_MOCA_SOC_PM_WOM_EVENT_DATA_T     wom;
} MXL_MOCA_SOC_PM_EVENT_DATA_T;    /* Hi_PmEventData_t */

typedef struct
{
  int32_t  event;          // -1 if no event
  MXL_MOCA_SOC_PM_EVENT_DATA_T eventData;
  uint32_t result;
} MXL_MOCA_SOC_GET_PM_EVENT_RSP_T;    /*  Hi_TxnGetPmEventRsp_t */

// MXL_MOCA_SOC_HOST_IF_READY / HI_TXNTYPE_HOST_IF_READY

typedef struct
{
  uint32_t result;
} MXL_MOCA_SOC_HOST_IF_READY_RSP_T;    /* Hi_TxnHostIfReadyRsp_t */


///QOS

typedef struct
{
  uint32_t hi;
  uint32_t lo;
} MXL_MOCA_MAC_ADDR_T;

typedef uint32_t MXL_MOCA_SOC_FLOWCODE_T;

typedef struct
{
  MXL_MOCA_MAC_ADDR_T      flowId;             //!< PQoS flow ID
  uint32_t                 tPktSize;           //!< Traffic packet size
  MXL_MOCA_MAC_ADDR_T      ingressGuid;        //!< Ingress GUID
  MXL_MOCA_MAC_ADDR_T      egressGuid;         //!< Egress GUID
  uint32_t                 flowTag;            //!< Flow tag
  MXL_MOCA_MAC_ADDR_T      packetDa;           //!< Packet destination address
  uint32_t                 tPeakDataRateKbps;  //!< Traffic peak data rate
  uint32_t                 tLeaseTimeSec;      //!< Traffic lease time
  uint32_t                 tBurstSize;         //!< Traffic burst size
  uint32_t                 maxLatencyMs;       //!< M2: Maximum latency
  uint32_t                 stAvgRatio;         //!< M2: Short term average ratio
  uint32_t                 maxNumRetry;        //!< M2: Maximum number of retx attempts
  uint32_t                 flowPer;            //!< M2: Flow PER (NPER or VLPER)
  uint32_t                 inOrderDeliver;     //!< M2, no query: In order delivery recommendation
  uint32_t                 tProtocol;          //!< M2, no query: Traffic protocol
  uint32_t                 ingressClass;       //!< M2: Ingress classification rule
  uint32_t                 vlanTag;            //!< M2: VLAN tag
  uint32_t                 dscpMoca;           //!< M2: DSCP MoCA value
} MXL_MOCA_SOC_PFP_CMD_T;              /* Hi_PFP_t */

// MXL_MOCA_SOC_CREATE_FLOW / HI_TXNTYPE_CREATE_FLOW
// MXL_MOCA_SOC_UPDATE_FLOW / HI_TXNTYPE_UPDATE_FLOW

typedef struct
{
  MXL_MOCA_SOC_FLOWCODE_T flowCode;           //!< Response code, encodes DECISION
  uint32_t                postStps;           //!< W1REQ: POST_STPS
  uint32_t                postTxps;           //!< W1REQ: POST_TXPS
  uint32_t                dfid;               //!< W1REQ: DFID
  uint32_t                maxRetryNum;        //!< W1REQ: Maximum allowed retry number
  uint32_t                maxShortTermAvRatio;//!< W1REQ: Maximum allowed short term avg ratio
  uint32_t                remainKbps;         //!< W1REQ: MAX_PEAK_DATA_RATE
  uint32_t                remainBurst;        //!< W1REQ: MAX_BURST_SIZE
  uint32_t                decision;           //!< W1REQ: DECISION
  uint32_t                bwLimitInfo;        //!< W1REQ: BW_LIMIT_INFO
  uint32_t                isIngressLimited;   //!< W1REQ: BW_LIMIT_INFO(3)
  uint32_t                isEgressLimited;    //!< W1REQ: BW_LIMIT_INFO(3)
  uint32_t                isAggrStpsLimited;  //!< W1REQ: BW_LIMIT_INFO(1)
  uint32_t                isAggrTxpsLimited;  //!< W1REQ: BW_LIMIT_INFO(0)
  uint32_t                currMaxStps;        //!< Estimate of current max MOCA STPS
  uint32_t                currMaxTxps;        //!< Estimate of current max MOCA TXPS
} MXL_MOCA_SOC_FLOW_CREATE_OR_UPDATE_RSP_T;  /* Hi_FlowCURsp_t */

// MXL_MOCA_SOC_DELETE_FLOW / HI_TXNTYPE_DELETE_FLOW

//! Flow delete response
typedef struct
{
  MXL_MOCA_SOC_FLOWCODE_T flowCode;        //!< Response
  uint32_t                numNodesDelRsrc; //!< Number of nodes that deleted something
} MXL_MOCA_SOC_FLOW_DELETE_RSP_T;      /* Hi_FlowDeleteRsp_t */

// MXL_MOCA_SOC_LIST_FLOW / HI_TXNTYPE_LIST_FLOW

//! Flow list request.  
typedef struct
{
  MXL_MOCA_MAC_ADDR_T     ingressGuid;  //!< Queried node
  uint32_t                flowStartIndex;        //!< First flow index to return in logical table
  uint32_t                flowMaxReturn;         //!< Maximum number of flows to return
} MXL_MOCA_SOC_FLOW_LIST_CMD_T;        /* Hi_FlowListReq_t */

//! Flow list response. 
typedef struct
{
  MXL_MOCA_SOC_FLOWCODE_T flowCode; //!< Response
  uint32_t                flowUpdateCount;    //!< Event for each flow change at node
  uint32_t                totalFlowIdCount;   //!< Flows for which node is Ingress
  uint32_t                numReturnedFlowIds; //!< Num flows populated in table below
  MXL_MOCA_MAC_ADDR_T     returnedFlowId[MAX_FLOWS_IN_LIST];
} MXL_MOCA_SOC_FLOW_LIST_RSP_T;        /* Hi_FlowListRsp_t */

typedef struct
{
  MXL_MOCA_SOC_FLOWCODE_T flowCode;      //!< Response code, encodes W1REQ DECISION
  uint32_t                leaseTimeLeft; //!< Lease time left (seconds)
  MXL_MOCA_SOC_PFP_CMD_T  pfp;           //!< Parameters for PQoS Flow
  uint32_t                dfid;          //!< DFID, 8 bit routing number
} MXL_MOCA_SOC_FLOW_QUERY_RSP_T;       /* Hi_FlowQueryRsp_t */

// MXL_MOCA_SOC_GET_FLOW_STATS / HI_TXNTYPE_GET_FLOW_STATS

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_FLOW_STATS_CMD_T;   /* Hi_TxnGetFlowStatsCmd_t */

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
} MXL_MOCA_SOC_FLOW_STATS_RSP_T;     /* Hi_FlowStats_t */
 
///SEC
//

// MXL_MOCA_SOC_GET_SEC_PASSWORD / HI_TXNTYPE_GET_SEC_PASSWORD

typedef struct
{
  uint32_t result;
  uint32_t password[5];
} MXL_MOCA_SOC_GET_SEC_PASSWD_RSP_T;

// MXL_MOCA_SOC_GET_SEC_INFO / HI_TXNTYPE_GET_SEC_INFO

typedef struct
{
  uint32_t state;
  uint32_t mmk[2];         ///TODO 07/12/2011 API is of size 2*sizeof(uint32_t) SoC Hi is of sizeof(uint32_t)
  uint32_t pmkInit[2];     ///TODO 07/12/2011 API is of size 2*sizeof(uint32_t) SoC Hi is of sizeof(uint32_t)
  uint32_t pmk0[2];
  uint32_t pmk1[2];
  uint32_t activePmk;
  uint32_t tek0[2];
  uint32_t tek1[2];
  uint32_t activeTek;
  uint32_t ammk[4];
  uint32_t apmkInit[4];
  uint32_t apmk0[4];
  uint32_t apmk1[4];
  uint32_t activeApmk;
  uint32_t atek0[4];
  uint32_t atek1[4];
  uint32_t activeAtek;
  uint32_t permSalt[3];
} MXL_MOCA_SOC_GET_SEC_INFO_RSP_T;

// MXL_MOCA_SOC_GET_SEC_STATS / HI_TXNTYPE_GET_SEC_STATS

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_SEC_STATS_CMD_T;

typedef struct
{
  uint32_t txNoSec;
  uint32_t txPmkInit;
  uint32_t txMmk;
  uint32_t txPmk;
  uint32_t txTek;
  uint32_t txApmkInit;
  uint32_t txAmmk;
  uint32_t txApmk;
  uint32_t txAtek;
} MXL_MOCA_SOC_GET_SEC_STATS_RSP_T;

// MXL_MOCA_SOC_DO_TEST_CMD / HI_TXNTYPE_DO_TEST_CMD

typedef struct
{
  uint32_t dcapId;
  uint32_t param[64];
  uint32_t paramLen;
} MXL_MOCA_SOC_TEST_CMD_T;

typedef struct
{
  uint32_t result;
  uint32_t param[64];
  uint32_t paramLen;
} MXL_MOCA_SOC_TEST_RSP_T;

//! RLAPM configuration

typedef struct
{
  uint32_t chn_index;
} MXL_MOCA_SOC_GET_ACTIVE_RLAPM_CMD_T;

typedef struct
{
  //! RLAPM mode
  uint32_t rlapmMode;
  //! RLAPM tables
  struct
  {
    //! The frequency for the PHY margin table
    uint32_t freq;
    //! Added PHY margin table
    uint8_t
    phyMargin[MAX_RX_LEVELS_PADDED];
  } tbl;
} MXL_MOCA_SOC_GET_ACTIVE_RLAPM_RSP_T;

//! SAPM configuration

typedef struct
{
  uint32_t chn_index;
} MXL_MOCA_SOC_GET_ACTIVE_SAPM_CMD_T;

typedef struct
{
  //! SAPM mode
  uint32_t sapmMode;
  //! Added PHY margin table
  uint8_t  phyMargin[FFT_LENGTH];

  //! Threshold and frequency
  uint16_t frequency;
  uint8_t  threshold;
  uint8_t  pad;
} MXL_MOCA_SOC_GET_ACTIVE_SAPM_RSP_T;

// ! Energy detection result
typedef struct 
{
  uint32_t rawDetectedServiceBitmask;
  uint8_t  channelInfo50MhzBandE[7];
  uint8_t  channelInfo50MhzBandF[8];
  uint8_t  channelInfo50MhzBandD[21];
  uint8_t  channelInfo100MhzBandE[5];
  uint8_t  channelInfo100MhzBandF[6];
  uint8_t  channelInfo100MhzBandD[19]; 
  uint8_t  pad[2]; 
    
  uint32_t bandEF50MhzEnergyBitmask;
  uint32_t bandD50MhzEnergyBitmask;
  uint32_t bandEF100MhzEnergyBitmask;
  uint32_t bandD100MhzEnergyBitmask;
} MXL_MOCA_SOC_ENDET_STATUS_RSP_T;

// MXL_MOCA_SOC_GET_SWITCH_STATS / HI_TXNTYPE_GET_SWITCH_STATS

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_SWITCH_STATS_CMD_T;      /* Hi_TxnGetSwitchStatsCmd_t */

typedef struct
{
  uint32_t rxGood[4];
  uint32_t rxDrop[4];
  uint32_t rxError[4];
  uint32_t txGood[4];
  uint32_t txDrop[4];
  uint32_t txError[4];
} MXL_MOCA_SOC_GET_SWITCH_STATS_RSP_T;     /* Hi_TxnGetSwitchStatsRsp_t */

// MXL_MOCA_SOC_GET_SWITCH_CONFIG / HI_TXNTYPE_GET_SWITCH_CONFIG

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_GET_SWITCH_CONFIG_CMD_T;   /* Hi_TxnGetSwitchConfigCmd_t */

// MXL_MOCA_SOC_SET_SWITCH_CONFIG / HI_TXNTYPE_SET_SWITCH_CONFIG

/* eSwitch config:
 * ---------------
 * eSwitch port control eSwitchPortConfig[2:0]
 *  A value of 0 in a given bit position indicates disable. Default value is 111'b
 *  Bit0 EPHY0(RGMII) 
 *  Bit1 EPHY1(SGMII)
 *  Bit2 MoCA ifc. 
 *  Other bits are reserved 
 */
typedef struct
{  uint32_t eSwitchPortConfig; 
} MXL_MOCA_SOC_SET_SWITCH_PORT_CONFIG_CMD_T;  /* Hi_TxnSetSwitchPortConfigCmd_t */

typedef struct
{  uint32_t status; 
} MXL_MOCA_SOC_SET_SWITCH_PORT_CONFIG_RSP_T;  /* Hi_TxnSetSwitchPortConfigRsp_t */


/* == Bridge Detection == */
typedef struct
{
    uint32_t  reset;  // resets the event
} MXL_MOCA_SOC_GET_NETWORK_NAME_REC_CMD_T; /* Hi_TxnGetNetworkNameRecCmd_t */
// MXL_MOCA_SOC_GET_NETWORK_NAME_REC / HI_TXNTYPE_GET_NETWORK_NAME_REC

typedef struct
{
    uint32_t networkNameRecRsp;
} MXL_MOCA_SOC_GET_NETWORK_NAME_REC_RSP_T; /* Hi_TxnGetNetworkNameRecRsp_t */

//
typedef struct
{
    uint32_t reserved;
} MXL_MOCA_SOC_GET_NETWORK_NAME_PAYLOAD_CMD_T; /* Hi_TxnGetNetworkNamePayloadRecCmd_t */
// MXL_MOCA_SOC_GET_NETWORK_NAME_PAYLOAD / HI_TXNTYPE_GET_NETWORK_NAME_PAYLOAD_REC

typedef struct
{
    uint8_t  vendorInfo[MAX_NW_NAME_LEN+1]; 
    uint32_t networkNameRec;
} MXL_MOCA_SOC_GET_NETWORK_NAME_PAYLOAD_RSP_T; /* Hi_TxnGetNetworkNamePayloadRecRsp_t */

typedef struct
{
    uint8_t  vendorInfo[MOCA25_ME_IE_RSP_MBOX_SIZE_BYTES]; 
    uint32_t set;  // 1= set (use the vendor info from cmd); 0 = get (return in response structure)
} MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_TX_CMD_T; /* Hi_TxnSetMeIEPayloadTxCmd_t */
// MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_TX / HI_TXNTYPE_SET_ME_IE_PAYLOAD_TX

typedef struct
{
    uint8_t  vendorInfo[MOCA25_ME_IE_RSP_MBOX_SIZE_BYTES]; 
} MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_TX_RSP_T; /* Hi_TxnSetMeIEPayloadTxRsp_t */

typedef struct
{
   uint8_t  vendorInfo[MOCA25_ME_IE_REQ_MBOX_SIZE_BYTES];
   uint32_t set; // 1= set (use the vendor info from cmd); 0 = get (return in response structure)
} MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_RX_CMD_T; /* Hi_TxnGetMeIEPayloadRxCmd_t */
// MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_RX / HI_TXNTYPE_GET_ME_IE_PAYLOAD_RX

typedef struct
{
   uint8_t  vendorInfo[MOCA25_ME_IE_REQ_MBOX_SIZE_BYTES]; 
   uint32_t meIEPayloadRec;
} MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_RX_RSP_T; /* Hi_TxnGetMeIEPayloadRxRsp_t */

typedef struct /* combined with Join Cfg */
{
    uint32_t networkJoin; 
} MXL_MOCA_SOC_ME_NETWORK_JOIN_CMD_T; /* Hi_TxnSetNetworkJoinCmd_t */
// MXL_MOCA_SOC_ME_NETWORK_JOIN / HI_TXNTYPE_SET_ME_NETWORK_JOIN

typedef struct
{
  uint32_t networkJoinRsp;
} MXL_MOCA_SOC_ME_NETWORK_JOIN_RSP_T; /* Hi_TxnSetNetworkJoinRsp_t */

// Network Management Proxy
// Send pkt
typedef struct
{  
  uint8_t  pkt[MOCA_MAX_PACKET_SIZE];   
  uint32_t length;
} MXL_MOCA_SOC_NET_PROXY_PKT_SEND_REQ_CMD_T; /* Hi_TxnNetProxyPktSendCmd_t */

typedef struct
{  
  uint32_t status; 
} MXL_MOCA_SOC_NET_PROXY_PKT_RCV_RSP_T;  /* Hi_TxnNetProxyPktSendRsp_t */

// Pkt response
typedef struct
{  
  uint32_t  token;   // reserved
} MXL_MOCA_SOC_NET_PROXY_PKT_REPLY_CMD_T; /* Hi_TxnNetProxyPktReplyCmd_t */

typedef struct
{  
  uint32_t status; 
  uint8_t  pkt[MOCA_MAX_PACKET_SIZE];   
  uint32_t length;  
} MXL_MOCA_SOC_NET_PROXY_PKT_REPLY_RSP_T;  /* Hi_TxnNetProxyPktReplyRsp_t */

// dcap136
typedef struct 
{
  uint32_t enable; 
} MXL_MOCA_SOC_NET_PROXY_TEST_MODE_CTRL_CMD_T; /* Hi_TxnNetProxyTestModeCtrlCmd_t */

typedef struct 
{
  uint32_t reserved; 
} MXL_MOCA_SOC_NET_PROXY_TEST_MODE_CTRL_RSP_T; /* Hi_TxnNetProxyTestModeCtrlRsp_t */

typedef struct
{
  uint32_t macHi;
  uint32_t macLo;
  uint32_t tmo;
  uint32_t vlanCheck[2];
  uint32_t vlanStrip[2];
  uint32_t vlanPvid[2];
  uint32_t vlanId[2][SWITCH_PORT_MAX_VLAN_IDS];
} MXL_MOCA_SOC_GET_SWITCH_CONFIG_RSP_T;    /* Hi_TxnGetSwitchConfigRsp_t */

// MXL_MOCA_SOC_RESET_SWITCH_CONFIG / HI_TXNTYPE_RESET_SWITCH_CONFIG

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_RESET_SWITCH_CONFIG_CMD_T;  /* Hi_TxnResetSwitchConfigCmd_t */

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_RESET_SWITCH_CONFIG_RSP_T;  /* Hi_TxnResetSwitchConfigRsp_t */

// MXL_MOCA_SOC_SET_SWITCH_TMO / HI_TXNTYPE_SET_SWITCH_TMO

typedef struct
{
  uint32_t tmo;
} MXL_MOCA_SOC_SET_SWITCH_TMO_CMD_T;       /* Hi_TxnSetSwitchTmoCmd_t */

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_SET_SWITCH_TMO_RSP_T;       /* Hi_TxnSetSwitchTmoRsp_t */

// MXL_MOCA_SOC_ENA_SWTICH_VLAN_CHECK / HI_TXNTYPE_ENA_SWTICH_VLAN_CHECK

typedef struct
{
  int32_t port;
} MXL_MOCA_SOC_ENA_SWITCH_VLAN_CHECK_CMD_T; /* Hi_TxnEnaSwitchVlanCheckCmd_t */

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_ENA_SWITCH_VLAN_CHECK_RSP_T; /* Hi_TxnEnaSwitchVlanCheckRsp_t*/

// MXL_MOCA_SOC_DIS_SWTICH_VLAN_CHECK / HI_TXNTYPE_DIS_SWTICH_VLAN_CHECK

typedef struct
{
  int32_t port;
} MXL_MOCA_SOC_DIS_SWITCH_VLAN_CHECK_CMD_T;  /* Hi_TxnDisSwitchVlanCheckCmd_t*/

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_DIS_SWITCH_VLAN_CHECK_RSP_T;  /* Hi_TxnDisSwitchVlanCheckRsp_t*/

// MXL_MOCA_SOC_ENA_SWTICH_VLAN_STRIP / HI_TXNTYPE_ENA_SWTICH_VLAN_STRIP

typedef struct
{
  int32_t port;
} MXL_MOCA_SOC_ENA_SWITCH_VLAN_STRIP_CMD_T;  /* Hi_TxnEnaSwitchVlanStripCmd_t*/

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_ENA_SWITCH_VLAN_STRIP_RSP_T;  /* Hi_TxnEnaSwitchVlanStripRsp_t*/

// MXL_MOCA_SOC_DIS_SWTICH_VLAN_STRIP / HI_TXNTYPE_DIS_SWTICH_VLAN_STRIP

typedef struct
{
  int32_t port;
} MXL_MOCA_SOC_DIS_SWITCH_VLAN_STRIP_CMD_T;  /* Hi_TxnDisSwitchVlanStripCmd_t */

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_DIS_SWITCH_VLAN_STRIP_RSP_T;  /* Hi_TxnDisSwitchVlanStripRsp_t */

// MXL_MOCA_SOC_SET_SWTICH_VLAN_PVID / HI_TXNTYPE_SET_SWTICH_VLAN_PVID

typedef struct
{
  int32_t  port;
  uint32_t pvid;
} MXL_MOCA_SOC_SET_SWITCH_VLAN_PVID_CMD_T;   /* Hi_TxnSetSwitchVlanPvidCmd_t */

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_SET_SWITCH_VLAN_PVID_RSP_T;   /* Hi_TxnSetSwitchVlanPvidRsp_t */

// MXL_MOCA_SOC_ADD_SWTICH_VLAN_ID / HI_TXNTYPE_ADD_SWTICH_VLAN_ID

typedef struct
{
  int32_t  port;
  uint32_t vid;
} MXL_MOCA_SOC_ADD_SWITCH_VLANID_CMD_T;      /* Hi_TxnAddSwitchVlanIdCmd_t */

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_ADD_SWITCH_VLANID_RSP_T;      /* Hi_TxnAddSwitchVlanIdRsp_t */

// MXL_MOCA_SOC_DEL_SWTICH_VLAN_ID / HI_TXNTYPE_DEL_SWTICH_VLAN_ID

typedef struct
{
  int32_t  port;
  uint32_t vid;
} MXL_MOCA_SOC_DEL_SWITCH_VLANID_CMD_T;      /* Hi_TxnDelSwitchVlanIdCmd_t */

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_DEL_SWITCH_VLANID_RSP_T;      /* Hi_TxnDelSwitchVlanIdRsp_t */

// MXL_MOCA_SOC_SET_SWITCH_MGMT_MAC / HI_TXNTYPE_SET_SWTICH_MGMT_MAC

typedef struct
{
  uint32_t macHi;
  uint32_t macLo;
} MXL_MOCA_SOC_SET_SWITCH_MGMT_MAC_CMD_T;    /* Hi_TxnSetSwitchMgmtMacCmd_t */

typedef struct
{
  uint32_t reserved;
} MXL_MOCA_SOC_SET_SWITCH_MGMT_MAC_RSP_T;    /* Hi_TxnSetSwitchMgmtMacRsp_t */

// MXL_MOCA_SOC_GET_MAC_DATA_STATS / HI_TXNTYPE_GET_MAC_DATA_STATS

typedef struct
{
  uint32_t clear;
} MXL_MOCA_SOC_GET_MACDATA_STATS_CMD_T;      /* Hi_TxnGetMacDataStatsCmd_t */

typedef struct
{
  uint32_t pktsTx2Node[MAX_NUM_NODES];
} MXL_MOCA_SOC_GET_MACDATA_STATS_RSP_T;      /* Hi_TxnGetMacDataStatsRsp_t */

// MXL_MOCA_SOC_SET_INIT_PARAM_V2 / HI_TXNTYPE_SET_INIT_PARAM_V2

typedef struct
{
  uint8_t     confVersion;                //!< The config version to indicate the changes history of structure.
  uint8_t     testMode;                   //!< The test mode for the MoCA device.
  uint8_t     turboModeMask;              //!< The turbo mode for each band for the MoCA device.
  uint8_t     clnkLedMode;                //!< The CLINK LED mode (on or flash, 0 or 1)
  uint32_t    testParam1;                 //!< The test parameter 1 for the MoCA device.
  uint32_t    testParam2;                 //!< The test parameter 2 for the MoCA device.
  uint32_t    testParam3;                 //!< The test parameter 3 for the MoCA device.
  uint32_t    testParam4;                 //!< The test parameter 4 for the MoCA device.
  uint32_t    clinkGuidHi;                //!< The MoCA GUID high.
  uint32_t    clinkGuidLo;                //!< The MoCA GUID low.
  uint32_t    mpaMacAddrHi;               //!< Upper 4 bytes of MPA destination MAC Address
  uint16_t    mpaMacAddrLo;               //!< 2 bytes of MPA destination MAC Address
  uint16_t    mpaPktFilter;               //!< 2 bytes for MPA MoCA packet filter
  uint8_t     traceEnable;                //!< Trace type enable/disable.
  uint8_t     traceLevel;                 //!< Minimum trace level.
  uint8_t     traceModule;                //!< Trace module enable/disable.
  uint8_t     traceDest;                  //!< Trace destination enable/disable.
} MXL_MOCA_INIT_SYSTEM_V2_T;

typedef struct
{
  uint16_t    miiPausePriorityLevel;      //!< The MII pause priority level specifies the MoCA priority level
  uint8_t     policingModeMask;           //!< The policing mode for each band for the MoCA device.
  uint8_t     pqosClassificationMode;     //!< The PQoS classification mode specifies the classification mode
  uint32_t    miiSpeed;                   //!< The MII speed for the MoCA device.
  uint32_t    retransmissionPriority;     //!< The priorities to enable retransmission on the transmit side.
  uint32_t    transmissionPriorityPer;    //!< The PER profile to use for each priority on the transmit side.
  uint8_t     datapathMode;               //!< The data path mode for the MoCA device.
  uint8_t     mcastMode;                  //!< The multicast mode for the MoCA device.
  uint16_t    reserved2;
  uint32_t    mcastAddrHi[48];            //!< The multicast MAC address high.  There are a total of 48 MAC addresses.
  uint32_t    mcastAddrLo[48];            //!< The multicast MAC address high.  There are a total of 48 MAC addresses.
  uint32_t    rgmiiClkDelayCfg;          
  uint32_t    stripVlanTag;               //!< Bit 31 enable, LS 12 bit vlan id.
  uint32_t    macAddrAgingTimeInSecs;     //!< E-MAC Address Aging time out.
} MXL_MOCA_INIT_DATA_V2_T;

typedef struct
{
  uint8_t     failoverMode        : 1,    //!< bit 7 - The failover mode for the MoCA device.
              handoffMode         : 1,    //!< bit 6 - The handoff mode for the MoCA device.
              preferredNcMode     : 1,    //!< bit 5 - The preferred NC mode for the MoCA device.
              networkSearchMode   : 2,    //!< bit 4:3 - The network search mode for the MoCA device.
              lofUpdateMode       : 1,    //!< bit 2 - The LOF update mode for the MoCA device.
              channelBondEn       : 1,    //!< bit 1 - Control Channel Bonding when start up.
              reserved1           : 1;    // bit 0
  uint8_t     initialRole;                //!< The role is the initial role used to form or join the MoCA network
  uint8_t     lmoMode;                    //!< The LMO mode for the MoCA device.
  uint8_t     securityModeMask;           //!< The security mode for each band for the MoCA device.
  uint8_t     mocaPassword[MAX_BANDS][MAX_PASSWORD_LENGTH_PADDED];  
  uint16_t    mocaResetSeqNum[MAX_BANDS]; //!< The MoCA reset sequence number.
  uint16_t    lof;                        //!< The last operating frequency (LOF) in MHz.
  uint8_t     reserved2;
  uint8_t     bandMask;                   //!< The frequency band mask defines the frequency bands
  uint64_t    scanMask[MAX_BANDS];        //!< The scan mask defines the channels that the user has selected to scan.
  uint8_t     scanOffset[MAX_BANDS];      //!< The scan offset is the offset in 25MHz steps starting from 0 MHz for
                                          //!<   the scan mask.
  uint64_t    priChnAboveMask[MAX_BANDS]; //!< The primary channel above mask is a mask to indicate whether the center
                                          //!<   frequency of the primary channel is 25MHz above the beacon channel.
  uint64_t    priChnBelowMask[MAX_BANDS]; //!< The primary channel below mask is a mask to indicate whether the center
                                          //!<   frequency of the primary channel is 25MHz below the beacon channel.
  uint64_t    secChnAboveMask[MAX_BANDS]; //!< The secondary channel above mask is a mask to indicate whether the center
                                          //!<   frequency of the secondary channel is 125MHz above the primary channel.
  uint64_t    secChnBelowMask[MAX_BANDS]; //!< The secondary channel above mask is a mask to indicate whether the center
                                          //!<   frequency of the secondary channel is 125MHz below the primary channel.
  uint32_t    tabooMask   : 24,           //!< bits 31:8 - The taboo information defines the off-limits channels above
                                          //!<             and below the active channel which the NC will advertise
                                          //!<             to all nodes scanning to join a network.
                                          //!<             The taboo mask is a mask of the channels for the taboo info.
              tabooOffset : 8;            //!< bits 7:0  - The taboo offset is the offset from the beacon frequency for
                                          //!<             the taboo information.
  uint8_t     tlpMin[MAX_BANDS];          //!< The minimum time (in seconds) a node stays in listening phase of
                                          //!<   network search.
  uint8_t     tlpMax[MAX_BANDS];          //!< The maximum time (in seconds) a node stays in listening phase of
                                          //!<   network search.
  uint8_t     firstChannelOffset[MAX_BANDS]; //!< The offset of first channel's center frequency below the center 
                                             //!< frequency of the Primary Channel, the valid values range is 0-4.
  uint8_t     numberOfChannels[MAX_BANDS];  //!< The maximum number of 100MHz channels, the valid values range is 3-5.
} MXL_MOCA_INIT_CONTROL_V2_T;

typedef struct
{
  uint8_t     beaconTxPwr;                    //!< The beacon transmit power for the MoCA device when TPC is enabled.
  uint8_t     maxTxPwr;                       //!< The maximum transmit power (except beacons) for the MoCA device
  uint16_t    reserved1;
  uint32_t    encWdLenNper;                   //!< The encoding word length (kLDPCmax) for the NPER profile.
  uint32_t    encWdLenVlper;                  //!< The encoding work length (kLDPCmax) for the VLPER profile.
  uint32_t    featureEnFlags;                 //!< The PHY feature enable flags.
  uint8_t     phyMargin50[MAX_BANDS];         //!< The PHY margin controls the margin (in dB) used above the measured
                                              //!<   RX noise floor to accommodate variations due to return loss on
                                              //!<   the receive side for the 50MHz channel
  uint8_t     phyMarginNper[MAX_BANDS];       //!< The PHY margin controls the margin (in dB) used above the measured
                                              //!<   RX noise floor to accommodate variations due to return loss on
                                              //!<   the receive side for the 100MHz NPER channel
  uint8_t     phyMarginVlper[MAX_BANDS];      //!< The PHY margin controls the margin (in dB) used above the measured
                                              //!<   RX noise floor to accommodate variations due to return loss on
                                              //!<   the receive side for the 100MHz VLPER channel
  uint16_t    tpcMode;                        //!< The TPC mode enables/disables TPC from all other nodes to this node.
  uint16_t    tgtP2p50Rate;                   //!< The TPC target PHY rate for transmissions on the 50MHz channel from
                                              //!<   all other nodes to this node when TPC mode is enabled.
  uint16_t    tgtP2pNperRate;                 //!< The TPC target PHY rate for transmissions on the 100MHz NPER channel
                                              //!<   from all other nodes to this node when TPC mode is enabled.
  uint16_t    tgtP2pVlperRate;                //!< The TPC target PHY rate for transmissions on the 100MHz VLPER channel
                                              //!<   from all other nodes to this node when TPC mode is enabled.
  uint32_t    phyModBitmaskUcast50;           //!< The PHY modulation bitmask for the 50MHz unicast channel.
  uint32_t    phyModBitmaskUcast100;          //!< The PHY modulation bitmask for the 100MHz unicast channel.
  uint32_t    phyModBitmaskBcast50;           //!< The PHY modulation bitmask for the 50MHz broadcast channel.
  uint32_t    phyModBitmaskBcast100;          //!< The PHY modulation bitmask for the 100MHz broadcast channel.
  uint32_t    diplexerCalWord;                //!< The diplexer calibration word.
  uint32_t    hwCfgWord;                      //!< The hardware configuration word.
  uint32_t    loadingCaps;                    //!< The load capacitors configraion for the xtal.
} MXL_MOCA_INIT_PHY_V2A_T;

typedef struct
{
  MXL_MOCA_INIT_SYSTEM_V2_T   initSystemV2;   // System, was Hi_InitSystem_t initSystem;
  MXL_MOCA_INIT_DATA_V2_T     initDataV2;     // ECL,    was Hi_InitEcl_t    initEcl;
  MXL_MOCA_INIT_CONTROL_V2_T  initControlV2;  // LC,     was Hi_InitLc_t     initLc;
  MXL_MOCA_INIT_PHY_V2A_T     initPhyV2A;     // PHY,    was Pcl_HostCfg_t   initPhy;
} MXL_MOCA_SOC_SET_INIT_PARAM_CMD_T;          /* Hi_TxnSetInitParamCmd_t */

// MXL_MOCA_SOC_SET_INIT_RLAPM / HI_TXNTYPE_SET_INIT_RLAPM

typedef struct
{
  uint8_t     rlapmModeMask;                  //!< The RLAPM mode for each band for the MoCA device.
  uint8_t     reserved2;
  uint16_t    reserved3;
  uint16_t    rlapmFreq[MAX_RLAPM_FREQS];     //!< The frequency for the RLAPM tables.
  uint8_t     rlapmMargin[MAX_RLAPM_FREQS][MAX_RX_LEVELS_PADDED]; //!< The added PHY margin for the RLAPM tables.
  struct
  {
    uint16_t lowerbandstart;    // Lower band start signal
    uint16_t upperbandstart;    // Upper band start signal
    uint16_t lowerGap[MAX_ATSC_CATV_GAPS];
    uint16_t upperGap[MAX_ATSC_CATV_GAPS];
    uint16_t validChBitMask;    // Valid channel bitmask
    uint16_t reserved;
  } atscTbl[MAX_ATSC_CATV_FREQS];

  struct
  {
    uint16_t lowerbandstart;    // Lower band start signal
    uint16_t upperbandstart;    // Upper band start signal
    uint16_t lowerGap[MAX_ATSC_CATV_GAPS];
    uint16_t upperGap[MAX_ATSC_CATV_GAPS];
    uint16_t validChBitMask;    // Valid channel bitmask
    uint16_t reserved;
  } catvTbl[MAX_ATSC_CATV_FREQS];

  uint8_t atscwtHit;              // WT_HIT value
  uint8_t atscwtMiss;             // WT_MISS value
  uint8_t atscloPenalty;          // LO_PENALTY
  uint8_t atschiPenalty;          // HI_PENALTY
  uint8_t atscminCnt;             // MIN_CNT
  uint8_t atscmaxCnt;             // MAX_CNT
  uint16_t reserved4;
  uint16_t atscrxLvlMin;          // RX_LVL_MIN
  uint16_t atscrxLvlScale;        // RX_LVL_SCALE
  uint8_t atscchwt[MAX_WEIGHTS];  // Channel weight (WT0...WT3)
  uint32_t atscthreshold;
  uint8_t catvwtHit;              // WT_HIT value
  uint8_t catvwtMiss;             // WT_MISS value
  uint8_t catvloPenalty;          // LO_PENALTY
  uint8_t catvhiPenalty;          // HI_PENALTY
  uint8_t catvminCnt;             // MIN_CNT
  uint8_t catvmaxCnt;             // MAX_CNT
  uint16_t reserved5;
  uint16_t catvrxLvlMin;          // RX_LVL_MIN
  uint16_t catvrxLvlScale;        // RX_LVL_SCALE
  uint8_t catvchwt[MAX_WEIGHTS];  // Channel weight (WT0...WT3)
  uint32_t catvthreshold;
  struct
  {
    uint16_t btscsscvalue[MAX_BTS_CSS_VALUES];  // Channel measurement C1-C5
    uint16_t btscssnvalue[MAX_BTS_CSS_VALUES];  // Narrow gap measurements N1-N5
  } btsCssTbl[MAX_BTS_CSS_FREQS];

  uint16_t btsRxLvlMin;           // RX_LVL_MIN
  uint16_t btsRxLvlScale;         // RX_LVL_SCALE
  uint8_t btseChWt[MAX_WEIGHTS];  // BTS even channel weight
  uint8_t btsoChWt[MAX_WEIGHTS];  // BTS odd channel weight
  uint8_t btseWtHt;               // BTS even WT_HIT
  uint8_t btsoWtHt;               // BTS odd WT_HIT
  uint8_t btseWtMiss;             // BTS even WT_MISS
  uint8_t btsoWtMiss;             // BTS odd WT_MISS
  uint8_t btseMinCnt;             // BTS even MIN_CNT
  uint8_t btsoMinCnt;             // BTS odd MIN_CNT
  uint8_t btseMaxCnt;             // BTS even MAX_CNT
  uint8_t btsoMaxCnt;             // BTS odd MAX_CNT
  uint8_t btseLoPenalty;          // BTS even LO_PENALTY
  uint8_t btsoLoPenalty;          // BTS odd LO_PENALTY
  uint8_t btseHiPenalty;          // BTS even HI_PENALTY
  uint8_t btsoHiPenalty;          // BTS odd HI_PENALTY
  uint8_t btse1ValidChnList;      // Valid BTSe1 channel list
  uint8_t btse2ValidChnList;      // Valid BTSe2 channel list
  uint8_t btso1ValidChnList;      // Valid BTSo1 channel list
  uint8_t btso2ValidChnList;      // Valid BTSo2 channel list
  uint32_t btsThreshold;
  uint16_t cssRxLvlMin;           // RX_LVL_MIN
  uint16_t cssRxLvlScale;         // RX_LVL_SCALE
  uint8_t acssChWt[MAX_WEIGHTS];  // aCss channel weight
  uint8_t dcssChWt[MAX_WEIGHTS];  // dCss channel weight
  uint8_t acssWtHt;               // Analog CSS WT_HIT
  uint8_t dcssWtHt;               // Digital CSS WT_HIT
  uint8_t acssWtMiss;             // Analog CSS WT_MISS
  uint8_t dcssWtMiss;             // Digital CSS WT_MISS
  uint8_t acssMinCnt;             // Analog CSS MIN_CNT
  uint8_t dcssMinCnt;             // Digital CSS MIN_CNT
  uint8_t acssMaxCnt;             // Analog CSS MAX_CNT
  uint8_t dcssMaxCnt;             // Digital CSS MAX_CNT
  uint8_t acssLoPenalty;          // Analog CSS LO_PENALTY
  uint8_t dcssLoPenalty;          // Digital CSS LO_PENALTY
  uint8_t acssHiPenalty;          // Analog CSS HI_PENALTY
  uint8_t dcssHiPenalty;          // Digital CSS HI_PENALTY
  uint8_t acssValidChnList;       // Valid analog CSS channel list
  uint8_t dcss1ValidChnList;      // Valid digital CSS1 channel list
  uint8_t dcss2ValidChnList;      // Valid digital CSS2 channel list
  uint8_t reserved6;
  uint32_t cssThreshold;
  uint32_t bandEUsabilityThreshold;
  uint32_t bandFUsabilityThreshold;
  uint32_t bandDUsabilityThreshold;
  uint32_t bandEEnergyThreshold;
  uint32_t bandFEnergyThreshold;
  uint32_t bandDEnergyThreshold;
} MXL_MOCA_SOC_SET_INIT_RLAPM_CMD_T;   /* Hi_TxnSetInitRlapmCmd_t */

// MXL_MOCA_SOC_SET_INIT_SAPM / HI_TXNTYPE_SET_INIT_SAPM

typedef struct
{
  uint8_t     sapmModeMask;                   //!< The SAPM mode for each band for the MoCA device.
  uint8_t     reserved4;
  uint16_t    reserved5;
  uint16_t    sapmFreq[MAX_SAPM_FREQS];       //!< The frequency for the SAPM tables.
  uint8_t     sapmThreshold[MAX_SAPM_FREQS];  //!< The threshold for the SAPM tables.
  int16_t     sapmSubcarrierStart[MAX_SAPM_FREQS];                    //!< The starting subcarrier for the SAPM tables.
  uint8_t     miniSapmMargin[MAX_SAPM_FREQS][MAX_MINI_SAPM_MARGIN_LEN];   //!< The added PHY margin for the SAPM tables.
  int8_t      rssiFreqCompTbl[MAX_RSSI_LEN];  //!< The RSSI frequency compensation table.
} MXL_MOCA_SOC_SET_INIT_SAPM_CMD_T;    /* Hi_TxnSetInitSapmCmd_t */

// MXL_MOCA_SOC_SET_INIT_PARAM_V25 / HI_TXNTYPE_SET_INIT_PARAM_V25 

typedef struct
{
  /* Beacon Power */
  uint8_t     bcnPwrConfigEn;                 //!< ENABLED/ DISABLED
  int8_t      bcnPwrDist;                     //!< Absolute Beacon Power for Distribution -10dbm to +7dbm
  int8_t      bcnPwrLocal;                    //!< The beacon transmit power for the c.LINK device when TPC is enabled.
  uint8_t     hoToLowerMoCAVer;               //!< The configurtion for hand off to lower MoCA Version

  /* MPS */
  uint16_t    mpsPrivacyDown; 
  uint16_t    mpsPrivacyReceive;
  uint32_t    mpsUnPairedTime;
  uint32_t    mpsWalkTime;

  /* Enhanced Privacy */
  uint8_t     privacySupported;               //!< Lower 3 bits used for Privacy Supported  
  uint8_t     pad1[3];     
  uint8_t     mocaEPPassword[MAX_MOCA_EP_PASSWORD_LEN_PADDED]; //!< 12 to 64 ASCII printable characters 

  /* Bridge Detection */
  uint8_t     networkName[MAX_NW_NAME_LEN];   //!<  Network name updated from Host
  uint32_t    networkNameAdmitCfg;            //!<  Network Name Admit cfg
  uint8_t     meNetworkIePayloadTx[MAX_ME_NW_IE_PAYLOAD_LEN];
  uint32_t    meNetworkIeRespTimeout;
  uint32_t    meNetworkJoinEn;
  //! MAC Link Authentication params
  struct
  {
    uint16_t  reserved;
    uint16_t  etherType;                      //!< Ethernet Type
    uint16_t  bitmask[MAX_NUM_NODES];         //!< unicast traffics permission bitmasks
  } defaultAuthLink;
} MXL_MOCA_SOC_SET_INIT_PARAM_V25_CMD_T;     

typedef struct
{
  //! The frequency for the PHY margin table
  uint32_t freq;

  //! Added PHY margin table
  uint8_t phymargin[MAX_RX_LEVELS_PADDED];
} MXL_MOCA_RLAPM_TABLE_V2_T; // was ClnkLib_RlapmTableV2_t;

typedef struct
{
  //! RLAPM mode
  uint32_t rlapmmode;

  //! RLAPM tables
  MXL_MOCA_RLAPM_TABLE_V2_T tbl;
} MXL_MOCA_RLAPM_ACTIVE_CFG_V2_T; // ClnkLib_RlapmActiveCfgV2_t;

typedef struct
{
  /**
   * The exclusive set of multicast MAC address allowed to pass
   * from MoCA to host when filter mode is set to Enabled. When
   * filter mode is set to DfidOnly, then the following list of
   * MAC addresses are only filtered by MAP DFID and not by
   * precise MAC Address.  This is only useful for internal
   * testing.
   *
   * The first array entry with a 0 in macAddrHi indicates that it
   * and all following array entries do not contain a valid entry.
   */
  uint32_t   macAddrHi[48];
  uint32_t   macAddrLo[48];
  /**
   * The filter mode enables or disables multicast filtering
   * for the MoCA device.
   *
   * The values for filter mode are: Disabled (0), Enabled (1),
   *  and DfidOnly (2).
   */
  int32_t mode;///defval is 0;
} MXL_MOCA_MCAST_CFG_T; 

// CATV and ATSC
typedef struct
{
  uint8_t wtHit;              // WT_HIT value
  uint8_t wtMiss;             // WT_MISS value
  uint8_t loPenalty;          // LO_PENALTY
  uint8_t hiPenalty;          // HI_PENALTY
  uint8_t minCnt;             // MIN_CNT
  uint8_t maxCnt;             // MAX_CNT
  uint16_t reserved;
  uint16_t rxLvlMin;          // RX_LVL_MIN
  uint16_t rxLvlScale;        // RX_LVL_SCALE
  uint8_t chwt[MAX_WEIGHTS];  // Channel weight (WT0...WT3)
  uint32_t threshold;
} MXL_MOCA_ATSC_CATV_WEIGHT_T; // AtscCatvWeight_t;

// Summing over 25 subcarriers for Ui and Li, each one separate by 31. i.e. U2 = U1+31
// ATSC and CATV table
// E2, EE3, F3, F7
typedef struct
{
  uint16_t lowerbandstart;    // Lower band start signal
  uint16_t upperbandstart;    // Upper band start signal
  uint16_t lowerGap[MAX_ATSC_CATV_GAPS];
  uint16_t upperGap[MAX_ATSC_CATV_GAPS];
  uint16_t validChBitMask;    // Valid channel bitmask
  uint16_t reserved;
} MXL_MOCA_BAND_E_TABLE_T; // BandEFTbl_t;

// D2, D4, D6, D8, D10, 1100
typedef struct
{
  uint16_t btscsscvalue[MAX_BTS_CSS_VALUES];  // Channel measurement C1-C5
  uint16_t btscssnvalue[MAX_BTS_CSS_VALUES];  // Narrow gap measurements N1-N5
} MXL_MOCA_BAND_D_TABLE_T; // BandDTbl_t;

// BTS constants
typedef struct
{
  uint16_t btsRxLvlMin;           // RX_LVL_MIN
  uint16_t btsRxLvlScale;         // RX_LVL_SCALE
  uint8_t btseChWt[MAX_WEIGHTS];  // BTS even channel weight
  uint8_t btsoChWt[MAX_WEIGHTS];  // BTS odd channel weight
  uint8_t btseWtHt;               // BTS even WT_HIT
  uint8_t btsoWtHt;               // BTS odd WT_HIT
  uint8_t btseWtMiss;             // BTS even WT_MISS
  uint8_t btsoWtMiss;             // BTS odd WT_MISS
  uint8_t btseMinCnt;             // BTS even MIN_CNT
  uint8_t btsoMinCnt;             // BTS odd MIN_CNT
  uint8_t btseMaxCnt;             // BTS even MAX_CNT
  uint8_t btsoMaxCnt;             // BTS odd MAX_CNT
  uint8_t btseLoPenalty;          // BTS even LO_PENALTY
  uint8_t btsoLoPenalty;          // BTS odd LO_PENALTY
  uint8_t btseHiPenalty;          // BTS even HI_PENALTY
  uint8_t btsoHiPenalty;          // BTS odd HI_PENALTY
  uint8_t btse1ValidChnList;      // Valid BTSe1 channel list
  uint8_t btse2ValidChnList;      // Valid BTSe2 channel list
  uint8_t btso1ValidChnList;      // Valid BTSo1 channel list
  uint8_t btso2ValidChnList;      // Valid BTSo2 channel list
  uint32_t btsThreshold;
} MXL_MOCA_BTS_T; // Bts_t;

// CSS constants
typedef struct
{
  uint16_t cssRxLvlMin;           // RX_LVL_MIN
  uint16_t cssRxLvlScale;         // RX_LVL_SCALE
  uint8_t acssChWt[MAX_WEIGHTS];  // aCss channel weight
  uint8_t dcssChWt[MAX_WEIGHTS];  // dCss channel weight
  uint8_t acssWtHt;               // Analog CSS WT_HIT
  uint8_t dcssWtHt;               // Digital CSS WT_HIT
  uint8_t acssWtMiss;             // Analog CSS WT_MISS
  uint8_t dcssWtMiss;             // Digital CSS WT_MISS
  uint8_t acssMinCnt;             // Analog CSS MIN_CNT
  uint8_t dcssMinCnt;             // Digital CSS MIN_CNT
  uint8_t acssMaxCnt;             // Analog CSS MAX_CNT
  uint8_t dcssMaxCnt;             // Digital CSS MAX_CNT
  uint8_t acssLoPenalty;          // Analog CSS LO_PENALTY
  uint8_t dcssLoPenalty;          // Digital CSS LO_PENALTY
  uint8_t acssHiPenalty;          // Analog CSS HI_PENALTY
  uint8_t dcssHiPenalty;          // Digital CSS HI_PENALTY
  uint8_t acssValidChnList;       // Valid analog CSS channel list
  uint8_t dcss1ValidChnList;      // Valid digital CSS1 channel list
  uint8_t dcss2ValidChnList;      // Valid digital CSS2 channel list
  uint8_t reserved;
  uint32_t cssThreshold;
} MXL_MOCA_CSS_T; // Css_t;

// Energy and usability related thresholds
typedef struct
{
  uint32_t bandEUsabilityThreshold;
  uint32_t bandFUsabilityThreshold;
  uint32_t bandDUsabilityThreshold;
  uint32_t bandEEnergyThreshold;
  uint32_t bandFEnergyThreshold;
  uint32_t bandDEnergyThreshold;
} MXL_MOCA_BAND_DEF_THRESHOLDS_T; //.BandDEFThresholds_t;

// Energy detection configuration parameters
typedef struct
{
  MXL_MOCA_BAND_E_TABLE_T atscTbl[MAX_ATSC_CATV_FREQS];   // ATSC table
  MXL_MOCA_BAND_E_TABLE_T catvTbl[MAX_ATSC_CATV_FREQS];   // CATV table
  MXL_MOCA_ATSC_CATV_WEIGHT_T atscWeight;                 // ATSC weight constants
  MXL_MOCA_ATSC_CATV_WEIGHT_T catvWeight;                 // CATV weight constants
  MXL_MOCA_BAND_D_TABLE_T btsCssTbl[MAX_BTS_CSS_FREQS];   // BTS/CSS table
  MXL_MOCA_BTS_T bts;                                     // BTS tuning constants
  MXL_MOCA_CSS_T css;                                     // CSS tuning constants
  MXL_MOCA_BAND_DEF_THRESHOLDS_T bandDEFThresholds;       // Energy and usability related thresholds
} MXL_MOCA_ENERGY_DETECT_CFG_T; 

//! RLAPM configuration
typedef struct
{
  //! RLAPM mode
  //     Bit 0-Enabled(1) or disabled(0) for Extended Band D
  //     Bit 1-Enabled(1) or disabled(0) for Sub-Band D-Low
  //     Bit 2-Enabled(1) or disabled(0) for Sub-Band D-High
  //     Bit 3-Enabled(1) or disabled(0) for Band E
  //     Bit 4-Enabled(1) or disabled(0) for Band F Satellite
  //     Bit 5-Enabled(1) or disabled(0) for Band F Cable
  //     Bit 6-Enabled(1) or disabled(0) for Band H
  //     Bit 7-Enabled(1) or disabled(0) for Custom Band
  uint32_t rlapmmode;

  //! RLAPM tables
  MXL_MOCA_RLAPM_TABLE_V2_T tbl[MAX_RLAPM_FREQS];
} MXL_MOCA_RLAPM_CFG_V2_T;

typedef struct
{
  uint8_t  modNum[40];
  uint32_t diplexerCal;
  uint32_t rgmiiClkCfg;
  uint32_t hwCfg;
} MXL_MOCA_PLATFORM_CFG_T;

typedef struct
{
  //! The frequency for the PHY margin table
  uint32_t freq;
  //! The threshold
  uint32_t threshold;
  //! The starting subcarrier index
  int32_t scstart;
  //! Added PHY margin table
  uint8_t phymargin[MAX_MINI_SAPM_MARGIN_LEN];
} MXL_MOCA_SAPM_TABLE_V2_T;

//! SAPM configuration
typedef struct
{
  //! SAPM mode
  //     Bit 0-Enabled(1) or disabled(0) for Extended Band D
  //     Bit 1-Enabled(1) or disabled(0) for Sub-Band D-Low
  //     Bit 2-Enabled(1) or disabled(0) for Sub-Band D-High
  //     Bit 3-Enabled(1) or disabled(0) for Band E
  //     Bit 4-Enabled(1) or disabled(0) for Band F Satellite
  //     Bit 5-Enabled(1) or disabled(0) for Band F Cable
  //     Bit 6-Enabled(1) or disabled(0) for Band H
  //     Bit 7-Enabled(1) or disabled(0) for Custom Band
  uint32_t sapmmode;
  //! SAPM tables
  MXL_MOCA_SAPM_TABLE_V2_T tbl[MAX_SAPM_FREQS];
} MXL_MOCA_SAPM_CFG_V2_T;

typedef struct
{
  uint8_t freqCompTbl[MAX_FREQ_COMP_TBL_LEN];
  uint8_t startFreqIndex;
  uint8_t channelSpacing;
  int8_t logGainOffset;   // signed
} MXL_MOCA_RSSI_CFG_V2_T; 

// MoCA Configuration Parameters
typedef struct
{
  /** The conf version is the Serial number to indicate the changes history of data strucutre.
      The default value for version is 1.
  */
  uint8_t     confVersion;
  /** The test mode for the MoCA device.  The values for test mode are as follows:
      normal operation (0),
      continuous transmit mode (1),
      continuous wave mode (2),
      no SoC download mode (3),
      no health monitoring mode (4)
      or test harness mode (5).
  */
  uint8_t     testMode;
  /** The turbo mode for the MoCA device.
      The values for turbo mode are as follows: disabled (0) or enabled (1).
  */
  uint8_t     turboMode;
  /** The CLINK LED mode for the MoCA device:
      bit 0:
      CLINK LED constant glow with data traffic (0)
      CLINK LED blinks with data traffic (1)
      bit 7:
      Turn off all Leds control bit, when set, it will drive GPIO in gpioAllLedsOffMask */
  uint8_t     clnkLedMode;
  /** The test parameter 1 for the MoCA device.  This parameter is used for internal purposes only.  */
  uint32_t    testParam1;
  /** The test parameter 2 for the MoCA device.  This parameter is used for internal purposes only.  */
  uint32_t    testParam2;
  /** The test parameter 3 for the MoCA device.  This parameter is used for internal purposes only.  */
  uint32_t    testParam3;
  /** The test parameter 4 for the MoCA device.  This parameter is used for internal purposes only.  */
  uint32_t    testParam4;

  /** The MII pause priority level specifies the MoCA priority level
      where MII pause packets are generated for transmit packets of that priority or above.
      This field is valid only when using xMII mode for the data path.
      The values for MII pause priority level are:
      dynamic (-1) or MoCA priority (0-4).
  */
  int16_t     miiPausePriorityLevel;
  /** The MII speed for the MoCA device.  The values for MII speed are as follows:
      MII             (1),
      TMII            (2),
      GMII            (3)
      RGMII two ports (4).
      RGMII one port  (5).
      Strap option    (0x80).
  */
  uint8_t     policingMode;
  /** The PQoS classification mode specifies the classification mode to
      use for PQoS flows on the transmit side.  The values for PQoS classification mode are:
      MoCA 1.1 classification (0) or
      Pre-UpnP PQoS classification (1).
  */
  uint8_t     pqosClassificationMode;
  /** The priorities to enable retransmission on the transmit side.
      The bits for retransmission priorities are defined as follows:
      Bit 3: High priority enable
      Bit 2: Medium priority enable
      Bit 1: Low priority enable
      Bit 0: Background priority enable
  */
  uint32_t    miiSpeed;
  /** The policing mode for the MoCA device.  This field is used only when the node is NC.
      The values for policing mode are as follows:
      disabled (0) or
      enabled (1).
  */
  uint32_t    retransmissionPriority;
  /** The PER profile to use for each priority on the transmit side.
      The bits for PER priority are defined as follows:
      Bit 3 -- High priority (NPER (0) or VLPER (1))
      Bit 2 -- Medium priority (NPER (0) or VLPER (1))
      Bit 1 -- Low priority (NPER (0) or VLPER (1))
      Bit 0 -- Background priority (NPER (0) or VLPER (1))
  */
  uint32_t    transmissionPriorityPer;    //!< The PER profile to use for each priority on the transmit side.

  uint8_t  reserverd: 2,
          /** The LOF update mode for the MoCA device.  The values for LOF update mode are as follows:
              disabled (0) or
              enabled (1).
          */
          lofUpdateMode: 1,
          /** The network search mode for the MoCA device.
              The values for network search mode are as follows:
              disabled (0) or
              enabled (1).
          */
          networkSearchMode : 2,
          /** The preferred NC mode for the MoCA device.
              The values for preferred NC mode are as follows:
              disabled (0),
              enabled (1) or
              reserved (2).
          */
          preferredNcMode: 1,
          /**The handoff mode for the MoCA device.  This field is used only when the node is NC.
              The values for handoff mode are as follows:
              disabled (0) or enabled (1).
          */
          handoffMode: 1,
          /** The failover mode for the MoCA device.
              This field is used only when the node is NC.
              The values for failover mode are as follows:
              disabled (0) or enabled (1)
          */
          failoverMode: 1;
    /** The role is the initial role used to form or join the MoCA network for the MoCA device.
        The values for role are as follows:
        normal (0),
        NC only (1) or
        client only (2).
    */
  uint8_t initialRole;
  /** The LMO mode for the MoCA device.  This field is used only when the node is NC.
      The values for LMO mode are as follows:
      disabled (0) or
      enabled (1).
  */
  uint8_t lmoMode;
  /** The security mode for the MoCA device.  The values for security mode are as follows:
      disabled (0) or
      enabled (1).
  */
  uint8_t securityMode;
  /** The MoCA password is a user definable password for the MoCA network from 12 to 17 bytes.  */
  uint8_t mocaPassword[MAX_BANDS][18];
  /** The MoCA reset sequence number. */
  uint16_t    mocaResetSeqNum16[MAX_BANDS];
  /** The last operating frequency (LOF) in MHz. */
  uint16_t lof16;
  
  /* If customBandProfcGapMode set to 0 means, then there is no 25MHz gap between 
  Prof C Bonded channels. Default must be 1. */
  uint8_t  customBandProfcGapMode;
  /** The frequency band mask defines the frequency bands that the user has selected to scan.
      The bits for frequency band mask are defined as follows:
      Bit 0 -  Extended Band D
      Bit 1 -  Sub-Band D-Low
      Bit 2 -  Sub-Band D-High
      Bit 3 -  Band E
      Bit 4 -  Band F Satellite
      Bit 5 -  Band F Cable
      Bit 6 -  Band H
      Bit 7 -  Custom Band
  */
  uint8_t  freqBandMask8;
  /** The scan mask defines the channels that the user has selected to scan.
      The channel represents the center frequency of the beacon.  The bits for scan mask are defined as follows:
      Bit 63 - Beacon freq scan offset x 25 MHz
      Bit 62 - Beacon freq (scan offset+1) x 25 MHz
      --
      Bit 0  - Beacon freq (scan offset+63) x 25MHz
  */
  uint64_t    scanMask64[MAX_BANDS];
  /** The scan offset is the offset in 25MHz steps starting from 0 MHz for the scan mask.
      The starting frequency for the scan can be calculated as follows: scan offset x 25 MHz.
  */
  uint8_t     scanOffset8[MAX_BANDS];
  /** The primary channel offset is a mask to indicate whether the center frequency of
      the primary channel is 25 MHz above the beacon channel.  This field is used only when
      the node is NC.  The bits for primary channel offset are defined as follows:
      Bit 63 - Same (0), above (1) for beacon freq scan offset x 25 MHz
      Bit 62 - Same (0), above (1) for beacon freq (scan offset+1) x 25 MHz
      --
      Bit 0  - Same (0), above (1) for beacon freq (scan offset+63) x 25MHz
  */
  uint64_t   primChnMaskAbove64[MAX_BANDS];
  /** The primary channel offset is a mask to indicate whether the center frequency of
      the primary channel is 25 MHz below the beacon channel.  This field is used only when
      the node is NC.  The bits for primary channel offset are defined as follows:
      Bit 63 - Same (0), below (1) for beacon freq scan offset x 25 MHz
      Bit 62 - Same (0), below (1) for beacon freq (scan offset+1) x 25 MHz
      --
      Bit 0  - Same (0), below (1) for beacon freq (scan offset+63) x 25MHz
  */
  uint64_t    primChnMaskBelow64[MAX_BANDS];
  /** The secondary channel offset is a mask to indicate whether the center frequency of the
      secondary channel is 125 MHz above the primary channel.  This field is used only when the node is NC.
      The bits for secondary channel offset are defined as follows:
      Bit 63 - None (0), above (1) for primary freq scan offset x 25 MHz
      Bit 62 - None (0), above (1) for primary freq (scan offset+1) x 25 MHz
      --
      Bit 0  - None (0), above (1) for primary freq (scan offset+63) x 25MHz
  */
  uint64_t    secChnMaskAbove64[MAX_BANDS];
  /** The secondary channel offset is a mask to indicate whether the center frequency of
      the secondary channel is 125 MHz below the primary channel.
      This field is used only when the node is NC.
      The bits for secondary channel offset are defined as follows:
      Bit 63 - None (0), below (1) for primary freq scan offset x 25 MHz
      Bit 62 - None (0), below (1) for primary freq (scan offset+1) x 25 MHz
      --
      Bit 0  - None (0), below (1) for primary freq (scan offset+63) x 25MHz
  */
  uint64_t    secChnMaskBelow64[MAX_BANDS];
  /** The taboo offset is the offset from the beacon frequency for the taboo information. */
  uint32_t    tabooOffset:8,
           /** The taboo information defines the off-limits channels above and below the active channel
               which the NC will advertise to all nodes scanning to join a network.  The taboo mask is a mask of
               the channels for the taboo information.
               The bits for taboo mask are defined as follows:
               Bit 31: Beacon frequency - taboo offset x 25MHz
               Bit 30: Beacon frequency - (taboo offset + 1) x 25MHz
               --
               Bit 8: Beacon frequency - (taboo offset +23) x 25MHz
           */
           tabooMask  : 24;
  /** The minimum time (in seconds) a node stays in listening phase of network search.  */
  uint8_t     tlpMin8[MAX_BANDS];
  /** The maximum time (in seconds) a node stays in listening phase of network search.  */
  uint8_t     tlpMax8[MAX_BANDS];
  /**  Power level to use when transmiting beacons*/
  uint8_t     beaconTxPwr;
  /**  Max Tx power for all packets except beacons*/
  uint8_t     maxTxPwr;
  /**  Encoding word length (kLDPCmax) for NPER profiles*/
  uint32_t    encWdLenNper;
  /**  Encoding word length (kLDPCmax) for VLPER profiles*/
  uint32_t    encWdLenVlper;
  /**  dynamic PCL feature enable flags*/
  uint32_t    featureEnFlags;
  /**  PHY Bitloading margin adj for 50 MHz in 0.5dB steps*/
  uint8_t     phyMargin50[MAX_BANDS];
  /**  PHY Bitloading margin adj for 100 MHz NPER in 0.5dB steps*/
  uint8_t     phyMarginNper[MAX_BANDS];
  /**  PHY Bitloading margin adj for 100 MHz VLPER in 0.5dB steps*/
  uint8_t     phyMarginVlper[MAX_BANDS];
  /**  The TPC mode enables/disables TPC from all other nodes to this node.
      The values for TPC mode are as follows:
      disabled (0),
      enabled (1) or
      reserved (2).
  */
  uint16_t    tpcMode;
  /** The TPC target PHY rate for transmissions on the 50MHz channel from all other nodes to this node
      when TPC mode is enabled.  The value for TPC target PHY rate is in Mbps.
      When operating in MoCA 2.0 mode, the default value is 600 Mbps.
      When operating in MoCA 1.1 mode, the default value is 235 Mbps.
  */
  uint16_t    tgtP2p50Rate;

  /** The TPC target PHY rate for transmissions on the 100MHz NPER channel from all other nodes to this node
      when TPC mode is enabled.  The value for TPC target PHY rate is in Mbps.
      When operating in MoCA 2.0 mode, the default value is 600 Mbps.
      When operating in MoCA 1.1 mode, the default value is 235 Mbps.
  */
  uint16_t    tgtP2pNperRate;
  /** The TPC target PHY rate for transmissions on the 100MHz VLPER channel from all other nodes to this node
      when TPC mode is enabled.  The value for TPC target PHY rate is in Mbps.
      When operating in MoCA 2.0 mode, the default value is 600 Mbps.
      When operating in MoCA 1.1 mode, the default value is 235 Mbps.
  */
  uint16_t    tgtP2pVlperRate;
  /** The vlan tag to be removed. Bit 31 is used to enable the strip of vlan tag. The LS 12 bits indicate the vlan id to be stripped.
      If bit 31 is 1, packets with vlan ID equals to the LS 12 bits of this parameter will have the vlan tag be stripped.
  */
  uint32_t    stripVlanTag;
  /** Upper 4 bytes of MPA destination MAC Address */
  uint32_t    mpaMacAddrHi;
  /** 2 bytes of MPA destination MAC Address */
  uint16_t    mpaMacAddrLo;
  /** 2 bytes for MPA MoCA packet filter */
  uint16_t    mpaPktFilter;
  /** The offset of first channel's center frequency below the center frequency of the Primary Channel, the valid values range is 0-4.
      The first channel's center frequency can be calculated as follows:
      primary channel frequency - frist channel offset * 100 MHz;
  */
  uint8_t     firstChannelOffset[MAX_BANDS];
  /** The maximum number of 100MHz channels, the valid values range is 3-5. */
  uint8_t     numberOfChannels[MAX_BANDS];
  /** The load capacitors configraion for the xtal. */
  uint32_t    loadingCaps;   
  /** E-MAC Address Aging time out. If there is no activity in ETH traffic for this period, FW will remove this entry from DIC-TC CAM. */
  uint32_t    macAddrAgingTimeInSecs;       
  /** Enable or disable beacon power mode. */
  uint8_t     bcnPwrConfigEn;
  /** Absolute Beacon Power for Distribution -10dbm to +7dbm. */
  int8_t      bcnPwrDist;
  /** Absolute Beacon Power for Local -10dbm to +7dbm. */
  int8_t      bcnPwrLocal;
  /** Enable/disable handoff to lower MoCA version. */
  uint8_t     handoffToLowerMoCAVer;    
  /** Privacy support mode for moca version**/
  int8_t      privacySupported;  
  /** Enhanced password for 12 to 64 ASCII printable characters **/
  uint8_t     mocaEPPassword[MAX_MOCA25_ENHANCED_PASSWORD_LEN];         
  /** MPS privacy downgrade, enable or diable **/
  uint16_t    mpsPrivacyDown; 
  /** MPS privacy receive, enable or diable **/
  uint16_t    mpsPrivacyReceive;
  /** MPS unpaired time, range is 120 ~ 7200 **/
  uint32_t    mpsUnPairedTime;
  /** MPS unpaired time, range is 12 ~ 1200 **/
  uint32_t    mpsWalkTime;
  /* Bridge Detection */
  uint8_t     networkName[MAX_NW_NAME_LEN];  //!<  Network name updated from Host
  uint32_t    networkNameAdmitCfg;           //!<  Network Name Admit cfg
  // meNetworkIePayloadTx: The payload of the Management Entity Request Network IE (when the Node is trying to join the network), 
  // or of the Management Entity Response Network IE (when the Node is the NC).
  uint8_t     meNetworkIePayloadTx[MAX_ME_NW_IE_PAYLOAD_LEN]; 
  // meNetworkIeRespTimeout: Indicates the number of Discovery Request transmission opportunities that a NN has to wait for a response.
  uint32_t    meNetworkIeRespTimeout; //!< default of 10 seconds = 1000. Specified in 10msec step
  uint32_t    meNetworkJoinEn;  //!< Configures as enable/disable whether managed parameter NETWORKJOIN is yes or no in OSP mode if the Information Fields match.
  /* MAC Link Authentication params */
  struct
  {
    uint16_t reserved;
    uint16_t etherType;                      //!< Ethernet Type
    uint16_t bitmask[MAX_NUM_NODES];         //!< unicast traffics permission bitmasks
  } defaultAuthLink;
  /** Trace type enable/disable. */
  uint8_t     traceEnable;
  /** Minimum trace level. */
  uint8_t     traceLevel;
  /** Trace module enable/disable. */
  uint8_t     traceModule;
  /** Trace destination enable/disable. */
  uint8_t     traceDest;
  /** Trace host control (common/common2 queue display in host driver enable/disable etc)
   * Bit0: common queue on/off, Bit1: common2 queue on/off. 
   * Bit8: background clnktrace app on/off flag (only for MXL ECA usage) 
   * Bit16: check and print soc config parameters on/off flag
   */
  uint32_t    traceHostCtl;
} MXL_MOCA_CFG_V2_T; 

// New MoCA2.5 paramters
// Beacon Power
typedef enum
{
  MXL_BCN_PWR_TYPE_LOCAL = 0,
  MXL_BCN_PWR_TYPE_DIST=1
} MXL_BCN_PWR_TYPE_T;

typedef struct
{
  int32_t  bcnPwr;                  // Absolute Beacon Power, Range:-10 to +7 (dbm)
  MXL_BCN_PWR_TYPE_T bcnPwrType;    // BCN_PWR_TYPE_LOCAL(=0) or BCN_PWR_TYPE_DIST(=1)
} MXL_MOCA_SOC_SET_BCN_POWER_CMD_T;

typedef struct
{
  uint32_t   result;                // 0 - SUCCESS 1 - FAILURE
} MXL_MOCA_SOC_SET_BCN_POWER_RSP_T;

typedef struct
{
  int32_t  bcnPwrDist;              // Distribute Beacon Power, Range:-10 to +7 (dbm)
  int32_t  bcnPwrLocal;             // Local Beacon Power, Range:-10 to +7 (dbm)
} MXL_MOCA_SOC_GET_BCN_POWER_RSP_T;

typedef struct
{
  int32_t   bcnPwrPieTxPwr;         // Absolute Bcn tx Power(-10 to +7)
  uint32_t  bcnPwrPieTxPwrType;     //  BCN_PWR_PIE_TYPE_INDIVIDUAL(=0) or BCN_PWR_TYPE_NETWORKWIDE(=1)
  uint32_t  bcnPwrPieNodeMask;      // Nodemask of the nodes to which Host wants to send the Bcn Power PIE (0x0000  0xFFFF)
} MXL_MOCA_SOC_REQUEST_BCNPWR_PIE_CMD_T;

typedef struct
{
  uint32_t   result;                // 0 - SUCCESS 1 - FAILURE
} MXL_MOCA_SOC_REQUEST_BCNPWR_PIE_RSP_T;

// MPS
typedef struct
{
  /* SUCCESS = 0, FAILURE = 1 */
  uint32_t   mpsTriggerResult;
} MXL_MOCA_SOC_SET_MPS_TRIGGER_RSP_T;

typedef enum
{
  MPS_REPORT_TYPE_SCAN_PAYLOAD         = 0,
  MPS_REPORT_TYPE_NETWORK_NAME_PAYLOAD = 1,
  MPS_REPORT_TYPE_MPS_PAIR_STATUS      = 2,
  MPS_REPORT_TYPE_MPS_PRIVACY_CHANGE   = 3,
  MPS_REPORT_TYPE_MPS_IN_PROGRESS      = 4,
  MPS_REPORT_TYPE_MPS_SESSION_TIMEOUT  = 5,
  MPS_REPORT_TYPE_MPS_SSN_DONE_CONTINUE_NORMAL_ADM = 6,
  MPS_REPORT_TYPE_MPS_TRIGGER_FWD_NC   = 7,
  MPS_REPORT_TYPE_MPS_FAILED = 8,
  MPS_REPORT_TYPE_MAX
} MPS_REPORT_TYPE_T;


typedef  struct
{
  /* CODE = 0 means no MPS session triggered */
  uint8_t nc_moca_version;
  /* Channel Num on which the MPS network found */
  uint8_t chn_num;                                         
  /* CODE = 0 means no MPS session triggered */
  uint8_t code; 
  /* MPS Response from NC - As per spec Section 6.3.2.2.7. Meaning of LSB bits
     Bit3:    Reserved,
     Bit2:    NC Privacy Enabled/Disabled,
     Bit1:    NC Recive Privacy
     Bit0:    NC downgrade Privacy
  */
  uint8_t mps_params; 
} MPS_INIT_SCAN_PAYLOAD_T;

typedef struct
{
  /* My Node Privacy Support.0x03 (M20P, M1P), 0x04 (EP), 0x6 (EP, M20P), 0x07(EP, M20P, M1P) */
  uint8_t  privacySupport;
  /* 1 means Privacy is Enabled, 0 means Disabled */
  uint8_t  privacyEn;
  uint16_t pad; 
  /* 20 Random Decimal Digits */
  uint8_t  Pswd[MAX_PASSWORD_LENGTH_PADDED];        
  /* 64Random ASCII characters */
  uint8_t  Epswd[MAX_MOCA_EP_PASSWORD_LEN_PADDED];        
} MPS_PRIVACY_PARAMS_T;

typedef struct
{
  uint32_t mpsState;
}MXL_MOCA_SOC_GET_MPS_STATE_RSP_T;

typedef struct
{
  /* MPS report types are valid in this report.
     Refer MPS_REPORT_TYPE_T for bit positions of the Report. */
  uint32_t           mpsReportTypeBitMask;
  /* 0 means the MPS report is Invalid. 1 Means Mps Report is Valid */
  uint8_t           isMpsReportValid;  
   /* 0 - MPS_PAIR_FAIL, 1-MPS_PAIR_SUCCESS */
  uint8_t           mpsPairStatus; 
  /* mpsPrivacyChanged=1 if privacy changed else mpsPrivacyChanged=0 */
  uint8_t           mpsPrivacyChanged; 
                          
  uint8_t           mpsInPrgPbNodeId;

  /* Network Name */
  uint8_t           networkName[32];            
  /* MPS Init scan params. */
  MPS_INIT_SCAN_PAYLOAD_T mpsInitScanPayLoad;      
  MPS_PRIVACY_PARAMS_T mpsPrivacyParams;
} MXL_MOCA_SOC_GET_MPS_REPORT_RSP_T;

// Enhanced Privacy
typedef struct
{
  uint32_t   result;
  uint32_t   password[16];
} MXL_MOCA_SOC_GET_SEC_EP_PASSWORD_RSP_T; /* Hi_TxnGetSecPasswordRsp_t */

typedef struct
{
  /* Enhanced Security Keys */
  uint32_t   eammk[4];
  uint32_t   eapmkInit[4];
  uint32_t   eapmk[2][4];
  uint32_t   activeEApmk;
  uint32_t   eatek[2][4];
  uint32_t   activeEAtek;
} MXL_MOCA_SOC_GET_EP_SEC_INFO_RSP_T;   /* Hi_TxnGetEPSecInfoRsp_t  */

// Traffic Permission

// MXL_MOCA_SOC_SET_TRAFFIC_PERMIT_CFG / HI_TXNTYPE_SET_TRAFFIC_PERMIT_CFG

typedef struct
{
  uint32_t  ethernetType;            //!<  Ethernet Type for Traffic permission , 0~0xffff
  uint16_t  trafficNodeBitMask[16];  //!< Bit Mask for each node > 
} MXL_MOCA_SOC_SET_TRAFFIC_PERMIT_CFG_CMD_T;  /* Hi_TxnSetTrafficPermissionCfgCmd_t */

// MXL_MOCA_SOC_GET_TRAFFIC_PERMIT_STS / HI_TXNTYPE_GET_TRAFFIC_PERMIT_STS

typedef struct
{
  uint32_t  trafficPermissionStatus; //!<  Traffice permission Response for the command already sent , 1-successful, 2-failed
} MXL_MOCA_SOC_GET_TRAFFIC_PERMIT_STS_RSP_T;  /* Hi_TxnGetTrafficPermissionStsRsp_t */

// MXL_MOCA_SOC_GET_ETHER_TYPE_INFO / HI_TXNTYPE_GET_ETHER_TYPE_INFO

typedef struct
{
  uint32_t  ethernetType;            //!<  Ethernet Type for Traffic permission , 0~0xffff
} MXL_MOCA_SOC_GET_ETHER_TYPE_INFO_RSP_T;  /* Hi_TxnGetEtherTypeInfoRsp_t */

/*
 * MxL MoCA Driver command data structure
 */

// MXL_MOCA_DRV_GET_CFG_AND_FW_DIR 

typedef struct
{
  char confDir[CONFDIR_NAME_MAX];
  char fwDir[FWDIR_NAME_MAX];
} MXL_MOCA_DRV_GET_CFG_AND_FW_DIR_RSP_T;

// MXL_MOCA_DRV_GET_DAEMON_INFO 

typedef struct
{
  uint32_t clear;
} MXL_MOCA_DRV_GET_DAEMON_INFO_CMD_T;

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
} MXL_MOCA_DRV_GET_DAEMON_INFO_RSP_T;

// MXL_MOCA_DRV_SET_SOC_BOOTED

typedef struct
{
  uint32_t dlOnly;
} MXL_MOCA_DRV_SET_SOC_BOOTED_CMD_T;

// MXL_MOCA_DRV_SET_CD_FIRMWARE_CMD_T / MXL_MOCA_DRV_SET_CD_FIRMWARE

typedef struct 
{
  uint32_t size;
  uint8_t  *pData;
  void     **ppPages;
} MXL_MOCA_FIRMWARE_ENTRY_T;

typedef struct
{
  MXL_MOCA_FIRMWARE_ENTRY_T *fw_entry[MXL_MOCA_MAX_FW_IDX];
  uint32_t                  ret;
} MXL_MOCA_DRV_SET_CD_FIRMWARE_CMD_T;

typedef MXL_MOCA_DRV_SET_CD_FIRMWARE_CMD_T MXL_MOCA_FIRMWARE_T;

// MXL_MOCA_DRV_GET_CHIP_TYPE

typedef struct
{
  uint32_t type; // 0: Jaws, 1: Malaga, 2: Cardiff, 3: Leucadia
} MXL_MOCA_DRV_GET_CHIP_TYPE_RSP_T;

// MXL_MOCA_DBG_SET_MEM

typedef struct
{
  uint32_t socMemAddr[CLNK_CTL_MAX_IN_LEN/4];  // The first word is the address, following words are data
} MXL_MOCA_DRV_SET_DBG_MEM_CMD_T;

// MXL_MOCA_DBG_GET_MEM

typedef struct
{
  uint32_t socMemAddr;
} MXL_MOCA_DRV_GET_DBG_MEM_CMD_T;

typedef struct
{
  uint32_t usrData[CLNK_CTL_MAX_OUT_LEN/4];
} MXL_MOCA_DRV_GET_DBG_MEM_RSP_T;

// MXL_MOCA_DRV_START_DAEMON

typedef struct
{
  uint8_t  confDir[CONFDIR_NAME_MAX];
  uint8_t  fwDir[FWDIR_NAME_MAX];
  uint32_t macHi;
  uint32_t macLo;
  uint32_t dlOnly;
  uint32_t dlVerify;
} MXL_MOCA_DRV_START_DAEMON_CMD_T;

// SIOC_MXL_MOCA_START_DAEMON

typedef struct
{
  uint16_t phyId;
  uint16_t regNum;
  uint16_t valIn;
  uint16_t valOut;
  uint16_t devAddr;  //Used for Clause 45 MDIO
} MXL_MOCA_MMI_IOCTL_DATA_T;  ///< mmi_ioctl_data

typedef struct
{
  uint8_t name[16];
  union 
  {
    uint32_t                  *ptr;
    MXL_MOCA_MMI_IOCTL_DATA_T mmi;
    uint8_t                   mac[8];
    uint32_t                  otpDword;
  } dat;
} MXL_MOCA_DRV_RW_MII_REG_CMD_T; ///< ioctl_stuff

// MXL_MOCA_DRV_SET_APPLICATION_TRACE

typedef struct
{
  uint32_t flag;
} MXL_MOCA_DRV_SET_APPLICATION_TRACE_CMD_T;

typedef struct
{
  uint16_t valOut;
} MXL_MOCA_DRV_RD_MII_REG_RSP_T;

// MoCA Control Commands
typedef struct
{
  // moca command
  uint32_t mocaCmdId; // MoCA command: MXL_MOCA_SOC_GET_SOC_STATUS, MXL_MOCA_DRV_GET_ETH_STATS

  // in_len
  uint32_t mocaCmdLenInBytes; // 0 or actual size in uCmd (0 means Cmd is not input argument in uCmd

  // union structure for request of all Moca Cmd
  union
  {
    MXL_MOCA_SOC_SET_INIT_PARAM_CMD_T         socSetInitParamCmd;
    MXL_MOCA_SOC_SET_INIT_RLAPM_CMD_T         socSetInitRlapmCmd;
    MXL_MOCA_SOC_SET_INIT_SAPM_CMD_T          socSetInitSapmCmd;
    MXL_MOCA_SOC_SET_INIT_PARAM_V25_CMD_T     socSetInitParamV25Cmd;       
    MXL_MOCA_SOC_GET_AGGR_STATS_CMD_T         socGetAggrStatsCmd;
    MXL_MOCA_SOC_GET_BRIDGE_STATS_CMD_T       socGetBridgeStatsCmd;
    MXL_MOCA_SOC_GET_BRIDGE_TABLE_CMD_T       socGetBridgeTableCmd;
    MXL_MOCA_SOC_GET_EPHY_STATS_CMD_T         socGetEphyStatsCmd;
    MXL_MOCA_SOC_GET_FRAME_STATS_CMD_T        socGetFrameStatsCmd;
    MXL_MOCA_SOC_GET_NET_INFO_CMD_T           socGetNetInfoCmd;
    MXL_MOCA_SOC_SET_MULTICAST_FILTER_CMD_T   socSetMultiCastFilterCmd;
    MXL_MOCA_SOC_GET_RETRANS_STATS_CMD_T      socGetRetransStatsCmd;
    MXL_MOCA_SOC_GET_FRAG_STATS_CMD_T         socGetFragStatsCmd;
    MXL_MOCA_SOC_GET_L2ME_STATS_CMD_T         socGetL2meStatsCmd;
    MXL_MOCA_SOC_SET_MCAST_MODE_CMD_T         socSetMcastModeCmd;
    MXL_MOCA_SOC_SET_PHYMODE_BITMASK_CMD_T    socSetPhyModBitmaskCmd;
    MXL_MOCA_SOC_REQ_HANDOFF_CMD_T            socReqHandOffCmd;
    MXL_MOCA_SOC_REQ_LMO_CMD_T                socReqLMOCmd;
    MXL_MOCA_SOC_GET_FMR_INFO_CMD_T           socGetFmrInfoCmd;
    MXL_MOCA_SOC_GET_DD_INFO_CMD_T            socGetDdInfoCmd;
    MXL_MOCA_SOC_MOCA_RESET_CMD_T             socMocaResetCmd;
    MXL_MOCA_SOC_REQ_NMS_CMD_T                socReqNmsCmd;
    MXL_MOCA_SOC_GET_PHY_TX_PROFILE_CMD_T     socGetPhyTxProfileCmd;
    MXL_MOCA_SOC_GET_PHY_RX_PROFILE_CMD_T     socGetPhyRxProfileCmd;
    MXL_MOCA_SOC_GET_PHY_NODE_CMD_T           socGetPhyNodeCmd;
    MXL_MOCA_SOC_GET_PHY_EVM_CMD_T            socGetPhyEvmCmd;
    MXL_MOCA_SOC_GET_OFDMA_TX_PROFILE_CMD_T   socGetOfdmaTxProfileCmd;
    MXL_MOCA_SOC_GET_OFDMA_RX_PROFILE_CMD_T   socGetOfdmaRxProfileCmd;
    MXL_MOCA_SOC_POWER_SLEEP_CMD_T            socPowerSleepCmd;
    MXL_MOCA_SOC_POWER_WAKE_CMD_T             socPowerWakeCmd;
    MXL_MOCA_SOC_QUERY_PM_STATE_CMD_T         socQueryPmStateCmd;
    MXL_MOCA_SOC_GET_PM_EVENT_CMD_T           socGetPmEventCmd;
    MXL_MOCA_SOC_PFP_CMD_T                    socCreateFlowCmd;
    MXL_MOCA_SOC_PFP_CMD_T                    socUpdateFlowCmd;
    MXL_MOCA_MAC_ADDR_T                       socDeleteFlowCmd;
    MXL_MOCA_SOC_FLOW_LIST_CMD_T              socFlowListCmd;
    MXL_MOCA_MAC_ADDR_T                       socQueryFlowCmd;
    MXL_MOCA_SOC_GET_FLOW_STATS_CMD_T         socGetFlowStatsCmd;
    MXL_MOCA_SOC_GET_SEC_STATS_CMD_T          socGetSecStatsCmd;
    MXL_MOCA_SOC_SET_MULTICAST_CFG_CMD_T      socSetMultiCastCfgCmd;
    MXL_MOCA_SOC_SET_GET_TXPWR_VAR_CMD_T      socSetGetTxPwrVarCmd;
    MXL_MOCA_SOC_TEST_CMD_T                   socTestCmd;
    MXL_MOCA_SOC_GET_SWITCH_STATS_CMD_T       socGetSwitchStatsCmd;
    MXL_MOCA_SOC_GET_SWITCH_CONFIG_CMD_T      socGetSwitchConfigCmd;

    MXL_MOCA_SOC_GET_NETWORK_NAME_REC_CMD_T     socGetNetworkNameRecCmd;
    MXL_MOCA_SOC_GET_NETWORK_NAME_PAYLOAD_CMD_T socGetNetworkNamePayloadCmd;
    MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_TX_CMD_T socMeNetworkIePayloadTxCmd;
    MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_RX_CMD_T socMeNetworkIePayloadRxCmd;
    MXL_MOCA_SOC_ME_NETWORK_JOIN_CMD_T          socMeNetworkJoinCmd;
    
    MXL_MOCA_SOC_RESET_SWITCH_CONFIG_CMD_T    socResetSwitchConfigCmd;
    MXL_MOCA_SOC_SET_SWITCH_TMO_CMD_T         socSetSwitchTmoCmd;
    MXL_MOCA_SOC_ENA_SWITCH_VLAN_CHECK_CMD_T  socEnaSwitchVlanCheckCmd;
    MXL_MOCA_SOC_DIS_SWITCH_VLAN_CHECK_CMD_T  socDisSwitchVlanCheckCmd;
    MXL_MOCA_SOC_ENA_SWITCH_VLAN_STRIP_CMD_T  socEnaSwitchVlanStripCmd;
    MXL_MOCA_SOC_DIS_SWITCH_VLAN_STRIP_CMD_T  socDisSwitchVlanStripCmd;
    MXL_MOCA_SOC_SET_SWITCH_VLAN_PVID_CMD_T   socSetSwitchVlanPvidCmd;
    MXL_MOCA_SOC_ADD_SWITCH_VLANID_CMD_T      socAddSwitchVlanIdCmd;
    MXL_MOCA_SOC_DEL_SWITCH_VLANID_CMD_T      socDelSwitchVlanIdCmd;
    MXL_MOCA_SOC_SET_SWITCH_MGMT_MAC_CMD_T    socSetSwitchMgmtMacCmd;
    MXL_MOCA_SOC_GET_MACDATA_STATS_CMD_T      socGetMacDataStatsCmd;
    MXL_MOCA_SOC_SET_SWITCH_PORT_CONFIG_CMD_T socSetSwitchPortConfigCmd;
    MXL_MOCA_SOC_GET_ACTIVE_RLAPM_CMD_T       socGetActiveRlapmCmd;
    MXL_MOCA_SOC_GET_ACTIVE_SAPM_CMD_T        socGetActiveSapmCmd;
    MXL_MOCA_SOC_NET_PROXY_PKT_SEND_REQ_CMD_T  socSendNetProxyPktCmd;
    MXL_MOCA_SOC_NET_PROXY_TEST_MODE_CTRL_CMD_T socSendNetProxyTestModeCtrlCmd;
    MXL_MOCA_SOC_SET_BCN_POWER_CMD_T          socSetBcnPowerCmd;
    MXL_MOCA_SOC_REQUEST_BCNPWR_PIE_CMD_T     socReqBcnPowerPiecmd;
    MXL_MOCA_SOC_SET_TRAFFIC_PERMIT_CFG_CMD_T socSetTrafficPermitCfgCmd;
    MXL_MOCA_SOC_GET_ECB_STATS_CMD_T          socGetEcbStatsCmd;

    MXL_MOCA_DRV_SET_OTP_CONFIG_CMD_T         drvSetOtpConfigCmd;
    MXL_MOCA_DRV_GET_DAEMON_INFO_CMD_T        drvGetDaemonInfoCmd;
    MXL_MOCA_DRV_SET_SOC_BOOTED_CMD_T         drvSetSocBootedCmd;
    MXL_MOCA_DRV_SET_DBG_MEM_CMD_T            drvSetDbgMemCmd;
    MXL_MOCA_DRV_GET_DBG_MEM_CMD_T            drvGetDbgMemCmd;
    MXL_MOCA_DRV_SET_CD_FIRMWARE_CMD_T        drvSetDaemonFirmwareCmd;
    MXL_MOCA_DRV_START_DAEMON_CMD_T           drvStartDaemonCmd;
    MXL_MOCA_MAC_ADDR_T                       drvSetMacAddrCmd;
    MXL_MOCA_DRV_RW_MII_REG_CMD_T             drvRwMiiRegCmd;
    MXL_MOCA_DRV_SET_APPLICATION_TRACE_CMD_T  drvSetAppTraceCmd;

  } uCmd;

} MXL_MOCA_OP_CMD_T;

typedef struct
{
  // moca command
  uint32_t mocaCmdId; // Callee should return the same command Id that it got from Op Command, if it's NULL, Response had been failed.

  // out_len
  uint32_t mocaRspLenInBytes; // 0 or actual size in uRsp (0 means Cmd is not output argument in uRsp)

  // union structure for response of all Moca Cmd
  union
  {
    MXL_MOCA_SOC_GET_SOC_STATUS_RSP_T         socGetStatusRsp;
    MXL_MOCA_SOC_GET_IMAGE_INFO_RSP_T         socGetImageInfoRsp;
    MXL_MOCA_SOC_INIT_SYSTEM_START_RSP_T      socSetInitSystemStartRsp;
    MXL_MOCA_SOC_GET_AGGR_STATS_RSP_T         socGetAggrStatsRsp;
    MXL_MOCA_SOC_GET_BRIDGE_STATS_RSP_T       socGetBridgeStatsRsp;
    MXL_MOCA_SOC_GET_BRIDGE_TABLE_RSP_T       socGetBridgeTableRsp;
    MXL_MOCA_SOC_GET_EPHY_STATS_RSP_T         socGetEphyStatsRsp;
    MXL_MOCA_SOC_GET_FRAME_STATS_RSP_T        socGetFrameStatsRsp;
    MXL_MOCA_SOC_GET_DATA_STATS_RSP_T         socGetDataStatsRsp;
    MXL_MOCA_SOC_GET_LOCAL_INFO_RSP_T         socGetLocalInfoRsp;
    MXL_MOCA_SOC_GET_NET_INFO_RSP_T           socGetNetInfoRsp;
    MXL_MOCA_SOC_SET_MULTICAST_FILTER_RSP_T   socSetMultiCastFilterRsp;
    MXL_MOCA_SOC_GET_RETRANS_STATS_RSP_T      socRetransStatsRsp;
    MXL_MOCA_SOC_GET_FRAG_STATS_RSP_T         socGetFragStatsRsp;
    MXL_MOCA_SOC_GET_L2ME_STATS_RSP_T         socGetL2meStatsRsp;
    MXL_MOCA_SOC_GET_MCAST_MODE_RSP_T         socGetMcastModeRsp;
    MXL_MOCA_SOC_SET_MCAST_MODE_RSP_T         socSetMcastModeRsp;
    MXL_MOCA_SOC_SET_PHYMODE_BITMASK_RSP_T    socSetPhyModBitmaskRsp;
    MXL_MOCA_SOC_REQ_HANDOFF_RSP_T            socReqHandoffRsp;
    MXL_MOCA_SOC_REQ_LMO_RSP_T                socReqLMORsp;
    MXL_MOCA_SOC_GET_FMR_INFO_RSP_T           socGetFmrInfoRsp;
    MXL_MOCA_SOC_GET_DD_INFO_RSP_T            socGetDdInfoRsp;
    MXL_MOCA_SOC_MOCA_RESET_RSP_T             socMocaResetRsp;
    MXL_MOCA_SOC_REQ_NMS_RSP_T                socReqNmsRsp;
    MXL_MOCA_SOC_GET_PHY_TX_PROFILE_RSP_T     socGetPhyTxProfileRsp;
    MXL_MOCA_SOC_GET_PHY_RX_PROFILE_RSP_T     socGetPhyRxProfileRsp;
    MXL_MOCA_SOC_GET_PHY_NODE_RSP_T           socGetPhyNodeRsp;
    MXL_MOCA_SOC_GET_PHY_EVM_RSP_T            socGetPhyEvmRsp;
    MXL_MOCA_SOC_GET_PHY_MISC_RSP_T           socGetPhyMiscRsp;
    MXL_MOCA_SOC_GET_PHY_OFDMA_DEF_RSP_T      socGetPhyOfdmaDefRsp;
    MXL_MOCA_SOC_GET_PHY_OFDMA_ASSIGN_RSP_T   socGetPhyOfdmaAssignRsp;
    MXL_MOCA_SOC_GET_OFDMA_PARAMS_RSP_T       socGetOfdmaParamsRsp;
    MXL_MOCA_SOC_GET_OFDMA_TX_PROFILE_RSP_T   socGetOfdmaTxProfileRsp;
    MXL_MOCA_SOC_GET_OFDMA_RX_PROFILE_RSP_T   socGetOfdmaRxProfileRsp;
    MXL_MOCA_SOC_POWER_SLEEP_RSP_T            socPowerSleepRsp;
    MXL_MOCA_SOC_POWER_WAKE_RSP_T             socPowerWakeRsp;
    MXL_MOCA_SOC_GET_PM_CAPS_RSP_T            socGetPmCapsRsp;
    MXL_MOCA_SOC_QUERY_PM_STATE_RSP_T         socQueryPmStateRsp;
    MXL_MOCA_SOC_GET_PM_EVENT_RSP_T           socGetPmEventRsp;
    MXL_MOCA_SOC_HOST_IF_READY_RSP_T          socHostIfReadyRsp;
    MXL_MOCA_SOC_FLOW_CREATE_OR_UPDATE_RSP_T  socFlowCreateRsp;
    MXL_MOCA_SOC_FLOW_CREATE_OR_UPDATE_RSP_T  socFlowUpdateRsp;
    MXL_MOCA_SOC_FLOW_DELETE_RSP_T            socFlowDeleteRsp;
    MXL_MOCA_SOC_FLOW_LIST_RSP_T              socFlowListRsp;
    MXL_MOCA_SOC_FLOW_QUERY_RSP_T             socFlowQueryRsp;
    MXL_MOCA_SOC_FLOW_STATS_RSP_T             socFlowStatsRsp;
    MXL_MOCA_SEC_PASSWORD_T                   socSecPassword;
    MXL_MOCA_SOC_GET_SEC_PASSWD_RSP_T         socGetSecPasswdRsp;
    MXL_MOCA_SOC_GET_SEC_INFO_RSP_T           socGetSecInfoRsp;
    MXL_MOCA_SOC_GET_SEC_STATS_RSP_T          socGetSecStatsRsp;
    MXL_MOCA_SOC_SET_MULTICAST_CFG_RSP_T      socSetMultiCastCfgRsp;
    MXL_MOCA_SOC_SET_GET_TXPWR_VAR_RSP_T      socSetGetTxPwrVarRsp;
    MXL_MOCA_SOC_GET_ACTIVE_RLAPM_RSP_T       socGetActiveRlapmRsp;
    MXL_MOCA_SOC_GET_ACTIVE_SAPM_RSP_T        socGetActiveSapmRsp;
    MXL_MOCA_SOC_ENDET_STATUS_RSP_T           socEndetStatusRsp;
    MXL_MOCA_SOC_TEST_RSP_T                   socTestRsp;
    MXL_MOCA_SOC_GET_SWITCH_STATS_RSP_T       socGetSwitchStatsRsp;
    MXL_MOCA_SOC_GET_SWITCH_CONFIG_RSP_T      socGetSwitchConfigRsp;

    MXL_MOCA_SOC_GET_NETWORK_NAME_REC_RSP_T     socGetNetworkNameRecRsp;
    MXL_MOCA_SOC_GET_NETWORK_NAME_PAYLOAD_RSP_T socGetNetworkNamePayloadRsp;
    MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_TX_RSP_T socMeNetworkIePayloadTxRsp;
    MXL_MOCA_SOC_ME_NETWORK_IE_PAYLOAD_RX_RSP_T socMeNetworkIePayloadRxRsp;
    MXL_MOCA_SOC_ME_NETWORK_JOIN_RSP_T          socMeNetworkJoinRsp;
    MXL_MOCA_SOC_GET_TRAFFIC_PERMIT_STS_RSP_T   socGetTrafficPermitStsRsp;
    MXL_MOCA_SOC_GET_ETHER_TYPE_INFO_RSP_T      socGetEtherTypeInfoRsp;

    MXL_MOCA_SOC_NET_PROXY_TEST_MODE_CTRL_RSP_T socSendNetProxyTestModeCtrlRsp;
    
    MXL_MOCA_SOC_RESET_SWITCH_CONFIG_RSP_T    socResetSwitchConfigRsp;
    MXL_MOCA_SOC_SET_SWITCH_TMO_RSP_T         socSetSwitchTmoRsp;
    MXL_MOCA_SOC_ENA_SWITCH_VLAN_CHECK_RSP_T  socEnaSwitchVlanCheckRsp;
    MXL_MOCA_SOC_DIS_SWITCH_VLAN_CHECK_RSP_T  socDisSwitchVlanCheckRsp;
    MXL_MOCA_SOC_ENA_SWITCH_VLAN_STRIP_RSP_T  socEnaSwitchVlanStripRsp;
    MXL_MOCA_SOC_DIS_SWITCH_VLAN_STRIP_RSP_T  socDisSwitchVlanStripRsp;
    MXL_MOCA_SOC_SET_SWITCH_VLAN_PVID_RSP_T   socSetSwitchVlanPvidRsp;
    MXL_MOCA_SOC_ADD_SWITCH_VLANID_RSP_T      socAddSwitchVlanIdRsp;
    MXL_MOCA_SOC_DEL_SWITCH_VLANID_RSP_T      socDelSwitchVlanIdRsp;
    MXL_MOCA_SOC_SET_SWITCH_MGMT_MAC_RSP_T    socSetSwitchMgmtMacRsp;
    MXL_MOCA_SOC_GET_MACDATA_STATS_RSP_T      socGetMacDataStatsRsp;
    MXL_MOCA_SOC_SET_SWITCH_PORT_CONFIG_RSP_T socSetSwitchConfigRsp;
    MXL_MOCA_SOC_SET_BCN_POWER_RSP_T          socSetBcnPowerRsp;
    MXL_MOCA_SOC_GET_BCN_POWER_RSP_T          socGetBcnPowerRsp;
    MXL_MOCA_SOC_REQUEST_BCNPWR_PIE_RSP_T     socReqBcnPowerPieRsp;
    MXL_MOCA_SOC_SET_MPS_TRIGGER_RSP_T        socSetMpsTriggerRsp;
    MXL_MOCA_SOC_GET_MPS_REPORT_RSP_T         socGetMpsReportRsp;
    MXL_MOCA_SOC_GET_MPS_STATE_RSP_T          socGetMpsStateRsp;
    MXL_MOCA_SOC_GET_SEC_EP_PASSWORD_RSP_T    socGetSecEpPasswordRsp;
    MXL_MOCA_SOC_GET_EP_SEC_INFO_RSP_T        socGetEpSecInfoRsp;
    MXL_MOCA_SOC_NET_PROXY_PKT_RCV_RSP_T      socSendNetProxyPktRsp;
    MXL_MOCA_DRV_GET_CFG_AND_FW_DIR_RSP_T     drvGetCfgAndFwDirRsp;
    MXL_MOCA_DRV_GET_DAEMON_INFO_RSP_T        drvGetDaemonInfoRsp;
    MXL_MOCA_DRV_GET_CHIP_TYPE_RSP_T          drvGetChipTypeRsp;
    MXL_MOCA_DRV_GET_DBG_MEM_RSP_T            drvGetDbgMemRsp;
    MXL_MOCA_DRV_RD_MII_REG_RSP_T             drvRwMiiRegRsp;
    MXL_MOCA_SOC_GET_EPHY_STATS_RSP_T         socGetEcbStatsRsp;
    
  } uRsp;

} MXL_MOCA_OP_RSP_T;

typedef struct
{
  void* pParam1;
  void* pParam2;
  void* pParam3;
} MXL_MOCA_CMD_PARAMS;

/*****************************************************************************************
    Global Variable Declarations
 ****************************************************************************************/

/*****************************************************************************************
    Function Prototypes
 ****************************************************************************************/

#endif // __MXL_MOCA_SOC_CMD_H__
