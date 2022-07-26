#include "event.hpp"

static const char *WIFI = "WIFI: ";

Event::Event(/* args */)
{
}

Event::~Event()
{
}

void Event::wifi_event_handler(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
  switch (event_id)
  {
  case WIFI_EVENT_STA_START:
    //esp_wifi_connect(); 
    ESP_LOGI(WIFI,"connecting...\n");
    break; 

  case WIFI_EVENT_STA_CONNECTED:
    ESP_LOGI(WIFI,"connected\n");
    break;

  case IP_EVENT_STA_GOT_IP:
    ESP_LOGI(WIFI,"got ip\n");
    break;

  case WIFI_EVENT_STA_DISCONNECTED:
    ESP_LOGI(WIFI,"disconnected\n");
    break;

  default:
    break;
  }
}