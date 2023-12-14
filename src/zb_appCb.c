/********************************************************************************************************
 * @file    zb_appCb.c
 *
 * @brief   This is the source file for zb_appCb
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

#if (__PROJECT_TL_MOTION_SENSOR__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "motionSensor.h"
#include "app_ui.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork);
void zbdemo_bdbCommissioningCb(u8 status, void *arg);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime);


/**********************************************************************
 * LOCAL VARIABLES
 */
bdb_appCb_t g_zbDemoBdbCb = 
{
	zbdemo_bdbInitCb, 
	zbdemo_bdbCommissioningCb, 
	zbdemo_bdbIdentifyCb, 
	NULL
};

#ifdef ZCL_OTA
ota_callBack_t motionSensor_otaCb =
{
	motionSensor_otaProcessMsgHandler,
};
#endif

ev_timer_event_t *steerTimerEvt = NULL;
ev_timer_event_t *myRejoinBackoffTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */
s32 motionSensor_bdbNetworkSteerStart(void *arg){
	bdb_networkSteerStart();

	return -1;
}

s32 motionSensor_rejoinBacckoff(void *arg){
	if(zb_isDeviceFactoryNew()){
		myRejoinBackoffTimerEvt = NULL;
		return -1;
	}

	zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
    return 0;
}


/*********************************************************************
 * @fn      zbdemo_bdbInitCb
 *
 * @brief   application callback for bdb initiation
 *
 * @param   status - the status of bdb init BDB_INIT_STATUS_SUCCESS or BDB_INIT_STATUS_FAILURE
 *
 * @param   joinedNetwork  - 1: node is on a network, 0: node isn't on a network
 *
 * @return  None
 */
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork){
	if(status == BDB_INIT_STATUS_SUCCESS){
		/*
		 * for non-factory-new device:
		 * 		load zcl data from NV, start poll rate, start ota query, bdb_networkSteerStart
		 *
		 * for factory-new device:
		 * 		steer a network
		 *
		 */
		if(joinedNetwork){
			zb_setPollRate(POLL_RATE*MY_POLL_RATE_COEFF);
		    updateAttributesCb(NULL);

#ifdef ZCL_OTA
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL*MY_OTA_QUERY_RATE_COEFF);
#endif

#ifdef ZCL_POLL_CTRL
			motionSensor_zclCheckInStart();
#endif
		}else{
			u16 jitter = 0;
			do{
				jitter = zb_random() % 0x0fff;
			}while(jitter == 0);
			if(steerTimerEvt){
				TL_ZB_TIMER_CANCEL(&steerTimerEvt);
			}
			steerTimerEvt = TL_ZB_TIMER_SCHEDULE(motionSensor_bdbNetworkSteerStart, NULL, jitter);
		}

	}else{
		if(joinedNetwork){
			if(!myRejoinBackoffTimerEvt){
				myRejoinBackoffTimerEvt = TL_ZB_TIMER_SCHEDULE(motionSensor_rejoinBacckoff, NULL, 60 * 1000);
			}
		}
	}
}

/*********************************************************************
 * @fn      zbdemo_bdbCommissioningCb
 *
 * @brief   application callback for bdb commissioning
 *
 * @param   status - the status of bdb commissioning
 *
 * @param   arg
 *
 * @return  None
 */
