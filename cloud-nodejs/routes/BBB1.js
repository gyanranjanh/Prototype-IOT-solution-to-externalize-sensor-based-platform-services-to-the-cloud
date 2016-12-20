var express = require('express');
var router = express.Router();


/* GET users listing. */
router.get('/', function(req, res, next) {

    //res.render('BBB1', {ledarray: global.name});

    res.render('BBB1');
});



module.exports = router;


