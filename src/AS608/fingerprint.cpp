#include "fingerprint.h"
#include <wiringPi.h>

/// @brief Wait until user place his finger on the sensor
/// @param wait_time is the waiting time for the user to place his finger
/// @return true : user placed his finger, false: otherwise
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

/// @brief Wait until user lift his finger on the sensor
/// @param wait_time is the waiting time for the user to lift his finger
/// @return true : user lifted his finger, false: otherwise
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

/// @brief 
/// @return 1: wiringPi setup failed, 2: serial device setup failed, 0: Sensor init succeed
int Fingerprint::fp_init(){
    g_config.address = 0xffffffff;
    g_config.password= 0x00000000;
    g_config.has_password = 0;
    g_config.baudrate = 57600;
    g_config.detect_pin = 1; 
    strcpy(g_config.serial, "/dev/ttyAMA0");

    g_as608.detect_pin   = g_config.detect_pin;
    g_as608.has_password = g_config.has_password;
    g_as608.password     = g_config.password;
    g_as608.chip_addr    = g_config.address;
    g_as608.baud_rate    = g_config.baudrate;

    writeConfig(); 


  if (g_verbose == 1)
    printConfig();
  
  if (-1 == wiringPiSetup()) {
    printf("wiringPi setup failed!\n");
    return 1;
  }

  pinMode(g_config.detect_pin, INPUT);

	if((g_fd = serialOpen(g_config.serial, g_config.baudrate)) < 0)	{
		fprintf(stderr,"Unable to open serial device: %s\n", strerror(errno));
		return 2;
	}

  atexit(atExitFunc);

  PS_Setup(g_config.address, g_config.password) ||  PS_Exit();
return 0;
}

/// @brief Find unused index in fingersensor and return its address
/// @param indexList is the fingerprint index library
/// @param size is indexlist size
/// @return -1: Sensor memory full, otherwise: fingerprint address
int findUnusedIndex(const int* indexList, const int size) {
    bool found = false;
    int index = -1;
    for (int i = 0; i < 300 && !found; i++) {
        for (int j = 0; j < size; j++) {
            if (i == indexList[j]) {
                break;
            }
            if (j == size - 1) {
                found = true;
                index = i;
            }
        }
    }
    return index;
}

/// @brief Enroll user fingerprint data in to an empty memory, only read user finger one 
/// @return fingerprint address
int Fingerprint::fp_enroll(){
    int address = -1;
    int indexList[512] = { 0 };
    PS_ReadIndexTable(indexList, 512) ||  PS_Exit();

    address = findUnusedIndex(indexList,512);  
    int count = 0;
    printf("Please put your finger on the moudle\n");
    while (digitalRead(g_as608.detect_pin) == LOW) {
      delay(500);
      if(waitUntilDetectFinger(5000)){
        break;
      }else{
        printf("No finger detected!\n");
        return -1;
      }
    }     
    int pageID = address;
    PS_Enroll(&pageID) || PS_Exit();
    indexList[512] = { 0 };
    PS_ReadIndexTable(indexList, 512) ||  PS_Exit();
    for(int i=0;i<300;++i){
      if(indexList[i]==pageID)
        break;
      
      if(indexList[i]==-1)
        return -1;
    }
return pageID;
}

