/**
 * Created by siddh on 11/24/2016.
 */
var express = require('express');
var router = express.Router();
var bodyParser = require('body-parser');
var app = express();
var jsonParser = bodyParser.json();
var fs = require("fs");

global.name = [];
global.other = [];
global.name2 = [];
global.other2 = [];
/* GET home page. */
router.get('/', function(req, res, next) {
    res.render('index');
});

var IS = require('initial-state');

router.post('/',jsonParser, function (req, res) {
    if (!req.body) return res.sendStatus(400)

    else {
        var myobject = req.body;
        console.log(myobject);
        for (var attributename in myobject) {
            console.log(attributename);
            var bucket = IS.bucket(attributename, '7NWgod6vuE8L60n2R40pwOFQgacL4P1Z');

            //fs.writeFile( attributename+".json", JSON.stringify( myobject ), "utf8", yourCallback );

            Object.keys(myobject[attributename]).forEach(function (key) {

                console.log(key);
                if(key.includes("LED") || key.includes("led")) {
                    console.log("req: " + key);
                   if(attributename=="BBB1")
                       global.name.push(key);
                   else if (attributename=="BBB2")
                       global.name2.push(key);

                       }
            else {
                    if (attributename == "BBB1")
                        global.other.push(key);
                    else if (attributename=="BBB2")
                        global.other2.push(key);
                }
            });

        }
    }
});
module.exports = router;



