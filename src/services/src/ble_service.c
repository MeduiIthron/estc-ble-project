#include "stdint.h"
#include "string.h"
#include "nrf_sdh_ble.h"

#include "../ble_service.h"
#include "../color_service.h"
#include "../flash_service.h"

#define BLE_BLINKY_HSB_CHAR_UUID 0x1300
#define BLE_BLINKY_HSB_CHAR_SIZE sizeof(uint16_t) * 3

#define BLE_BLINKY_HSB_NOTIFY_CHAR_UUID 0x1301
#define BLE_BLINKY_HSB_NOTIFY_CHAR_SIZE sizeof(uint16_t) * 3

typedef enum
{
    READ = 1,
    WRITE = 2,
    NOTIFY = 4,
    INDICATE = 8
} ble_estc_char_modifier_t;

typedef struct
{
    uint16_t service_handle;
    uint16_t connection_handle;
    ble_gatts_char_handles_t blinky_hsb_char_handle;
    ble_gatts_char_handles_t blinky_hsb_notify_char_handle;
} ble_estc_service_t;

static ble_estc_service_t m_estc_service;
static uint16_t* m_conn_handle;
static ret_code_t ble_service_add_characteristic(ble_estc_service_t *service, uint16_t uuid, const char* description, uint8_t* data, uint16_t data_size,
                                          ble_estc_char_modifier_t modifiers, ble_gatts_char_handles_t* handler);
ret_code_t ble_service_notify(ble_estc_service_t *service);
static uint16_t color_value[3] = {0, 100, 100};

ret_code_t ble_service_init(uint16_t* conn_handle)
{
    flash_service_load_color(color_value);
    color_service_set_hsb_color(color_value[0], color_value[1], color_value[2]);

    ret_code_t error_code = NRF_SUCCESS;

    m_conn_handle = conn_handle;
    ble_uuid_t service_uuid;
    service_uuid.uuid = BLE_SERVICE_UUID;

    ble_uuid128_t m_base_uuid128 = {
        BLE_BASE_UUID
    };
    error_code = sd_ble_uuid_vs_add(&m_base_uuid128, &service_uuid.type);
    APP_ERROR_CHECK(error_code);

    error_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &m_estc_service.service_handle);
    APP_ERROR_CHECK(error_code);

    error_code = ble_service_add_characteristic(&m_estc_service, BLE_BLINKY_HSB_CHAR_UUID, "Set color characteristic", 
                                                (uint8_t*)color_value, BLE_BLINKY_HSB_CHAR_SIZE, WRITE, 
                                                &m_estc_service.blinky_hsb_char_handle);
    APP_ERROR_CHECK(error_code);

    error_code = ble_service_add_characteristic(&m_estc_service, BLE_BLINKY_HSB_NOTIFY_CHAR_UUID, "Get color characteristic", 
                                                (uint8_t*)color_value, BLE_BLINKY_HSB_NOTIFY_CHAR_SIZE, READ | NOTIFY, 
                                                &m_estc_service.blinky_hsb_notify_char_handle);
    
    APP_ERROR_CHECK(error_code);

    return NRF_SUCCESS;
}

ret_code_t ble_service_add_characteristic(ble_estc_service_t *service, uint16_t uuid, const char* description, uint8_t* data, uint16_t data_size,
                                       ble_estc_char_modifier_t modifiers, ble_gatts_char_handles_t* handler)
{
    ret_code_t error_code = NRF_SUCCESS;

    ble_uuid128_t service_uuid = {
        BLE_BASE_UUID
    };
    ble_uuid_t char_uuid = {
        .uuid = uuid,
        .type = BLE_UUID_TYPE_VENDOR_BEGIN
    };

    error_code = sd_ble_uuid_vs_add(&service_uuid, &char_uuid.type);
    APP_ERROR_CHECK(error_code);

    ble_gatts_char_md_t char_md = { 0 };
    char_md.char_props.read = modifiers & READ ? 1 : 0;
    char_md.char_props.write = modifiers & WRITE ? 1 : 0;
    char_md.char_props.notify = modifiers & NOTIFY ? 1 : 0;
    char_md.char_props.indicate = modifiers & INDICATE ? 1 : 0;

    char_md.p_char_user_desc = (uint8_t *) description;
    char_md.char_user_desc_size = strlen(description);
    char_md.char_user_desc_max_size = strlen(description);

    ble_gatts_attr_md_t attr_md = { 0 };
    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_t attr_char_value = { 0 };
    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = data_size;
    attr_char_value.max_len = data_size;
    attr_char_value.p_value = data;

    error_code = sd_ble_gatts_characteristic_add(service->service_handle, &char_md, &attr_char_value, handler);
    APP_ERROR_CHECK(error_code);

    return error_code;
}

ret_code_t ble_service_write_handler(ble_evt_t const * p_ble_evt)
{
    ret_code_t err_code = NRF_SUCCESS;

    switch (p_ble_evt->header.evt_id)
    {
         case BLE_GATTS_EVT_WRITE:
         {
            const ble_gatts_evt_write_t *  p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
            if (p_evt_write->handle == m_estc_service.blinky_hsb_char_handle.value_handle)
            {
                ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
                uint16_t* colors = (uint16_t*) p_evt_write->data;
                color_value[0] = colors[0];
                color_value[1] = colors[1];
                color_value[2] = colors[2];

                color_service_set_hsb_color(colors[0], colors[1], colors[2]);
                ble_service_notify(&m_estc_service);
                flash_service_save_color(colors[0], colors[1], colors[2]);
            }
            break;
         }
    }
    return err_code;
}

ret_code_t ble_service_notify(ble_estc_service_t *service) {
    if (*m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        ble_gatts_hvx_params_t hvx_params = {0};
        ret_code_t err_code;
        uint16_t len = BLE_BLINKY_HSB_NOTIFY_CHAR_SIZE;

        hvx_params.handle = service->blinky_hsb_notify_char_handle.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t*)color_value;

        err_code = sd_ble_gatts_hvx(*m_conn_handle, &hvx_params);
        if ((err_code != NRF_SUCCESS) &&
            (err_code != NRF_ERROR_INVALID_STATE) &&
            (err_code != NRF_ERROR_RESOURCES) &&
            (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING))
        {
            APP_ERROR_HANDLER(err_code);
        }
    }
    return NRF_SUCCESS;
}

/**
 * TODO:
 * [+] add write observer
 * [+] add write notification
 * [ ] add write saving
 */