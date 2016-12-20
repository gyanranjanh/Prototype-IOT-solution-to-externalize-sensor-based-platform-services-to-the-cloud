var express = require('express');
var router = express.Router();
var fileUpload = require('express-fileupload');
var fs = require('fs');
var mv = require('mv');
var mkdirp = require('mkdirp');
var request = require('request');
global.bbbid = global;



/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

/* GET Hello World page. */
router.get('/helloworld', function(req, res) {
  res.render('helloworld', { title: 'Hello, World!' });
});

/* GET Userlist page. */
router.get('/userlist', function(req, res) {
  var db = req.db;
  var collection = db.get('bbb1collection');
  collection.find({},{},function(e,docs){
    res.render('userlist', {
      "userlist" : docs
    });
  });
});

/* POST to Add User Service */
router.post('/adduser', function(req, res) {

  // Set our internal DB variable
  var db = req.db;

  // Get our form values. These rely on the "name" attributes
  var bbbid = req.body.bbbid;
  var numofsensors = req.body.numofsensors;

  // Set our collection
  var collection = db.get('bbb1collection');

  // Submit to the DB
  collection.insert({
    "bbbid" : bbbid,
    "numofsensors" : numofsensors,
  }, function (err, doc) {
    if (err) {
      // If it failed, return error
      res.send("There was a problem adding the information to the database.");
    }

    else {
      // And forward to success page
      res.redirect("upload");
    }

      fs.mkdir('/uploads/'+bbbid ,function(err){
          if (err) {
              return console.error(err);
          }
          console.log("Directory created successfully!");

      });
  });
});


/* POST to Upload Page */
// default options
router.use(fileUpload());

var j=0;
var count = 1;
var sensor = 1;
var count1 = 2;
var count2 = 3;
var filecount = 4;

router.post('/upload', function(req, res) {

    var sampleFile;
    var bbbid = req.body.bbbid;
    DDLFile = req.files.DDLFile;

    var myfiles3 = [];

    var arrayOfFiles = fs.readdirSync('/uploads/' + bbbid);

//Yes, the following is not super-smart, but you might want to process the files. This is how:
    arrayOfFiles.forEach(function (file) {
        myfiles3.push(file);
        console.log(myfiles3);
    });

    var j = myfiles3.length;
    j++;

    DDLFile.mv('/uploads/' + bbbid + '/device' + j + '.json', function (err) {
        if (err) {
            res.status(500).send(err);
        }
        else {
            console.log("No of files in directory:", j);
            var myfiles = [];
            var fs = require('fs');

            var arrayOfFiles = fs.readdirSync('/uploads/' + bbbid);

//Yes, the following is not super-smart, but you might want to process the files. This is how:
            arrayOfFiles.forEach(function (file) {
                myfiles.push(file);
                console.log(myfiles);
            });

            console.log('No of Files:', myfiles.length);
            var files = myfiles.length;
            console.log('Files:', files);
            console.log('J', j);

            //var bbb_id = '"+bbbid+"';
            var cddl = '{' + '"' + bbbid + '":';
            if (files == 0) {

                console.log('Entered if loop');
                cddl = cddl + '}';
                console.log(cddl);

            }
            else {
                var i = 0;
                /*var obj;
                 fs.readFile('/uploads/' + bbbid + '/device' + j + '.json', 'utf8', function (err, data) {
                 if (err) throw err;
                 obj = JSON.parse(data);
                 });*/


                if (files == 1) {
                    console.log('Entered if one file loop');
                    console.log('Count:', count);
                    console.log('Sensor:', sensor);
                    var obj = require('/uploads/' + bbbid + '/device' + count + '.json');
                    //console.log('everything good');
                    //JSON.stringify(obj);
                    //var obj1 = JSON.parse(obj);
                    console.log('hi');
                    var ddl = require('/uploads/' + bbbid + '/device' + count + '.json');
                    console.log('yo');
                    //var ddl = require('/uploads/' + bbbid + '/device' + count + '.json');
                    //var cdll = cdl + '}';
                    var cdl3 = cddl + "{ " + '"' + obj.DDL.Sensor.Description.Name + '"' + ":" + JSON.stringify(ddl) + "}}"
                    fs.writeFile('/uploads/' + bbbid + '.json', cdl3, function (err) {
                        if (err) return console.log(err);
                        console.log('wrote to uploads');
                    });
                    console.log(cdl3);
                    res.redirect("fileuploaded");
                }
                if (files == 2) {

                    //for (i = 0; i < files; i++) {
                    var cdl1 = require('/uploads/' + bbbid + '.json');
                    var cdk = JSON.stringify(cdl1);
                    cdk = cdk.slice(0, -2);
                    console.log(cdk);
                    var obj1 = require('/uploads/' + bbbid + '/device' + count1 + '.json');
                    var ddl1 = require('/uploads/' + bbbid + '/device' + count1 + '.json');
                    var dl = cdk + ", " + '"' + obj1.DDL.Sensor.Description.Name + '"' + ":" + JSON.stringify(ddl1) + "}}"
                    JSON.stringify(dl);
                    console.log(dl);
                    // var dl1 = JSON.stringify(cdl1) + ", "+ '"' + obj1.DDL.Sensor.Description.Verbose_Description +'"'+ ":" + JSON.stringify(ddl1) + "}"
                    fs.writeFile('/upload/' + bbbid + '.json', dl, function (err) {
                        if (err) return console.log(err);
                        console.log('wrote to upload');
                    });
                    res.redirect("fileuploaded");
                }


                if (files == 3) {

                    //for (i = 0; i < files; i++) {
                    var cdl2 = require('/upload/' + bbbid + '.json');
                    var cdk1 = JSON.stringify(cdl2);
                    cdk1 = cdk1.slice(0, -2);

                    var obj2 = require('/uploads/' + bbbid + '/device' + count2 + '.json');
                    var ddl2 = require('/uploads/' + bbbid + '/device' + count2 + '.json');
                    var dl1 = cdk1 + ",  " + '"' + obj2.DDL.Sensor.Description.Name + '"' + ":" + JSON.stringify(ddl2) + "}}"
                    JSON.stringify(dl1);
                    console.log(dl1);
                    fs.writeFile('/uploadddl/' + bbbid + '.json', dl1, function (err) {
                        if (err) return console.log(err);
                        console.log('wrote to uploadddl');
                    });
                    res.redirect("fileuploaded");
                }
                console.log('Count:', count);
                console.log('Sensor:', sensor);
            }
            //cddl = cddl + '}';
            //JSON.stringify(cddl);
            //console.log(cddl);
        }




    });



});

