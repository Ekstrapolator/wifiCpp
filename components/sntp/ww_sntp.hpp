#pragma once
#include <esp_log.h>
#include <esp_sntp.h>

#define SNTP_SERVER_ADDRESS "tempus1.gum.gov.pl"

namespace sntp
{
    void block_until_time_ready();
    void get_time_from_sntp();
}