<?php

	require_once("Rest.inc.php");
	include("../connect.php");
	
	class API extends REST{
		public function __construct()
		{
			parent::__construct();				// Init parent contructor
		}
		/*
		 * Public method for access api.
		 * This method dynmically call the method based on the query string
		 *
		 */
		public function processApi()
		{
			$this->createEntry();
		}
		/* s
		 *	Insert new User into Database system
		 *  method : GET
		 *  data   : json data
		 *  Output : json data
		 */
		
		private function hello(){
    	echo str_replace("this","that","HELLO WORLD!!");
 		}
 		
		
		private function createEntry(){ //when Request already with Image
			$con = Connection();
	
			if(!$con)
			{
				$arr_res = array();
				$arr_res['error']  = array("msg" => "no DB Connection");
				$arr_res['result'] = array('status' => "Failed");
				$this->response($this->json($arr_res), 503);
		 	}
		 	$entityBody = file_get_contents('php://input');
		 	$myfile = fopen("newfile.txt", "w") or die("Unable to open file!");
			$txt = $entityBody;
			fwrite($myfile, $txt);
			fclose($myfile);

 			$data = $this->convert_json_to_array($entityBody);
 			
 			$myfile2 = fopen("newfile2.txt", "w") or die("Unable to open file!");
			$txt2 = $data;
			fwrite($myfile2, $txt2);
			fclose($myfile2);
			$timeStamp= date("Y-m-d H:i:s");
			$uid = $data['clientID'];
			$base64Image= $data['base64Image'];
 			$query = "INSERT INTO ringings (timeStamp, base64Image, uid) VALUES ('$timeStamp','$base64Image', '$uid')"; 
   
			if(!mysqli_query($con,$query))
			{
					$arr_res = array();
					$arr_res['error']  = array("msg" => "failed Adding Entry");
					$arr_res['result'] = array('status' => "Failed");
					$this->response($this->json($arr_res), 417);
			}
			else
			{
				$last_id = $con->insert_id;
				$str_array = array('status' => "Success" ,'id' => $last_id, 'reply' => Null, 'base64Image' => $base64Image, 'uid' => $uid);
				
				$arr_res = array();
				$arr_res['error']  = $error;
				$arr_res['result'] = $str_array;
				$this->response($this->json($arr_res), 200);	
				
				
		 		}
		 		
		 	$con->close();
		}
		
		
		
		
		private function updateReply(){ //For Adding Reply-Message to Container with ID
			$con = Connection();
	
			if(!$con)
			{
				$arr_res = array();
				$arr_res['error']  = array("msg" => "no DB Connection");
				$arr_res['result'] = array('status' => "Failed");
				$this->response($this->json($arr_res), 503);
		 	}

 			$data = $this->convert_json_to_array($this->_request['data']);
			$id= $data['id'];
			$reply= $data['reply'];
 			$query = "UPDATE ringings SET reply = '$reply' WHERE id = '$id'"; 
   
			if(!mysqli_query($con,$query))
			{
					$arr_res = array();
					$arr_res['error']  = array("msg" => "failed Adding Reply");
					$arr_res['result'] = array('status' => "Failed");
					$this->response($this->json($arr_res), 417);
			}
			else
			{
				$last_id = $con->insert_id;
				$str_array = array('status' => "Success" ,'id' => $id, 'reply' => $reply);
				
				$arr_res = array();
				$arr_res['error']  = $error;
				$arr_res['result'] = $str_array;
				$this->response($this->json($arr_res), 200);	
		 		}
		 	$con->close();
		}
		
		
		/*
		 *	Decode JSON into array
		*/
		private function convert_json_to_array($json)
		{		 
			return json_decode($json,true);
		}
		
		/*
		 *	Encode array into JSON
		*/
		private function json($data)
		{
			if (is_array($data))
			{
				return json_encode($data);
			}
		}
		
	}
	
	// Initiate Library
	$api = new API;
	$api->processApi();
?>
