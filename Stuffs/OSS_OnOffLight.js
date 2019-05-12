const all = require("./index");
const googleTools = require("../tools/googleTools");
const wsTools = require("../tools/WsTools");
const _ = require ("lodash");


let onOffLight = _.cloneDeep(all.types.LIGHT);
let onOffTrait = _.cloneDeep(all.traits.OnOff);
onOffLight.device_info.manufacturer = 'OpenSmartStuff';
onOffLight.device_info.model = 'OnOffLight';
onOffLight.device_info.hwVersion = '0.1';
onOffLight.device_info.swVersion = '0.1';

onOffTrait.states.get = function (state) {
    console.log("Get for id " + id);
};

onOffTrait.commands.OnOff = async function (UUID, params) {
    let isOn = params.on;
    const connectedStuffs = require('../Stuffs').connectedStuffs;
    if (connectedStuffs[UUID] == null)
        return {
            status: "OFFLINE",
            errorCode: 'deviceTurnedOff',
            online: false
        };

    let ws = connectedStuffs[UUID].ws;
    ws.send(JSON.stringify({type: "COMMAND", on: isOn}));
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
        type: googleTools.convertType('LIGHT'),
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

onOffLight.getState = async function (UUID) {
    const connectedStuffs = require('../Stuffs').connectedStuffs;
    if(connectedStuffs[UUID] == null)
        return {
            online: false
        };

    let ws = connectedStuffs[UUID].ws;
    ws.send(JSON.stringify({type: "GET_STATE"}));
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



