#include <iostream>
#include <string>
#include <Windows.h>
#include <cstdio>
#include <conio.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <chrono>
using namespace std;

bool eng = 0; //v1.2.0 再更新  
bool white = 1, green = 0, fast = 0; //白色还是黑色  护眼  加快鸣谢 
bool _return = 0; //直接去世  
HWND hwnd = FindWindow(NULL, "计时器 v1.1.0");
int sz = 1; //设置选项 
SYSTEMTIME wtime; //时间  
struct time
{
    int year;
    int month;
    string week;
    int day;
    int hour;
    int minute;
    int second;
} ti;

void setcolor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 修复的计时器类
class FixedTimer {
private:
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point pause_start_time;
    double total_paused_time = 0; // 累计暂停时间
    bool is_paused = false;

public:
    void start() {
        start_time = std::chrono::steady_clock::now();
        is_paused = false;
        total_paused_time = 0;
    }

    void pause() {
        if (!is_paused) {
            pause_start_time = std::chrono::steady_clock::now();
            is_paused = true;
        }
    }

    void resume() {
        if (is_paused) {
            auto resume_time = std::chrono::steady_clock::now();
            total_paused_time += std::chrono::duration<double>(resume_time - pause_start_time).count();
            is_paused = false;
        }
    }

    double getElapsedSeconds() const {
        if (is_paused) {
            return std::chrono::duration<double>(pause_start_time - start_time).count() - total_paused_time;
        } else {
            auto current_time = std::chrono::steady_clock::now();
            return std::chrono::duration<double>(current_time - start_time).count() - total_paused_time;
        }
    }

    bool paused() const {
        return is_paused;
    }
};

void gettime()
{
    GetLocalTime(&wtime);
    ti.year = wtime.wYear;
    ti.month = wtime.wMonth;
    int week = wtime.wDayOfWeek;
    string we;
    switch (week)
    {
    case 1:
    {
        we = "一";
        break;
    }
    case 2:
    {
        we = "二";
        break;
    }
    case 3:
    {
        we = "三";
        break;
    }
    case 4:
    {
        we = "四";
        break;
    }
    case 5:
    {
        we = "五";
        break;
    }
    case 6:
    {
        we = "六";
        break;
    }
    case 0:
    {
        we = "日";
        break;
    }
    }
    ti.week = we;
    ti.day = wtime.wDay;
    ti.hour = wtime.wHour;
    ti.minute = wtime.wMinute;
    ti.second = wtime.wSecond;
}

void color(short x)
{

    if (x == 15)
        if (white == 1 && green == 0)
            system("color F0");
        else if (green == 1)
            system("color 27");
        else
            system("color 0F");
    else if (x == 2)
        if (white == 1 && green == 0)
            system("color FA");
        else if (green == 1)
            system("color 2A");
        else
            system("color 0A");
    else if (x == 12)
        if (white == 1 && green == 0)
            system("color FC");
        else if (green == 1)
            system("color 2C");
        else
            system("color 0C");
    else if (x == 11)
        if (white == 1 && green == 0)
            system("color FB");
        else if (green == 1)
            system("color 2B");
        else
            system("color 0B");
}

