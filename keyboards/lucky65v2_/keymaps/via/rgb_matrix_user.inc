// !!! DO NOT ADD #pragma once !!! //
// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(my_cool_effect)
RGB_MATRIX_EFFECT(my_cool_effect2)
RGB_MATRIX_EFFECT(num_light)
RGB_MATRIX_EFFECT(better_splash)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include "my_globals.h"

// static bool num_light(effect_params_t* params){
//   RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   for (uint8_t i = led_min; i < led_max; i++){
//     rgb_matrix_set_color(i, 255, 255, 255);
//   }
//   return rgb_matrix_check_finished_leds(led_max)
// }



uint8_t get_avg(RGB coola){
  return (coola.r + coola.g + coola.b);
}

bool get_brighter_rgb(RGB oone, RGB twwo){
  if (get_avg(oone) > get_avg(twwo)){
    return true;
  }
  else{
    return false;
  }

}

RGB get_brighter_rgbret(RGB oone, RGB twwo){
  if (get_avg(oone) > get_avg(twwo)){
    return oone;
  }
  else{
    return twwo;
  }

}

bool effect_runner_reactive_splash_bet(bool fast_lane, uint8_t start, effect_params_t* params, reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    static uint8_t locked[MATRIX_ROWS][MATRIX_COLS] = {0};
    uint8_t r, g, b;
    uint8_t count = g_last_hit_tracker.count;
    // for (uint8_t i = led_min; i < led_max; i++) {
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row){
      for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
        HSV hsv;
        uint8_t i = g_led_config.matrix_co[row][col];
        RGB_MATRIX_TEST_LED_FLAGS();
        if (!fast_lane){
          hsv = (HSV){71, 255, 200};
        }
        else{
          hsv = (HSV){42, 127, 200};
        }
        hsv.v   = 0;
        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));
            hsv           = effect_func(hsv, dx, dy, dist, tick);

        }

        hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        uint8_t check_lock = locked[row][col];
        rgb_matrix_get_color(i, &r, &g, &b);
        RGB rgb_current = (RGB){ .r = r, .g = g, .b = b };

        if ((row == 2 && col == 0) || (row == 2 && (col == 6 || col == 7 || col == 8 || col == 9)) || (row == 4 && col == 3)) {

          if (fast_lane){

              if (check_lock == 0){
                // fast lane lock OFF
                if(get_avg(rgb) > 150){
                  rgb = get_brighter_rgbret(rgb, rgb_current);
                  locked[row][col] = get_avg(rgb_current);
                }
              }
              else {
                // fast lane lock ON
                rgb = rgb_current;
              }
          }
          else {
            if (check_lock != 0){
              //normal lane lock ON
              if(get_avg(rgb) >  50){
                locked[row][col] = 0; // UNLOCK
              }
              else{
                rgb = rgb_current;
              }
            }
          }
        }
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b); 


      } 
    }
    return rgb_matrix_check_finished_leds(led_max);
}

bool better_splash(effect_params_t* params) {
    if (fast_lane == 1){
      // RGB_MATRIX_USE_LIMITS(led_min, led_max);

      // for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
      //   for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      //     uint8_t index = g_led_config.matrix_co[row][col];
      //     if (row == 2 && col == 0){
      //       rgb_matrix_set_color(index, 128,255,200);
      //     }
      //     else if (row == 2 && (col == 6 || col == 7 || col == 8 || col == 9)){
      //       rgb_matrix_set_color(index, 128,255,200);
      //     }
      //     else if (row == 4 && col == 3){
      //       rgb_matrix_set_color(index, 128,255,200);
      //     }
      //   }
      // }
      return effect_runner_reactive_splash_bet(true, 0, params, &SOLID_SPLASH_math);
    }
    else{
      return effect_runner_reactive_splash_bet(false, 0, params, &SOLID_SPLASH_math);
    }
}

static bool num_light(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      uint8_t index = g_led_config.matrix_co[row][col];
      if (row == 2 && col == 0){
        rgb_matrix_set_color(index, RGB_GREEN+(col * 2));
      }
      else if (row == 2 && (col == 6 || col == 7 || col == 8 || col == 9)){
        rgb_matrix_set_color(index, RGB_GREEN+(col * 2));
      }
      else if (row == 4 && col == 3){
        rgb_matrix_set_color(index, RGB_GREEN+(col * 2));
      }
    }
  }
  return rgb_matrix_check_finished_leds(led_max);
}

// e.g: A simple effect, self-contained within a single method
static bool my_cool_effect(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0xff, 0xff, 0x00);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

// e.g: A more complex effect, relying on external methods and state, with
// dedicated init and run methods
static uint8_t some_global_state;
static void my_cool_effect2_complex_init(effect_params_t* params) {
  some_global_state = 1;
}
static bool my_cool_effect2_complex_run(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0xff, some_global_state++, 0xff);
  }
  return rgb_matrix_check_finished_leds(led_max);
}
static bool my_cool_effect2(effect_params_t* params) {
  if (params->init) my_cool_effect2_complex_init(params);
  return my_cool_effect2_complex_run(params);
}

typedef HSV (*reactive_splash_f)(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS