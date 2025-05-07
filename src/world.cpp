#include "world.h"

World::World(void)
{

}

void World::render_world_minimap(Map &map, Player &player)
{
	//RENDER MAP
	for (int i = 0; i < MAP_NUM_ROWS; i++)
    {
        for (int j = 0; j < MAP_NUM_COLS; j++) 
        {
            int tileX =  j * TILE_SIZE;
            int tileY =  i * TILE_SIZE;
            uint32_t tileColor = map.get_map_content_at(tileY, tileX) != 0 ? 0x32FFFFFF : 0x32333333;

            //int teste = map.get_map_content_at(tileY, tileX);
            //printf("%d", teste);

            Graphic::instance() -> drawn_rect_in_color_buffer(tileY*MINIMAP_SCALE_FACTOR+MINIMAP_SPACE_FACTOR, tileX*MINIMAP_SCALE_FACTOR+MINIMAP_SPACE_FACTOR, TILE_SIZE*MINIMAP_SCALE_FACTOR, TILE_SIZE*MINIMAP_SCALE_FACTOR, tileColor);
        }
        //printf("\n");
    }     
    //printf("\n");

    //RENDER PLAYER
    Graphic::instance() -> drawn_rect_in_color_buffer(MINIMAP_SCALE_FACTOR * player.x+MINIMAP_SPACE_FACTOR, MINIMAP_SCALE_FACTOR * player.y+MINIMAP_SPACE_FACTOR, MINIMAP_SCALE_FACTOR * player.width, MINIMAP_SCALE_FACTOR * player.height, 0xFF00FF00);

    //RENDER RAYS
    for(int i=0; i<NUM_RAYS; i++)
	{
		Graphic::instance() -> drawn_line_in_color_buffer(
            MINIMAP_SCALE_FACTOR * (player.x + (player.width/2))+MINIMAP_SPACE_FACTOR ,
            MINIMAP_SCALE_FACTOR * (player.y + (player.height/2))+MINIMAP_SPACE_FACTOR ,
            MINIMAP_SCALE_FACTOR * player.wall_rays[i].hit_x+MINIMAP_SPACE_FACTOR,
            MINIMAP_SCALE_FACTOR * player.wall_rays[i].hit_y+MINIMAP_SPACE_FACTOR,
            0xFF0000FF
        );
	}
}

void World::render_world_projection(Map &map, Player &player)
{
    for(int i = 0; i < NUM_RAYS; i++)
    {
    	if(player.wall_rays[i].hit_content != 0)
    	{
			//check cos table
			float m_temp_value = player.wall_rays[i].ray_angle - player.turn_angle;
			if (player.cosine_table.count(m_temp_value) <= 0)
			{
				player.cosine_table.insert(make_pair(m_temp_value, cos(m_temp_value)));
			}

			float correct_wall_ray_distance = player.wall_rays[i].distance * player.cosine_table.at(m_temp_value);

			//float wall_strip_height = (TILE_SIZE / player.wall_rays[i].distance) * DISTANCE_PROJECTION_PLANE; //fisheye projection
	        float wall_strip_height = (TILE_SIZE / correct_wall_ray_distance) * DISTANCE_PROJECTION_PLANE; //flat projection

	        //WALL
	        int wall_top_pixel = (WINDOW_HEIGHT/2) - (wall_strip_height/2);
		    int wall_bottom_pixel = (WINDOW_HEIGHT/2) + (wall_strip_height/2);

		    //ROOF
	        int roof_top_pixel = 0;
    		int roof_bottom_pixel = wall_top_pixel;

    		//FLOOR
	        int floor_top_pixel = wall_bottom_pixel;
			int floor_bottom_pixel = WINDOW_HEIGHT;

			this->render_sky_box(&player.wall_rays[i], roof_top_pixel, roof_bottom_pixel, 6);
		    this->render_wall(&player.wall_rays[i], wall_strip_height, wall_top_pixel, wall_bottom_pixel, correct_wall_ray_distance);
		   
		   	/* Todo: criar uma matriz no mapa com o numero de andar de cada bloco, fazer um for aqui em baixo com esse valor */
		    if(player.wall_rays[i].hit_content == 3 || player.wall_rays[i].hit_content == 4)
		    {
				this->render_wall(&player.wall_rays[i], wall_strip_height, wall_top_pixel, wall_bottom_pixel, correct_wall_ray_distance, 1);
		    }

			this->render_floor(&player.wall_rays[i], floor_top_pixel, floor_bottom_pixel);
		}
    }
}

