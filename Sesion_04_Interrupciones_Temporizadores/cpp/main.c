#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_SIGNAL_PIN 15  // LED blanco (Señal de reacción)
#define LED_WAIT_PIN   14  // LED azul (Espera)
#define BUTTON_PIN     16  // Botón con resistencia Pull-Up interna

typedef enum {
    STATE_WAITING = 0,
    STATE_READY = 1,
    STATE_DONE = 2
} GameState;

volatile GameState state = STATE_DONE;
volatile uint32_t start_ms = 0;
volatile uint32_t reaction_ms = 0;
volatile bool result_ready = false;
volatile bool false_start = false;
volatile uint32_t last_irq_ms = 0;
int round_number = 0;

int64_t alarm_callback(alarm_id_t id, void *user_data);
alarm_id_t current_alarm = -1;

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    if (state == STATE_WAITING) {
        gpio_put(LED_SIGNAL_PIN, 1);
        gpio_put(LED_WAIT_PIN, 0);
        start_ms = to_ms_since_boot(get_absolute_time());
        state = STATE_READY;
    }
    current_alarm = -1;
    return 0; 

void schedule_round() {
    round_number++;
    gpio_put(LED_SIGNAL_PIN, 0);
    gpio_put(LED_WAIT_PIN, 1);
    result_ready = false;
    false_start = false;
    state = STATE_WAITING;
    uint32_t delay_ms = 1000 + (rand() % 9001);
    printf("\n===============================\n");
    printf("Round %d\n", round_number);
    printf("Espera la señal visual. No presiones antes.\n");
    printf("Delay aleatorio: %lu ms\n", delay_ms);
    printf("=================================\n");
  
    if (current_alarm >= 0) {
        cancel_alarm(current_alarm);
    }
    current_alarm = add_alarm_in_ms(delay_ms, alarm_callback, NULL, true);
}
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio != BUTTON_PIN) return;
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_irq_ms < 100) {
        return;
    }
    last_irq_ms = now;

    if (state == STATE_READY) {
        reaction_ms = now - start_ms;
        gpio_put(LED_SIGNAL_PIN, 0);
        result_ready = true;
        state = STATE_DONE;
    } 
    else if (state == STATE_WAITING) {
        if (current_alarm >= 0) {
            cancel_alarm(current_alarm);
            current_alarm = -1;
        }
        false_start = true;
        result_ready = true;
        state = STATE_DONE;
    }
}

int main() {
    stdio_init_all();
    sleep_ms(500);
    gpio_init(LED_SIGNAL_PIN);
    gpio_set_dir(LED_SIGNAL_PIN, GPIO_OUT);
    gpio_init(LED_WAIT_PIN);
    gpio_set_dir(LED_WAIT_PIN, GPIO_OUT);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    printf("JUEGO de LOS REFLEJOS (C Version)\n");
    schedule_round();
  
    while (true) {
        if (result_ready) {
            if (false_start) {
                printf("SALIDA EN FALSO: presionaste antes de la señal\n");
            } else {
                printf("Tiempo de reaccion: %lu ms\n", reaction_ms);
            }

            sleep_ms(1800);
            while (gpio_get(BUTTON_PIN) == 0) {
                sleep_ms(10);
            }
            sleep_ms(50);
            schedule_round();
        }
        sleep_ms(20);
    }

    return 0;
}
