#ifndef ESTC_PROJECT_SERVICES_BLE_SERVICE_H
#define ESTC_PROJECT_SERVICES_BLE_SERVICE_H

#include "ble.h"
#include "app_error.h"

#define BLE_BASE_UUID { 0x12, 0xDD, 0x29, 0x01, 0x05, 0x05, /* - */ 0x9B, 0xB0, /* - */ 0xAB, 0x4C, /* - */ 0xA2, 0x8D, /* - */ 0x00, 0x00, 0x0C, 0x74 } // UUID: 740c6551-8da2-4cab-b09b-05050129dd12
#define BLE_SERVICE_UUID 0x1984

ret_code_t ble_service_init(uint16_t* conn_handle);
ret_code_t ble_service_write_handler(ble_evt_t const * p_ble_evt);

#endif // ESTC_PROJECT_SERVICES_BLE_SERVICE_H