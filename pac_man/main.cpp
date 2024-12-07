#pragma comment(lib,"winmm.lib")

#include "easyx/easyx.h"
#include <Windows.h>
#include <mmsystem.h>
#include <chrono>

import std;
import ui;
import character;
import draw;
import music;
import map;

using namespace std;
using namespace chrono;

namespace g
{
	auto start{chrono::system_clock::now()};
	auto end{chrono::system_clock::now()};

	double time{0};
	int num{0}, num1{0}, g1_track_x{0}, g1_track_y{0};
	int last_step_x{1}, last_step_y{1};
	int phase{0}, ghost2_goal{0}, shake{1}, close_time{300}, pos{0};
	bool voice{true};

	int map[20][20]{0};

	IMAGE man[4][2], manClose[2], enemy[2][4], enemy3[2][4], mask, title, title2, introduction[3], fruit[2], scared;

	ExMessage key{0}, msg{0};

	static bool isWin{ true };
}

// 函数列表
bool init_game();
optional<int> menu_start();
void game_start();
optional<int> game_core();
void menu_end();
optional<int> game_end(bool is_win);


int main(int argc, char* argv[])
{
BEGIN:
	using namespace g;

	// 确保正确初始化
	try {
		auto isInit = init_game();
		if (!isInit) {
			throw runtime_error{"Init filed!!!"};
		}
	}
	catch (exception& e) {
		cerr << format("{: <30}", e.what());
		return 0;
	}

	BeginBatchDraw();

	// 开始菜单
	{
		auto menuStatus = menu_start();

		switch (menuStatus.value()) {
		case -1:
			return 0;
		case 1:
			break;
		default:
			return 0;
		}
	}

	// 游戏主程序
	{
		auto gameStatue = game_core();
		switch (gameStatue.value()) {
		case -1:
			return 0;
		case 1:
			isWin = false;
			break;
		case 2:
			isWin = true;
			break;
		default:
			cerr << "error! Line93";
		}
	}

	// 游戏结束
	{
		auto endStatus = game_end(isWin);

		switch (endStatus.value()) {
		case -1:
			EndBatchDraw();
			return 0;
		case 1:
			goto BEGIN;
		default:
			EndBatchDraw();
			return 0;
		}
	}

}

/**
 * @brief 初始化游戏资源
 * @return true 成功 false 失败
 */
bool init_game()
{
	using namespace g;

	initgraph(ui.graph.width, ui.graph.height);
	init_map(g::map);

	// 加载美术资源
	loadimage(&g::man[0][0], "../src/open0.jpg");
	loadimage(&g::man[0][1], "../src/open0-mask.jpg");
	loadimage(&g::man[1][0], "../src/open1.jpg");
	loadimage(&g::man[1][1], "../src/open1-mask.jpg");
	loadimage(&g::man[2][0], "../src/open2.jpg");
	loadimage(&g::man[2][1], "../src/open2-mask.jpg");
	loadimage(&g::man[3][0], "../src/open3.jpg");
	loadimage(&g::man[3][1], "../src/open3-mask.jpg");
	loadimage(&manClose[0], "../src/close.jpg");
	loadimage(&manClose[1], "../src/close-mask.jpg");
	loadimage(&enemy[0][0], "../src/ghost1-right.jpg");
	loadimage(&enemy[0][1], "../src/ghost1-up.jpg");
	loadimage(&enemy[0][2], "../src/ghost1-left.jpg");
	loadimage(&enemy[0][3], "../src/ghost1-down.jpg");
	loadimage(&enemy[1][0], "../src/ghost2-right.jpg");
	loadimage(&enemy[1][1], "../src/ghost2-up.jpg");
	loadimage(&enemy[1][2], "../src/ghost2-left.jpg");
	loadimage(&enemy[1][3], "../src/ghost2-down.jpg");
	loadimage(&enemy3[0][0], "../src/ghost3-right-0.jpg");
	loadimage(&enemy3[0][1], "../src/ghost3-up-0.jpg");
	loadimage(&enemy3[0][2], "../src/ghost3-left-0.jpg");
	loadimage(&enemy3[0][3], "../src/ghost3-down-0.jpg");
	loadimage(&enemy3[1][0], "../src/ghost3-right-1.jpg");
	loadimage(&enemy3[1][1], "../src/ghost3-up-1.jpg");
	loadimage(&enemy3[1][2], "../src/ghost3-left-1.jpg");
	loadimage(&enemy3[1][3], "../src/ghost3-down-1.jpg");
	loadimage(&mask, "../src/ghost-mask.jpg");
	loadimage(&title, "../src/title.png");
	loadimage(&title2, "../src/title2.png");
	loadimage(&introduction[0], "../src/introduction.png");
	loadimage(&introduction[1], "../src/introduction2.jpg");
	loadimage(&introduction[2], "../src/introduction3.jpg");
	loadimage(&fruit[0], "../src/fruit.jpg");
	loadimage(&fruit[1], "../src/fruit-mask.jpg");
	loadimage(&scared, "../src/ghost3-scared.jpg");

	player.x = 36;
	player.y = 648;
	player.direction = 0;
	player.speed = 3;
	ghost[0].x = 648;
	ghost[0].y = 36;
	ghost[0].direction = 2;
	ghost[0].speed = 2;
	ghost[0].live = true;
	ghost[1].x = 648;
	ghost[1].y = 576;
	ghost[1].direction = 2;
	ghost[1].speed = 2;
	ghost[1].live = true;
	ghost[2].x = 36;
	ghost[2].y = 36;
	ghost[2].direction = 0;
	ghost[2].speed = 1;
	ghost[2].form = 0;
	ghost[2].live = true;

	return true;
}

