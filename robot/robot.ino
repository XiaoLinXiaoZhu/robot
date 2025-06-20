#include "robot.h"
#include "SmartLoad.h"

/*
  舵机引脚连接示意图：
  __________ __________ _________________
  |(pin9)_____)(pin8)  (pin2)(______(pin3)|  // 前腿舵机连接
  |__|       |left FRONT right|        |__|
            |                |
            |                |
            |                |               // 身体部分
  _________ |                | __________
  |(pin7)_____)(pin6)__(pin4)(______(pin5)|  // 后腿舵机连接
  |__|                                 |__|

  超声波传感器：
  TRIGGER引脚 - 12
  ECHO引脚    - 11

  蜂鸣器引脚 - 13
*/

void checkMemory() {
  // 检查内存使用情况
  extern int __heap_start, *__brkval;
  int v;
  int freeMemory = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "Free Memory: %d bytes", freeMemory);
  debug(buffer);
}

void setTrim(int index, int value) {
  // 设置舵机偏移量
  debug("setting trim");
  robot.setTrim(index, value);
  robot.home();  // 回到初始位置
  delay(2000);   // 延时200毫秒
}

/**
 * @brief 初始化函数
 * @details 设置串口通信、LED矩阵、机器人初始状态等
 */
void setup() {
  Serial.begin(9600);          // 初始化串口通信
  randomSeed(analogRead(A6));  // 设置随机种子

  if (getEEPROMFastLoad()) {
    debug("Fast Reload");
  } else {
    debug("Slow Load");
    delay(4000);
  }
  setEEPROMFastLoad(true);  // 设置快速加载标志



  // 初始化机器人
  robot.init(PIN_Buzzer);

  checkMemory();  // 检查内存使用情况
  // 设置反转情况
  // robot.reverseServo(0);  // 反转前右腿
  robot.reverseServo(2);  // 反转前左腿
  robot.reverseServo(3);
  // robot.reverseServo(7);


  // 校准程序
  debug("Calibrating...");  // 显示校准信息
  robot.home();             // 回到初始位置
  delay(2000);
  robot.refresh();  // 刷新舵机位置
  robot.putMouth(smile);  // 显示笑脸
  delay(200);

  // 注册串口命令处理函数
  SerialCmd.addCommand("S", receiveStop);         // 停止命令
  SerialCmd.addCommand("L", receiveLED);          // LED控制
  SerialCmd.addCommand("M", receiveMovement);     // 运动控制
  SerialCmd.addCommand("H", receiveGesture);      // 手势控制
  SerialCmd.addCommand("K", receiveSing);         // 声音控制
  SerialCmd.addCommand("D", requestDistance);     // 距离请求
  SerialCmd.addCommand("I", requestProgramId);    // 程序ID请求
  SerialCmd.addCommand("J", requestMode);         // 模式请求
  SerialCmd.addCommand("C", requestCalibration);  // 校准请求
  SerialCmd.addDefaultHandler(receiveStop);       // 默认处理函数

  // 设置默认模式
  setMode(DEFAULT_MODE);  // 使用新模式设置函数
  // setMode(3); // 设置为预留模式3(可以根据需要修改为其他模式)
  // moveId = 2;  // 初始化动作ID
  // robot.setRestState(false);  // 确保机器人不处于休息状态
}

/**
 * @brief 主循环函数
 * @details 根据当前模式执行不同行为，处理串口输入
 */
void loop() {
  // delay(200);  // 暂停200毫秒，避免过快循环
  // checkMemory(); // 检查内存使用情况


  // 串口数据处理
  if (Serial.available() > 0 && CURRENT_MODE != 4) {
    SerialCmd.readSerial();     // 读取串口命令
    robot.putMouth(happyOpen);  // 显示张嘴表情
  }

  // 模式切换处理
  switch (CURRENT_MODE) {
  case 0:  // 模式0: 待机
    break;

  case 1:  // 模式1: 随机舞蹈
    randomDance = random(5, 15);
    if (randomDance == 5)
      randomDance = 6;
    robot.putMouth(random(10, 21));  // 随机表情
    robot.home();                    // 回到初始位置
    pause(750);                      // 暂停
    gaits(randomDance);              // 执行随机步态
    pause(8000);                     // 暂停8秒
    break;

  case 2:                   // 模式2: 避障模式
    robot.run(0);           // 停止移动
    robot.putMouth(smile);  // 笑脸表情
    pause(500);
    obstacleDetector();  // 检测障碍物
    pause(500);

    if (obstacleDetected)  // 如果检测到障碍物
    {
      robot.home();                 // 回到初始位置
      robot.putMouth(bigSurprise);  // 惊讶表情
      pause(3000);
      robot.putMouth(xMouth);  // X嘴表情
      robot.run(1);            // 后退
      pause(3000);
      robot.putMouth(sad);  // 悲伤表情
      robot.home();
      pause(3000);
      robot.turnL(1, 550);  // 左转
      pause(3000);
      obstacleDetector();  // 再次检测
    }

    robot.refresh();  // 刷新状态
    break;

  case 3:  // 模式3: (预留)
    // 直接前进
    if (robot.getRestState() == false)  // 如果不是休息状态
    {
      setEEPROMFastLoad(false);  // 禁用快速加载
      gaits(moveId);             // 执行指定动作
    }

    robot.refresh();  // 刷新状态(相当于update)
    break;
  case 4:                    // 模式4: 手动控制模式
    SerialCmd.readSerial();  // 读取串口命令

    if (robot.getRestState() == false)  // 如果不是休息状态
    {
      setEEPROMFastLoad(false);  // 禁用快速加载
      gaits(moveId);             // 执行指定动作
    }

    robot.refresh();  // 刷新状态(相当于update)
    break;
  default:
    // 默认回到模式0
    setMode(0);  // 使用新模式设置函数
    break;
  }
}

