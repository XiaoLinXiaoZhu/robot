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

// OLED 最多显示 4 行,使用 uint8_t 类型
#define MaxLine uint8_t(4)
// OLED 最多显示 99 行,使用 uint8_t 类型
#define MaxDebugCount uint8_t(99)
#define oled OLED::getInstance()

class OLED
{
private:
  // Private constructor for singleton
  OLED();

  // Static instance
  static OLED *instance;

  // Debug counter
  uint8_t debugCount = 0;

public:
  OLEDTYPE o;

  // Delete copy constructor and assignment operator
  OLED(const OLED &) = delete;
  void operator=(const OLED &) = delete;

  // Get the singleton instance
  static OLED *getInstance()
  {
    if (instance == nullptr)
    {
      instance = new OLED();
    }
    return instance;
  }

  // 初始化OLED显示屏
  void init();

  // 在指定位置显示文本
  void displayText(const char *text, int x, int y);

  // 在默认位置显示文本
  void displayText(const char *text);

  void clear();

  void writeLine(const char *text)
  {
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
    if (debugCount > MaxDebugCount)
    {
      debugCount = 0;
    }
  }
};

#endif // OLED_H