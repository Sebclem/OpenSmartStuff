const Model = require('sequelize').Model;

module.exports = function(sequelize, DataType) {
    class Stuffs extends Model {}
    Stuffs.init({

        id: {
            autoIncrement: true,
            primaryKey: true,
            type: DataType.INTEGER
        },

        type: {
            type: DataType.STRING,
            notEmpty: true
        },

        uuid: {
            type: DataType.STRING,
            notEmpty: true
        },
        token: {
            type: DataType.STRING,
            notEmpty: true
        },
        roomHint: {
            type: DataType.STRING,
            notEmpty: false
        },
        name: {
            type: DataType.STRING,
            notEmpty: true
        },
        nicknames: {
            type: DataType.STRING,
            notEmpty: false
        },
        option: {
            type: DataType.STRING,
            notEmpty: false
        }



    }, {sequelize});
    return Stuffs;

};