#include "stdint.h"
#include "nrfx_pwm.h"
#include "boards.h"

#include "../led.h"
#include "../rgb_led.h"

#define RGB_MAX_VALUE 255.0
#define PWM_MAX_VALUE 1000.0
#define PWM_CONFIG \
    { \
        .output_pins  = { \
            NRFX_PWM_PIN_NOT_USED, \
            BSP_LED_1 | NRFX_PWM_PIN_INVERTED, \
            BSP_LED_2 | NRFX_PWM_PIN_INVERTED, \
            BSP_LED_3 | NRFX_PWM_PIN_INVERTED \
        }, \
        .irq_priority = NRFX_PWM_DEFAULT_CONFIG_IRQ_PRIORITY, \
        .base_clock   = (nrf_pwm_clk_t)NRFX_PWM_DEFAULT_CONFIG_BASE_CLOCK, \
        .count_mode   = (nrf_pwm_mode_t)NRFX_PWM_DEFAULT_CONFIG_COUNT_MODE, \
        .top_value    = NRFX_PWM_DEFAULT_CONFIG_TOP_VALUE, \
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL, \
        .step_mode    = (nrf_pwm_dec_step_t)NRFX_PWM_DEFAULT_CONFIG_STEP_MODE \
    }

// RGB LED pwm
static nrfx_pwm_t pwm_instance = NRFX_PWM_INSTANCE(0);
static nrfx_pwm_config_t pwm_config = PWM_CONFIG;
static nrf_pwm_values_individual_t pwm_values_channels[] = {{ 0, 0, 0, 0 }};
static nrf_pwm_values_t pwm_values;
static nrf_pwm_sequence_t pwm_sequence;

static void empty_pwm_handler(nrfx_pwm_evt_type_t eventType)
{
    // Do Nothing
}

// RGB LED Current color
static uint16_t* color_red_value;
static uint16_t* color_green_value;
static uint16_t* color_blue_value;

void rgb_led_init()
{
    color_red_value = &pwm_values_channels[0].channel_1;
    color_green_value = &pwm_values_channels[0].channel_2;
    color_blue_value = &pwm_values_channels[0].channel_3;

    // Init RGB PWM
    pwm_values.p_individual = pwm_values_channels;
    nrfx_pwm_init(&pwm_instance, &pwm_config, empty_pwm_handler);

    // Start RGB PWM
    pwm_sequence.values = pwm_values;
    pwm_sequence.length = NRF_PWM_VALUES_LENGTH(pwm_values_channels);
    pwm_sequence.repeats = 0;
    pwm_sequence.end_delay = 0;
}

void rgb_led_on()
{
    nrfx_pwm_simple_playback(&pwm_instance, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);
}

void rgb_led_set_color(uint16_t red, uint16_t green, uint16_t blue)
{
    *color_red_value = (uint16_t)(((float)red) / RGB_MAX_VALUE * PWM_MAX_VALUE);
    *color_green_value = (uint16_t)(((float)green) / RGB_MAX_VALUE * PWM_MAX_VALUE);
    *color_blue_value = (uint16_t)(((float)blue) / RGB_MAX_VALUE * PWM_MAX_VALUE);
}

void rgb_led_off()
{
    nrfx_pwm_stop (&pwm_instance, false);
    led_off (BSP_LED_1);
    led_off (BSP_LED_2);
    led_off (BSP_LED_3);
}