router.post('/fileuploaded', function(req, res) {
    console.log('woohoo');
    var myfiles1 = [];
    var fs = require('fs');


    var arrayOfFiles = fs.readdirSync('/uploads/BBB'+filecount);

//Yes, the following is not super-smart, but you might want to process the files. This is how:
    arrayOfFiles.forEach(function (file) {
        myfiles1.push(file);
        console.log(myfiles1);
    });
    console.log('No of Files:', myfiles1.length);
    var files = myfiles1.length;
    console.log('Files:', files);
    if(files == 1)
    {
        var cddl1 = require('/uploads/BBB'+filecount+ '.json');
        var cddf = JSON.stringify(cddl1);
        console.log(cddf);
        var json_obj1 = JSON.parse(cddf);
        request.post({
            headers: {'content-type':'application/json'},
            url:'http://10.34.75.88:3000/cddl',
            body:    JSON.stringify(json_obj1)
        },function(error, response, body){
            console.log(JSON.stringify(body));
        });
       /* request.post('http://10.34.75.88:3000/cddl',
            { json: true, body: cddf },
            function(err, res, body) {
                // `body` is a js object if request was successful
            });*/
    }

    if(files == 2)
    {
        var cddl2 = require('/upload/BBB'+filecount+ '.json');
        var cddg = JSON.stringify(cddl2);
        console.log(cddg);
        var json_obj2 = JSON.parse(cddg);
        request.post({
            headers: {'content-type':'application/json'},
            url:'http://10.34.75.88:3000/cddl',
            body:    JSON.stringify(json_obj2)
        },function(error, response, body){
            console.log(body);
        });
        /*request.post('https://www.googleapis.com/qpxExpress/v1/trips/search?key=myApiKey',
            { json: true, body: cddg },
            function(err, res, body) {
                // `body` is a js object if request was successful
            });*/
    }

    if(files == 3)
    {
        var cddl3 = require('/uploadddl/BBB'+filecount+ '.json');
        var cddh = JSON.stringify(cddl3);
        console.log(cddh);
        var json_obj3 = JSON.parse(cddh);
        request.post({
            headers: {'content-type':'application/json'},
            url:'http://10.34.75.88:3000/cddl',
            body:    JSON.stringify(json_obj3)
        },function(error, response, body){
            console.log(JSON.stringify(body));
        });
    }
    filecount++;
    res.redirect("userlist");
});


/* GET New User page. */
router.get('/newuser', function(req, res) {
  res.render('newuser', { title: 'Add New User' });
});

/* GET Upload page. */
router.get('/upload', function(req, res) {
  res.render('upload', { title: 'Add DDL File' });
});

/* GET File Uploaded page. */
router.get('/fileuploaded',function(req, res) {
    res.render('fileuploaded', { title: 'File Uploaded' });
});

module.exports = router;

//var cddl =  "{BBBID:"+bbbid+"}";





