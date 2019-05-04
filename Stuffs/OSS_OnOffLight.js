const all = require("./index");
const googleTools = require("../tools/googleTools");

let onOffLight = Object.assign({}, all.types.LIGHT);
let onOffTrait = Object.assign({}, all.traits.OnOff);
let isOn = false;
onOffLight.device_info.manufacturer = 'OpenSmartStuff';
onOffLight.device_info.model = 'OnOffLight';
onOffLight.device_info.hwVersion = '0.1';
onOffLight.device_info.swVersion = '0.1';

onOffTrait.states.get = function (state) {
    console.log("Get for id " + id);
};

onOffTrait.commands.OnOff = function (UUID, params) {
    isOn = params.on;
    //TODO send command to object
    return {
        status: "SUCCESS",
        states: {
            online: true,
            on: isOn
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

onOffLight.getState = function(UUID){
    //TODO get the state of the light
    return {
        online: true,
        on: isOn
    }
};
module.exports = onOffLight;

