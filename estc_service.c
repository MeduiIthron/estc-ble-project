#include "estc_service.h"

#include "app_error.h"
#include "nrf_log.h"

#include "ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"

ret_code_t estc_ble_service_init(ble_estc_service_t *service)
{
    ret_code_t error_code = NRF_SUCCESS;

    ble_uuid_t service_uuid;
    service_uuid.uuid = ESTC_SERVICE_UUID;

    ble_uuid128_t m_base_uuid128 = {
        ESTC_BASE_UUID
    };
    error_code = sd_ble_uuid_vs_add(&m_base_uuid128, &service_uuid.type);
    APP_ERROR_CHECK(error_code);

    error_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &service->service_handle);
    APP_ERROR_CHECK(error_code);

    NRF_LOG_DEBUG("%s:%d | Service UUID: 0x%04x", __FUNCTION__, __LINE__, service_uuid.uuid);
    NRF_LOG_DEBUG("%s:%d | Service UUID type: 0x%02x", __FUNCTION__, __LINE__, service_uuid.type);
    NRF_LOG_DEBUG("%s:%d | Service handle: 0x%04x", __FUNCTION__, __LINE__, service->service_handle);

    return NRF_SUCCESS;
}

ret_code_t estc_ble_add_characteristic(
                                        ble_estc_service_t *service,
                                        uint16_t uuid,
                                        const char* description,
                                        uint8_t* data,
                                        uint16_t data_size,
                                        ble_estc_char_modifier_t modifiers,
                                        ble_gatts_char_handles_t* handler
                                    ) {
    ret_code_t error_code = NRF_SUCCESS;

    ble_uuid128_t service_uuid = {
        ESTC_BASE_UUID
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