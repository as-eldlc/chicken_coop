"""
Author : Antoine Scherrer
github repo : https://github.com/as-eldlc/chicken_coop
"""
import logging
import time
from datetime import datetime
from logging.handlers import RotatingFileHandler

import serial
import sys

logger = logging.getLogger('chicken_coop')
hdlr = RotatingFileHandler('/tmp/coop_serial.log', mode='a', maxBytes=1024 * 1024, backupCount=10, encoding="UTF-8")
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
hdlr.setFormatter(formatter)
logger.addHandler(hdlr)
logger.setLevel(logging.DEBUG)

ch = logging.StreamHandler(sys.stdout)
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)

if __name__ == "__main__":
    while True:
        logger.info("Open serial port")
        message = ""
        try:
            with serial.Serial("/dev/ttyUSB0", 115200, timeout=0) as ser:
                logger.info("Serial port openned")
                while True:
                    try:
                        d = ser.read()
                        message += str(d.decode("utf-8"))
                    except Exception as e:
                        logger.error(str(e))
                        logger.info("Try again in 5 seconds")
                        time.sleep(5)
                        break
                    if "\n" in message:
                        message = message.replace("\r\n", "")
                        message = message.replace("\r", "")
                        message = message.replace("\n", "")
                        da = datetime.now()
                        file = open("/tmp/coop_serial.txt", "w")
                        file.write(da.strftime("%c") + ": " + message + "\n")
                        logger.info("NEW MESSAGE " + message)
                        message = ""
                        file.close()
        except Exception as e:
            logger.error(str(e))
            logger.info("Try again in 5 seconds")
            time.sleep(5)
            
