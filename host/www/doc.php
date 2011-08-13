<?php

mysql_connect('localhost','root','');
mysql_select_db('webmind');

if (isset($_GET['id'])) {
    $id = (int) $_GET['id'];
    if ($id===0) die('{0}');


    $query = "SELECT * FROM `nodes` WHERE doc=$id";
    $result = mysql_query($query);
    $return = array();
    while ($data = mysql_fetch_assoc($result)) {
        if (!isset($data['parent']) || $data['parent']===null)
            $data['parent']=0;
        $return[] = $data;
    }
    echo json_encode($return);

} else {
    $result = mysql_query("SELECT id, title FROM `docs`");
    $return = array();
    while ($data = mysql_fetch_assoc($result)) {
        $return[] = $data;
    }
    echo json_encode($return);    
}
mysql_close();
die();
