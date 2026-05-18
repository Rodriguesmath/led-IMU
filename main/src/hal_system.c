/**
 * @file hal_system.c
 * @brief Implementação dos serviços de abstração e conversão de dados de hardware.
 * @author Rodrigues
 * @date 2026
 */

#include "hal_system.h"

/* Inclusão das BSPs internas (A HAL enxerga a BSP, mas a main não precisa mais enxergar) */
#include "bsp_led.h"
#include "bsp_button.h"
#include "bsp_potentiometer.h"
#include "bsp_imu.h"

esp_err_t hal_system_init(void *sys_semaphore) {
    esp_err_t err;

    /* Inicializa os periféricos básicos */
    bsp_led_init();
    bsp_potentiometer_init();
    bsp_button_init((SemaphoreHandle_t)sys_semaphore);

    /* Inicializa a IMU via I2C e checa se respondeu corretamente */
    err = bsp_imu_init();
    if (err != ESP_OK) {
        return err;
    }

    return ESP_OK;
}

void hal_led_set_percent(uint32_t percentage) {
    if (percentage > 100) percentage = 100;

    /* Converte a porcentagem (0-100%) para a resolução do PWM de 13 bits (0-8191) */
    uint32_t duty_cycle_13bit = (percentage * 8191) / 100;
    
    bsp_led_set_brightness(duty_cycle_13bit);
}

void hal_potentiometer_get_data(uint32_t *raw_value, uint32_t *voltage_mv) {
    if (raw_value != NULL) {
        *raw_value = bsp_potentiometer_read_raw();
    }
    if (voltage_mv != NULL) {
        *voltage_mv = bsp_potentiometer_read_mv();
    }
}

esp_err_t hal_imu_get_accel(hal_imu_data_t *imu_data) {
    if (imu_data == NULL) return ESP_ERR_INVALID_ARG;

    /* Cria uma estrutura temporária compatível com a BSP */
    imu_data_t bsp_data;
    esp_err_t err = bsp_imu_read_accel(&bsp_data);

    if (err == ESP_OK) {
        /* Copia os dados convertendo para a estrutura da HAL */
        imu_data->x = bsp_data.x;
        imu_data->y = bsp_data.y;
        imu_data->z = bsp_data.z;
    }

    return err;
}