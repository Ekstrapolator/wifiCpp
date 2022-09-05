#include <string>
#include "ww_tcp_client.hpp"

static const char *TAG = "TCP";
constexpr int PORT = 1200;
std::string TCP_REMOTE_ADDR = "192.168.2.8"; // check for real one adress


void tcp::client_task_main_function(void *_tpcClientQueue)
{
   QueueHandle_t tpcClientQueue; //= std::static_cast<QueueHandle_t>(_tpcClientQueue);

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

      for (;;)
      {
      }
   }
}
  