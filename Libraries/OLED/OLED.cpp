#include "OLED.h"
// Initialize the static instance pointer
OLED *OLED::instance = nullptr;

// 构造函数
OLED::OLED()
  : o() {
  init();  // 初始化OLED显示屏
}

// 初始化OLED显示屏
void OLED::init() {
  o.begin();            // 初始化屏幕
  o.setFont(FONT);     // 设置字体
  o.clear();           // 清除屏幕
}

// 在指定位置显示文本(char*类型)
void OLED::displayText(const char *text, int x, int y) {
  o.drawString(x, y, text);
}

// 清除屏幕
void OLED::clear() {
  o.clear();
}

// 在OLED上写入一行文本
void OLED::writeLine(const char *text) {
  // 当前逻辑行号（0~3）
  uint8_t currentLine = debugCount & 0x03; // MaxLine == 4 → & 0x03
  uint8_t lastLine = (currentLine - 1) & 0x03;

  // 格式化 buffer：">%02u %.12s"
  char buffer[32];
  snprintf(buffer, sizeof(buffer), ">%02u %.12s", debugCount, text);

  // 清除当前行 + 写入新内容
  o.clearLine(currentLine);
  o.setFont(FONT);
  o.drawString(0, currentLine, buffer);
  // 清除上一行（如果存在）
  o.drawString(0, lastLine, " ");

  // 更新计数器并循环（最大到 99）
  debugCount++;
  if (debugCount > MaxDebugCount) {
    debugCount = 0;
  }
}


