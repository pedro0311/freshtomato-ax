/*****************************************************************************************
 * FILE NAME          : ClnkLibDev.h
 *
 *
 * DATE CREATED       : Mar/15/2017
 * LAST MODIFIED      :
 *
 * DESCRIPTION        : c.LINK library device header file.
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
 * Copyright (c) 2017, MaxLinear, Inc.
 ****************************************************************************************/

#ifndef __CLNK_LIB_DEV_H__
#define __CLNK_LIB_DEV_H__
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *      Includes
 ******************************************************************************/
#include "ClnkLibApi.h"

/*******************************************************************************
 *      Constants
 ******************************************************************************/

/*******************************************************************************
 *      Macros
 ******************************************************************************/

/*****************************************************************************
 *      Types
 ******************************************************************************/

/*******************************************************************************
 *      Public/Private Data
 ******************************************************************************/

/*******************************************************************************
 *      Public/Private Functions
 ******************************************************************************/

int32_t ClnkLibDev_Open(ClnkLib_Handle_t *pHandle);
int32_t ClnkLibDev_OpenByName(ClnkLib_Handle_t *pHandle, char *pName);
int32_t ClnkLibDev_ListNames(ClnkLib_Name_t *ifNamePtr, int32_t max);
int32_t ClnkLibDev_GetName(ClnkLib_Handle_t *pHandle, ClnkLib_Name_t *pName);
int32_t ClnkLibDev_DisplayInterfaces();
void ClnkLibDev_Close(ClnkLib_Handle_t *pHandle);
int32_t ClnkLibDev_SendCmd(ClnkLib_Handle_t *pHandle, int cmd,
                           void *in_buf, uint32_t in_len,
                           void *out_buf, uint32_t out_len);

#ifdef __cplusplus
}
#endif
#endif /* __CLNK_LIB_DEV_H__ */

