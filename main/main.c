#include <stdio.h>
#include <stdbool.h>
#include "countdown.h"
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "light_sensor.h"
#include "i2c.h"
#include "bme280.h"
#include "wifi.h"
#include "battery.h"
#include "api.h"
#include "time_helper.h"

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
  bme280_data_t bme280_data;

  //init wifi hardware
  wifi_init();
  wifi_connect();

  //init battery GPI0 ADC
  battery_init();

  //init obj
  sensor_reading_body api_body;

  //set mac adress
  get_mac_addr(api_body.mac_addr, MAC_ADDR_STR_LEN);

  bool connected;

  while(1) {
    //read sensor data
    light_sensor_read_lux(&api_body.lux);
    bme280_read_values(&bme280_data);

    //read battery v
    battery_read(&api_body.battery_mv);

    //upload values
    connected = test_connection();
    if(connected) {
      api_body.humidity = bme280_data.humidity;
      api_body.pressure = bme280_data.pressure;
      api_body.temperature = bme280_data.temperature;
      time_get_iso8601(api_body.read_at, TIMESTAMP_STR_LEN);


      //print values
      printf("battery pin v: %d mV (%.2f V)\n", api_body.battery_mv, (float)api_body.battery_mv / 1000.0);
      printf("lux: %f\n", api_body.lux);
      printf("Pressure: %f\n", bme280_data.pressure);
      printf("Temperature: %f\n", bme280_data.temperature);
      printf("Humidity: %f\n", bme280_data.humidity);
      printf("TimeStamp: %s\n", api_body.read_at);
      printf("Mac Adress: %s\n", api_body.mac_addr);

      post_sensor_reading(&api_body);
    }

    //delay
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}