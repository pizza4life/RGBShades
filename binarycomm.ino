void setup() {
  Serial.begin(9600);
}

void loop() {
  String ayyy = Serial.readString();

  for (int i = 0; i < ayyy.length(); i++) {
    char mooo = ayyy.charAt(i);

    for (int ytho = 0; ytho < 7; ytho++) {
      byte numeros = bitRead(mooo, ytho);
      Serial.print(numeros, BIN);
    }
    Serial.println(" ");
  }
}
