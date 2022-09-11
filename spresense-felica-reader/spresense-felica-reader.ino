#include "RCS620S_ESP32.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(22, 23);    // RX,TXの割り当て

RCS620S rcs620s(mySerial);

#define COMMAND_TIMEOUT  400
#define PUSH_TIMEOUT     2100
#define POLLING_INTERVAL 500

void ledLightUp()
{
    digitalWrite(LED0, HIGH);
    delay(100);
    digitalWrite(LED1, HIGH);
    delay(100);
    digitalWrite(LED0, LOW);
    digitalWrite(LED2, HIGH);
    delay(100);
    digitalWrite(LED1, LOW);
    digitalWrite(LED3, HIGH);
    delay(100);
    digitalWrite(LED2, LOW);
    delay(100);
    digitalWrite(LED3, LOW);
}

void setup() {
  // 確認用LED
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  Serial.begin(115200);
  mySerial.begin(115200);
  delay(1000);

  int ret = rcs620s.initDevice();
  Serial.print("RCS620S Init = ");
  Serial.println(ret);
}

void loop() {
    rcs620s.timeout = COMMAND_TIMEOUT;
    int ret = rcs620s.polling();
    Serial.print("RCS620S polling = ");
    Serial.println(ret);
    if (ret) {
      
      Serial.print("idm = ");
      for (int i = 0; i < 8; i++) {
        Serial.print(rcs620s.idm[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
  
      Serial.print("pmm = ");
      for (int i = 0; i < 8; i++) {
        Serial.print(rcs620s.pmm[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      rcs620s.readWithEncryption(
        rcs620s.pmm,
        0x000B,
        1 /* block id = 1 の末尾に番号が入っている*/);

      ledLightUp();

      while(1){
        if(rcs620s.polling() == 0){
          break;
        }
        delay(1);
      }
    }
  
    rcs620s.rfOff();
    delay(POLLING_INTERVAL);
}
