#include "ww_tcp_client.hpp"

static const char *TAG = "TCP";
constexpr int PORT = 1200;
std::string TCP_REMOTE_ADDR = "192.168.2.6"; // check for real one adress
static int tcp_client_sock;

void tcp::client_task_main_function(void *_tpcClientQueue)
{
   QueueHandle_t tpcClientQueue = static_cast<QueueHandle_t>(_tpcClientQueue);
   printf("Queue head adress %X \n", tpcClientQueue);
   BaseType_t xStatus;
   for (;;)
   {
      struct sockaddr_in destAddr;
      destAddr.sin_addr.s_addr = inet_addr(TCP_REMOTE_ADDR.data());
      destAddr.sin_family = AF_INET;
      destAddr.sin_port = htons(PORT);
      tcp_client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

      if (tcp_client_sock < 0)
      {
         ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
         break;
      }
      ESP_LOGI(TAG, "TCP CLIENT Socket created: %d", tcp_client_sock);

      int err = connect(tcp_client_sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
      if (err != 0)
      {
         ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
         close(tcp_client_sock);
         ESP_LOGW(TAG, "Close client socket");
         continue;
      }
      for (;;)
      {
         ble::Sensor recivedFromQueue = {};
         xStatus = xQueueReceive( tpcClientQueue, &recivedFromQueue, portMAX_DELAY );
         if(xStatus == pdPASS)
         {
            printf("Recived data from queue\n");
         }
      }
      vTaskDelay(pdMS_TO_TICKS(1000));
   }
   vTaskDelete(NULL);
}