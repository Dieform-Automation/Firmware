const uint8_t relay0  = 42;                                        //Pins
const uint8_t relay1  = 43;
const uint8_t relay2  = 44;     
const uint8_t relay3  = 45;   
const uint8_t relay4  = 41;     
const uint8_t relay5  = 40;  
const uint8_t relay6  = 37;   
const uint8_t relay7  = 36;

const uint8_t dOut0  = 30;                                        //Pins
const uint8_t dOut1  = 31;
const uint8_t dOut2  = 32;     
const uint8_t dOut3  = 33;   
const uint8_t dOut4  = 34;     
const uint8_t dOut5  = 35;  



void setup() {
	pinMode(relay0, OUTPUT);
	pinMode(relay1, OUTPUT);
	pinMode(relay2, OUTPUT);
	pinMode(relay3, OUTPUT);
	pinMode(relay4, OUTPUT);
	pinMode(relay5, OUTPUT);
	pinMode(relay6, OUTPUT);
	pinMode(relay7, OUTPUT);
	
	pinMode(dOut0, OUTPUT);
	pinMode(dOut1, OUTPUT);
	pinMode(dOut2, OUTPUT);
	pinMode(dOut3, OUTPUT);
	pinMode(dOut4, OUTPUT);
	pinMode(dOut5, OUTPUT);
}

void loop() {
  digitalWrite(relay0, HIGH);
  delay(1000);
  digitalWrite(relay1, HIGH);
  delay(1000);
    digitalWrite(relay2, HIGH);
  delay(1000);
    digitalWrite(relay3, HIGH);
  delay(1000);
    digitalWrite(relay4, HIGH);
  delay(1000);
    digitalWrite(relay5, HIGH);
  delay(1000);
    digitalWrite(relay6, HIGH);
  delay(1000);
    digitalWrite(relay7, HIGH);
  delay(1000);

    digitalWrite(dOut5, HIGH);
  delay(1000);
  digitalWrite(dOut4, HIGH);
  delay(1000);
    digitalWrite(dOut3, HIGH);
  delay(1000);
    digitalWrite(dOut2, HIGH);
  delay(1000);
    digitalWrite(dOut1, HIGH);
  delay(1000);
    digitalWrite(dOut0, HIGH);
  delay(1000);

 }