/**
 * @brief 开始菜单
 * @return game 1 end -1
 */
optional<int> menu_start()
{
	using namespace g;

	bool isRun{true};
	while (isRun) {
		// 打印文字到屏幕
		{
			putimage(ui.title1.x, ui.title1.y, &title);
			putimage(ui.title2.x, ui.title2.y, &title2);

			settextstyle(ui.start_menu_text_style.height,
			             ui.start_menu_text_style.width, "Elephant");

			outtextxy(ui.start.x, ui.start.y, "START");
			outtextxy(ui.quit.x, ui.quit.y, "  QUIT");
		}

		// 获取键盘 鼠标 的 状态 位置
		{
			peekmessage(&msg, EX_MOUSE);
			//peekmessage(&key, EX_KEY);
		}

		// 鼠标时间判断
		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x > ui.buttonStart.x_l && msg.x < ui.buttonStart.x_g
				&& msg.y > ui.buttonStart.y_l && msg.y < ui.buttonStart.y_g) {
				cleardevice();
				return optional<int>{1};
			}
			if (msg.x > ui.buttonQuit.x_l && msg.x < ui.buttonQuit.x_g
				&& msg.y > ui.buttonQuit.y_l && msg.y < ui.buttonQuit.y_g) {
				cleardevice();
				return optional<int>{-1};
			}
		}

		FlushBatchDraw();
	}
}

/**
 * @brief 主游戏程序
 * @return win 2 lose 1 quit -1
 */
