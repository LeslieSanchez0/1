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


## Funcionamiento
El sistema ejecuta un flujo continuo de decisión y estados para garantizar un cruce seguro:  

1. **Estado S0 (Reposo):** Los vehículos tienen paso (**Auto Verde = 1**) y los peatones esperan (**Peatón Rojo = 1**).  
2. **Petición de Paso:** Al presionar el botón (`GP16` pasa a `0`), la CPU detecta la petición de cruce.  
3. **Filtro Debounce y Wait for Release:** Se aplica un retardo de $30\text{ ms}$ para validar la presión y un bucle de espera (`while button.value() == 0`) para evitar que mantener presionado el botón repita o encole secuencias.  
4. **Estado S1 (Transición):** El tráfico vehicular frena (**Auto Amarillo = 1** por 1.5 s, **Peatón Rojo = 1**).  
5. **Estado S2 (Cruce Peatonal):** Los autos se detienen y los peatones cruzan (**Auto Rojo = 1**, **Peatón Verde = 1** por 4 s).  
6. **Estado S3 (Fin de Cruce):** El LED verde peatonal parpadea (4 ciclos con toggle cada $300\text{ ms}$) antes de volver al reposo en S0.  

> **Invariante de Seguridad:** Auto Verde y Peatón Verde NUNCA están activos al mismo tiempo.  

---

## Pull-up / Pull-down

### El Pin Flotante
Una entrada digital no conectada queda en un estado no garantizado (*floating*), captando ruido ambiental al no estar fijada ni a $0\text{ V}$ ni a $3.3\text{ V}$.  

### Comparativa de Configuración

| Configuración | Circuito / Estado Libre | Estado Presionado | Aplicación en la Práctica |
| :--- | :--- | :--- | :--- |
| **Pull-Up** | Pin conectado a $3.3\text{ V}$ vía resistencia. Lectura por defecto = `1`. | Botón cierra circuito a `GND`. Lectura = `0`. | **Utilizado:** Usamos el Pull-Up interno (`GP16` -> Botón -> `GND`). |
| **Pull-Down** | Pin conectado a `GND` vía resistencia. Lectura por defecto = `0`. | Botón cierra circuito a $3.3\text{ V}$. Lectura = `1`. | Referencia técnica de laboratorio. |

---

## Pruebas Realizadas

Se aplicó el plan de pruebas técnico para validar la seguridad y estabilidad del semáforo:  

| Prueba / Evento | Comportamiento Esperado | Resultado Registrado |
| :--- | :--- | :-: |
| **Encender sistema** | Autos verde / Peatón rojo | **PASS** |
| **Pulsar una vez** | Ejecuta una secuencia completa de cruce | **PASS** |
| **Mantener botón** | No repite la secuencia inmediatamente (*wait-for-release*) | **PASS** |
| **Pulsar varias veces** | Mantiene estabilidad del sistema sin cierres o fallos | **PASS** |
| **Durante el cruce** | Nunca coinciden Auto Verde + Peatón Verde | **PASS**|

---

## Problemas Encontrados

1. **No le sé :(:**
   * *Problema:* Me costó muchísimo trabajo. El problema soy yo :,(

---

## Conclusión
El desarrollo de esta sesión demostró que los pines GPIO son el puente fundamental entre el software y el mundo físico. Se comprobó que el control de hardware exige más que la escritura de lógica digital: requiere gestionar fenómenos eléctricos reales como las referencias de voltaje por **Pull-Up/Pull-Down** para evitar el ruido de pines flotantes, así como filtrar mecánicamente las señales mediante rutinas de **debounce**. Asimismo, la abstracción mediante funciones y máquinas de estado permitió implementar una regla de seguridad crítica en el semáforo,, demostrando que una misma lógica algorítmica puede ser portada fácilmente entre MicroPython y C/C++ Pico SDK.
