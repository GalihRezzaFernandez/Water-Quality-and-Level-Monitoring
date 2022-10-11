//deklarasi pin yg dipakai trig dan echo
//#define pintriger 5
//#define pinecho 18

const int pintriger = 5;
const int pinecho = 18;

//deklarasi tipe data
long durasi;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //pinmode utk input dan output
  pinMode (pintriger, OUTPUT);
  pinMode (pinecho, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (pintriger,LOW);
  delayMicroseconds(20);
  digitalWrite (pintriger,HIGH);
  delayMicroseconds(20);
  digitalWrite (pintriger,LOW);
  delayMicroseconds(20);

  durasi = pulseIn(pinecho,HIGH);
  float cm1 = ((0.0177*durasi) - 0.8775);

//Serial.println(durasi);
Serial.println(cm1);

delay (500);

}
