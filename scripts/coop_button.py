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
    GPIO.setup(n, GPIO.OUT, initial=GPIO.LOW)
    GPIO.output(n, GPIO.HIGH)
    time.sleep(1)
    GPIO.output(n, GPIO.LOW)


if __name__ == "__main__":
    but = sys.argv[1]
    GPIO.setmode(GPIO.BOARD)
    if but == "OPEN":
        print("Force open door")
        change_gpio(OPEN_DOOR)
    elif but == "CLOSE":
        print("Force close door")
        change_gpio(CLOSE_DOOR)
    elif but == "LED":
        print("Force led toggle")
        change_gpio(TOGGLE_LED)
    else:
        print("unknown command")