#include "time.h"
#include <NTPClient.h>
#include <WiFiManager.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
bool res;

//LED pins
const byte hoursPin10 = 48;
const byte hoursPin20 = 35;

const byte hoursPin1 = 13;
const byte hoursPin2 = 14;
const byte hoursPin4 = 21;
const byte hoursPin8 = 47;

const byte minutesPin10 = 10;
const byte minutesPin20 = 11;
const byte minutesPin40 = 12;

const byte minutesPin1 = 18;
const byte minutesPin2 = 8;
const byte minutesPin4 = 3;
const byte minutesPin8 = 9;

const byte secondsPin10 = 15;
const byte secondsPin20 = 16;
const byte secondsPin40 = 17;


const byte secondsPin1 = 4;
const byte secondsPin2 = 5;
const byte secondsPin4 = 6;
const byte secondsPin8 = 7;

//nc LEDS
//array[]

// Button Input(s)
const byte resetWifi = 42;


void setup() {
  // Start Serial Communication
  WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP

  Serial.begin(115200);
  // Wifi manager setup
  WiFiManager wm;
  wm.setConnectTimeout(5);
  res = wm.autoConnect("Bianary_Clock");  // anonymous ap
  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  //NPT Sever Setup
  timeClient.begin();
  timeClient.setTimeOffset(-18000);
  //printLocalTime();

  //Brightness PWM Pin gate control of P-Channel mosfet
  ledcSetup(0, 1000, 12);
  ledcAttachPin(36, 0);

  //Button PIN Assignments(s)
  pinMode(resetWifi, INPUT_PULLUP);

  //Pin Modes For Leds
  pinMode(hoursPin10, OUTPUT);
  pinMode(hoursPin20, OUTPUT);
  pinMode(hoursPin1, OUTPUT);
  pinMode(hoursPin2, OUTPUT);
  pinMode(hoursPin4, OUTPUT);
  pinMode(hoursPin8, OUTPUT);
  pinMode(minutesPin10, OUTPUT);
  pinMode(minutesPin20, OUTPUT);
  pinMode(minutesPin40, OUTPUT);
  pinMode(minutesPin1, OUTPUT);
  pinMode(minutesPin2, OUTPUT);
  pinMode(minutesPin4, OUTPUT);
  pinMode(minutesPin8, OUTPUT);
  pinMode(secondsPin10, OUTPUT);
  pinMode(secondsPin20, OUTPUT);
  pinMode(secondsPin40, OUTPUT);
  pinMode(secondsPin1, OUTPUT);
  pinMode(secondsPin2, OUTPUT);
  pinMode(secondsPin4, OUTPUT);
  pinMode(secondsPin8, OUTPUT);
}

void loop() {

  serialCOM();

  brightness();

  displayLights();

  WiFiManager wm;

  if (digitalRead(resetWifi) == LOW)
    wm.resetSettings();

  delay(50);
}

void serialCOM() {

  timeClient.update();

  int Hours = timeClient.getHours();
  Serial.print("Hour:  ");
  Serial.println(Hours);
  int Minutes = timeClient.getMinutes();
  Serial.print("Minute:  ");
  Serial.println(Minutes);
  int Seconds = timeClient.getSeconds();
  Serial.print("Second: ");
  Serial.println(Seconds);
  Serial.println();
}


//Brightness Control

void brightness() {
  const byte photoPin = 1;
  int brightVal;
  int brightVal2;
  brightVal = analogRead(photoPin);
  brightVal2 = brightVal;
  Serial.println(brightVal);
  brightVal = map(brightVal, 4095, 1000, 0, 4095);
  Serial.println(brightVal);
  if (brightVal2 > 1000) {
    ledcWrite(0, brightVal);
  } else {
    ledcWrite(0, 4095);
  }
}


//----- Display Function -----

