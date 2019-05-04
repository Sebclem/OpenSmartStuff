const express = require('express');
const router = express.Router();
const isLogged = require('../routes/auth-manager').isLoggedIn;
const models = require("../models");
const userRepo = models.user;
const accessTokenRepo = models.accessToken;



router.get('/auth', function (req, res, next) {
    let data = {
        client_id: req.query.client_id,
        redirect_url: req.query.redirect_uri,
        state: req.query.state
    };
    res.cookie('data', JSON.stringify(data));
    if (!req.isAuthenticated()) {
        res.cookie('destination', '/generateAuth');
        res.redirect("/signin");
    } else
        res.redirect("/generateAuth");
});

router.get('/token', function (req, res, next) {
    let client_id = req.query.client_id;
    let redirect_url = req.query.redirect_uri;
    let state = req.query.state;

    res.send(req.query);
});


router.get('/generateAuth', isLogged, function (req, res, next) {
    let data = JSON.parse(req.cookies.data);
    if (checkoAuthData(data)) {
        userRepo.findByPk(req.user.id).then((user) => {
            getToken(user, data.client_id).then(token => {
                let url = data.redirect_url + '#access_token='+token.token+'&token_type=bearer&state=' + data.state;
                res.redirect(url);
            });
        });
    } else {
        console.warn("[/generateAuth] GET - Failed to check data");
        res.status(403).send();
    }

});

function checkoAuthData(data) {
    if (data) {
        if (data.client_id) {
            if (data.redirect_url) {
                if (data.state) {
                    if (data.client_id === "google_smart_stuff") {
                        if (data.redirect_url.startsWith("https://oauth-redirect.googleusercontent.com/r/"))
                            return true
                    }

                }
            }
        }
    }
    return false
}


async function getToken(user, serviceID) {
    let tokens = await user.getAccessTokens({where: {serviceID: serviceID}});
    if (tokens.length !== 0) {
        return tokens[0];
    } else {
        return await genToken(user, serviceID);
    }
}

async function genToken(user, serviceID) {
    console.log("[oauth] Generate new token for user " + user.email + " service ID : " + serviceID );
    let token = require('crypto').randomBytes(48).toString('hex');
    let tokenDb = await accessTokenRepo.create({serviceID: serviceID, token: token});
    user.addAccessToken(tokenDb);
    return token;

}

module.exports = router;