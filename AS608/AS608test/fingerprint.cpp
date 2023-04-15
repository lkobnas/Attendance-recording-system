#include "fingerprint.h"

bool Fingerprint::waitUntilDetectFinger(int wait_time) {
  while (true) {
    if (PS_DetectFinger()) {
      return true;
    }
    else {
      delay(100);
      wait_time -= 100;
      if (wait_time < 0) {
        return false;
      }
    }
  }
}

bool Fingerprint::waitUntilNotDetectFinger(int wait_time) {
  while (true) {
    if (!PS_DetectFinger()) {
      return true;
    }
    else {
      delay(100);
      wait_time -= 100;
      if (wait_time < 0) {
        return false;
      }
    }
  }
}

int Fingerprint::fp_init(){
      // 1.读取配置文件，获得芯片地址和通信密码
//   if (!readConfig())
//     exit(1);
    g_config.address = 0xffffffff;
    g_config.password= 0x00000000;
    g_config.has_password = 0;
    g_config.baudrate = 57600;
    g_config.detect_pin = 1; 
    strcpy(g_config.serial, "/dev/ttyAMA0");

    writeConfig(); 


  if (g_verbose == 1)
    printConfig();
  
  // 3.初始化wiringPi库
  if (-1 == wiringPiSetup()) {
    printf("wiringPi setup failed!\n");
    return 1;
  }

  // 4.检测是否有手指放上的GPIO端口，设为输入模式
  pinMode(g_config.detect_pin, INPUT);

  // 5.打开串口
	if((g_fd = serialOpen(g_config.serial, g_config.baudrate)) < 0)	{
		fprintf(stderr,"Unable to open serial device: %s\n", strerror(errno));
		return 2;
	}

  // 6.注册退出函数(打印一些信息、关闭串口等)
  atexit(atExitFunc);

  // 7.初始化 AS608 模块
  // 地址 密码
  PS_Setup(g_config.address, g_config.password) ||  PS_Exit();
return 0;
}

void Fingerprint::fp_add(int address)
{
    printf("Please put your finger on the module.\n");
    if (waitUntilDetectFinger(5000)) {
      delay(500);
      PS_GetImage() || PS_Exit();
      PS_GenChar(1) || PS_Exit();
    }
    else {
      printf("Error: Didn't detect finger!\n");
      exit(1);
    }

    // 判断用户是否抬起了手指，
    printf("Ok.\nPlease raise your finger!\n");
    if (waitUntilNotDetectFinger(5000)) {
      delay(100);
      printf("Ok.\nPlease put your finger again!\n");
      // 第二次录入指纹
      if (waitUntilDetectFinger(5000)) {
        delay(500);
        PS_GetImage() || PS_Exit();
        PS_GenChar(2) || PS_Exit();
      }
      else {
        printf("Error: Didn't detect finger!\n");
        exit(1);
      }
    }
    else {
      printf("Error! Didn't raise your finger\n");
      exit(1);
    }

    int score = 0;
    if (PS_Match(&score)) {
      printf("Matched! score=%d\n", score);
    }
    else {
      printf("Not matched, raise your finger and put it on again.\n");
      exit(1);
    }
    
    if (g_error_code != 0x00)
      PS_Exit();

    // 合并特征文件
    PS_RegModel() || PS_Exit();
    PS_StoreChar(2, address) || PS_Exit();
}
void Fingerprint::fp_list(){
    int indexList[512] = { 0 };
      PS_ReadIndexTable(indexList, 512) ||  PS_Exit();

      int i = 0;
      for (i = 0; i < 300; ++i) {
        if (indexList[i] == -1)
          break;
        printf("%d\n", indexList[i]);
      }
      if (i == 0) {
        printf("The database is empty!\n");
      } 
}

void Fingerprint::atExitFunc() {
  if (g_verbose == 1)
    printf("Exit\n");
  if (g_fd > 0)
    serialClose(g_fd); 
}
bool Fingerprint::PS_Exit() {
  printf("ERROR! code=%02X, desc=%s\n", g_error_code, PS_GetErrorDesc());
  exit(2);
  return true;
}

void Fingerprint::printConfig() {
  printf("address=%08x\n", g_config.address);
  if (g_config.has_password)
    printf("password=%08x\n", g_config.password);
  else
    printf("password=none(no password)\n");
  printf("serial_file=%s\n",   g_config.serial);
  printf("baudrate=%d\n",   g_config.baudrate);
  printf("detect_pin=%d\n",   g_config.detect_pin);
}

