var express = require('express');
var router = express.Router();

router.post('/fileuploaded', function(req, res) {
    console.log('joo');
    res.redirect("upload");
});

