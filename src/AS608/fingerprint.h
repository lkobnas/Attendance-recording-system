#include "AS608_lib/as608.h"
#include "AS608_lib/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdint.h>

extern AS608 g_as608;
extern int g_fd;
extern int g_verbose;
extern char  g_error_desc[128];
extern uchar g_error_code;

class Fingerprint{

    public:
        int fp_init();
        int fp_add();
        int fp_enroll();

        void fp_list();
        int fp_identify();

    private:

        int  g_argc = 0;   // 参数个数，g_argc = argc - g_option_count
        int  g_option_count = 0; // 选项个数-v、-h等
        char g_command[16] = { 0 };     // 即argv[1]
        Config g_config;   // 配置文件 结构体，定义在"./utils.h"头文件中

        bool waitUntilDetectFinger(int wait_time);   // 阻塞至检测到手指，最长阻塞wait_time毫秒
        bool waitUntilNotDetectFinger(int wait_time);
        void printConfig();
        bool readConfig();  // 读取文件到 g_config
        bool writeConfig(); // 将 g_config 写入文件
        void asyncConfig(); 
        void syncConfig();
        static void atExitFunc();
        bool PS_Exit();
    
};