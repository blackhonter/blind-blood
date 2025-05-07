#ifndef DEFS_H
	#define DEFS_H

	//INCLUDES
		#include <math.h>
		#include "functions.h"
		#include <stdbool.h>

	//DEFINITIONS
		#define GAME_NAME "Blind Blood"
		#define FPS 30
		#define FRAME_TARGET_TIME (1000 / FPS)

		#define TEXTURE_SIZE 64
		#define TILE_SIZE 64
		#define MAP_NUM_ROWS 8
		#define MAP_NUM_COLS 8

		#define WINDOW_WIDTH 640
		#define WINDOW_HEIGHT 400

		#define GPIO_ENABLE false
		#define MATRIX_NUM_ROWS 4
		#define MATRIX_NUM_COLS 4

		#define FOV_ANGLE deg_to_rad(60)
		#define DISTANCE_PROJECTION_PLANE ((WINDOW_WIDTH/2)/tan(FOV_ANGLE/2))

		#define WALL_STRIP_WIDTH 4
		#define NUM_RAYS WINDOW_WIDTH / WALL_STRIP_WIDTH

		#define MINIMAP_SCALE_FACTOR 0.2
		#define MINIMAP_SPACE_FACTOR 10

		#define PI 3.14159265



#endif