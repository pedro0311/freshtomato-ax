/*****************************************************************************************   
 *
 * FILE NAME          : mxl_moca_config.h
 * 
 *
 *
 * DATE CREATED       : Sep/14/2016
 *
 * LAST MODIFIED      : 
 *
 * DESCRIPTION        : Common definitions used by the driver and the lib
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

#ifndef __MXL_MOCA_CONFIG_H__
#define __MXL_MOCA_CONFIG_H__


/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
 ****************************************************************************************/

#include "mxl_data_types.h"

/*****************************************************************************************
    Macros
 ****************************************************************************************/

#define DRV_NAME_MDIO "mxl_moca_ctrl"
#define DRV_NAME_PCIE "mxl_moca_pcie" 

#define MAX_NUM_CHANNELS            5

#define LINK_UP                     1
#define LINK_DOWN                   0
#define LINK_FORCE_DOWN            -1

#define MXL_MOCA_DATAPATH_MODE_PCIE          0
#define MXL_MOCA_DATAPATH_MODE_XMII          1
#define MXL_MOCA_DATAPATH_MODE_UNKNOWN      -1

#define FFT_LENGTH                  512
#define MAX_RX_LEVELS               81
#define MAX_RX_LEVELS_PADDED        (((MAX_RX_LEVELS + 3) / 4) * 4)     // 84
#define MAX_RLAPM_FREQS             6
#define MAX_SAPM_FREQS              4
#define MAX_MINI_SAPM_MARGIN_LEN    256
#define MOCA_MAX_PACKET_SIZE        1520 //must align with word boundary

// PclInit_statusWd defines ([31:16]exceptions, [15:0] status)
#define PCL_INIT_EXCEPTION_SUMMARY_BIT        0x80000000

// PclInit_exceptionWd defines:
#define PCL_INIT_EXCEPTION_SUMMARY_BIT        0x80000000
#define GPIO_TO_ASPEN_FAILED                  0x00000001
//set if Aspen Rev B0 in ASPEN_C0 build
#define INVALID_DEV_ASPEN_C0_BLD              0x00000002
//set if ! Aspen Rev B0 in !ASPEN_C0 build
#define INVALID_DEV_ASPEN_B0_BLD              0x00000004
#define JAWS_SERDES_PLL_LOCK_FAILED           0x00000008
#define ASPEN_SERDES_PLL_LOCK_FAILED          0x00000010
#define JAWS_ASPEN_SERDES_LOCK_FAILED         0x00000020
#define JAWS_ASPEN_SERDES_COMM_CHECK_FAILED   0x00000040
#define JAWS_ASPEN_SERDES_COMM_FAILURE        0x00000080
#define LPF_CAL_VALIDATION_FAILED             0x00000100
#define IQ_COMP_TX_XFER_STS_FAILED            0x00000200
#define IQ_COMP_RX_XFER_STS_FAILED            0x00000400
#define IQ_COMP_FAILED_TO_RX_PROBE            0x00000800
#define PCL_INIT_EXCEPTION_C                  0x00001000
#define PCL_INIT_EXCEPTION_D                  0x00002000
#define PCL_INIT_EXCEPTION_E                  0x00004000
#define PCL_INIT_EXCEPTION_F                  0x00008000


#define MAX_NUM_NODES                   16

#define IFNAME_MAX                      15
#define MAX_NODES                       16
#define MAX_FLOWS_IN_LIST               32

// ADM
#define ADM_RESULT_ERROR                6

// Energy detect array dimensions
#define MAX_ATSC_CATV_FREQS             4
#define MAX_ATSC_CATV_GAPS              8
#define MAX_WEIGHTS                     4
#define MAX_BTS_CSS_FREQS               6
#define MAX_BTS_CSS_VALUES              5

// Energy detection related constant
#define CATV_DETECTED                   1<<0
#define OTA_ATSC_DETECTED               1<<1
#define CSS_DETECTED                    1<<2
#define BTS_DETECTED                    1<<3
#define NUM_BAND_E_100MHZ_RANK          5
#define NUM_BAND_F_100MHZ_RANK          6
#define NUM_BAND_D_100MHZ_RANK          19

#define NUM_BAND_E_50MHZ_RANK           7
#define NUM_BAND_F_50MHZ_RANK           8
#define NUM_BAND_D_50MHZ_RANK           21

#define GET_BIT(var, bitoffset)         (((var) >> (bitoffset)) & 1)

