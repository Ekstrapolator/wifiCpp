#include "ww_wifi.hpp"

Wifi::Wifi(/* args */)
{
    m_initCalled = false;

}

Wifi::~Wifi()
{
}

void Wifi::set_event_handler(Event* event)
{
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, event->wifi_event_handler, NULL);
}

void Wifi::init_sta(wifi_mode_t wifi_mode, const std::string& ssid, const std::string& password)
{
    esp_err_t initError;
    initError = nvs_flash_init();
    initError = esp_wifi_set_storage(WIFI_STORAGE_RAM);

    esp_event_loop_create_default();

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    initError = esp_wifi_init(&init_cfg);

    //esp register event handler

    initError = esp_wifi_set_mode(wifi_mode);

    

    wifi_config_t sta_config;
    memset(&sta_config, 0, sizeof(sta_config));
    memcpy(sta_config.sta.ssid, ssid.data(), ssid.size());
    memcpy(sta_config.sta.password, password.data(), password.size());

    initError = esp_wifi_set_config(WIFI_IF_STA, &sta_config);

    ESP_ERROR_CHECK(initError);
    m_initCalled = true;
    ESP_LOGI("WIFI", "TEST");
}