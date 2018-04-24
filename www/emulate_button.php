<?php
    if (isset($_GET["button"]) && isset($_GET["pass"])){
        $button = $_GET["button"];
        $pass = $_GET["pass"];
        if (sha1($pass) == "245262042730d3520eedcc33de4870d99ce3be1e"){
            echo exec('/usr/bin/python3 /home/pi/chicken_coop/scripts/coop_button.py ' . $button . '');
            echo " OK";
        } else {
            echo "bad password !";
        }
    }

?>
