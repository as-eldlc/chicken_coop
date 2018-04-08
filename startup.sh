#!/bin/bash

sleep 60

cd /home/pi/chicken_coop
./stop_serial.sh

sleep 2

cd /home/pi/chicken_coop/set_time
platformio run -t upload

sleep 5

cd /home/pi/chicken_coop/main_coop
platformio run -t upload

sleep 5

cd /home/pi/chicken_coop
./start_serial.sh &
