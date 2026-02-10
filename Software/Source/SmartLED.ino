#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>

#include "index.h"

const char* ssid     = "HUAWEI Mate 10 Pro";
const char* password = "59befa2de9a3";

#define LED_PIN     4     
#define RADAR_PIN   18   
#define DHT_PIN     21  
#define LDR_PIN     36    

#define NUM_PIXELS  60   
#define DHT_TYPE    DHT22

Adafruit_NeoPixel pixels(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
DHT dht(DHT_PIN, DHT_TYPE);
WebServer server(80);

const unsigned long CZAS_SWIECENIA = 10000;
unsigned long ostatniRuchCzas = 0;
unsigned long ostatniOdczytSensorow = 0;

bool trybAuto = true;
bool manualState = false;
uint32_t manualColor = 0;
int manualBrightness = 50;

float temperatura = 0.0;
int jasnoscLDR = 0;
bool wykrytoRuch = false;

void setup() {
  Serial.begin(9600);

  pinMode(RADAR_PIN, INPUT_PULLDOWN);
  pinMode(LDR_PIN, INPUT);

  dht.begin();
  pixels.begin();
  pixels.setBrightness(100); 
  pixels.show();

  Serial.print("Łączenie z WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPołączono!");
  Serial.print("Adres IP strony: http://");
  Serial.println(WiFi.localIP());

  
  server.on("/", []() {
    server.send(200, "text/html", index_html);
  });

  server.on("/status", []() {
    String json = "{";
    json += "\"temp\":" + String(temperatura) + ",";
    json += "\"ldr\":" + String(jasnoscLDR) + ",";
    json += "\"radar\":" + String(wykrytoRuch) + ",";
    json += "\"modeAuto\":" + String(trybAuto);
    json += "}";
    server.send(200, "application/json", json);
  });

  server.on("/set", handleControl);

  server.begin();
}

void loop() {
  server.handleClient(); 
  
  unsigned long aktualnyCzas = millis();
  
  if (aktualnyCzas - ostatniOdczytSensorow > 2000) {
    float t = dht.readTemperature();
    if (!isnan(t)) temperatura = t;
    
    jasnoscLDR = analogRead(LDR_PIN);
    
    wykrytoRuch = (digitalRead(RADAR_PIN) == HIGH);
    
    ostatniOdczytSensorow = aktualnyCzas;
  }

  if (trybAuto) {
    logikaAutomatyczna(aktualnyCzas);
  } else {
    logikaManualna();
  }
}

void logikaAutomatyczna(unsigned long czas) {
  int stanRadaru = digitalRead(RADAR_PIN);

  if (stanRadaru == HIGH) {
    ostatniRuchCzas = czas;
  }

  if (czas - ostatniRuchCzas < CZAS_SWIECENIA) {
    
    uint32_t kolor;
    if (temperatura < 20.0)      kolor = pixels.Color(0, 0, 255); 
    else if (temperatura > 26.0) kolor = pixels.Color(255, 0, 0); 
    else                         kolor = pixels.Color(0, 255, 0); 

    int jasnoscPWM = map(jasnoscLDR, 0, 4095, 255, 10);
    jasnoscPWM = constrain(jasnoscPWM, 10, 255);

    pixels.setBrightness(jasnoscPWM);
    fillStrip(kolor);
    
  } else {
    pixels.clear();
    pixels.show();
  }
}

void logikaManualna() {
  pixels.setBrightness(manualBrightness);

  if (manualState) {
    if (manualColor == 0) manualColor = pixels.Color(255, 255, 255);
    fillStrip(manualColor);
  } else {
    pixels.clear();
    pixels.show();
  }
}

void handleControl() {
  if (server.hasArg("mode")) {
    String m = server.arg("mode");
    if (m == "auto") trybAuto = true;
    if (m == "manual") trybAuto = false;
  }
  
  if (server.hasArg("manual")) {
    manualState = (server.arg("manual").toInt() == 1);
  }

  if (server.hasArg("brightness")) {
    manualBrightness = server.arg("brightness").toInt();
  }

  if (server.hasArg("color")) {
    String hex = server.arg("color");
    long number = strtol(hex.c_str(), NULL, 16);
    int r = number >> 16;
    int g = (number >> 8) & 0xFF;
    int b = number & 0xFF;
    manualColor = pixels.Color(r, g, b);
  }
  server.send(200, "text/plain", "OK");
}

void fillStrip(uint32_t color) {
  for(int i=0; i<NUM_PIXELS; i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}