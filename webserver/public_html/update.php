
<?php

// required headers
//header("Access-Control-Allow-Origin: *");
//header("Content-Type: application/json; charset=UTF-8");
//header("Access-Control-Allow-Methods: POST");
//header("Access-Control-Max-Age: 3600");
//header("Access-Control-Allow-Headers: Content-Type, Access-Control-Allow-Headers, Authorization, X-Requested-With");

  	
   include("connect.php");
	$id= $_POST['id'];
	$base64Image = $_POST['base64Image'];
	$reply = $_POST['reply'];


	$con = Connection();
	
	if(!$con)
	{
		echo 'no DB connection';
 	}
 	// get posted data
	//$data = json_decode(file_get_contents("php://input"));
	//$timeStamp= $data ->timeStamp;
	//$base64Image= $data ->base64Image;

	//
	
	
	
		$query = "UPDATE ringings SET ";	
		if($base64Image <> "") {$query .= "base64Image = '$base64Image' ,";}
		if($reply != "") {$query .= "reply = '$reply' ,";}
		$query = trim($query, ','); //remove any trailing comma 
		$query .= "WHERE id = $id"; 
   
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

header("refresh:2; url=index.php");
	
?>
