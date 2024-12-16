export module file_io;

import std;

using namespace std;

// 全局变量
constexpr  int RECORD_NUMBER = 100;  // 读取记录数的上限

export struct PacRecord
{
    double m_time_s{ 0 };
    unsigned long long score{ 0 };
};

export
{
	optional<int> fileRead(PacRecord record, int month); // 文件读入
	optional<int> fileWrite(int option, int month);
}

optional<int> fileRead(PacRecord record, int month)
{
    cout << endl;
    cout << "正在读取文件。\n";

	ifstream fin("data.txt", ios_base::in | ios_base::binary);



    fin.close();
    cout << "读取文件成功！\n";
    
    return true;
}


optional<int> fileWrite(PacRecord record, int month)
{
    static int lineNumber = 1;

    ofstream fout("data.txt", ios_base::out | ios_base::app);

    // 获取当前时间点
    auto now = std::chrono::system_clock::now();
    // 将时间点转换为time_t以便进一步转换为本地时间
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    // 将time_t转换为tm结构以便打印
    std::tm* now_tm = std::localtime(&now_time);

    if (lineNumber == 1)    // 本次运行的第一次
    {
        fout << '\n' << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S") << '\n';
    }

    auto [time_cost, player_score] = record;

    // 打印对局信息
    fout << format("{: >4} : {}, Time cost: {}, Score: {}", 
        lineNumber, std::put_time(now_tm, "%Y-%m-%d %H:%M:%S"), time_cost, player_score);
    fout.close();

    ++lineNumber;

    /* 旧实现
     * ios_base::fmtflags old = fout.setf(ios::left, ios::adjustfield);
     * fout.setf(old, ios::adjustfield);
     */

    return true;
}