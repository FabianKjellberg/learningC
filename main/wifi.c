#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_netif.h"
#include "nvs_flash.h"

esp_err_t wifi_init(void) {
  esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
  ESP_ERROR_CHECK(ret);
  
  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());

  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_err_t ret2 = esp_wifi_init(&cfg);
  if (ret2 != ESP_OK) {
      return ret;
  }

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  
  ESP_ERROR_CHECK(esp_wifi_start());

  printf("Wi-Fi driver started successfully!\n");
  return ESP_OK;
}
