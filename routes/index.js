const express = require('express');
const router = express.Router();
const isLoggedIn = require('./auth-manager').isLoggedIn;
const models = require("../models");
const userRepo = models.user;
const stuffRepo = models.stuffs;
const googleTools = require("../tools/googleTools");
const stuffsList = require("../Stuffs").stuffs;
/* GET home page. */
router.get('/', isLoggedIn, function(req, res, next) {
  userRepo.findByPk(req.user.id).then((user)=>{
    user.getStuffs().then((stuffs) =>{
      res.render("index", {user: user, stuffs: stuffs});
    });

  })

});

router.get('/sync', async function (req, res, next) {
  let inDb = await stuffRepo.findByPk(6);
  if(inDb){
    let stuffObject = stuffsList[inDb.type];

    res.send(await stuffObject.getState(inDb));
      }
});



module.exports = router;
