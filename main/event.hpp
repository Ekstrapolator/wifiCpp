#pragma once

#include <esp_wifi.h>
#include <esp_log.h>

class Event
{
private:
    /* data */
public:
    Event(/* args */);
    ~Event();
    static void wifi_event_handler(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

};


