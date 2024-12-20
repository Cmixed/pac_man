﻿module character;

import std;
import map;

using namespace std;

int ghost1(int g_map_x, int g_map_y, int p_map_x, int p_map_y, int* g1_track_x, int* g1_track_y)
{
	int map1[20][20]{0};
	init_map(map1);

	int trace[400][3]{0};
	int k = 0, l = 0, derection = 2;

	trace[k][0] = g_map_x;
	trace[k][1] = g_map_y;
	trace[k][2] = 0;
	int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
	int pc = 0, pre = -1;
	map1[g_map_y][g_map_x] = 2;

	while (trace[k][0] != p_map_x || trace[k][1] != p_map_y) {
		if (pc == 0)
			pre++;
		if (map1[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] == 1) {
			k++;
			trace[k][0] = trace[pre][0] + dx[pc];
			trace[k][1] = trace[pre][1] + dy[pc];
			trace[k][2] = pre;
			map1[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] = 2;
		}
		pc = (pc + 1) % 4;
	}
	*g1_track_x = trace[pre][0];
	*g1_track_y = trace[pre][1];
	while (trace[k][2] != 0) {
		k = trace[k][2];
	}
	if ((g_map_x) < trace[k][0])
		derection = 0;
	if ((g_map_x) > trace[k][0])
		derection = 2;
	if ((g_map_y) > trace[k][1])
		derection = 1;
	if ((g_map_y) < trace[k][1])
		derection = 3;
	return derection;
}

int ghost1Run(int g_map_x, int g_map_y, int p_map_x, int p_map_y)
{
	int map1[20][20]{0};
	init_map(map1);

	int derection[4] = {0}, Derection = 0, max = 0;
	for (int i = 0; i < 4; i++) {
		int g_around_x = g_map_x;
		int g_around_y = g_map_y;
		switch (i) {
		case 0:
			g_around_x++;
			break;
		case 1:
			g_around_y--;
			break;
		case 2:
			g_around_x--;
			break;
		case 3:
			g_around_y++;
			break;
		}
		if (map1[g_around_y][g_around_x] == 1)
			derection[i] = abs(g_around_y - p_map_y) + abs(g_around_x - p_map_x);
	}
	for (int i = 0; i < 4; i++) {
		if (derection[i] > max) {
			max = derection[i];
			Derection = i;
		}
	}
	return Derection;
}

int ghost2(int g_map_x, int g_map_y, int p_map_x, int p_map_y, int g1_track_x, int g1_track_y)
{
	int map1[20][20]{0};
	init_map(map1);

	map1[g1_track_y][g1_track_x] = 2;
	int trace[400][3]{ 0 };
	int k = 0, l = 0, derection = 2;
	trace[k][0] = g_map_x;
	trace[k][1] = g_map_y;
	trace[k][2] = 0;
	int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
	int pc = 0, pre = -1;
	map1[g_map_y][g_map_x] = 2;
	while (trace[k][0] != p_map_x || trace[k][1] != p_map_y) {
		if (pc == 0)
			pre++;
		if (map1[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] == 1) {
			k++;
			trace[k][0] = trace[pre][0] + dx[pc];
			trace[k][1] = trace[pre][1] + dy[pc];
			trace[k][2] = pre;
			map1[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] = 2;
		}
		pc = (pc + 1) % 4;
	}
	while (trace[k][2] != 0)
		k = trace[k][2];
	if ((g_map_x) < trace[k][0])
		derection = 0;
	if ((g_map_x) > trace[k][0])
		derection = 2;
	if ((g_map_y) > trace[k][1])
		derection = 1;
	if ((g_map_y) < trace[k][1])
		derection = 3;
	return derection;
}

int ghost2Run(int g_map_x, int g_map_y, int* ghost2_goal)
{
	int map1[20][20]{0};
	init_map(map1);

	if (g_map_x == 1 && g_map_y == 1)
		*ghost2_goal = 1;
	if (g_map_x == 1 && g_map_y == 18)
		*ghost2_goal = 2;
	if (g_map_x == 18 && g_map_y == 16)
		*ghost2_goal = 3;
	if (g_map_x == 18 && g_map_y == 1)
		*ghost2_goal = 0;
	int goal[4][2]{ 0 };
	goal[0][0] = 1;
	goal[0][1] = 1;
	goal[1][0] = 1;
	goal[1][1] = 18;
	goal[2][0] = 18;
	goal[2][1] = 16;
	goal[3][0] = 18;
	goal[3][1] = 1;
	int trace[400][3]{ 0 };
	int k = 0, l = 0, derection = 2;
	trace[k][0] = g_map_x;
	trace[k][1] = g_map_y;
	trace[k][2] = 0;
	int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
	int pc = 0, pre = -1;
	map1[g_map_y][g_map_x] = 2;
	while (trace[k][0] != goal[*ghost2_goal][0] || trace[k][1] != goal[*ghost2_goal][1]) {
		if (pc == 0)
			pre++;
		if (map1[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] == 1) {
			k++;
			trace[k][0] = trace[pre][0] + dx[pc];
			trace[k][1] = trace[pre][1] + dy[pc];
			trace[k][2] = pre;
			map1[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] = 2;
		}
		pc = (pc + 1) % 4;
	}
	while (trace[k][2] != 0) {
		k = trace[k][2];
	}
	if ((g_map_x) < trace[k][0])
		derection = 0;
	if ((g_map_x) > trace[k][0])
		derection = 2;
	if ((g_map_y) > trace[k][1])
		derection = 1;
	if ((g_map_y) < trace[k][1])
		derection = 3;
	return derection;
}