bool gaits(int cmd) {
  bool manualMode = false;
  bool taken = true;

  if (prev_cmd == cmd)  // 如果与上次命令相同则跳过
    return;

  // 动作指令处理
  switch (cmd) {
  case 1:
    robot.run(0);
    break;  // 停止
  case 2:
    robot.run(1, 4.0F, moveMentPeriod);
    break;  // 前进
  case 3:
    robot.turnL(1, 550);
    break;  // 左转
  case 4:
    robot.turnR(1, 550);
    break;  // 右转
  case 5:
    robot.home();
    break;  // 归位
  case 6:
    robot.pushUp();
    break;  // 俯卧撑
  case 7:
    robot.upDown();
    break;  // 上下运动
  case 8:
    robot.waveHAND();
    break;  // 挥手
  case 9:
    robot.Hide();
    break;  // 隐藏
  case 10:
    robot.omniWalk(true);
    break;  // 全向行走(前)
  case 11:
    robot.omniWalk(false);
    break;  // 全向行走(后)
  case 12:
    robot.dance(1, 1000);
    break;  // 舞蹈1
  case 13:
    robot.frontBack(1, 750);
    break;  // 前后运动
  case 14:
    robot.jump();
    break;  // 跳跃
  case 15:
    robot.scared();
    break;  // 害怕动作

  case 90:
    robot.runSingle(0);
    break;  // 0 号舵机单独动作
  case 91:
    robot.runSingle(1);
    break;  // 1 号舵机单独动作
  case 92:
    robot.runSingle(2);
    break;  // 2 号舵机单独动作
  case 93:
    robot.runSingle(3);
    break;  // 3 号舵机单独动作
  case 94:
    robot.runSingle(4);
    break;  // 4 号舵机单独动作
  case 95:
    robot.runSingle(5);
    break;  // 5 号舵机单独动作
  case 96:
    robot.runSingle(6);
    break;  // 6 号舵机单独动作
  case 97:
    robot.runSingle(7);
    break;  // 7 号舵机单独动作

  default:
    taken = false;
    manualMode = true;
  }

  if (!manualMode) {
    sendFinalAck();  // 发送完成确认
  }
  if (taken)
    prev_cmd = cmd;  // 记录当前命令
  return taken;
}

/**
 * @brief 暂停函数
 * @param period 暂停时间(毫秒)
 */
void pause(int period) {
  long timeout = millis() + period;
  do {
    robot.refresh();  // 保持刷新状态
  } while (millis() <= timeout);
}

void obstacleDetector() {
  int distance = robot.getDistance();  // 获取距离

  if (distance < 15)  // 如果距离小于15cm
  {
    obstacleDetected = true;  // 设置障碍物标志
  } else {
    obstacleDetected = false;  // 清除障碍物标志
  }
}

/**
 * @brief 停止命令处理函数
 */
void receiveStop() {
  sendAck();       // 发送确认
  robot.home();    // 回到初始位置
  prev_cmd = '.';  // 重置前一个命令
  sendFinalAck();  // 发送完成确认
}

/**
 * @brief 发送确认信号
 */
void sendAck() {
  delay(30);
  Serial.print(F("&&"));    // 开始标记
  Serial.print(F("A"));     // 确认标识
  Serial.println(F("%%"));  // 结束标记
  Serial.flush();
}

/**
 * @brief 发送最终确认信号
 */
