#include "estc_service.h"

#include "app_error.h"
#include "nrf_log.h"

#include "ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"

static ret_code_t estc_ble_add_characteristics(ble_estc_service_t *service);

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

    return estc_ble_add_characteristics(service);
}

static ret_code_t estc_ble_add_characteristics(ble_estc_service_t *service)
{
    ret_code_t error_code = NRF_SUCCESS;
    ble_uuid128_t service_uuid = {
        ESTC_BASE_UUID
    };

    ble_uuid_t char_uuid = {
        .uuid = ESTC_GATT_CHAR_UUID,
        .type = BLE_UUID_TYPE_VENDOR_BEGIN,
    };

    error_code = sd_ble_uuid_vs_add(&service_uuid, &char_uuid.type);
    APP_ERROR_CHECK(error_code);

    ble_gatts_char_md_t char_md = { 0 };
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    uint8_t description[] = "Test BLE GATT Characteristic";
    char_md.p_char_user_desc = description;
    char_md.char_user_desc_size = sizeof(description);
    char_md.char_user_desc_max_size = sizeof(description);

    ble_gatts_attr_md_t attr_md = { 0 };
    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_t attr_char_value = { 0 };
    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = 1;
    attr_char_value.max_len = 1;
    attr_char_value.p_value = description;

    error_code = sd_ble_gatts_characteristic_add(service->service_handle, &char_md, &attr_char_value, &service->characteristic_handle);
    APP_ERROR_CHECK(error_code);

    return NRF_SUCCESS;
}