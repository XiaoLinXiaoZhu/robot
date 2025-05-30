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

// 在默认位置显示文本(char*类型)
void OLED::displayText(const char *text) {
  displayText(0, 1, text);
}

// 清除屏幕
void OLED::clear() {
  o.clear();
}


