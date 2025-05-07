#ifndef WORLD_H
	#define WORLD_H

	#include <stdio.h>
	#include <stdint.h>
	#include <math.h>

	#include "definitions.h"
	#include "graphic.h"
	#include "texture.h"
	#include "player.h"
	#include "map.h"


	class World
	{
		public:
			World(void);

			void render_world_minimap(Map &map, Player &player);
			void render_world_projection(Map &map, Player &player);

		private:
			void render_wall(ray_t *ray, float wall_strip_height, int wall_top_pixel, int wall_bottom_pixel, float distance, int level = 0);
			void render_roof(ray_t *ray, int roof_top_pixel, int roof_bottom_pixel);
			void render_floor(ray_t *ray, int floor_top_pixel, int floor_bottom_pixel);
			void render_sky_box(ray_t *ray, int floor_top_pixel, int floor_bottom_pixel, int num_texture);
	};

#endif