/*****************************************************************************************
 * FILE NAME          : ClnkIoCtl.h
 *
 *
 * DATE CREATED       : Oct/11/2016
 * LAST MODIFIED      :
 *
 * DESCRIPTION        : c.LINK Library I/O API(Internal) header file.
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

#ifndef __CLNK_API_LIO_H__
#define __CLNK_API_LIO_H__


/*******************************************************************************
 *      Includes
 ******************************************************************************/
#include "mxl_data_types.h"
#include "mxl_moca_config.h"
#include "ClnkLibApi.h"
#ifdef __linux__
#include "ClnkAppOsalLnx.h"
#endif
#include "mxl_moca_soc_cmd.h"


/*******************************************************************************
 *      Constants
 ******************************************************************************/


/*******************************************************************************
 *      Macros
 ******************************************************************************/


/*******************************************************************************
 *      Types
 ******************************************************************************/


/*******************************************************************************
 *      Public/Private Data
 ******************************************************************************/

/*!
 * \brief c.LINK Library I/O Structure
 */
typedef struct
{
  uint32_t      *in;
  uint32_t      in_len;
  uint32_t      *out;
  uint32_t      out_len;
} ClnkLio_t;

/*!
 * \brief c.LINK Library NetIO Structure
 */
typedef struct
{
  uint32_t      magic;
  uint32_t      version;
  uint32_t      cmd;
  uint32_t      transid;
  uint32_t      in_len;
  uint32_t      out_len;
  uint32_t      status;
  uint32_t      reserved[5];
} ClnkLioNetio_t;

/*******************************************************************************
 *      Public/Private Functions
 ******************************************************************************/
int32_t ClnkLio_Open(char *iface, const char *drvname);
int32_t ClnkLio_Close(char *iface);
int32_t ClnkLio_SendCmd(ClnkLib_Handle_t *pClnkIf, int cmd, ClnkLio_t *io, int32_t expBytes);

/* 1 argument (typically either "clear stats" or "node ID"); some output struct */
#define CLNK_CTL_GET_C(ctx, cmd, out_type, outv, code) \
    { \
        ClnkLio_t io; \
        int tmp = 0;\
        if (outv == NULL) \
            return CLNK_ERR; \
        io.in = 0;\
        io.in_len = 0; \
        io.out = (uint32_t *) outv; \
        io.out_len = sizeof(out_type); \
        tmp = ClnkLio_SendCmd(ctx, cmd, &io, -1); \
        code;\
        return tmp;\
    }


/* 1 argument (typically either "clear stats" or "node ID"); some output struct */
#define CLNK_CTL_GET_1(ctx, cmd, out_type, outv, inv) \
    { \
        ClnkLio_t io; \
        if (outv == NULL) \
            return CLNK_ERR; \
        io.in = (uint32_t *)(unsigned long) inv; \
        io.in_len = 0; \
        io.out = (uint32_t *) outv; \
        io.out_len = sizeof(out_type); \
        return(ClnkLio_SendCmd(ctx, cmd, &io, -1)); \
    }

/* null input argument ; some output struct */
#define CLNK_CTL_GET_0(ctx, cmd, out_type, outv) \
    { \
        ClnkLio_t io; \
        if (outv == NULL) \
            return CLNK_ERR; \
        io.in = NULL; \
        io.in_len = 0; \
        io.out = (uint32_t *) outv; \
        io.out_len = sizeof(out_type); \
        return(ClnkLio_SendCmd(ctx, cmd, &io, -1)); \
    }

/* some input struct argument ; null output value */
#define CLNK_CTL_SET(ctx, cmd, in_type, inv) \
    { \
        ClnkLio_t io; \
        if (inv == NULL) \
            return CLNK_ERR; \
        io.in = (void *) inv; \
        io.in_len = sizeof(in_type); \
        io.out = NULL; \
        io.out_len = 0; \
        return(ClnkLio_SendCmd(ctx, cmd, &io, -1)); \
    }
/* only command, null arguments */
#define CLNK_CTL_DO(ctx, cmd) \
    { \
        ClnkLio_t io; \
        io.in = NULL; \
        io.in_len = 0; \
        io.out = NULL; \
        io.out_len = 0; \
        return(ClnkLio_SendCmd(ctx, cmd, &io, -1)); \
    }
