#include <stdio.h>
#include "countdown.h"
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
  printf("Hello world\n");
  
  xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);

  while(1) {
    printf("changing state to blue\n");
    led_set_state(LED_STATE_BLINKING_BLUE);
    vTaskDelay(pdMS_TO_TICKS(10000));

    printf("changing state to red\n");
    led_set_state(LED_STATE_BLINKING_RED);
    vTaskDelay(pdMS_TO_TICKS(10000));

    printf("changing state to green\n");
    led_set_state(LED_STATE_BLINKING_GREEN);
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

