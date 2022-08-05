#pragma once
#include <esp_log.h>
#include <esp_err.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_gap_ble_api.h>
#include <string>
#include <vector>

namespace ble
{
    struct Sensor
    {
        std::string name;
        esp_bd_addr_t address;
        float temperature;
        int humidity;
        int battery;
        bool alredyFound;
    };
    esp_err_t init();
    esp_err_t scan();
}