const all = require("./index");
const googleTools = require("../tools/googleTools");
const wsTools = require("../tools/WsTools");
const _ = require("lodash");


let thermostat = _.cloneDeep(all.stuffTemplate);
thermostat.device_info.manufacturer = 'OpenSmartStuff';
thermostat.device_info.model = 'BME280 Sensor';
thermostat.device_info.hwVersion = '0.1';
thermostat.device_info.swVersion = '0.1';
thermostat.type = all.types.THERMOSTAT;


let TemperatureSetting = _.cloneDeep(all.traits.TemperatureSetting);
TemperatureSetting.attributes.queryOnlyTemperatureSetting = true;

thermostat.traits.TemperatureSetting = TemperatureSetting;


thermostat.getSync = function (stuffInDb) {

    let finalObject = {
        id: stuffInDb.id,
        type: googleTools.convertType(thermostat.type),
        name: {
            defaultNames: ["Light"],
            name: stuffInDb.name,
            nicknames: JSON.parse(stuffInDb.nicknames)
        },
        deviceInfo: thermostat.device_info,
        willReportState: false,
    };

    finalObject.traits = googleTools.getTraits(this);
    finalObject.attributes = googleTools.getAttributes(this);
    return finalObject;
};

thermostat.getGoogleState = async function (inDb) {
    let UUID = inDb.uuid;
    const connectedStuffs = require('../Stuffs').connectedStuffs;
    if (connectedStuffs[UUID] == null)
        return {
            online: false,
            state: "OFFLINE"
        };

    let ws = connectedStuffs[UUID].ws;
    ws.send(JSON.stringify({type: "GET_STATE"}));
    let state = await wsTools.waitForResponse(UUID);
    if (state == null) {
        return {
            online: false
        }
    } else {

        console.log("inside");
        return {
            online: true,
            thermostatTemperatureAmbient: state.temperature,
            thermostatHumidityAmbient: state.humidity
        }
    }


};
module.exports = thermostat;