/// @brief Enroll user fingerprint data in to an empty memory, read user finger twice 
/// @return fingerprint address
int Fingerprint::fp_add()
{
    int address = -1;
    int indexList[512] = { 0 };
    PS_ReadIndexTable(indexList, 512) ||  PS_Exit();

    address = findUnusedIndex(indexList,512);

    printf("Please put your finger on the module.\n");
    if (waitUntilDetectFinger(5000)) {
      PS_GetImage() || PS_Exit();
      PS_GenChar(1) || PS_Exit();
    }
    else {
      printf("Error: Didn't detect finger!\n");
      return -1;
    }

    // check if finger lifted
    printf("Ok.\nPlease raise your finger!\n");
    if (waitUntilNotDetectFinger(5000)) {
      //delay(100);
      printf("Ok.\nPlease put your finger again!\n");
      // second record
      if (waitUntilDetectFinger(5000)) {
        //delay(500);
        PS_GetImage() || PS_Exit();
        PS_GenChar(2) || PS_Exit();
      }
      else {
        printf("Error: Didn't detect finger!\n");
        return -1;
      }
    }
    else {
      printf("Error! Didn't raise your finger\n");
      return -1;
    }

    int score = 0;
    if (PS_Match(&score)) {
      printf("Matched! score=%d\n", score);
    }
    else {
      printf("Not matched, raise your finger and put it on again.\n");
      return -1;
    }
    
    if (g_error_code != 0x00)
      PS_Exit();

    PS_RegModel() || PS_Exit();
    PS_StoreChar(2, address) || PS_Exit();

    return address;
}

/// @brief List all fingerprint ID in sensor memory, for debug use only
void Fingerprint::fp_list(){
    int indexList[512] = { 0 };
      PS_ReadIndexTable(indexList, 512) ||  PS_Exit();
      int i = 0;
      for (i = 0; i < 300; ++i) {
        printf("IndexList %d: %d",i,indexList[i]);
        if (indexList[i] == -1)
          break;
        printf("%d\n", indexList[i]);
      }
      if (i == 0) {
        printf("The database is empty!\n");
      } 
      int address = findUnusedIndex(indexList, 512);
      printf("Address: %d",address);
}

/// @brief Check and compare user fingerprint
/// @return -1: fingerprint not found, otherwise: fingerprint address 
int Fingerprint::fp_identify(){
    int pageID = 0;
    int score = 0;
    
    // Check if finger exist
    int count = 0;
    printf("Please put your finger on the moudle\n");
    while (digitalRead(g_as608.detect_pin) == LOW) {
      // while (digitalRead(1) == LOW) {
      delay(1);
      if ((count++) > 100) {
        printf("No finger detected!\n"); 
        return -1;
      }
    }
    PS_Identify(&pageID, &score) || PS_Exit();
    if(score<10){
      return -1;
    }
    printf("Matched! pageID=%d score=%d\n", pageID, score); 
return pageID;
}

void Fingerprint::atExitFunc() {
  if (g_verbose == 1)
    printf("Exit\n");
  if (g_fd > 0)
    serialClose(g_fd); 
}
bool Fingerprint::PS_Exit() {
  //printf("Recurring: code=%02X, desc=%s\n", g_error_code, PS_GetErrorDesc());
  printf("Recurring: No finger detected");
  return false;
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

// Sync config and other variables
void Fingerprint::syncConfig() {
  g_as608.detect_pin   = g_config.detect_pin;
  g_as608.has_password = g_config.has_password;
  g_as608.password     = g_config.password;
  g_as608.chip_addr    = g_config.address;
  g_as608.baud_rate    = g_config.baudrate;
}

bool Fingerprint::readConfig() {
  FILE* fp;

  char filename[256] = { 0 };
  sprintf(filename, "%s/.fpconfig", getenv("HOME"));
  
  if (false){//access(filename, F_OK) == 0) { 
    //trimSpaceInFile(filename);
    //fp = fopen(filename, "r");
  }
  else {
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

    int offset = 0;
    if (value[0] == '0' && (value[1] == 'x' || value[1] == 'X'))
      offset = 2;

    if (strcmp(key, "address") == 0) {
      g_config.address = toUInt(value+offset);
    }
    else if (strcmp(key, "password") == 0) {
      if (strcmp(value, "none") == 0 || strcmp(value, "false") == 0) {
        g_config.has_password = 0; 
      }
      else {
        g_config.has_password = 1; 
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
 * Write config file
*/
bool Fingerprint::writeConfig() {
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
  return true;
}