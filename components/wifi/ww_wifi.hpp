#pragma once


#include <esp_log.h>
#include <nvs_flash.h>
#include <cstring>
#include <string>
#include <esp_wifi.h>
#include <esp_system.h>
#include "../../main/event.hpp"

class Wifi
{
private:
    bool m_initCalled;
public:

    Wifi();
    ~Wifi();
    void init_sta(wifi_mode_t wifi_mode, const std::string& sssid, const std::string& password);
    void set_event_handler(Event* event);
    
};


