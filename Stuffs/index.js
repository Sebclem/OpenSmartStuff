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
        getState: function(UUID){}

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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
        getState: function(UUID){}
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
            get: function (state) {
            } // -> query and return the right formatted current state
        },
        commands: {
            BrightnessAbsolute: function (parameters) {
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
            },
            get: function (state) {
            } // -> query and return the right formatted current state
        },
        commands: {
            ColorAbsolute: function (parameters) {
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
            get: function (state) {
            } //-> query and return the right formatted current state
        },
        commands: {
            OnOff: function (parameters) {
            }
        }
    }


};






exports.types = types;
exports.traits = traits;

const OSS_OnOffLight = require("./OSS_OnOffLight");
let stuffs = {};
stuffs.OSS_OnOffLight = OSS_OnOffLight;

exports.stuffs = stuffs;