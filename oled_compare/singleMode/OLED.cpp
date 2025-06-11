#include "OLED.h"

// 构造函数
OLED::OLED()
  : o(), debugCount(0), needInit(true) {
}

OLED oled; // 全局实例

// 初始化OLED显示屏
void OLED::init() {
  if (!needInit) return; // 如果不需要初始化，直接返回
  needInit = false; // 设置为不需要初始化
  o.begin();            // 初始化屏幕
  o.setFont(FONT);     // 设置字体
  o.clear();           // 清除屏幕

  // 打印第一行
  o.drawString(0, debugCount & 0x03, ">00 OLED Initialized");
  debugCount = 0; // 初始化debug计数器
}

// 在指定位置显示文本(char*类型)
void OLED::displayText(const char *text, int x, int y) {
  checkInited(); // 确保OLED已初始化
  o.drawString(x, y, text);
}

// 清除屏幕
void OLED::clear() {
  checkInited(); // 确保OLED已初始化
  o.clear();
}

// 在OLED上写入一行文本
void OLED::writeLine(const char *text) {
  checkInited(); // 确保OLED已初始化


  char buffer[32];
  // 当前逻辑行号（0~3）
  // 上一行 清除掉开头的指示器 >
  o.drawString(0, debugCount & 0x03, " ");
  debugCount = (debugCount + 1) % (MaxDebugCount + 1);

  // 清除当前行 + 写入新内容
  o.clearLine(debugCount & 0x03);
  o.setCursor(0, debugCount & 0x03); // 设置光标位置
  o.print(">");
  o.print(debugCount < 10 ? "0" : ""); // 前导零
  o.print(debugCount);
  o.print(" ");

  // 只打印前12字符
  for (uint8_t i = 0; i < 12 && text[i]; i++) {
    o.write(text[i]);
  }
}

void OLED::checkInited() {
  if (needInit) {
    init();
  }
}



