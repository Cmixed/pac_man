import std;
import data;
import music;

#include <ege/graphics.h>

namespace g
{
	auto begin = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    
    LOGFONTW f{};

    const char * g_Mid_File_Path = R"(../src/music/music.mid)";
}

using namespace std;

static bool init_graph();
static bool start_game();
static optional<int> game_core();
static bool end_game();

int main()
{
    std::thread playMusic_t(play_music,g::g_Mid_File_Path);

	init_graph();

    end_game();

    try {
        if (playMusic_t.joinable()) {
            playMusic_t.join();
        } else {
            throw std::runtime_error("Music thread cannot join!!!");
        }
    } catch(const std::runtime_error &e) {
        std::cerr << e.what();
    }

    return 0;
}

bool init_graph()
{
	initgraph(800,800,INIT_RENDERMANUAL);
    setbkcolor(EGERGB(0x00,0x00,0x00));
    setfillcolor(EGECOLORA(WHITE,0xff));
    ege_enable_aa(true);

	return true;
}

bool start_game()
{
	cleardevice();


    for ( ; is_run(); delay_fps(60)) {
    
	    if (kbmsg()) {
	       
	        key_msg k{0};
	        k = getkey();
	        
	        if (k.key != key_esc) {
	            if (k.key == key_space) {
                    ;
	            }
	        } else {
	            closegraph();
	            return true;
	        }
	    }
	}

	return true;	
}

optional<int> game_core()
{
	return 1;
}

bool end_game()
{
	cleardevice();


    for ( ; is_run(); delay_fps(60)) {
    
	    if (kbmsg()) {
	       
	        key_msg k{0};
	        k = getkey();
	        
	        if (k.key != key_esc) {
	            if (k.key == key_space) {
                    ;
	            }
	        } else {
	            closegraph();
	            return true;
	        }
	    }
	}

	return true;
}