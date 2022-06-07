#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include "WiFi.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "esp_adc_cal.h"
#include "config.h"

//Globals
TFT_eSPI tft = TFT_eSPI(135, 240);
HTTPClient http;

const char* tickersToCycle[] = {
  "MHC.L",
  "RBLX",
  "BIDS.L",
  "KOD.L",
  "RTX",
  "WIZZ.L",
  "NTLA"
};
size_t TICKERSIZE = sizeof(tickersToCycle) / sizeof(tickersToCycle[0]);
int activeTicker = 0;
//END: Globals

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
    Serial.println(TICKERSIZE);
    displayTicker(tickersToCycle[activeTicker]);
    delay(3500);
    if (activeTicker == TICKERSIZE) {
      activeTicker = 0;
    } else {
      activeTicker++;
    }
    
  }
}


void displayTicker(String ticker) {
    String url = String(API_URL) + "/stockinfo/" + ticker;
    http.begin(url.c_str());

    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
        String payload = http.getString();

        JSONVar jsonResult = JSON.parse(payload);
        
        tft.fillScreen(TFT_BLACK);

        tft.setTextSize(1.8);
        tft.drawString(jsonResult["companyName"], tft.width() / 2, tft.height());
        tft.setTextSize(2);
        tft.drawString(jsonResult["price"], tft.width() / 2, 100);

        delay(3500);
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
    
    delay(2500);
  }

  Serial.print("Wifi connected, IP address: ");
  Serial.println(WiFi.localIP());

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("[Wifi Connected!]", tft.width() / 2, tft.height() / 2);
  delay(1500);
}
