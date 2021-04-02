#include "network.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include <string.h>
#include "routing.h"

#include "lwip/sockets.h"

const char *TAG = "NUB WiFi";
int sock;

#define ESP_WIFI_SSID "NUB"
#define ESP_WIFI_PASS "capstone1234"


#define UDP_PORT 3333

static esp_err_t event_handler(void *ctx, system_event_t *event) {

    switch(event->event_id) {
    case SYSTEM_EVENT_AP_STACONNECTED: // Client connects
        ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED: // Client disconnects
        ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
                 MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);

        // Remove device from device table
        wifi_device disconn_device;
        disconn_device.mac = pvPortMalloc(MAC_LENGTH * sizeof(uint8_t));
        disconn_device.state = Disconnected;
        copy_MAC(event->event_info.sta_disconnected.mac, disconn_device.mac);
        xQueueSendToBack(q_wifi_state, &disconn_device, portMAX_DELAY);
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED: // Client is assigned an IP address
        ESP_LOGI(TAG, "assigned ip:%s",
                 ip4addr_ntoa(&event->event_info.ap_staipassigned.ip));
        break;
    default:
        break;
    }
    return ESP_OK;
}

void wifi_init_softap(void) {
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

void udp_init(void) {
    q_wifi_rx_frames = xQueueCreate(256, sizeof(message_frame));
    q_wifi_tx_frames = xQueueCreate(256, sizeof(message_frame));

    xTaskCreate(udp_server_task, "udp_server", 4096, NULL, 5, NULL);
}

void udp_recv_task(void *pvParameters) {
    //char rx_buffer[128]; // perhaps use buffer struct from mcu code for this
    char addr_str[128];
    

    while (1) {
        message_frame rx_frame;
        char *rx_buffer;
        rx_buffer = pvPortMalloc(128*sizeof(char));
        ESP_LOGI(TAG, "Waiting for data");
        struct sockaddr_in sourceAddr;

        socklen_t socklen = sizeof(sourceAddr);
        int len = recvfrom(sock, rx_buffer, 128*sizeof(char) - 1, 0, (struct sockaddr *)&sourceAddr, &socklen);

        // Error occured during receiving
        if (len < 0) {
            ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
            //break;
        }
        // Data received
        else {
            // Get the sender's ip address as string
            inet_ntoa_r(((struct sockaddr_in *)&sourceAddr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);

            rx_buffer[len] = 0;
            rx_frame.data = (uint8_t *)rx_buffer;
            rx_frame.len = len;
            rx_frame.devaddr = sourceAddr;
            //rx_frame.ip_addr = sourceAddr.sin_addr.s_addr;
            //rx_frame.port = sourceAddr.sin_port;

            xQueueSendToBack(q_wifi_tx_frames, (void *)&rx_frame, pdMS_TO_TICKS( 1000 ));

            //rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
            ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
            ESP_LOGI(TAG, "%s", rx_buffer);

            /*int err = sendto(sock, rx_buffer, len, 0, (struct sockaddr *)&sourceAddr, sizeof(sourceAddr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
                //break;
            }*/
        }
    }
}

void udp_trans_task(void *pvParameters) {
    message_frame tx_frame;
    struct sockaddr_in destAddr;
    //socklen_t socklen = sizeof(destAddr);
    while (1) {
        if (xQueueReceive(q_wifi_tx_frames, &tx_frame, portMAX_DELAY)) {
            ESP_LOGI(TAG, "Sending String: %s", tx_frame.data);
            //destAddr.sin_addr.s_addr = tx_frame.ip_addr;
            //destAddr.sin_port = tx_frame.port;
            destAddr = tx_frame.devaddr;
            sendto(sock, tx_frame.data, tx_frame.len, 0, (struct sockaddr *)&destAddr, sizeof(destAddr));
            vPortFree(tx_frame.data);
        }
    }
}

// use code from sdk example and adapt
void udp_server_task(void *pvParameters) {
    int addr_family;
    int ip_protocol;

    while (1) {
        struct sockaddr_in destAddr;
        destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(UDP_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP; // IPV4
        //inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

        sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");

        int err = bind(sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
        if (err < 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(TAG, "Socket binded");
        xTaskCreate(udp_recv_task, "udp_receive", 4096, NULL, 5, NULL);
        xTaskCreate(udp_trans_task, "udp_transmit", 4096, NULL, 5, NULL);

        while (1) {
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

