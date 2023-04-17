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

/// @brief A class for managing the fingerprint sensor
class Fingerprint{

    public:
        int fp_init();
        int fp_add();
        int fp_enroll();

        void fp_list();
        int fp_identify();

    private:

        int  g_argc = 0;  
        int  g_option_count = 0; 
        char g_command[16] = { 0 };    
        Config g_config;  

        bool waitUntilDetectFinger(int wait_time);   
        bool waitUntilNotDetectFinger(int wait_time);
        void printConfig();
        bool readConfig();  
        bool writeConfig(); 
        void asyncConfig(); 
        void syncConfig();
        static void atExitFunc();
        bool PS_Exit();
    
};