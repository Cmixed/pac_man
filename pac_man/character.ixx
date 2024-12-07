export module character;

import std;

using namespace std;

export
{
	int ghost1(int g_map_x, int g_map_y, int p_map_x, int p_map_y, int* g1_track_x, int* g1_track_y);
	int ghost1Run(int g_map_x, int g_map_y, int p_map_x, int p_map_y);
	int ghost2(int g_map_x, int g_map_y, int p_map_x, int p_map_y, int g1_track_x, int g1_track_y);
	int ghost2Run(int g_map_x, int g_map_y, int* ghost2_goal);
	int ghost3(int g_map_x, int g_map_y, int p_map_x, int p_map_y, int* speed, int* form, int* last_step_x, 
		int* last_step_y, int last_direction);
}
