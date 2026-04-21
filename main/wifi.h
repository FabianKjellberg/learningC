#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"

esp_err_t wifi_init(void); 
esp_err_t wifi_connect(void);
esp_err_t test_connection(void);

#endif