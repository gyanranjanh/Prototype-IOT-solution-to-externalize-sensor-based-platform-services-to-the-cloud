var express = require('express');
var router = express.Router();
var bodyParser = require('body-parser');
var app = express();
var jsonParser = bodyParser.json();
/* GET home page. */
var low;
var high;
router.get('/', function(req, res, next) {
    console.log(req.query);
  if (req.query.low!=null) {
      low = req.query.low;
      high = req.query.high;
  }
  else {
      res.render('index');
  }
});
var IS = require('initial-state');
router.post('/',jsonParser, function (req, res) {
    if (!req.body) return res.sendStatus(400)

    else {
        var myobject = req.body;

        console.log(myobject);
        for (var attributename in myobject) {
            console.log(attributename );
            var bucket = IS.bucket(attributename, '7NWgod6vuE8L60n2R40pwOFQgacL4P1Z');

            for (var sensors in myobject[attributename]) {

                console.log(sensors);
                console.log(myobject[attributename][sensors]["reading"]);
                bucket.push(sensors, myobject[attributename][sensors]["reading"],[myobject[attributename][sensors]["timestamp"]]);
                var bucket = IS.bucket('DOSProject', '7NWgod6vuE8L60n2R40pwOFQgacL4P1Z');
                bucket.push(sensors, myobject[attributename][sensors]["reading"],[myobject[attributename][sensors]["timestamp"]]);


                    if((myobject[attributename][sensors]["reading"]>low)&&(myobject[attributename][sensors]["reading"]<high))
                    {
                        if(!(sensors.includes("TMP36") ))
                        {
                            requestify.get('http://10.34.85.175:8000/get_service?BBBID='+attributename+'&actuator=LED1&toggle=on').then(function(response) {
                                // Get the response body
                                console.log(response.body);
                            });}
                            else{

                            requestify.get('http://10.34.85.175:8000/get_service?BBBID='+attributename+'&actuator=LED2&toggle=on').then(function(response) {
                                // Get the response body
                                console.log(response.body);
                            });
                        }
                        }
                    }

            }

        }

});
module.exports = router;


