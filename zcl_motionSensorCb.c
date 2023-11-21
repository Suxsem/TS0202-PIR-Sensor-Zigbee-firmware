/********************************************************************************************************
 * @file    zcl_motionSensorCb.c
 *
 * @brief   This is the source file for zcl_motionSensorCb
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
#ifdef ZCL_READ
static void motionSensor_zclReadRspCmd(u16 clusterId, zclReadRspCmd_t *pReadRspCmd);
#endif
#ifdef ZCL_WRITE
static void motionSensor_zclWriteRspCmd(u16 clusterId, zclWriteRspCmd_t *pWriteRspCmd);
static void motionSensor_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd);
#endif
#ifdef ZCL_REPORT
static void motionSensor_zclCfgReportCmd(u16 clusterId, zclCfgReportCmd_t *pCfgReportCmd);
static void motionSensor_zclCfgReportRspCmd(u16 clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void motionSensor_zclReportCmd(u16 clusterId, zclReportCmd_t *pReportCmd);
#endif
static void Sensor_zclDfltRspCmd(u16 clusterId, zclDefaultRspCmd_t *pDftRspCmd);

/**********************************************************************
 * GLOBAL VARIABLES
 */

/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_timer_event_t *identifyTimerEvt = NULL;
#endif

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      motionSensor_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void motionSensor_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
	u16 cluster = pInHdlrMsg->msg->indInfo.cluster_id;
	switch(pInHdlrMsg->hdr.cmd)
	{
#ifdef ZCL_READ
		case ZCL_CMD_READ_RSP:
			motionSensor_zclReadRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
#ifdef ZCL_WRITE
		case ZCL_CMD_WRITE_RSP:
			motionSensor_zclWriteRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_WRITE:
			motionSensor_zclWriteReqCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
#ifdef ZCL_REPORT
		case ZCL_CMD_CONFIG_REPORT:
			motionSensor_zclCfgReportCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_CONFIG_REPORT_RSP:
			motionSensor_zclCfgReportRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_REPORT:
			motionSensor_zclReportCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
		case ZCL_CMD_DEFAULT_RSP:
			Sensor_zclDfltRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		default:
			break;
	}
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      motionSensor_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void motionSensor_zclReadRspCmd(u16 clusterId, zclReadRspCmd_t *pReadRspCmd)
{
    printf("motionSensor_zclReadRspCmd\n");

}
#endif	/* ZCL_READ */

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      motionSensor_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void motionSensor_zclWriteRspCmd(u16 clusterId, zclWriteRspCmd_t *pWriteRspCmd)
{
    printf("motionSensor_zclWriteRspCmd\n");

}

/*********************************************************************
 * @fn      motionSensor_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void motionSensor_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd)
{
#ifdef ZCL_POLL_CTRL
	u8 numAttr = pWriteReqCmd->numAttr;
	zclWriteRec_t *attr = pWriteReqCmd->attrList;

	if(clusterId == ZCL_CLUSTER_GEN_POLL_CONTROL){
		for(s32 i = 0; i < numAttr; i++){
			if(attr[i].attrID == ZCL_ATTRID_CHK_IN_INTERVAL){
				motionSensor_zclCheckInStart();
				return;
			}
		}
	}
#endif
}
#endif	/* ZCL_WRITE */


/*********************************************************************
 * @fn      motionSensor_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void Sensor_zclDfltRspCmd(u16 clusterId, zclDefaultRspCmd_t *pDftRspCmd)
{
    printf("motionSensor_zclDfltRspCmd\n");

}

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      motionSensor_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void motionSensor_zclCfgReportCmd(u16 clusterId, zclCfgReportCmd_t *pCfgReportCmd)
{
    printf("motionSensor_zclCfgReportCmd\n");

}

/*********************************************************************
 * @fn      motionSensor_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void motionSensor_zclCfgReportRspCmd(u16 clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd)
{
    printf("motionSensor_zclCfgReportRspCmd\n");

}

/*********************************************************************
 * @fn      motionSensor_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void motionSensor_zclReportCmd(u16 clusterId, zclReportCmd_t *pReportCmd)
{
    printf("motionSensor_zclReportCmd\n");

}
#endif	/* ZCL_REPORT */

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      motionSensor_basicCb
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t motionSensor_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_BASIC */

