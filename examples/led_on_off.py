# script.py generated by GPIO Studio v0.0.5
import gpiozero
import time
MyLED1 = gpiozero.LED(2)
MyLED1.off()
time.sleep(5)
MyLED1.on()