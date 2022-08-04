#pragma once
#include <esp_log.h>
#include <esp_err.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_gap_ble_api.h>

namespace ble
{
    esp_err_t init();
    esp_err_t scan();
}