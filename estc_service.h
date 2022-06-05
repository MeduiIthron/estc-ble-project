#ifndef ESTC_SERVICE_H__
#define ESTC_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

#define ESTC_BASE_UUID { 0x12, 0xDD, 0x29, 0x01, 0x05, 0x05, /* - */ 0x9B, 0xB0, /* - */ 0xAB, 0x4C, /* - */ 0xA2, 0x8D, /* - */ 0x00, 0x00, 0x0C, 0x74 } // UUID: 740c6551-8da2-4cab-b09b-05050129dd12
#define ESTC_SERVICE_UUID 0x1984

#define ESTC_GATT_CHAR_UUID 0x1399
#define ESTC_GATT_CHAR_INDICATE_UUID 0x1380
#define ESTC_GATT_CHAR_NOTIFICATE_UUID 0x1381

#define CHAR_NOTIFICATION_UPDATE_TIME 10000
#define CHAR_INDICATION_UPDATE_TIME 20000

typedef struct
{
    uint16_t service_handle;
    uint16_t connection_handle;
    ble_gatts_char_handles_t char_handle;
    ble_gatts_char_handles_t char_indicate_handle;
    ble_gatts_char_handles_t char_notificate_handle;
} ble_estc_service_t;

typedef enum
{
    READ = 1,
    WRITE = 2,
    NOTIFY = 4,
    INDICATE = 8
} ble_estc_char_modifier_t;

/**
 * @brief Add Service to BLE
 * 
 * @param service 
 * @return ret_code_t 
 */
ret_code_t estc_ble_service_init(ble_estc_service_t *service);

/**
 * @brief Add Characteristic to Service
 * 
 * @param service BLE Service
 * @param uuid Characteristic Identifier
 * @param description Characteristic Description
 * @param data Characteristic Value
 * @param data_size Characteristic Value Size
 * @param modifiers Characteristic Modifiers
 * @param handler Characteristic Handler
 * @return ret_code_t 
 */
ret_code_t estc_ble_add_characteristic(
                                        ble_estc_service_t *service,
                                        uint16_t uuid,
                                        const char* description,
                                        uint8_t* data,
                                        uint16_t data_size,
                                        ble_estc_char_modifier_t modifiers,
                                        ble_gatts_char_handles_t* handler
                                    );

#endif /* ESTC_SERVICE_H__ */