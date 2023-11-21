/********************************************************************************************************
 * @file    app_cfg.h
 *
 * @brief   This is the header file for app_cfg
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

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


/**********************************************************************
 * Version configuration
 */
#include "version_cfg.h"

/**********************************************************************
 * Product Information
 */
/* Debug mode config */
#define	UART_PRINTF_MODE				0
#define USB_PRINTF_MODE         		0
//#define BAUDRATE 						1000000//1M

/* PM */
#define PM_ENABLE						1

/* PA */
#define PA_ENABLE						0

/* BDB */
#define TOUCHLINK_SUPPORT				0
#define FIND_AND_BIND_SUPPORT			0


/* Board define */
#define FLASH_CAP_SIZE_1M			1
#define CLOCK_SYS_CLOCK_HZ  		48000000
#include "board_ZP01-ZTU-V-1-1.h"

/* Voltage detect module */
/* If VOLTAGE_DETECT_ENABLE is set,
 * if MCU_CORE_8258 or MCU_CORE_8278 is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be in a floating state.
 */
#define VOLTAGE_DETECT_ENABLE						1
#define VOLTAGE_DETECT_ADC_PIN					GPIO_PC5

/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE						0

/* UART module */
#define	MODULE_UART_ENABLE							0

#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID || ZBHCI_UART)
	#define ZBHCI_EN								1
#endif


/**********************************************************************
 * ZCL cluster support setting //TODO review
 */
#define ZCL_IAS_ZONE_SUPPORT						1
/* disable poll control - not supported in z2m */
// #define ZCL_POLL_CTRL_SUPPORT						1
#define ZCL_OTA_SUPPORT								1
#define ZCL_POWER_CFG_SUPPORT                       1

#define MY_OTA_QUERY_RATE_COEFF                    384 //in 5*60 seconds - 1day
#define MY_POLL_RATE_COEFF                       10800 // in 4quarterseconds = 1 seconds - 4h
#define UPDATE_ATTRIBUTES_INTERVAL            10800000 // in milliseconds - 4h


/**********************************************************************
 * Stack configuration
 */
#include "stack_cfg.h"


/**********************************************************************
 * EV configuration
 */
typedef enum{
	EV_POLL_ED_DETECT,
	EV_POLL_HCI,
    EV_POLL_IDLE,
	EV_POLL_MAX,
}ev_poll_e;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
