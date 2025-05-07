#include "player.h"

Player::Player(void)
{
	this->width = 20;
	this->height = 20;

	this->x = (WINDOW_WIDTH / 2);
	this->y = (WINDOW_HEIGHT / 2);

	this->turn_direction = 0;
	this->turn_angle = 0; //0º
    this->turn_speed = deg_to_rad(90);

    this->walk_direction = 0;
    this->walk_speed = 100;

    for (int col = 0; col < NUM_RAYS; col++)
    {
        r_temp[col] = (col - NUM_RAYS / 2) * WALL_STRIP_WIDTH / DISTANCE_PROJECTION_PLANE;

        if (atangent_table.count(r_temp[col]) <= 0)
        {
            atangent_table.insert(make_pair(r_temp[col], atan(r_temp[col])));
        }
    }
}

void Player::set_turn_direction(int turn_direction)
{
	this->turn_direction = turn_direction;
}

void Player::set_walk_direction(int walk_direction)
{
	this->walk_direction = walk_direction;
}

void Player::move_player(Map &map, float delta_time)
{
	//this->turn_angle += normalize_angle(this->turn_direction * this->turn_speed * delta_time);

    if(this->last_turn_direction != this->turn_direction)
    {
        this->turn_angle += normalize_angle(this->turn_direction * this->turn_speed);

        this->last_turn_direction = this->turn_direction;
    }
  

	float move_step = this->walk_direction * (this->walk_speed * delta_time);

    //check cos table
    if (cosine_table.count(this->turn_angle) <= 0)
    {
        cosine_table.insert(make_pair(this->turn_angle, cos(this->turn_angle)));
    }
    float new_player_x = this->x + (cosine_table.at(this->turn_angle) * move_step);

    //check sin table
    if (sine_table.count(this->turn_angle) <= 0)
    {
        sine_table.insert(make_pair(this->turn_angle, sin(this->turn_angle)));
    }
    float new_player_y = this->y + (sine_table.at(this->turn_angle) * move_step);

    float x_to_check =new_player_x;
    float y_to_check =new_player_y;

    //check normalized_deg_angles_table
    if (normalized_deg_angles_table.count(this->turn_angle) <= 0)
    {
        normalized_deg_angles_table.insert(make_pair(this->turn_angle, rad_to_deg(normalize_angle(this->turn_angle))));
    }

    bool is_ray_facing_right = normalized_deg_angles_table.at(this->turn_angle) <= 90 || normalized_deg_angles_table.at(this->turn_angle) >= 270; //bool (90º ~270º)
    bool is_ray_facing_left = !is_ray_facing_right; //bool

    bool is_ray_facing_down = normalized_deg_angles_table.at(this->turn_angle) >= 0 && normalized_deg_angles_table.at(this->turn_angle) <= 180; //bool (0 ~180º)
    bool is_ray_facing_up = !is_ray_facing_down; //bool

    if( (is_ray_facing_right && this->walk_direction > 0) || (is_ray_facing_left && this->walk_direction < 0) )
    {
        x_to_check = new_player_x + this->width * 2;
    }

    if( (is_ray_facing_right && this->walk_direction < 0) || (is_ray_facing_left && this->walk_direction > 0) )
    {
        x_to_check = new_player_x - this->width;
    }

    if( (is_ray_facing_up && this->walk_direction > 0) || (is_ray_facing_down && this->walk_direction < 0) )
    {
        y_to_check = new_player_y - this->height;
    }

    if( (is_ray_facing_up && this->walk_direction < 0) || (is_ray_facing_down && this->walk_direction > 0) )
    {
        y_to_check = new_player_y + this->height * 2;
    }

	if(map.get_map_content_at(x_to_check, y_to_check) == 0)
	{
		this->x = new_player_x;
		this->y = new_player_y;
	}
    else
    {
        //EventSystem::instance() -> send_event("COLISION", (void*) 1);

        //imprime tacton
        /* todo: criar classe de tactons */
        int a0[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        int a1[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        int a2[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        vector<int*> vect1;
        vect1.push_back(a0);
        vect1.push_back(a1);
        vect1.push_back(a2);
        GPIO::instance() -> matrix_print_tacton(vect1);

        //executa som
        Sound::instance()->play_efx(0, 0, normalized_deg_angles_table.at(this->turn_angle), this->wall_rays[NUM_RAYS/2].distance);
    }

	this->cast_rays(map);
}

void Player::cast_ray(Map &map, float ray_angle, int strip_id)
{
    //check normalized_deg_angles_table
    if (normalized_rad_angles_table.count(ray_angle) <= 0)
    {
        normalized_rad_angles_table.insert(make_pair(ray_angle, normalize_angle(ray_angle)));
    }

    ray_angle = normalized_rad_angles_table.at(ray_angle);

    bool is_ray_facing_down = ray_angle > 0 && ray_angle < PI; //bool (0 ~180º)
    bool is_ray_facing_up = !is_ray_facing_down; //bool

    bool is_ray_facing_right = ray_angle < 1.5708f || ray_angle > 4.71239f; //bool (90º ~270º)
    bool is_ray_facing_left = !is_ray_facing_right; //bool

	float x_intercept, y_intercept;
    float x_step, y_step;

    if (tangent_table.count(ray_angle) <= 0)
    {
        tangent_table.insert(make_pair(ray_angle, tan(ray_angle)));
    }

    ///////////////////////////////////////////
    // HORIZONTAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    float horz_wall_hit_x = 0;
    float horz_wall_hit_y = 0;

    bool found_horz_wall_hit = false;
    int horz_wall_content = 0;

    // Find the y-coordinate of the closest horizontal grid intersection
    y_intercept = floor(this->y / TILE_SIZE) * TILE_SIZE;
    y_intercept += is_ray_facing_down ? TILE_SIZE : 0;

    // Find the x-coordinate of the closest horizontal grid intersection
    x_intercept = this->x + (y_intercept - this->y) / tangent_table.at(ray_angle);

    // Calculate the increment xstep and ystep
    y_step = TILE_SIZE;
    y_step *= is_ray_facing_up ? -1 : 1;

    x_step = TILE_SIZE / tangent_table.at(ray_angle);
    x_step *= (is_ray_facing_left && x_step > 0) ? -1 : 1;
    x_step *= (is_ray_facing_right && x_step < 0) ? -1 : 1;

    float next_horz_touch_x = x_intercept;
    float next_horz_touch_y = y_intercept;

    // Increment xstep and ystep until we find a wall
    while (map.is_inside_map(next_horz_touch_x, next_horz_touch_y))
    {
    	float x_to_check = next_horz_touch_x;
    	float y_to_check = next_horz_touch_y + (is_ray_facing_up ? -1 : 0);

        if (map.get_map_content_at(x_to_check, y_to_check) != 0) 
        {
            found_horz_wall_hit = true;

            horz_wall_hit_x = next_horz_touch_x;
            horz_wall_hit_y = next_horz_touch_y;
            
            horz_wall_content = map.get_map_content_at(x_to_check, y_to_check);
            
            break;
        } 
        else 
        {
            next_horz_touch_x += x_step;
            next_horz_touch_y += y_step;
        }
    }

    ///////////////////////////////////////////
    // VERTICAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
  	float vert_wall_hit_x = 0;
    float vert_wall_hit_y = 0;

    bool found_vert_wall_hit = false;
    int vert_wall_content = 0;

    // Find the x-coordinate of the closest vertical grid intersenction
    x_intercept = floor(this->x / TILE_SIZE) * TILE_SIZE;
    x_intercept += is_ray_facing_right ? TILE_SIZE : 0;

    // Find the y-coordinate of the closest vertical grid intersection
    y_intercept = this->y + (x_intercept - this->x) * tangent_table.at(ray_angle);

    // Calculate the increment xstep and ystep
    x_step = TILE_SIZE;
    x_step *= is_ray_facing_left ? -1 : 1;

    y_step = TILE_SIZE * tangent_table.at(ray_angle);
    y_step *= (is_ray_facing_up && y_step > 0) ? -1 : 1;
    y_step *= (is_ray_facing_down && y_step < 0) ? -1 : 1;

    float next_vert_touch_x = x_intercept;
    float next_vert_touch_y = y_intercept;

    // Increment xstep and ystep until we find a wall
    while (map.is_inside_map(next_vert_touch_x, next_vert_touch_y))
    {
    	float x_to_check = next_vert_touch_x + (is_ray_facing_left ? -1 : 0);
		float y_to_check = next_vert_touch_y;

        if (map.get_map_content_at(x_to_check, y_to_check) != 0) 
        {
            found_vert_wall_hit = true;

            vert_wall_hit_x = next_vert_touch_x;
            vert_wall_hit_y = next_vert_touch_y;

            vert_wall_content = map.get_map_content_at(x_to_check, y_to_check);

            break;
        } 
        else 
        {
            next_vert_touch_x += x_step;
            next_vert_touch_y += y_step;
        }
    }

    ///////////////////////////////////////////
    // CALCULATE DE DISTANCES (HORIZONTAL AND VERTICAL) AND CHOSE THE SMALLEST
    ///////////////////////////////////////////

    float horz_hit_distance = found_horz_wall_hit
        ? distance_between_points(this->x, this->y, horz_wall_hit_x, horz_wall_hit_y)
        : FLT_MAX;
    float vert_hit_distance = found_vert_wall_hit
        ? distance_between_points(this->x, this->y, vert_wall_hit_x, vert_wall_hit_y)
        : FLT_MAX;

    //store ray values
    this->wall_rays[strip_id].ray_index = strip_id;
    this->wall_rays[strip_id].ray_angle = ray_angle;

    //only store de minimum smalest of distance
    if(horz_hit_distance < vert_hit_distance)
    {
    	this->wall_rays[strip_id].distance = horz_hit_distance;

        this->wall_rays[strip_id].hit_x = horz_wall_hit_x;
        this->wall_rays[strip_id].hit_y = horz_wall_hit_y;
        
        this->wall_rays[strip_id].was_hit_horizontal = true;
        this->wall_rays[strip_id].was_hit_vertical = false;

        this->wall_rays[strip_id].hit_content = horz_wall_content;
    }
    else
    {
    	this->wall_rays[strip_id].distance = vert_hit_distance;

        this->wall_rays[strip_id].hit_x = vert_wall_hit_x;
        this->wall_rays[strip_id].hit_y = vert_wall_hit_y;
        
        this->wall_rays[strip_id].was_hit_horizontal = false;
        this->wall_rays[strip_id].was_hit_vertical = true;

        this->wall_rays[strip_id].hit_content = vert_wall_content;
    }
}

void Player::cast_rays(Map &map)
{
	for (int col = 0; col < NUM_RAYS; col++) 
    {
        float ray_angle = this->turn_angle + atangent_table.at(r_temp[col]);

        cast_ray(map, ray_angle, col);
    }
}