#define MAX_START_FREQ_INDEX            48
#define MIN_CHANNEL_SPACING             1
#define MAX_CHANNEL_SPACING             4
#define MIN_LOG_GAIN_OFFSET             -128
#define MAX_LOG_GAIN_OFFSET             127
#define MAX_GCD_TX_POWER                5  // 5dbm out of Chip & ~2.5dbm at F connector on ECA.
#define MAX_ALLOWED_BACKOFF             30 // db       

#define MAX_VERSION_STR_SIZE            24
#define MAX_HOST_VERSION_STR_SIZE       128
#define SWITCH_PORT_MAX_VLAN_IDS        16
#define SWITCH_CAM_STATIC_ENTRIES       128
#define SWITCH_CAM_DYNAMIC_ENTRIES      128
#define CLNK_SWITCH_BRIDGE_STATIC_DST   0
#define CLNK_SWITCH_BRIDGE_DYNAMIC_DST  1

#define MAX_SAPM_PROFILES               16
#define MAX_RLAPM_PROFILES              16
#define MAX_FREQ_COMP_TBL_LEN           49
#define MAX_RSSI_VARS                   3
#define MAX_RSSI_LEN                    (MAX_FREQ_COMP_TBL_LEN + MAX_RSSI_VARS)

#define LMO_ACA_RPT_PWR_PRFL_SZ         512
#define MAX_OFDMA_PROFILES              3 // defines for OFDMA
#define CONFDIR_NAME_MAX                256
#define FWDIR_NAME_MAX                  256
#define HDRCMD_READ_SW_RESET_STATUS     0x80
#define HDRCMD_READ_DIP_LINE_STATUS     0x81
#define HDRCMD_VAL_ON                   0x88
#define HDRCMD_VAL_OFF                  0xff

// Bridge detection
#define MAX_MOCA25_ENHANCED_PASSWORD_LEN     64
#define MAX_NW_NAME_LEN                      32
#define MOCA25_ME_IE_REQ_PAYLOAD_SIZE_BYTES  60 
#define MOCA25_ME_IE_RSP_PAYLOAD_SIZE_BYTES  62 
#define MOCA25_ME_IE_REQ_MBOX_SIZE_BYTES     (MOCA25_ME_IE_RSP_PAYLOAD_SIZE_BYTES + 10)
#define MOCA25_ME_IE_RSP_MBOX_SIZE_BYTES     (MOCA25_ME_IE_RSP_PAYLOAD_SIZE_BYTES + 10) 
#define MAX_ME_NW_IE_PAYLOAD_LEN             MOCA25_ME_IE_REQ_MBOX_SIZE_BYTES
#define DEFAULT_ME_IE_RSP_TIMEOUT            1000 /* Increments of 10 msec */

#define MIN_VAL(x, y) (((x) > (y)) ? (y) : (x))
#define MAX_VAL(x, y) (((x) > (y)) ? (x) : (y))

/* NOTE: must be large enough to hold EPP data, EVM data, or bridge table */
#define CLNK_CTL_MAX_IN_LEN    0x4800
#define CLNK_CTL_MAX_OUT_LEN   0x4800

#define CLNK_HI_PCIE                0
#define CLNK_HI_MII                 1
#define CLNK_MII_SPD_100M           1
#define CLNK_MII_SPD_200M           2
#define CLNK_MII_SPD_1000M          3
#define CLNK_MII_SPD_1000M_RGMII    4

#define CLNK_BRIDGE_UCAST_DST       0
#define CLNK_BRIDGE_MCAST_DST       1
#define CLNK_BRIDGE_PQOS_FLOW       2
#define CLNK_BRIDGE_UCAST_SRC       3
#define CLNK_BRIDGE_MCAST_SRC       4


// hwCfgWord bit definitions
#define HW_CFG_HW_REV_0304             0x00000001   // 1 - HW rev 0304; 0 - pre-0304 HW revs
#define DIPLEXER_ONT                   0x00000010
#define DIPLEXER_BHR                   0x00000020
#define DIPLEXER_UNV                   0x00000040
#define SERDES_TRACE_LENGTH_MASK       0x000F0000
#define SERDES_TRACE_LENGTH_MIN_TRACE  0x00010000
#define SERDES_TRACE_LENGTH_MID_TRACE  0x00020000
#define SERDES_TRACE_LENGTH_MAX_TRACE  0x00040000   //Default - the setting of this bit is not required
#define SERDES_TRACE_LENGTH_MS_SWITCH  0x00080000
#define HW_CFG_BAND_D_EXTENDED         0x80000000
#define HW_CFG_BAND_D_LOW              0x40000000
#define HW_CFG_BAND_D_HIGH             0x20000000
#define HW_CFG_BAND_E                  0x10000000
#define HW_CFG_BAND_F                  0x08000000
#define MINDSPEED_CPS_INSTALLED        SERDES_TRACE_LENGTH_MS_SWITCH

