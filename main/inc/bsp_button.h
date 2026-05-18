/**
 * @file bsp_button.h
 * @brief Board Support Package para gerenciamento de botões com tratamento de interrupção e debounce.
 * @author Rodrigues
 * @date 2026
 */

#ifndef BSP_BUTTON_H_
#define BSP_BUTTON_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/** @brief Pino GPIO conectado ao Botão. */
#define PIN_BTN 4

/**
 * @brief Inicializa o botão de entrada com interrupções ativas e associa o semáforo do sistema.
 * * Configura o pino como entrada em Pull-Up, registra o serviço de ISR e cria o Timer de
 * software do FreeRTOS para realizar o debounce por software.
 * * @param sys_semaphore Handler do semáforo binário que será liberado na confirmação do clique.
 */
void bsp_button_init(SemaphoreHandle_t sys_semaphore);

#endif /* BSP_BUTTON_H_ */