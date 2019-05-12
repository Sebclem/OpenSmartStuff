const express = require('express');
const router = express.Router();
const isLoggedInApi = require('./auth-manager').isLoggedInApi;
const google = require("../routes/GoogleSmartHome");
const models = require("../models");
const userRepo = models.user;


router.post('/api/forceGoogleSync', isLoggedInApi, function (req, res, next) {
    userRepo.findByPk(req.user.id).then((user) => {
        if (user == null) {
            res.status(401);
            res.send();
        } else {
            google.requestSync(user.id.toString()).then(() => {
                res.send();
            }).catch((response) => {
                let responseObj = JSON.parse(response);
                if(responseObj.error.status === "INVALID_ARGUMENT"){
                    res.status(412);
                    res.send({error: "GOOGLE_NOT_REGISTERED"})
                }
                else{
                    res.status(500);
                    res.send(responseObj);
                }

            })
        }
    }).catch(()=>{
        res.status(500);
        res.send();
    })
});


module.exports = router;