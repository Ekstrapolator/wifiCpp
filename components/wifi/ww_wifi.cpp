#include "ww_wifi.hpp"

Wifi::Wifi(/* args */)
{
    m_initCalled = false;

}

Wifi::~Wifi()
{
}

void Wifi::event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(WIFI, "connecting...\n");
        break;

    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(WIFI, "connected\n");
        break;

    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(WIFI, "got ip\n");
        break;

    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI(WIFI, "disconnected\n");
        break;

    default:
        ESP_LOGI("UNSPECYFIED EVENT", "id: %d base: %s", event_id, event_base);
        break;
    }
}

void Wifi::init_sta(wifi_mode_t wifi_mode, const std::string& ssid, const std::string& password)
{
    esp_err_t initError;
    initError = nvs_flash_init();
    initError = esp_wifi_set_storage(WIFI_STORAGE_RAM);

    

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    initError = esp_wifi_init(&init_cfg);

    //esp register event handler
    //create event loope before registering
    esp_event_loop_create_default();
    //register wifi related events
    initError = esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, this->event_handler, NULL);
    //register TCP/IP related events
    initError = esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, this->event_handler, NULL);
    //IP_EVENT_STA_GOT_IP ESP_EVENT_ANY_ID

    initError = esp_wifi_set_mode(wifi_mode);

    

    wifi_config_t sta_config;
    memset(&sta_config, 0, sizeof(sta_config));
    memcpy(sta_config.sta.ssid, ssid.data(), ssid.size());
    memcpy(sta_config.sta.password, password.data(), password.size());

    initError = esp_wifi_set_config(WIFI_IF_STA, &sta_config);

    initError = esp_wifi_start();
    ESP_ERROR_CHECK(initError);
    m_initCalled = true;
    ESP_LOGI("WIFI", "TEST");
}