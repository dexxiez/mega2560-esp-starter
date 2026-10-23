#include "heartbeat.h"

#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "heartbeat";

static void heartbeat_task(void *arg)
{
    const TickType_t period = pdMS_TO_TICKS((uint32_t)arg);
    uint32_t beats = 0;

    for (;;) {
        ESP_LOGI(TAG, "beat %u, free heap %u", beats++, esp_get_free_heap_size());
        vTaskDelay(period);
    }
}

void heartbeat_start(uint32_t period_ms)
{
    xTaskCreate(heartbeat_task, "heartbeat", 2048, (void *)period_ms, 5, NULL);
}
