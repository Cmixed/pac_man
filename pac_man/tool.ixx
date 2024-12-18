module;

#include <chrono>

export module tool;

import std;
#include <easyx/easyx.h>

export
{
	std::string timeDifferenceInSeconds(const std::chrono::time_point<std::chrono::system_clock>& begin,
    const std::chrono::time_point<std::chrono::system_clock>& end);

    template<std::integral T>
    std::string cast2String(T const x);

    unsigned int targetFPS(const unsigned int target_fps);
}

/**
 * @brief 计算两个时间点的差值，并转化为字符串
 * @param t1 
 * @param t2 
 * @return 
 */
std::string timeDifferenceInSeconds(const std::chrono::time_point<std::chrono::system_clock>& begin,
                                    const std::chrono::time_point<std::chrono::system_clock>& end) {
        // 计算两个时间点的差值
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

        // 将差值转换为字符串
        std::stringstream ss;
		
        ss << duration; // seconds
        return ss.str();
    }

/**
 * @brief 将整数转换为 string 类型
 * @tparam T 
 * @param x 
 * @return 
 */
template<std::integral T>
std::string cast2String(T const x)
{
    std::stringstream ss;
    ss << x;

    return ss.str();
}

/**
 * @brief 延迟每帧时间
 * @param target_fps 
 * @return now fps order
 */
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

    return preFps;
}