optional<int> game_core()
{
	using namespace g;

	mciSendString("close ../src/music/start.mp3", nullptr, 0, nullptr);
	mciSendString("open ../src/music/start.mp3", nullptr, 0, nullptr);
	mciSendString("play ../src/music/start.mp3 repeat", nullptr, 0, nullptr);

	while (true) {
		auto start = chrono::system_clock::now();

		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 20; j++) {
				setfillcolor(BLUE);
				if (g::map[i][j] != 0)
					solidrectangle(36 * j, 36 * i, 36 * j + 36, 36 * i + 36);
				setfillcolor(WHITE);
				if (g::map[i][j] == 1)
					solidrectangle(36 * j + 16, 36 * i + 16, 36 * j + 20, 36 * i + 20);
			}
		}
		if (phase == 1) {
			putimage(396, 324, &fruit[1], SRCAND);
			putimage(396, 324, &fruit[0], SRCPAINT);
		}
		putimage(0, 720, &introduction[phase]);

		if (num == 149) {
			phase = 1;
			if (player.map_y == 9 && player.map_x == 11) {
				close_time = 100;
				ghost[2].speed = 0;
				phase = 2;
				num++;
				mciSendString("close ../src/music/start.mp3", NULL, 0, NULL);
				mciSendString("close ../src/Fruit.mp3", NULL, 0, NULL);
				mciSendString("open ../src/Fruit.mp3", NULL, 0, NULL);
				mciSendString("play ../src/Fruit.mp3", NULL, 0, NULL);
			}
		}


		if (phase == 2 && voice) {
			voice = false;
			mciSendString("close ../src/Stronger.mp3", NULL, 0, NULL);
			mciSendString("open ../src/Stronger.mp3", NULL, 0, NULL);
			mciSendString("play ../src/Stronger.mp3 repeat from 0", NULL, 0, NULL);
		}

		if (num1 == 3) {
			return optional<int>{2};
		}

		// 处理键盘事件
		{
			peekmessage(&key, EX_KEY);
			if (key.message == WM_KEYDOWN) {
				switch (key.vkcode) {
				case 'W':
					if (g::map[player.map_y - 1][player.map_x] != 0) {
						player.direction = 1;
						player.x = player.map_x * 36;
					}
					break;
				case 'A':
					if (g::map[player.map_y][player.map_x - 1] != 0) {
						player.direction = 2;
						player.y = player.map_y * 36;
					}
					break;
				case 'S':
					if (g::map[player.map_y + 1][player.map_x] != 0) {
						player.direction = 3;
						player.x = player.map_x * 36;
					}
					break;
				case 'D':
					if (g::map[player.map_y][player.map_x + 1] != 0) {
						player.direction = 0;
						player.y = player.map_y * 36;
					}
					break;
				case VK_ESCAPE:
					return optional<int>{-1};
				default:
					cerr << "error";
				}
			}
		}

		// 更新对象状态
		{
			switch (player.direction) {
			case 0:
				if (g::map[player.map_y][player.x / 36 + 1] != 0)
					player.x += player.speed;
				break;
			case 1:
				if (g::map[player.y / 36][player.map_x] != 0)
					player.y -= player.speed;
				break;
			case 2:
				if (g::map[player.map_y][player.x / 36] != 0)
					player.x -= player.speed;
				break;
			case 3:
				if (g::map[player.y / 36 + 1][player.map_x] != 0)
					player.y += player.speed;
				break;
			default:
				cerr << "error";
			}

			for (int i{0}; i < 3; i++) {
				switch (ghost[i].direction) {
				case 0:
					ghost[i].x += ghost[i].speed;
					break;
				case 1:
					ghost[i].y -= ghost[i].speed;
					break;
				case 2:
					ghost[i].x -= ghost[i].speed;
					break;
				case 3:
					ghost[i].y += ghost[i].speed;
					break;
				default:
					cerr << "error";
				}
			}
		}

		// 绘制对象贴图
		{
			if (player.close == 0) {
				putimage(player.x, player.y, &g::man[player.direction][1], SRCAND);
				putimage(player.x, player.y, &g::man[player.direction][0], SRCPAINT);
			}
			else if (player.close == 1) {
				putimage(player.x, player.y, &manClose[1], SRCAND);
				putimage(player.x, player.y, &manClose[0], SRCPAINT);
			}

			if (ghost[0].live) {
				putimage(ghost[0].x, ghost[0].y, &mask, SRCAND);
				putimage(ghost[0].x, ghost[0].y, &enemy[0][ghost[0].direction], SRCPAINT);
			}
			if (ghost[1].live) {
				putimage(ghost[1].x, ghost[1].y, &mask, SRCAND);
				putimage(ghost[1].x, ghost[1].y, &enemy[1][ghost[1].direction], SRCPAINT);
			}
			if (ghost[2].live) {
				if (phase == 2) {
					shake = -shake;
					putimage(ghost[2].x, ghost[2].y + shake, &mask, SRCAND);
					putimage(ghost[2].x, ghost[2].y + shake, &scared, SRCPAINT);
				}
				else {
					putimage(ghost[2].x, ghost[2].y, &mask, SRCAND);
					putimage(ghost[2].x, ghost[2].y, &enemy3[ghost[2].form][ghost[2].direction], SRCPAINT);
				}
			}
		}

		// 更新坐标
		{
			player.map_x = (player.x + 17) / 36;
			player.map_y = (player.y + 17) / 36;
			ghost[0].map_x = (ghost[0].x + 17) / 36;
			ghost[0].map_y = (ghost[0].y + 17) / 36;
			ghost[1].map_x = (ghost[1].x + 17) / 36;
			ghost[1].map_y = (ghost[1].y + 17) / 36;
			ghost[2].map_x = (ghost[2].x + 17) / 36;
			ghost[2].map_y = (ghost[2].y + 17) / 36;
		}


		// 失败
		for (int i{0}; i < 3; i++) {
			if (phase != 2 && ghost[i].map_x == player.map_x && ghost[i].map_y == player.map_y) {
				return optional<int>{1};
			}
		}

		if (ghost[0].x % 36 == 0 && ghost[0].y % 36 == 0) {
			if (phase == 2) {
				ghost[0].direction = ghost1Run(ghost[0].map_x, ghost[0].map_y, player.map_x, player.map_y);
			}
			else {
				ghost[0].direction = ghost1(ghost[0].map_x, ghost[0].map_y, player.map_x, player.map_y, &g1_track_x,
				                            &g1_track_y);
			}
		}

		if (ghost[1].x % 36 == 0 && ghost[1].y % 36 == 0) {
			if (phase == 2)
				ghost[1].direction = ghost2Run(ghost[1].map_x, ghost[1].map_y, &ghost2_goal);
			else
				ghost[1].direction = ghost2(ghost[1].map_x, ghost[1].map_y, player.map_x, player.map_y, g1_track_x,
				                            g1_track_y);
		}

		if (ghost[2].x % 36 == 0 && ghost[2].y % 36 == 0 && phase != 2) {
			ghost[2].direction = ghost3(ghost[2].map_x, ghost[2].map_y, player.map_x, player.map_y, &ghost[2].speed,
			                            &ghost[2].form, &last_step_x, &last_step_y, ghost[2].direction);
		}


		if (g::map[player.map_y][player.map_x] == 1) {
			g::map[player.map_y][player.map_x] = 2;
			num++;
			play_music_eat();
		}


		for (int i = 0; i < 3; i++) {
			if (ghost[i].live && phase == 2 && ghost[i].map_x == player.map_x && ghost[i].map_y == player.map_y) {
				ghost[i].live = 0;
				num1++;
				mciSendString("close ../src/Ghost.mp3", NULL, 0, NULL);
				mciSendString("open ../src/Ghost.mp3", NULL, 0, NULL);
				mciSendString("play ../src/Ghost.mp3", NULL, 0, NULL);
			}
		}

		// 每次渲染间隔时间
		Sleep(30);
		FlushBatchDraw();
		cleardevice();

		auto end = chrono::system_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

		g::time += static_cast<double>(duration.count());

		if (g::time >= close_time) {
			player.close = !player.close;
			g::time = 0;
		}

	}
}

