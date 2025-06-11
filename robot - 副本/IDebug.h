#pragma once

#include <Arduino.h>

// 调试输出方式选择：
//   启用其中一个，或者都不启用以关闭调试输出
// #define DEBUGOUTPUT_SERIAL
#define DEBUGOUTPUT_OLED

#ifdef DEBUGOUTPUT_SERIAL
    #define debug(message) Serial.println(message)
#elif defined(DEBUGOUTPUT_OLED)
    #include "oled_lite.h"
    #define debug(message) do { Serial.println(message);OLED_Lite::writeLine(message);} while(0)
#else
    #define debug(message) do {} while (0)  // 不执行任何操作
#endif