#include <BLEDevice.h>
#include <BLEServer.h>

#include <BLEUtils.h>
// #include <BLE_Carousel.h>
#include <BLE2902.h>
#include <iostream>
#include <algorithm>
#include <ArduinoJson.h>
using namespace std;

#include <String.h>
#include <DS3231.h>
#include <Wire.h>
#define RTC_Line 21

DS3231 myRTC;

#include <Time.h>

#define mainService "19B10010-E8F2-537E-4F6C-D104768A1214"
#define bedtimeService "19B10011-E8F2-537E-4F6C-D104768A1214"
#define luxService "19B10012-E8F2-537E-4F6C-D104768A1214"

const int Touch_Pin = 12;
const int Photo_Pin = 14;
const int Buzzer_Pin = 26;


bool h12;
bool hPM;
unsigned int hours;
bool toggleRun;
unsigned long startTime, endTime, intervalStartTime, intervalEndTime, hourStartTime, hourEndTime;

void setTime(){
  myRTC.setYear(24);
  myRTC.setMonth(2);
  myRTC.setDate(17);
  myRTC.setDoW(7);
  myRTC.setHour(21);
  myRTC.setMinute(36);
  myRTC.setSecond(10);
  myRTC.setClockMode(false);
}

int readTime(){
  bool h12, hPM;
  return myRTC.getHour(h12,hPM);
  // unsigned int minute = myRTC.getMinute();
  // unsigned int sec = myRTC.getSecond();
  // Serial.print("Time is ");
  // Serial.print(hour);
  // Serial.println(" hours");
  // Serial.print(":");
  // Serial.print(minute);
  // Serial.print(":");
  // Serial.println(sec);
}

class pBedtimeCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      Serial.println(rxValue);
      if ((rxValue.length() > 0) && (rxValue.find ('&') == std::string::npos)) {
        Serial.println("*********");
      }
    }
}; //end of callback

class pLuxCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      Serial.println(rxValue);
      if ((rxValue.length() > 0) && (rxValue.find ('&') == std::string::npos)) {
        Serial.println("*********");
      }
    }
}; //end of callback


void start_up(){
  // Serial.println(touchRead(Touch_Pin));
  // readTime();
  toggleRun = true;
  startTime = millis();
}
const int threshold = 17;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  
  hours = readTime();
  startTime  = millis();
  intervalStartTime = millis();
  hourStartTime = millis();

  BLEDevice::init("ESP32_SleeperBuild");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(mainService);
  BLECharacteristic *pCharacteristicBedtime = pService->createCharacteristic(bedtimeService, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  BLECharacteristic *pCharacteristicLux = pService->createCharacteristic(luxSercive, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY );
  pCharacteristicBedtime->setCallbacks(new pBedtimeCallbacks());
  pCharacteristicLux->setCallbacks(new pLuxCallbacks());

  pService->start();
  pAdvertising->setScanResponse(true);

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(mainService);
  pAdvertising->setScanResponse(true);
  
  BLEDevice::startAdvertising();

  pinMode(Photo_Pin, INPUT);
  pinMode(Buzzer_Pin,OUTPUT);
  // setTime();

  touchAttachInterrupt(Touch_Pin, start_up, threshold);
}

#define min_night 19
#define night_day_switch 4
#define max_day 7
#define timeout 5
#define sendDataTime 1
#define buzzerLux 1000

//bedtime set, and target_lux

int brightCount = 0;
void loop() {
  // put your main code here, to run repeatedly:

  // Serial.println(hours);
  // Serial.println(toggleRun);
  endTime = (millis() - startTime)/1000;
  if(intervalEndTime >= 1) intervalStartTime = millis();
  // Serial.println(intervalEndTime);
    
  if((hours > min_night || hours < night_day_switch) && (toggleRun && endTime < timeout)){
    //bright code start?
    int light = analogRead(Photo_Pin);
    // Serial.println(light);

    float v_light = ((light)/4095.0) * 3.3;
    float current = v_light / 5000.0;
    float res_photo = (3.3 - v_light)/current;
    float lux = (1.25*pow(10,7))*pow(res_photo,-1.4059);
    // Serial.println(lux);
    
    intervalEndTime = (millis() - intervalStartTime)/1000;
    // Serial.println(intervalEndTime);
    if(lux > 200 && intervalEndTime >= 1){
      brightCount += lux;
      hourEndTime = (millis() - hourStartTime)/1000;
      if(hourEndTime >= sendDataTime){
        //send data after sendDataTime seconds
        // Serial.println("Send data to array");
        hourStartTime = millis();
      }
    }

  }else if((hours > night_day_switch || hours < max_day) && (toggleRun && endTime < timeout)){
    int light = analogRead(Photo_Pin);
    // Serial.println(light);

    float v_light = ((light)/4095.0) * 3.3;
    float current = v_light / 5000.0;
    float res_photo = (3.3 - v_light)/current;
    float lux = (1.25*pow(10,7))*pow(res_photo,-1.4059);
    // Serial.println(lux);
    
    intervalEndTime = (millis() - intervalStartTime)/1000;
    // Serial.println(intervalEndTime);
    if(lux > 200 && intervalEndTime >= 1) {
      brightCount += lux;
      hourEndTime = (millis() - hourStartTime)/1000;
      if(hourEndTime >= sendDataTime){
        //send data after sendDataTime seconds
        // Serial.println("Send data to array");
        hourStartTime = millis();
      }
    }
  }else{
    hours = readTime();
    toggleRun = false;
    delay(100);
    brightCount = 0;
  }

  // Serial.println(brightCount);
  if(brightCount >= buzzerLux){
    //activate buzzer
    digitalWrite(Buzzer_Pin,HIGH);
  }else digitalWrite(Buzzer_Pin,LOW);

}
