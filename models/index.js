"use strict";

const fs = require("fs");
const path = require("path");
const Sequelize = require("sequelize");


const sequelize = new Sequelize( process.env.DB_NAME, process.env.DB_USER, process.env.DB_PASSWORD, {
    dialect: 'mariadb',
    host: process.env.DB_HOST,
    logging: false
});


const db = {};


const user = sequelize.import(__dirname + '/user.js' );
const accessToken = sequelize.import(__dirname + '/access_token.js');
const stuffs = sequelize.import(__dirname + '/stuffs.js');

user.hasMany(accessToken);
accessToken.belongsTo(user);
user.hasMany(stuffs);
stuffs.belongsTo(user);


db.user = user;
db.accessToken = accessToken;
db.stuffs = stuffs;

db.sequelize = sequelize;
db.Sequelize = Sequelize;

module.exports = db;