void watch()
{
	WORD c1 = 16 | 128 , c2 = 64 | 128 , c3 = 32 | 128 ;
    int s_w = 7; //秒针长度 
    int m_w = 5; //分针长度 
    int h_w = 3; //时针长度 
    int xs[17][17] = { 0 };
    double s = 0, f = 0, m = 0;
    char sr = ' ';
    while (sr != 'r')
    {
        system("cls");
        if (_kbhit())
		{
			sr = _getch() ;
			if(sr == 'r')
				break ;
		}
        time_t t = time(0);
        tm* temp = localtime(&t);
        s = temp->tm_hour;
        f = temp->tm_min;
        m = temp->tm_sec;
        f += m / 60;
        s += f / 60;
        m *= 6;
        f *= 6;
        s *= 30;
        xs[9][9] = true;
        int m1, m2;
        m1 = int(-cos((360 - m) / 180.0 * 3.14) * s_w + 9);
        m2 = int(-sin((360 - m) / 180.0 * 3.14) * s_w + 9);
        xs[m1][m2] = true;
        int f1, f2;
        f1 = int(-cos((360 - f) / 180.0 * 3.14) * m_w + 9);
        f2 = int(-sin((360 - f) / 180.0 * 3.14) * m_w + 9);
        xs[f1][f2] = true;
        int s1, s2;
        s1 = int(-cos((360 - s) / 180.0 * 3.14) * h_w + 9);
        s2 = int(-sin((360 - s) / 180.0 * 3.14) * h_w + 9);
        xs[s1][s2] = true;
        for (double i = 0; i < 1; i += 0.14)
        {
            xs[int(9 * i + m1 * (1 - i))][int(9 * i + m2 * (1 - i))] = 1 ;
            xs[int(9 * i + f1 * (1 - i))][int(9 * i + f2 * (1 - i))] = 2 ;
            xs[int(9 * i + s1 * (1 - i))][int(9 * i + s2 * (1 - i))] = 3 ;
        }
        for (int i = 0; i < 17; ++i)
        {
            for (int j = 0; j < 17; ++j)
            {
                if (xs[i][j] == 1)
                    //setcolor(114);
                    setcolor(c1);   // 青色 (蓝色+绿色 = 1+2)
                else if(xs[i][j] == 2)
                	setcolor(c2) ;
                else if(xs[i][j] == 3)
                	setcolor(c3) ;
                else
                    setcolor(0);
                if (j == 9 && i == 0)
                {
                    setcolor(8);
                    printf("12\n");
                    printf("                  |");
                }
                else if (j == 9 && i == 16)
                {
                    setcolor(8);
                    printf(" |\n");
                    printf("                   6\n");
                }
                else if (j == 0 && i == 9)
                {
                    setcolor(8);
                    printf("9__");
                }
                else if (j == 16 && i == 9)
                {
                    setcolor(8);
                    printf("__3");
                }
                else
                    printf("  ");
                xs[i][j] = false;
            }
            printf("\n");
        }

        setcolor(8);
        cout << "\n\n        按r键返回主界面";
        gettime();
        printf("\n       现在时间 %02d:%02d:%02d", ti.hour, ti.minute, (ti.second + 59) % 60);
        setcolor(c1) ;
        cout << "\n            秒针            \n" ;
        setcolor(c2) ;
        cout << "            分针            \n" ;
        setcolor(c3) ;
        cout << "            时针            " ;
        m /= 6;
        f /= 6;
        s /= 30;
        Sleep(800);
        setcolor(0) ;
    }
}
void watch_ai()
{
    int hour = 0, minute = 0, second = 0;
    char sr = ' ';
    
    while (sr != 'r' && sr != 'R')
    {
        system("cls");
        
        // 获取当前时间
        time_t t = time(0);
        tm* temp = localtime(&t);
        hour = temp->tm_hour;
        minute = temp->tm_min;
        second = temp->tm_sec;
        
        // 设置控制台窗口大小
        system("mode con cols=50 lines=25");
        
        // 时钟标题
        setcolor(11);
        cout << "        ╔══════════════════════════╗\n";
        cout << "        ║         模拟时钟         ║\n";
        cout << "        ╠══════════════════════════╣\n";
        printf("        ║         %02d:%02d:%02d         ║\n", hour, minute, second);
        cout << "        ╚══════════════════════════╝\n\n";
        
        // 创建ASCII艺术表盘
        setcolor(15);
        cout << "              .--.  .--.\n";
        cout << "             |    ||    |\n";
        cout << "           .-'-.  ||  .-'-.\n";
        cout << "         .'     '.||.'     '.\n";
        cout << "        |   12    ||    1    |\n";
        cout << "        |         ||         |\n";
        cout << "        |   11    ||    2    |\n";
        cout << "         '.     .'||'.     .'\n";
        cout << "           '-.-'  ||  '-.-'\n";
        cout << "             |    ||    |\n";
        cout << "           .-'-.  ||  .-'-.\n";
        cout << "         .'     '.||.'     '.\n";
        cout << "        |   10    ||    3    |\n";
        cout << "        |         ||         |\n";
        cout << "        |    9    ||    4    |\n";
        cout << "         '.     .'||'.     .'\n";
        cout << "           '-.-'  ||  '-.-'\n";
        cout << "             |    ||    |\n";
        cout << "           .-'-.  ||  .-'-.\n";
        cout << "         .'     '.||.'     '.\n";
        cout << "        |    8    ||    5    |\n";
        cout << "        |         ||         |\n";
        cout << "        |    7    ||    6    |\n";
        cout << "         '.     .'||'.     .'\n";
        cout << "           '-.-'  ||  '-.-'\n";
        cout << "             '--'  '--'\n\n";
        
        // 显示指针状态
        cout << "   指针状态:\n";
        setcolor(12); cout << "   ● 秒针 ";
        setcolor(10); cout << "● 分针 ";
        setcolor(11); cout << "● 时针\n";
        
        setcolor(8);
        cout << "\n    按 R 键返回主界面\n";
        
        // 非阻塞键盘检测
        if (_kbhit())
        {
            sr = _getch();
        }
        
        Sleep(500);
    }
    
    // 恢复原来的控制台大小
    system("mode con cols=31 lines=20");
}
void watch_ai2()
{
    int hour = 0, minute = 0, second = 0;
    char sr = ' ';
    
    while (sr != 'r' && sr != 'R')
    {
        system("cls");
        
        // 获取当前时间
        time_t t = time(0);
        tm* temp = localtime(&t);
        hour = temp->tm_hour;
        minute = temp->tm_min;
        second = temp->tm_sec;
        
        // 简单文本时钟界面
        setcolor(11);
        cout << "    ╔══════════════╗\n";
        cout << "    ║   模拟时钟   ║\n";
        cout << "    ╠══════════════╣\n";
        printf("    ║   %02d:%02d:%02d    ║\n", hour, minute, second);
        cout << "    ╚══════════════╝\n\n";
        
        // 简化的表盘
        setcolor(15);
        cout << "        12\n";
        cout << "    11      1\n";
        cout << "  10          2\n";
        cout << " 9       ○      3\n"; 
        cout << "  8            4\n";
        cout << "    7       5\n";
        cout << "        6\n";
        
        setcolor(8);
        cout << "\n    按 R 键返回主界面";
        
        if (_kbhit())
        {
            sr = _getch();
        }
        
        Sleep(500);
    }
}
void qp()
{
    system("cls");
}

