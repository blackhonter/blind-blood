#ifndef PLAYER_H
	#define PLAYER_H

	#include <limits.h>
	#include <stdint.h>
	#include <stdbool.h>
	#include <float.h>
	#include <iostream>
	#include <map>

	#include <SDL2/SDL.h>

	#include "definitions.h"
	#include "functions.h"
	#include "sound.h"
	#include "gpio.h"
	#include "eventSystem.h"
	#include "map.h"

	typedef struct
	{
		float ray_index;
		
		float ray_angle;

	    float hit_x;
	    float hit_y;

	    float distance;

	    int hit_content;

	    bool was_hit_vertical;
	    bool was_hit_horizontal;
	} ray_t;

	class Player
	{
		public:
			friend class World;
			friend class Game;

			Player(void);

			void set_turn_direction(int turn_direction);
			void set_walk_direction(int walk_direction);
			void move_player(Map &map, float delta_time);
		
		protected:
			void cast_ray(Map &map, float ray_angle, int strip_id);
			void cast_rays(Map &map);
			
			map<float, float> sine_table;
			map<float, float> cosine_table;
			map<float, float> tangent_table;
			map<float, float> atangent_table;
			map<float, float> normalized_deg_angles_table;
			map<float, float> normalized_rad_angles_table;
			
			float r_temp[NUM_RAYS];

		private:
			float x;
			float y;

			float width;
			float height;

			int turn_direction; // -1 if left, +1 if right
			int walk_direction; // -1 if back, +1 if front

			float turn_angle;

			float walk_speed;
			float turn_speed;

			ray_t wall_rays[NUM_RAYS];
			ray_t floor_rays[NUM_RAYS];
			ray_t roof_rays[NUM_RAYS];

			int last_turn_direction;
			int last_walk_direction;
	};
		
#endif