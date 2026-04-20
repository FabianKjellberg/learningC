#include <stdio.h>
#include "countdown.h"
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "light_sensor.h"
#include "i2c.h"
#include "bme280.h"
#include "wifi.h"
#include "battery.h"

void app_main(void)
{
  printf("Hello world\n");
  
  //init GPIO handles
  i2c_init();
  i2c_print_devices();  

  //start blinking task (debug)
  light_sensor_init();
  xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);
  led_set_state(LED_STATE_BLINKING_PINK);
  
  //init bme and create data structures to mutate
  bme280_init();
  bme280_data_t data;
  float lux;

  //init wifi hardware
  wifi_init();

  //init battery GPI0 ADC
  battery_init();
  int v;

  while(1) {
    //read sensor data
    light_sensor_read_lux(&lux);
    bme280_read_values(&data);

    //read battery v
    battery_read(&v);

    //print values
    printf("battery pin v: %d mV (%.2f V)\n", v, (float)v / 1000.0);
    printf("lux: %f\n", lux);
    printf("Pressure: %f\n", data.pressure);
    printf("Temperature: %f\n", data.temperature);
    printf("Humidity: %f\n", data.humidity);


    //delay
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}