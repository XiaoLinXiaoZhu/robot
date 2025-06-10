// #include <setjmp.h>
// jmp_buf jump_env;                                                // 跳转环境(用于异常处理)

#include "OTTOKame.h"
// #include "OLED.h"
#include "OttoSerialCommand.h"
#include "IDebug.h"

const char programID[] = "Otto_KAME7";                            // 程序标识符

#ifdef VSCODE
#include "HardwareSerial0.cpp"  // VSCode环境下需要指明硬件串口实现
#endif

//- 硬件引脚定义
#define CAL_TRIGGER_PIN 10          // 校准触发引脚
#define LED_PIN 13                  // LED指示灯引脚
#define PIN_Buzzer 13               // 蜂鸣器引脚
#define TIME_INTERVAL 5000          // 时间间隔常量(毫秒)

MiniKame robot;                                                   // 机器人控制对象
bool obstacleDetected = false;                                    // 障碍物检测标志

//- 运动参数
int moveMentPeriod = 1000;                                                     // 动作时间参数
int moveId = 0;                                                   // 当前动作ID
int moveSize = 15;                                                // 动作幅度参数

//- 自定义指令
OttoSerialCommand SerialCmd;  // 串口命令处理对象
static char prev_cmd = '.';                                        // 上一个命令字符

int randomDance = 0;  // 随机舞蹈编号
// 使用一个 uint8_t 来同时保存 上次的模式和当前模式

// 低4位表示当前模式，高4位表示上次模式
//- 模式定义
/**
 * @brief 机器人模式ID，前四位表示上次模式，后四位表示当前模式
 * @details 例如：0x01 表示上次模式为 0，当前模式为 1
 */
volatile uint8_t _ROBOT_MODE = 0B00000000;     // 上次模式和当前模式的组合
#define CURRENT_MODE (_ROBOT_MODE & 0x0F)              // 当前模式(低4位)
#define PREV_MODE ((_ROBOT_MODE >> 4) & 0x0F)  // 上次模式(高4位)
#define DEFAULT_MODE 4                           // 默认模式

//-====================== 函数声明 =========================

//- 定义 SerialCmd 命令处理函数
void receiveStop();
void receiveLED();
void receiveMovement();
void receiveGesture();
void receiveSing();
void requestDistance();
void requestProgramId();
void requestMode();
void requestCalibration();
void receiveStop();

/**
 * @brief 执行步态动作
 * @param cmd 动作指令编号
 * @return bool 是否成功执行
 */
bool gaits(int);

/**
 * @brief 检测障碍物
 */
void detectObstacle();

//------------------
/**
 * @brief 设置机器人模式
 * @param modeId 模式ID
 */
void setMode(int newMode) {
    switch (newMode) {
    case 0:
        debug("CM: Standby");
        break;
    case 1:
        debug("CM: Random Dance");
        break;
    case 2:
        debug("CM: Obstacle Avoidance");
        break;
    case 3:
        debug("CM: Reserved");
        break;
    case 4:
        debug("CM: Manual Control");
        break;
    default:
        debug("CM: Unknown");
        break;
    }
    _ROBOT_MODE = (_ROBOT_MODE & 0x0F) | (newMode & 0x0F);
}
