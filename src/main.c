#include <pebble.h>
#include "database.h"
  
static Window *s_main_window;

static TextLayer *month_layer;
static TextLayer *day_layer;
static TextLayer *text_layer;

static GFont month_font;
static GFont day_font;
static GFont text_font;

static char month_text[32];
static char day_text[32];

const char monthsNames[12][20] = {
  "Январь", 
  "Февраль", 
  "Март", 
  "Апрель", 
  "Май", 
  "Июнь", 
  "Июль", 
  "Август", 
  "Сентябрь", 
  "Октябрь", 
  "Ноябрь", 
  "Декабрь"
};

struct tm *get_time() {
  time_t tt = time(0);
  return localtime(&tt);
}

static void update_time() {
  struct tm *tick_time = get_time();

  snprintf(month_text, sizeof(month_text), "%s", monthsNames[tick_time->tm_mon]);
  snprintf(day_text, sizeof(day_text), "%i", tick_time->tm_mday);

  text_layer_set_text(month_layer, month_text);
  text_layer_set_text(day_layer, day_text);
  text_layer_set_text(text_layer, get_event(tick_time->tm_mon + 1, tick_time->tm_mday));
}

static void main_window_load(Window *window) {
  month_layer = text_layer_create(GRect(0, 0, 144, 28));
  day_layer = text_layer_create(GRect(0, 30, 144, 70));
  text_layer = text_layer_create(GRect(0, 98, 144, 70));

  month_font = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  day_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_BOLWE_BOL_64));
  text_font = fonts_get_system_font(FONT_KEY_GOTHIC_18);

  text_layer_set_font(month_layer, month_font);
  text_layer_set_font(day_layer, day_font);
  text_layer_set_font(text_layer, text_font);

  text_layer_set_text_alignment(month_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(month_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_unload(Window *window) {
  fonts_unload_custom_font(day_font);

  text_layer_destroy(month_layer);
  text_layer_destroy(day_layer);
  text_layer_destroy(text_layer);
}

static void init() {
  s_main_window = window_create();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
