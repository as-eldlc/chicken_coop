<?php
    $file = "/tmp/coop_serial.txt";
    $c = "?";
    if (file_exists($file)){
        $c = file_get_contents($file);
    } else {
        $c = "? (file not found)";
    }

 echo($c);

?>