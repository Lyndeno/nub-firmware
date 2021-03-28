/*
   NUB WiFi Main File
*/
#include <string.h>
//#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "rom/ets_sys.h"
#include "esp_wifi.h"
//#include "esp_system.h"
//#include "esp_spi_flash.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "network.h"
#include "comms.h"

/*
static void uartEventTask(void *pvParameters) {
    uart_event_t event;
    uint8_t *tempBuffer = (uint8_t *)malloc(UART_MAX_NUM_RX_BYTES);
}*/

void app_main() {
    //UART_init();
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();

    xTaskCreate(udp_server_task, "udp_server", 4096, NULL, 5, NULL);

    while (1)
    {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        //uart_write_bytes(UART_NUM_0, "Hello there\r\n", 13);
    }
    
}
