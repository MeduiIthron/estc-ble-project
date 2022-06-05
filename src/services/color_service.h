#ifndef ESTC_PROJECT_SERVICES_COLOR_SERVICE_H
#define ESTC_PROJECT_SERVICES_COLOR_SERVICE_H

#include "stdint.h"
#include "app_error.h"

ret_code_t color_service_init();
ret_code_t color_service_set_rgb_color(uint16_t red, uint16_t green, uint16_t blue);
ret_code_t color_service_set_hsb_color(uint16_t red, uint16_t green, uint16_t blue);

#endif // ESTC_PROJECT_SERVICES_COLOR_SERVICE_H