/* legal values for priv->enable */
#define PRIV_DISABLED                  0   /* must be zero */
#define PRIV_ENABLED                   1
#define PRIV_SHUTDOWN                  2
#define PRIV_STATUS_EOE                3   /* END OF ENUM */


#ifndef MAX_PASSWORD_LENGTH
#define MAX_PASSWORD_LENGTH             17
#endif
#define MAX_PASSWORD_LENGTH_PADDED      (((MAX_PASSWORD_LENGTH + 3) / 4) * 4)
#define MAX_MOCA_EP_PASSWORD_LEN_PADDED 64

#define GPIO_PIN_INVALID                -1

// MoCA 2.0 spec limits maximum Ethernet packets per aggregation to 20
#define MAX_AGGR_FRAMES      20
// MoCA 2.5 support maximum Ethernet packets per aggregation to 160
#define MAX_AGGR_FRAMES_V25  160

// The Number of Channel range is 1~5
#define FREQ_CHN_NUM_MIN 1
#define FREQ_CHN_NUM_MAX 5

// Define NULL for beacon power
#define BEACON_POWER_NULL 100 // decimal, the same value as in default config file

#define MAX_BEACON_TX_POWER 10

#define ADM_STATUS_INCORRECT_PSWD 5

/** The module debug flag */
#define TRACE_HAL       (0x00000001)
#define TRACE_CPCR      (0x00000008)
#define TRACE_OSWP      (0x00000020)
#define TRACE_MBX       (0x00000040)
#define TRACE_CD        (0x00000100)

/*******************************************************************************
*                    # f e a t u r e   d e f i n i t i o n                     *
********************************************************************************/

#define NETWORK_TYPE_HIRF_MESH         1
#define NETWORK_TYPE_MIDRF_MESH        0
#define NETWORK_TYPE_MESH              1 // needed for mara to compile

/* For SoC JTAG debug */     
#define CARDIFF_JTAG_DEBUG             0  
#
#define FEATURE_QOS                    1
#define FEATURE_SAPM                   1
#define FEATURE_RLAPM                  1

/**
 * This marks code belonging to the FEIC Power Calibration project.
 */
#if ! defined(FEATURE_FEIC_PWR_CAL)
#define FEATURE_FEIC_PWR_CAL           1
#endif

/**
 * This marks code belonging to the Pre UPnP PQoS NMS project.
 */
#if ! defined(FEATURE_PUPQ_NMS)
#define FEATURE_PUPQ_NMS               1
#endif

/**
 * This marks code added to round out last minute customer requests.
 */
#if ! defined(FEATURE_PUPQ_NMS_QUICK_APIS)
#define FEATURE_PUPQ_NMS_QUICK_APIS    1
#endif

/**
 * This marks code belonging to the Pre UPnP PQoS NMS project-
 * new configuration items.  It is not tested disabled.
 */
#if ! defined(FEATURE_PUPQ_NMS_CONF)
#define FEATURE_PUPQ_NMS_CONF          1
#endif

/**
 * This marks code used to collect per-node statistics on receiver
 * received packet errors, both sync and async.
 */
#if ! defined(FEATURE_PUPQ_RX_NODE_STATS)
#define FEATURE_PUPQ_RX_NODE_STATS     1
#endif

/**
 * Define which UEVENTS to generate, note udev is required for systems using
 * this feature.  UEVENTS disabled by default, set to 1 to enable 
 */
#define ENABLE_LINK_UEVENT             0 
#define ENABLE_ADM_STATUS_UEVENT       0 

/*****************************************************************************************
    User-Defined Types (Typedefs)
 ****************************************************************************************/
 // Return Codes
