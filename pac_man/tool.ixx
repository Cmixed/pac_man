module;

export module tool;

import std;
#include <easyx/easyx.h>

export
{
	std::string timeDifferenceInSeconds(const std::chrono::time_point<std::chrono::system_clock>& t1,
    const std::chrono::time_point<std::chrono::system_clock>& t2);

    template<std::integral T>
    std::string cast2String(T x);

    unsigned int targetFPS(const unsigned int target_fps);
}

std::string timeDifferenceInSeconds(const std::chrono::time_point<std::chrono::system_clock>& t1,
    const std::chrono::time_point<std::chrono::system_clock>& t2) {
        // 计算两个时间点的差值
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
        
        // 将差值转换为字符串
        std::stringstream ss;
		
        ss << duration << " seconds";
        return ss.str();
    }

template<std::integral T>
std::string cast2String(T x)
{
    std::stringstream ss;
    ss << x;

    return ss.str();
}


unsigned int targetFPS(const unsigned int target_fps)
{
    static unsigned long preFpsTime{ 0 };
    static unsigned int preFps{ 0 };

    if (preFpsTime != 0) {
	    Sleep(preFpsTime);
    } else {
		preFpsTime = (1000 / target_fps);
        Sleep(preFpsTime);
    }

    preFps = ((++preFps) /= target_fps);

    return preFps;
}