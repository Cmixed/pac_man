﻿#pragma comment(lib,"winmm.lib")

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
import tool;
import file_io;

using namespace std;
using namespace chrono;

namespace g
{
	// 设置目标帧率
	static unsigned long Target_FPS{ 60 };
	static unsigned long Skill_Time{ (1000 / Target_FPS) * 2 };

	auto start{chrono::system_clock::now()};

	int num{0}, num1{0}, g1_track_x{0}, g1_track_y{0};
	int last_step_x{1}, last_step_y{1};
	int phase{0}, ghost2_goal{0}, shake{1}, pos{0};
	bool voice{true};

	int map[20][20]{0};

	IMAGE startbk, bk, man[4][2], manClose[2], enemy[2][4], enemy3[2][4], mask, title, title2, introduction[3], fruit[2], scared;

	bool isWin{true}, isEat{false}, isSkilled{false};
	unsigned int eatNumber{ 0 };

	unsigned int player_speed{ 10 }, ghost_speed{ 10 };
}

// 函数列表
bool init_game();
optional<int> menu_pause();
optional<int> menu_start();
optional<int> game_core();
optional<int> game_end(bool is_win, unsigned long long score);


int main(int argc, char* argv[])
{
	using namespace g;

BEGIN:

	// 确保正确初始化
	try {
		if (!init_game()) {
			throw runtime_error{"Init filed!!!"};
		}
	}
	catch (exception& e) {
		cerr << format("{: <30}", e.what());
		return 0;
	}

	// cmd 命令行命令
	{
		if (argv[1] == "God") {
			g::num = 149;
		}
		if (argv[1] == "Win") {
			g::isWin = true;
		}
		if (argv[1] == "Music") {
			play_music_pause();
		}

	}

	BeginBatchDraw();

	// 开始菜单
	{
		switch (menu_start().value()) {
		case -1:
			return 0;
		case 1:
			break;
		default:
			return 0;
		}

		PacConfigure configure;
		fileRead(configure);
		g::Target_FPS = configure.m_fps;
		g::player_speed = configure.m_playerSpeed;
		g::ghost_speed = configure.m_ghostSpeed;
	}

	unsigned long long score{ 0 };
	bool is_exit{ false };
	std::mutex m;
	std::thread score_t([&]()
	{
		std::unique_lock<std::mutex> lock(m);
		while (true) {
			this_thread::sleep_for(100ms);
			score++;
			if (is_exit) {
				score /= 10;
				break;
			}
		}
	});

	// 游戏主程序
	{
		switch (game_core().value()) {
		case -1:
			is_exit = true;
			score_t.join();
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


	is_exit = true;
	score_t.join();

	// 游戏结束 && 状态判定
	{
		if (argv[1] == "Win") {
			g::isWin = true;
		}

		switch (game_end(isWin, score).value()) {
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
	loadimage(&g::startbk , "../src/startbk.jpg");
	loadimage(&g::bk, "../src/bk.jpg");
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
 * @brief 暂停菜单
 * @param
 * @return 返回 1 退出 -1
 */
optional<int> menu_pause()
{
	using namespace g;

	auto begin{ chrono::system_clock::now() };

	// 返回 menu
	while (true) {

		{
			static bool is_first{ true };
			auto end{ chrono::system_clock::now() };
			if (((end-begin) >= 5s) && is_first) {
				play_music_pause();
				is_first = false;
			}
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
			outtextxy(140, 316, "     P A U S E  ");
			settextstyle(40, 0, "Elephant");
			outtextxy(300, 750, "RETURN");
		}

		ExMessage k_m{ 0 };
		peekmessage(&k_m, EX_KEY | EX_MOUSE);
		{
			if (k_m.vkcode == VK_ESCAPE) {
				return optional<int>{-1};
			}
			if (k_m.vkcode == VK_SPACE) {
				return optional<int>{1};
			}

			if (k_m.message == WM_LBUTTONDOWN &&
				k_m.x > 292 && k_m.x < 416 && k_m.y > 750 && k_m.y < 786) {
				cleardevice();
				return optional<int>{1};
			}

		}

		targetFPS(Target_FPS);
		FlushBatchDraw();
		
	}
}

/**
 * @brief 开始菜单
 * @return game 1 end -1
 */
optional<int> menu_start()
{
	using namespace g;

	while (true) {
		// 打印文字到屏幕
		{
			putimage(0, 0, &g::startbk);

			putimage(ui.title1.x, ui.title1.y, &title);
			putimage(ui.title2.x, ui.title2.y, &title2);

			settextstyle(ui.start_menu_text_style.height,
			             ui.start_menu_text_style.width, "Elephant");

			outtextxy(ui.start.x, ui.start.y, "START");
			outtextxy(ui.quit.x, ui.quit.y, "  QUIT");

			settextstyle(32, 0, "Elephant");
			outtextxy(ui.start.x-30, ui.quit.y + 80 , "Settings(Press 1)");
			outtextxy(ui.start.x-30, ui.quit.y + 120, "  Date(Press 2)");
		}

		// 输入件判断
		{
			auto filePathData = ".\\data.txt";
			auto filePathConfigure = ".\\configure.ini";

			ExMessage k_m{ 0 };
			peekmessage(&k_m, EX_KEY | EX_MOUSE);
			if (k_m.message == WM_KEYDOWN) {
				if (k_m.vkcode == VK_ESCAPE) {
					cleardevice();
					return optional<int>{-1};
				}
				if (k_m.vkcode == VK_SPACE) {
					cleardevice();
					return optional<int>{1};
				}
				if (k_m.vkcode == '1') {
					std::string command = "notepad \"" + std::string(filePathConfigure) + "\"";
					system(command.c_str());
				
				}
				if (k_m.vkcode == '2') {
					std::string command = "notepad \"" + std::string(filePathData) + "\"";
					system(command.c_str());
				}
			}

			if (k_m.message == WM_LBUTTONDOWN) {
				if (k_m.x > ui.buttonStart.x_l && k_m.x < ui.buttonStart.x_g
					&& k_m.y > ui.buttonStart.y_l && k_m.y < ui.buttonStart.y_g) {
					cleardevice();
					return optional<int>{1};
				}
				if (k_m.x > ui.buttonQuit.x_l && k_m.x < ui.buttonQuit.x_g
					&& k_m.y > ui.buttonQuit.y_l && k_m.y < ui.buttonQuit.y_g) {
					cleardevice();
					return optional<int>{-1};
				}
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

	{
		mciSendString("close ../src/music/start.mp3", nullptr, 0, nullptr);
		mciSendString("open ../src/music/start.mp3", nullptr, 0, nullptr);
		mciSendString("play ../src/music/start.mp3 repeat", nullptr, 0, nullptr);
	}

	g::start = chrono::system_clock::now();

	while (true) {

		putimage(0, 0, &g::bk);

		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 20; j++) {
				setfillcolor(DARKGRAY);
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
				ghost[2].speed = 0;
				phase = 2;
				num++;
				mciSendString("close ../src/music/start.mp3", nullptr, 0, nullptr);
				mciSendString("close ../src/Fruit.mp3", nullptr, 0, nullptr);
				mciSendString("open ../src/Fruit.mp3", nullptr, 0, nullptr);
				mciSendString("play ../src/Fruit.mp3", nullptr, 0, nullptr);
			}
		}


		if (phase == 2 && voice) {
			voice = false;
			mciSendString("close ../src/Stronger.mp3", nullptr, 0, nullptr);
			mciSendString("open ../src/Stronger.mp3", nullptr, 0, nullptr);
			mciSendString("play ../src/Stronger.mp3 repeat from 0", nullptr, 0, nullptr);
		}

		if (num1 == 3) {
			return optional<int>{2};
		}

		// 处理键盘事件
		{
			ExMessage key{ 0 };
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
				case 'P':
					{
						switch (menu_pause().value()) {
						case -1:
							return optional<int>{-1};
						case 1:
							break;
						default:
							break;
						}
					}
					break;
				case VK_ESCAPE:
					return optional<int>{-1};
				case VK_SPACE:
					g::isSkilled = true;
					break;
				default:
					break;
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
			player.map_x = (player.x + player_speed) / 36;
			player.map_y = (player.y + player_speed) / 36;
			ghost[0].map_x = (ghost[0].x + ghost_speed) / 36;
			ghost[0].map_y = (ghost[0].y + ghost_speed) / 36;
			ghost[1].map_x = (ghost[1].x + ghost_speed) / 36;
			ghost[1].map_y = (ghost[1].y + ghost_speed) / 36;
			ghost[2].map_x = (ghost[2].x + ghost_speed) / 36;
			ghost[2].map_y = (ghost[2].y + ghost_speed) / 36;
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
			if (phase == 2) {
				ghost[1].direction = ghost2Run(ghost[1].map_x, ghost[1].map_y, &ghost2_goal);
			}
			else {
				ghost[1].direction = ghost2(ghost[1].map_x, ghost[1].map_y,
				                            player.map_x, player.map_y, g1_track_x, g1_track_y);
			}
		}

		if (ghost[2].x % 36 == 0 && ghost[2].y % 36 == 0 && phase != 2) {
			ghost[2].direction = ghost3(ghost[2].map_x, ghost[2].map_y, player.map_x,
			                            player.map_y, &ghost[2].speed, &ghost[2].form, &last_step_x, &last_step_y,
			                            ghost[2].direction);
		}

		if (g::map[player.map_y][player.map_x] == 1) {
			g::map[player.map_y][player.map_x] = 2;
			num++;
			{
				play_music_eat();
				g::isEat = true;
				++g::eatNumber;
			}
		}

		for (int i = 0; i < 3; i++) {
			if (ghost[i].live && phase == 2 && ghost[i].map_x == player.map_x && ghost[i].map_y == player.map_y) {
				ghost[i].live = 0;
				num1++;
				mciSendString("close ../src/Ghost.mp3", nullptr, 0, nullptr);
				mciSendString("open ../src/Ghost.mp3", nullptr, 0, nullptr);
				mciSendString("play ../src/Ghost.mp3", nullptr, 0, nullptr);
			}
		}

		// 控制每帧更新
		{
			auto nowFps = targetFPS(g::Target_FPS);

			static unsigned int calSkllFps{ 0 };

			if (isSkilled) {
				if (calSkllFps <= 30) {
					Sleep(g::Skill_Time);
					++calSkllFps;
				} else {
					calSkllFps = 0;
					g::isSkilled = false;
				}
			}

			constexpr unsigned int changeCal{ 15 };
			static int calFps{ 0 };

			// 刷新主角状态
			if (nowFps == 0) {
				++calFps;
				if (g::isEat == true && calFps >= changeCal) {
					player.close = !player.close;
					calFps = 0;
					g::isEat = false;
				}
			}

			// 计时
			{
				auto endTime = chrono::system_clock::now();
				auto duration = timeDifferenceInSeconds(g::start, endTime);
				const char* time = duration.c_str();
				settextstyle(30, 0, "Elephant");
				outtextxy(10, 10, "TIME(s): ");
				outtextxy(140, 10, _T(time));
			}

			FlushBatchDraw();
			cleardevice();
		}

	}
}

/**
 * 
 * @param is_win 是否获胜
 * @return 继续 1 退出 -1
 */
optional<int> game_end(bool is_win, unsigned long long score)
{
	using namespace g;

	score += g::eatNumber;
	const auto temp_text = cast2String(score);
	const char * text = temp_text.c_str();

	auto endTime = chrono::system_clock::now();
	auto duration = timeDifferenceInSeconds(g::start, endTime);
	const char* time = duration.c_str();

	// 将对局信息写入文件
	{
		auto record = PacRecord{ duration, score };
		fileWrite(record);
	}

	// 获胜
	if (is_win) {
		// 音乐处理
		{
			mciSendString("close ../src/Stronger.mp3", nullptr, 0, nullptr); 
			mciSendString("close ../src/Winner.mp3", nullptr, 0, nullptr);
			mciSendString("open ../src/Winner.mp3", nullptr, 0, nullptr);
			mciSendString("play ../src/Winner.mp3", nullptr, 0, nullptr);
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

			settextstyle(35, 0, "Elephant");
			outtextxy(10, 740, "TIME(s): ");
			outtextxy(140, 740, _T(time));
			settextstyle(35, 0, "Elephant");
			outtextxy(10, 790, "SCORE: ");
			outtextxy(140, 790, _T(text));
			FlushBatchDraw();
		}
	}
	else {
		// 音乐处理
		{
			mciSendString("close ../src/Start.mp3", nullptr, 0, nullptr);
			mciSendString("close ../src/Death.mp3", nullptr, 0, nullptr);
			mciSendString("open ../src/Death.mp3", nullptr, 0, nullptr);
			mciSendString("play ../src/Death.mp3", nullptr, 0, nullptr);
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

			settextstyle(35, 0, "Elephant");
			outtextxy(10, 740, "TIME(s): ");
			outtextxy(140, 740, _T(time));
			settextstyle(35, 0, "Elephant");
			outtextxy(10, 790, "SCORE: ");
			outtextxy(140, 790, _T(text));
			FlushBatchDraw();
		}
	}

	// 返回 menu
	{
		while (true) {

			cleardevice();

			ExMessage k_m{ 0 };
			peekmessage(&k_m, EX_KEY | EX_MOUSE);

			if (k_m.vkcode == VK_ESCAPE) {
				return optional<int>{-1};
			}
			if (k_m.vkcode == VK_SPACE) {
				return optional<int>{1};
			}

			if (k_m.message == WM_LBUTTONDOWN &&
				k_m.x > 292 && k_m.x < 416 && k_m.y > 750 && k_m.y < 786) {
				return optional<int>{1};
			}

			targetFPS(Target_FPS);
		}
	}
}