typedef enum
{
  MXL_MOCA_OK                          = 0,   /* OK */
  MXL_MOCA_ERR                         = -1,  /* Error */
  MXL_MOCA_NODEV_ERR                   = -2,  /* No MoCA device */
  MXL_MOCA_WHICHDEV_ERR                = -3,  /* Which MoCA device to select */
  MXL_MOCA_MEM_ALLOC_ERR               = -4,  /* Memory allocation error */
  MXL_MOCA_RESET_ERR                   = -5,  /* Reset */
  MXL_MOCA_NOT_OPEN_ERR                = -6,  /* Not open */
  MXL_MOCA_LINK_DOWN_ERR               = -7,  /* Link down */
  MXL_MOCA_NO_HOST_DESC_ERR            = -8,  /* No host description */
  MXL_MOCA_UCAST_FLOOD_ERR             = -9,  /* Unicast flood */
  MXL_MOCA_NO_KEY_ERROR                = -10, /* No key */
  MXL_MOCA_PKT_LEN_ERR                 = -11, /* Packet length error */ // SYS Define
  MXL_MOCA_INPUT_OUTPUT_ERR            = -12, /* Input/output error */
  MXL_MOCA_PERMISSION_ERR              = -13, /* Permission denied (-EACCESS) */
  MXL_MOCA_INVALID_ADDRESS_ERR         = -14, /* Invalid address */
  MXL_MOCA_INVALID_ARGUMENT_ERR        = -15, /* Invalid argument */
  MXL_MOCA_OUT_OF_SPACE_ERR            = -16, /* Out of space */
  MXL_MOCA_DIR_NOT_EMPTY_ERR           = -17, /* Directory not empty */
  MXL_MOCA_BAD_MSG_TYPE_ERR            = -18, /* Bad message type */
  MXL_MOCA_INVALID_CMD_ERR             = -19, /* Operation not supported */
  MXL_MOCA_TIMEOUT_ERR                 = -20, /* Timeout */
  MXL_MOCA_INVALID_PARAM_ERR           = -21, /* Invalid input paramters to functions */

  MXL_MOCA_SIGNAL_RECVED               = -22, /* Signal received */
  MXL_MOCA_NO_SOC_RSP                  = -23, /* SOC did not respond to a command yet */
  MXL_MOCA_UNKNOWN_SOC_CMD             = -24, /* SOC did not recognize a command */ 
  MXL_MOCA_BUSY_ERR                    = -25, /* The device is busy */
  MXL_MOCA_KTHREAD_SHOULD_STOP         = -26, /* Thread should stop */
  MXL_MOCA_BUS_ERR                     = -27, /* Bus Error */
  
  MXL_MOCA_RET_CODE_MAX                = -28, /* This must always be last*/
} MXL_MOCA_STATUS_E;

typedef enum 
{
  MXL_MOCA_CCPU_IDX=0,   // MoCA firmware
  MXL_MOCA_CLINK_IDX,    // MoCA Config: MXL_MOCA_CFG_V2_T
  MXL_MOCA_MCAST_IDX,    // MXL_MOCA_MCAST_CFG_T
  MXL_MOCA_RLAPM_IDX,    // Receive-level added PHY margin
  MXL_MOCA_ENDET_IDX,    // MXL_MOCA_ENERGY_DETECT_CFG_T
  MXL_MOCA_SAPM_IDX,     // Subcarrier added PHY margin
  MXL_MOCA_RSSI_IDX,     // MXL_MOCA_RSSI_CFG_V2_T
  MXL_MOCA_PLATFORM_IDX, // MXL_MOCA_PLATFORM_CFG_T
  MXL_MOCA_MAX_FW_IDX,
} MXL_MOCA_FIRMWARE_IDX_E;

typedef enum
{
  MXL_MOCA_SOC_TYPE_JAWS = 0,
  MXL_MOCA_SOC_TYPE_MALAGA,
  MXL_MOCA_SOC_TYPE_CARDIFF,
  MXL_MOCA_SOC_TYPE_LEUCADIA,    
  MXL_MOCA_SOC_TYPE_UNKNOWN,
  MXL_MOCA_SOC_TYPE_MAX
} MXL_MOCA_SOC_TYPE_E;

typedef enum
{
  MOCA_BAND_D_EXT,
  MOCA_BAND_D_LO,
  MOCA_BAND_D_HI,
  MOCA_BAND_E,
  MOCA_BAND_F_SAT,
  MOCA_BAND_F_CBL,
  MOCA_BAND_H,
  MOCA_BAND_CUSTOM,
  MAX_BANDS
} MXL_MOCA_BAND_E; 

enum {
  CHANNEL_PRIMARY   = 0,   // Primary Channel
  CHANNEL_SECONDARY = 1,   // Secondary Channel
  CHANNEL_MAX, 
};

enum {
  TYPE_OF_LEGACY_PASSWORD   = 0,
  TYPE_OF_ENHANCED_PASSWORD = 1,
  TYPE_OF_PASSWORD_MAX, 
};
/*****************************************************************************************
    Global Variable Declarations
 ****************************************************************************************/

/*****************************************************************************************
    Function Prototypes
 ****************************************************************************************/

#endif // __MXL_MOCA_CONFIG_H__
