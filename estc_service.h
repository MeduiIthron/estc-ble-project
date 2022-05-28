#ifndef ESTC_SERVICE_H__
#define ESTC_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

#define ESTC_BASE_UUID { 0x12, 0xDD, 0x29, 0x01, 0x05, 0x05, /* - */ 0x9B, 0xB0, /* - */ 0xAB, 0x4C, /* - */ 0xA2, 0x8D, /* - */ 0x00, 0x00, 0x0C, 0x74 } // UUID: 740c6551-8da2-4cab-b09b-05050129dd12
#define ESTC_SERVICE_UUID 0x1984
#define ESTC_GATT_CHAR_UUID 0x1399

typedef struct
{
    uint16_t service_handle;
    uint16_t connection_handle;
    ble_gatts_char_handles_t characteristic_handle;
} ble_estc_service_t;

ret_code_t estc_ble_service_init(ble_estc_service_t *service);

void estc_ble_service_on_ble_event(const ble_evt_t *ble_evt, void *ctx);

void estc_update_characteristic_value(ble_estc_service_t *service, int32_t *value);

#endif /* ESTC_SERVICE_H__ */