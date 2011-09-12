var SQLClient = require('mysql').Client;
var mysql = new SQLClient();
var qs = require('querystring');
var fs = require("fs");
var nstatic = require("node-static")
var staticserver = new nstatic.Server("./www")


mysql.user = 'root';
mysql.password = '';
mysql.database = 'webmind';


var server = require('http').createServer(function(req, res){

    // offer a document in the current state

    // Add DOCUMENT/*
    if (req.url=='/addDocument' && req.method=='POST') {
        body = '';
        req.on('data', function (data) {
            body += data;
        });
        req.on('end', function () {
            var POST = qs.parse(body);
            if (POST.doc) {
                mysql.query("INSERT INTO `docs` SET title=?",[POST.doc], function(derr, dinfo) {
                    if (derr) {

                    } else {
                        console.log(dinfo);
                        mysql.query("INSERT INTO `nodes` SET doc=?",[""+dinfo.insertId], function(err, info) {
                            if (err) {
                                console.log(err);
                            } else {
                                console.log(dinfo + info);
                                res.writeHead(302, {'Location': 'http://localhost/#'+dinfo.insertId});
                                res.end();
                            }
                        });
                    }
                });
            }
        });
    }
});
server.listen(1337);
var fileserv = require("http").createServer(function(req, res){
	req.addListener("end", function(){
		staticserver.serve(req, res);
	})
}).listen(80)	
var now = require("now");
var everyone = now.initialize(server);
now.options.socketio['log level'] = 1;


everyone.connected(function(){
    console.log("Joined: " + this.now.name);
});


everyone.disconnected(function(){
    console.log("Left: " + this.now.name);
});

everyone.now.serverAddNode = function(parent, doc){
    console.log("Attempting insert of node from "+parent)
    mysql.query("INSERT INTO `nodes` SET parent=?, doc=?",[parent, doc], function(err, info) {
        if (err) {
            console.log(err);
        } else {
            console.log("Added "+info.insertId+" node");
            everyone.now.addNode(info.insertId, parent);
        }
    })};

everyone.now.serverDelNode = function(id){
    console.log("Deleting the node "+id)
    mysql.query("SELECT parent FROM `nodes` WHERE id=?", [id], function (err, results, info) {
        if (!err) {
            console.log(results);
            mysql.query("UPDATE `nodes` SET parent=? WHERE parent=?",[results.parent, id], function (err, info) {
                if (!err) {
                    mysql.query("DELETE FROM `nodes` WHERE id=?",[id], function(err, info){
                        if (!err)   {
                            everyone.now.delNode(id);
                        }else   {
                            // alta chestie
                        }
                    });
                } else {
                    // chestie
                }
            });

        } else {
            // chestie
        }

    });
};

everyone.now.serverDelBr = function(id)  {
    console.log('Deleting a branch. haha');
    mysql.query("DELETE FROM `nodes` WHERE id=?",[id], function(err, info) {
        if (!err) {
            everyone.now.delBr(id);
        } else {
            // chestie
        }
    });
}

everyone.now.serverUpdateNodeText = function (id, text) {
    console.log ('Updating text to : '+text+' for node '+id);
    mysql.query("UPDATE `nodes` SET text=? WHERE id=?",[text, id], function(err, info){
        if (!err) {
            everyone.now.updateNodeText(id, text);
        } else {
            // naspa
        }
    });
}

everyone.now.serverUpdateNode = function(id, text, x, y) {
    console.log(id +"\n"+ text +"\n"+ x +"\n"+ y);
    mysql.query("UPDATE `nodes` SET text=?, x=?, y=? WHERE id=?",[text, x, y, id], function(err, info){
        if (!err) {
            everyone.now.updateNode(id, text, x, y);
        } else {
            // naspa
        }
    });
}

everyone.now.serverLetterPress = function(id, text, sender)   {
    console.log("Lettering "+id+" on "+text);
    everyone.now.letterPress(id, text, sender);
}

