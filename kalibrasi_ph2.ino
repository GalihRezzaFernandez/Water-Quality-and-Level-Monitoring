float Voltageph,adcph,ph;

void setup() 
{ 
  Serial.begin(115200);
} 
void loop() 
{ 
  adcph = analogRead(34);
  Voltageph = adcph * (3.3 / 4096.0);
  ph = (-(6.1743*Voltageph) + 25.314);
  Serial.println(ph);
  delay(400);
}
