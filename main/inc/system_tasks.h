/**
 * @file system_tasks.h
 * @brief Gerenciador de Tarefas (Engine) do Sistema Operacional.
 * @author Rodrigues
 * @date 2026
 * * Esta biblioteca encapsula todas as tasks do FreeRTOS, filas, semáforos 
 * e mutexes. A main.c precisa apenas chamar a inicialização desta camada.
 */

#ifndef SYSTEM_TASKS_H_
#define SYSTEM_TASKS_H_

#include "esp_err.h"

/**
 * @brief Inicializa os recursos do RTOS e cria as tarefas do sistema.
 * * Cria internamente as filas, semáforos e mutexes necessários, inicializa 
 * a camada HAL e registra as 4 tarefas principais no scheduler do FreeRTOS.
 * * @return esp_err_t ESP_OK se o hardware e os recursos do RTOS forem criados com sucesso.
 */
esp_err_t system_tasks_init(void);

#endif /* SYSTEM_TASKS_H_ */