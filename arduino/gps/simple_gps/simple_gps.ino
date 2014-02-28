void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
}

char c;

void loop() {
  // put your main code here, to run repeatedly: 
  while(Serial1.available()) {
    c = Serial1.read();
    Serial.print(c);
  }
  delay(100);
}
