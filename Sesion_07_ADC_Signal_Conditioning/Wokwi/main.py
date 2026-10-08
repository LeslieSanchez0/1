from machine import Pin, ADC
from time import sleep_ms

sensor = ADC(Pin(26))   
white = Pin(13, Pin.OUT)       
yellow = Pin(14, Pin.OUT)       
red = Pin(15, Pin.OUT)         

VREF = 3.3   
WARNING = 50  
ALARM = 75   
WINDOW_SIZE = 10  
PERIODO_MS = 300  
window = []


def read_raw():
    return sensor.read_u16()   

def to_voltage(raw):
    return raw * VREF / 65535

def to_percent(raw):
    return raw * 100 / 65535

def filter_average(raw):
    window.append(raw)
    if len(window) > WINDOW_SIZE:
        window.pop(0)
    return sum(window) / len(window)

def classify(percent):
    if percent >= ALARM:
        return "ALARM"
    elif percent >= WARNING:
        return "WARNING"
    else:
        return "NORMAL"

def update_outputs(state):
    white.value(state == "NORMAL")
    yellow.value(state == "WARNING")
    red.value(state == "ALARM")

def print_status(raw, filtered, voltage, percent, state):
    print("raw:", raw, "| filtered:", int(filtered), "| V:", round(voltage, 2), "| %:", round(percent, 1), "| state:", state)

print("RETO 07 (BONUS) - Smart Analog Monitor con sensor de gas")
print("Sensor: gas, AOUT -> GP26 (ADC0) | LEDs: GP13 verde, GP14 amarillo, GP15 rojo")
print("Umbrales: WARNING >=", WARNING, "% | ALARM >=", ALARM, "% | Filtro:", WINDOW_SIZE, "lecturas")

update_outputs("")

while True:
    raw = read_raw()
    filtered = filter_average(raw)
    voltage = to_voltage(filtered)
    percent = to_percent(filtered)
    state = classify(percent)
    update_outputs(state)
    print_status(raw, filtered, voltage, percent, state)
    sleep_ms(PERIODO_MS)
