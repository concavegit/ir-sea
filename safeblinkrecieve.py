import time
import RPi.GPIO as GPIO

class Safeguards:
    def __enter__(self):
        return self
    def __exit__(self,*rabc):
        GPIO.cleanup()
        print("Safe exit succeeded")
        return not any(rabc)
def prepare_pin(pin=23):
    GPIO.setmode(GPIO.BCM)  #use Broadcom (BCM) GPIO numbers on breakout pcb

    GPIO.setup(pin,GPIO.IN) # allow pi to read levels

def read_pin(pin):
    return GPIO.input(pin)  # set 3.3V level on GPIO output

def delay(duration):            # sleep for duration seconds where duration is a float.
    time.sleep(duration)

def receive(blinks=40,duration=1,pin=23):
    prepare_pin(pin)
    s=[]

    for i in range(blinks):
        if read_pin(pin):
            s.append(1)
        else:
            s.append(0)
       # print(s)
        print("|{}".format("==" if read_pin(pin) else ""))
        delay(duration)
    print(s)
