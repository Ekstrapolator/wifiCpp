#include <stdio.h>
#include <iostream>
#include <sdkconfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <esp_wifi.h>

#include "../components/wifi/ww_wifi.hpp"
#include "../components/wifi/ww_wifi_creds.hpp"
#include "event.hpp"



extern "C"{
    void app_main(void);
}





void app_main(void)
{
    Event* event = new Event;
    Wifi wifi;
    //wifi.set_event_handler();
    wifi.set_event_handler(event);
    wifi.init_sta(WIFI_MODE_STA, SSID, PASSWORD);
    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());


}
