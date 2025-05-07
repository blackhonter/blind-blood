#include "game.h"

Game::Game(void)
{
	this->is_running = Graphic::instance() -> initialize();
	this->is_running = Graphic::instance() -> initialize_window();
	this->is_running = Graphic::instance() -> initialize_renderer();
	this->is_running = Graphic::instance() -> initialize_textures();

	this->is_running = Sound::instance() -> initialize();

	this->is_running = GPIO::instance() -> initialize();
}

void Game::setup(void)
{
	//Textures
	Texture::instance() -> load_textures(this->texture_files);

	//Sounds
	Sound::instance() -> load_sounds(this->sound_efx_files, efx);
	Sound::instance() -> load_sounds(this->music_files, music);

		//executa som
        Sound::instance()->play_music(0);
}

void Game::process_input(void)
{
	//SDL CHECK
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				//printf("sdl event quit \n");

				this->is_running = false;
				break;

			case SDL_KEYDOWN:
				//printf("sdl event keydown \n");
	    		
	    		switch(event.key.keysym.sym) 
	    		{
	    			case SDLK_ESCAPE: 
						this->is_running = false;
						break;

					case SDLK_LEFT:
						this->player.set_turn_direction(-1);
						break;

					case SDLK_RIGHT:
						this->player.set_turn_direction(+1);
						break;

					case SDLK_UP:
						this->player.set_walk_direction(+1);
						break;

					case SDLK_DOWN:
						this->player.set_walk_direction(-1);
						break;
				}
				break;

			case SDL_KEYUP:
				//printf("sdl event keyup \n");

				/* Todo: tirar daqui e colocar no render com uma taxa de atualização menor %fps */
				GPIO::instance() -> matrix_print_signal((int *)this->map.get_part_of_map(this->player.x,this->player.y, this->player.turn_angle));

				switch(event.key.keysym.sym) 
	    		{
	    			case SDLK_LEFT:
	    				this->player.last_turn_direction = 0;
	    				this->player.set_turn_direction(0);
						break;

					case SDLK_RIGHT:
						this->player.last_turn_direction = 0;
						this->player.set_turn_direction(0);
						break;

					case SDLK_UP:
						this->player.walk_direction = 0;
						this->player.set_walk_direction(0);
						break;

					case SDLK_DOWN:
						this->player.walk_direction = 0;
						this->player.set_walk_direction(0);
						break;

					case SDLK_m:
						this->is_minimap_visible = !is_minimap_visible;
				}
			    break;

			case SDL_MOUSEMOTION:
				//printf("sdl event mouse motion \n");
				break;

			case SDL_MOUSEBUTTONDOWN:
				//printf("sdl event mouse button down \n");

				switch (event.button.button)
			    {
			        case SDL_BUTTON_LEFT:
			            break;

			        case SDL_BUTTON_RIGHT:
			            break;

			        default:
			            break;
			    }
                break;
                
            case SDL_MOUSEBUTTONUP:
            	//printf("sdl event mouse button up \n");

            	switch (event.button.button)
			    {
			        case SDL_BUTTON_LEFT:
			            break;

			        case SDL_BUTTON_RIGHT:
			            break;

			        default:
			            break;
			    }
           		break;
		}
	}

	//GPIO Check
	switch(GPIO::instance() -> process_input())
	{
		case KEYUP:
			printf("key press up \n");
			break;

		case KEYDOWN:
			printf("key press down \n");
			break;

		case KEYLEFT:
			printf("key press left \n");
			break;

		case KEYRIGHT:
			printf("key press rigth \n");
			break;

		default:
			break;
	}
}

void Game::fix_update(void)
{
	/*
	* Todo: calc FixDeltaTime for Physics
	*/
}

void Game::update(void)
{
	//FPS Control (Fix FPS)
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - this->last_frame_time);
	if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_wait);
	}

	this->frame_time = (SDL_GetTicks() - this->last_frame_time) / 1000.0f;
	float frame_rate = (1.0f / this->frame_time);

	string new_title = (string) GAME_NAME + " - " + to_string(frame_rate);
	const char* title = new_title.c_str();
	Graphic::instance() -> update_window_title(title);

	//Deltatime
	this->delta_time = (SDL_GetTicks() - this->last_frame_time) / 1000.0f;
	this->last_frame_time = SDL_GetTicks();

	EventSystem::instance() -> process_events();

	/*
	* Game Logig Here
	*/	
		this->player.move_player(this->map, this->delta_time);
}

void Game::render(void)
{
	Graphic::instance() -> clear_color_buffer(0xFF000000);

	/*
	* Game Render Here
	*/	
		this->world.render_world_projection(this->map, this->player);

		if (is_minimap_visible)
			this->world.render_world_minimap(this->map, this->player);

		Graphic::instance()->render_color_buffer();
}

void Game::release_resources(void)
{
	EventSystem::instance() -> shut_down();
	Texture::instance() -> free_textures();
	Sound::instance() -> free_sounds();
	Graphic::instance() -> destroy_window();
}