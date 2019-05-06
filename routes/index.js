const express = require('express');
const router = express.Router();
const isLoggedIn = require('./auth-manager').isLoggedIn;
const models = require("../models");
const google = require("../routes/GoogleSmartHome");
const light = require("../Stuffs/OSS_OnOffLight");
/* GET home page. */
router.get('/', isLoggedIn, function(req, res, next) {
  res.render("index", {title:"test"})
});

router.get('/sync', async function (req, res, next) {
});



module.exports = router;
