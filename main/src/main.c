/**
 * @file main.c
 * @brief Ponto de entrada do firmware de arquitetura ultra-mínima.
 * @author Rodrigues
 * @date 2026
 * * * Inicializa a biblioteca gerenciadora de tarefas e passa o controle
 * total da CPU para o kernel do FreeRTOS.
 */

#include <stdio.h>
#include "system_tasks.h"

void app_main(void) {
    /* Inicializa a biblioteca de tarefas, aloca os recursos e prepara o hardware */
    if (system_tasks_init() != ESP_OK) {
        printf("Falha Crítica na inicialização do motor de tarefas do sistema!\n");
        while(1); /* Trava por segurança caso falte memória ou hardware falhe */
    }

    /* * No ESP-IDF / ESP32, após o término da execução da função app_main(), 
     * a própria task principal do sistema se auto-destrói de forma limpa, 
     * enquanto as outras 4 tarefas criadas continuam rodando indefinidamente 
     * no scheduler do FreeRTOS.
     */
}