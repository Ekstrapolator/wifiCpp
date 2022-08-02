#include "ww_sntp.hpp"

#define FIVE_MINUTES 300000 / portTICK_PERIOD_MS
#define THREE_MINUTES 180000 / portTICK_PERIOD_MS
#define THIRTY_SECONDS 30000 / portTICK_PERIOD_MS
#define TEN_SECONDS 10000 / portTICK_PERIOD_MS
#define FIVE_SECONDS 5000 / portTICK_PERIOD_MS
#define THREE_SECOND 3000 / portTICK_PERIOD_MS

static const char *TAG = "SNTP: ";

SemaphoreHandle_t blockUntilGotTime;

static void sntp_event_handler(struct timeval *tv)
{
    setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
    tzset();
    struct tm *timeinfo = localtime(&tv->tv_sec);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "%c", timeinfo);
    ESP_LOGI("RTC", "DATE AND TIME UPDATET TO: %s", buffer);
    xSemaphoreGive(blockUntilGotTime);
}

void sntp::get_time_from_sntp()
{
    blockUntilGotTime = xSemaphoreCreateBinary();
    sntp_setoperatingmode(SNTP_SYNC_MODE_IMMED); // SNTP_SYNC_MODE_IMMED (default): Updates system time immediately upon receiving a response
    sntp_setservername(0, SNTP_SERVER_ADDRESS);
    sntp_init();
    sntp_set_time_sync_notification_cb(sntp_event_handler);
}

void sntp::block_until_time_ready()
{
    if (xSemaphoreTake(blockUntilGotTime, THIRTY_SECONDS))
    {
        ESP_LOGI(TAG, "TIME IS SYNCHRONIZED\n");
    } 
    else
    {
        ESP_LOGI(TAG, "RECIVED TIMEOUT WHEN TRYING TO SYNCHRONIZE THE TIME\n");
        esp_restart();
    }
}
