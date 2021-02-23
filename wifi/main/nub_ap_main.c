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

#define BUF_SIZE 1024 // change this as appropriate

#define ESP_WIFI_SSID "NUB"
#define ESP_WIFI_PASS "capstone1234"
#define ESP_WIFI_MAX_CONN 4

static const char *TAG = "NUB WiFi";

const int WIFI_CONNECTED_BIT = BIT0;

static EventGroupHandle_t wifi_event_group;

static QueueHandle_t s_uart0Queue;

static esp_err_t event_handler(void *ctx, system_event_t *event) {
    /* For accessing reason codes in case of disconnection */
    system_event_info_t *info = &event->event_info;

    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP: // TODO: Probably not needed, only using AP mode
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
                 MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED: // TODO: Probably not needed, only using AP mode
        ESP_LOGE(TAG, "Disconnect reason : %d", info->disconnected.reason);
        if (info->disconnected.reason == WIFI_REASON_BASIC_RATE_NOT_SUPPORT) {
            /*Switch to 802.11 bgn mode */
            esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N);
        }
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        ESP_LOGI(TAG, "assigned ip:%s",
                 ip4addr_ntoa(&event->event_info.ap_staipassigned.ip));
        break;
    default:
        break;
    }
    return ESP_OK;
}

void UART_init (void) {
    uart_config_t uart_config = {
        .baud_rate = 115200, // match this with MCU baud
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_NUM_0, &uart_config);

    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, BUF_SIZE * 2, 100, &s_uart0Queue, 0);

    //xTaskCreate(uartEventTask, "uartEventTask", 2048, NULL, 12, NULL); //change as needed
}

/*
static void uartEventTask(void *pvParameters) {
    uart_event_t event;
    uint8_t *tempBuffer = (uint8_t *)malloc(UART_MAX_NUM_RX_BYTES);
}*/

void wifi_init_softap() {
    wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = ESP_WIFI_SSID,
            .ssid_len = strlen(ESP_WIFI_SSID),
            .password = ESP_WIFI_PASS,
            .max_connection = ESP_WIFI_MAX_CONN, // documentation says max is 4
            .authmode = WIFI_AUTH_WPA2_PSK
        }
    };

    if (strlen(ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI( TAG, "wifi_init_softap finished. SSID: %s password: %s", ESP_WIFI_SSID, ESP_WIFI_PASS);
}

void app_main() {
    UART_init();
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();

    while (1)
    {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        uart_write_bytes(UART_NUM_0, "Hello there\r\n", 12);
    }
    
}
