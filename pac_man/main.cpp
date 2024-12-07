#pragma comment(lib,"winmm.lib")

#include "easyx/easyx.h"
#include <Windows.h>
#include <mmsystem.h>

import std;
import character;
import draw;

using namespace std;
using namespace chrono;

struct man
{
	int x;
	int y;
	int map_x;
	int map_y;
	int derection;
	int speed;
	bool live;
	bool close;
} player;

struct ghost
{
	int x;
	int y;
	int map_x;
	int map_y;
	int derection;
	int speed;
	int form;
	bool live;
} ghost[3];

ExMessage key { 0 };
ExMessage msg { 0 };

int main()
{
	initgraph(720, 840);
TITLE:
	int map[20][20] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					    {0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0},
					    {0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0},
						{0,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,1,0},
						{0,1,0,1,1,1,1,1,1,0,0,1,0,0,0,0,1,1,1,0},
						{0,1,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0},
						{0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0},
						{0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0},
						{0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,1,0,0,0},
						{0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0},
						{0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
						{0,1,0,1,0,0,1,1,1,1,0,1,0,0,0,0,1,1,1,0},
						{0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0},
						{0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0},
						{0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,0},
						{0,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
						{0,1,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0},
						{0,1,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0},
						{0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };
	double time = 0;
	int num = 0, num1 = 0, g1_track_x = 0, g1_track_y = 0;
	int last_step_x = 1, last_step_y = 1;
	int phase = 0, ghost2_goal = 0, shake = 1, close_time = 300, pos = 0;
	bool voice = 1;
	//���ؽ�ɫ
	IMAGE man[4][2], manClose[2], enemy[2][4], enemy3[2][4], mask, title, title2, introduction[3], fruit[2], scared;
	loadimage(&man[0][0], "../src/open0.jpg");
	loadimage(&man[0][1], "../src/open0-mask.jpg");
	loadimage(&man[1][0], "../src/open1.jpg");
	loadimage(&man[1][1], "../src/open1-mask.jpg");
	loadimage(&man[2][0], "../src/open2.jpg");
	loadimage(&man[2][1], "../src/open2-mask.jpg");
	loadimage(&man[3][0], "../src/open3.jpg");
	loadimage(&man[3][1], "../src/open3-mask.jpg");
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
	player.derection = 0;
	player.speed = 3;
	ghost[0].x = 648;
	ghost[0].y = 36;
	ghost[0].derection = 2;
	ghost[0].speed = 2;
	ghost[0].live = 1;
	ghost[1].x = 648;
	ghost[1].y = 576;
	ghost[1].derection = 2;
	ghost[1].speed = 2;
	ghost[1].live = 1;
	ghost[2].x = 36;
	ghost[2].y = 36;
	ghost[2].derection = 0;
	ghost[2].speed = 1;
	ghost[2].form = 0;
	ghost[2].live = 1;
	BeginBatchDraw();
	//����
	while (true)
	{
		putimage(66, 66, &title);
		putimage(116, 600, &title2);
		settextstyle(40, 0, "Elephant");
		outtextxy(290, 330, "START");
		outtextxy(290, 400, "  QUIT");
		peekmessage(&msg, EX_MOUSE);
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x > 288 && msg.x < 416 && msg.y > 330 && msg.y < 364)
			{
				cleardevice();
				goto GAME;
			}
			if (msg.x > 288 && msg.x < 416 && msg.y > 408 && msg.y < 442)
			{
				cleardevice();
				return 0;
			}
		}
		FlushBatchDraw();
	}
    //��Ϸ
GAME:
	mciSendString("close ../src/Start.mp3", NULL, 0, NULL);
	mciSendString("open ../src/Start.mp3", NULL, 0, NULL);
	mciSendString("play ../src/Start.mp3 repeat", NULL, 0, NULL);
	while (true)
	{
		auto start = system_clock::now();
		//���ɵ�ͼ
		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				setfillcolor(BLUE);
				if (map[i][j] != 0)
					solidrectangle(36 * j, 36 * i, 36 * j + 36, 36 * i + 36);
				setfillcolor(WHITE);
				if (map[i][j] == 1)
					solidrectangle(36 * j + 16, 36 * i + 16, 36 * j + 20, 36 * i + 20);
			}
		}
		if (phase == 1)
		{
			putimage(396, 324, &fruit[1], SRCAND);
			putimage(396, 324, &fruit[0], SRCPAINT);
		}
		putimage(0, 720, &introduction[phase]);
		//�жϽ�����һ�׶�
		if (num == 149)
		{
			phase = 1;
			if (player.map_y == 9 && player.map_x == 11)
			{
				close_time = 100;
				ghost[2].speed = 0;
				phase = 2;
				num++;
				mciSendString("close ../src/Start.mp3", NULL, 0, NULL);
				mciSendString("close ../src/Fruit.mp3", NULL, 0, NULL);
				mciSendString("open ../src/Fruit.mp3", NULL, 0, NULL);
				mciSendString("play ../src/Fruit.mp3", NULL, 0, NULL);
			}
		}
		//���׶���Ч
		if (phase == 2 && voice)
		{
			voice = 0;
			mciSendString("close ../src/Stronger.mp3", NULL, 0, NULL);
			mciSendString("open ../src/Stronger.mp3", NULL, 0, NULL);
			mciSendString("play ../src/Stronger.mp3 repeat from 0", NULL, 0, NULL);
		}
		//�ж�ʤ��
		if (num1 == 3)
			goto WIN;
		//��ȡ����
		peekmessage(&key, EX_KEY);
		if (key.message == WM_KEYDOWN)
		{
			switch(key.vkcode)
			{
			case 'W':
				if (map[player.map_y - 1][player.map_x] != 0)
				{
					player.derection = 1;
					player.x = player.map_x * 36;
				}
				break;
			case 'A':
				if (map[player.map_y][player.map_x - 1] != 0)
				{
					player.derection = 2;
					player.y = player.map_y * 36;
				}
				break;
			case 'S':
				if (map[player.map_y + 1][player.map_x] != 0)
				{ 
					player.derection = 3;
					player.x = player.map_x * 36;
				}
				break;
			case 'D':
				if (map[player.map_y][player.map_x + 1] != 0)
				{ 
					player.derection = 0;
					player.y = player.map_y * 36;
				}
				break;
			}
		}
		switch(player.derection)
		{
		case 0:
			if (map[player.map_y][player.x / 36 + 1] != 0)
				player.x += player.speed;
			break;
		case 1:
			if (map[player.y / 36][player.map_x] != 0)
			    player.y -= player.speed;
			break;
		case 2:
			if (map[player.map_y][player.x / 36] != 0)
			    player.x -= player.speed;
			break;
		case 3:
			if (map[player.y / 36 + 1][player.map_x] != 0)
			    player.y += player.speed;
			break;
		}
		//ȷ����ķ���
		for (int i = 0; i < 3; i++)
		{
			switch (ghost[i].derection)
			{
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
			}
		}
		//��ʾ���
		if (player.close == 0)
		{
			putimage(player.x, player.y, &man[player.derection][1], SRCAND);
			putimage(player.x, player.y, &man[player.derection][0], SRCPAINT);
		}
		else if (player.close == 1)
		{
			putimage(player.x, player.y, &manClose[1], SRCAND);
			putimage(player.x, player.y, &manClose[0], SRCPAINT);
		}
		//��ʾ��
		if (ghost[0].live)
		{
			putimage(ghost[0].x, ghost[0].y, &mask, SRCAND);
			putimage(ghost[0].x, ghost[0].y, &enemy[0][ghost[0].derection], SRCPAINT);
		}
		if (ghost[1].live)
		{
			putimage(ghost[1].x, ghost[1].y, &mask, SRCAND);
			putimage(ghost[1].x, ghost[1].y, &enemy[1][ghost[1].derection], SRCPAINT);
		}
		if (ghost[2].live)
		{
			if (phase == 2)
			{
				shake = -shake;
				putimage(ghost[2].x, ghost[2].y + shake, &mask, SRCAND);
				putimage(ghost[2].x, ghost[2].y + shake, &scared, SRCPAINT);
			}
			else
			{
				putimage(ghost[2].x, ghost[2].y, &mask, SRCAND);
				putimage(ghost[2].x, ghost[2].y, &enemy3[ghost[2].form][ghost[2].derection], SRCPAINT);
			}
		}
		//�����������λ��
		player.map_x = (player.x + 17) / 36;
		player.map_y = (player.y + 17) / 36;
		ghost[0].map_x = (ghost[0].x + 17) / 36;
		ghost[0].map_y = (ghost[0].y + 17) / 36;
		ghost[1].map_x = (ghost[1].x + 17) / 36;
		ghost[1].map_y = (ghost[1].y + 17) / 36;
		ghost[2].map_x = (ghost[2].x + 17) / 36;
		ghost[2].map_y = (ghost[2].y + 17) / 36;
		//�ж�ʧ��
		for (int i = 0; i < 3; i++)
		{
			if (phase != 2 && ghost[i].map_x == player.map_x && ghost[i].map_y == player.map_y)
				goto LOSE;
		}
		//���Ѱ·
		if (ghost[0].x % 36 == 0 && ghost[0].y % 36 == 0)
		{
			if (phase == 2)
				ghost[0].derection = ghost1Run(ghost[0].map_x, ghost[0].map_y, player.map_x, player.map_y);
			else
				ghost[0].derection = ghost1(ghost[0].map_x, ghost[0].map_y, player.map_x, player.map_y, &g1_track_x, &g1_track_y);
		}
		if (ghost[1].x % 36 == 0 && ghost[1].y % 36 == 0)
		{
			if (phase == 2)
				ghost[1].derection = ghost2Run(ghost[1].map_x, ghost[1].map_y, &ghost2_goal);
			else
				ghost[1].derection = ghost2(ghost[1].map_x, ghost[1].map_y, player.map_x, player.map_y, g1_track_x, g1_track_y);
		}
		if (ghost[2].x % 36 == 0 && ghost[2].y % 36 == 0 && phase != 2)
			ghost[2].derection = ghost3(ghost[2].map_x, ghost[2].map_y, player.map_x, player.map_y, &ghost[2].speed, &ghost[2].form, &last_step_x, &last_step_y, ghost[2].derection);
		//��ⶹ�ӱ���
		if(map[player.map_y][player.map_x]==1)
		{
			map[player.map_y][player.map_x] = 2;
			num++;
			PlaySound(TEXT("../src/Eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		//�������
		for (int i = 0; i < 3; i++)
		{
			if (ghost[i].live && phase == 2 && ghost[i].map_x == player.map_x && ghost[i].map_y == player.map_y)
			{
				ghost[i].live = 0;
				num1++;
				mciSendString("close ../src/Ghost.mp3", NULL, 0, NULL);
				mciSendString("open ../src/Ghost.mp3", NULL, 0, NULL);
				mciSendString("play ../src/Ghost.mp3", NULL, 0, NULL);
			}
		}
		//FPS
		Sleep(30);
		FlushBatchDraw();
		cleardevice();
		auto end = system_clock::now();
		auto duration = duration_cast<milliseconds>(end - start);
		time += double(duration.count());
		if (time >= close_time)
		{
			player.close = !player.close;
			time = 0;
		}
	}
	EndBatchDraw();
	getchar();
LOSE:
	mciSendString("close ../src/Start.mp3", NULL, 0, NULL);
	mciSendString("close ../src/Death.mp3", NULL, 0, NULL);
	mciSendString("open ../src/Death.mp3", NULL, 0, NULL);
	mciSendString("play ../src/Death.mp3", NULL, 0, NULL);
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
	while (true)
	{
		peekmessage(&msg, EX_MOUSE);
		if (msg.message == WM_LBUTTONDOWN && msg.x > 292 && msg.x < 416 && msg.y > 750 && msg.y < 786)
		{
			cleardevice();
			goto TITLE;
		}
	}
WIN:
	mciSendString("close ../src/Stronger.mp3", NULL, 0, NULL);
	mciSendString("close ../src/Winner.mp3", NULL, 0, NULL);
	mciSendString("open ../src/Winner.mp3", NULL, 0, NULL);
	mciSendString("play ../src/Winner.mp3", NULL, 0, NULL);
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
	while (true)
	{
		peekmessage(&msg, EX_MOUSE);
		if (msg.message == WM_LBUTTONDOWN && msg.x > 292 && msg.x < 416 && msg.y > 750 && msg.y < 786)
		{
			cleardevice();
			goto TITLE;
		}
	}
	system("pause");
	return 0;
}