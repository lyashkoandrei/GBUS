#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// отправляем запрос и в ответ ждём данные с АЦП
#define TX_PIN 4      // пин
#define RX_ADDR 3     // адрес приёмника
#define TX_ADDR 5     // наш адрес
#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

LiquidCrystal_I2C lcd(0x27, 20, 4);

struct myStruct {
  float distance;
  float ls;
  float lpm;
  int addr;
};

myStruct rxData;              // приёмная структура
byte buffer[sizeof(rxData)];  // приёмный буфер

void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
}

void loop() {
  // отправляем запрос каждые три секунды
  static uint32_t tmr;
  if (millis() - tmr > 200) {
    tmr = millis();
    GBUS_send_request(TX_PIN, RX_ADDR, TX_ADDR);
  }

  if (GBUS_read(TX_PIN, TX_ADDR, buffer, sizeof(buffer))) {
    lcd.clear();
    unpackDataBytes(buffer, rxData);  //распаковка упакованного буфера
    Serial.print("Всего литров: ");Serial.println(rxData.ls);    
    Serial.print("Литров/мин: ");Serial.println(rxData.lpm);
    Serial.print("Расстояние: ");Serial.println(rxData.distance);
    lcd.print("LpM: ");lcd.print(rxData.lpm);
    lcd.setCursor(0, 1);
    lcd.print("Dis: ");lcd.print(rxData.distance);
    lcd.setCursor(0, 2);
    lcd.print("OoOops");
    lcd.setCursor(0, 3);
    lcd.print(rxData.addr);
  }
}
