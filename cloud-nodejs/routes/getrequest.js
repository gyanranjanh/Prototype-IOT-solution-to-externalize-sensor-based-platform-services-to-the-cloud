/**
 * Created by siddh on 11/27/2016.
 */
var requestify = require('requestify');

var express = require('express');
var router = express.Router();
router.get('/', function(req, res, next) {
    var BBBID =  req.query.BBBID;
    var actuator = req.query.actuator;
    var sensor = req.query.sensor;
    var interval = req.query.interval;
    var toggle = req.query.toggle;
    console.log(req.query);
    if (actuator!=null){
    requestify.get('http://10.34.85.175:8000/get_service?BBBID='+BBBID+'&actuator='+actuator+'&toggle='+toggle).then(function(response) {
        // Get the response body
        console.log(response.body);
    });}
    if (sensor!=null){
        requestify.get('http://10.34.85.175:8000/get_service?BBBID='+BBBID+'&sensor='+sensor+'&interval='+interval).then(function(response) {
            // Get the response body
            console.log(response.body);
        });}


});
/*

requestify.get('http://10.34.85.175:8000/get_service', {
    params: {
        BBB1: 1,
        actuator: "LED1",
        toggle: "on"
    },.then(function(response) {
    // Get the response body
 console.log(response.body);
});
*/
module.exports = router;