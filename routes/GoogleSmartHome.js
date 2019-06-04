// Import the appropriate service
const { smarthome } = require('actions-on-google');

// Create an app instance
const app = smarthome({key: process.env.GOOGLE_API_KEY });
const models = require("../models");

const stuffRepo = models.stuffs;
const googleTools = require("../tools/googleTools");
const stuffsList = require("../Stuffs").stuffs;

// Register handlers for Smart Home intents




app.onExecute(async (body, headers) => {
    console.debug("[GOOGLE] Receive Execute");
    let authorization = headers.authorization;
    let user = await googleTools.getUserFromAuthorization(authorization);
    if(user == null){
        return {
            requestId: body.requestId,
            payload: {errorCode: "authFailure"}
        }
    }
    let responseCommand = await googleTools.execute(user, body.inputs[0].payload.commands[0]);
    let toReturn = {};
    if(responseCommand.asError){
        toReturn = {
            requestId: body.requestId,
            payload: {
                commands: responseCommand.commands,
                // errorCode: "deviceOffline"
            }
        }
    }
    else{
        toReturn = {
            requestId: body.requestId,
            payload: {
                commands: responseCommand.commands
            }
        }
    }
    console.log(JSON.stringify(toReturn));
    return toReturn;
});

app.onQuery(async (body, headers) => {
    console.debug("[GOOGLE] Receive Query");
    let authorization = headers.authorization;
    let user = await googleTools.getUserFromAuthorization(authorization);
    if(user == null){
        return {
            requestId: body.requestId,
            payload: {errorCode: "authFailure"}
        }
    }
    let devices = body.inputs[0].payload.devices;
    let toReturn = {};
    for(const device of devices){

        let inDb = await stuffRepo.findByPk(device.id);
        if(inDb){
            let stuffObject = stuffsList[inDb.type];

            toReturn[device.id] = await stuffObject.getGoogleState(inDb);
        }
    }
    console.log(toReturn);
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
    if(user == null){
        return {
            requestId: body.requestId,
            payload: {errorCode: "authFailure"}
        }
    }
    let stuffs = await user.getStuffs();
    let devices = [];
    stuffs.forEach((stuff) => {
        let stuffObject = stuffsList[stuff.type];
        devices.push(stuffObject.getSync(stuff));
    });
    console.log(devices);
    return {
        requestId: body.requestId,
        payload: {
            agentUserId: user.id,
            devices: devices
        }
    };
});

module.exports = app;

