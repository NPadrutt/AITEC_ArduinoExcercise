<?php
    /**
     * Created by PhpStorm.
     * User: Adrian Kauz
     * Date: 16/12/2017
     * Time: 03:19
     */

    // Database Configuration. Add your details below
    // We know that "db_user" and "db_pass" are in cleartext here ;-)
    // It's only for an educational purpose.

    $dbOptions = array(
        'db_host' => 'localhost',
        'db_user' => 'bouncer',
        'db_pass' => 'Dakommstnichtrein!',
        'db_name' => 'doorkeeper'
    );

    /* Database Config End */

    require "classes/DB.class.php";
    require "classes/Handler.class.php";

    try {
        // Connecting to the database
        DB::init($dbOptions);

        $response = array();

        // Handling the supported actions:
        switch($_GET['action']) {
            case 'addNewEntry':
                $response = Handler::addNewEntry($_SERVER['REMOTE_ADDR'], $_SERVER['HTTP_USER_AGENT'], file_get_contents("php://input"));
                echo json_encode($response);
                break;

            case 'submitReply':
                $response = Handler::submitReply($_POST['reply']);
                echo json_encode($response);
                break;

            case 'getReply':
                echo Handler::getReply($_GET['UUID']);
                break;

            case 'getAllEntries':
                $response = Handler::getAllEntries();
                echo json_encode($response);
                break;
            default:
                throw new Exception('Wrong action');
        }
    }
    catch(Exception $e){
        die(json_encode(array('error' => $e->getMessage())));
    }
?>