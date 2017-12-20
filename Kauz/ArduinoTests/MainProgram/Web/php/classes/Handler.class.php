<?php
/**
 * Created by PhpStorm.
 * User: Adrian Kauz
 * Date: 16/12/2017
 * Time: 03:38
 */

class Handler
{
    /*
    ================
    addNewEntry()
    ================
    */
    public static function addNewEntry($deviceIP, $deviceAgent, $newJSONBody)
    {
        $data = json_decode($newJSONBody);

        DB::query("
                  INSERT INTO ringings (device_ip, device_agent, device_uuid, base64Image)
                  VALUES (
                      '".DB::esc($deviceIP)."',
                      '".DB::esc($deviceAgent)."',
                      '".DB::esc($data->{'device_uuid'})."',
                      '".DB::esc($data->{'base64Image'})."'
                  )");

        return DB::getMySQLiObject();
    }

    /*
    ================
    submitReply()
    ================
    */
    public static function submitReply($newJSONObject)
    {
        $replyObject = json_decode($newJSONObject);

        $result = DB::query("UPDATE ringings SET reply = '".DB::esc($replyObject->replyText)."' WHERE id = '".DB::esc($replyObject->id)."'");

        return array(
            'isOK' => true
        );
    }

    /*
    ================
    getReply()
    ================
    */
    public static function getReply($newUUID)
    {
        $result = DB::query("
                      SELECT reply
                      FROM ringings
                      WHERE device_uuid = '".DB::esc($newUUID)."'
                      ORDER BY id DESC
                      LIMIT 1")->fetch_object()->reply;

        if($result == null) {
            return "0";
        }
        else {
            return $result;
        }
    }

    /*
    ================
    getAllEntries()
    ================
    */
    public static function getAllEntries()
    {
        $result = DB::query("SELECT * FROM ringings ORDER BY id DESC");

        $entries = array();
        while($entry = $result->fetch_object()){
            $entry->time = array(
                'year'		    => gmdate('Y',strtotime($entry->created)),
                'month_name'    => gmdate('F',strtotime($entry->created)),
                'month_number'  => gmdate('m',strtotime($entry->created)),
                'day_name'      => gmdate('l',strtotime($entry->created)),
                'day_number'    => gmdate('d',strtotime($entry->created)),
                'hours'		    => gmdate('H',strtotime($entry->created)),
                'minutes'	    => gmdate('i',strtotime($entry->created)),
                'seconds'	    => gmdate('s',strtotime($entry->created))
            );

            $entries[] = $entry;
        }

        return $entries;
    }
}
?>