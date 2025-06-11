#pragma once

#ifndef OLED_LITE_H
#define OLED_LITE_H
#include <U8x8lib.h>
namespace OLED_Lite {
#define OLEDTYPE U8X8_SH1106_128X32_VISIONOX_HW_I2C
#define FONT u8x8_font_5x7_f
#define MaxDebugCount 99
#define debugCount state._debugCount
#define needInit state._needInit

    OLEDTYPE o; // 创建 OLED 对象
    struct oled_lite_config
    {
        uint8_t _debugCount :7; // 当前行号
        bool _needInit :1;
        oled_lite_config(uint8_t debugCount_ = 0, bool needInit_ = false)
            : _debugCount(debugCount_), _needInit(needInit_) {
        }
    } state;

    void init() {
        if (!needInit) return;
        o.begin();            // 初始化屏幕
        o.setFont(FONT);     // 设置字体
        o.clear();           // 清除屏幕

        o.drawString(0, debugCount & 0x03, ">00 OLED Initialized");
    }
    void checkInited() {
        if (needInit) {
            init();
        }
    }

    // 在OLED上写入一行文本
    void writeLine(const char *text) {
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

    // 在指定位置显示文本(char*类型)
    void displayText(const char *text, int x, int y) {
        checkInited(); // 确保OLED已初始化
        o.drawString(x, y, text);
    }

    // 清除屏幕
    void clear() {
        checkInited(); // 确保OLED已初始化
        o.clear();
    }
} // namespace OLED_Lite

#endif