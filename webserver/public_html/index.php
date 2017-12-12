<?php
 
 include("connect.php");

$url = 'http://192.168.178.101';
$con = Connection();
$sql = "SELECT * FROM ringings ORDER BY id DESC";
$result = $con->query($sql);

?>

<html>
   <head>
      <title>Doorkeeper</title>
   </head>
<body>
   <h1>Who rang the bell?</h1>
   
   
   <?php
if (isset($_GET["action"]) && isset($_GET["id"]) && $_GET["action"] == "get_visitor") 
{
  $visitor_info = file_get_contents($url.'/api.php?action=get_visitor&id=' . $_GET["id"]);
  $visitor_info = json_decode($visitor_info, true);
  ?>
  <div>
    <table>
      <tr>
        <td>id: </td><td> <?php echo $visitor_info["id"] ?></td>
      </tr>
      <tr>
        <td>Timestamp: </td><td> <?php echo $visitor_info["timeStamp"] ?></td>
      </tr>
      <tr>
        <td>Image: </td><td> <?php echo '<img width="100px" src="data:image/jpeg;base64,' . $visitor_info["base64Image"] . '" />' ?></td>
      </tr>
      
      <tr>
      	<td>Reply: </td><td>
      	<form action="update.php" method="post">
				<input tpye="text" name="id" value="<?php echo $visitor_info["id"] ?>" hidden="true"/><br />
		
		<!--Image: <input type="textarea" name="base64Image" value="" rows="4" cols="50"/><br />
		-->
				<textarea rows="3" cols="20" name="reply" ><?php echo $visitor_info["reply"] ?></textarea><br/>
			</td>
		</tr>
		<tr>
			<td>&nbsp;</td><td> 
			<input type="Submit" value="send reply" /><br />

	 		</form>
	 		</td>
	 </tr>
      
    </table>
    <br />
   </div>

<div>
	<a href="/?action=get_visitor_list"> Return to the visitor overview</a>
   <!-- <a href="'<?php echo 'api.php?action=get_visitor_list'; ?>'">Return to the visitorr overview</a> -->
</div>

  <?php
}
else // else take the visitor list
{
  $visitor_list = file_get_contents($url.'/api.php?action=get_visitor_list');
  $visitor_list = json_decode($visitor_list, true);
  ?>
  
  
  <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td>&nbsp;ID&nbsp;</td>
			<td>&nbsp;TimeStamp&nbsp;</td>
			<td>&nbsp;Image&nbsp;</td>
			<td>&nbsp;Reply&nbsp;</td>
		</tr>
		
    <ul>
    <?php 
		  foreach ($visitor_list as $visitor):
		        printf("<tr><td> &nbsp;%s </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td></tr>", 
		          $visitor["id"], '<a href="'.$url.'/?action=get_visitor&id='.$visitor["id"].'">'.$visitor["timeStamp"].'/>','<img width="100px" src="data:image/jpeg;base64,' . $visitor["base64Image"] . '" />', $visitor["reply"]);
		endforeach; ?>
   </table>

  <?php
} ?>
   

   
<!-- 
<form action="add.php" method="post">
Time: <input type="text" name="timeStamp" value="2017-12-08 00:04:40"/><br />
<input type="submit" name"add New Row"/><br/>
</form>
-->



</body>
</html>
