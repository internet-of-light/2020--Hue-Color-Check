#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

WiFiClient espClient;

const char* ssid = "University of Washington"; // Wifi network SSID
const char* password = ""; // Wifi network password

String ip = "172.28.219.179"; // Sieg Master IP
String api_token = "rARKEpLebwXuW01cNVvQbnDEkd2bd56Nj-hpTETB"; // Sieg Master API Token

//Sieg lower floor
int lowerLights[] = {10, 23, 11, 15, 7, 14, 22, 21, 16};
int upperLights[] = {18, 20, 12, 25, 26, 5, 8, 19, 13, 24, 9, 17};

typedef struct {
  String col;
  String pot;
} randomValues;

randomValues rv;

//POTENTIOMETER SETTINGS
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;  // value read from the pot
int outputValue = 0;  // value to output to a PWM pin

//BUTTON SETTINGS
const int buttonPin = 16;
int buttonState = 0;

unsigned long currentTime, lastPattern, timerOne, timerTwo; //timing

bool debug = true;

void setup() {
  // initialize serial communication at 115200
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  setup_wifi();
}

void loop() {
  currentTime = millis(); //Update time 
  potCheck();
  if (currentTime - timerOne > 2000) {
    buttonCheck();
  }
}

void setLights(){
  for(int i = 0; i < 9; i++) {
        changeLight(lowerLights[i], 0, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
  }
}

void buttonCheck(){
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("ON");
    setLights();
    timerOne = currentTime;
  } else {
    Serial.println("OFF");
  }
}

void potCheck(){
  sensorValue = analogRead(analogInPin);
  // map it to the range of the PWM out
  outputValue = map(sensorValue, 0, 1024, 0, 65280);
  // print the readings in the Serial Monitor
  rv.pot = String(sensorValue);
  rv.col = String(outputValue);
  Serial.print("color value = ");
  Serial.println(rv.col);
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Connected to the WiFi network");
}
