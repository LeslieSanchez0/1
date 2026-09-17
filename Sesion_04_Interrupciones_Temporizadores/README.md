# Sesión 04: Interrupciones y Temporizadores (Juego de Reflejos)


## Descripción General
Este repositorio documenta el desarrollo de la Sesión 04 orientada a sistemas embebidos utilizando la Raspberry Pi Pico. El proyecto implementa un juego de interactividad y reflejos en entornos virtuales (Wokwi) y hardware real, aplicando conceptos avanzados de programación asíncrona como interrupciones por hardware (ISR), funciones *callback* en MicroPython, rutinas en C/C++ y temporizadores de un solo disparo para evitar el bloqueo del procesador.
---

## 1. Objetivo
Entender la diferencia entre el bloqueo por polling y la reacción inmediata mediante interrupciones (ISR). Programar callbacks en MicroPython e interrupciones en C/C++ para desarrollar un juego de reflejos funcional que mida el tiempo de reacción en milisegundos sin congelar el microcontrolador.

## 2. Circuito
El montaje se realizó utilizando los siguientes componentes y conexiones (tanto en Wokwi como en placa física):
* **GP15:** Conectado a una resistencia de 330 Ω y un LED blanco (señal visual) hacia GND.
* **GP14:** Conectado a una resistencia de 330 Ω y un LED azul (estado/espera) hacia GND.
* **GP16:** Conectado a un botón configurado con Pull-up interno (`Pin.PULL_UP`) hacia GND.
* **Evento de disparo:** `IRQ_FALLING` (detecta la transición de $1$ a $0$ al presionar el botón).

## 3. Qué es una interrupción
Una interrupción es un mecanismo por el cual el hardware avisa a la CPU que ha ocurrido un evento externo (en este caso, un cambio eléctrico en el pin del botón).
* A diferencia del *polling* (donde el programa pregunta continuamente en un ciclo `while True` desperdiciando ciclos de procesamiento), la interrupción ejecuta una función de atención llamada **ISR (Interrupt Service Routine)** solo cuando el evento sucede.
* **Regla de oro:** La ISR debe ser rápida y simple (solo cambiar banderas o guardar tiempos) para no bloquear ni desestabilizar el sistema.

## 4. Qué es un temporizador
Un temporizador (*Timer*) es un recurso del microcontrolador que permite programar acciones futuras sin congelar o bloquear el flujo principal del programa (a diferencia de usar un `sleep` bloqueante). En el juego de reflejos, se utiliza un temporizador de un solo disparo (`Timer.ONE_SHOT`) para esperar un tiempo aleatorio antes de encender el LED de señal y comenzar a medir el tiempo de reacción del usuario.

## 5. Resultados en ms
Registro de la ejecución de 5 intentos en el juego de reflejos:

| Intento | Tiempo (ms) | Observación |
| :---: | :---: | :--- |
| **1** | 245 ms | Respuesta correcta |
| **2** | 198 ms | Respuesta correcta |
| **3** | *Salida falsa* | Se presionó el botón antes de la señal |
| **4** | 280 ms | Respuesta correcta |
| **5** | 225 ms | Respuesta correcta |

* **Mejor tiempo:** 198 ms
* **Peor tiempo:** 280 ms
* **Promedio:** 237 ms
* **Salidas falsas detectadas:** 1

## 6. Problemas encontrados
* **Rebotes mecánicos (Bounce):** Al presionar el botón físico o virtual, el contacto generaba múltiples pulsos eléctricos falsos en microsegundos. Se solucionó implementando un control de *debounce* con `ticks_diff()` mayor a $80\text{ ms}$.
* **Pulsos invertidos:** Al principio el sistema no respondía debido a que no se había configurado correctamente la resistencia de pull-up interno (`Pin.PULL_UP`), haciendo que el estado de reposo no fuera el esperado ($1$ lógico).

## 7. Conclusión
El uso de interrupciones y temporizadores demuestra ser una solución eficiente y robusta frente al método tradicional de sondeo (*polling*). Aprendimos a separar responsabilidades: la ISR captura eventos críticos de forma inmediata, el temporizador gestiona los retardos asíncronos y el bucle principal se encarga de la lógica de presentación de datos, permitiendo construir aplicaciones interactivas y escalables tanto en MicroPython como en C/C++.
EOF
