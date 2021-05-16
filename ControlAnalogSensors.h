#pragma once
#include <Arduino.h>

//библиотека для работы с сенсорами через аналоговые и цифровые пины по опр. эл. схеме.
//макс кол-во сенсоров = подкл аналоговые пины * подкл цифровые пины
class ControlAnalogSensors {

  public:
    ControlAnalogSensors(byte *digitalPins, byte *analogPins, byte countDigitalPins, byte countAnalogPins);
    byte getResult(byte sensor);
    byte getCountDigitalPins();
    byte getCountAnalogPins();
    byte getMaxCountSensor();
    byte getCountConnectSensor();
    bool isConnectSensor(byte sensor);
    unsigned int getSettingSensor(byte sensor);
    unsigned int getValueSetting(byte sensor);
    unsigned int getTimeWait();
    void editSettingSensor(byte sensor, unsigned int setting);
    void editTimeWait(unsigned int timeWait);

  private:
    byte _countDigitalPins, _countAnalogPins, _maxCountSensor;
    unsigned int _timeWait = 1;
    byte *_digitalPins, *_analogPins;
    unsigned int *_setting;
};

ControlAnalogSensors::ControlAnalogSensors(byte *digitalPins, byte *analogPins, byte countDigitalPins, byte countAnalogPins) {
  _countDigitalPins = countDigitalPins;
  _countAnalogPins = countAnalogPins;
  _maxCountSensor = countAnalogPins * countDigitalPins;
  _setting = new unsigned int[_maxCountSensor];
  _digitalPins = new byte[countDigitalPins];
  _analogPins = new byte[countAnalogPins];
  for (byte index = 0; index < countDigitalPins; index++) {
    _digitalPins[index] = digitalPins[index];
    pinMode(digitalPins[index], OUTPUT);
  }
  for (byte index = 0; index < countAnalogPins; index++) {
    _analogPins[index] = analogPins[index];
    pinMode(analogPins[index], INPUT);
  }
  for (byte index = 0; index < _maxCountSensor; index++) {
    _setting[index] = 0;
  }
}

byte ControlAnalogSensors::getResult(byte sensor) {
  byte pin1 = _analogPins[(byte)floor(sensor / _countDigitalPins)];
  byte pin2 = _digitalPins[sensor % _countDigitalPins];
  digitalWrite(pin2, HIGH);
  delay(_timeWait);
  unsigned int input = analogRead(pin1);
  digitalWrite(pin2, LOW);
  byte result = input > _setting[sensor] ? round(sqrt(((input - _setting[sensor]) / (1023.0 - _setting[sensor]))) * 100.0) : 0;
  return result;
}

bool ControlAnalogSensors::isConnectSensor(byte sensor) {
  bool result = 0;
  byte pin1 = _analogPins[(byte)floor(sensor / _countDigitalPins)];
  if (analogRead(pin1) == 0) {
    byte pin2 = _digitalPins[sensor % _countDigitalPins];
    digitalWrite(pin2, HIGH);
    delay(_timeWait);
    result = analogRead(pin1) != 0;
    digitalWrite(pin2, LOW);
  }
  return result;
}

byte ControlAnalogSensors::getCountConnectSensor() {
  byte countConnect = 0;
  for (byte sensor = 1; sensor <= _maxCountSensor; sensor++) {
    countConnect += isConnectSensor(sensor);
  }
  return countConnect;
}

unsigned int ControlAnalogSensors::getValueSetting(byte sensor) {
  byte pin1 = _analogPins[(byte)floor(sensor / _countDigitalPins)];
  byte pin2 = _digitalPins[sensor % _countDigitalPins];
  digitalWrite(pin2, HIGH);
  unsigned int result = analogRead(pin1);
  digitalWrite(pin2, LOW);
  return result;
}

byte ControlAnalogSensors::getCountDigitalPins() {
  return _countDigitalPins;
}

byte ControlAnalogSensors::getCountAnalogPins() {
  return _countAnalogPins;
}

byte ControlAnalogSensors::getMaxCountSensor() {
  return _maxCountSensor;
}

unsigned int ControlAnalogSensors::getSettingSensor(byte sensor) {
  return _setting[sensor];
}

unsigned int ControlAnalogSensors::getTimeWait() {
  return _timeWait;
}

void ControlAnalogSensors::editSettingSensor(byte sensor, unsigned int setting) {
  _setting[sensor] = setting;
}

void ControlAnalogSensors::editTimeWait(unsigned int timeWait) {
  _timeWait = timeWait;
}
