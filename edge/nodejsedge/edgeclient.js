//var express = require('express');

//var request = require('request');

//var app = express();

var udp = require('dgram');

var PORT = 7894;

var HOST = '192.168.2.4';

var MQTT_PUBLISH_MESSAGE  =  1

var MQTT_SUBSCRIBE_MESSAGE = 2

var MQTT_UNSUBSCRIBE_MESSAGE  = 3

var EDGE = 0

var BBBID1 = 1

var BBBID2 = 2

var BBBID3 = 3

var BBBID4 = 4

var BBBID5 = 5

var BBBID6 = 6

var BBBID7 = 7

var BBBID8 = 8

var BBBID9 = 9


var rcvdmsg;

var    MQTT_TOPIC_GET_TEMP_ANALOG = 1

var    MQTT_TOPIC_GET_TEMP_DIGITAL = 2 

var    MQTT_TOPIC_NOTIFY_TEMP_ANALOG = 3

var    MQTT_TOPIC_NOTIFY_TEMP_DIGITAL = 4

var    MQTT_TOPIC_CUR_TEMP_ANALOG = 5

var    MQTT_TOPIC_CUR_TEMP_DIGITAL = 6

var    MQTT_TOPIC_LED = 7

var    MQTT_TOPIC_GET_LED_STATUS = 8

var    MQTT_TOPIC_LED_STATUS =  9



var    MQTT_SERVICE_TEMP_ANALOG = 0

var    MQTT_SERVICE_TEMP_DIGITAL = 1

var    MQTT_SERVICE_BASIC_LED = 2

var    MQTT_SERVICE_UNKNOWN = 3

var    NO_MQTT_SERVICES = MQTT_SERVICE_UNKNOWN 



function serializemessage( message, bbbid, msgtype, topic, sensorname, sensordata ) {

	message.writeInt32LE(bbbid,0);
	message.writeInt8(msgtype,4);
	message.writeInt16LE(topic,5);
	message.writeInt8((1 + sensorname.length + 4 ),7);
	message.writeInt8(sensorname.length,8);      
	message.write(sensorname,9,sensorname.length,'ascii');
	message.writeInt32LE(sensordata,(9 + sensorname.length))
	                  
}

function deserializemessage(message) {

	//this.bbbid1 = message.readUInt32LE(0);
	this.msgtype = message.readUInt8(4);
	this.topic = message.readUInt16LE(5);
	this.length1 = message.readUInt8(7);
	this.sensornamelength = message.readInt8(8);
	this.sensorname = message.toString('ascii',9,9+ this.sensornamelength);
	this.sensordata = message.readInt32LE((9 + this.sensornamelength));
	console.log(message[4].toString());
}



message = Buffer.alloc(1024);

serializemessage(message,2,MQTT_SUBSCRIBE_MESSAGE,MQTT_TOPIC_GET_TEMP_ANALOG,'a', 0);





// --------------------creating a udp socket --------------------

var recvsocket = udp.createSocket('udp4');
recvsocket.bind(7896);
// emits when any error occurs
recvsocket.on('error',function(error){
  console.log('Error: ' + error);
  recvsocket.close();
});

message = Buffer.alloc(1024);

//subscribe to all relevant topics
serializemessage(message,EDGE,MQTT_SUBSCRIBE_MESSAGE,MQTT_TOPIC_CUR_TEMP_ANALOG,'x', 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});
 message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_SUBSCRIBE_MESSAGE,MQTT_TOPIC_CUR_TEMP_DIGITAL,'x', 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});
 message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_SUBSCRIBE_MESSAGE,MQTT_TOPIC_LED_STATUS,'x', 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});

 message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_TEMP_ANALOG, 'tmp36', 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});

 message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_TEMP_ANALOG, 'tmp36', 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});

setTimeout(function(){

message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_LED, 'led1', 1);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});
},5000); 

setTimeout(function(){

message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_LED_STATUS, 'led1', 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});
},10000); 

setTimeout(function(){

message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_LED, 'led1', 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});
},15000); 

setTimeout(function(){

message = Buffer.alloc(1024);
serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_LED_STATUS, 'led1', 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});
},20000); 

// emits on new datagram msg
recvsocket.on('message',function(msg,info){
  console.log('Data received from BBB : ');
  //var bbbid, msgtype, topic,length, sensornamelength, sensorname, sensordata;
  rcvdmsg = new deserializemessage(msg)
  console.log('bbbid: '+'msgtype '+rcvdmsg.msgtype.toString()+'topic '+rcvdmsg.topic.toString()+'length '+rcvdmsg.length1.toString()+'sensorname '+rcvdmsg.sensorname+'sensornamelength '+rcvdmsg.sensornamelength.toString()+'sensordata '+rcvdmsg.sensordata);
  console.log('Received %d bytes from %s:%d\n',msg.length, info.address, info.port);
  rcvdmsg = 0;
  
//TODO send request
  rcvdmsg.bbbid 
	rcvdmsg.msgtype 
	rcvdmsg.topic 
	rcvdmsg.length1
	rcvdmsg.sensornamelength 
	rcvdmsg.sensorname  
	rcvdmsg.sensordata
	/*
	request('http://www.google.com', function (error, response, body) {
  if (!error && response.statusCode == 200) {
    console.log(body) // Show the HTML for the Google homepage.
  }
})
*/
});

/*

//emits after the socket is closed using socket.close();
recvsocket.on('close',function(){
  console.log('Socket is closed !');
});

var    MQTT_TOPIC_GET_TEMP_ANALOG = 1

var    MQTT_TOPIC_GET_TEMP_DIGITAL = 2 

var    MQTT_TOPIC_NOTIFY_TEMP_ANALOG = 3

var    MQTT_TOPIC_NOTIFY_TEMP_DIGITAL = 4

var    MQTT_TOPIC_CUR_TEMP_ANALOG = 5

var    MQTT_TOPIC_CUR_TEMP_DIGITAL = 6

var    MQTT_TOPIC_LED = 7

var    MQTT_TOPIC_GET_LED_STATUS = 8

var    MQTT_TOPIC_LED_STATUS =  9


// all relevant services rendered are added here

app.get('/get_service',function(req,res){
  
  //BBBID='ID'&actuator='ID'&toggle='on/off'
  //BBBID='ID'&sensor='ID'&interval='valueinseconds'
  var BBBID =  req.query.BBBID;
  var actuator = req.query.actuator;
  var sensor = req.query.sensor;
  var interval = req.query.interval;
  var toggle = req.query.interval
  
//if(sensor = )
serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_TEMP_ANALOG, 'tmp36', 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});


serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_GET_TEMP_DIGITAL,sensor, 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});


serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_NOTIFY_TEMP_ANALOG,sensor, 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});

serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_NOTIFY_TEMP_DIGITAL,sensor, 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});

serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_LED,actuator, 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});

serializemessage(message,EDGE,MQTT_PUBLISH_MESSAGE,MQTT_TOPIC_LED_STATUS,actuator, 0);
recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
});


request('http://www.google.com', function (error, response, body) {
  if (!error && response.statusCode == 200) {
    console.log(body) // Show the HTML for the Google homepage.
  }
});

  recvsocket.send(message,0,message.length,PORT,HOST,function(error){
  if(error){
    recvsocket.close();
  }else{
    console.log('Data sent !!!');
  }
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
