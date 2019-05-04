// Import the appropriate service
const { smarthome } = require('actions-on-google');

// Create an app instance
const app = smarthome({key: process.env.GOOGLE_API_KEY });
const models = require("../models");
const userRepo = models.user;
const stuffRepo = models.stuffs;
const googleTools = require("../tools/googleTools");
const stuffsList = require("../Stuffs").stuffs;

// Register handlers for Smart Home intents




app.onExecute(async (body, headers) => {
    console.debug("[GOOGLE] Receive Execute");
    let responseCommand = await googleTools.execute(body.inputs[0].payload.commands[0]);
    return {
        requestId: body.requestId,
        payload: {
            commands: responseCommand
        }
    }
});

app.onQuery(async (body, headers) => {
    console.debug("[GOOGLE] Receive Query");
    let authorization = headers.authorization;
    let user = await googleTools.getUserFromAuthorization(authorization);
    let devices = body.inputs[0].payload.devices;
    let toReturn = {};
    for(const device of devices){

        let inDb = await stuffRepo.findByPk(device.id);
        if(inDb){
            let stuffObject = stuffsList[inDb.type];

            toReturn[device.id] = stuffObject.getState(inDb.uuid);
        }
    }
    return {
        requestId: body.requestId,
        payload: {
            devices: toReturn
        }
    }
});

app.onSync(async (body, headers) => {
    console.debug("[GOOGLE] Receive Sync");
    let authorization = headers.authorization;
    let user = await googleTools.getUserFromAuthorization(authorization);
    let stuffs = await user.getStuffs();
    let devices = [];
    stuffs.forEach((stuff) => {
        let stuffObject = stuffsList[stuff.type];
        devices.push(stuffObject.getSync(stuff));
    });
    return {
        requestId: body.requestId,
        payload: {
            agentUserId: user.id,
            devices: devices
        }
    };
});


module.exports = app;

