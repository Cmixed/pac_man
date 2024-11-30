module;

#include <Windows.h>
#include <mmsystem.h>
#include <cstdio>

#pragma comment (lib,"winmm.lib")

export module music;

import std;

export bool play_music(const char* file_path);
export bool play_music2(const char* filePath);

export void play_music_test()
{
	PlaySound(TEXT("../src/music/eason.wav"), nullptr, SND_FILENAME | SND_ASYNC);
}

/**
 * @brief 播放 wav 格式文件
 * @param file_path 
 * @return 
 */
bool play_music(const char* file_path)
{
    PlaySound((LPCWSTR)file_path,nullptr, SND_FILENAME | SND_ASYNC);

    return true;
}

bool play_music2(const char* filePath)
{

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