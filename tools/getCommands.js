

function getCommands(stuff) {
    let traits = stuff.traits;
    let commands = {};
    for(const aTrait in traits){
        commands = Object.assign(commands, traits[aTrait].commands);
    }
    return commands;

}

module.exports = getCommands;