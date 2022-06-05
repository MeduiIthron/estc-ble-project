#include "stdint.h"
#include "nrf_fstorage.h"
#include "nrf_fstorage_sd.h"
#include "nrfx_nvmc.h"
#include "nrf_log.h"

#include "../flash_service.h"

#define APP_BOOTLOADER_ADDRESS 0xE0000
#define APP_DATA_AREA_SIZE 0x3000
#define APP_MEMORY_ADDRESS (APP_BOOTLOADER_ADDRESS - APP_DATA_AREA_SIZE)

static uint32_t buffer;
static void storage_evt_handler(nrf_fstorage_evt_t * p_evt) {
    // Do Nothing
}

NRF_FSTORAGE_DEF(nrf_fstorage_t storage) =
{
    .evt_handler = storage_evt_handler,
    .start_addr = APP_MEMORY_ADDRESS,
    .end_addr   = APP_BOOTLOADER_ADDRESS - 1,
};


ret_code_t flash_service_init() 
{
    ret_code_t ret = nrf_fstorage_init(&storage, &nrf_fstorage_sd, NULL);
    APP_ERROR_CHECK(ret);

    return NRF_SUCCESS;
}

ret_code_t flash_service_clear_storage(uint32_t position) {
    ret_code_t ret_code;
    ret_code = nrf_fstorage_erase(&storage, position, 1, NULL);

    if (ret_code != NRF_SUCCESS)
        NRF_LOG_INFO("nrf_fstorage_write ERROR. Code %d.", ret_code);

    return NRF_SUCCESS;
}

ret_code_t flash_service_write_word(uint32_t position, uint32_t value)
{
    ret_code_t ret_code;
    buffer = value;

    ret_code = nrf_fstorage_write(&storage, position, &buffer, sizeof(value), NULL);

        if (ret_code != NRF_SUCCESS)
        NRF_LOG_INFO("nrf_fstorage_write ERROR. Code %d.", ret_code);
    
    return NRF_SUCCESS;
}

ret_code_t flash_service_save_color(uint16_t hue, uint16_t saturation, uint16_t brightness)
{
    uint32_t data = 0;
    data |= hue << 16;
    data |= saturation << 8;
    data |= brightness;

    flash_service_clear_storage(APP_MEMORY_ADDRESS);
    flash_service_write_word(APP_MEMORY_ADDRESS, data);
    return NRF_SUCCESS;
}

ret_code_t flash_service_load_color(uint16_t* color)
{
    uint32_t* data = (uint32_t*)APP_MEMORY_ADDRESS;
    if (*data == 0xFFFFFFFF) {
        color[0] = 0;
        color[1] = 100;
        color[2] = 100;
        return NRF_SUCCESS;
    }
    uint32_t dataValue = *data;
    color[0] = dataValue >> 16;
    color[1] = dataValue - ((dataValue >> 8) << 8);
    color[2] = (dataValue >> 8) - (color[0] << 8);
    return NRF_SUCCESS;
}