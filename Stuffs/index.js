
const wsTools = require("../tools/WsTools");
const stuffTemplate = {
    type:"",
    traits: {},
    device_info: {
        manufacturer: '',
        model: '',
        hwVersion: '',
        swVersion: ''
    },
    getSync: function (stuffInDb) {
    },
    getState: async function(inDb) {
        let UUID = inDb.uuid;
        let option = JSON.parse(inDb.option);
        const connectedStuffs = require('../Stuffs').connectedStuffs;
        if (connectedStuffs[UUID] == null)
            return {
                online: false
            };

        let ws = connectedStuffs[UUID].ws;
        ws.send(JSON.stringify({type: "GET_STATE", option: option}));
        let state = await wsTools.waitForResponse(UUID);
        if (state == null) {
            return {
                online: false
            }
        } else {

            return {
                online: true,
                state: state
            }
        }
    },
    getGoogleState: function (inDb) {
    },
    updateState: function (stateObject) {

    },
    getWidget : function () {
        
    }
};


const types = {
    AC_UNIT: "AC_UNIT",
    AIRFRESHENER: "AIRFRESHENER",
    AIRPURIFIER: "AIRPURIFIER",
    BLINDS: "BLINDS",
    CAMERA: "CAMERA",
    COFFEE_MAKER: "COFFEE_MAKER",
    DISHWASHER: "DISHWASHER",
    DRYER: "DRYER",
    FAN: "FAN",
    FIREPLACE: "FIREPLACE",
    HEATER: "HEATER",
    KETTLE: "KETTLE",
    LIGHT: "LIGHT",
    OUTLET: "OUTLET",
    OVEN: "OVEN",
    REFRIGERATOR: "REFRIGERATOR",
    SCENE: "SCENE",
    SPRINKLER: "SPRINKLERr",
    SWITCH: "SWITCH",
    THERMOSTAT: "THERMOSTAT",
    VACUUM: "VACUUM",
    WASHER: "WASHER"
};


const traits = {
    Brightness: { // -> https://developers.google.com/actions/smarthome/traits/brightness
        attributes: {
            get: function () {
                return {}
            }
        },
        states: {
            brightness: -1,
        },
        commands: {
            BrightnessAbsolute: function (UUID, params) {
            }
        }
    },

    ColorSetting: {  // -> https://developers.google.com/actions/smarthome/traits/colorsetting
        attributes: {
            colorModel: "", // -> if !colorTemperatureRange -> rgb or hsv
            colorTemperatureRange: {
                temperatureMinK: 2000,
                temperatureMaxK: 6500,
            },
            commandOnlyColorSetting: false,
            get: (colorSetting) => {
                if (colorSetting.attributes.colorModel === "") {
                    return colorSetting.attributes.colorTemperatureRange;
                } else {
                    return {colorModel: colorSetting.attributes.colorModel}
                }
            }

        },
        states: {
            temperatureK: -1,// if colorTemperatureRange
            spectrumRgb: -1, // if colorModel == "rgb"
            spectrumHsv: {
                hue: 0.0, // -> hue as positive degrees in the range of  [0.0, 360.0]
                saturation: 0.0, // -> Saturation as a percentage in the range [0.0, 1.0].
                value: 0.0 // -> Value as a percentage in the range [0.0, 1.0]
            }
        },
        commands: {
            ColorAbsolute: function (UUID, params) {
            }
        }
    },

    OnOff: {
        attributes: {
            get: function () {
                return null
            }
        },
        states: {
            on: false,
        },
        commands: {
            OnOff: function (UUID, params) {
            }
        }
    },
    TemperatureSetting: {  // -> https://developers.google.com/actions/smarthome/traits/colorsetting
        attributes: {
            availableThermostatModes: "", // -> https://developers.google.com/actions/smarthome/traits/temperaturesetting#device-attributes
            thermostatTemperatureUnit: "C", // C or F
            bufferRangeCelsius: 2, // Double. Optional. If unset, defaults to 2 degrees Celsius. Specifies the minimum offset between heat-cool setpoints in Celsius, if heatcool mode is supported.
            commandOnlyTemperatureSetting: false,
            queryOnlyTemperatureSetting: false,
            get: (temperatureSetting) => {

                if(temperatureSetting.attributes.queryOnlyTemperatureSetting){
                    return {queryOnlyTemperatureSetting: true, thermostatTemperatureUnit: temperatureSetting.attributes.thermostatTemperatureUnit}
                }
                return temperatureSetting.attributes;
            }

        },
        states: {
            thermostatMode: "",
            thermostatTemperatureSetpoint: -1,
            thermostatTemperatureAmbient: -1,
            thermostatTemperatureSetpointHigh: -1,
            thermostatTemperatureSetpointLow: -1,
            thermostatHumidityAmbient: -1

        },
        commands: {
            ThermostatTemperatureSetpoint: function (UUID, params) {
            },
            ThermostatTemperatureSetRange: function (UUID, params) {
            },
            ThermostatSetMode: function (UUID, params) {
            },
            TemperatureRelative: function (UUID, params) {
            }
        }
    },


};


exports.types = types;
exports.traits = traits;
exports.stuffTemplate = stuffTemplate;

const OSS_OnOffLight = require("./OSS_OnOffLight");
const OSS_BrightnessLight = require("./OSS_BightnessLight");
const OSS_BME280 = require("./OSS_BME280");
let stuffs = {};
stuffs.OSS_OnOffLight = OSS_OnOffLight;
stuffs.OSS_BrightnessLight = OSS_BrightnessLight;
stuffs.OSS_BME280 = OSS_BME280;
exports.connectedStuffs = {};
exports.stuffs = stuffs;
