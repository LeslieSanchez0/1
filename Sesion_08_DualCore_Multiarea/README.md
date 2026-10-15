# Sesion 08: Dual-Core y Multitarea con MicroPython
---

## Descripción de la Práctica
Esta sesión aborda el paso de un programa secuencial bloqueante a un sistema embebido concurrente y multitarea utilizando la Raspberry Pi Pico (RP2040 / RP2350). El objetivo principal es aprender a monitorear un sensor crítico a alta velocidad sin congelar la interfaz de usuario, las alarmas o los mensajes por terminal.

---

## Conceptos Clave
* **¿Qué es la Multitarea / Concurrencia?:**
  Organizar diferentes tareas (lectura de sensores, actualización de interfaz, registro en terminal) para que avancen de manera intercalada sin bloquearse mutuamente, evitando los retrasos provocados por funciones de pausa larga como sleep_ms(1000).
  * **Simulación en Wokwi (Un solo núcleo):**
    Debido a que Wokwi simula la ejecución en un solo core para la Raspberry Pi Pico, se utiliza un   enfoque de temporización no bloqueante con ticks_ms() y ticks_diff() para alternar tareas a  intervalos específicos (ej. sensor cada $20\,\text{ms}$, UI cada $200\,\text{ms}$).
  * **Prueba Real con Pico Física (Dual-Core con _thread):**
    La placa física permite aprovechar el segundo núcleo real del RP2040:
        * Core 1: Tarea rápida dedicada exclusivamente a leer el ADC, aplicar filtros y actualizar             los datos compartidos.
        *Core 0: Tarea de interfaz encargada de gestionar los LEDs, la terminal serial, los botones           y las alarmas.
  * **Protección de Datos Compartidos (_thread.allocate_lock()):**
    Para evitar condiciones de carrera (lecturas y escrituras simultáneas e incompletas entre núcleos), se implementan cerrojos (lock.acquire() y lock.release()).
  * **Acondicionamiento de Señales (Filtro de Promedio Móvil):**
    Se utiliza un búfer de lecturas recientes para reducir el ruido eléctrico del sensor antes de       clasificar su estado en umbrales.

---

## Umbrales y Lógica de Estados
El valor analógico del potenciómetro ($0-100\%$) clasifica el estado del sistema en tres niveles:
* **NORMAL ($< 50\%$):** LED verde encendido. Sistema estable.
* **WARNING (50% - 74%):** LED amarillo encendido. Atención requerida.
* **ALARM ($\ge 75\%$):** LED rojo encendido. Condición crítica.
* **Heartbeat (LED azul):** Parpadea constantemente para demostrar que el sistema no se encuentra bloqueado.
---

### Plan de Pruebas y Resultados

| Prueba | Condición / Acción | Comportamiento Esperado | Resultado |
| :--- | :--- | :--- | :--- |
| **Potenciómetro Bajo** | Posición $< 50\%$ | LED verde activo, estado NORMAL | Éxito |
| **Potenciómetro Medio** | Posición $50\% - 74\%$ | LED amarillo activo, estado WARNING | Éxito |
| **Potenciómetro Alto** | Posición $\ge 75\%$ | LED rojo activo, estado ALARM | Éxito |
| **Heartbeat** | Funcionamiento continuo | El LED azul parpadea sin interrupciones | Éxito |
| **Terminal Serial** | Impresión periódica | Actualización fluida cada $1\,\text{s}$ sin retrasos | Éxito |
| **Filtro de Ruido** | Variaciones rápidas| Atenuación de fluctuaciones espurias en lecturas | Éxito |
| **Botón (Ack/Reset)** | Presionar pulsador | Reconocimiento de eventos y reseteo de alarma | Éxito |

---
## Conclusiones
* La simulación en Wokwi resulta fundamental para validar la lógica algorítmica y la programación basada en eventos no bloqueantes.
* Lo tqm Wicho, hubo pánico en la práctica, pero se logró :)  , nada se quemó.


---


