"""
Author : Antoine Scherrer
github repo : https://github.com/as-eldlc/chicken_coop
"""

import sys
import time

import RPi.GPIO as GPIO

OPEN_DOOR = 11
CLOSE_DOOR = 13
TOGGLE_LED = 15

GPIO.setwarnings(False)


def change_gpio(n):
    GPIO.setup(n, GPIO.OUT, initial=GPIO.LOW)
    GPIO.output(n, GPIO.HIGH)
    time.sleep(0.5)
    GPIO.output(n, GPIO.LOW)
    time.sleep(1)


if __name__ == "__main__":
    if len(sys.argv) > 1:
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
        elif but == "RESET":
            print("RESET GPIO")
            GPIO.setup(OPEN_DOOR, GPIO.OUT, initial=GPIO.LOW)
            GPIO.output(OPEN_DOOR, GPIO.LOW)
            GPIO.setup(CLOSE_DOOR, GPIO.OUT, initial=GPIO.LOW)
            GPIO.output(CLOSE_DOOR, GPIO.LOW)
            GPIO.setup(TOGGLE_LED, GPIO.OUT, initial=GPIO.LOW)
            GPIO.output(TOGGLE_LED, GPIO.LOW)
    else:
        print("Missing parameter")
    GPIO.cleanup()
