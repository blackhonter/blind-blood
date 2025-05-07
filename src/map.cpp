#include "map.h"

Map::Map(void)
{
    this->mtm = new int[MATRIX_NUM_ROWS*MATRIX_NUM_COLS];
}

bool Map::is_inside_map(float x, float y)
{
    return (x >= 0 && x <= MAP_NUM_COLS * TILE_SIZE && y >= 0 && y <= MAP_NUM_ROWS * TILE_SIZE);
}

int Map::get_map_content_at(float x, float y) 
{
    //test if is outside of window
    if (x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS * TILE_SIZE) 
    {
        return 0;
    }

    //floor position to index
    int map_grid_index_x = floor(x / TILE_SIZE);
    int map_grid_index_y = floor(y / TILE_SIZE);

    return this->ambient[map_grid_index_x][map_grid_index_y];
}

int * Map::get_part_of_map(int x, int y, float a)
{
    if (x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS * TILE_SIZE) 
    {
        return this->mtm;
    }

    int map_grid_index_x = floor(x / TILE_SIZE);
    int map_grid_index_y = floor(y / TILE_SIZE);

    int arr[MATRIX_NUM_ROWS][MATRIX_NUM_COLS];

    int i_idx;
    int j_idx;

    //make local matrix with map matrix part
    i_idx = 0;
    for (int i = (int) map_grid_index_x - 2; i < (int) map_grid_index_x + 2; i++)
    {
        j_idx = 0;
        for (int j = (int) map_grid_index_y - 2; j < (int) map_grid_index_y + 2; j++)
        {
            int content = 0;

            if( (i>0 || i<=MAP_NUM_COLS) && (j>0 || j<=MAP_NUM_ROWS) )
            {
                content = this->ambient[i][j] != 0 ? 1 : 0;
            }

            arr[i_idx][j_idx] = content;      

            //Player Position
            if( (i == map_grid_index_x) && (j == map_grid_index_y) )
            {
                arr[i_idx][j_idx] = 9;
            }

            j_idx++;
        }

        i_idx++;
    }


    //ROTATE MATRIX BY ANGLE
    /*
    double sinx = sin(a);
    double cosx = cos(a);

    int xCenter = MATRIX_NUM_ROWS;
    int yCenter = MATRIX_NUM_COLS;

    for(x=0; x<MATRIX_NUM_ROWS; x++) 
    {
        int xt = x - xCenter;

        double xt_cosx = xt*cosx;
        double xt_sinx = xt*sinx;

        for(y=0; y<MATRIX_NUM_COLS; y++) 
        {
            int yt = y - yCenter;

            long xRotate = lround(xt_cosx - (yt*sinx)) + xCenter;
            long yRotate = lround((yt*cosx) + xt_sinx) + yCenter;   

            if( (xRotate >= 0) && (xRotate < MATRIX_NUM_ROWS) && (yRotate >= 0) && (yRotate < MATRIX_NUM_COLS) ) 
            {
                arr[x][y] = arr[xRotate][yRotate];
            }
        }
    }
    */   

    //ROTATE MATRIX BY 90 ANGLE
    int a_time = (rad_to_deg(normalize_angle(a))/90)+1;

    std::cout<<rad_to_deg(a)<<std::endl;
    std::cout<<a_time<<std::endl;

    for(int t=0; t<a_time; t++)
    {
        for (int i = 0; i < MATRIX_NUM_ROWS / 2; i++) 
        {
            for (int j = i; j < MATRIX_NUM_COLS - i - 1; j++) 
            {
                int temp = arr[i][j];

                arr[i][j] = arr[MATRIX_NUM_COLS - 1 - j][i];
                arr[MATRIX_NUM_COLS - 1 - j][i] = arr[MATRIX_NUM_ROWS - 1 - i][MATRIX_NUM_COLS - 1 - j];
                arr[MATRIX_NUM_ROWS - 1 - i][MATRIX_NUM_COLS - 1 - j] = arr[j][MATRIX_NUM_ROWS - 1 - i];
                arr[j][MATRIX_NUM_ROWS - 1 - i] = temp;
            }
        }
    } 

    //make memory matrix and send
    //for (int i = MATRIX_NUM_ROWS - 1; i >= 0; i--) //reverse
    for (int i = 0; i < MATRIX_NUM_ROWS; i++)
    {
        //for(int j = MATRIX_NUM_COLS - 1; j >= 0; j--)//reverse
        for (int j = 0; j < MATRIX_NUM_COLS; j++)
        {
           this->mtm[(MATRIX_NUM_COLS * j) + i] = arr[i][j];
        }
    }

    return this->mtm;
}