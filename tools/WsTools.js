
const timeOut = 1000; //Time out in ms
const sleepTime = 2; //Sleep time in ms

async function waitForResponse(uuid) {
    let i = 0;
    const connectedStuffs = require('../Stuffs').connectedStuffs;
    if(connectedStuffs[uuid] == null)
        return null;
    while (!connectedStuffs[uuid].state.newState) {
        await sleep(sleepTime);
        if (i < timeOut / sleepTime) {
            i++;
        } else {
            console.debug("[WFR] TimeOut");
            break;
        }
    }

    if (i >= timeOut / sleepTime)
        return null;
    connectedStuffs[uuid].state.newState = false;
    return connectedStuffs[uuid].state.stateJSON;
}

function sleep(ms){
    return new Promise(resolve=>{
        setTimeout(resolve,ms)
    })
}

exports.waitForResponse = waitForResponse;
