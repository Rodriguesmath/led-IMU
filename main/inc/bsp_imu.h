/**
 * @file bsp_imu.h
 * @brief Board Support Package para comunicação I2C e conversão de dados do sensor inercial MPU6050.
 * @author Rodrigues
 * @date 2026
 */

#ifndef BSP_IMU_H_
#define BSP_IMU_H_

#include <stdint.h>
#include "esp_err.h"

#define I2C_MASTER_SDA_IO           21      /**< Pino SDA físico do barramento I2C */
#define I2C_MASTER_SCL_IO           22      /**< Pino SCL físico do barramento I2C */
#define MPU6050_ADDR                0x68    /**< Endereço I2C do MPU6050 */

/**
 * @struct imu_data_t
 * @brief Estrutura que armazena as acelerações convertidas em g para os três eixos espaciais.
 */
typedef struct {
    float x; /**< Aceleração no eixo X em g */
    float y; /**< Aceleração no eixo Y em g */
    float z; /**< Aceleração no eixo Z em g */
} imu_data_t;

/**
 * @brief Inicializa o barramento I2C em modo mestre e acorda o chip MPU6050.
 * * Define a frequência para 100kHz (Standard Mode), tira o sensor do modo sleep 
 * e parametriza a escala do acelerômetro em ±2g.
 * * @return esp_err_t Retorna ESP_OK em caso de sucesso na inicialização ou códigos de falha do I2C.
 */
esp_err_t bsp_imu_init(void);

/**
 * @brief Lê os registradores de aceleração da IMU e converte os valores brutos para g.
 * * Realiza uma transação única de escrita/leitura via I2C coletando os 6 bytes sequenciais de dados.
 * * @param[out] data Ponteiro para a estrutura onde os dados convertidos serão salvos.
 * @return esp_err_t ESP_OK em caso de sucesso na leitura do barramento.
 */
esp_err_t bsp_imu_read_accel(imu_data_t *data);

#endif /* BSP_IMU_H_ */