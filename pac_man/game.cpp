import std;

import data;

#include <ege/graphics.h>

namespace g
{
	
}


int main()
{
	initgraph(640, 480);	// 初始化一个640×480的绘图窗口
	circle(320, 240, 100);	// 画圆，圆心为(320, 240)，半径为100
	getch();	// 暂停等待按键
	closegraph();	// 关闭绘图窗口
	return 0;
}
