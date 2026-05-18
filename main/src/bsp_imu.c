/**
 * @file bsp_imu.c
 * @brief Implementação dos drivers I2C e formatação matemática dos dados da IMU.
 * @author Rodrigues
 * @date 2026
 */

#include "bsp_imu.h"
#include "driver/i2c.h"

#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          100000  /**< Frequência de 100kHz */

esp_err_t bsp_imu_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) return err;
    
    err = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    if (err != ESP_OK) return err;

    // Acorda o sensor desativando o bit de SLEEP no PWR_MGMT_1 (Reg 0x6B)
    uint8_t wake_cmd[] = {0x6B, 0x00};
    err = i2c_master_write_to_device(I2C_MASTER_NUM, MPU6050_ADDR, wake_cmd, sizeof(wake_cmd), pdMS_TO_TICKS(1000));
    if (err != ESP_OK) return err;

    // Configura a escala em ±2g (Sensibilidade = 16384 LSB/g) no ACCEL_CONFIG (Reg 0x1C)
    uint8_t scale_cmd[] = {0x1C, 0x00};
    return i2c_master_write_to_device(I2C_MASTER_NUM, MPU6050_ADDR, scale_cmd, sizeof(scale_cmd), pdMS_TO_TICKS(1000));
}

esp_err_t bsp_imu_read_accel(imu_data_t *data) {
    uint8_t reg_addr = 0x3B; 
    uint8_t raw_buffer[6];

    esp_err_t err = i2c_master_write_read_device(I2C_MASTER_NUM, MPU6050_ADDR, &reg_addr, 1, raw_buffer, 6, pdMS_TO_TICKS(100));
    if (err != ESP_OK) return err;

    int16_t raw_x = (raw_buffer[0] << 8) | raw_buffer[1];
    int16_t raw_y = (raw_buffer[2] << 8) | raw_buffer[3];
    int16_t raw_z = (raw_buffer[4] << 8) | raw_buffer[5];

    // Sensibilidade de 16384 LSB/g
    data->x = (float)raw_x / 16384.0f;
    data->y = (float)raw_y / 16384.0f;
    data->z = (float)raw_z / 16384.0f;

    return ESP_OK;
}