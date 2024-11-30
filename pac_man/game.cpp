import std;

import data;

#include <ege/graphics.h>

namespace g
{
	
}

using namespace std;

static bool init_graph();

int main()
{
	init_graph();

	return 0;
}

static bool init_graph()
{
	initgraph(800,800,INIT_RENDERMANUAL);
    setbkcolor(EGERGB(0x00,0x00,0x00));
    setfillcolor(EGECOLORA(WHITE,0xff));
    ege_enable_aa(true);

	return true;
}