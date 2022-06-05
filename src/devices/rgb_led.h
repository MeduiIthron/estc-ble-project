#ifndef ESTC_PROJECT_DEVICE_RGB_LED_H
#define ESTC_PROJECT_DEVICE_RGB_LED_H

#include "stdint.h"

/**
 * @brief  the module for working with RGB LED
 */
void rgb_led_init();

/**
 * @brief Sets the color of the LED
 * @param color New LED color
 */
void rgb_led_set_color(uint16_t red, uint16_t green, uint16_t blue);

/**
 * @brief Activates the LED
 */
void rgb_led_on();

/**
 * @brief Disables the LED
 */
void rgb_led_off();

#endif // ESTC_PROJECT_DEVICE_RGB_LED_H