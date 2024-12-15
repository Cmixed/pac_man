module;

export module tool;

import std;

export
{
	std::string timeDifferenceInSeconds(const std::chrono::time_point<std::chrono::system_clock>& t1,
    const std::chrono::time_point<std::chrono::system_clock>& t2);

    template<std::integral T>
    std::string cast2String(T x);
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