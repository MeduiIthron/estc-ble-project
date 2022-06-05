#ifndef ESTC_PROJECT_DEVICE_LED_H
#define ESTC_PROJECT_DEVICE_LED_H

#include "stdint.h"

/**
 * @brief LED container
 */
typedef uint32_t led_t;

/**
 * @brief Initializes the module for working with LEDs
 */
void led_init();

/**
 * @brief Provides power to the LED
 * @param led LED
 */
void led_on(led_t led);

/**
 * @brief Disconnects power from the LED
 * @param led LED
 */
void led_off(led_t led);

/**
 * @brief Reverses the active state of an LED
 * @param led LED
 */
void led_invert(led_t led);

/**
 * @brief Gets an built-in LED by its number
 * @param number Built-in LED number
 * @return led_t LED
 */
led_t led_get_by_number(uint32_t number);

/**
 * @brief Gets a LED by its gpio pin
 * @param pin LED gpio pin
 * @return led_t LED
 */
led_t led_get_by_pin(uint32_t pin);

/**
 * @brief Gets gpio pin of LED
 * @param led LED
 * @return uint32_t LED gpio pin
 */
uint32_t led_get_pin(led_t led);

#endif // ESTC_PROJECT_DEVICE_LED_H
