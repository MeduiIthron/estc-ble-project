#ifndef ESTC_SERVICE_H__
#define ESTC_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

#define ESTC_BASE_UUID { 0x12, 0xDD, 0x29, 0x01, 0x05, 0x05, /* - */ 0x9B, 0xB0, /* - */ 0xAB, 0x4C, /* - */ 0xA2, 0x8D, /* - */ 0x00, 0x00, 0x0C, 0x74 } // UUID: 740c6551-8da2-4cab-b09b-05050129dd12
#define ESTC_SERVICE_UUID 0x1984

typedef struct
{
    uint16_t service_handle;
} ble_estc_service_t;

ret_code_t estc_ble_service_init(ble_estc_service_t *service);

#endif /* ESTC_SERVICE_H__ */