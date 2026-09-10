# Sesión 03: GPIO, Entrada/Salida Digital y Resistencias Pull-Up / Pull-Down

## Descripción General
Este repositorio contiene el desarrollo práctico y la documentación técnica de la **Sesión 03**, centrada en la interfaz entre software y hardware mediante pines GPIO en la tarjeta **Raspberry Pi Pico**. 

En esta práctica se aborda la lectura de entradas digitales, la resolución del problema de *pines flotantes* mediante configuraciones **Pull-Up/Pull-Down**, el filtrado de rebotes mecánicos (*debounce*) por software y la implementación de un **semáforo peatonal interactivo seguro**. Los ejemplos y retos fueron desarrollados en **MicroPython** y transferidos conceptualmente a **C/C++ (Pico SDK)**.

---

## Objetivos de la Sesión
1. Comprender la frontera entre software y el mundo físico mediante el uso de pines GPIO configurados como entradas (`GPIO_IN`) y salidas (`GPIO_OUT`).
2. Identificar el problema de un *pin flotante* y solucionarlo aplicando resistencias de **Pull-Up** y **Pull-Down** (internas y externas).
3. Implementar técnicas de **debounce** por software y el patrón *wait-for-release* para garantizar la estabilidad en la lectura de pulsadores mecánicos.
4. Diseñar e implementar un sistema de control por estados para un **semáforo peatonal interactivo**, asegurando la invariante de seguridad (evitar verde de autos y verde de peatones simultáneos).
5. Validar la lógica mediante simulación en **Wokwi** y pruebas en la tarjeta física **Raspberry Pi Pico**.

---

## Diagrama y Conexión del Circuito

El circuito consta de dos etapas principales:
* **Etapa A:** Lectura de botón en GP16 con Pull-Up interno conectado a GND.
* **Etapa B:** Módulo de LEDs del semáforo con resistencias limitadoras de corriente de $330\,\Omega$ conectadas a GND.

### Mapeo de Pines (Pinout)

| Componente | Pin GPIO | Configuración GPIO | Estado / Acción |
| :--- | :---: | :---: | :--- |
| **Pulsador Peatonal** | `GP16` | Entrada (`IN`, `PULL_UP`) | Presionado = `0` (GND), Libre = `1` (3.3V) |
| **LED Auto Rojo** | `GP15` | Salida (`OUT`) | `1` = Encendido, `0` = Apagado |
| **LED Auto Amarillo**| `GP14` | Salida (`OUT`) | `1` = Encendido, `0` = Apagado |
| **LED Auto Verde** | `GP13` | Salida (`OUT`) | `1` = Encendido, `0` = Apagado |
| **LED Peatón Rojo** | `GP12` | Salida (`OUT`) | `1` = Encendido, `0` = Apagado |
| **LED Peatón Verde** | `GP11` | Salida (`OUT`) | `1` = Encendido, `0` = Apagado |

### Esquema Electrónico
```text
               +----------------------------------+
               |        Raspberry Pi Pico         |
               |                                  |
               |  [GP16] <--- Botón ---> GND      |
               |                                  |
               |  [GP15] ---> [330Ω] ---> LED Auto Rojo  ---> GND
               |  [GP14] ---> [330Ω] ---> LED Auto Amar. ---> GND
               |  [GP13] ---> [330Ω] ---> LED Auto Verde ---> GND
               |  [GP12] ---> [330Ω] ---> LED Peatón Rojo---> GND
               |  [GP11] ---> [330Ω] ---> LED Peatón Verd---> GND
               +----------------------------------+
