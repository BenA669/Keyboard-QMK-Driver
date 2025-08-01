#pragma once


#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED

typedef HSV (*reactive_splash_f)(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

bool effect_runner_reactive_splash_bet(bool fast_lane, uint8_t start, effect_params_t* params, reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        // uint8_t index = g_led_config.matrix_co[row][col];
        // keypos_t kp = rgb_matrix_config.led_config.led_to_key[i];
        uint8_t index = g_led_config.matrix_co[2][0];
        // bool allowed = (kp.row == 2 && kp.col == 0);
        bool allowed = (i == index)
        if (!allowed) {
            continue;
        }


        if (!fast_lane){
            last_hit_t tracker_copy = g_last_hit_tracker;
        }


        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = rgb_matrix_config.hsv;
        hsv.v   = 0;
        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - tracker_copy.x[j];
            int16_t  dy   = g_led_config.point[i].y - tracker_copy.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(tracker_copy.tick[j], qadd8(rgb_matrix_config.speed, 1));
            hsv           = effect_func(hsv, dx, dy, dist, tick);
        }
        hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_KEYREACTIVE_ENABLED