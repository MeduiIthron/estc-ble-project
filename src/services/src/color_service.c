#include "../color_service.h"
#include "../../devices/led.h"
#include "../../devices/rgb_led.h"
#include "../../shared/color.h"

static rgb_color_t current_rgb_color;
static hsb_color_t current_hsb_color;

ret_code_t color_service_init() 
{
    ret_code_t error_code = NRF_SUCCESS;

    rgb_led_init();
    current_rgb_color.red = 255;
    current_hsb_color.saturation = 100;
    current_hsb_color.brightness = 100;
    rgb_led_set_color(current_rgb_color.red, current_rgb_color.blue, current_rgb_color.green);
    rgb_led_on();

    return error_code;
}

ret_code_t color_service_set_rgb_color(uint16_t red, uint16_t green, uint16_t blue) 
{
    ret_code_t error_code = NRF_SUCCESS;

    current_rgb_color.red = red;
    current_rgb_color.green = green;
    current_rgb_color.blue = blue;
    color_prepare_rgb_color(&current_rgb_color);
    current_hsb_color = color_rgb_to_hsb(&current_rgb_color);
    rgb_led_set_color(current_rgb_color.red, current_rgb_color.blue, current_rgb_color.green);

    return error_code;
}

ret_code_t color_service_set_hsb_color(uint16_t hue, uint16_t saturation, uint16_t brightness) 
{
    ret_code_t error_code = NRF_SUCCESS;

    current_hsb_color.hue = hue;
    current_hsb_color.saturation = saturation;
    current_hsb_color.brightness = brightness;
    color_prepare_hsb_color(&current_hsb_color);
    current_rgb_color = color_hsb_to_rgb(&current_hsb_color);
    rgb_led_set_color(current_rgb_color.red, current_rgb_color.blue, current_rgb_color.green);

    return error_code;
}