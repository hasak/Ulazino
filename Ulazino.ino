#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define PISKATOR_PIN 8
#define PISKATOR_DURATION 333
#define PISKATOR_POGDUR 100
#define PISKATOR_POGPAU 20

MFRC522 mfrc522(SS_PIN, RST_PIN);
const String ulaz[]={"29 10 A5 56","87 AB 3A 26"};

void setup() {
  pinMode(PISKATOR_PIN,OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Postavi kljuc");
  Serial.println();
}

void loop() {
  String kljuc;
  if(!mfrc522.PICC_IsNewCardPresent())return;
  if(!mfrc522.PICC_ReadCardSerial())return;
  
  Serial.print("Kljuc: ");
  kljuc=procitajkljuc();
  Serial.print(kljuc);
  Serial.println();
  Serial.print("Message: ");
  if(kljuc == "26 ED 28 F8"){
    Serial.println("Authorized access");
    Serial.println();
    pisni(PISKATOR_PIN,PISKATOR_DURATION);
  }
  else{
    Serial.println(" Access denied");
    pisni(PISKATOR_PIN,PISKATOR_POGDUR,PISKATOR_POGPAU);
  }
  delay(1500);
}

String procitajkljuc(){
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}

void pisni(const int pin, const int dur){
  digitalWrite(pin,1);
  delay(dur);
  digitalWrite(pin,0);
}

void pisni(const int pin, const int dur, const int pausedur){
  digitalWrite(pin,1);
  delay(dur);
  digitalWrite(pin,0);
  delay(pausedur);
  digitalWrite(pin,1);
  delay(dur);
  digitalWrite(pin,0);
  delay(pausedur);
  digitalWrite(pin,1);
  delay(dur);
  digitalWrite(pin,0);
}
