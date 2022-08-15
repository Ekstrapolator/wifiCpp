#include "ww_ble.hpp"

static const char *TAG = "BLE";

std::vector<ble::Sensor> sensor_vec = {
    {"lazienka", {0xA4, 0xC1, 0x38, 0x07, 0x43, 0x7B}, 0, 0, 0, 0},
    {"sypialnia", {0xA4, 0xC1, 0x38, 0x71, 0x86, 0xE4}, 0, 0, 0, 0},
    {"balkon", {0xA4, 0xC1, 0x38, 0xA4, 0xFA, 0x78}, 0, 0, 0, 0},
    {"kuchnia", {0xA4, 0xC1, 0x38, 0x56, 0x3E, 0xD4}, 0, 0, 0, 0}

};

static void check_if_sensor_alredy_found(esp_ble_gap_cb_param_t *param, std::vector<ble::Sensor> &sensor_vec);
static bool check_if_all_sensor_ready(std::vector<ble::Sensor> &sensor_vec);

static void ble_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    uint32_t duration = 0;

    switch (event)
    {
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
        // the unit of the duration is second, 0 means scan permanently have to manual stop "esp_ble_gap_stop_scanning"
        if (param->scan_param_cmpl.status == ESP_BT_STATUS_SUCCESS)
            esp_ble_gap_start_scanning(duration);
        else
            ESP_LOGI(TAG, "ADAPTER INITIALIZATION ERROR");
        break;
    
    case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
        if (param->scan_start_cmpl.status == ESP_BT_STATUS_SUCCESS)
            ESP_LOGI(TAG, "SCAN STARTED");
        else
            ESP_LOGI(TAG, "SCAN FAILED");
        break;

    case ESP_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT:
        if (param->update_whitelist_cmpl.wl_operation == ESP_BLE_WHITELIST_ADD)
            ESP_LOGI(TAG, "WHITE LIST UPDATED SUCESS");
        else
            ESP_LOGI(TAG, "FAILED TO UPDATE");
        break;

    case ESP_GAP_BLE_SCAN_RESULT_EVT:
        
        check_if_sensor_alredy_found(param, sensor_vec);
        check_if_all_sensor_ready(sensor_vec);

        break;

    default:
        ESP_LOGI(TAG, "EVENT %d UNHANDLED\n", event);
        break;
    }
}

void check_if_sensor_alredy_found(esp_ble_gap_cb_param_t *param, std::vector<ble::Sensor> &sensor_vec)
{
    for (auto it = sensor_vec.begin(); it != sensor_vec.end(); it++)
    {
        if (param->scan_rst.bda[3] == it->address[3] && it->alredyFound == false)
        {
            int temp{0};
            float float_temp{0};
            temp = param->scan_rst.ble_adv[10] << 8;
            temp = temp | param->scan_rst.ble_adv[11];
            float_temp = (float)temp / 10;
            if (param->scan_rst.ble_adv[13] < 100 && param->scan_rst.ble_adv[13] > 0 && 
                param->scan_rst.ble_adv[12] < 100 && param->scan_rst.ble_adv[12] > 0 &&
                float_temp > -20 && float_temp < 100)
                {
                    it->battery = param->scan_rst.ble_adv[13];
                    it->humidity = param->scan_rst.ble_adv[12];
                    it->temperature = float_temp;
                    it->alredyFound = true;
                    ESP_LOGW(TAG, "%s", (it->name).c_str());
                }
            break;
        }
    }
}
bool check_if_all_sensor_ready(std::vector<ble::Sensor> &sensor_vec)
{
    int sum{0};
    for (auto it = sensor_vec.begin(); it != sensor_vec.end(); it++)
    {
        sum += it->alredyFound;
    }
    if (sum == sensor_vec.size())
    {
        esp_ble_gap_stop_scanning();
        for (auto it = sensor_vec.begin(); it != sensor_vec.end(); it++)
        {
            it->alredyFound = false;
        }
        ESP_LOGI(TAG, "FOUND ALL SENSOR STOP SCANNING");
        return true;
    }
      
    return false;
}
esp_err_t ble::init()
{   
    esp_err_t initError;
    initError = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (initError)
        return initError;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    initError = esp_bt_controller_init(&bt_cfg);
    if (initError != ESP_OK)
        return initError;
    initError = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (initError != ESP_OK)
        return initError;
    initError = esp_bluedroid_init();
    if (initError != ESP_OK)
        return initError;
    initError = esp_bluedroid_enable();
    if (initError != ESP_OK)
        return initError;
    initError = esp_ble_gap_register_callback(ble_event_handler); // callbac function registration
    if (initError != ESP_OK)
        return initError;
    initError = esp_ble_gap_clear_whitelist();
    if (initError != ESP_OK)
        return initError;

    for (auto it = sensor_vec.begin(); it != sensor_vec.end(); it++)
    {
        initError = esp_ble_gap_update_whitelist(ESP_BLE_WHITELIST_ADD, it->address, BLE_WL_ADDR_TYPE_PUBLIC);
        if (initError != ESP_OK)
            return initError;
    }

    return ESP_OK;

}

esp_err_t ble::scan()
{
    esp_ble_scan_params_t ble_scan_params = {
    .scan_type = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ONLY_WLST,
    .scan_interval = 0x640,
    .scan_window = 0x640,
    .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE
    };

    esp_err_t scanError = esp_ble_gap_set_scan_params(&ble_scan_params);
    if(scanError != ESP_OK)
        return scanError;

    return ESP_OK;

}
