/********************************************************************************************************
 * @file    board_ZP01-ZTU-V-1-1.h
 *
 * @brief   This is the header file for board_ZP-ZTU-V-1-1 motion sensor
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

// BUTTON
// Net Key button
#define	BUTTON1 					GPIO_PC0
#define PC0_FUNC					AS_GPIO
#define PC0_OUTPUT_ENABLE			0
#define PC0_INPUT_ENABLE			1
//#define	PULL_WAKEUP_SRC_PC0			PM_PIN_PULLUP_10K				pull-up is on board
#define	BUTTON1_PM_WAKEUP_LEVEL		PM_WAKEUP_LEVEL_LOW

// motion sensor
#define	BUTTON2 					GPIO_PD7
#define PD7_FUNC					AS_GPIO
#define PD7_OUTPUT_ENABLE			0
#define PD7_INPUT_ENABLE			1
//#define	PULL_WAKEUP_SRC_PD7			PM_PIN_PULLUP_10K				no need for pull-upfor PIR sensors
#define	BUTTON2_PM_WAKEUP_LEVEL		PM_WAKEUP_LEVEL_HIGH

//LED
#define LED1						GPIO_PB4
#define PB4_FUNC					AS_GPIO
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

//to disable led blink on wake ???
//#define PB4_DATA_OUT                0/1 ???

#define LED_ON						1
#define LED_OFF						0


// UART
#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PB1 //print
#endif

enum{
	VK_SW1 = 0x01
};

#define	KB_MAP_NORMAL	{\
		{VK_SW1,}, }

#define	KB_MAP_NUM		KB_MAP_NORMAL
#define	KB_MAP_FN		KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
