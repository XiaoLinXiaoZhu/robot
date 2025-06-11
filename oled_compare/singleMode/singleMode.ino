#include <Wire.h>
#include "OLED.h"
#define DEBUG(message) \
  do { \
      oled.writeLine(message); \
      Serial.print("DEBUG: "); \
      Serial.println(message); \
  } while (0)


void setup(void) {
  Serial.begin(9600);              // 初始化串口通信
  delay(200);                      // 延时2秒
  Serial.print("Serial Setup");
  oled.init(); // 初始化OLED显示屏
  delay(200);                      // 延时0.2秒
  DEBUG("Serial initialized"); // 调试信息
  DEBUG("Your input");
  DEBUG("will be print");
  DEBUG("below");
  delay(200); // 延时0.2秒
}

void loop(void) {
  if (Serial.available()) {                                                     // 检查是否有可用的数据

    readSerial(); // 调用readSerial函数处理串口输入
  }
}

void readSerial() {
  String receivedData = Serial.readStringUntil('\n');
  DEBUG(receivedData.c_str());
}