#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"

QueueHandle_t testy_queue;

char *TAG = "Queue-Test";

void writing_task(void* pvParams) {
    ESP_LOGI(TAG, "Starting the writing task...");

    int16_t testy_value[4] = {0, 0, 0, 0};
    testy_queue = xQueueCreate(1, sizeof(testy_value));

    while (1) {
        testy_value[0] += 1;
//         xQueueSendToBack(testy_queue, (void *) testy_value, 0);
        xQueueOverwrite(testy_queue, (void *) testy_value);
        ESP_LOGI(TAG, "Sent %d", testy_value[0]);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void reading_task(void* pvParams) {
    ESP_LOGI(TAG, "Starting the reading task...");

    int16_t read_value[4];

    while (1) {
        if (xQueueReceive(testy_queue, (void *) read_value, 0) == pdTRUE) {
            ESP_LOGI(TAG, "Received %d", read_value[0]);
        } else {
            ESP_LOGI(TAG, "No data received...");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    xTaskCreate( writing_task, "Writing Task", 4096, NULL, 1, NULL);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    xTaskCreate( reading_task, "Reading Task", 4096, NULL, 1, NULL);
}
