/********************************************************************************************************
 * @file    motionSensor.c
 *
 * @brief   This is the source file for motionSensor
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
#if ZBHCI_EN
#include "zbhci.h"
#endif


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
app_ctx_t g_sensorAppCtx;


#ifdef ZCL_OTA
extern ota_callBack_t motionSensor_otaCb;

//running code firmware information
ota_preamble_t motionSensor_otaInfo = {
	.fileVer 			= FILE_VERSION,
	.imageType 			= IMAGE_TYPE,
	.manufacturerCode 	= MANUFACTURER_CODE_TELINK,
};
#endif


//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
	bdb_zdoStartDevCnf,//start device cnf cb
	NULL,//reset cnf cb
	NULL,//device announce indication cb
	motionSensor_leaveIndHandler,//leave ind cb
	motionSensor_leaveCnfHandler,//leave cnf cb
	NULL,//nwk update ind cb
	NULL,//permit join ind cb
	NULL,//nlme sync cnf cb
	NULL,//tc join ind cb
	NULL,//tc detects that the frame counter is near limit
};


/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
	.linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
	.linkKey.tcLinkKey.key = (u8 *)tcLinkKeyCentralDefault,       		//can use unique link key stored in NV

	.linkKey.distributeLinkKey.keyType = MASTER_KEY,
	.linkKey.distributeLinkKey.key = (u8 *)linkKeyDistributedMaster,  	//use linkKeyDistributedCertification before testing

	.linkKey.touchLinkKey.keyType = MASTER_KEY,
	.linkKey.touchLinkKey.key = (u8 *)touchLinkKeyMaster,   			//use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
	.touchlinkEnable = 1,												/* enable touch-link */
#else
	.touchlinkEnable = 0,												/* disable touch-link */
#endif
	.touchlinkChannel = DEFAULT_CHANNEL, 								/* touch-link default operation channel for target */
	.touchlinkLqiThreshold = 0xA0,			   							/* threshold for touch-link scan req/resp command */
};

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */
drv_pm_pinCfg_t g_sensorPmCfg[] = {
	{
		BUTTON1,
		BUTTON1_PM_WAKEUP_LEVEL
	},
	{
		BUTTON2,
		BUTTON2_PM_WAKEUP_LEVEL
	}
};
#endif
/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
	printf("stack_init - ");
	zb_init();
	zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{
#if ZCL_POLL_CTRL_SUPPORT
	af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_PERIODICALLY);
#else
	af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_WHEN_STIMULATED);
#endif

    /* Initialize ZCL layer */
	/* Register Incoming ZCL Foundation command/response messages */
	zcl_init(motionSensor_zclProcessIncomingMsg);

	/* Register endPoint */
	af_endpointRegister(MOTION_SENSOR_ENDPOINT, (af_simple_descriptor_t *)&motionSensor_simpleDesc, zcl_rx_handler, NULL);

	/* Init attributes and reporting table */
	zcl_reportingTabInit();

	/* Register ZCL specific cluster information */
	zcl_register(MOTION_SENSOR_ENDPOINT, MOTION_SENSOR_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_motionSensorClusterList);

#if ZCL_OTA_SUPPORT
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&motionSensor_simpleDesc, &motionSensor_otaInfo, &motionSensor_otaCb);
#endif
}



void led_init(void)
{
	light_init();
}

bool readSensorStatus() {
	bool newSensorStatus = drv_gpio_read(BUTTON2);
	if (newSensorStatus != g_sensorAppCtx.sensorStatus) {
		g_sensorAppCtx.sensorStatus = newSensorStatus;
		u16 attrValue = g_sensorAppCtx.sensorStatus ? ZONE_STATUS_BIT_ALARM1 : 0;
		zcl_setAttrVal(MOTION_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*)&attrValue);
		zclAttrInfo_t *pAttrEntry;
		pAttrEntry = zcl_findAttribute(MOTION_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS);

		return true;
	}
	return false;
}

