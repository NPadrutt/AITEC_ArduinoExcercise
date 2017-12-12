
<?php

// required headers
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: POST");
header("Access-Control-Max-Age: 3600");
header("Access-Control-Allow-Headers: Content-Type, Access-Control-Allow-Headers, Authorization, X-Requested-With");
//header("HTTP/1.1 
  	
   include("connect.php");
	$timeStamp= $_POST['timeStamp'];
	$base64Image= $_POST['base64Image'];


	$con = Connection();
	
	if(!$con)
	{
		echo 'no DB connection';
 	}
 	// get posted data
	$data = json_decode(file_get_contents("php://input"));
	//echo $data
	//$timeStamp= $data ->timeStamp;
	//$base64Image= $data ->base64Image;

	//
	$query = "INSERT INTO ringings (timeStamp, base64Image) VALUES ('$timeStamp','$base64Image')"; 
   
   if(!mysqli_query($con,$query))
   {
   	echo ' Not Inserted';
   }
   else
   {
   	echo 'successfully added';	
   }
   
   	mysqli_query($query,$link);
		mysqli_close($link);

header("refresh:0.1; url=index.php");
	
?>
