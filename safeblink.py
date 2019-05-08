
import time
import RPi.GPIO as GPIO
dot = [1, 0]
dash = [1, 1, 1, 0]

wor_brk = [0]*4
letter_dict = {"1": dash,
               "0": dot.# 1 is a dot, 0 is a dash, the variation is in time. There will be a
               "  ": wor_brk}


def letter_to_morse(letter):

    return letter_dict[letter] + let_brk

def sequence_to_morse(sequence):
    sequence_in_morse = []
    for letter in sequence:
        sequence_in_morse += letter_to_morse(letter)
    return sequence_in_morse[:-2]
s=sequence_to_morse("1110 1100 1110 1000 1100 1100 1000 1000  ")

if __name__ == '__main__':
    pass
    #print sequence_to_morse("hello there")


class Safeguards:
    def __enter__(self):
        return self
    def __exit__(self,*rabc):
        GPIO.cleanup()
        print("Safe exit succeeded")
        return not any(rabc)

def prepare_pin(pin=17):
    GPIO.setmode(GPIO.BCM)  #use Broadcom (BCM) GPIO numbers on breakout pcb

    GPIO.setup(pin,GPIO.OUT) # allow pi to set 3.3v and 0v levels

def turn_high(pin):
    GPIO.output(pin,GPIO.HIGH)  # set 3.3V level on GPIO output

def turn_low(pin):
    GPIO.output(pin,GPIO.LOW)   # set ground (0) level on GPIO output

def delay(duration):            # sleep for duration seconds where duration is a float.
    time.sleep(duration)


def dot(duration,pin):
    turn_high(pin)
    delay(duration)
    turn_low(pin)
    delay(duration)
def dash(duration,pin):
    turn_high(pin)
    delay(duration*3)
    turn_low(pin)
    delay(duration)
def space_letter(duration):
    delay(duration*3)
def space_word(duration):
    delay(duration*7)
def blink(duration=0.05,pin=17):
    prepare_pin(pin)


    for i in range(len(s)):
        d=s[i]
        if d==1:
            turn_high(pin)
            delay(duration)
        elif d==0:
            turn_low(pin)
            delay(duration)






if __name__ == "__main__":
    with Safeguards():
        blink()
