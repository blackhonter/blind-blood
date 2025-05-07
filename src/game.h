#ifndef GAME_H
	#define GAME_H

	#include <iostream>
	#include <stdio.h>
	#include <stdint.h>
	#include <stdbool.h>
	#include <bits/stdc++.h>
	#include <string>
	#include <vector>

	#include <SDL2/SDL.h>

	#include "definitions.h"
	#include "graphic.h"
	#include "texture.h"
	#include "sound.h"
	#include "gpio.h"
	#include "eventSystem.h"
	#include "map.h"
	#include "player.h"
	#include "world.h"

	using namespace std;

	class Game
	{
		public:
			Game(void);

			void setup(void);
			void process_input(void);
			void fix_update(void);
			void update(void);
			void render(void);
			void release_resources(void);

			bool is_running = false;
			
		protected:			
			Map map;
			Player player;
			World world;

			vector<string> texture_files{
				"images/tile-0.png",
				"images/tile-1.png",
				"images/tile-2.png",
				"images/tile-3.png",
				"images/tile-4.png",
				"images/tile-5.png",
				"images/tile-6.png",
				"images/skybox.png",
				"images/sprite-0.png",
				"images/sprite-1.png",
			};

			vector<string> sound_efx_files{
				"sounds/click.wav"
			};

			vector<string> music_files{
				"music/ambient.mp3"
			};

			bool is_minimap_visible = true;

		private:
			float last_frame_time = 0.0f;
			float delta_time = 0.0f;
			float frame_time = 0.0f;
	};
	
#endif