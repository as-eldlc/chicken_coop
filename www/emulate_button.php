<?php
    if (isset($_GET["button"]) && isset($_GET["pass"])){
        $button = $_GET["button"];
        $pass = $_GET["pass"];
        if ($pass == "grotte!"){
            echo exec('/usr/bin/python3 /home/pi/chicken_coop/scripts/coop_button.py ' . $button . '');
            echo "OK";
        } else {
            echo "bad password !";
        }
    }

?>