void zbdemo_bdbCommissioningCb(u8 status, void *arg){
	switch(status){
		case BDB_COMMISSION_STA_SUCCESS:
			light_blink_start(2, 200, 200);

			zb_setPollRate(POLL_RATE*MY_POLL_RATE_COEFF);

			if(steerTimerEvt){
				TL_ZB_TIMER_CANCEL(&steerTimerEvt);
			}

			if(myRejoinBackoffTimerEvt){
				TL_ZB_TIMER_CANCEL(&myRejoinBackoffTimerEvt);
			}

		    updateAttributesCb(NULL);

#ifdef ZCL_POLL_CTRL
			motionSensor_zclCheckInStart();
#endif
#ifdef ZCL_OTA
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL*MY_OTA_QUERY_RATE_COEFF);
#endif
			break;
		case BDB_COMMISSION_STA_IN_PROGRESS:
			break;
		case BDB_COMMISSION_STA_NOT_AA_CAPABLE:
			break;
		case BDB_COMMISSION_STA_NO_NETWORK:
		case BDB_COMMISSION_STA_TCLK_EX_FAILURE:
		case BDB_COMMISSION_STA_TARGET_FAILURE:
			{
				u16 jitter = 0;
				do{
					jitter = zb_random() % 0x0fff;
				}while(jitter == 0);
				if(steerTimerEvt){
					TL_ZB_TIMER_CANCEL(&steerTimerEvt);
				}
				steerTimerEvt = TL_ZB_TIMER_SCHEDULE(motionSensor_bdbNetworkSteerStart, NULL, jitter);
			}
			break;
		case BDB_COMMISSION_STA_FORMATION_FAILURE:
			break;
		case BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE:
			break;
		case BDB_COMMISSION_STA_BINDING_TABLE_FULL:
			break;
		case BDB_COMMISSION_STA_NO_SCAN_RESPONSE:
			break;
		case BDB_COMMISSION_STA_NOT_PERMITTED:
			break;
		case BDB_COMMISSION_STA_PARENT_LOST:
			//zb_rejoinSecModeSet(REJOIN_INSECURITY);
			zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
			break;
		case BDB_COMMISSION_STA_REJOIN_FAILURE:
			if(!myRejoinBackoffTimerEvt){
				myRejoinBackoffTimerEvt = TL_ZB_TIMER_SCHEDULE(motionSensor_rejoinBacckoff, NULL, 60 * 1000);
			}
			break;
		default:
			break;
	}
}


extern void motionSensor_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime){
	motionSensor_zclIdentifyCmdHandler(endpoint, srcAddr, identifyTime);
}



#ifdef ZCL_OTA
void motionSensor_otaProcessMsgHandler(u8 evt, u8 status)
{
	if(evt == OTA_EVT_START){
		if(status == ZCL_STA_SUCCESS){
			zb_setPollRate(QUEUE_POLL_RATE);
		}
	}else if(evt == OTA_EVT_COMPLETE){
		zb_setPollRate(POLL_RATE*MY_POLL_RATE_COEFF);

		if(status == ZCL_STA_SUCCESS){
			ota_mcuReboot();
		}else{
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL*MY_OTA_QUERY_RATE_COEFF);
		}
	}else if(evt == OTA_EVT_IMAGE_DONE){
		zb_setPollRate(POLL_RATE*MY_POLL_RATE_COEFF);
	}
}
#endif

/*********************************************************************
 * @fn      motionSensor_leaveCnfHandler
 *
 * @brief   Handler for ZDO Leave Confirm message.
 *
 * @param   pRsp - parameter of leave confirm
 *
 * @return  None
 */
void motionSensor_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf)
{
    if(pLeaveCnf->status == SUCCESS){
    	//SYSTEM_RESET();

		if(myRejoinBackoffTimerEvt){
			TL_ZB_TIMER_CANCEL(&myRejoinBackoffTimerEvt);
		}
    }
}

/*********************************************************************
 * @fn      motionSensor_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pInd - parameter of leave indication
 *
 * @return  None
 */
void motionSensor_leaveIndHandler(nlme_leave_ind_t *pLeaveInd)
{
    //printfArray(pLeaveInd->device_address, 8);
}

s32 updateAttributesCb(void *arg) {
	u16 voltage, percentage;
	u8 converted_voltage, percentage2;
	voltage = drv_get_adc_data();
	g_sensorAppCtx.batteryLow = voltage < 2500;
	//printf("voltage %d\n", voltage);
	converted_voltage = (u8)(voltage/100);
	percentage = ((voltage - BATTERY_SAFETY_THRESHOLD)/4);
	if (percentage > 0xc8) percentage=0xc8;
	percentage2 = (u8)percentage;
	//printf("converted voltage %d diff %d", converted_voltage, (voltage - BATTERY_SAFETY_THRESHOLD));
	//printf(" , percentage2 %d\n", percentage2);
	zcl_setAttrVal(MOTION_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE, &converted_voltage);
	zcl_setAttrVal(MOTION_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, &percentage2);

	zclAttrInfo_t *pAttrEntry;
	pAttrEntry = zcl_findAttribute(MOTION_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS);
	if (g_sensorAppCtx.batteryLow) {
		*pAttrEntry->data |= 1<<3;
	} else {
		*pAttrEntry->data &= ~(1<<3);
	}
	zcl_setAttrVal(MOTION_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, pAttrEntry->data);

	if(zb_isDeviceJoinedNwk()){
		reportAttrTimerCb();
	}

	return 0;
}

#endif  /* __PROJECT_TL_MOTION_SENSOR__ */
