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
			$func = strtolower(trim(str_replace("/","",$_REQUEST['action'])));
			if ((int)method_exists($this,$func) > 0)
			{
				$this->$func();
			}
			else
			{
				$this->response('',404); // If the method not exist with in this class, response would be "Page not found".
			}
		}
		/* 
		 *	Insert new User into Database system
		 *  method : GET
		 *  data   : json data
		 *  Output : json data
		 */
		
		private function hello(){
    	echo str_replace("this","that","HELLO WORLD!!");
 		}
 		
 		private function createContainer(){ //when firstSending Request without Image
			$con = Connection();
	
			if(!$con)
			{
				$arr_res = array();
				$arr_res['error']  = array("msg" => "no DB Connection");
				$arr_res['result'] = array('status' => "Failed");
				$this->response($this->json($arr_res), 503);
		 	}

 			$data = $this->convert_json_to_array($this->_request['data']);
			$timeStamp= date("Y-m-d H:i:s");
 			$query = "INSERT INTO ringings (timeStamp) VALUES ('$timeStamp')"; 
   
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
				$str_array = array('status' => "Success" ,'id' => $last_id, 'reply' => Null, 'base64Image' => Null);
				
				$arr_res = array();
				$arr_res['error']  = $error;
				$arr_res['result'] = $str_array;
				$this->response($this->json($arr_res), 200);	
		 		}
		 	$con->close();
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

 			$data = $this->convert_json_to_array($this->_request['data']);
			$timeStamp= date("Y-m-d H:i:s");
			$base64Image= 'base64Image';
			//$base64Image= $data['base64Image'];
 			$query = "INSERT INTO ringings (timeStamp, base64Image) VALUES ('$timeStamp','$base64Image')"; 
   
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
				$str_array = array('status' => "Success" ,'id' => $last_id, 'reply' => Null, 'base64Image' => $base64Image);
				
				$arr_res = array();
				$arr_res['error']  = $error;
				$arr_res['result'] = $str_array;
				$this->response($this->json($arr_res), 200);	
				
				
		 		}
		 		
		 	$con->close();
		}
		
		
		private function updateImage(){ //For Adding image to Container with ID
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
			$base64Image= $data['base64Image'];
 			$query = "UPDATE ringings SET base64Image = '$base64Image' WHERE id = '$id'"; 
   
			if(!mysqli_query($con,$query))
			{
					$arr_res = array();
					$arr_res['error']  = array("msg" => "failed Adding Image");
					$arr_res['result'] = array('status' => "Failed");
					$this->response($this->json($arr_res), 417);
			}
			else
			{
				$last_id = $con->insert_id;
				$str_array = array('status' => "Success" ,'id' => $id, 'reply' => Null, 'base64Image' => $base64Image);
				
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
