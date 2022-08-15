#pragma once
#include <esp_log.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>




namespace tcp
{
    void client_task_main_function(void *_tpcClientQueue);
}