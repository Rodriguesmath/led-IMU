/**
 * @file hal_system.h
 * @brief Camada de Abstração de Hardware (HAL) para o sistema multitarefa.
 * @author Rodrigues
 * @date 2026
 * * * Esta camada abstrai as chamadas diretas da BSP, unificando a inicialização 
 * do hardware e fornecendo funções limpas de leitura e escrita para a main.c.
 */

#ifndef HAL_SYSTEM_H_
#define HAL_SYSTEM_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/**
 * @struct hal_imu_data_t
 * @brief Estrutura unificada na HAL para dados do acelerômetro.
 */
typedef struct {
    float x; /**< Aceleração em g no eixo X */
    float y; /**< Aceleração em g no eixo Y */
    float z; /**< Aceleração em g no eixo Z */
} hal_imu_data_t;

/**
 * @brief Inicializa todo o ecossistema de hardware do dispositivo.
 * * Abstrai a inicialização sequencial de todos os periféricos (LED, Botão, Potenciômetro e IMU).
 * @param sys_semaphore Semáforo binário do RTOS para repassar ao driver do botão.
 * @return esp_err_t Retorna ESP_OK se todos os periféricos vitais subirem com sucesso.
 */
esp_err_t hal_system_init(void *sys_semaphore);

/**
 * @brief Define a intensidade luminosa do LED de status.
 * @param percentage Valor em porcentagem (0 a 100%) desejado para o brilho.
 */
void hal_led_set_percent(uint32_t percentage);

/**
 * @brief Lê o valor atual do potenciômetro de forma estruturada.
 * @param[out] raw_value Ponteiro para salvar o valor bruto do ADC (0-4095).
 * @param[out] voltage_mv Ponteiro para salvar a tensão convertida em milivolts.
 */
void hal_potentiometer_get_data(uint32_t *raw_value, uint32_t *voltage_mv);

/**
 * @brief Coleta as acelerações convertidas do sensor inercial.
 * @param[out] imu_data Ponteiro para a estrutura onde serão salvos os eixos X, Y e Z.
 * @return esp_err_t ESP_OK em caso de sucesso na transação de leitura.
 */
esp_err_t hal_imu_get_accel(hal_imu_data_t *imu_data);

#endif /* HAL_SYSTEM_H_ */