void syncSensorStatus() {
	if(zb_isDeviceJoinedNwk()){

		if (g_sensorAppCtx.sensorStatus) {
			light_blink_start(1, 500, 0);
		}

		epInfo_t dstEpInfo;
		memset((u8 *)&dstEpInfo, 0, sizeof(epInfo_t));

		dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEpInfo.dstEp = MOTION_SENSOR_ENDPOINT;
		dstEpInfo.dstAddr.shortAddr = 0x0000;
		dstEpInfo.profileId = HA_PROFILE_ID;

		zoneStatusChangeNoti_t statusChangeNotification;

		statusChangeNotification.zoneStatus = g_sensorAppCtx.sensorStatus ? ZONE_STATUS_BIT_ALARM1 : 0;
		statusChangeNotification.extStatus = 0;
		statusChangeNotification.zoneId = ZCL_ZONE_ID_INVALID;
		statusChangeNotification.delay = 0;

		if (g_sensorAppCtx.batteryLow) {
			statusChangeNotification.zoneStatus |= 1<<3;
		} else {
			statusChangeNotification.zoneStatus &= ~(1<<3);
		}

		status_t result = zcl_iasZone_statusChangeNotificationCmd(MOTION_SENSOR_ENDPOINT, &dstEpInfo, TRUE, &statusChangeNotification);
		if (result == 0) {
			g_sensorAppCtx.remoteSensorStatus = g_sensorAppCtx.sensorStatus;
		}
	}
}

void app_task(void)
{

	app_key_handler();
	if (g_sensorAppCtx.sensorStatus != g_sensorAppCtx.remoteSensorStatus) {
		syncSensorStatus();
	}

#if PM_ENABLE
	if(!readSensorStatus() && bdb_isIdle() && !g_sensorAppCtx.keyPressed){
		drv_pm_lowPowerEnter();
	}
#else
	readSensorStatus();
#endif

}

static void motionSensorSysException(void)
{
#if 1
	SYSTEM_RESET();
#else
	light_on();
	while(1);
#endif
}

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
	readSensorStatus();
	printf("MotionSensorStatus: %d - ", g_sensorAppCtx.sensorStatus);

//	printf("user init(%d)\n", isRetention);
	/* Initialize LEDs*/
	led_init();

#if PA_ENABLE
	rf_paInit(PA_TX, PA_RX);
#endif

#if ZBHCI_EN
	zbhciInit();
#endif

#if PM_ENABLE
	g_sensorPmCfg[1].wakeupLevel = g_sensorAppCtx.sensorStatus ? !BUTTON2_PM_WAKEUP_LEVEL : BUTTON2_PM_WAKEUP_LEVEL;
	drv_pm_wakeupPinConfig(g_sensorPmCfg, sizeof(g_sensorPmCfg)/sizeof(drv_pm_pinCfg_t));
#endif

	if(!isRetention){
		/* Initialize Stack */
		stack_init();

		/* Initialize user application */
		user_app_init();

		/* Register except handler for test */
		sys_exceptHandlerRegister(motionSensorSysException);

		/* User's Task */
#if ZBHCI_EN
		ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
		ev_on_poll(EV_POLL_IDLE, app_task);

		/* Load the pre-install code from flash */
		if(bdb_preInstallCodeLoad(&g_sensorAppCtx.tcLinkKey.keyType, g_sensorAppCtx.tcLinkKey.key) == RET_OK){
			g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = g_sensorAppCtx.tcLinkKey.keyType;
			g_bdbCommissionSetting.linkKey.tcLinkKey.key = g_sensorAppCtx.tcLinkKey.key;
		}

	    /* Set default reporting configuration */
	    u8 reportableChange = 0x00;
	    bdb_defaultReportingCfg(MOTION_SENSOR_ENDPOINT, HA_PROFILE_ID, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS,
	    						1, 1, (u8 *)&reportableChange);
	    bdb_defaultReportingCfg(MOTION_SENSOR_ENDPOINT, HA_PROFILE_ID, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE,
	    	    						1, 1, (u8 *)&reportableChange);
	    bdb_defaultReportingCfg(MOTION_SENSOR_ENDPOINT, HA_PROFILE_ID, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING,
	    	    						1, 1, (u8 *)&reportableChange);

		/* Initialize BDB */
		u8 repower = drv_pm_deepSleep_flag_get() ? 0 : 1;
		bdb_init((af_simple_descriptor_t *)&motionSensor_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, repower);

		// Periodic reporting
	    g_sensorAppCtx.timerUpdateAttributesCbEvt = TL_ZB_TIMER_SCHEDULE(updateAttributesCb, NULL, UPDATE_ATTRIBUTES_INTERVAL);

		if(zb_isDeviceJoinedNwk()){
			updateAttributesCb(NULL);
	    }

	}else{
		/* Re-config phy when system recovery from deep sleep with retention */
		mac_phyReconfig();
	}
}

#endif  /* __PROJECT_TL_MOTION_SENSOR__ */
