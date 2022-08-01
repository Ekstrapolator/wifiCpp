#include "ww_sntp.hpp"
#define NTP_TIMEOUT 5

static const char *TAG = "SNTP: ";

void start_ntp_service()
{
  ESP_LOGI(TAG, "Acquire time using NTP");
  sntp_setoperatingmode(SNTP_SYNC_MODE_IMMED); //SNTP_SYNC_MODE_IMMED (default): Updates system time immediately upon receiving a response
  sntp_setservername(0, SNTP_SERVER);
  sntp_init();
  for(int i=0; i < NTP_TIMEOUT; ++i)
  {
    if(sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED)
    {
      return;
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  ESP_LOGE(TAG, "Couldn't obtain NTP date!");
}
