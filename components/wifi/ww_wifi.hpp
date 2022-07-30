#pragma once


#include <esp_log.h>
#include <nvs_flash.h>
#include <cstring>
#include <string>
#include <esp_wifi.h>
#include <esp_system.h>
//#include "../../main/event.hpp"



class Wifi
{
private:
    bool m_initCalled;
    bool m_stopComInit;
    uint8_t m_recconectAttempts;
public:

    Wifi();
    ~Wifi();
    void init_sta(wifi_mode_t wifi_mode, const std::string& sssid, const std::string& password);
    void stop();
    
private:

    static void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    void reConnect();

};