void sc(string x[], int len)
{
    color(11);
    for (int i = 0; i < len; i++)
    {
        cout << x[i];
        if (!fast)
            Sleep(140);
    }
    cout << '\n';
}

void mx()
{
    string a[] = { "8", "1", "3", "3", "工", "作", "室", "&", "l", "e", "a", "v", "e", "s", "工", "作", "室", "出", "品" };
    string b[] = { "作", "者", ":", "8", "1", "3", "3", "w", "t", "5", ",", "吹", "叶", "清", "风" };
    string c[] = { "感", "谢", "D", "E", "V", " ", "C", "+", "+", "5", ".", "1", "1", "提", "供", "的", "编", "辑", "环", "境" };
    string d[] = { "这", "已", "经", "是", "第", "1", "3", "次", "更", "新", "了", ",", "请", "多", "多", "支", "持", "!" };
    sc(a, 19);
    sc(b, 15);
    sc(c, 20);
    sc(d, 18);
}

void print(string middle, bool flag, bool flag2) //middle是输出的，flag控制输出箭头 flag2控制输出竖线 
{
    if (flag2)
        cout << "|" << " ";
    if (flag)
        cout << "=>";
    else
        cout << "  ";
    cout << middle;
    if (flag)
        cout << "<=";
    else
        cout << "  ";
    if (flag2)
        cout << " " << "|" << '\n';
    else
        cout << "\n";
}

