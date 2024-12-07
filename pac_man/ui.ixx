export module ui;

export constexpr struct Ui
{
	struct Graph
	{
		int width{ 720 };
		int height{ 840 };
	} graph;

	struct Title1
	{
		int x{ 66 };
		int y{ 66 };
	} title1;

	struct Title2
	{
		int x{ 116 };
		int y{ 600 };
	} title2;

	struct Start
	{
		int x{ 290 };
		int y{ 330 };
	} start;

	struct Quit
	{
		int x{ 290 };
		int y{ 400 };
	} quit;

	struct TextStyle
	{
		int height{ 40 };
		int width{ 0 };
	} start_menu_text_style;

	struct ButtonStart
	{
		int x_l{ 288 }, x_g{416};
		int y_l{ 330 }, y_g{ 364 };
	} buttonStart;

	struct ButtonQuit
	{
		int x_l{ 288 }, x_g{ 416 };
		int y_l{ 408 }, y_g{ 442 };
	} buttonQuit;
} ui;