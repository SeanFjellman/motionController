#include <SPI.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("NRF24 check...");

  bool began = radio.begin();
  Serial.print("radio.begin(): ");
  Serial.println(began ? "OK" : "FAIL");

  Serial.print("isChipConnected(): ");
  Serial.println(radio.isChipConnected() ? "YES" : "NO");

  if (radio.isChipConnected()) {
    radio.printDetails();
  } else {
    Serial.println("NO = still wiring/power/seat/SPI header issue.");
  }
}

void loop() {}
