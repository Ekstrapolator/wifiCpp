#pragma once


#include <esp_log.h>
#include <nvs_flash.h>
#include <cstring>
#include <string>
#include <esp_wifi.h>
#include <esp_system.h>
//#include "../../main/event.hpp"

namespace wifi
{
    struct WifiState
    {
        bool m_initCalled;
        bool m_donotreconnectOnStop;
        uint8_t m_reConnectAttempts;
    };

    void init_sta(wifi_mode_t wifi_mode, const std::string &sssid, const std::string &password);
    void stop();
    void re_connect();

};