/* input arguments and out struct should be provided*/
#define CLNK_CTL_SET_GET(ctx, cmd, in_type,inv,out_type,outv) \
    { \
        ClnkLio_t io; \
        if ((inv == NULL)||(outv == NULL)) \
            return CLNK_ERR; \
        io.in = (void *) inv; \
        io.in_len = sizeof(in_type); \
        io.out = (uint32_t *)outv; \
        io.out_len = sizeof(out_type); \
        return(ClnkLio_SendCmd(ctx, cmd, &io, -1)); \
    }


typedef enum
{
  /// Ethernet commands
  CLNK_MBX_ETH_RESET_CMD                        = 0x00,   /*!< Alias: ETH_MB_RESET,                             */
  CLNK_MBX_ETH_ALLOC_TX_FIFO_CMD                = 0x01,   /*!< Alias: ETH_MB_ALLOCATE_TX_FIFO,                  */
  CLNK_MBX_ETH_ALLOC_RX_FIFO_CMD                = 0x02,   /*!< Alias: ETH_MB_ALLOCATE_RX_FIFO,                  */
  CLNK_MBX_ETH_JOIN_MCAST_CMD                   = 0x03,   /*!< Alias: ETH_MB_JOIN_MULTICAST,                    */
  CLNK_MBX_ETH_LEAVE_MCAST_CMD                  = 0x04,   /*!< Alias: ETH_MB_LEAVE_MULTICAST,                   */
  CLNK_MBX_ETH_REGISTER_MCAST_CMD               = 0x05,   /*!< Alias: ETH_MB_REGISTER_MULTICAST,                */
  CLNK_MBX_ETH_GET_STATS_CMD                    = 0x06,   /*!< Alias: ETH_MB_QUERY_RCV_STATS,                   */
  CLNK_MBX_ETH_SET_RCV_MODE_CMD                 = 0x07,   /*!< Alias: ETH_MB_SET_RCV_MODE,                      */
  CLNK_MBX_ETH_GET_RCV_MODE_CMD                 = 0x08,   /*!< Alias: ETH_MB_GET_RCV_MODE,                      */
  CLNK_MBX_ETH_PUB_UCAST_CMD                    = 0x09,   /*!< Alias: ETH_MB_PUBLISH_UNICAST_ADDR,              */
  CLNK_MBX_ETH_DEREGISTER_MCAST_CMD             = 0x0a,   /*!< Alias: ETH_MB_DEREGISTER_MULTICAST,              */
  CLNK_MBX_ETH_DATA_BUF_CMD                     = 0x0b,   /*!< Alias: ETH_MB_DATA_BUF_CMD,                      */
  CLNK_MBX_ETH_GET_STATUS_CMD                   = 0x0c,   /*!< Alias: ETH_MB_GET_STATUS,                        */
  CLNK_MBX_ETH_SET_SW_CONFIG_CMD                = 0x0d,   /*!< Alias: ETH_MB_SET_SW_CONFIG,                     */
  CLNK_MBX_ETH_ECHO_PROFILE_PROBE_RESPONSE      = 0x0e,   /*!< Alias: ETH_MB_ECHO_PROFILE_PROBE_RESPONSE,       */
  CLNK_MBX_ETH_GET_CMD                          = 0x0f,   /*!< Alias: ETH_MB_GET_CMD,                           */
  CLNK_MBX_ETH_SET_CMD                          = 0x10,   /*!< Alias: ETH_MB_SET_CMD,                           */
  CLNK_MBX_ETH_CLINK_ACCEPT_SMALL_ROUTED_MESSAGE = 0x11,  /*!< Alias: ETH_MB_CLINK_ACCEPT_SMALL_ROUTED_MESSAGE, */
  CLNK_MBX_ETH_SET_BEACON_POWER_LEVEL_CMD       = 0x12,   /*!< Alias: ETH_MB_SET_BEACON_POWER_LEVEL_CMD,        */
  CLNK_MBX_ETH_QOS_ASYNC_REQ_BLOB               = 0x13,   /*!< Alias: ETH_MB_QOS_ASYNC_REQ_BLOB,                */
  CLNK_MBX_ETH_QFM_RESP_BLOB                    = 0x14,   /*!< Alias: ETH_MB_QFM_RESP_BLOB,                     */
  CLNK_MBX_ETH_SET_MIXED_MODE_ACTIVE            = 0x15,   /*!< Alias: ETH_MB_SET_MIXED_MODE_ACTIVE,             */
  CLNK_MBX_ETH_GET_MIXED_MODE_ACTIVE            = 0x16,   /*!< Alias: ETH_MB_GET_MIXED_MODE_ACTIVE,             */
  CLNK_MBX_ETH_QFM_CAM_DONE                     = 0x17,   /*!< Alias: ETH_MB_QFM_CAM_DONE,                      */
  // MPEG commands
  CLNK_MBX_MPEG_CREATE_INPUT_CHANNEL_CMD        = 0x20,
  CLNK_MBX_MPEG_JOIN_CHANNEL_CMD                = 0x21,
  CLNK_MBX_MPEG_JOIN_AS_PASSIVE_LISTENER_CMD    = 0x22,
  CLNK_MBX_MPEG_SUBSCRIPTION_REQUEST_CMD        = 0x23,
  CLNK_MBX_MPEG_MODIFY_INPUT_CHANNEL_CMD        = 0x24,
  CLNK_MBX_MPEG_MODIFY_OUTPUT_CHANNEL_CMD       = 0x25,
  CLNK_MBX_MPEG_LEAVE_INPUT_CHANNEL_CMD         = 0x26,
  CLNK_MBX_MPEG_LEAVE_OUTPUT_CHANNEL_CMD        = 0x27,
  CLNK_MBX_MPEG_CREATE_OUTPUT_CHANNEL_CMD       = 0x28,
  CLNK_MBX_MPEG_RESERVED_CMD_FROM               = 0x29,
  CLNK_MBX_MPEG_RESERVED_CMD_TO                 = 0x2F,
  CLNK_MBX_MPEG_READ_TRANSMIT_STATS_CMD         = 0x30,
  CLNK_MBX_MPEG_READ_RECEIVE_STATS_CMD          = 0x31,
  CLNK_MBX_MPEG_SUBSCRIPTION_REPORT_REQUEST_CMD = 0x32,

  CLNK_MBX_MAX_CMD  /*!< This must always be last */
} clnk_mbx_cmd_codes_t;
#define DRV_CLNK_CTL             55

