#ifndef OLED_H
#define OLED_H

#include <U8g2lib.h>

// 定义字体
// #define FONT u8g2_font_t0_22_tf
#define FONT  u8g2_font_6x13_tf  // 使用新字体，避免编译错误
// 定义屏幕类型
#define U8G2TYPE U8G2_SH1106_128X64_NONAME_1_HW_I2C

class OLED {
public:
  // 使用成员初始化列表正确构造 o 对象
  OLED()
    : o(U8G2_R0, U8X8_PIN_NONE) {  // reset 引脚设为无（U8X8_PIN_NONE）
  }

  U8G2TYPE o;

  void init() {
    o.begin();            // 初始化屏幕
    o.enableUTF8Print();  // 可选：启用 UTF-8 支持
  }

  void displayText(const String& text, int x, int y) {
    o.firstPage();
    do {
      o.setFont(FONT);
      o.setCursor(x, y);
      o.print(text);
    } while (o.nextPage());
  }

  void displayText(const char* text, int x, int y) {
    o.firstPage();
    do {
      o.setFont(FONT);
      o.setCursor(x, y);
      o.print(text);
    } while (o.nextPage());
  }

  void displayText(const char* text) {
    displayText(text, 28, 45);  // 默认位置
  }

  void displayText(const String& text) {
    displayText(text, 28, 45);  // 默认位置
  }
};

#endif  // OLED_H