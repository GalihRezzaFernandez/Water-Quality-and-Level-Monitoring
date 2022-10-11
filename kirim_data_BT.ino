#include "BluetoothSerial.h"//bluetooth serial library

BluetoothSerial SerialBT;//objek bluetooth serial

String phString = "";
uint8_t address[6]  = {0x78, 0x21, 0x84, 0x7B, 0xF3, 0xDE};//alamat bluetooth receiver
bool connected;

unsigned long interval=1000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.

void setup() {
  Serial.begin(115200);//inisialisasi serial komunikasi baud rate
  
  SerialBT.begin("ESP32test", true);//Bluetooth device name
  Serial.println("The device started in master mode, make sure remote BT device is on!");//tanda bluetooth sudah siap disambungkan
  
  connected = SerialBT.connect(address);
  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  } 
//  if (SerialBT.disconnect()) {
//    Serial.println("Disconnected Succesfully!");
//  } 
  SerialBT.connect();
}

void loop() { 
  unsigned long currentMillis = millis(); // grab current time

 if ((unsigned long)(currentMillis - previousMillis) >= interval) {
 int sensorpin = analogRead(35);
  Serial.print("bit :");
  Serial.println(sensorpin);

 phString = String(sensorpin);
 SerialBT.println(phString);
 previousMillis = millis();
 }
}
