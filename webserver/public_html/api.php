<?php
// This is the API, 2 possibilities: show the visitor list or show a specific visitor by id.
// This would normally be pulled from a database but for demo purposes, I will be hardcoding the return values.

include("connect.php");
$arrayOfResults = array();


function get_visitor_by_id($id)
{
  //normally this info would be pulled from a database.
  //build JSON array
  $con = Connection();
  $sql = "SELECT * FROM ringings WHERE id = $id";
  $result = $con->query($sql);
  if($result->num_rows > 0){
		     while($row = $result->fetch_assoc()) {
		     $visitor_info = array("id" => $row["id"], "uid" =>$row["uid"], "timeStamp" => $row["timeStamp"], "reply" => $row["reply"],"base64Image" => $row["base64Image"]);
		     }
 }

  return $visitor_info;
}


function get_visitor_list()
{
  //normally this info would be pulled from a database.
  //build JSON array
  $con = Connection();
  $sql = "SELECT * FROM ringings ORDER BY id DESC";
  $result = $con->query($sql);
  if($result->num_rows > 0){
		     while($row = $result->fetch_assoc()) {
		     $arrayOfResults[] = array("id" => $row["id"],"uid" =>$row["uid"], "timeStamp" => $row["timeStamp"], "reply" => $row["reply"],"base64Image" => $row["base64Image"]);
		     }
 }
  
  $visitor_list = $arrayOfResults;

  return $visitor_list;
}

$possible_url = array("get_visitor_list", "get_visitor");

$value = "An error has occurred";

if (isset($_GET["action"]) && in_array($_GET["action"], $possible_url))
{
  switch ($_GET["action"])
    {
      case "get_visitor_list":
        $value = get_visitor_list();
        break;
      case "get_visitor":
        if (isset($_GET["id"]))
          $value = get_visitor_by_id($_GET["id"]);
        else
          $value = "Missing argument";
        break;
    }
}

//return JSON array
exit(json_encode($value));
?>
