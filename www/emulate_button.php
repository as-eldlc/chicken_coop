<?php
    if (isset($_GET["button"])){
        $button = $_GET["button"];
        echo exec('/usr/bin/python3 /home/pi/chicken_coop/scripts/coop_button.py ' . $button . '');
    }

?>