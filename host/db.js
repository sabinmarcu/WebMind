var connected=false;
this.connect = function() {
    mysql.user = 'root';
    mysql.password = 'ion';
    mysql.database = 'mindstuff';
    mysql.connect();
    if (!mysql.err) {
        this.connected=true;
        return true;
    } else {
        return false;
    }
}
