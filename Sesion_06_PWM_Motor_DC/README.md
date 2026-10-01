# Sesión 06: PWM y Control de Motor DC con L298N

Repositorio de evidencias, código y documentación correspondiente a la **Sesión 06** del Laboratorio de Elementos Programables. En esta práctica se implementó el control de velocidad, sentido de giro y rampas de aceleración para un motor DC utilizando una **Raspberry Pi Pico**, un **Puente H L298N** y **MicroPython**.
---

## Conceptos Clave

### 1. ¿Qué es PWM (Pulse Width Modulation)?
La Modulación por Ancho de Pulso (PWM) consiste en encender y apagar una señal digital a alta frecuencia para controlar la **energía promedio** entregada a una carga.
* **No es un voltaje analógico real:** La salida continúa oscilando entre 0 V y 3.3 V (en la Pico), pero varía el tiempo que permanece en estado alto (*Duty Cycle*).
* **Duty Cycle Alto:** Mayor energía promedio (mayor brillo en LED o mayor velocidad en el motor).
* **Duty Cycle Bajo:** Menor energía promedio.

### 2. Controladores y Asignación de Pines (IN1 / IN2 / ENA)
Para evitar dañar los pines del microcontrolador con los picos de corriente del motor, se utiliza un **Puente H L298N** como etapa de potencia. La Raspberry Pi Pico toma las decisiones lógicas y el L298N entrega la potencia.

* **IN1 y IN2 (Dirección):** Definen el sentido de giro del motor.
  * `0, 0` → Stop (Detenido)
  * `1, 0` → Forward (Giro adelante)
  * `0, 1` → Reverse (Giro en reversa)
  * `1, 1` → Freno / Stop
* **ENA / PWM (Velocidad):** Recibe la señal PWM para regular la velocidad de giro (0% a 100%).

#### Mapeo de Pines Físicos y Virtuales:
| Raspberry Pi Pico | L298N / Periférico | Descripción |
| :--- | :--- | :--- |
| **GP2** | `IN1` | Dirección de giro 1 |
| **GP3** | `IN2` | Dirección de giro 2 |
| **GP4** | `ENA` | Habilitador y control PWM de velocidad |
| **GND** | `GND` | Tierra común obligatoria (Pico y fuente externa) |

---

## Implementación: Rampas y Cambio Seguro de Dirección

### Rampa de Aceleración y Desaceleración
Para evitar cambios bruscos de corriente y esfuerzo mecánico en el motor, la velocidad no se aplica de golpe, sino mediante un algoritmo de rampa utilizando bucles y la función `range()`:
* **Rampa Ascendente:** Incremento progresivo de 0% a 100%.
* **Rampa Descendente:** Reducción progresiva de 100% a 0%.

### Regla de Seguridad: Cambio Seguro de Dirección
**Nunca se debe invertir el sentido de giro a alta velocidad.** Antes de cambiar de `FORWARD` a `REVERSE` (o viceversa), el sistema implementa una pausa obligatoria pasando el ciclo de trabajo a **0%** para proteger el circuito y el motor.

---

## Tabla de Pruebas (PASS / FAIL)

| Test / Estado | Comportamiento Esperado | Resultado |
| :--- | :--- | :--- |
| **STOP** | El motor se encuentra completamente detenido. | **PASS** |
| **FORWARD** | El motor gira en el sentido horario (adelante). | **PASS** |
| **REVERSE** | El motor gira en el sentido antihorario (reversa). | **PASS** |
| **25 / 50 / 75 / 100 %** | Cambio escalonado correcto en la velocidad del motor. | **PASS** |
| **Ramp UP** | Aceleración suave y progresiva desde 0% hasta 100%. | **PASS** |
| **Ramp DOWN** | Desaceleración suave y progresiva desde 100% hasta 0%. | **PASS** |
| **Cambio de dirección** | El motor desacelera a 0% antes de invertir el giro. | **PASS** |

---

## Problemas Encontrados y Soluciones

1. **El motor no gira:** 
   * *Causa:* Falta de tierra común (GND) entre la fuente externa y la Raspberry Pi Pico, o el puente ENA seguía puesto en la tablilla física.
   * *Solución:* Unir los GNDs y retirar el jumper ENA para conectar el pin PWM de la Pico.

2. **LEDs NO encendidos del L298N:** 
   * *Causa:* Desconexión o ausencia de referencia de tierra en el sistema.
   * *Solución:* Tierras conectadas.


