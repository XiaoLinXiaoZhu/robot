## 这是什么？

这里用于记录一些代码片段。作为备忘录，可以随时查看。

## 使用元件型号

主板型号：Arduino UNO
[Arduino UNO](https://www.arduino.cc/en/Guide/ArduinoUno)
拓展版型号：Gravity: IO Expansion & Motor Driver Shield
[Gravity: IO Expansion & Motor Driver Shield](https://wiki.dfrobot.com.cn/_SKU_DFR0502__Gravity__IO_Expansion_%26_Motor_Driver_Shield)
显示器型号：SH1106_128X32_VISIONOX_HW_I2C

## 问题解决

### 为什么我的Arduino UNO 在上传代码之后能正常工作但是无法通过USB连接到电脑？

因为你已经成功上传过代码，所以说你的电脑这一部分其实是正常的。

问题出在Arduino UNO 上，当机器人尝试控制电机运动，但是点击没能成功移动（比如说被结构阻挡），这个时候Arduino UNO 会出现故障，导致 IO 口无法正常工作。

解决方法是避免电机被阻挡，不要将其直接放到地上，而是使用支架将其抬高，并且即检查电机的偏移量是否正确。  


## 端口功能

### 普通针脚操作功能

#### 设置工作状态

```c++
pinMode(PIN_INDEX, OUTPUT);
// 设置为输出模式,之后可以使用digitalWrite()函数来控制引脚的电平状态。
digitalWrite(PIN_INDEX, LOW);
```

```c++
pinMode(PIN_INDEX, INPUT);
// 设置为输入模式,之后可以使用digitalRead()函数来读取引脚的电平状态。
bool value = digitalRead(PIN_INDEX);
// value将会是HIGH或LOW，表示引脚的电平状态。
```

#### 读写功能

读写之前需要先设置引脚的工作状态。

```c++
digitalWrite(PIN_INDEX, HIGH);
// 将引脚设置为高电平
digitalWrite(PIN_INDEX, LOW);
// 将引脚设置为低电平
```

```c++
bool value = digitalRead(PIN_INDEX);
// 读取引脚的电平状态
// value将会是HIGH或LOW，表示引脚的电平状态。
```

