#include <FirebaseESP32.h>
#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//alamat firebase
#define FIREBASE_HOST "https://monitoring-kualias-air-default-rtdb.firebaseio.com/"
#define FIREBASE_Authorization_key "GG0j1KySuSi0nyHT6xy27tEqIg92SkJRNC1OqJIa"
//wifi
#define WIFI_SSID "Galih Rezza" // Change the name of your WIFI
#define WIFI_PASSWORD "sayangkamu" // Change the password of your WIFI

#define pintriger 5
#define pinecho 18
#define pompabuang 2
#define pompaisi 4

long durasi;
float cm1, cm2;
char incomingChar;
String message = "";
int flag = 0;
String status_pompabuang = "";
String status_pompaisi = "";

unsigned long interval=1000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.


FirebaseData firebaseData;
FirebaseJson json;

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode (pintriger, OUTPUT);
  pinMode (pinecho, INPUT);
  pinMode (pompabuang, OUTPUT);
  pinMode (pompaisi, OUTPUT);
   
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
   Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  
  Serial.println(F("Connection Success"));
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key); 
}

void pompa(){
  if (Firebase.getString(firebaseData, "/FB_MONITOR/Pompa Buang"))
{
  
  if (firebaseData.dataType() == "string"){
    status_pompabuang = firebaseData.stringData();
    Serial.print("Pompa Buang: ");
    Serial.print(status_pompabuang);
    Serial.print("");
    if (status_pompabuang == "1"){
      digitalWrite (pompabuang,0);
      Serial.println("pompa buang on");
    } else if (status_pompabuang == "0"){
      digitalWrite (pompabuang,1);
      Serial.println("pompa buang off");
    }
}
}
if (Firebase.getString(firebaseData, "/FB_MONITOR/Pompa Isi"))
{
  if (firebaseData.dataType() == "string"){
    status_pompaisi = firebaseData.stringData();
    Serial.print("Pompa Isi: ");
    Serial.print(status_pompaisi);
    Serial.print("");
    if (status_pompaisi == "1"){
      digitalWrite (pompaisi,0);
      Serial.println("pompa isi on");
    } else if (status_pompaisi == "0"){
      digitalWrite (pompaisi,1);
      Serial.println("pompaisi off");
    }
}
}
}

void pH2(){
  int adcph2 = analogRead(34);
  float volt2 = adcph2 * (3.3 / 4096.0);
  float ph2 = ((-(6.1743*volt2)) + 25.314);
  Serial.print("pH2: ");  
  Serial.println(ph2);
  Firebase.setFloat(firebaseData, "/FB_MONITOR/pH2", ph2);
  Firebase.pushFloat(firebaseData, "/HISTORY/pH2", ph2);
}
void ultrasonic(){
   digitalWrite (pintriger,LOW);
  delayMicroseconds(20);
  digitalWrite (pintriger,HIGH);
  delayMicroseconds(20);
  digitalWrite (pintriger,LOW);
  delayMicroseconds(20);
  
  durasi = pulseIn(pinecho,HIGH);
  //jarak dasar kolam ke sensor
  cm1 = (0.0177 * durasi) - 0.8775;
  //jarak sesungguhnya
//  cm2 = 130- cm1;
   
  Serial.print("Ketinggian: ");
  Serial.println(cm1);
  Firebase.setFloat(firebaseData, "/FB_MONITOR/Ultrasonic", cm1);
  Firebase.pushFloat(firebaseData, "/HISTORY/Ultrasonic", cm1);
}
void loop() {
  {
  unsigned long currentMillis = millis(); // grab current time
 if ((unsigned long)(currentMillis - previousMillis) >= interval) {
pompa();
ultrasonic();
pH2();
previousMillis = millis();
 }
  }
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar); 
      flag = flag + 1;
    }
    else{
      message = "";
      flag = 0;
    }
  }
  if (flag == 4){
  
  float adcph1 = message.toFloat();
  float volt1 = adcph1 * (3.3 / 4096.0);
  float ph1 = ((-(5.6179*volt1)) + 21.161);

  Serial.print("bit: ");
  Serial.println(adcph1);
  Serial.print("pH1: ");
  Serial.println(ph1);
   //kirim firebase
    Firebase.setFloat(firebaseData, "/FB_MONITOR/pH1", ph1);
    //history firebase
    Firebase.pushFloat(firebaseData, "/HISTORY/pH1", ph1);
  }
 }
