var express = require('express');
var router = express.Router();


router.get('/', function (req, res, next) {
    let client_id = req.query.client_id;
    let redirect_url = req .query.redirect_uri;
    let state = req.query.state;

    res.send('OK');
});

module.exports = router;