#ifdef ZCL_IDENTIFY
s32 motionSensor_zclIdentifyTimerCb(void *arg)
{
	if(g_zcl_identifyAttrs.identifyTime <= 0){
		identifyTimerEvt = NULL;
		return -1;
	}
	g_zcl_identifyAttrs.identifyTime--;
	return 0;
}

void motionSensor_zclIdentifyTimerStop(void)
{
	if(identifyTimerEvt){
		TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
	}
}

/*********************************************************************
 * @fn      motionSensor_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param	endpoint
 * @param	srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void motionSensor_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
	g_zcl_identifyAttrs.identifyTime = identifyTime;

	if(identifyTime == 0){
		motionSensor_zclIdentifyTimerStop();
		light_blink_stop();
	}else{
		if(!identifyTimerEvt){
			light_blink_start(identifyTime, 500, 500);
			identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(motionSensor_zclIdentifyTimerCb, NULL, 1000);
		}
	}
}

/*********************************************************************
 * @fn      motionSensor_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void motionSensor_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
	u8 effectId = pTriggerEffect->effectId;
	//u8 effectVariant = pTriggerEffect->effectVariant;

	switch(effectId){
		case IDENTIFY_EFFECT_BLINK:
			light_blink_start(1, 500, 500);
			break;
		case IDENTIFY_EFFECT_BREATHE:
			light_blink_start(15, 300, 700);
			break;
		case IDENTIFY_EFFECT_OKAY:
			light_blink_start(2, 250, 250);
			break;
		case IDENTIFY_EFFECT_CHANNEL_CHANGE:
			light_blink_start(1, 500, 7500);
			break;
		case IDENTIFY_EFFECT_FINISH_EFFECT:
			light_blink_start(1, 300, 700);
			break;
		case IDENTIFY_EFFECT_STOP_EFFECT:
			light_blink_stop();
			break;
		default:
			break;
	}
}

/*********************************************************************
 * @fn      motionSensor_zclIdentifyQueryRspCmdHandler
 *
 * @brief   Handler for ZCL Identify Query response command.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyRsp
 *
 * @return  None
 */
static void motionSensor_zclIdentifyQueryRspCmdHandler(u8 endpoint, u16 srcAddr, zcl_identifyRspCmd_t *identifyRsp)
{
#if FIND_AND_BIND_SUPPORT
	if(identifyRsp->timeout){
		findBindDst_t dstInfo;
		dstInfo.addr = srcAddr;
		dstInfo.endpoint = endpoint;

		bdb_addIdentifyActiveEpForFB(dstInfo);
	}
#endif
}

/*********************************************************************
 * @fn      motionSensor_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t motionSensor_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == MOTION_SENSOR_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_IDENTIFY:
					motionSensor_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
					break;
				case ZCL_CMD_TRIGGER_EFFECT:
					motionSensor_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
					break;
				default:
					break;
			}
		}else{
			if(cmdId == ZCL_CMD_IDENTIFY_QUERY_RSP){
				motionSensor_zclIdentifyQueryRspCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, (zcl_identifyRspCmd_t *)cmdPayload);
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_IDENTIFY */

#ifdef ZCL_IAS_ZONE
/*********************************************************************
 * @fn      motionSensor_zclIasZoneEnrollRspCmdHandler
 *
 * @brief   Handler for ZCL IAS ZONE Enroll response command.
 *
 * @param   pZoneEnrollRsp
 *
 * @return  None
 */
static void motionSensor_zclIasZoneEnrollRspCmdHandler(zoneEnrollRsp_t *pZoneEnrollRsp)
{
	printf("zclIasZoneEnrollRspCmdHandler code:%d zone_id:%d\n", pZoneEnrollRsp->code, pZoneEnrollRsp->zoneId);
	if (pZoneEnrollRsp->zoneId != ZCL_ZONE_ID_INVALID) {
		u8 zoneState;
		zoneState = ZONE_STATE_ENROLLED;
		zcl_setAttrVal(MOTION_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &(pZoneEnrollRsp->zoneId));
		zcl_setAttrVal(MOTION_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATE, &zoneState);
	}
}

/*********************************************************************
 * @fn      motionSensor_zclIasZoneInitNormalOperationModeCmdHandler
 *
 * @brief   Handler for ZCL IAS ZONE normal operation mode command.
 *
 * @param
 *
 * @return  status
 */
