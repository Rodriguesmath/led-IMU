/**
 * @file bsp_led.h
 * @brief Board Support Package para controle do LED utilizando o periférico LEDC (PWM).
 * @author Rodrigues
 * @date 2026
 */

#ifndef BSP_LED_H_
#define BSP_LED_H_

#include <stdint.h>

/** @brief Pino GPIO conectado ao LED. */
#define PIN_LED 15

/**
 * @brief Inicializa o periférico LEDC (PWM) para o controle do LED.
 */
void bsp_led_init(void);

/**
 * @brief Atualiza a intensidade do brilho do LED (Duty Cycle).
 * @param duty_cycle Valor do ciclo de trabalho mapeado em 13 bits (0 a 8191).
 */
void bsp_led_set_brightness(uint32_t duty_cycle);

#endif /* BSP_LED_H_ */