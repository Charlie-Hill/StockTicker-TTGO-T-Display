#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include "esp_adc_cal.h"
#include "config.h"

TFT_eSPI tft = TFT_eSPI(135, 240);

void setup() {
  Serial.begin(9600);

  tft.init();
  tft.setRotation(1);

  WiFi.disconnect();
  delay(1);
  connectWifi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1.60);
    tft.drawString("[Connected!]", tft.width() / 2, tft.height() / 2);

    Serial.print("Wifi connected, IP address: ");
    Serial.println(WiFi.localIP());
  }
}


void connectWifi() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1.25);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {

    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.drawString("[Attempting Wifi connection...]", tft.width() / 2, tft.height() / 2);

    Serial.println("Attempting connection with ");
    Serial.println(WIFI_SSID);
    Serial.println(WIFI_PASSWORD);
    
    delay(2500);
  }

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("[Wifi Connected!]", tft.width() / 2, tft.height() / 2);
  delay(1500);
}
