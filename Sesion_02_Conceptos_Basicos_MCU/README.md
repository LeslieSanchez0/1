
# Sesión 02: Conceptos Básicos de Microcontroladores — Memoria, Registros y Hardware

## Objetivo
Comprender la arquitectura interna de la Raspberry Pi Pico (RP2040 / RP2350), analizando cómo una instrucción de código se traduce en cambios de voltaje en los registros físicos y en el mapa de memoria. La práctica abarca la inspección de regiones de memoria (Flash, SRAM, Stack y Heap), el monitoreo del uso de memoria en MicroPython, la inspección de direcciones de memoria en C/C++, y la manipulación de GPIO mediante acceso directo a registros SIO frente al uso de abstracciones del Pico SDK.

---

## Arquitectura: RP2040 vs RP2350

| Característica | RP2040 / Pico W | RP2350 / Pico 2 W |
| :--- | :--- | :--- |
| **CPU** | 2x Arm Cortex-M0+ | 2x Arm Cortex-M33 o 2x Hazard3 (RISC-V) |
| **SRAM** | 264 KB | 520 KB |
| **PIO (State Machines)** | 8 máquinas de estado | 12 máquinas de estado |
| **Target Board SDK** | `pico_w` | `pico2_w` |
| **Enfoque** | Aprendizaje e IoT básico | Mayor memoria, potencia y seguridad |

---

## Mapa de Memoria

### Regiones Principales

| Dirección Inicio | Región | Descripción / Tipo de Datos |
| :--- | :--- | :--- |
| `0x00000000` | **ROM** | Código de arranque (*bootloader*). |
| `0x10000000` | **XIP / FLASH** | Almacenamiento no volátil del programa y constantes (`const`). |
| `0x20000000` | **SRAM** | Memoria volátil de ejecución para variables, buffers y objetos vivos. |
| `0x40000000` | **APB** | Control de periféricos lentos (UART, SPI, I2C). |
| `0x50000000` | **AHB** | Control de periféricos rápidos y bus del sistema. |
| `0xD0000000` | **SIO** | Periféricos de E/S rápidas (Single-cycle I/O) para control directo de GPIO. |

### Distribución de Datos en SRAM

* **Stack:** Almacena variables locales y llamadas a funciones; crece y decrece dinámicamente.
* **Heap:** Almacena memoria dinámica asignada en tiempo de ejecución (`malloc` / `bytearray`).

---

## Implementación y Ejercicios

### 1. MicroPython: Inspección de Memoria Dinámica (`memory_probe.py`)
Medición de la memoria libre en la SRAM antes, durante y después de la asignación de memoria dinámica.

```python
import gc, os, machine

print("Info del sistema:", os.uname())
print("Frecuencia CPU:", machine.freq(), "Hz")

gc.collect()
print("SRAM libre inicial:", gc.mem_free(), "bytes")

# Reserva de buffer en el Heap
buffer = bytearray(10000)
print("SRAM libre tras reservar 10,000 bytes:", gc.mem_free(), "bytes")

# Liberación y recolección de basura
del buffer
gc.collect()
print("SRAM libre tras liberar y gc.collect():", gc.mem_free(), "bytes")
