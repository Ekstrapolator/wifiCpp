#include "ww_wifi.hpp"

#define FIVE_MINUTES 300000 / portTICK_PERIOD_MS
#define THREE_MINUTES 180000 / portTICK_PERIOD_MS
#define THIRTY_SECONDS 30000 / portTICK_PERIOD_MS
#define TEN_SECONDS 10000 / portTICK_PERIOD_MS
#define FIVE_SECONDS 5000 / portTICK_PERIOD_MS
#define THREE_SECOND 3000 / portTICK_PERIOD_MS

static const char *TAG = "WIFI: ";

wifi::WifiState wifiState = {
    0, 0, 0, 0, 0
};

SemaphoreHandle_t blockUntilGotIp;

void wifi::block_until_wifi_ready()
{
    if (xSemaphoreTake(blockUntilGotIp, THIRTY_SECONDS))
    {
        ESP_LOGI(TAG, "WIFI READY GOT IP \n");
    }
    else
    {
        ESP_LOGI(TAG, "RECIVED TIMEOUT WHEN ACQUISITION OF IP\n");
        esp_restart();
    }
}

//static function should not be included in heder files
static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id) // przerobic na if  ?
    {
    case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(TAG, "connecting...\n");
        break;

    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "connected\n");
        break;

    case IP_EVENT_STA_GOT_IP:
        xSemaphoreGive(blockUntilGotIp);
        break;

    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG, "DISCONNECTED EVENT\n");
        wifi::re_connect();
        break;

    default:
        ESP_LOGI("UNSPECYFIED EVENT", "id: %d base: %s", event_id, event_base);
        break;
    }
}

void wifi::init_sta(wifi_mode_t wifi_mode, const std::string& ssid, const std::string& password)
{
    blockUntilGotIp = xSemaphoreCreateBinary();

    esp_err_t initError;
    initError = esp_netif_init();
    initError = esp_wifi_set_storage(WIFI_STORAGE_RAM);

    //create event loope before registering
    initError = esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    initError = esp_wifi_init(&init_cfg);

    //esp register event handler register wifi related events
    initError = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL, NULL);
    //register TCP/IP related events
    initError = esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL, NULL);
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
    ESP_LOGI(TAG, "STOP");
    stopError = esp_wifi_stop();
    ESP_ERROR_CHECK(stopError);
    wifiState.m_donotreconnectOnStop = true;

}

//robimy na wypadek braku mozliwosci polaczenia sie
void wifi::re_connect()
{
    ESP_LOGI(TAG, "RECONNECT ATTEMPT NUMBER: %d\n", wifiState.m_reConnectAttempts);
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
        ESP_LOGI(TAG, "STOP ISSUED DO NOT RECONNECT\n");
    }
}