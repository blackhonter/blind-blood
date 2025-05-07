#ifndef MAP_H
	#define MAP_H

	#include <stdint.h>
	#include <stdbool.h>
	#include <iostream>

	#include "definitions.h"
	#include "functions.h"

	class Map
	{
		public:
			int ambient[MAP_NUM_ROWS][MAP_NUM_COLS] = 
			{
			    {1, 1, 1, 2, 2, 1, 1, 1},
			    {1, 0, 0, 0, 0, 3, 3, 1},
			    {1, 0, 4, 4, 0, 0, 0, 1},
			    {1, 0, 4, 4, 0, 0, 0, 1},
			    {1, 0, 0, 0, 0, 0, 0, 1},
			    {1, 0, 0, 0, 0, 0, 0, 1},
			    {1, 0, 0, 4, 4, 0, 0, 1},
			    {1, 5, 5, 5, 5, 6, 5, 1}
			};

			Map(void);

			bool is_inside_map(float x, float y);
			int get_map_content_at(float x, float y);
			int * get_part_of_map(int x, int y, float a);

		private:
			int* mtm;
	};

#endif