void sendFinalAck() {
  delay(30);
  Serial.print(F("&&"));    // 开始标记
  Serial.print(F("F"));     // 完成标识
  Serial.println(F("%%"));  // 结束标记
  Serial.flush();
}

/**
 * @brief 运动命令处理函数
 */
void receiveMovement() {
  sendAck();

  if (robot.getRestState() == true)  // 如果处于休息状态
  {
    robot.setRestState(false);  // 退出休息状态
  }

  robot.clearMouth();  // 清除嘴部显示
  char *arg;
  arg = SerialCmd.next();  // 获取第一个参数(动作ID)
  if (arg != NULL) {
    moveId = atoi(arg);     // 设置动作ID
    robot.putMouth(smile);  // 显示笑脸
  } else {
    robot.putMouth(xMouth);  // 显示X嘴
    delay(2000);
    robot.clearMouth();
    moveId = 0;
  }

  // 获取时间参数
  arg = SerialCmd.next();
  if (arg != NULL) {
    moveMentPeriod = atoi(arg);
  } else {
    moveMentPeriod = 1000;
  }

  // 获取幅度参数
  arg = SerialCmd.next();
  if (arg != NULL) {
    moveSize = atoi(arg);
  } else {
    moveSize = 15;
  }
}

/**
 * @brief LED控制处理函数
 */
void receiveLED() {
  sendAck();
  robot.home();

  unsigned long int matrix;
  char *arg;
  char *endstr;
  arg = SerialCmd.next();  // 获取LED模式参数
  if (arg != NULL) {
    matrix = strtoul(arg, &endstr, 2);  // 二进制字符串转数值
    robot.putMouth(matrix, false);      // 设置嘴部LED
  } else {
    robot.putMouth(xMouth);  // 显示X嘴
    delay(2000);
    robot.clearMouth();
  }

  sendFinalAck();
}

/**
 * @brief 声音控制处理函数
 */
void receiveSing() {
  sendAck();
  robot.home();

  int sing = 0;
  char *arg;
  arg = SerialCmd.next();  // 获取声音编号
  if (arg != NULL)
    sing = atoi(arg);
  else {
    robot.putMouth(xMouth);  // 显示X嘴
    delay(2000);
    robot.clearMouth();
  }

  // 播放对应声音
  // 我们没有蜂鸣器，这里无法播放声音
  // switch (sing) {
  // case 1:
  //   robot.sing(S_connection);
  //   break;
  // case 2:
  //   robot.sing(S_disconnection);
  //   break;
  // case 3:
  //   robot.sing(S_surprise);
  //   break;
  // case 4:
  //   robot.sing(S_OhOoh);
  //   break;
  // case 5:
  //   robot.sing(S_OhOoh2);
  //   break;
  // case 6:
  //   robot.sing(S_cuddly);
  //   break;
  // case 7:
  //   robot.sing(S_sleeping);
  //   break;
  // case 8:
  //   robot.sing(S_happy);
  //   break;
  // case 9:
  //   robot.sing(S_superHappy);
  //   break;
  // case 10:
  //   robot.sing(S_happy_short);
  //   break;
  // case 11:
  //   robot.sing(S_sad);
  //   break;
  // case 12:
  //   robot.sing(S_confused);
  //   break;
  // case 13:
  //   robot.sing(S_fart1);
  //   break;
  // case 14:
  //   robot.sing(S_fart2);
  //   break;
  // case 15:
  //   robot.sing(S_fart3);
  //   break;
  // case 16:
  //   robot.sing(S_mode1);
  //   break;
  // case 17:
  //   robot.sing(S_mode2);
  //   break;
  // case 18:
  //   robot.sing(S_mode3);
  //   break;
  // case 19:
  //   robot.sing(S_buttonPushed);
  //   break;
  // default:
  //   break;
  // }
  sendFinalAck();
}

/**
 * @brief 距离请求处理函数
 */
void requestDistance() {
  robot.home();
  int distance = robot.getDistance();  // 获取距离
  Serial.print(F("&&"));               // 开始标记
  Serial.print(F("D "));               // 距离标识
  Serial.print(distance);              // 距离值
  Serial.println(F("%%"));             // 结束标记
  Serial.flush();
}

/**
 * @brief 电池状态请求处理函数
 */
void requestBattery() {
  robot.home();
  double batteryLevel = robot.getBatteryLevel();  // 获取电池电量

  Serial.print(F("&&"));       // 开始标记
  Serial.print(F("B "));       // 电池标识
  Serial.print(batteryLevel);  // 电量值
  Serial.println(F("%%"));     // 结束标记
  Serial.flush();
}

/**
 * @brief 程序ID请求处理函数
 */
