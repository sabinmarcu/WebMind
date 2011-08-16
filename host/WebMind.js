fs = require("fs");

config = null;
database = null;
runner = 0;

initializeApplication = function(c)	{
	if (!c) c = "./config.json";
	getConfigFromFile(c, getDatabaseConnection);
}
getConfigFromFile = function(c, callback)	{
	fs.readFile(__dirname + "/" + c, function(err, data)	{
		console.log("Read the config file : " + data);
		getConfigFromString(data, callback);
	})
}
getConfigFromString = function(string, callback)	{
	config = JSON.parse(string);	
	console.log("Parsed the Config into : " + config);
	if (callback) callback();
}

getConfigInfo = function()	{
	if (!config) return false;
	console.log(config);
}
getDatabaseConnection = function()	{
	database = require(__dirname + "/mysqlServer");
	database.connect(config.mysql);
}
getDatabaseInfo = function()	{
	if (!database) return null;
	return database.dbInfo();
}





exports.getConfigInfo = getConfigInfo;
exports.getDatabseInfo = getDatabaseInfo;
exports.init = initializeApplication;
