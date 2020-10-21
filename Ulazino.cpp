#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ludiESPpinovi.h>
 
#define SS_PIN  D1
#define RST_PIN D2
#define OUT_PIN D3

#define HI 0
#define LO 1

/*
#define BUZZ_PIN 5

#define BUZZ_FREQ 1000
#define BUZZ_DUR 200
#define BUZZ_WDUR 100
#define BUZZ_WSTP 20
#define BUZZ_NTIMES 3
*/

const char* ssid     = "Rezidencija Hasak";
const char* password = "pisenamodemu";

String admini[]={"D9 55 AD 47","87 40 3B 26","39 A7 A8 56","19 4A D0 02"};
String ulazi[]={};

const char* str="<!doctype html><html lang='bs'><head><title>Otkljucavanje...</title></head><body><p>Ova stranica bi se trebala zatvoriti sama...</p><script>window.history.back();</script></body></html>\n\n";

unsigned int soadm=sizeof(admini)/sizeof(String);
unsigned int soulz=sizeof(ulazi)/sizeof(String);

MFRC522 rfid(SS_PIN, RST_PIN);

ESP8266WebServer server(3221);

IPAddress local_IP(192, 168, 1, 9);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


String skeniraj();
bool provjeri(String zap);
bool jelAdmin(String zap);
void prosao();
void nijeProsao();
void otkljucaj();

void setup() {
  //pinMode(BUZZ_PIN,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(OUT_PIN,OUTPUT);
  digitalWrite(OUT_PIN,LO);
  digitalWrite(LED_BUILTIN,LO);
  //Serial.begin(9600);
  Serial.println(soadm);
  Serial.println(soulz);
  SPI.begin();
  rfid.PCD_Init();

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED_BUILTIN,HI);
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/otkljucaj",otkljucaj);
  server.begin();


  int i;
  for(i<0;i<soadm;i++){
    Serial.println(admini[i]);
  }
  for(i<0;i<soulz;i++){
    Serial.println(ulazi[i]);
  }
}

void loop() {
  digitalWrite(OUT_PIN,LO);
  server.handleClient();
  if(!rfid.PICC_IsNewCardPresent()) return;
  if(!rfid.PICC_ReadCardSerial()) return;
  String tag=skeniraj();
  Serial.println(tag);
  if(provjeri(tag)){
    prosao();
  }
  else{
    nijeProsao();
    delay(1000);
  }
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
    //Serial.println(admini[i]);
    //Serial.println(zap);
    if(ulazi[i].equals(zap))
      return true;
  }
  return jelAdmin(zap);
}

bool jelAdmin(String zap){
  int i;
  for(i<0;i<soadm;i++){
    //Serial.println(admini[i]);
    //Serial.println(zap);
    if(admini[i].equals(zap))
      return true;
  }
  return false;
}

void prosao(){
  digitalWrite(OUT_PIN,HI);
  delay(5000);
  digitalWrite(OUT_PIN,LO);
  Serial.println("Prosao");
  /*
  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_DUR);
  delay(BUZZ_DUR);
  noTone(BUZZ_PIN);
  */
}

void nijeProsao(){
  Serial.println("Nijeprosao");
  // int i;
  // for(i<0;i<BUZZ_NTIMES;i++){
  //  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_WDUR);
  //  delay(BUZZ_WDUR+BUZZ_WSTP);
  // }
  /*
  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_WDUR);
  delay(BUZZ_WDUR+BUZZ_WSTP);
  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_WDUR);
  delay(BUZZ_WDUR+BUZZ_WSTP);
  tone(BUZZ_PIN,BUZZ_FREQ,BUZZ_WDUR);
  delay(BUZZ_WDUR+BUZZ_WSTP);
  noTone(BUZZ_PIN);
  */
}

void otkljucaj(){
  if(server.argName(0).equals("kod") && server.arg(0).equals("pustimeunutra")){
    prosao();
    server.send(200, "text/html", str);
  }
}