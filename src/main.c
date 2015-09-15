#include <pebble.h>

Window *window;
TextLayer *month_layer;
TextLayer *day_layer;
TextLayer *text_layer;

static char month_text[] = "month";
static char day_text[] = "00";

void init(void) {  
  window = window_create();
  window_stack_push(window, true);
  
  text_layer = text_layer_create(GRect(0, 92, 144, 76));
//   text_layer_set_text(text_layer, "Lorem ipsum dolor sit amet, consectetur adipiscing elit");
  text_layer_set_text(text_layer, "477 лет со дня запуска первой твердотопливной бабы яги");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
}

static void second_tick(struct tm* tick_time, TimeUnits units_changed) {
  strftime(month_text, 13, "%B", tick_time);
  
  month_layer = text_layer_create(GRect(0, 0, 144, 40));
  text_layer_set_text(month_layer, month_text);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(month_layer));
  text_layer_set_font(month_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(month_layer, GTextAlignmentCenter);
  
  strftime(day_text, sizeof(day_text), "%d", tick_time);
  
  day_layer = text_layer_create(GRect(0, 40, 144, 49));
  text_layer_set_text(day_layer, day_text);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));
  text_layer_set_font(day_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
}

void deinit(void) {
  text_layer_destroy(month_layer);
  text_layer_destroy(day_layer);
  text_layer_destroy(text_layer);
  
  window_destroy(window);
}

int main(void) {
  init();
 
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  second_tick(current_time, SECOND_UNIT);
  
  tick_timer_service_subscribe(SECOND_UNIT, &second_tick);
  
  app_event_loop();
  
  deinit();
}