void displayLights() {
  timeClient.update();
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  if (hours >= 20) {
    digitalWrite(hoursPin20, LOW);
    digitalWrite(hoursPin10, HIGH);
  } else if (hours >= 10) {
    digitalWrite(hoursPin20, HIGH);
    digitalWrite(hoursPin10, LOW);
  } else {
    digitalWrite(hoursPin20, HIGH);
    digitalWrite(hoursPin10, HIGH);
  }

  int hoursUnit = hours % 10;
  if (hoursUnit >= 8) {
    digitalWrite(hoursPin8, LOW);
    digitalWrite(hoursPin4, HIGH);
    digitalWrite(hoursPin2, HIGH);
  } else if (hoursUnit >= 6) {
    digitalWrite(hoursPin8, HIGH);
    digitalWrite(hoursPin4, LOW);
    digitalWrite(hoursPin2, LOW);
  } else if (hoursUnit >= 4) {
    digitalWrite(hoursPin8, HIGH);
    digitalWrite(hoursPin4, LOW);
    digitalWrite(hoursPin2, HIGH);
  } else if (hoursUnit >= 2) {
    digitalWrite(hoursPin8, HIGH);
    digitalWrite(hoursPin4, HIGH);
    digitalWrite(hoursPin2, LOW);
  } else {
    digitalWrite(hoursPin8, HIGH);
    digitalWrite(hoursPin4, HIGH);
    digitalWrite(hoursPin2, HIGH);
  }
  if (hoursUnit % 2 == 1) {
    digitalWrite(hoursPin1, LOW);
  } else {
    digitalWrite(hoursPin1, HIGH);
  }







  if (minutes >= 40) {
    digitalWrite(minutesPin40, LOW);
    digitalWrite(minutesPin20, HIGH);
    if (minutes >= 50) {
      digitalWrite(minutesPin10, LOW);
    } else {
      digitalWrite(minutesPin10, HIGH);
    }
  } else if (minutes >= 20) {
    digitalWrite(minutesPin40, HIGH);
    digitalWrite(minutesPin20, LOW);
    if (minutes >= 30) {
      digitalWrite(minutesPin10, LOW);
    } else {
      digitalWrite(minutesPin10, HIGH);
    }
  } else if (minutes >= 10) {
    digitalWrite(minutesPin40, HIGH);
    digitalWrite(minutesPin20, HIGH);
    digitalWrite(minutesPin10, LOW);
  } else {
    digitalWrite(minutesPin40, HIGH);
    digitalWrite(minutesPin20, HIGH);
    digitalWrite(minutesPin10, HIGH);
  }

  int minutesUnit = minutes % 10;
  if (minutesUnit >= 8) {
    digitalWrite(minutesPin8, LOW);
    digitalWrite(minutesPin4, HIGH);
    digitalWrite(minutesPin2, HIGH);
  } else if (minutesUnit >= 6) {
    digitalWrite(minutesPin8, HIGH);
    digitalWrite(minutesPin4, LOW);
    digitalWrite(minutesPin2, LOW);
  } else if (minutesUnit >= 4) {
    digitalWrite(minutesPin8, HIGH);
    digitalWrite(minutesPin4, LOW);
    digitalWrite(minutesPin2, HIGH);
  } else if (minutesUnit >= 2) {
    digitalWrite(minutesPin8, HIGH);
    digitalWrite(minutesPin4, HIGH);
    digitalWrite(minutesPin2, LOW);
  } else {
    digitalWrite(minutesPin8, HIGH);
    digitalWrite(minutesPin4, HIGH);
    digitalWrite(minutesPin2, HIGH);
  }
  if (minutesUnit % 2 == 1) {
    digitalWrite(minutesPin1, LOW);
  } else {
    digitalWrite(minutesPin1, HIGH);
  }








  if (seconds >= 40) {
    digitalWrite(secondsPin40, LOW);
    digitalWrite(secondsPin20, HIGH);
    if (seconds >= 50) {
      digitalWrite(secondsPin10, LOW);
    } else {
      digitalWrite(secondsPin10, HIGH);
    }
  } else if (seconds >= 20) {
    digitalWrite(secondsPin40, HIGH);
    digitalWrite(secondsPin20, LOW);
    if (seconds >= 30) {
      digitalWrite(secondsPin10, LOW);
    } else {
      digitalWrite(secondsPin10, HIGH);
    }
  } else if (seconds >= 10) {
    digitalWrite(secondsPin40, HIGH);
    digitalWrite(secondsPin20, HIGH);
    digitalWrite(secondsPin10, LOW);
  } else {
    digitalWrite(secondsPin40, HIGH);
    digitalWrite(secondsPin20, HIGH);
    digitalWrite(secondsPin10, HIGH);
  }

  int secondsUnit = seconds % 10;
  if (secondsUnit >= 8) {
    digitalWrite(secondsPin8, LOW);
    digitalWrite(secondsPin4, HIGH);
    digitalWrite(secondsPin2, HIGH);
  } else if (secondsUnit >= 6) {
    digitalWrite(secondsPin8, HIGH);
    digitalWrite(secondsPin4, LOW);
    digitalWrite(secondsPin2, LOW);
  } else if (secondsUnit >= 4) {
    digitalWrite(secondsPin8, HIGH);
    digitalWrite(secondsPin4, LOW);
    digitalWrite(secondsPin2, HIGH);
  } else if (secondsUnit >= 2) {
    digitalWrite(secondsPin8, HIGH);
    digitalWrite(secondsPin4, HIGH);
    digitalWrite(secondsPin2, LOW);
  } else {
    digitalWrite(secondsPin8, HIGH);
    digitalWrite(secondsPin4, HIGH);
    digitalWrite(secondsPin2, HIGH);
  }
  if (secondsUnit % 2 == 1) {
    digitalWrite(secondsPin1, LOW);
  } else {
    digitalWrite(secondsPin1, HIGH);
  }
}
