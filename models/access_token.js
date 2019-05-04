const Model = require('sequelize').Model;

module.exports = function(sequelize, DataType) {
    class AccessToken extends Model {}
    AccessToken.init({

        id: {
            autoIncrement: true,
            primaryKey: true,
            type: DataType.INTEGER
        },

        serviceID: {
            type: DataType.STRING,
            notEmpty: true
        },

        token: {
            type: DataType.STRING,
            notEmpty: true
        }



    }, {sequelize});
    return AccessToken;

};