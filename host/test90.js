var server = require("http");
var static = require("node-static");
var file = new(static.Server)("./www");

require("./WebMind.js");

WebMind.init('config.json');
console.log(WebMind, WebMind.config);

server.createServer(function(req, res) {
	req.addListener('end', function()	{
		file.serve(req, res);
	});
}).listen(80, '127.0.0.1');
