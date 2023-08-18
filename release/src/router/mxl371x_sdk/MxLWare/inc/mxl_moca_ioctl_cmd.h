/*****************************************************************************************   
 *
 * FILE NAME          : mxl_moca_ioctl_cmd.h
 * 
 *
 *
 * DATE CREATED       : Sep/20/2016
 *
 * LAST MODIFIED      : 
 *
 * DESCRIPTION        : Supported systems/debug IOCTL commands 
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

#ifndef __MXL_MOCA_IOCTL_CMD_H__
#define __MXL_MOCA_IOCTL_CMD_H__


/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
 ****************************************************************************************/


/*****************************************************************************************
    Macros
 ****************************************************************************************/

#define DEVPRIVATE                 0x89F0
#define SIOC_MXL_MOCA_CMD          (DEVPRIVATE+1) //Systems IOCTL for MaxLinear MoCA command 
#define SIOC_MXL_MOCA_DBG_GET_MEM  (DEVPRIVATE+2) //Debug IOCTL for extended SoC memory read operation.
#define SIOC_MXL_MOCA_DBG_SET_MEM  (DEVPRIVATE+3) //Debug IOCTL for extended SoC memory write operation.
#define SIOC_MXL_MOCA_START_DAEMON (DEVPRIVATE+4) //Systems IOCTL for start MoCA Daemon command


/*****************************************************************************************
    User-Defined Types (Typedefs)
 ****************************************************************************************/

// ECB OTP (One Time Program) Parameters
typedef struct
{
  char              acEcbModNum[30];
  char              acEcbSerNum[30];
  uint8_t           acClkMacAddr[6];
  uint8_t           acEthMacPort0[6];
  uint8_t           acEthMacPort1[6];
  uint8_t           acEthMacPort2[6];
#if MXLWARE_CONFIG_BOARD_ECA_5M_L3_MVL || MXLWARE_CONFIG_BOARD_ECA_8M_L3_MVL
  uint8_t           acCpuMacAddr[6];
#elif MXLWARE_CONFIG_BOARD_ECA_9M_L3_NXP
  uint8_t           acEthMacPort3[6];
  uint8_t           acEthMacPort4[6];
#endif
#if MXLWARE_CONFIG_BOARD_ECA_5M_L3_MVL || MXLWARE_CONFIG_BOARD_ECA_8M_L3_MVL || MXLWARE_CONFIG_BOARD_ECA_9M_L3_NXP
  uint32_t          acClnkFreqBand;
  uint32_t          acMiiMode;
  uint32_t          uiMasterMode;
  uint32_t          uiMacMode;
#endif
  uint32_t          acDiplexerCal;
  uint32_t          acRgmiiClkCfg;
  uint32_t          acHwCfg;
  uint32_t          uiCfgCrcNum;
} MXL_MOCA_DRV_SET_OTP_CONFIG_CMD_T;


/*****************************************************************************************
    Global Variable Declarations
 ****************************************************************************************/

/*****************************************************************************************
    Function Prototypes
 ****************************************************************************************/

#endif // __MXL_MOCA_IOCTL_CMD_H__