// 同步g_config变量内容和其他变量内容
void Fingerprint::syncConfig() {
  g_as608.detect_pin   = g_config.detect_pin;
  g_as608.has_password = g_config.has_password;
  g_as608.password     = g_config.password;
  g_as608.chip_addr    = g_config.address;
  g_as608.baud_rate    = g_config.baudrate;
}

// 读取配置文件
bool Fingerprint::readConfig() {
  FILE* fp;

  // 获取用户主目录
  char filename[256] = { 0 };
  sprintf(filename, "%s/.fpconfig", getenv("HOME"));
  
  // 主目录下的配置文件
  if (false){//access(filename, F_OK) == 0) { 
    //trimSpaceInFile(filename);
    //fp = fopen(filename, "r");
  }
  else {
    // 如果配置文件不存在，就在主目录下创建配置文件，并写入默认配置
    // 设置默认值
    g_config.address = 0xffffffff;
    g_config.password= 0x00000000;
    g_config.has_password = 0;
    g_config.baudrate = 57600;
    g_config.detect_pin = 1; 
    strcpy(g_config.serial, "/dev/ttyAMA0");

    writeConfig();

    printf("Please config the address and password in \"~/.fpconfig\"\n");
    printf("  fp cfgaddr   0x[address]\n");
    printf("  fp cfgpwd    0x[password]\n");
    printf("  fp cfgserial [serialFile]\n");
    printf("  fp cfgbaud   [rate]\n");
    printf("  fp cfgpin    [GPIO_pin]\n");
    return false;
  }

  char key[16] = { 0 };
  char value[16] = { 0 };
  char line[32] = { 0 };

  char *tmp;
  while (!feof(fp)) {
    fgets(line, 32, fp);
    
    // 分割字符串，得到key和value
    if (tmp = strtok(line, "="))
      trim(tmp, key);
    else
      continue;
    if (tmp = strtok(NULL, "="))
      trim(tmp, value);
    else
      continue;
    while (!tmp)
      tmp = strtok(NULL, "=");

    // 如果数值以 0x 开头
    int offset = 0;
    if (value[0] == '0' && (value[1] == 'x' || value[1] == 'X'))
      offset = 2;

    if (strcmp(key, "address") == 0) {
      g_config.address = toUInt(value+offset);
    }
    else if (strcmp(key, "password") == 0) {
      if (strcmp(value, "none") == 0 || strcmp(value, "false") == 0) {
        g_config.has_password = 0; // 无密码
      }
      else {
        g_config.has_password = 1; // 有密码
        g_config.password = toUInt(value+offset);
      }
    }
    else if (strcmp(key, "serial") == 0) {
      int len = strlen(value);
      if (value[len-1] == '\n')
        value[len-1] = 0;
      strcpy(g_config.serial, value);
    }
    else if (strcmp(key, "baudrate") == 0) {
      g_config.baudrate = toInt(value);
    }
    else if (strcmp(key, "detect_pin") == 0) {
      g_config.detect_pin = toInt(value);
    }
    else {
      printf("Unknown key:%s\n", key);
      fclose(fp);
      return false;
    }

  } // end while(!feof(fp))

  asyncConfig();

  fclose(fp);
  return true;
}
void Fingerprint::asyncConfig() {
  g_as608.detect_pin   = g_config.detect_pin;
  g_as608.has_password = g_config.has_password;
  g_as608.password     = g_config.password;
  g_as608.chip_addr    = g_config.address;
  g_as608.baud_rate    = g_config.baudrate;
}

/*
 * 写配置文件
*/
bool Fingerprint::writeConfig() {
  // 获取用户主目录
  char filename[256] = { 0 };
  sprintf(filename, "%s/.fpconfig", getenv("HOME"));
  
  FILE* fp = fp = fopen(filename, "w+");
  if (!fp) {
    printf("Write config file error!\n");
    exit(0);
  }

  fprintf(fp, "address=0x%08x\n", g_config.address);
  if (g_config.has_password)
    fprintf(fp, "password=0x%08x\n", g_config.password);
  else
    fprintf(fp, "password=none\n");
  fprintf(fp, "baudrate=%d\n", g_config.baudrate);
  fprintf(fp, "detect_pin=%d\n", g_config.detect_pin);
  fprintf(fp, "serial=%s\n", g_config.serial);

  fclose(fp);
}