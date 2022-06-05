#include "stdint.h"

#include "../color.h"

rgb_color_t color_hsb_to_rgb (hsb_color_t* color) {
    rgb_color_t result_color;

    uint32_t p = color->brightness * (100 - color->saturation) / 100;
    uint32_t a = (color->brightness - p) * (color->hue % 60) / 60;
    uint32_t q = p + a;
    uint32_t t = color->brightness - a;
    uint32_t i = color->hue / 60 % 6;

    switch(i) {
        case 0: {
            result_color.red = color->brightness;
            result_color.green = q;
            result_color.blue = p;
            break;
        }
        case 1: {
            result_color.red = t;
            result_color.green = color->brightness;
            result_color.blue = p;
            break;
        }
        case 2: {
            result_color.red = p;
            result_color.green = color->brightness;
            result_color.blue = q;
            break;
        }
        case 3: {
            result_color.red = p;
            result_color.green = t;
            result_color.blue = color->brightness;
            break;
        }
        case 4: {
            result_color.red = q;
            result_color.green = p;
            result_color.blue = color->brightness;
            break;
        }
        case 5: {
            result_color.red = color->brightness;
            result_color.green = p;
            result_color.blue = t;
            break;
        }
    }
    return result_color;
}

hsb_color_t color_rgb_to_hsb (rgb_color_t* color) {
    hsb_color_t result_color;

    double min, max, delta;
    double r, g, b;
    double h, s, v;
    r = color->red / 255;
    g = color->green / 255;
    b = color->blue / 255;

    min = r < g ? r : g;
    min = min  < b ? min  : b;

    max = r > g ? r : g;
    max = max  > b ? max : b;

    v = max;
    delta = max - min;

    if (delta < 0.00001) {
        s = 0;
        h = 0;
        result_color.hue = h * 360;
        result_color.saturation = s * 100;
        result_color.brightness = v * 100;
        return result_color;
    }

    if ( max > 0.0 ) {
        s = (delta / max);
    } else {
        s = 0.0;
        h = 0.0;
        result_color.hue = h * 360;
        result_color.saturation = s * 100;
        result_color.brightness = v * 100;
        return result_color;
    }
    if ( r >= max ) {
        h = (g - b) / delta;
    }
    else if ( g >= max ) {
        h = 2.0 + (b - r) / delta;
    }
    else {
        h = 4.0 + (r - g) / delta;
    }
    h *= 60.0;
    if( h < 0.0 ) {
        h += 360.0;
    }
    result_color.saturation = s * 100;
    result_color.brightness = v * 100;
    return result_color;
}

void color_prepare_rgb_color (rgb_color_t* color) {
    color->red %= 255;
    color->green %= 255;
    color->blue %= 255;
}

void color_prepare_hsb_color (hsb_color_t* color) {
    color->hue %= 361;
    color->saturation %= 101;
    color->brightness %= 101;
}

hsb_color_t color_parse_hsb (uint32_t data) {
    uint32_t hue = data >> 16;
    uint32_t brightness = data - ((data >> 8) << 8);
    uint32_t saturation = (data >> 8) - (hue << 8);
    hsb_color_t color = {
        .hue = hue,
        .saturation = saturation,
        .brightness = brightness
    };
    return color;
}

uint32_t color_compile_hsb (hsb_color_t* color) {
    uint32_t data = 0;
    data |= color->hue << 16;
    data |= color->saturation << 8;
    data |= color->brightness;
    return data;
}