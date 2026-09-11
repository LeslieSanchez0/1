from machine import Pin
from time import sleep_ms

button=Pin(16, Pin.IN, Pin.PULL_UP)
last=1
print("Presiona el botón para registrar")

while True:
    now=button.value()
    if last==1 and now==0:
        sleep_ms(30)
        if button.value()==0:
            print("CLICK válido")
            while button.value()==0:
                sleep_ms(10)
    last=now
    sleep_ms(10)
