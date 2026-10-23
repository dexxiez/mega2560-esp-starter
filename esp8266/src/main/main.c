#include "esp_log.h"
#include "esp_system.h" // IWYU pragma: export
#include "heartbeat.h"

static const char *TAG = "main";

void app_main(void) {
    ESP_LOGI(TAG, "esp8266 up. sdk %s", esp_get_idf_version());
    heartbeat_start(1000);
}
