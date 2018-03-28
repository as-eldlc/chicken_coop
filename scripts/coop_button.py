"""
Author : Antoine Scherrer
github repo : https://github.com/as-eldlc/chicken_coop
"""

import RPi.GPIO as GPIO
import time
import sys

OPEN_DOOR = 11
CLOSE_DOOR = 13
TOGGLE_LED = 15


def change_gpio(n):
    GPIO.setup(n, GPIO.OUT, pull_up_down=GPIO.PUD_UP)
    GPIO.output(n, GPIO.HIGH)
    time.sleep(2)
    GPIO.output(n, GPIO.LOW)


if __name__ == "__main__":
    but = sys.argv[1]
    GPIO.setmode(GPIO.BOARD)
    if but == "OPEN":
        change_gpio(OPEN_DOOR)
    elif but == "CLOSE":
        change_gpio(CLOSE_DOOR)
    elif but == "LED":
        change_gpio(TOGGLE_LED)
