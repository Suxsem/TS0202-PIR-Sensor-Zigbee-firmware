/********************************************************************************************************
 * @file    version_cfg.h
 *
 * @brief   This is the header file for version_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#pragma once

#include "../common/comm_cfg.h"

#define CHIP_TYPE					TLSR_8258_1M

#define APP_RELEASE							0x10//app release 1.0
#define APP_BUILD							0x04//app build 02
#define STACK_RELEASE						0x30//stack release 3.0
#define STACK_BUILD							0x01//stack build 01
#define HARDWARE_REV                        0x01

#ifndef ZCL_BASIC_MFG_NAME
	#define ZCL_BASIC_MFG_NAME          {6,'S','u','x','s','e','m'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
	#define ZCL_BASIC_MODEL_ID          {9,'M','y','_','T','S','0','2','0','2'}
#endif
#ifndef ZCL_BASIC_DATE_CODE
	#define ZCL_BASIC_DATE_CODE    {8,'2','0','2','3','1','1','0','5'}
#endif
#ifndef ZCL_BASIC_LOC_DESC
	#define ZCL_BASIC_LOC_DESC     {7,'U','N','K','N','O','W','N'}
#endif
#ifndef ZCL_BASIC_BUILD_ID
	#define ZCL_BASIC_BUILD_ID     {10,'0','0','0','0','0','0','0','0','0','1'}
#endif

#ifndef ZCL_BASIC_SW_BUILD_ID //max 16 chars
	#define ZCL_BASIC_SW_BUILD_ID       {8,'v',(APP_RELEASE >> 4) + 0x30,'.',APP_BUILD + 0x30,'s',(STACK_RELEASE >> 4) + 0x30,'.',STACK_BUILD + 0x30}
#endif

/*********************************************************************************************
 * During OTA upgrade, the upgraded device will check the rules of the following three fields.
 * Refer to ZCL OTA specification for details.
 */
#define MANUFACTURER_CODE_TELINK           	0x1141//Telink ID
#define	IMAGE_TYPE							((CHIP_TYPE << 8) | IMAGE_TYPE_MOTION_SENSOR_0202)
#define	FILE_VERSION					  	((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)

/* Pre-compiled link configuration. */
#define IS_BOOT_LOADER_IMAGE				0
#define RESV_FOR_APP_RAM_CODE_SIZE			0
#define IMAGE_OFFSET						APP_IMAGE_ADDR
