<?php

function Connection(){
$dbname = 'doorkeeper';
$dbuser = 'bouncer';
$dbpass = 'Dakommstnichtrein!';
$dbhost = 'localhost';

$connection = mysqli_connect($dbhost, $dbuser, $dbpass, $dbname) or die("Unable to Connect to '$dbhost'");

if (!$connection) {
    die("Connection failed: " . mysqli_connect_error());
}

		return $connection;
}
?>
