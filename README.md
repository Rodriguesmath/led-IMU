# led-IMU

[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-compatible-blue)](https://docs.espressif.com/projects/esp-idf/)
[![Linguagem](https://img.shields.io/badge/C-embedded-lightgrey)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Documentação](https://img.shields.io/badge/docs-Doxygen-2b7a78)](https://rodriguesmath.github.io/led-IMU/html/index.html)

Projeto embarcado em ESP-IDF para um sistema RTOS com leitura de potenciômetro, controle de LED por PWM, captura de dados de IMU e saída periódica no console serial.

## Visão Geral

O firmware foi organizado em camadas para manter o código simples de manter:

- `main.c` apenas inicializa a engine de tarefas do sistema.
- `system_tasks.c` cria e coordena as tarefas FreeRTOS.
- `hal_system.c` centraliza a abstração de hardware.
- Os arquivos `bsp_*` fazem o acesso direto aos periféricos.

O fluxo principal do sistema é:

1. Inicializar a infraestrutura do hardware.
2. Ler o potenciômetro periodicamente.
3. Atualizar o brilho do LED com base na leitura.
4. Ler a IMU via I2C e manter os dados compartilhados protegidos por mutex.
5. Exibir o estado do sistema na UART em intervalos regulares.

## Funcionalidades

- Leitura analógica de potenciômetro via ADC em modo oneshot.
- Controle de LED por PWM com mapeamento de 0 a 100%.
- Leitura do acelerômetro MPU6050 via I2C.
- Botão com interrupção e debounce para alternar entre os estados `LIVE` e `HOLD`.
- Saída textual no console com status do sistema e valores dos sensores.

## Arquitetura

O projeto foi dividido em três níveis:

- **BSP**: acesso aos periféricos físicos da placa.
- **HAL**: camada intermediária que padroniza leitura, escrita e inicialização.
- **RTOS**: tarefas FreeRTOS responsáveis por cada função do sistema.

Essa separação reduz o acoplamento entre a aplicação e o hardware.

## Mapeamento de Hardware

- LED: GPIO 15
- Botão: GPIO 4
- Potenciômetro: ADC1 Channel 6, correspondente ao GPIO 34
- I2C SDA: GPIO 21
- I2C SCL: GPIO 22
- IMU: MPU6050 no endereço 0x68

## Requisitos

- ESP-IDF instalado e configurado
- Placa compatível com os periféricos usados no projeto
- Conexão serial para monitoramento da saída

## Como Compilar

Na raiz do projeto:

```bash
idf.py build
```

## Como Gravar e Monitorar

Substitua a porta serial conforme o seu ambiente:

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

Se necessário, ajuste o target antes de compilar:

```bash
idf.py set-target esp32
```

## Documentação

A documentação completa do projeto está disponível aqui:

- [Abrir documentação gerada](docs/index.html)

## Estrutura do Projeto

```text
.
├── CMakeLists.txt
├── Doxyfile
├── docs/
├── main/
│   ├── inc/
│   └── src/
└── sdkconfig
```

## Observações

- O estado `HOLD` congela a atualização do LED, mas mantém a leitura e exibição dos dados do sistema.
- A saída no console é periódica e reflete o estado interno, o potenciômetro e os dados da IMU.