static status_t motionSensor_zclIasZoneInitNormalOperationModeCmdHandler(void)
{
	printf("zclIasZoneInitNormalOperationModeCmdHandler\n");
	u8 status = ZCL_STA_FAILURE;

	return status;
}

/*********************************************************************
 * @fn      motionSensor_zclIasZoneInitTestModeCmdHandler
 *
 * @brief   Handler for ZCL IAS ZONE test mode command.
 *
 * @param   pZoneInitTestMode
 *
 * @return  status
 */
static status_t motionSensor_zclIasZoneInitTestModeCmdHandler(zoneInitTestMode_t *pZoneInitTestMode)
{
	printf("zclIasZoneInitNormalOperationModeCmdHandler\n");
	u8 status = ZCL_STA_FAILURE;

	return status;
}

/*********************************************************************
 * @fn      motionSensor_iasZoneCb
 *
 * @brief   Handler for ZCL IAS Zone command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t motionSensor_iasZoneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dstEp == MOTION_SENSOR_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_ZONE_ENROLL_RSP:
					motionSensor_zclIasZoneEnrollRspCmdHandler((zoneEnrollRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_INIT_NORMAL_OPERATION_MODE:
					motionSensor_zclIasZoneInitNormalOperationModeCmdHandler();
					break;
				case ZCL_CMD_INIT_TEST_MODE:
					motionSensor_zclIasZoneInitTestModeCmdHandler((zoneInitTestMode_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return status;
}
#endif  /* ZCL_IAS_ZONE */

