#include <pebble.h>
#include "main.h"
#include "random.h"
	
Window *g_window;
Layer *window_layer;
TextLayer *g_text_layer;
static Layer *stars_layer;
int MAX_STARS = 25;
static const GPathInfo STAR_MICRO_PATH_POINTS = {
	5,
	(GPoint []) {
		{1,0},
		{0,1},
		{1,2},
		{2,1},
		{1,0}
	}
};

static const GPathInfo STAR_SMALL_PATH_POINTS = {
	5,
	(GPoint []) {
		{2,0},
		{0,2},
		{2,3},
		{3,2},
		{2,0}
	}
};

static const GPathInfo STAR_BIG_PATH_POINTS = {
	5,
	(GPoint []) {
		{10,5},
		{20,20},
		{0,10},
		{20,10},
		{0,20},
		{10,5}
	}
};
//static GPath *star_big_path;

static void path_layer_update_callback(Layer *me, GContext *ctx) {
	(void)me;
	graphics_context_set_stroke_color(ctx, GColorWhite);
	
	for(int i = 0; i < MAX_STARS; i++) {
		GPath *path = NULL;
		int x = random(140 + i);
		int y = random(164 + i);
		
		if(i % 3 == 0) {
			path = gpath_create(&STAR_SMALL_PATH_POINTS);
		} else {
			path = gpath_create(&STAR_MICRO_PATH_POINTS);
		}
		
		gpath_move_to(path, GPoint(x, y));
		APP_LOG(APP_LOG_LEVEL_INFO, "%i X: %i Y: %i", i, x, y);
		gpath_draw_outline(ctx, path);
		gpath_destroy(path);
	}
}

void draw_stars() {
  	GRect bounds = layer_get_frame(window_layer);
	stars_layer = layer_create(bounds);
	layer_set_update_proc(stars_layer, path_layer_update_callback);
	layer_add_child(window_layer, stars_layer);
}

void window_load(Window *window) {
	window_set_background_color(g_window, GColorBlack);
	
	g_text_layer = text_layer_create(GRect(0, 57, 144, 50));
	text_layer_set_background_color(g_text_layer, GColorClear);
	text_layer_set_text_color(g_text_layer, GColorWhite);
	
	text_layer_set_font(g_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
	text_layer_set_text_alignment(g_text_layer, GTextAlignmentCenter);
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(g_text_layer));
	
	draw_stars();
}

void window_unload(Window *window) {
	// TODO: Destroy ALL THE THINGS!
	text_layer_destroy(g_text_layer);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	static char buffer[] = "00:00";
	
	strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
	
	text_layer_set_text(g_text_layer, buffer);
}

void init() {
	g_window = window_create();
	window_layer = window_get_root_layer(g_window);
	window_set_window_handlers(g_window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload
	});
	
	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);
	
	window_stack_push(g_window, true);
}

void deinit() {
	window_destroy(g_window);
}
	
int main(void) {
	init();
	app_event_loop();
	deinit();
}