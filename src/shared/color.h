#ifndef ESTC_PROJECT_SHARED_COLOR_H
#define ESTC_PROJECT_SHARED_COLOR_H

#include "stdint.h"

/**
 * @brief RGB Color Object
 */
typedef struct {
    uint32_t red;     /// < Red Channel
    uint32_t green;   /// < Green Channel
    uint32_t blue;    /// < Blue Channel
} rgb_color_t;

typedef struct {
    uint32_t hue;        /// < Hue
    uint32_t saturation; /// < Saturation
    uint32_t brightness; /// < Brightness
} hsb_color_t;

/**
 * @brief Convert HSB color to RGB
 * @param color HSB Color
 * @return rgb_color_t RGB Color
 */
rgb_color_t color_hsb_to_rgb (hsb_color_t* color);

/**
 * @brief Convert RGB color to HSB
 * @param color RGB Color
 * @return hsb_color_t HSB Color
 */
hsb_color_t color_rgb_to_hsb (rgb_color_t* color);

/**
 * @brief Converts color to the correct RGB notation
 * @param color RGB Color
 */
void color_prepare_rgb_color (rgb_color_t* color);

/**
 * @brief Converts color to the correct HSB notation
 * @param color HSB Color
 */
void color_prepare_hsb_color (hsb_color_t* color);

/**
 * @brief Retrieves a HSB color from a storage object
 * @param data Compiled color
 * @return hsb_color_t HSB Color
 */
hsb_color_t color_parse_hsb (uint32_t data);

/**
 * @brief Converts a HSB color to a storage object
 * @param color HSB Color
 * @return uint32_t Compiled color
 */
uint32_t color_compile_hsb (hsb_color_t* color);

#endif // ESTC_PROJECT_SHARED_COLOR_H