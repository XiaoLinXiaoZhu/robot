// 改为使用 U8X8 库的无完整缓冲版本
// 因为 U8G2 库的完整缓冲版本会导致占用（128x64 = 512 bit）太大了
// 这个 Arduino 的 内存只有 2KB
#ifndef OLED_H
#define OLED_H
#include <U8x8lib.h>

// 定义字体
// #define FONT u8x8_font_chroma48medium8_r
#define FONT u8x8_font_5x7_f
// 使用 U8X8 库的无完整缓冲版本
#define OLEDTYPE U8X8_SH1106_128X32_VISIONOX_HW_I2C 

// OLED 最多显示 4 行,使用 uint8_t 类型
#define MaxLine uint8_t(4)

class OLED {
public:
  // 构造函数
  OLED();

  OLEDTYPE o;

  // 初始化OLED显示屏
  void init();

  // 在指定位置显示文本
  void displayText(const char* text, int x, int y);
  
  // 在默认位置显示文本
  void displayText(const char* text);

  void clear();
  
  uint8_t debugCount = 0;
  void writeLine(const char* text) {
    // 将 [debugCount] 拼接到 text 前面
    char buffer[32];
    uint8_t currentLine = debugCount % MaxLine; // 当前行数，最多显示 MaxLine 行
    snprintf(buffer, sizeof(buffer), "%d>%s", debugCount++, text);
    // 在 OLED 上显示文本
    o.setFont(FONT); // 设置字体
    o.clearLine(currentLine); // 清除当前行
    o.drawString(1, currentLine, buffer);
  }
  
};

#endif // OLED_H