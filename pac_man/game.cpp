import std;
import data;

#include <ege/graphics.h>
#include <Windows.h>
#include <cstdio>

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

static bool play_music(const char* filePath);

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

bool play_music(const char* filePath) {

    HMODULE module = LoadLibraryA("winmm.dll");

    typedef MCIERROR(WINAPI* MciSendStringT)(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback);
    
    auto func_mciSendStringA = (MciSendStringT)GetProcAddress(module, "mciSendStringA");
    if (func_mciSendStringA == nullptr) return false;
    
    char buff[255]{ 0 }, command[100]{ 0 };
    sprintf_s(command, 100, "open %s alias playsound_134", filePath);
    func_mciSendStringA(command, buff, 254, nullptr);
    sprintf_s(command, 100, "set playsound_134 time format milliseconds");
    func_mciSendStringA(command, buff, 254, nullptr);
    sprintf_s(command, 100, "status playsound_134 length");
    func_mciSendStringA(command, buff, 254, nullptr);
    sprintf_s(command, 100, "play playsound_134 from 0 to %s", buff);
    func_mciSendStringA(command, buff, 254, nullptr);
    return true;
}