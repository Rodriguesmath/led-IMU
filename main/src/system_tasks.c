/**
 * @file system_tasks.c
 * @brief Implementação e gerenciamento das rotinas internas das Tarefas do RTOS.
 * @author Rodrigues
 * @date 2026
 */

#include "system_tasks.h"
#include "hal_system.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/* --- Definições de Configuração Interna das Tasks --- */
#define TASK_POT_STACK_SIZE     2048
#define TASK_LED_STACK_SIZE     2048
#define TASK_IMU_STACK_SIZE     3072
#define TASK_CONSOLE_STACK_SIZE 3072

#define TASK_POT_PRIORITY       2 /**< Prioridade Média/Baixa para o potenciômetro. */
#define TASK_LED_PRIORITY       3 /**< Prioridade Média/Alta para atualização do LED. */
#define TASK_IMU_PRIORITY       3 /**< Prioridade Média/Alta para amostragem do I2C. */
#define TASK_CONSOLE_PRIORITY   1 /**< Prioridade Baixa para escrita na UART. */

/* --- Primitivos de Sincronização Locais (Escondidos da Main) --- */
static QueueHandle_t xPotQueue = NULL;        /**< Fila de controle do LED. */
static SemaphoreHandle_t xBtnSemaphore = NULL; /**< Semáforo binário do botão de HOLD. */
static SemaphoreHandle_t xImuMutex = NULL;     /**< Mutex para proteção de leitura/escrita da IMU. */

/**
 * @enum system_state_t
 * @brief Máquina de estados interna do firmware.
 */
typedef enum { 
    STATE_LIVE, 
    STATE_HOLD  
} system_state_t;

/* --- Variáveis de Estado Internas --- */
static system_state_t g_system_state = STATE_LIVE;
static hal_imu_data_t g_shared_imu_data = {0.0f, 0.0f, 0.0f};
static uint32_t g_current_pot_raw = 0;
static uint32_t g_current_pot_mv = 0;
static uint32_t g_current_led_percentage = 0;

/* --- Declaração das Funções de Task (Locais/Static) --- */
static void vTaskPotentiometer(void* pvParameters);
static void vTaskLED(void* pvParameters);
static void vTaskIMU(void* pvParameters);
static void vTaskConsole(void* pvParameters);

esp_err_t system_tasks_init(void) {
    xPotQueue = xQueueCreate(5, sizeof(uint32_t));
    xBtnSemaphore = xSemaphoreCreateBinary();
    xImuMutex = xSemaphoreCreateMutex();

    if ((xPotQueue == NULL) || (xBtnSemaphore == NULL) || (xImuMutex == NULL)) {
        return ESP_ERR_NO_MEM;
    }

    esp_err_t err = hal_system_init(xBtnSemaphore);
    if (err != ESP_OK) {
        return err;
    }


    xTaskCreate(vTaskPotentiometer, "Task_Pot", TASK_POT_STACK_SIZE,     NULL, TASK_POT_PRIORITY,     NULL);
    xTaskCreate(vTaskLED,           "Task_LED", TASK_LED_STACK_SIZE,     NULL, TASK_LED_PRIORITY,     NULL);
    xTaskCreate(vTaskIMU,           "Task_IMU", TASK_IMU_STACK_SIZE,     NULL, TASK_IMU_PRIORITY,     NULL);
    xTaskCreate(vTaskConsole,       "Task_Con", TASK_CONSOLE_STACK_SIZE, NULL, TASK_CONSOLE_PRIORITY, NULL);

    return ESP_OK;
}

/* ========================================================================
   --- Implementação das Rotinas de Loop das Tasks (Escopo Local) ---
   ======================================================================== */

static void vTaskPotentiometer(void* pvParameters) {
    uint32_t raw, mv;
    while(1) {
        hal_potentiometer_get_data(&raw, &mv);
        g_current_pot_raw = raw;
        g_current_pot_mv = mv;

        uint32_t target_percentage = (raw * 100) / 4095;
        xQueueSend(xPotQueue, &target_percentage, 0);

        vTaskDelay(pdMS_TO_TICKS(50)); 
    }
}

static void vTaskLED(void* pvParameters) {
    uint32_t target_percent = 0;
    while(1) {
        if (xSemaphoreTake(xBtnSemaphore, 0) == pdTRUE) {
            g_system_state = (g_system_state == STATE_LIVE) ? STATE_HOLD : STATE_LIVE;
        }

        if (xQueueReceive(xPotQueue, &target_percent, pdMS_TO_TICKS(20)) == pdTRUE) {
            if (g_system_state == STATE_LIVE) {
                g_current_led_percentage = target_percent;
                hal_led_set_percent(g_current_led_percentage);
            }
        }
    }
}

static void vTaskIMU(void* pvParameters) {
    hal_imu_data_t local_data;
    while(1) {
        if (hal_imu_get_accel(&local_data) == ESP_OK) {
            if (xSemaphoreTake(xImuMutex, portMAX_DELAY) == pdTRUE) {
                g_shared_imu_data = local_data;
                xSemaphoreGive(xImuMutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(40)); 
    }
}

static void vTaskConsole(void* pvParameters) {
    hal_imu_data_t display_imu;
    while(1) {
        if (xSemaphoreTake(xImuMutex, portMAX_DELAY) == pdTRUE) {
            display_imu = g_shared_imu_data;
            xSemaphoreGive(xImuMutex);
        }

        /* Formatação de saída do console requisitada */
        printf("=====================================================\n");
        printf("STATUS: [%s] | POT: %lu (%lu mV) | LED: %lu%%\n", 
               (g_system_state == STATE_LIVE) ? "LIVE" : "HOLD",
               g_current_pot_raw, g_current_pot_mv, g_current_led_percentage);
        printf("IMU ACCEL (g): X: %.2f | Y: %.2f | Z: %.2f\n", 
               display_imu.x, display_imu.y, display_imu.z);
        printf("=====================================================\n\n");

        vTaskDelay(pdMS_TO_TICKS(200)); 
    }
}