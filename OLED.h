#ifndef OLED_H
#define OLED_H
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// // 定义u8g2实例
// // 选择正确的屏幕芯片SH1106，分辨率128X64，接线方式I2C
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
// #define FONT u8g2_font_unifont_t_chinese2 // 定义字体
#define FONT u8g2_font_t0_22_tf // 定义字体

class OLED {
public:
    OLED() {
        u8g2.begin(); // 初始化u8g2
        // u8g2.enableUTF8Print();  // 启动UTF8，需要显示中文，必须要开启
    }

    void displayText(const String& text, int x, int y) {
        u8g2.firstPage();
        do {
            u8g2.setFont(FONT); // 设置字体
            u8g2.setCursor(x, y);
            u8g2.print(text);
        } while (u8g2.nextPage());
    }

    void displayText(const char* text, int x, int y) {
        u8g2.firstPage();
        do {
            u8g2.setFont(FONT); // 设置字体
            u8g2.setCursor(x, y);
            u8g2.print(text);
        } while (u8g2.nextPage());
    }
    
    // Add overloaded method with default coordinates
    void displayText(const char* text) {
        displayText(text, 28, 45); // Default position at (28, 45)
    }
    
    void displayText(const String& text) {
        displayText(text, 28, 45); // Default position at (28, 45)
    }
};

#endif // OLED_H