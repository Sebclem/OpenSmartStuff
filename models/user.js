module.exports = (sequelize, DataTypes) => {
    class User extends sequelize.Model { }
    User.init({
        name: DataTypes.STRING,
        description: DataTypes.TEXT
    }, { sequelize });
    return Projectl
}