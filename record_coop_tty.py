import serial

if __name__ == "__main__":
    message = ""
    with serial.Serial("/dev/ttyUSB0", 115200, timeout=0) as ser:
        print("Serial port openned")
        while True:
            try:
                d = ser.read()
                message += str(d.decode("utf-8"))
            except Exception as e:
                print(str(e))
                pass
            if "\n" in message:
                message = message.replace("\r\n","")
                message = message.replace("\r","")
                message = message.replace("\n","")
                file = open("/tmp/coop_serial.txt","w")
                file.write(message + "\n")
                print("NEW MESSAGE ", message)
                message = ""

