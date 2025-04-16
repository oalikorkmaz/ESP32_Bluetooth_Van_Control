/*
13: Dış Arka Led
12: Dış Ön Led
14: Arka Spot
27: Webosto
26: Orta Led Spot
25: Ön Led Spot
33: WC Led Spot
32: Mutfak Aydınlatma
15: Oturma Yeri Aydınlatma
2: Ayak Altı Aydınlatma
4: Temiz Su
16: Pis Su
17: Boiler
5: Pompa 1
18: Pompa 2
19: Yedek
*/

#include "Arduino.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!
#endif

BluetoothSerial SerialBT;

const int relayPins[] = {13, 12, 14, 27, 26, 25, 33, 32, 15, 2, 4, 16, 17, 5, 18, 19}; 

const int relayCount = sizeof(relayPins) / sizeof(relayPins[0]);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32-VanControl");

  // Röle pinlerini çıkış olarak ayarla
  for (int i = 0; i < relayCount; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW); // ilk başta hepsi kapalı olsun
  }

  Serial.println("Basladi...");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }

  if (SerialBT.available()) {
    int cmd = SerialBT.read();  // Gelen veri
    delay(20);

    int index = (cmd - 1) / 2;  // Kaçıncı röle olduğunu bul (0 tabanlı)

    if (index >= 0 && index < relayCount) {
      bool state = (cmd % 2 == 1);  // Tekse HIGH, çiftse LOW
      digitalWrite(relayPins[index], state ? HIGH : LOW);

      Serial.print("Röle ");
      Serial.print(index + 1);
      Serial.println(state ? " açıldı" : " kapatıldı");
    } else {
      Serial.print("Geçersiz komut: ");
      Serial.println(cmd);
    }
  }

  delay(20);
}
