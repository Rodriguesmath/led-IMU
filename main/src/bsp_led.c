/**
 * @file bsp_led.c
 * @brief Implementação dos recursos de hardware (LEDC/PWM) para controle de intensidade do LED.
 * @author Rodrigues
 * @date 2026
 */

#include "bsp_led.h"
#include "driver/ledc.h"

/**
 * @brief Inicializa o periférico LEDC (PWM) para o controle do LED.
 * * Configura o timer com resolução de 13 bits e frequência de 5kHz,
 * vinculando o canal ao pino configurado em #PIN_LED.
 */
void bsp_led_init(void) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_13_BIT, /**< Resolução de 13 bits exigida */
        .freq_hz          = 5000,              /**< Frequência de 5kHz */
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = PIN_LED,
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
}

/**
 * @brief Atualiza a intensidade do brilho do LED (Duty Cycle).
 * @param duty_cycle Valor do ciclo de trabalho mapeado em 13 bits (0 a 8191).
 */
void bsp_led_set_brightness(uint32_t duty_cycle) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_cycle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}