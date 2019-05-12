const types = {
    AC_UNIT: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }

    },
    AIRFRESHENER: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    AIRPURIFIER: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    BLINDS: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    CAMERA: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    COFFEE_MAKER: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    DISHWASHER: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    DRYER: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    FAN: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    FIREPLACE: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    HEATER: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    KETTLE: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    LIGHT: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    OUTLET: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    OVEN: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    REFRIGERATOR: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    SCENE: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    SPRINKLER: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    SWITCH: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    THERMOSTAT: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    VACUUM: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    },
    WASHER: {
        traits: {},
        device_info: {
            manufacturer: '',
            model: '',
            hwVersion: '',
            swVersion: ''
        },
        getSync: function (stuffInDb) {
        },
        getState: function(UUID){},
        updateState: function (stateObject) {

        }
    }
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
    }


};






exports.types = types;
exports.traits = traits;

const OSS_OnOffLight = require("./OSS_OnOffLight");
const OSS_BrightnessLight = require("./OSS_BightnessLight");
let stuffs = {};
stuffs.OSS_OnOffLight = OSS_OnOffLight;
stuffs.OSS_BrightnessLight = OSS_BrightnessLight;
exports.connectedStuffs = {};
exports.stuffs = stuffs;