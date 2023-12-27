/*
Copyright 2020 Evy Dekkers

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

RGB_MATRIX_EFFECT(solid_esc)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
#define MAX3(a, b, c) ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))

static HSV rgb_to_hsv(RGB rgb) {
    HSV hsv;
    uint8_t rgb_min, rgb_max;
    rgb_min = MIN3(rgb.r, rgb.g, rgb.b);
    rgb_max = MAX3(rgb.r, rgb.g, rgb.b);

    hsv.v = rgb_max;
    if (hsv.v == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * (rgb_max - rgb_min) / hsv.v;
    if (hsv.s == 0) {
        hsv.h = 0;
        return hsv;
    }

    if (rgb_max == rgb.r) {
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgb_max - rgb_min);
    } else if (rgb_max == rgb.g) {
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgb_max - rgb_min);
    } else {
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgb_max - rgb_min);
    }

    return hsv;
}

// Solid + reactive ESC
static bool solid_esc(effect_params_t* params) {
    effect_runner_reactive(params, &SOLID_REACTIVE_math);

    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // yellow ESC
    HSV hsv = rgb_to_hsv((RGB){0xFF, 0xFF, 0x00});
    hsv.v = rgb_matrix_config.hsv.v;
    RGB rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(0, rgb.r, rgb.g, rgb.b);

    // blue-white F1-F12 prScreen
    hsv = rgb_to_hsv((RGB){0xAD, 0xD8, 0xE6});
    hsv.v = rgb_matrix_config.hsv.v;
    rgb = hsv_to_rgb(hsv);
    for (uint8_t i = 1 ; i < 13; i++) {
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    rgb_matrix_set_color(13, rgb.r, rgb.g, rgb.b);

    // yellow insert delete backspace enter rShift rAlt rCtrl tab lShift lAlt lCtrl pgUp pgDn home end
    hsv = rgb_to_hsv((RGB){0x5F, 0xFF, 0x00});
    hsv.v = rgb_matrix_config.hsv.v;
    rgb = hsv_to_rgb(hsv);
    uint8_t static_keys[] = {14, 15, 16, 17, 31, 45, 46, 47, 60, 71, 73, 77, 79, 81, 83};
    uint8_t num_static_keys = sizeof(static_keys) / sizeof(static_keys[0]);
    for (uint8_t i = 0 ; i < num_static_keys; i++) {
        rgb_matrix_set_color(static_keys[i], rgb.r, rgb.g, rgb.b);
    }

    // red caps lock Fn
    hsv = rgb_to_hsv((RGB){0x00, 0xFF, 0x00});
    hsv.v = rgb_matrix_config.hsv.v;
    rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(59, rgb.r, rgb.g, rgb.b);
    rgb_matrix_set_color(78, rgb.r, rgb.g, rgb.b);

    // green arrows
    hsv = rgb_to_hsv((RGB){0xFF, 0x00, 0x00});
    hsv.v = rgb_matrix_config.hsv.v;
    rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(72, rgb.r, rgb.g, rgb.b);
    rgb_matrix_set_color(74, rgb.r, rgb.g, rgb.b);
    rgb_matrix_set_color(75, rgb.r, rgb.g, rgb.b);
    rgb_matrix_set_color(76, rgb.r, rgb.g, rgb.b);

    // blue WIN
    hsv = rgb_to_hsv((RGB){0x00, 0x00, 0xFF});
    hsv.v = rgb_matrix_config.hsv.v;
    rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(82, rgb.r, rgb.g, rgb.b);

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
