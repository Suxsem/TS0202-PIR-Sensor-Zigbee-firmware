const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
const tz = require('zigbee-herdsman-converters/converters/toZigbee');
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const extend = require('zigbee-herdsman-converters/lib/extend');
const ota = require('zigbee-herdsman-converters/lib/ota');
const tuya = require('zigbee-herdsman-converters/lib/tuya');
const utils = require('zigbee-herdsman-converters/lib/utils');
const globalStore = require('zigbee-herdsman-converters/lib/store');
const e = exposes.presets;
const ea = exposes.access;

const definition = {
        fingerprint: [
        {
            modelID: 'My_TS0202',
            manufacturerName: 'Suxsem',
        },
    ],
    model: 'ZP01',
    vendor: 'Suxsem, ex TuYa',
    description: 'PIR motion sensor',
    fromZigbee: [fz.ias_occupancy_alarm_1, fz.ias_occupancy_alarm_1_report, fz.battery],
    toZigbee: [],
        configure: async (device, coordinatorEndpoint, logger) => {
                const firstEndpoint = device.getEndpoint(1);
                await reporting.bind(firstEndpoint, coordinatorEndpoint, ['genPowerCfg', 'ssIasZone']);
        },
    exposes: [e.occupancy(), e.battery(), e.battery_voltage(), e.battery_low()],
    ota: ota.zigbeeOTA,
};
module.exports = definition;
