
import time
import RPi.GPIO as GPIO
dot = [1, 0]
dash = [1, 1, 1, 0]
let_brk = [0]*2
wor_brk = [0]*4
letter_dict = {"a": dot + dash,
                "b": dash + dot + dot + dot,
                "c": dash + dot + dash + dot,
                "d": dash + dot + dot,
                "e": dot,
                "f": dot + dot + dash + dot,
                "g": dash + dash + dot,
                "h": dot + dot + dot + dot,
                "i": dot + dot,
                "j": dot + dash + dash + dash,
                "k": dash + dot + dash,
                "l": dot + dash + dot + dot,
                "m": dash + dash,
                "n": dash + dot,
                "o": dash + dash + dash,
                "p": dot + dash + dash + dot,
                "q": dash + dash + dot + dash,
                "r": dot + dash + dot,
                "s": dot + dot + dot,
                "t": dash,
                "u": dot + dot + dash,
                "v": dot + dot + dot + dash,
                "w": dot + dash + dash,
                "x": dash + dot + dot + dash,
                "y": dash + dot + dash + dash,
                "z": dash + dash + dot + dot,
                "1": dot + dash + dash + dash + dash,
                "2": dot + dot + dash + dash + dash,
                "3": dot + dot + dot + dash + dash,
                "4": dot + dot + dot + dot + dash,
                "5": dot + dot + dot + dot + dot,
                "6": dash + dot + dot + dot + dot,
                "7": dash + dash + dot + dot + dot,
                "8": dash + dash + dash + dot + dot,
                "9": dash + dash + dash + dash + dot,
                "0": dash + dash + dash + dash + dash,
                " ": wor_brk}

def letter_to_morse(letter):

    return letter_dict[letter] + let_brk

def sequence_to_morse(sequence):
    sequence_in_morse = []
    for letter in sequence:
        sequence_in_morse += letter_to_morse(letter)
    return sequence_in_morse[:-2]
s=sequence_to_morse("why why why pi pi pi pip ip ip i pi p i pi pi p ip ip ipipi pi pi pi pi pi pi pi pi pip ip ip ip ip ip i pi pi pi pi pi pi i   jhj hj hj hj hj jhjhcjch ")

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
