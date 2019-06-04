const all = require("./index");
const googleTools = require("../tools/googleTools");
const wsTools = require("../tools/WsTools");
const _ = require ("lodash");


let onOffLight = _.cloneDeep(all.stuffTemplate);
let onOffTrait = _.cloneDeep(all.traits.OnOff);
onOffLight.device_info.manufacturer = 'OpenSmartStuff';
onOffLight.device_info.model = 'OnOffLight';
onOffLight.device_info.hwVersion = '0.1';
onOffLight.device_info.swVersion = '0.1';
onOffLight.type = all.types.LIGHT;


onOffTrait.commands.OnOff = async function (inDb, params) {
    let UUID = inDb.uuid;
    let option = JSON.parse(inDb.option);
    let isOn = params.on;
    const connectedStuffs = require('../Stuffs').connectedStuffs;
    if (connectedStuffs[UUID] == null)
        return {
            status: "OFFLINE",
            errorCode: 'deviceTurnedOff',
            online: false
        };

    let ws = connectedStuffs[UUID].ws;
    ws.send(JSON.stringify({type: "COMMAND", on: isOn, option : option}));
    let state = await wsTools.waitForResponse(UUID);
    if (state == null) {
        return {
            status: "OFFLINE",
            errorCode: 'deviceTurnedOff',
            online: false

        }
    } else {

        return {
            status: "SUCCESS",
            states: {
                online: true,
                on: isOn
            }

        }
    }

};

onOffLight.traits.OnOff = onOffTrait;


onOffLight.getSync = function (stuffInDb) {

    let finalObject = {
        id: stuffInDb.id,
        type: googleTools.convertType(onOffLight.type),
        name: {
            defaultNames: ["Light"],
            name : stuffInDb.name,
            nicknames: JSON.parse(stuffInDb.nicknames)
        },
        deviceInfo: onOffLight.device_info,
        willReportState: false,
    };

    finalObject.traits = googleTools.getTraits(this);
    finalObject.attributes =  googleTools.getAttributes(this);
    return finalObject;
};

onOffLight.getGoogleState = async function (inDb) {
    let UUID = inDb.uuid;
    let option = JSON.parse(inDb.option);
    const connectedStuffs = require('../Stuffs').connectedStuffs;
    if(connectedStuffs[UUID] == null)
        return {
            online: false
        };

    let ws = connectedStuffs[UUID].ws;
    ws.send(JSON.stringify({type: "GET_STATE", option: option}));
    let state = await wsTools.waitForResponse(UUID);
    if(state == null){
        return {
            online: false
        }
    }
    else{

        return {
            online: true,
            on: state.on
        }
    }


};

module.exports = onOffLight;



