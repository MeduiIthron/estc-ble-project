#ifndef ESTC_PROJECT_SERVICES_FLASH_SERVICE_H
#define ESTC_PROJECT_SERVICES_FLASH_SERVICE_H

#include "stdint.h"
#include "app_error.h"

ret_code_t flash_service_init();
ret_code_t flash_service_save_color(uint16_t hue, uint16_t saturation, uint16_t brightness);
ret_code_t flash_service_load_color(uint16_t* color);

#endif // ESTC_PROJECT_SERVICES_FLASH_SERVICE_H