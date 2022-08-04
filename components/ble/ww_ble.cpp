#include "ww_ble.hpp"

static const char *TAG = "BLE: ";

static void ble_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    uint32_t duration = 2;

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


        break;

    default:
        ESP_LOGI(TAG, "EVENT %d UNHANDLED\n", event);
        break;
    }
}

esp_err_t ble::init()
{
    esp_bd_addr_t sypialnia = {0xA4, 0xC1, 0x38, 0x71, 0x86, 0xE4};
    esp_bd_addr_t balkon = {0xA4, 0xC1, 0x38, 0xA4, 0xFA, 0x78};
    esp_bd_addr_t lazienka = {0xA4, 0xC1, 0x38, 0x07, 0x43, 0x7B};
    esp_bd_addr_t kuchnia = {0xA4, 0xC1, 0x38, 0x56, 0x3E, 0xD4};

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
    initError = esp_ble_gap_update_whitelist(ESP_BLE_WHITELIST_ADD, sypialnia, BLE_WL_ADDR_TYPE_PUBLIC);
    if (initError != ESP_OK)
        return initError;
    initError = esp_ble_gap_update_whitelist(ESP_BLE_WHITELIST_ADD, balkon, BLE_WL_ADDR_TYPE_PUBLIC);
    if (initError != ESP_OK)
        return initError;
    initError = esp_ble_gap_update_whitelist(ESP_BLE_WHITELIST_ADD, lazienka, BLE_WL_ADDR_TYPE_PUBLIC);
    if (initError != ESP_OK)
        return initError;
    initError = esp_ble_gap_update_whitelist(ESP_BLE_WHITELIST_ADD, kuchnia, BLE_WL_ADDR_TYPE_PUBLIC);
    if (initError != ESP_OK)
        return initError;

    
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
