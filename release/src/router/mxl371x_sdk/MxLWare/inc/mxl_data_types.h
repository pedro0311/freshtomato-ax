/*****************************************************************************************   
 *
 * FILE NAME          : mxl_data_types.h
 * 
 *
 *
 * DATE CREATED       : Sep/14/2016
 *
 * LAST MODIFIED      : 
 *
 * DESCRIPTION        : This file contains MaxLinear's data types.
 *                      Instead of using ANSI C data type directly in source code
 *                      All modules should include this header file.
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

#ifndef __MXL_DATA_TYPES_H__
#define __MXL_DATA_TYPES_H__


/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
 ****************************************************************************************/

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif


/*****************************************************************************************
    Macros
 ****************************************************************************************/

/*****************************************************************************************
    User-Defined Types (Typedefs)
 ****************************************************************************************/

typedef float               real32_t;
typedef double              real64_t;

/*****************************************************************************************
    Global Variable Declarations
 ****************************************************************************************/

/*****************************************************************************************
    Function Prototypes
 ****************************************************************************************/

#endif // __MXL_DATA_TYPES_H__
