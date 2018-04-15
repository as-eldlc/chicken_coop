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
import smtplib

from email.message import EmailMessage

def send_email(message):
        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()
        server.login("coop.grotte", "grotte2015!")
        msg = EmailMessage()
        msg['From'] = "coop.grotte@gmail.com"
        msg['To'] = "picakoch@gmail.com"
        msg['Subject'] = "ALERTE POULAILLER"

        msg.set_content("Pb au Poulailler " + message)
        server.send_message(msg)
        server.quit()




if __name__ == "__main__":
    send_email("ahou")
