# Define os arquivos de código-fonte apontando para a pasta src
set(component_srcs
    "src/main.c"
    "src/system_tasks.c"
    "src/hal_system.c"
    "src/bsp_led.c"
    "src/bsp_button.c"
    "src/bsp_potentiometer.c"
    "src/bsp_imu.c"
)

# Define a pasta onde o compilador buscará os arquivos de cabeçalho (.h)
set(component_includes
    "inc"
)

# Registra o componente no sistema de build do ESP-IDF
idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "${component_includes}")