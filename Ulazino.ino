
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define BUZZ_PIN 5

#define BUZZ_FREQ 1000
#define BUZZ_DUR 200
#define BUZZ_WDUR 100
#define BUZZ_WSTP 20
#define BUZZ_NTIMES 3

MFRC522 rfid(SS_PIN, RST_PIN);

String admini[]={"29 10 A5 56","23 55 66 22","29 10 A5 56","29 10 A5 56"};
String ulazi[]={"26 ED 28 F8","26 ED 28 F8","87 35 22 88","26 ED 28 F8","26 ED 28 F8","26 ED 28 F8","26 ED 28 F8"};
unsigned int soadm=sizeof(admini)/sizeof(String);
unsigned int soulz=sizeof(ulazi)/sizeof(String);

String skeniraj();
bool provjeri(String zap);
bool jelAdmin(String zap);
void prosao();
void nijeProsao();

void setup() {
  pinMode(BUZZ_PIN,OUTPUT);
  Serial.begin(9600);
  Serial.println(soadm);
  Serial.println(soulz);
  SPI.begin();
  rfid.PCD_Init();
  int i;
  for(i<0;i<soadm;i++){
    Serial.println(admini[i]);
  }
  for(i<0;i<soulz;i++){
    Serial.println(ulazi[i]);
  }
}

void loop() {
  if(!rfid.PICC_IsNewCardPresent()) return;
  if(!rfid.PICC_ReadCardSerial()) return;
  String tag=skeniraj();
  Serial.println(tag);
  if(provjeri(tag)){
    prosao();
  }
  else{
    nijeProsao();
  }
  delay(1000);
}

String skeniraj(){
  String content="";
  byte i;
  for (i=0;i<rfid.uid.size;i++) {
    content.concat(String(rfid.uid.uidByte[i]<0x10?" 0":" "));
    content.concat(String(rfid.uid.uidByte[i],HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}

bool provjeri(String zap){
  int i;
  for(i<0;i<soulz;i++){
    Serial.println(admini[i]);
    Serial.println(zap);
    if(ulazi[i].equals(zap))
      return true;
  }
  return jelAdmin(zap);
}

bool jelAdmin(String zap){
  int i;
  for(i<0;i<soadm;i++){
    Serial.println(admini[i]);
    Serial.println(zap);
    if(admini[i].equals(zap))
      return true;
  }
  return false;
}

void prosao(){
  Serial.println("Prosao");
  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_DUR);
  delay(BUZZ_DUR);
  noTone(BUZZ_PIN);
}

void nijeProsao(){
  Serial.println("Nijeprosao");
  // int i;
  // for(i<0;i<BUZZ_NTIMES;i++){
  //  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_WDUR);
  //  delay(BUZZ_WDUR+BUZZ_WSTP);
  // }
  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_WDUR);
  delay(BUZZ_WDUR+BUZZ_WSTP);
  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_WDUR);
  delay(BUZZ_WDUR+BUZZ_WSTP);
  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_WDUR);
  delay(BUZZ_WDUR+BUZZ_WSTP);
  noTone(BUZZ_PIN);
}
