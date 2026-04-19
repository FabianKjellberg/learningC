idf_component_register(
    SRCS "main.c" "fizzbuzzpop.c" "countdown.c" "led.c"
    INCLUDE_DIRS "."
    REQUIRES led_strip    
)