// 格式化时间显示函数（只显示到秒）
void formatTime(double totalSeconds, bool isCountdown = false) {
    int total = static_cast<int>(totalSeconds);
    int hours = total / 3600;
    int minutes = (total % 3600) / 60;
    int seconds = total % 60;
    
    if (hours > 0) {
        printf("%d小时%02d分钟%02d秒", hours, minutes, seconds);
    } else if (minutes > 0) {
        printf("%02d分钟%02d秒", minutes, seconds);
    } else {
        printf("%02d秒", seconds);
    }
}

int main()
{
    system("color F0");
    system("mode con cols=31 lines=20");
    SetConsoleTitle("计时器 v1.1.03");
    if(!system("upd check 1.1.03"))
    {
        cout<<"检测到了更新！！！"<<endl;
        if(!system("upd log"))
        {
        	cout<<"正在打开日志……"<<endl;
            system("start %temp%\\upd\\upd_log.txt");
        }
        system("pause");
		cout<<"开始下载更新……"<<endl; 
        if(!system("upd run"))
        {
            exit(0);
        }
        cout<<"下载更新失败，请手动下载或重试！！！"<<endl;
        system("pause"); 
    }
    int n = 1;
    while (1)
    {
        qp();
        color(15);
        while (1)
        {
            gettime();
            if (_return)
                break;
            qp();
            cout << "≡≡≡≡≡≡≡≡≡≡≡≡≡≡\n";
            cout << "|           计时器          |\n";
            cout << "|       版本:Windows  7     |\n";
            cout << "| 上一次更新时间:2025/11/26 |\n";
            printf("|现在是%04d年%02d月%02d号 ", ti.year, ti.month, ti.day);
            cout << "星期" << ti.week << "|" << '\n';
            printf("|          %02d时%02d分         |\n", ti.hour, ti.minute);
            //                                      |标线  
            cout << "| ≡≡≡≡≡≡≡≡≡≡≡≡≡|\n";
            print("       1.正计时      ", n == 1, 1);
            print("       2.倒计时      ", n == 2, 1);
            print("      3.退出程序     ", n == 3, 1);
            print("        4.鸣谢       ", n == 4, 1);
            print("     5.颜色选取器    ", n == 5, 1);
            print("      6.反馈错误     ", n == 6, 1);
            print("        7.设置       ", n == 7, 1);
            print("   8.时钟模式(黑色)  ", n == 8, 1);
            cout << "|         按w键上移箭头     |\n";
            cout << "|         按s键下移箭头     |\n";
            cout << "|          按r键刷新        |\n";
            cout << "|          按h键还原        |\n";
            cout << "|  按c键选择箭头所在的条目  |\n";
            cout << "| ≡≡≡≡≡≡≡≡≡≡≡≡≡|\n";
            char ch = getch();
            if (ch == 's' || ch == 'S')//下移 
                n = (n == 8 ? 1 : n + 1);
            else if (ch == 'w' || ch == 'W')//上移 
                n = (n == 1 ? 8 : n - 1);
            else if (ch == 'c')//确认 
            {
                break;
            }
            else if (ch == 'h') //还原 
            {
                n = 1;
            }
            else if (ch == '8')
            {
                qp();
                cout << "彩蛋1\n";
                cout << "管理员模式的密码第一位是8\n";
                cout << "按任意键继续";
                _getch();
            }
            else if (ch == 'r')
                continue;
        }
        if (_return)
            break;
        if (n > 8 || n < 1)
        {
            bool flag = 1;
            while (flag)
            {
                qp();
                cout << "输入错误!\n";
                cout << "n = " << n << '\n';
                cout << "按任意键返回";
                Sleep(300);
                for (int i = 0; i < 3; i++)
                {
                    Sleep(300);
                    cout << '.';
                    if (_kbhit())
                    {
                        char ch = _getch();
                        flag = 0;
                        break;
                    }
                }
            }
            continue;
        }
        if (n == 8)
        {
            int w = white;
            int g = green;
            white = 0;
            green = 0;
            color(15);
            watch();
            white = w;
            green = g;
        }
        else if (n == 3)
            break;
        else if (n == 4)
        {
            system("mode con cols=33 lines=17");
            qp();
            mx();
            cout << "按任意键退出鸣谢名单";
            char ch = _getch();
            system("mode con cols=31 lines=20");
            if (ch == '1')
            {
                qp();
                cout << "彩蛋2\n";
                cout << "管理员密码第二位为1\n";
                cout << "按任意键继续";
                ch = getch();
                if (ch == 'w')
                {
                    qp();
                    cout << "彩蛋2\n";
                    cout << "管理员密码第二位为1\n";
                    cout << "按任意键继续";
                    ch = getch();
                    if (ch == 't')
                    {
                        qp();
                        cout << "彩蛋2\n";
                        cout << "管理员密码第二位为1\n";
                        cout << "按任意键继续";
                        ch = getch();
                        if (ch == 'h')
                        {
                            qp();
                            cout << "彩蛋3\n";
                            cout << "按任意键告诉你彩蛋内容\n";
                            getch();
                            cout << "\n你的东西都保存好了吗？？？";
                            Sleep(50);
                            qp();
                            cout << 3;
                            Sleep(500);
                            qp();
                            cout << 2;
                            Sleep(500);
                            qp();
                            cout << 1;
                            Sleep(500);
                            qp();
                            cout << "掉！";
                            Sleep(500);
                            qp();
                            cout << "你掉进陷阱了\n";
                            cout << "是200个百度哦!";
                            cout << "[企鹅叫]";
                            Sleep(300);
                            for (int i = 0; i < 200; i++)
                            {
                                system("start https://www.baidu.com");
                            }
                            _return = 1;
                            break;
                        }
                    }
                }
            }
        }
        else if (n == 1)
        {
            qp();
            string name;
            cout << "请输入计时名称(必填):";
            cin >> name;
            
            FixedTimer timer;
            timer.start();
            bool running = true;
            
            while (running)
            {
                // 运行状态
                while (!timer.paused() && running)
                {
                    qp();
                    color(2);
                    cout << "计时: " << name;
                    cout << "状态 : ";
                    cout << "计时中\n\n";
                    cout << "按'b'键暂停\n按'r'键结束\n\n";
                    cout << "已过去了";
                    formatTime(timer.getElapsedSeconds());
                    
                    // 检查按键输入
                    if (_kbhit()) {
                        char ch = _getch();
                        if (ch == 'b') {
                            timer.pause();
                            break;
                        } else if (ch == 'r') {
                            running = false;
                        }
                    }
                    Sleep(100); // 控制刷新频率
                }
                
                // 暂停状态
                while (timer.paused() && running)
                {
                    qp();
                    color(12);
                    cout << "计时: " << name;
                    cout << "状态 : ";
                    cout << "已暂停\n\n";
                    cout << "按'c'键继续\n按'r'键结束\n\n";
                    cout << "已过去了";
                    formatTime(timer.getElapsedSeconds());
                    
                    char ch = _getch();
                    if (ch == 'c') {
                        timer.resume();
                        break;
                    } else if (ch == 'r') {
                        running = false;
                    }
                }
            }
        }
        else if (n == 2)
        {
            qp();
            string name;
            cout << "请输入计时名称(必填):";
            cin >> name;
            qp();
            cout << "小时数(小于65536):";
            short h, m, s;
            cin >> h;
            qp();
            cout << "分钟数:";
            cin >> m;
            qp();
            cout << "秒数:";
            cin >> s;
            if (h > 65535 || m > 59 || s > 59 || h < 0 || m < 0 || s < 0)
            {
                cout << "输入不合法!\n";
                cout << "按任意键继续";
                _getch();
                continue;
            }
            
            double totalSeconds = (h * 60.0 + m) * 60.0 + s;
            FixedTimer timer;
            timer.start();
            bool running = true;
            bool completed = false;
            
            while (running && !completed)
            {
                // 运行状态
                while (!timer.paused() && running && !completed)
                {
                    double elapsed = timer.getElapsedSeconds();
                    double remaining = totalSeconds - elapsed;
                    
                    if (remaining <= 0) {
                        completed = true;
                        break;
                    }
                    
                    qp();
                    color(2);
                    cout << "计时: " << name;
                    cout << "状态 : ";
                    cout << "计时中\n\n";
                    cout << "按'b'键暂停\n按'r'键结束\n\n";
                    cout << "还剩";
                    formatTime(remaining, true);
                    
                    // 检查按键输入
                    if (_kbhit()) {
                        char ch = _getch();
                        if (ch == 'b') {
                            timer.pause();
                            break;
                        } else if (ch == 'r') {
                            running = false;
                        }
                    }
                    Sleep(100); // 控制刷新频率
                }
                
                // 暂停状态
                while (timer.paused() && running && !completed)
                {
                    double elapsed = timer.getElapsedSeconds();
                    double remaining = totalSeconds - elapsed;
                    
                    qp();
                    color(12);
                    cout << "计时: " << name;
                    cout << "状态 : ";
                    cout << "已暂停\n\n";
                    cout << "按'c'键继续\n按'r'键结束\n\n";
                    cout << "还剩";
                    formatTime(remaining, true);
                    
                    char ch = _getch();
                    if (ch == 'c') {
                        timer.resume();
                        break;
                    } else if (ch == 'r') {
                        running = false;
                    }
                }
            }
            
            // 计时完成
            if (completed)
            {
                bool flag = true;
                while (flag)
                {
                    qp();
                    cout << "计时: " << name << "状态 : ";
                    color(11);
                    cout << "计时完成\n\n";
                    cout << "按任意键继续";
                    if (_kbhit())
                    {
                        _getch();
                        flag = false;
                    }
                    Sleep(100);
                }
            }
        }
        else if (n == 5)
        {
            qp();
            cout << "请输入密码:";
            string x2;
            cin >> x2;
            if (x2 == "8133t5&wth123")
            {
                cout << "欢迎,管理员";
                qp();
                if (white == 1 && green == 0)
                {
                    system("color F1");
                    cout << "F1 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 21");
                    cout << "21 示例文本\n";
                }
                else
                {
                    system("color 01");
                    cout << "01 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color F2");
                    cout << "F2 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 23");
                    cout << "23 示例文本\n";
                }
                else
                {
                    system("color 02");
                    cout << "02 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color F3");
                    cout << "F3 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 24");
                    cout << "24 示例文本\n";
                }
                else
                {
                    system("color 03");
                    cout << "03 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color F4");
                    cout << "F4 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 25");
                    cout << "25 示例文本\n";
                }
                else
                {
                    system("color 04");
                    cout << "04 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color F5");
                    cout << "F5 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 26");
                    cout << "26 示例文本\n";
                }
                else
                {
                    system("color 05");
                    cout << "05 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color F6");
                    cout << "F6 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 27");
                    cout << "27 示例文本\n";
                }
                else
                {
                    system("color 06");
                    cout << "06 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color F7");
                    cout << "F7 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 28");
                    cout << "28 示例文本\n";
                }
                else
                {
                    system("color 07");
                    cout << "07 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color F8");
                    cout << "F8 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 29");
                    cout << "29 示例文本\n";
                }
                else
                {
                    system("color 08");
                    cout << "08 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color F9");
                    cout << "F9 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 2A");
                    cout << "2A 示例文本\n";
                }
                else
                {
                    system("color 09");
                    cout << "09 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color FA");
                    cout << "FA 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 2B");
                    cout << "2B 示例文本\n";
                }
                else
                {
                    system("color 0A");
                    cout << "0A 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color FB");
                    cout << "FB 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 2C");
                    cout << "2C 示例文本\n";
                }
                else
                {
                    system("color 0B");
                    cout << "0B 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color FC");
                    cout << "FC 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 2D");
                    cout << "2D 示例文本\n";
                }
                else
                {
                    system("color 0C");
                    cout << "0C 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color FD");
                    cout << "FD 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 2E");
                    cout << "2E 示例文本\n";
                }
                else
                {
                    system("color 0D");
                    cout << "0D 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                if (white == 1 && green == 0)
                {
                    system("color FE");
                    cout << "FE 示例文本\n";
                }
                else if (green == 1)
                {
                    system("color 2F");
                    cout << "2F 示例文本\n";
                }
                else
                {
                    system("color 0E");
                    cout << "0E 示例文本\n";
                }
                cout << "按任意键继续";
                _getch();
                qp();
                color(15);
                cout << "再见，管理员！";
                cout << "按任意键继续";
                _getch();
            }
        }
        else if (n == 6)
        {
            system("mode con cols=51 lines=17");
            qp();
            cout << "请私信8133wt5(洛谷)或fjxmwth(洛谷)以反馈错误(会回)\n\n";
            cout << "感谢你的反馈，我们会在下一个版本改善问题\n";
            cout << "需要帮您打开洛谷吗?(按'1打开)\n";
            cout << "按任意键返回(除了1)\n";
            char xy;
            xy = _getch();
            if (xy == '1')
            {
                system("start https://www.luogu.com.cn/chat");
            }
            system("mode con cols=30 lines=17");
        }
        else if (n == 7)
        {
            while (1)
            {
                qp();
                cout << "状态\n";
                cout << "模式:";
                if (green == 1)
                {
                    cout << "“护眼」";
                }
                else if (white == 1)
                {
                    cout << "白色";
                }
                else if (white == 0)
                {
                    cout << "黑色";
                }
                cout << '\n';
                cout << "鸣谢:";
                if (fast == 1)
                    cout << "加快";
                else
                    cout << "正常";
                cout << "\n\n\n";
                cout << "设置\n";
                cout << "按c键切换箭头所在的模式\n";
                cout << "按w向上,按s向下\n\n";
                if (green == 1)
                {
                    print("   黑色   ", sz == 1, 0);
                    print("白色(默认)", sz == 2, 0);
                }
                else if (white == 1)
                {
                    print("   黑色   ", sz == 1, 0);
                    print(" 「护眼」 ", sz == 2, 0);
                }
                else if (white == 0)
                {
                    print("白色(默认)", sz == 1, 0);
                    print(" 「护眼」 ", sz == 2, 0);
                }
                if (!fast)
                    print(" 加快鸣谢 ", sz == 3, 0);
                else
                    print(" 正常鸣谢 ", sz == 3, 0);
                print("   退出   ", sz == 4, 0);
                char ch = _getch();
                if (ch == 'c')
                    break;
                else if (ch == 'w')
                    sz = sz == 1 ? 4 : sz - 1;
                else if (ch == 's')
                    sz = sz == 4 ? 1 : sz + 1;
            }
            if (sz == 4)
                continue;
            else if (sz == 3)
                fast = !fast;
            if (green == 1)
            {
                green = 0;
                if (sz == 1)
                    white = 0;
                else if (sz == 2)
                    white = 1;
            }
            else if (white == 1)
            {
                if (sz == 1)
                    white = 0;
                else if (sz == 2)
                    green = 1;
            }
            else if (white == 0)
            {
                if (sz == 1)
                    white = 1;
                else if (sz == 2)
                    green = 1;
            }
        }
    }
    SendMessage(hwnd, WM_CLOSE, 0, 0);
}
