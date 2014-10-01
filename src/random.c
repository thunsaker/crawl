#include <pebble.h>
#include "random.h"
// Thanks FlashBIOS: http://forums.getpebble.com/discussion/4456/random-number-generation
static long default_seed = 1234;

int random_min_max_seed(int min, int max, long seed) {
	seed = (((seed * 214013L + 2531011L) >> 16) & 32767);

	return ((seed % max) + 1);
}

int random_min_max(int min, int max) {
	return random_min_max_seed(min, max, default_seed);
}

int random_seed(int max, long seed) {
	return random_min_max(0, max);
}

int random(int max) {
	return random_seed(max, default_seed);
}