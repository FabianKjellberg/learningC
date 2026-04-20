#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "esp_err.h"

esp_err_t light_sensor_init(void);
esp_err_t light_sensor_read_lux(float *lux);

#endif