void requestProgramId() {
  robot.home();
  Serial.print(F("&&"));    // 开始标记
  Serial.print(F("I "));    // ID标识
  Serial.print(programID);  // 程序ID
  Serial.println(F("%%"));  // 结束标记
  Serial.flush();
}

/**
 * @brief 模式请求处理函数
 */
void requestMode() {
  sendAck();
  robot.home();
  char *arg;
  arg = SerialCmd.next();  // 获取模式编号
  if (arg != NULL) {
    setMode(atoi(arg));  // 使用新模式设置函数
    robot.putMouth(heart);  // 显示心形
    delay(1000);
    robot.clearMouth();
  } else {
    robot.putMouth(xMouth);  // 显示X嘴
    delay(2000);
    robot.clearMouth();
    setMode(0);  // 如果没有参数则设置为模式0
  }

  // 设置对应模式
  switch (CURRENT_MODE) {
  case 0:
    setMode(0);  // 使用新模式设置函数
    break;       // 模式0: 待机
  case 1:        // 模式1: 随机舞蹈
    setMode(1);  // 使用新模式设置函数
    // robot.sing(S_mode1);
    robot.putMouth(one);
    delay(1000);
    delay(200);
    break;
  case 2:  // 模式2: 避障模式
    setMode(2);
    // robot.sing(S_mode2);
    robot.putMouth(two);
    delay(1000);
    break;
  case 3:  // 模式3: (预留)
    setMode(3);
    // robot.sing(S_mode3);
    robot.putMouth(three);
    delay(1000);
    break;
  case 4:  // 模式4: 手动控制
    // robot.sing(S_mode1);
    robot.putMouth(four);
    delay(1000);
    setMode(4);
    break;
  default:
    setMode(0);
    break;  // 默认模式0
  }
  sendFinalAck();
  robot.clearMouth();
  robot.putMouth(smile);  // 显示笑脸
}

/**
 * @brief 手势控制处理函数
 */
void receiveGesture() {
  sendAck();
  robot.home();

  int gesture = 0;
  char *arg;
  arg = SerialCmd.next();  // 获取手势编号
  if (arg != NULL) {
    gesture = atoi(arg);
  } else {
    robot.putMouth(xMouth);  // 显示X嘴
    delay(2000);
    robot.clearMouth();
  }

  // 执行对应手势
  switch (gesture) {
  case 1:
    robot.playGesture(OttoHappy);
    break;  // 开心手势
  case 2:
    robot.playGesture(OttoSuperHappy);
    break;  // 超级开心手势
  case 3:
    robot.playGesture(OttoSad);
    break;  // 悲伤手势
  case 7:
    robot.playGesture(OttoLove);
    break;  // 爱心手势
  default:
    break;
  }

  sendFinalAck();
}

/**
 * @brief 将浮点数转换为格式化的字符串
 * @param value 要格式化的浮点数
 * @param buffer 输出缓冲区
 * @param bufferSize 缓冲区大小
 */
void formatFloat(float value, char *buffer, int bufferSize) {
  int intPart = (int)value;
  int decPart = abs((int)(value * 100) % 100);
  snprintf(buffer, bufferSize, "%d.%02d", intPart, decPart);
}

/**
 * @brief 校准请求处理函数
 */
void requestCalibration() {
  sendAck();

  if (robot.getRestState() == false)  // 如果不处于休息状态
  {
    robot.setRestState(true);  // 进入休息状态
  }

  // 获取参数： servo ID
  int servoId = 0;
  char *arg = SerialCmd.next();  // 获取参数
  if (arg != NULL) {
    servoId = atoi(arg);  // 转换为整数
    if (servoId < 0 || servoId > 7) {
      servoId = 0;  // 如果超出范围则默认为0
    }
  } else {
    servoId = 0;  // 默认值
  }

  // 获得参数：目标角度
  int targetAngle = 0;  // 默认目标角度
  arg = SerialCmd.next();  // 获取目标角度参数
  if (arg != NULL) {
    targetAngle = atoi(arg);  // 转换为整数
    if (targetAngle < -90 || targetAngle > 90) {
      targetAngle = 0;  // 如果超出范围则默认为0
    }
  } else {
    targetAngle = 0;  // 默认值
  }

  robot.setTrim(servoId, targetAngle);  // 保存偏移值
  // debug
  char debugBuffer[20];
  snprintf(debugBuffer, sizeof(debugBuffer), "CS %d to %d", servoId, targetAngle);
  debug(debugBuffer);  // 显示校准信息
  robot.home();  // 回到初始位置
  delay(2000);

  robot.storeTrim();  // 保存偏移值到EEPROM

  sendFinalAck();
}
