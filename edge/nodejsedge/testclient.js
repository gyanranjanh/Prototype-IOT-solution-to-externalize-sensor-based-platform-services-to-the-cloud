//var express = require('express');

//var request = require('request');

//var app = express();

var udp = require('dgram');

var PORT = 7894;

var HOST = '192.168.2.4';

var MQTT_PUBLISH_MESSAGE  =  1;

var MQTT_SUBSCRIBE_MESSAGE = 2;

var MQTT_UNSUBSCRIBE_MESSAGE  = 3;

var EDGE = 0;

var BBBID1 = 1;

var BBBID2 = 2;

var BBBID3 = 3;

var BBBID4 = 4;

var BBBID5 = 5;

var BBBID6 = 6;

var BBBID7 = 7;

var BBBID8 = 8;

var BBBID9 = 9;


var    MQTT_TOPIC_GET_TEMP_ANALOG = 1;

var    MQTT_TOPIC_GET_TEMP_DIGITAL = 2;

var    MQTT_TOPIC_NOTIFY_TEMP_ANALOG = 3;

var    MQTT_TOPIC_NOTIFY_TEMP_DIGITAL = 4;

var    MQTT_TOPIC_CUR_TEMP_ANALOG = 5;

var    MQTT_TOPIC_CUR_TEMP_DIGITAL = 6;

var    MQTT_TOPIC_LED = 7;

var    MQTT_TOPIC_GET_LED_STATUS = 8;

var    MQTT_TOPIC_LED_STATUS =  9;

var    MQTT_SERVICE_TEMP_ANALOG = 0;

var    MQTT_SERVICE_TEMP_DIGITAL = 1;

var    MQTT_SERVICE_BASIC_LED = 2;

var    MQTT_SERVICE_UNKNOWN = 3;

var    NO_MQTT_SERVICES = MQTT_SERVICE_UNKNOWN;

var message = Buffer.alloc(1024);

function serializemessage( message, bbbid, msgtype, topic, sensorname, sensordata ) {

	message.writeInt32LE(bbbid,0);
	message.writeInt8(msgtype,4);
	message.writeInt16LE(topic,5);
	message.writeInt8((1 + sensorname.length + 4 ),7);
	message.writeInt8(sensorname.length,8);      
	message.write(sensorname,9,sensorname.length,'ascii');
	message.writeInt32LE(sensordata,(9 + sensorname.length));
	                   // Function returns the product of a and b
}

function deserializemessage(message) {

	this.bbbid = message.readUInt32LE(0);
	this.msgtype = message.readUInt8(4);
	this.topic = message.readUInt16LE(5);
	this.length = message.readUInt8(7);
	this.sensornamelength = message.readUInt8(8);
	this.sensorname = message.toString('ascii', 9, 9 + this.sensornamelength);
	if (this.topic == MQTT_TOPIC_LED_STATUS) {
		this.sensordata = message.readUInt8(9 + this.sensornamelength);
	} else if (this.topic == MQTT_TOPIC_CUR_TEMP_ANALOG) {
		this.sensordata = message.readInt32LE(9 + this.sensornamelength);
	} else if (this.topic == MQTT_TOPIC_CUR_TEMP_DIGITAL) {
		this.sensordata = message.readInt32LE(9 + this.sensornamelength);
	} else {
		console.log('unhandled topic');
	}
}

function socketerrorhandler(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
}

// --------------------creating a udp socket --------------------

var recvsocket = udp.createSocket('udp4');
recvsocket.bind(7896);
// emits when any error occurs
recvsocket.on('error',function(error){
  console.log('Error: ' + error);
  recvsocket.close();
});

//subscribe to all relevant topics

message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_SUBSCRIBE_MESSAGE,MQTT_TOPIC_CUR_TEMP_ANALOG,'x', 0);
recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler);

message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_SUBSCRIBE_MESSAGE,MQTT_TOPIC_CUR_TEMP_DIGITAL,'x', 0);
recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler);

