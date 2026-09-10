#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

// Constante en memoria FLASH / XIP (rango 0x10000000)
const uint32_t ROM_DATA = 0x87654321;

// Variable global en memoria SRAM (rango 0x20000000)
uint32_t global_counter = 0;

int main() {
    // Configuración del puerto serie/USB
    stdio_init_all();
    sleep_ms(3000);
    // Variable local almacenada en la PILA (STACK)
    uint32_t stack_value = 0xABCDEF01;
    // Reserva de memoria dinámica en la MONTÍCULO (HEAP)
    uint8_t *heap_buffer = (uint8_t *)malloc(1024);
    printf("Flash const: %p\n", (void *) &flash_const);
    printf("Global var : %p\n", (void *) &global_counter);
    printf("Stack var  : %p\n", (void *) &stack_value);
    printf("Heap ptr   : %p\n", (void *) heap_buffer);
    // Liberación del bloque de memoria reservado
    free(dynamic_mem_ptr);
    while (true) {
        sleep_ms(1000);
    }

    return 0;
}
