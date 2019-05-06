const WebSocket = require('ws');
const connectedStuffs = require('../Stuffs').connectedStuffs;
const mapper = {};
const models = require("../models");
const stuffRepo = models.stuffs;

exports.createWS = function (server) {
    const wss = new WebSocket.Server({server});
    console.log("[WS] Websocket created !");
    wss.on('connection', (ws) => {
        console.debug("[WS] New Connection");
        //connection is up, let's add a simple simple event
        ws.on('message', (message) => {

            //log the received message and send it back to the client
            let received = JSON.parse(message);
            if (received.type === "AUTH") {
                console.debug("[WS] AUTH Receive, UUID: " + received.uuid + " Token: " + received.token);
                stuffRepo.findOne({where: {uuid: received.uuid, token: received.token}}).then((finded) => {
                    if (finded) {
                        console.debug("[WS] Login Success for " + finded.name);
                        ws.send(JSON.stringify({type: "AUTH", state: "SUCCESS"}));
                        connectedStuffs[finded.uuid] = {
                            ws: ws,
                            state: {newState: false, stateJSON: {}}
                        }
                    } else {
                        console.debug("[WS] Login fail for " + received.uuid);
                        ws.send(JSON.stringify({type: "AUTH", state: "FAIL"}));
                        ws.close();
                    }


                })
            } else if (received.type === "STATE") {
                let state = connectedStuffs[received.uuid].state;
                state.stateJSON = received.state;
                state.newState = true;
            }
        });
        ws.on('close', () => {
            console.debug("[WS] Connection Closed.");
            for (let index in connectedStuffs) {
                if (connectedStuffs[index].ws === ws) {
                    delete connectedStuffs[index];
                    break;
                }
            }
        });


        ws.on("ping", function () {
            clearTimeout(this.pingTimeout);

            // Use `WebSocket#terminate()` and not `WebSocket#close()`. Delay should be
            // equal to the interval at which your server sends out pings plus a
            // conservative assumption of the latency.
            this.pingTimeout = setTimeout(() => {
                this.terminate();
                console.debug("[WS] Ping time Out");
            }, 30000 + 1000);
        })
    });
};


function ping() {

}
