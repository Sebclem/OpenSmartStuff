const express = require('express');
const router = express.Router();
const isLoggedIn = require('./auth-manager').isLoggedInApi;
const models = require("../models");
const userRepo = models.user;
const stuffRepo = models.stuffs;
const googleTools = require("../tools/googleTools");
const stuffsList = require("../Stuffs").stuffs;


router.get('/api/getStuffState', isLoggedIn, function (req, res, next) {
    let id = req.query.id;
    if (id) {
        userRepo.findByPk(req.user.id).then((user) => {
            user.getStuffs({where: {id: id}}).then(async (inDb) => {
                if (inDb.length > 0) {
                    let stuffObject = stuffsList[inDb[0].type];
                    let state = await stuffObject.getState(inDb[0]);
                    res.send(state);
                }
                else{
                    res.status(404);
                    res.send();
                }
            });


        });
    } else {
        res.status(400);
        res.send({message: "Missing \'id\' param"});
    }


});


module.exports = router;
