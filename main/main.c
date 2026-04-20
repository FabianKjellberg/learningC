#include <stdio.h>
#include "countdown.h"
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "light_sensor.h"
#include "i2c.h"
#include "bme280.h"

void app_main(void)
{
  printf("Hello world\n");
  
  i2c_init();
  i2c_print_devices();  

  light_sensor_init();
  xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);
  float lux;

  bme280_init();

  bme280_data_t data;
  
  bme280_read_values(&data);

  while(1) {


    light_sensor_read_lux(&lux);
    printf("lux: %f\n", lux);
    printf("changing state to blue\n");
    bme280_read_values(&data);
    printf("Pressure: %f\n", data.pressure);
    printf("Temperature: %f\n", data.temperature);
    printf("Humidity: %f\n", data.humidity);
    led_set_state(LED_STATE_BLINKING_BLUE);
    vTaskDelay(pdMS_TO_TICKS(10000));
    

    light_sensor_read_lux(&lux);
    printf("lux: %f\n", lux);
    printf("changing state to red\n");
    bme280_read_values(&data);
    printf("Pressure: %f\n", data.pressure);
    printf("Temperature: %f\n", data.temperature);
    printf("Humidity: %f\n", data.humidity);
    led_set_state(LED_STATE_BLINKING_RED);
    vTaskDelay(pdMS_TO_TICKS(10000));

    light_sensor_read_lux(&lux);
    printf("lux: %f\n", lux);
    printf("changing state to green\n");
    bme280_read_values(&data);
    printf("Pressure: %f\n", data.pressure);
    printf("Temperature: %f\n", data.temperature);
    printf("Humidity: %f\n", data.humidity);
    led_set_state(LED_STATE_BLINKING_GREEN);
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

