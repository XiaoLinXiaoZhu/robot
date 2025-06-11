#include <Wire.h>
#include "OLED.h"
#define DEBUG(message) \
  do { \
      oled.writeLine(message); \
      Serial.print("DEBUG: "); \
      Serial.println(message); \
  } while (0)


void setup(void) {
  Serial.begin(9600);              // 初始化串口通信
  delay(200);                      // 延时2秒
  oled.writeLine("Arduino OLED"); // 在指定位置显示文本
  delay(200);                      // 延时2秒
  oled.writeLine("Your input");
  oled.writeLine("will be print");
  oled.writeLine("below");
  delay(200); // 延时2秒
}

void loop(void) {
  if (Serial.available()) {                                                     // 检查是否有可用的数据
    // String receivedData = Serial.readStringUntil('\n'); // 读取一行数据直到遇到换行符
    // oled->writeLine(receivedData.c_str());              // 调用writeLine函数显示数据

    readSerial(); // 调用readSerial函数处理串口输入
  }
}

char inChar; // Variable to hold the incoming character
char term = '\r'; // Terminator character, default is '\r' (carriage return)
char buffer[64]; // Buffer to hold the command
char *token; // Pointer to hold the current token
char *last; // Pointer to hold the last token
int bufPos = 0; // Position in the buffer
const char *delim = " \t"; // Delimiters for tokenization, space and tab
// Function to read from the serial port and process commands
void readSerial() {
  bool onlyOneCommand = true;
  // If we're using the Hardware port, check it.   
  // Otherwise check the user-created OttoSoftwareSerial Port
  while ((Serial.available() > 0) && (onlyOneCommand == true)) {
    int i;
    boolean matched;

    inChar = Serial.read(); // Read single available character, there may be more waiting
    // debug
    char debugBuffer[32];
    snprintf(debugBuffer, sizeof(debugBuffer), "0x%02X", inChar);
    DEBUG(debugBuffer); // Print the character in hex format for debugging

    if (inChar == term) { // Check for the terminator (default '\r') meaning end of command

      onlyOneCommand = false; //

      bufPos = 0; // Reset to start of buffer
      // debug buffer
      char debugBuffer[64];
      snprintf(debugBuffer, sizeof(debugBuffer), "Command: %s", buffer);
      DEBUG(debugBuffer); // Print the command for debugging
      token = strtok_r(buffer, delim, &last); // Search for command at start of buffer
    }
  }
}