const all = require("./index");
const googleTools = require("../tools/googleTools");
const wsTools = require("../tools/WsTools");
const _ = require("lodash");


let brightnessLight = _.cloneDeep(all.stuffTemplate);
brightnessLight.device_info.manufacturer = 'OpenSmartStuff';
brightnessLight.device_info.model = 'BrightnessLight';
brightnessLight.device_info.hwVersion = '0.1';
brightnessLight.device_info.swVersion = '0.1';
brightnessLight.type = all.types.LIGHT;


let onOffTrait = _.cloneDeep(all.traits.OnOff);
onOffTrait.commands.OnOff = async function (UUID, params) {
    let isOn = params.on;
    let brightness = params.brightness;
    const connectedStuffs = require('../Stuffs').connectedStuffs;
    if (connectedStuffs[UUID] == null)
        return {
            status: "ERROR",
            errorCode: 'deviceTurnedOff',
            online: false

        };

    let ws = connectedStuffs[UUID].ws;
    ws.send(JSON.stringify({type: "COMMAND", on: isOn}));
    let state = await wsTools.waitForResponse(UUID);
    if (state == null) {
        return {
            status: "ERROR",
            errorCode: 'deviceTurnedOff',
            online: false

        }
    } else {

        return {
            status: "SUCCESS",
            states: {
                online: true,
                on: isOn,
                brightness: brightness
            }

        }
    }

};

brightnessLight.traits.OnOff = onOffTrait;

let brightnessTrait = _.cloneDeep(all.traits.Brightness);
brightnessTrait.commands.BrightnessAbsolute = async function (inDb, params) {
    let UUID = inDb.uuid;
    let isOn = params.on;
    let brightness = params.brightness;
    const connectedStuffs = require('../Stuffs').connectedStuffs;
    if (connectedStuffs[UUID] == null)
        return {
            status: "ERROR",
            errorCode: 'deviceTurnedOff',
            online: false
        };

    let ws = connectedStuffs[UUID].ws;
    ws.send(JSON.stringify({type: "COMMAND", brightness: brightness}));
    let state = await wsTools.waitForResponse(UUID);
    if (state == null) {
        return {
            status: "ERROR",
            errorCode: 'deviceTurnedOff',
            online: false

        }
    } else {

        return {
            status: "SUCCESS",
            states: {
                online: true,
                on: isOn,
                brightness: brightness
            }

        }
    }
};
brightnessLight.traits.Brightness = brightnessTrait;


brightnessLight.getSync = function (stuffInDb) {

    let finalObject = {
        id: stuffInDb.id,
        type: googleTools.convertType(brightnessLight.type),
        name: {
            defaultNames: ["Light"],
            name: stuffInDb.name,
            nicknames: JSON.parse(stuffInDb.nicknames)
        },
        deviceInfo: brightnessLight.device_info,
        willReportState: false,
    };

    finalObject.traits = googleTools.getTraits(this);
    finalObject.attributes = googleTools.getAttributes(this);
    return finalObject;
};

brightnessLight.getGoogleState = async function (inDb) {
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

        return {
            online: true,
            on: state.on,
            brightness: state.brightness
        }
    }


};
module.exports = brightnessLight;