typedef struct
{
  uint32_t    cmd;
  void*       param1;
  void*       param2;
  void*       param3;
} IfrDataStruct;



/// Macros to package up the message properly

/// no input variable, simple response expected
#define CLNK_DRV_GET_0(io, out_type, outv) \
    { \
        if (outv == NULL) \
            return CLNK_ERR; \
        io.in = NULL; \
        io.in_len = 0; \
        io.out = (uint32_t *) outv; \
        io.out_len = sizeof(out_type); \
    }

/// 1 input variable, simple response expected
#define CLNK_DRV_GET_1(io, out_type, outv, inv) \
    { \
        if (outv == NULL) \
            return CLNK_ERR; \
        io.in = (uint32_t *)(unsigned long) inv; \
        io.in_len = 0; \
        io.out = (uint32_t *) outv; \
        io.out_len = sizeof(out_type); \
    }

/// 1 input variable, no response expected
#define CLNK_DRV_SET(io, in_type, inv) \
    { \
        if (inv == NULL) \
            return CLNK_ERR; \
        io.in = (void *) inv; \
        io.in_len = sizeof(in_type); \
        io.out = NULL; \
        io.out_len = 0; \
    }

/// no input variable, no response expected
#define CLNK_DRV_DO(io) \
    { \
        io.in = NULL; \
        io.in_len = 0; \
        io.out = NULL; \
        io.out_len = 0; \
    }
/// 1 input variable, complex response expected
#define CLNK_DRV_SET_GET(io, in_type, inv, out_type, outv) \
    { \
        if ((inv == NULL)||(outv == NULL)) \
            return CLNK_ERR; \
        io.in = (void *) inv; \
        io.in_len = sizeof(in_type); \
        io.out = (uint32_t *)outv; \
        io.out_len = sizeof(out_type); \
    }

#endif /* __CLNK_API_LIO_H__ */
