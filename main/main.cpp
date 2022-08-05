#include <stdio.h>
#include <iostream>
#include <sdkconfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

#include "../components/wifi/ww_wifi.hpp"
#include "../components/wifi/ww_wifi_creds.hpp"
#include "../components/sntp/ww_sntp.hpp"
#include "../components/ble/ww_ble.hpp"
#include "event.hpp"



extern "C"{
    void app_main(void);
}


void task(void* arg)
{
    for(;;)
    {
        vTaskDelay(400);
    }
}


void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi::init_sta(WIFI_MODE_STA, SSID, PASSWORD);
    wifi::block_until_wifi_ready();

    sntp::get_time_from_sntp();
    sntp::block_until_time_ready();

    ret = ble::init();
    if (ret == ESP_OK)
        ble::scan();

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
    

    for(;;)
    {
        printf("im not dead \n");
        vTaskDelay(pdMS_TO_TICKS(30000));
    }

} //Main task that simply calls app_main. This task will self delete when app_main returns
