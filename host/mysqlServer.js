

con = require("mysql").Client;

getDatabaseInfo = function()	{
	console.log(con);
}

connectToDatabase = function(json)	{
	con.user = json.user;
	con.password = json.password;
	con.database = json.database;
	console.log("Connected to the Database")
}
exports.connect = connectToDatabase;
exports.dbInfo = getDatabaseInfo;
