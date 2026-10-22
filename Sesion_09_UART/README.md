# Sesión 09: Comunicación UART
---

## Descripción de la Práctica
A continuación se presenta la documentación, arquitectura de código y plan de pruebas para la implementación de un protocolo de comunicación serial asíncrona (UART) utilizando MicroPython en la Raspberry Pi Pico 2W.
---

## ¿Qué es UART?
UART (Universal Asynchronous Receiver-Transmitter) es un protocolo de comunicación serial asíncrona que permite la transferencia de datos punto a punto bit a bit. Es ampliamente utilizado para conectar microcontroladores con periféricos, módulos de comunicación (Bluetooth, GPS, Wi-Fi) u otras computadoras.

---

## Pines TX y RX
* **TX (Transmit):** Pin encargado de transmitir los datos binarios hacia afuera del dispositivo.
* **RX (Receive):** Pin encargado de recibir los datos binarios entrantes.
  **Regla Física Fundamental:** En una conexión directa, **TX siempre se cruza hacia RX ($TX \rightarrow    RX$) y RX proviene de TX ($RX \leftarrow TX$)**, compartiendo además una línea de referencia común    **(GND)**. Nunca debe conectarse **$TX \rightarrow TX$ o $RX \rightarrow RX$.**
---

## Baud Rate y 8N1
Para que la comunicación asíncrona funcione correctamente sin un cable de reloj compartido, ambos dispositivos deben acordar los siguientes parámetros:
* **Baud Rate (9600 bps):** Determina la velocidad de transmisión (bits por segundo). A 9600 bps se transmiten aproximadamente 960 bytes/s. Si los baud rates no coinciden, la trama se interpreta como ruido o caracteres corruptos.
* **Trama 8N1:** Define la estructura de empaquetado de cada byte transmitido:
    * **8:** 8 bits de datos por carácter.
    * **N:** None (sin bit de paridad).
    * **1:** 1 bit de parada (stop bit).
    * (Incluye 1 bit de inicio / start bit, dando un total de $\approx 10$ bits por byte transmitido).
---
### USB/REPL vs UART Hardware
Es crucial diferenciar los canales físicos y lógicos de comunicación en la Raspberry Pi Pico:

| Característica | USB / REPL (print) | UART Hardware (UART0) |
| :--- | :--- | :--- |
| **Canal Físico** | Cable USB | Pines GPIO (GP0/GP1) |
| **Monitoreo Thonny** | Aparece directamente en la consola por USB. | No se muestra automáticamente en Thonny. |
| **Uso Principal** | Depuración y consola interactiva de MicroPython. | Comunicación con periféricos externos y otros microcontroladores. |

---

## Configuración Loopback (GP0 → GP1)
Dado que Thonny monitorea únicamente la interfaz USB, utilizamos un puente físico (jumper) entre GP0 (TX) y GP1 (RX). De esta forma, cada instrucción que el programa envía por UART0 sale por GP0 y regresa inmediatamente por GP1. Esto permite comprobar que el periférico UART hardware funciona de forma independiente al canal USB.

---
### Arquitectura del Protocolo
Para evitar que una cadena de texto se convierta en ruido sin sentido, la aplicación implementa un parser estricto basado en tramas delimitadas.

#### Formato de Trama
Los comandos enviados deben seguir la estructura:
ACCIÓN, DISPOSITIVO, VALOR\N
* **Ejemplo de comando SET:** SET,LED1,1
    *° SET: Acción a realizar.
    *° LED1: Dispositivo destino.
    *° 1: Estado solicitado (1 = ON, 0 = OFF).

---

## ACK, ERR y STATUS
El sistema procesa la trama recibida y emite tres tipos de respuestas estandarizadas:
* ACK (Acknowledge): Confirmación de comando procesado correctamente.
    * Respuesta: ACK,LED1,1
* STATUS: Reporte del estado actual de todos los actuadores.
    * Respuesta: STATUS,LED1,1,LED2,0
* ERR (Error): Trama inválida, parámetros fuera de rango o dispositivo no encontrado.
    * Ejemplo: ERR,BAD_SET_FRAME o ERR,INVALID_VAL

  ### Esquema de Conexiones
Es crucial diferenciar los canales físicos y lógicos de comunicación en la Raspberry Pi Pico:

| Elemento | Pin Pico | Conexión / Destino |
| :--- | :--- | :--- |
| **UART0 TX** | GP0 | Jumper hacia GP1 (Loopback) |
| **UART0 RX** | GP1 | Jumper desde GP0 (Loopback) |
| **Actuador LED 1** | GP13 | Ánodo LED 1 (con resistencia a GND) |
| **Actuador LED 2** | GP14 | Ánodo LED 2 (con resistencia a GND) |
| **Alimentación/REPL** | USB | Computadora (Thonny IDE) |
---

## Conclusiones
* Se muestra la diferencia entre el canal de depuración USB/REPL y el periférico UART hardware (UART0). Este diferenciador es importante para el diseño de sistemas embebidos donde la consola de desarrollo no debe interferir con la comunicación entre dispositivos.

Saluditos Wichoooo 🦆
---



