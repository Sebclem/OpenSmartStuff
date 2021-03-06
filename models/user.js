const Model = require('sequelize').Model;

module.exports = function(sequelize, DataType) {
    class User extends Model {}
    User.init({

        id: {
            autoIncrement: true,
            primaryKey: true,
            type: DataType.INTEGER
        },

        firstname: {
            type: DataType.STRING,
            notEmpty: true
        },

        lastname: {
            type: DataType.STRING,
            notEmpty: true
        },
        email: {
            type: DataType.STRING,
            validate: {
                isEmail: true
            }
        },
        password: {
            type: DataType.STRING,
            allowNull: false
        },
        last_login: {
            type: DataType.DATE
        },
        status: {
            type: DataType.ENUM('active', 'inactive'),
            defaultValue: 'active'
        }


    }, {sequelize});
    return User;

};