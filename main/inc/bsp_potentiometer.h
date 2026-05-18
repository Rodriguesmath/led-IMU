/**
 * @file bsp_potentiometer.h
 * @brief Board Support Package para leitura analógica de potenciômetro via driver ADC Oneshot.
 * @author Rodrigues
 * @date 2026
 */

#ifndef BSP_POTENTIOMETER_H_
#define BSP_POTENTIOMETER_H_

#include <stdint.h>

/** @brief Canal do ADC correspondente ao pino analógico utilizado (ADC1_CHANNEL_6 = GPIO34). */
#define POT_ADC_CHANNEL    ADC_CHANNEL_6 

/**
 * @brief Inicializa o conversor analógico-digital (ADC) no modo Oneshot.
 * * Configura o ADC1 com resolução de 12 bits (0 a 4095) e atenuação para escala de até 3.3V.
 */
void bsp_potentiometer_init(void);

/**
 * @brief Realiza a leitura do valor bruto do conversor ADC.
 * * @return uint32_t Valor digital bruto obtido entre 0 e 4095.
 */
uint32_t bsp_potentiometer_read_raw(void);

/**
 * @brief Realiza a leitura e converte o valor medido em milivolts.
 * * @return uint32_t Tensão calculada no pino em milivolts (0 a 3300 mV).
 */
uint32_t bsp_potentiometer_read_mv(void);

#endif /* BSP_POTENTIOMETER_H_ */