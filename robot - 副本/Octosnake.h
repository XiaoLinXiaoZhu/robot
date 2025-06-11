#ifndef octosnake_h
#define octosnake_h
#include <Arduino.h>
#include <Servo.h>

#ifndef PI
#define PI 3.14159
#endif


class Oscillator {

public:
  Oscillator();
  float update();
  void reset();
  void start();
  void start(unsigned long ref_time);
  void stop();
  boolean isStop();
  float time_to_radians(double time);
  float degrees_to_radians(float degrees);
  float degrees_to_time(float degrees);
  void setPeriod(int period);
  int getPeriod();
  void setAmplitude(int amplitude);
  void setPhase(int phase);
  void setOffset(int offset);
  void setTime(unsigned long ref);
  float getOutput();
  float getPhaseProgress();
  unsigned long getTime();

private:
  int16_t _period;
  int8_t _amplitude;
  int8_t _phase;
  int8_t _offset;
  float _output;
  bool _stop;
  unsigned long _ref_time = 0;
  float _delta_time = 0;
};

#endif