/*********************************************************************
 * @fn      motionSensor_powerCfgCb
 *
 * @brief   Handler for ZCL PowerCfg command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t motionSensor_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
//	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
//	}

	return ZCL_STA_SUCCESS;
}


#ifdef ZCL_POLL_CTRL
static ev_timer_event_t *zclFastPollTimeoutTimerEvt = NULL;
static ev_timer_event_t *zclCheckInTimerEvt = NULL;
static bool isFastPollMode = FALSE;

void motionSensor_zclCheckInCmdSend(void)
{
	printf("contactSensor_zclCheckInCmdSend()\n");
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
	dstEpInfo.dstEp = MOTION_SENSOR_ENDPOINT;
	dstEpInfo.profileId = HA_PROFILE_ID;

	zcl_pollCtrl_checkInCmd(MOTION_SENSOR_ENDPOINT, &dstEpInfo, TRUE);
}

s32 motionSensor_zclCheckInTimerCb(void *arg)
{
	printf("contactSensor_zclCheckInTimerCb()\n");
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if(!pPollCtrlAttr->chkInInterval){
		zclCheckInTimerEvt = NULL;
		return -1;
	}

	motionSensor_zclCheckInCmdSend();

	return 0;
}

void motionSensor_zclCheckInStart(void)
{
	printf("contactSensor_zclCheckInStart()\n");
	if(zb_bindingTblSearched(ZCL_CLUSTER_GEN_POLL_CONTROL, MOTION_SENSOR_ENDPOINT)){
		zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

		if(!zclCheckInTimerEvt){
			zclCheckInTimerEvt = TL_ZB_TIMER_SCHEDULE(motionSensor_zclCheckInTimerCb, NULL, pPollCtrlAttr->chkInInterval * POLL_RATE_QUARTERSECONDS);

			if(pPollCtrlAttr->chkInInterval){
				motionSensor_zclCheckInCmdSend();
			}
		}
	}
}

void motionSensor_zclSetFastPollMode(bool fastPollMode)
{
	printf("contactSensor_zclSetFastPollMode(), fastPollMode %d\n", fastPollMode);
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	isFastPollMode = fastPollMode;
	u32 pollRate = fastPollMode ? pPollCtrlAttr->shortPollInterval : pPollCtrlAttr->longPollInterval;

	zb_setPollRate(pollRate * POLL_RATE_QUARTERSECONDS);
	printf("motionSensor_zclSetFastPollMode - ");
}

s32 motionSensor_zclFastPollTimeoutCb(void *arg)
{
	motionSensor_zclSetFastPollMode(FALSE);

	zclFastPollTimeoutTimerEvt = NULL;
	return -1;
}

static status_t motionSensor_zclPollCtrlChkInRspCmdHandler(zcl_chkInRsp_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if(pCmd->startFastPolling){
		u32 fastPollTimeoutCnt = 0;

		if(pCmd->fastPollTimeout){
			if(pCmd->fastPollTimeout > pPollCtrlAttr->fastPollTimeoutMax){
				return ZCL_STA_INVALID_FIELD;
			}

			fastPollTimeoutCnt = pCmd->fastPollTimeout;

			if(zclFastPollTimeoutTimerEvt){
				TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
			}
		}else{
			if(!zclFastPollTimeoutTimerEvt){
				fastPollTimeoutCnt = pPollCtrlAttr->fastPollTimeout;
			}
		}

		if(!zclFastPollTimeoutTimerEvt && fastPollTimeoutCnt){
			motionSensor_zclSetFastPollMode(TRUE);

			zclFastPollTimeoutTimerEvt = TL_ZB_TIMER_SCHEDULE(motionSensor_zclFastPollTimeoutCb, NULL, fastPollTimeoutCnt * POLL_RATE_QUARTERSECONDS);
		}
	}else{
		//continue in normal operation and not required to go into fast poll mode.
	}

	return ZCL_STA_SUCCESS;
}

static status_t motionSensor_zclPollCtrlFastPollStopCmdHandler(void)
{
	if(!isFastPollMode){
		return ZCL_STA_ACTION_DENIED;
	}else{
		if(zclFastPollTimeoutTimerEvt){
			TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
		}
		motionSensor_zclSetFastPollMode(FALSE);
	}

	return ZCL_STA_SUCCESS;
}

static status_t motionSensor_zclPollCtrlSetLongPollIntervalCmdHandler(zcl_setLongPollInterval_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if((pCmd->newLongPollInterval >= 0x04) && (pCmd->newLongPollInterval <= 0x6E0000)
		&& (pCmd->newLongPollInterval <= pPollCtrlAttr->chkInInterval) && (pCmd->newLongPollInterval >= pPollCtrlAttr->shortPollInterval)){
		pPollCtrlAttr->longPollInterval = pCmd->newLongPollInterval;
		zb_setPollRate(pCmd->newLongPollInterval * POLL_RATE_QUARTERSECONDS);
		printf("motionSensor_zclPollCtrlSetLongPollIntervalCmdHandler - ");
	}else{
		return ZCL_STA_INVALID_VALUE;
	}

	return ZCL_STA_SUCCESS;
}

static status_t motionSensor_zclPollCtrlSetShortPollIntervalCmdHandler(zcl_setShortPollInterval_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if((pCmd->newShortPollInterval >= 0x01) && (pCmd->newShortPollInterval <= 0xff)
		&& (pCmd->newShortPollInterval <= pPollCtrlAttr->longPollInterval)){
		pPollCtrlAttr->shortPollInterval = pCmd->newShortPollInterval;
		zb_setPollRate(pCmd->newShortPollInterval * POLL_RATE_QUARTERSECONDS);
		printf("motionSensor_zclPollCtrlSetShortPollIntervalCmdHandler - ");
	}else{
		return ZCL_STA_INVALID_VALUE;
	}

	return ZCL_STA_SUCCESS;
}

/*********************************************************************
 * @fn      motionSensor_pollCtrlCb
 *
 * @brief   Handler for ZCL Poll Control command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t motionSensor_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dstEp == MOTION_SENSOR_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_CHK_IN_RSP:
					status = motionSensor_zclPollCtrlChkInRspCmdHandler((zcl_chkInRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_FAST_POLL_STOP:
					status = motionSensor_zclPollCtrlFastPollStopCmdHandler();
					break;
				case ZCL_CMD_SET_LONG_POLL_INTERVAL:
					status = motionSensor_zclPollCtrlSetLongPollIntervalCmdHandler((zcl_setLongPollInterval_t *)cmdPayload);
					break;
				case ZCL_CMD_SET_SHORT_POLL_INTERVAL:
					status = motionSensor_zclPollCtrlSetShortPollIntervalCmdHandler((zcl_setShortPollInterval_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return status;
}
#endif	/* ZCL_POLL_CTRL */


#endif  /* __PROJECT_TL_MOTION_SENSOR__ */
