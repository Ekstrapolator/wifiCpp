#include "ww_wifi.hpp"

static const char *WIFI = "WIFI: ";

wifi::WifiState wifiState = {
    0,
    0,
    0
};
//static function should not be included in heder files
static void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id) // przerobic na if  ?
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
        ESP_LOGI(WIFI, "DISCONNECTED EVENT\n");
        wifi::re_connect();
        break;

    default:
        ESP_LOGI("UNSPECYFIED EVENT", "id: %d base: %s", event_id, event_base);
        break;
    }
}

void wifi::init_sta(wifi_mode_t wifi_mode, const std::string& ssid, const std::string& password)
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
    initError = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL, NULL);
    //register TCP/IP related events
    initError = esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL, NULL);
    //IP_EVENT_STA_GOT_IP ESP_EVENT_ANY_ID

    initError = esp_wifi_set_mode(wifi_mode);

    wifi_config_t sta_config;
    memset(&sta_config, 0, sizeof(sta_config));
    memcpy(sta_config.sta.ssid, ssid.data(), ssid.size());
    memcpy(sta_config.sta.password, password.data(), password.size());

    initError = esp_wifi_set_config(WIFI_IF_STA, &sta_config);

    initError = esp_wifi_start();
    ESP_ERROR_CHECK(initError);
    wifiState.m_initCalled = true;
    
}

// dorobic jak bedzie pozniej
void wifi::stop()
{
    esp_err_t stopError;
    ESP_LOGI(WIFI, "STOP");
    stopError = esp_wifi_stop();
    ESP_ERROR_CHECK(stopError);
    wifiState.m_donotreconnectOnStop = true;

}

//robimy na wypadek braku mozliwosci polaczenia sie
void wifi::re_connect()
{
    ESP_LOGI(WIFI, "RECONNECT ATTEMPT NUMBER: %d\n", wifiState.m_reConnectAttempts);
    wifi_mode_t mode;
    if (wifiState.m_donotreconnectOnStop == false)
    {
        if (wifiState.m_reConnectAttempts >= 5)
            esp_restart();
        else if (esp_wifi_get_mode(&mode) == ESP_OK) // if wifi block alredy initiated we can try to connect
            esp_wifi_connect();

        wifiState.m_reConnectAttempts++;
    }
    else
    {
        ESP_LOGI(WIFI, "STOP DO NOT RECONNECT\n");
    }
}