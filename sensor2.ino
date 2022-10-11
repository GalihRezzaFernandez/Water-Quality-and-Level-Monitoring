float Voltageph,adcph,ph;

void setup() 
{ 
  Serial.begin(115200);
} 
 
void loop() 
{ 
  adcph = analogRead(35);
  Voltageph = adcph * (3.3 / 4096.0);
  ph = (-(5.6179*Voltageph) + 21.161);
  Serial.println(ph);
  delay(400);
}
