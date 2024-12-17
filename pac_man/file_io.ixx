module;

#include <ctime>

export module file_io;

import std;

using namespace std;

// 全局变量
const int RECORD_NUMBER = 100;  // 读取记录数的上限

export struct PacRecord
{
    double m_time_s{ 0 };
    unsigned long long score{ 0 };
};

export
{
	optional<int> fileRead(PacRecord record); // 文件读入
    optional<int> fileWrite(const PacRecord& record);
}

optional<int> fileRead(PacRecord record)
{
    cout << endl;
    cout << "正在读取文件。\n";

	ifstream fin("data.txt", ios_base::in | ios_base::binary);



    fin.close();
    cout << "读取文件成功！\n";
    
    return true;
}


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
    }

    // 打印信息
    {
        // 第一次运行
		static int lineNumber = 1;
	    if (lineNumber == 1) {
	        fout << '\n' << std::put_time(&nowTm, "%Y-%m-%d %H:%M:%S") << '\n';
	    }

	    auto [time_cost, player_score] = record;

	    fout << format("{: >4} : ", lineNumber) << std::put_time(&nowTm, "%Y-%m-%d %H:%M:%S")
    		<< format(" : Time cost: {}, Score: {}", time_cost, player_score);
        // 更新行数
		++lineNumber;
    }

    fout.close();

    std::cout << "FileIO over!\n";

    /* 旧实现
     * ios_base::fmtflags old = fout.setf(ios::left, ios::adjustfield);
     * fout.setf(old, ios::adjustfield);
     */

    return true;
}