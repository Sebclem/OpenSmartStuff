const express = require('express');
const router = express.Router();
const isLoggedIn = require('./auth-manager').isLoggedIn;
const models = require("../models");
const userRepo = models.user;
const google = require("../routes/GoogleSmartHome");
const light = require("../Stuffs/OSS_OnOffLight");
/* GET home page. */
router.get('/', isLoggedIn, function(req, res, next) {
  userRepo.findByPk(req.user.id).then((user)=>{
    user.getStuffs().then((stuffs) =>{
      res.render("index", {user: user, stuffs: stuffs});
    });

  })

});

router.get('/sync', async function (req, res, next) {
});



module.exports = router;
