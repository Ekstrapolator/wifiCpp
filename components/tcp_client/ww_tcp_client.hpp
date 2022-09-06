#pragma once
#include <esp_log.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <string>
#include <lwip/err.h>
#include <lwip/sockets.h>
#include <cstring>

#include "../ble/ww_ble.hpp"



namespace tcp
{
    void client_task_main_function(void *_tpcClientQueue);
}