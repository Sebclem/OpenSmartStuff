const express = require('express');
const router = express.Router();
const isLoggedIn = require('./auth-manager').isLoggedIn;
const models = require("../models");
const google = require("../routes/GoogleSmartHome");
/* GET home page. */
router.get('/', isLoggedIn, function(req, res, next) {
  res.render("index", {title:"test"})
});

router.get('/sync', async function (req, res, next) {
  google.requestSync("1")
      .then((response) =>{
        res.send(response)})
      .catch((response) => res.send(response));
});



module.exports = router;
