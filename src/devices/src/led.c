#include "stdint.h"
#include "boards.h"

#include "../led.h"

const uint32_t BUILD_IN_LEDS_COUNT = LEDS_NUMBER;
const uint32_t BUILD_IN_LEDS_LIST[LEDS_NUMBER] = LEDS_LIST;

void led_init()
{
    for (uint32_t i = 0; i < BUILD_IN_LEDS_COUNT; i += 1)
    {
        nrf_gpio_cfg_output(BUILD_IN_LEDS_LIST[i]);
        led_t led = led_get_by_number(i);
        led_off(led);
    }
}

void led_on(led_t led)
{
    nrf_gpio_pin_write((uint32_t)led, LEDS_ACTIVE_STATE);
}

void led_off(led_t led)
{
    nrf_gpio_pin_write((uint32_t)led, !LEDS_ACTIVE_STATE);
}

void led_invert(led_t led)
{
    nrf_gpio_pin_toggle((uint32_t)led);
}

led_t led_get_by_number(uint32_t number)
{
    return (led_t)(BUILD_IN_LEDS_LIST[number]);
}

led_t led_get_by_pin(uint32_t pin)
{
    return (led_t)pin;
}

uint32_t led_get_pin(led_t led)
{
    return (uint32_t)led;
}
