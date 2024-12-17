/**
 *  此模块包含了与文件输入输出有关的函数
 */

module;

#include <ctime>

export module file_io;

import std;

using namespace std;

// 全局变量
const int RECORD_NUMBER = 100;  // 读取记录数的上限

// 吃豆人记录格式
export struct PacRecord
{
    double m_time_s{ 0 };
    unsigned long long score{ 0 };
};

export
{
	optional<int> fileRead(PacRecord record); // 文件读入
    optional<int> fileWrite(const PacRecord& record); // 文件写入
}

optional<int> fileRead(PacRecord record)
{
	ifstream fin("settings.ini", ios_base::in | ios_base::binary);


    fin.close();

    cout << "ReadFile Successfully！\n";
    
    return true;
}

/**
 * @brief 将记录写入 data.txt
 * @param record 
 * @return normal 1 error 0
 */
optional<int> fileWrite(const PacRecord& record)
{
    ofstream fout("data.txt", ios_base::out | ios_base::app);

    const auto now = std::chrono::system_clock::now();
    const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm nowTm;

    try {
	    if (const auto err = localtime_s(&nowTm, &nowTime) != 0) {
            throw runtime_error("Time convert error!");
	    }
    }
    catch (exception &e) {
        std::cerr << e.what();
        return optional<int>{0};
    }

    // 打印信息
    {
        // 第一次运行
		static int lineNumber = 1;
	    if (lineNumber == 1) {
	        fout << "Game Start Time(Local): " << std::put_time(&nowTm, "%Y-%m-%d %H:%M:%S") << '\n';
	    }

	    auto [time_cost, player_score] = record;

	    fout << format("{: >4} : ", lineNumber) << std::put_time(&nowTm, "%Y-%m-%d %H:%M:%S")
    		<< format(" : Time cost: {}, Score: {}\n", time_cost, player_score);

        // 更新行数
		++lineNumber;
    }

    fout.close();

    std::cout << "FileIO over!\n";

    /* 旧实现
     * ios_base::fmtflags old = fout.setf(ios::left, ios::adjustfield);
     * fout.setf(old, ios::adjustfield);
     */

    return optional<int>{1};
}