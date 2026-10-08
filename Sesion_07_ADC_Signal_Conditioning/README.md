# Sesión 07: Conversión ADC y Acondicionamiento de Señal
---

## Objetivo de la Práctica
Aprender a leer señales analógicas mediante el conversor analógico-digital (ADC) de la Raspberry Pi Pico, transformar los datos crudos en magnitudes físicas útiles (voltaje y porcentaje), aplicar un filtro de promedio móvil para estabilizar la lectura, y configurar un sistema de alarmas visuales por umbrales con LEDs.

---

### ¿Qué es un ADC y qué significa read_u16()?
El **ADC (Analog-to-Digital Converter)** permite que el microcontrolador deje de preguntar "¿encendido o apagado?" (como en los pines digitales GPIO) y empiece a medir cuánto voltaje hay de forma continua.
* read_u16(): Es el comando de MicroPython que devuelve una lectura cruda (raw) normalizada en un entero sin signo de 16 bits, abarcando un rango desde $0$ ($0\text{ V}$) hasta $65535$ ($3.3\text{ V}$ máx.).

---

**Fórmulas de Conversión**
Para traducir el valor crudo a unidades comprensibles, utilizamos reglas de tres proporcionales:
* **Voltaje ($V$):**
$$\text{voltage} = \frac{\text{raw} \times 3.3}{65535}$$
* **Porcentaje (%):**
$$\text{percent} = \frac{\text{raw} \times 100}{65535}$$

---

### Circuito utilizado
El sistema consta de una Raspberry Pi Pico conectada a un potenciómetro como emulador de sensor analógico y tres indicadores LED para las alertas.

* **Raspberry Pi Pico Pinout / Conexiones:** 
  * **GP26 (ADC0):** Conectado al pin central (SIG) del potenciómetro (Extremos a $3.3\text{V}$ y $\text{GND}$).
  * **GP13:** LED Verde (Estado Normal).
  * **GP14:** LED Amarillo (Estado de Advertencia).
  * **GP15:** LED Rojo (Estado de Alarma).

---
 
### Explicación del Filtro (Promedio Móvil)
Una señal analógica real puede contener ruido eléctrico o pequeñas variaciones que provocan saltos indeseados en las mediciones.

* El filtro de promedio móvil almacena una ventana de las últimas lecturas (por ejemplo, $N = 10$) y calcula la media aritmética de estas.
* Esto no elimina la física del sensor, pero previene que una sola lectura ruidosa o un pico transitorio dispare una alarma falsa, garantizando estabilidad en el sistema.

---

 ### Umbrales elegidos
El estado del sistema se clasifica en función del porcentaje calculado de la señal filtrada:

| Estado | Rango de Porcentaje | Acción / Salida Física |
| :--- | :--- | :--- |
| **NORMAL** | 0% a 49% | LED Verde encendido (50% umbral inferior) |
| **WARNING** | 50% a 74% | LED Amarillo encendido (Se acerca al límite) |
| **ALARM** | 75% a 100% | LED Rojo encendido (Acción requerida) |

---

### Tabla de Pruebas (PASS / FAIL)

| Prueba | Condición / Descripción | Valor Esperado | Resultado (PASS / FAIL) |
| :--- | :--- | :--- | :--- |
| **ADC Mínimo** | Potenciómetro girado al mínimo | $\text{raw} \approx 0$, $0\,\%$ ($0\text{ V}$) | **PASS** |
| **ADC Medio** | Potenciómetro en la posición central | $\text{raw} \approx 32767$, $50\,\%$ ($\approx 1.65\text{ V}$) | **PASS** |
| **ADC Máximo** | Potenciómetro girado al máximo | $\text{raw} \approx 65535$, $100\,\%$ ($\approx 3.3\text{ V}$) | **PASS** |
| **Filtro** | Variación rápida en la perilla | La señal filtrada cambia de forma suave y sin saltos | **PASS** |
| **Normal** | Porcentaje <50% | LED Verde activo, Amarillo y Rojo apagados | **PASS** |
| **Warning** | 50% >= Porcentaje <75%| LED Amarillo activo, Verde y Rojo apagados | **PASS** |
| **Alarm** | Porcentaje >=75% | LED Rojo activo, Verde y Amarillo apagados | **PASS** |
| **Recuperación** | Disminuir la señal desde ALARM | Vuelve de ALARM a NORMAL al descender el valor | **PASS** |

---


