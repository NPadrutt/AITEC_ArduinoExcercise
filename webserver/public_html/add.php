
<?php
  	
   include("connect.php");
	$timeStamp= date("Y-m-d H:i:s");
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
