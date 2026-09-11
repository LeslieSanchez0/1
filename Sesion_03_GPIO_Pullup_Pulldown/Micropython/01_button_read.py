from machine import Pin
from time import sleep_ms

button=Pin(16, Pin.IN, Pin.PULL_UP)
print("Iniciando lectura de GP16 (Pull-up)")
while True:
    estado=button.value()
    print("button=",estado)
    sleep_ms(200)
