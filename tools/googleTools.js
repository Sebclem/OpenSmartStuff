const models = require("../models");
const Op = require("sequelize").Op;
const tokenRepo = models.accessToken;
const stuffRepo = models.stuffs;
const stuffsList = require("../Stuffs").stuffs;
const getCommand = require("./getCommands");


function convertTraits(traits) {
    let converted = [];
    traits.forEach((aTrait) => {
        converted.push("action.devices.traits." + aTrait)
    });
    return converted;
}

function convertType(type) {
    return "action.devices.types." + type
}

function getTraits(device) {
    let traits = [];
    for (let trait in device.traits) {
        traits.push(trait);
    }
    return convertTraits(traits);
}

function getAttributes(device) {
    let attributes = {};
    for (let trait in device.traits) {
        Object.assign(attributes, device.traits[trait].attributes.get(device.traits[trait]));
    }
    return attributes;
}


async function getUserFromAuthorization(authorization) {
    let splided = authorization.split(' ');
    let token = splided[1];
    let tokenInDb = await tokenRepo.findOne({
        where: {
            token: token
        }
    });
    if (tokenInDb === null) {
        console.warn("[GOOGLE TOKEN CHECK] Unknown token");
        return null;
    }

    return await tokenInDb.getUser();
}


async function execute(user, commands) {
    let command = commands.execution[0].command.replace("action.devices.commands.", "");
    let params = commands.execution[0].params;
    let idList = [];
    for (const device in commands.devices) {
        idList.push({id: commands.devices[device].id});
    }
    let stuffsIdDb = await user.getStuffs({
        where: {
            [Op.or]: idList
        }
    });
    let toReturn = {};
    let success = {
        ids: [],
        status: "SUCCESS",
        states: null
    };
    let otherStatus = [];
    let asError = false;
    for(const index in stuffsIdDb){

        let inDb = stuffsIdDb[index];
        let stuffObject = stuffsList[inDb.type];
        let objectCommands = getCommand(stuffObject);
        let state =  await objectCommands[command](inDb,params);
        if(state.status === "SUCCESS"){
            success.ids.push(inDb.id.toString());
            if(success.states === null){
                success.states = state.states;
            }
        }
        else{
            asError = true;
            otherStatus.push({
                ids: [inDb.id.toString()],
                status: state.status,
                errorCode: state.errorCode,
                online: state.online
            })
        }

    }

    let responseCommands = [];
    if(success.ids.length > 0)
        responseCommands = responseCommands.concat(success);
    if(otherStatus.length > 0)
        responseCommands = responseCommands.concat(otherStatus);

    return {asError: asError, commands: responseCommands};

}


exports.convertTraits = convertTraits;
exports.convertType = convertType;
exports.getTraits = getTraits;
exports.getAttributes = getAttributes;
exports.getUserFromAuthorization = getUserFromAuthorization;
exports.execute = execute;