message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_SUBSCRIBE_MESSAGE,MQTT_TOPIC_LED_STATUS,'x', 0);
recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler);

//tests start here

message = Buffer.alloc(1024);
serializemessage(message,1,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_TEMP_ANALOG, 'TMP36', 0);
recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler());

message = Buffer.alloc(1024);
serializemessage(message,1,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_LED, 'LED1', 1);
recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler());

setTimeout(function () {
message = Buffer.alloc(1024);
serializemessage(message,1,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_LED_STATUS, 'LED1', 0);
recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler());
  }, 1000);

setTimeout(function () {
message = Buffer.alloc(1024);
serializemessage(message,1,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_LED, 'LED1', 0);
recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler());
  }, 5000);
  
setTimeout(function () {
message = Buffer.alloc(1024);
serializemessage(message,1,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_LED_STATUS, 'LED1', 0);
recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler());
  }, 6000);

//tests end here
// emits on new datagram msg
recvsocket.on('message', function(msg,info){
  
  console.log('Data received from BBB : ');
  var rcvdmsg = new deserializemessage(msg);
  console.log('bbbid: '+rcvdmsg.bbbid.toString()+'msgtype '+rcvdmsg.msgtype.toString()+'topic '+rcvdmsg.topic.toString()+'length '+rcvdmsg.length.toString()+'sensorname '+rcvdmsg.sensorname.toString()+'sensornamelength '+rcvdmsg.sensornamelength.toString()+'sensordata '+rcvdmsg.sensordata);
  console.log('Received %d bytes from %s:%d\n',msg.length, info.address, info.port);
  
//TODO send request
  rcvdmsg.bbbid ;
	rcvdmsg.msgtype ;
	rcvdmsg.topic ;
	rcvdmsg.length;
	rcvdmsg.sensornamelength ;
	rcvdmsg.sensorname  ;
	rcvdmsg.sensordata;
	/*
	request('http://www.google.com', function (error, response, body) {
  if (!error && response.statusCode == 200) {
    console.log(body) // Show the HTML for the Google homepage.
  }
})
*/
});

//emits after the socket is closed using socket.close();
recvsocket.on('close', function(){
  console.log('Socket is closed !');
});


// all relevant services rendered are added here
/*
app.get('/get_service', function(req,res){
  
  //BBBID='ID'&actuator='ID'&toggle='on/off'
  //BBBID='ID'&sensor='ID'&interval='valueinseconds'
  var BBBID =  req.query.BBBID;
  var actuator = req.query.actuator;
  var sensor = req.query.sensor;
  var interval = req.query.interval;
  var toggle = req.query.interval;
  console.log(BBBID.tostring); 
  
  if(BBBID == BBBID1)
  {
      if(sensor == 'TMP36')
      {
        serializemessage(message,BBBID,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_TEMP_ANALOG, sensor, 0);
        recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler());
      }
      if(sensor == 'DS18B20')
      {
        serializemessage(message,BBBID,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_TEMP_DIGITAL, sensor, 0);
        recvsocket.send(message,0,message.length,PORT,HOST,socketerrorhandler());
      }
      
  }
  
   var request = require('request');
   var data;
   var json_obj = JSON.parse(data);
   request.post({
        headers: {'content-type':'application/json'},
        url:'http://localhost/PhpPage.php',
        form:    json_obj
    },function(error, response, body){
    console.log(body);
  });

 


});

app.get('/GET_TEMP_DIGITAL',function(req,res){
  res.send('{'+'}');
});

app.get('/NOTIFY_TEMP_ANALOG',function(req,res){
  res.send('{'+'}');
});

app.get('/NOTIFY_TEMP_DIGITAL',function(req,res){
  res.send('{'+'}');
});

app.get('/MQTT_TOPIC_LED',function(req,res){
  res.send('{'+'}');
});

app.get('/TOPIC_GET_LED_STATUS',function(req,res){
  res.send('{'+'}');
});

app.listen(process.env.PORT||8000);

*/
