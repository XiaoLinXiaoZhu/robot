#ifndef OTTOkame_h
#define OTTOkame_h
#include <Servo.h>
#include "Octosnake.h"
#include "US.h"
//! #include "MaxMatrix.h"
#include <BatReader.h>
// servo index to board_pins
#define FRONT_RIGHT_HIP 0
#define FRONT_LEFT_HIP 1
#define FRONT_RIGHT_LEG 2
#define FRONT_LEFT_LEG 3
#define BACK_RIGHT_HIP 4
#define BACK_LEFT_HIP 5
#define BACK_RIGHT_LEG 6
#define BACK_LEFT_LEG 7
//#define PIN_Buzzer  13
#define PIN_Trigger 12
#define PIN_Echo    11

#define zero_code                  0b00001100010010010010010010001100
#define one_code                   0b00000100001100000100000100001110
#define two_code                   0b00001100010010000100001000011110
#define three_code                 0b00001100010010000100010010001100
#define four_code                  0b00010010010010011110000010000010
#define five_code                  0b00011110010000011100000010011100
#define six_code                   0b00000100001000011100010010001100
#define seven_code                 0b00011110000010000100001000010000
#define eight_code                 0b00001100010010001100010010001100
#define nine_code                  0b00001100010010001110000010001110

#define smile_code                 0b00000000100001010010001100000000
#define happyOpen_code             0b00000000111111010010001100000000 
#define happyClosed_code       0b00000000111111011110000000000000
#define heart_code                 0b00010010101101100001010010001100

#define bigSurprise_code           0b00001100010010100001010010001100
#define smallSurprise_code         0b00000000000000001100001100000000
#define tongueOut_code             0b00111111001001001001000110000000
#define vamp1_code                 0b00111111101101101101010010000000
#define vamp2_code                 0b00111111101101010010000000000000
#define lineMouth_code             0b00000000000000111111000000000000
#define confused_code              0b00000000001000010101100010000000
#define diagonal_code              0b00100000010000001000000100000010
#define sad_code                 0b00000000001100010010100001000000
#define sadOpen_code           0b00000000001100010010111111000000
#define sadClosed_code         0b00000000001100011110110011000000
#define okMouth_code               0b00000001000010010100001000000000
#define xMouth_code                0b00100001010010001100010010100001
#define interrogation_code         0b00001100010010000100000100000100
#define thunder_code               0b00000100001000011100001000010000
#define culito_code                0b00000000100001101101010010000000 
#define angry_code                 0b00000000011110100001100001000000


//Mouths sorted by numbers, and after, by happy to sad mouths
#define zero        0
#define one         1                
#define two           2               
#define three               3  
#define four                4
#define five                5 
#define six                 6 
#define seven               7  
#define eight               8 
#define nine          9
#define smile               10                  
#define happyOpen         11  
#define happyClosed       12  
#define heart           13
#define bigSurprise         14  
#define smallSurprise     15
#define tongueOut       16
#define vamp1               17  
#define vamp2               18  
#define lineMouth         19
#define confused            20  
#define diagonal          21          
#define sad             22
#define sadOpen           23 
#define sadClosed         24
#define okMouth             25 
#define xMouth              26
#define interrogation       27
#define thunder           28
#define culito          29
#define angry         30  
#define OttoHappy     0
#define OttoSuperHappy  1
#define OttoSad     2
#define OttoSleeping  3
#define OttoFart    4
#define OttoConfused  5
#define OttoLove    6
#define OttoAngry     7
#define OttoFretful   8
#define OttoMagic     9
#define OttoWave    10
#define OttoVictory   11
#define OttoFail    12

//*** MOUTH ANIMATIONS***
#define littleUuh   0
#define dreamMouth    1   
#define adivinawi   2
#define wave      3


//#define PIN_NoiseSensor A6
extern "C" void pause(int);

class MiniKame {

public:
  MiniKame();
  void init(int Buzzer);
  void home();
  //-- Sensors functions
  float getDistance(); //US sensor
  int getNoise();      //Noise Sensor

  //-- Battery
  double getBatteryLevel();
  double getBatteryVoltage();
  void run(int dir = 1, float steps = 4, float T = 550);
  void runSingle(int id = 0, int dir = 1, float steps = 4, float T = 550);
  void walk(int dir = 1, float steps = 4, float T = 550);
  void turnL(float steps = 1, float period = 550);
  void turnR(float steps = 1, float period = 550);
  void omniWalk(bool side = true, float T = 1000, float turn_factor = 2);
  void moonwalkL(float steps = 10, float period = 2000);
  void dance(float steps = 1, float period = 2000);
  void upDown(float steps = 1, float period = 500);
  void waveHAND(float steps = 1, float period = 700);
  void Hide(float steps = 1, float period = 700);
  void pushUp(float steps = 1, float period = 5000);
  void frontBack(float steps = 1, float period = 2000);
  void reverseServo(int id);
  void hello();
  void jump();
  void scared();
  void moveServos(int time, float target[8]);
  void setServo(int id, float target);
  void setTrim(int index, int value) {
    trim[index] = value;
  }
  bool getRestState();
  void setRestState(bool state);


  void refresh();
  void storeTrim();
  void loadTrim();
  //! MaxMatrix ledmatrix=MaxMatrix(A1,A2,A3, 1);
  void putMouth(unsigned long int mouth, bool predefined = true);
  void putAnimationMouth(unsigned long int anim, int index);
  void clearMouth();
  //-- Gestures
  void playGesture(int gesture);
  void attachServo();
  void detachServo();
  //-- Sounds
  void _tone(float noteFrequency, long noteDuration, int silentDuration);
  void bendTones(float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration);
  void sing(int songName);


private:
  Oscillator oscillator[8];
  Servo servo[8];
  uint8_t board_pins[8];
  int8_t trim[8]; //deviation servo offset
  //unsigned long _init_time;
  // unsigned long _final_time;
  bool isOttoResting;
  void execute(float steps, float period[8], int8_t amplitude[8], int8_t offset[8], int8_t phase[8]);

  int EEPROMReadWord(int p_address);
  void EEPROMWriteWord(int p_address, int p_value);
  //-- Mouth & Animations
  bool reverse[8];
  unsigned long int getMouthShape(int number);
  unsigned long int getAnimShape(int anim, int index);
  US us;
  int pinBuzzer;
  int pinNoiseSensor;
  BatReader battery;


};

#endif