/**
 * 
 * @param is_win 是否获胜
 * @return 继续 1 退出 -1
 */
optional<int> game_end(bool is_win)
{
	using namespace g;

	// 获胜
	if (is_win) {
		// 音乐处理
		{
			mciSendString("close ../src/Stronger.mp3", NULL, 0, NULL);
			mciSendString("close ../src/Winner.mp3", NULL, 0, NULL);
			mciSendString("open ../src/Winner.mp3", NULL, 0, NULL);
			mciSendString("play ../src/Winner.mp3", NULL, 0, NULL);
		}
		// 打印到屏幕
		{
			setlinecolor(WHITE);
			settextcolor(WHITE);
			setfillcolor(BLACK);
			setlinestyle(BS_SOLID, 5);
			roundrect(120, 300, 600, 400, 10, 10);
			solidroundrect(120, 300, 600, 400, 10, 10);
			solidrectangle(0, 720, 720, 840);
			settextstyle(72, 0, "Elephant");
			outtextxy(180, 316, "YOU WIN!!!");
			settextstyle(40, 0, "Elephant");
			outtextxy(300, 750, "MENU");
			FlushBatchDraw();
		}
	} else {
		// 音乐处理
		{
			mciSendString("close ../src/Start.mp3", NULL, 0, NULL);
			mciSendString("close ../src/Death.mp3", NULL, 0, NULL);
			mciSendString("open ../src/Death.mp3", NULL, 0, NULL);
			mciSendString("play ../src/Death.mp3", NULL, 0, NULL);
		}
		// 打印到屏幕
		{
			setlinecolor(WHITE);
			settextcolor(WHITE);
			setfillcolor(BLACK);
			setlinestyle(BS_SOLID, 5);
			roundrect(120, 300, 600, 400, 10, 10);
			solidroundrect(120, 300, 600, 400, 10, 10);
			solidrectangle(0, 720, 720, 840);
			settextstyle(72, 0, "Elephant");
			outtextxy(140, 316, "GAME OVER...");
			settextstyle(40, 0, "Elephant");
			outtextxy(300, 750, "MENU");
			FlushBatchDraw();
		}
	}

	// 返回 menu
	{
		while (true) {
			peekmessage(&g::key, EX_KEY);
			peekmessage(&g::msg, EX_MOUSE);

			if (msg.message == WM_LBUTTONDOWN && msg.x > 292 && msg.x < 416 && msg.y > 750 && msg.y < 786) {
				cleardevice();
				return optional<int>{1};
			}

			if (key.message == WM_KEYDOWN) {
				if (key.vkcode == VK_ESCAPE) {
					return optional<int>{-1};
				}
			}
		}
		
	}

}
