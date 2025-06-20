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

    extern OLEDTYPE o; // 声明而非定义
    extern struct oled_lite_config
    {
        uint8_t _debugCount :7; // 当前行号
        bool _needInit :1;
        oled_lite_config(uint8_t debugCount_ = 0, bool needInit_ = true) // 默认需要初始化
            : _debugCount(debugCount_), _needInit(needInit_) {
        }
    } state;

    void init();
    void checkInited();
    void writeLine(const char *text);
    void displayText(const char *text, int x, int y);
    void clear();
} // namespace OLED_Lite

#endif