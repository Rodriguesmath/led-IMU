/**
 * @file bsp_potentiometer.c
 * @brief Implementação das leituras analógicas usando a API corrigida esp_adc (Oneshot mode).
 * @author Rodrigues
 * @date 2026
 */

#include "bsp_potentiometer.h"
#include "esp_adc/adc_oneshot.h"

static adc_oneshot_unit_handle_t adc1_handle;

void bsp_potentiometer_init(void) {
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,       /**< Resolução de 12 bits (0-4095) */
        .atten = ADC_ATTEN_DB_12,          /**< Atenuação para leitura de até 3.3V */
    };
    adc_oneshot_config_channel(adc1_handle, POT_ADC_CHANNEL, &config);
}

uint32_t bsp_potentiometer_read_raw(void) {
    int raw_val = 0;
    adc_oneshot_read(adc1_handle, POT_ADC_CHANNEL, &raw_val);
    return (uint32_t)raw_val;
}

uint32_t bsp_potentiometer_read_mv(void) {
    return (bsp_potentiometer_read_raw() * 3300) / 4095;
}