void World::render_wall(ray_t *ray, float wall_strip_height, int wall_top_pixel, int wall_bottom_pixel, float distance, int level)
{
	wall_top_pixel = wall_top_pixel - (level*wall_strip_height);
	wall_top_pixel = (wall_top_pixel < 0) ? 0 : wall_top_pixel;

	wall_bottom_pixel = wall_bottom_pixel - (level*wall_strip_height);
	wall_bottom_pixel = (wall_bottom_pixel > WINDOW_HEIGHT) ? WINDOW_HEIGHT : wall_bottom_pixel;

    int texture_wall_num = ray->hit_content - 1;
    int texture_wall_ofsset_x = (ray->was_hit_vertical) ? (int)ray->hit_y % TILE_SIZE : (int)ray->hit_x % TILE_SIZE;

    for(int x=(ray->ray_index*WALL_STRIP_WIDTH); x<(ray->ray_index*WALL_STRIP_WIDTH+WALL_STRIP_WIDTH); x++)
	{
        for(int y= wall_top_pixel; y<wall_bottom_pixel; y++)
        {
        	int texture_wall_ofsset_y = (y + ((wall_strip_height/2) - (WINDOW_HEIGHT/2)) + (level*wall_strip_height) ) * ((float) Texture::instance() -> print_texture(texture_wall_num).height / wall_strip_height);

        	uint32_t texel_color = Texture::instance() -> print_texture(texture_wall_num).texture_buffer[(Texture::instance() -> print_texture(texture_wall_num).width * texture_wall_ofsset_y) + (texture_wall_ofsset_x)];

        	if(ray->was_hit_horizontal)
        	{
        		Graphic::instance() -> change_color_intensity(&texel_color, 100/distance);
        	}
        	else
        	{
        		Graphic::instance() -> change_color_intensity(&texel_color, 160/distance);
        	}

        	Graphic::instance() -> drawn_pixel_in_color_buffer(x,y, texel_color);
        	//Graphic::instance() -> drawn_pixel_in_color_buffer(x,y,0xFF881188);
        }
    }
}

void World::render_roof(ray_t *ray, int roof_top_pixel, int roof_bottom_pixel)
{
	roof_top_pixel = (roof_top_pixel < 0) ? 0 : roof_top_pixel;
	roof_bottom_pixel = (roof_bottom_pixel > WINDOW_HEIGHT) ? WINDOW_HEIGHT : roof_bottom_pixel;

    for(int x=(ray->ray_index*WALL_STRIP_WIDTH); x<(ray->ray_index*WALL_STRIP_WIDTH+WALL_STRIP_WIDTH); x++)
	{
        for(int y= roof_top_pixel; y<roof_bottom_pixel; y++)
        {
        	Graphic::instance() -> drawn_pixel_in_color_buffer(x,y,0xFF444444);
        }
    }
}

void World::render_floor(ray_t *ray, int floor_top_pixel, int floor_bottom_pixel)
{
	floor_top_pixel = (floor_top_pixel < 0) ? 0 : floor_top_pixel;
	floor_bottom_pixel = (floor_bottom_pixel > WINDOW_HEIGHT) ? WINDOW_HEIGHT : floor_bottom_pixel;

	for(int x=(ray->ray_index*WALL_STRIP_WIDTH); x<(ray->ray_index*WALL_STRIP_WIDTH+WALL_STRIP_WIDTH); x++)
	{
	    for(int y= floor_top_pixel; y<floor_bottom_pixel; y++)
	    {
	    	Graphic::instance() -> drawn_pixel_in_color_buffer(x,y,0xFF24292E);
	    }
	}
}

void World::render_sky_box(ray_t *ray, int sky_top_pixel, int sky_bottom_pixel, int num_texture)
{
	sky_top_pixel = (sky_top_pixel < 0) ? 0 : sky_top_pixel;
	sky_bottom_pixel = (sky_bottom_pixel > WINDOW_HEIGHT) ? WINDOW_HEIGHT : sky_bottom_pixel;

	for(int x=(ray->ray_index*WALL_STRIP_WIDTH); x<(ray->ray_index*WALL_STRIP_WIDTH+WALL_STRIP_WIDTH); x++)
	{
        int texture_sky_ofsset_x = x % Texture::instance() -> print_texture(num_texture).width;

        for(int y= sky_top_pixel; y<sky_bottom_pixel; y++)
        {
            int texture_sky_ofsset_y = y % Texture::instance() -> print_texture(num_texture).width;

        	uint32_t texel_color = Texture::instance() -> print_texture(num_texture).texture_buffer[(Texture::instance() -> print_texture(num_texture).width * texture_sky_ofsset_y) + (texture_sky_ofsset_x)];

        	Graphic::instance() -> drawn_pixel_in_color_buffer(x,y, texel_color);
        	//Graphic::instance() -> drawn_pixel_in_color_buffer(x,y,0xFF000000);
        }
    }
}