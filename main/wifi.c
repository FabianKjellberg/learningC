#include <stdbool.h>
#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

static bool has_ip = false;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
  printf("this got called\n");
  printf("event_base: %s, event_id: %ld\n", event_base, event_id);
  
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    wifi_event_sta_disconnected_t *event = (wifi_event_sta_disconnected_t *) event_data;
    printf("Disconnected, reason=%d\n", event->reason);

    printf("trying to reconnect\n");
    has_ip = false;
    esp_wifi_connect(); 
  }

  if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    printf("sta got ip\n");
    has_ip = true;
  }
}

esp_err_t wifi_init(void) {
  esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
  ESP_ERROR_CHECK(ret);
  
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  esp_netif_t *netif = esp_netif_create_default_wifi_sta();
  esp_netif_set_hostname(netif, "Plant Monitor");

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_err_t ret2 = esp_wifi_init(&cfg);
  if (ret2 != ESP_OK) {
    return ret2;
  }

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

  printf("Wi-Fi driver started successfully!\n");
  return ESP_OK;
}

esp_err_t wifi_connect() {
  wifi_config_t wifi_cfg = {
    .sta = {
      .ssid = "Citron",
      .password = "Solros!1966",
    },
  };

  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));
  ESP_ERROR_CHECK(esp_wifi_connect());

  return ESP_OK;
}

void test_connection() {
    if(!has_ip) return;
    
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr("142.250.74.14"); //google.com IP
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(80);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Socket creation failed\n");
        return;
    }

    int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        printf("Connection failed\n");
    } else {
        printf("Connected to Google!\n");
    }

    close(sock);
}