int ghost3(int g_map_x, int g_map_y, int p_map_x, int p_map_y, int* speed, int* form, int* last_step_x,
           int* last_step_y, int last_direction)
{
	int map1[20][20]{0};
	init_map(map1);

	int map2[20][20]{ 0 };
	init_map(map2, 2);

	int derection = 0, now_step_x = 0, now_step_y = 0;
	if (abs(p_map_x - g_map_x) + abs(p_map_y - g_map_y) <= 6) {
		int trace[400][3]{ 0 };
		int k = 0, l = 0;
		trace[k][0] = g_map_x;
		trace[k][1] = g_map_y;
		trace[k][2] = 0;
		int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
		int pc = 0, pre = -1;
		map1[g_map_y][g_map_x] = 2;
		while (trace[k][0] != p_map_x || trace[k][1] != p_map_y) {
			if (pc == 0)
				pre++;
			if (map1[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] == 1) {
				k++;
				trace[k][0] = trace[pre][0] + dx[pc];
				trace[k][1] = trace[pre][1] + dy[pc];
				trace[k][2] = pre;
				map1[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] = 2;
			}
			pc = (pc + 1) % 4;
		}
		while (trace[k][2] != 0)
			k = trace[k][2];
		if ((g_map_x) < trace[k][0])
			derection = 0;
		if ((g_map_x) > trace[k][0])
			derection = 2;
		if ((g_map_y) > trace[k][1])
			derection = 1;
		if ((g_map_y) < trace[k][1])
			derection = 3;
		if (map2[g_map_y][g_map_x] == 9) {
			now_step_x = g_map_x;
			now_step_y = g_map_y;
		}
		*speed = 3;
		*form = 1;
	}
	else {
		if (map2[g_map_y][g_map_x] == 9) {
			if (last_step_x == 0 && last_step_y == 0) {
				switch (last_direction) {
				case 0:
					derection = 3;
					break;
				case 1:
					derection = 0;
					break;
				case 2:
					derection = 1;
					break;
				case 3:
					derection = 2;
					break;
				}
			}
			else {
				map2[*last_step_y][*last_step_x] = 2;
				int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1}, next_step_x = 0, next_step_y = 0;
				for (int i = 0; i < 4; i++) {
					if (map2[g_map_y + dy[i]][g_map_x + dx[i]] == 9) {
						next_step_x = g_map_x + dx[i];
						next_step_y = g_map_y + dy[i];
					}
				}
				if ((g_map_x) < next_step_x)
					derection = 0;
				if ((g_map_x) > next_step_x)
					derection = 2;
				if ((g_map_y) > next_step_y)
					derection = 1;
				if ((g_map_y) < next_step_y)
					derection = 3;
			}
			now_step_x = g_map_x;
			now_step_y = g_map_y;
		}
		else {
			int trace[400][3]{ 0 };
			int k = 0, l = 0;
			trace[k][0] = g_map_x;
			trace[k][1] = g_map_y;
			trace[k][2] = 0;
			int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
			int pc = 0, pre = -1;
			map2[g_map_y][g_map_x] = 2;
			while (true) {
				if (pc == 0)
					pre++;
				if (map2[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] == 1 || map2[trace[pre][1] + dy[pc]][trace[pre]
					[0] + dx[pc]] == 9) {
					k++;
					trace[k][0] = trace[pre][0] + dx[pc];
					trace[k][1] = trace[pre][1] + dy[pc];
					trace[k][2] = pre;
					if (map2[trace[k][1]][trace[k][0]] == 9)
						break;
					map2[trace[pre][1] + dy[pc]][trace[pre][0] + dx[pc]] = 2;
				}
				pc = (pc + 1) % 4;
			}
			while (trace[k][2] != 0)
				k = trace[k][2];
			if ((g_map_x) < trace[k][0])
				derection = 0;
			if ((g_map_x) > trace[k][0])
				derection = 2;
			if ((g_map_y) > trace[k][1])
				derection = 1;
			if ((g_map_y) < trace[k][1])
				derection = 3;
		}
		*speed = 1;
		*form = 0;
	}
	*last_step_x = now_step_x;
	*last_step_y = now_step_y;
	return derection;
}