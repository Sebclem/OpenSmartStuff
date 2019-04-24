var express = require('express');
var router = express.Router();
const test2 = require('../test');
/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

module.exports = router;
