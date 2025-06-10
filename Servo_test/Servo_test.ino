#include <EEPROM.h>  // 引入EEPROM库
#include <Servo.h>


//-================ 针脚定义=================-
#define FRONT_RIGHT_HIP 0
#define FRONT_LEFT_HIP 1
#define FRONT_RIGHT_LEG 2
#define FRONT_LEFT_LEG 3
#define BACK_RIGHT_HIP 4
#define BACK_LEFT_HIP 5
#define BACK_RIGHT_LEG 6
#define BACK_LEFT_LEG 7

//-================ 舵机定义=================-
//   board_pins[FRONT_RIGHT_HIP] = 2; // front left inner
//   board_pins[FRONT_LEFT_HIP] = 8;  // front right inner
//   board_pins[BACK_RIGHT_HIP] = 4;  // back left inner
//   board_pins[BACK_LEFT_HIP] = 6;   // back right inner
//   board_pins[FRONT_RIGHT_LEG] = 3; // front left outer
//   board_pins[FRONT_LEFT_LEG] = 9;  // front right outer
//   board_pins[BACK_RIGHT_LEG] = 5;  // back left outer
//   board_pins[BACK_LEFT_LEG] = 7;   // back right outer

int board_pins[8] = {
  2, 8, 4, 6, 3, 9, 5, 7
};

Servo servo[8];
void bindServo() {
  // 绑定舵机
  for (int i = 0; i < 8; i++) {
    servo[i].attach(board_pins[i]);  // 将舵机绑定到对应的引脚
  }
}

//-================ 快速加载设置=================-
#define EEPROM_FASTLOAD_MAGIC 0xD2  // EEPROM地址(用于设置是否快速启动)
#define EEPROM_FASTLOAD_OFFSET 25   // EEPROM偏移量(用于设置是否快速启动)
bool getEEPROMFastLoad() {
  // 获取EEPROM快速加载标志
  return EEPROM.read(EEPROM_FASTLOAD_OFFSET) == EEPROM_FASTLOAD_MAGIC;
}
void setEEPROMFastLoad(bool enable) {
  // 设置EEPROM快速加载标志
  EEPROM.write(EEPROM_FASTLOAD_OFFSET, enable ? EEPROM_FASTLOAD_MAGIC : 0);
}

#define EEPROM_MAGIC 0xabcd
#define EEPROM_OFFSET 2                    // eeprom starting offset to store trim[]
int trim[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };  // 舵机偏移量数组


//-================ EEPROM操作函数 =================-
// 读取EEPROM中的一个字（2字节）
int EEPROMReadWord(int p_address) {
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
// 写入一个字（2字节）到EEPROM
void EEPROMWriteWord(int p_address, int p_value) {
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}


//-================ 舵机偏移量操作函数 =================-
void loadTrim() {
  // 校验 magic number
  int val = EEPROMReadWord(0);
  if (val != EEPROM_MAGIC) {
    EEPROMWriteWord(0, EEPROM_MAGIC);
    storeTrim();
  }

  // 从EEPROM加载舵机偏移量
  for (int i = 0; i < 8; i++) {
    int value = EEPROMReadWord(i * 2 + EEPROM_OFFSET);
    if (value >= -90 && value <= 90) {
      trim[i] = value;
    }
  }
}

void storeTrim() {
  // 将舵机偏移量存储到EEPROM
  for (int i = 0; i < 8; i++) {
    EEPROMWriteWord(i * 2 + EEPROM_OFFSET, trim[i]);
    delay(100);
  }
}



//-================ 调试函数 =================-
void debug(const char* message) {
  // 简单的调试输出函数
  Serial.println(message);
}


//-================ 主程序 =================-
/**
 * @brief 初始化函数
 * @details 设置串口通信、LED矩阵、机器人初始状态等
 */
void setup() {
  Serial.begin(9600);          // 初始化串口通信
  // randomSeed(analogRead(A6));  // 设置随机种子

  if (getEEPROMFastLoad()) {
    debug("Fast Reload");
  } else {
    debug("Slow Reload");
    delay(5000);
  }
  setEEPROMFastLoad(true);  // 设置快速加载标志

  // 读取偏移量
  loadTrim();  // 从EEPROM加载舵机偏移量
  // 绑定舵机
  bindServo();  // 绑定舵机到对应的引脚
  // 初始化舵机位置
  setEEPROMFastLoad(false);  // 禁用快速加载标志
  for (int i = 0; i < 8; i++) {
    servo[i].write(90 + trim[i]);  // 设置舵机初始位置
    delay(100);                    // 等待舵机稳定
  }
}


/**
 * @brief 主循环函数
 * @details 处理舵机动作、传感器输入等
 */
void loop() {
  // 使用计时器控制速度
  // debug
  debug("Looping...");

  static unsigned long previousMillis = 0;
  static int angle = 0;
  static bool increasing = true;
  unsigned long currentMillis = millis();
  
  // 每20毫秒执行一次
  if (currentMillis - previousMillis >= 20) {
    previousMillis = currentMillis;

    setEEPROMFastLoad(false);  // 禁用快速加载标志
    
    // 在-20到20度之间摆荡
    if (increasing) {
      angle++;
      if (angle >= 20) increasing = false;
    } else {
      angle--;
      if (angle <= -20) increasing = true;
    }
    
    // 应用到所有舵机，考虑trim值
    for (int i = 0; i < 8; i++) {
      servo[i].write(90 + trim[i] + angle);
    }
  }
}