/**
 * @file bsp_button.c
 * @brief Tratamento de interrupções de hardware e filtro de debounce para botão com Pull-Up externo.
 * @author Rodrigues
 * @date 2026
 */

#include "bsp_button.h"
#include "driver/gpio.h"
#include "freertos/timers.h"
#include "esp_attr.h"

#define DEBOUNCE_TIME_MS 50

static TimerHandle_t button_debounce_timer = NULL;
static SemaphoreHandle_t xButtonSemaphore = NULL;

static void IRAM_ATTR button_isr_handler(void* arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    if (gpio_get_level(PIN_BTN) == 0) { 
        gpio_intr_disable(PIN_BTN);
        xTimerStartFromISR(button_debounce_timer, &xHigherPriorityTaskWoken);
    }
    
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void button_debounce_callback(TimerHandle_t xTimer) {
    if (gpio_get_level(PIN_BTN) == 0) {
        if (xButtonSemaphore != NULL) {
            xSemaphoreGive(xButtonSemaphore);
        }
    }
    gpio_intr_enable(PIN_BTN);
}

void bsp_button_init(SemaphoreHandle_t sys_semaphore) {
    xButtonSemaphore = sys_semaphore;

    gpio_config_t btn_cfg = {
        .pin_bit_mask = (1ULL << PIN_BTN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE, 
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&btn_cfg);

    button_debounce_timer = xTimerCreate("btn_debounce", pdMS_TO_TICKS(DEBOUNCE_TIME_MS), pdFALSE, NULL, button_debounce_callback);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(PIN_BTN, button_isr_handler, NULL);
}