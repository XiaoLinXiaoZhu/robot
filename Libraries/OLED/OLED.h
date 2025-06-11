// 改为使用 U8X8 库的无完整缓冲版本
// 因为 U8G2 库的完整缓冲版本会导致占用（128x64 = 512 bit）太大了
// 这个 Arduino 的 内存只有 2KB
#ifndef OLED_H
#define OLED_H
#include <U8x8lib.h>
#include <stdint.h>
// 定义字体
// #define FONT u8x8_font_chroma48medium8_r
#define FONT u8x8_font_5x7_f
// 使用 U8X8 库的无完整缓冲版本
#define OLEDTYPE U8X8_SH1106_128X32_VISIONOX_HW_I2C

// OLED 最多显示 99 行,使用 uint8_t 类型
constexpr uint8_t MaxDebugCount = 99;

class OLED {
public:
  OLED();
  U8X8_SH1106_128X32_VISIONOX_HW_I2C o;
  uint8_t debugCount : 7;
  bool needInit = true; // 是否需要初始化

  // 初始化OLED显示屏
  void init();
  void checkInited();

  // 在指定位置显示文本
  void displayText(const char *text, int x, int y);

  void clear();

  // 在OLED上写入一行文本，会附带行号和当前行标识
  void writeLine(const char *text);
};

extern OLED oled; // 全局实例

#endif // OLED_H