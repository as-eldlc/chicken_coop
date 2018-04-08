#!/bin/bash

cd /home/pi/chicken_coop
./stop_serial.sh

cd /home/pi/chicken_coop/main_coop
platformio run -t upload

sleep 5

cd /home/pi/chicken_coop
./start_serial.sh &
