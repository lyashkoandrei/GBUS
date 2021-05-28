// принимаем запрос и отправляем в ответ данные
#define RX_PIN 4      // пин
#define RX_ADDR 3     // наш адрес
#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

struct myStruct {
  float distance;
  float ls;
  float lpm;  
  int addr = RX_ADDR; 
};
myStruct txData;              // отправная структура

byte buffer[sizeof(txData)];  // отправной буфер

float trigPin = 10; 
float echoPin = 11; 
    
void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
}

void loop() {
  // ждём приём на адрес RX_ADDR, т.е. наш адрес RX_ADDR
  byte txaddr = GBUS_read_request(RX_PIN, RX_ADDR);
  if (txaddr) {    
      float duration;
      float distance;
      // для большей точности установим значение LOW на пине Trig
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(2); 
      // Теперь установим высокий уровень на пине Trig
      digitalWrite(trigPin, HIGH);
      // Подождем 10 μs 
      delayMicroseconds(10); 
      digitalWrite(trigPin, LOW); 
      // Узнаем длительность высокого сигнала на пине Echo
      duration = pulseIn(echoPin, HIGH); 
      // Рассчитаем расстояние
      txData.distance = duration / 58;    
      // если успешно приняли
      txData.ls = 17.25;
      txData.lpm = 18.423;
      packDataBytes(buffer, txData);    
      // отправляем обратно (по txaddr) с нашего адреса (RX_ADDR) c контролем целостности данных
      GBUS_send(RX_PIN, txaddr, RX_ADDR, buffer, sizeof(buffer));
  }
}
