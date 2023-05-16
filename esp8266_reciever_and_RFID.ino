#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h> 
#include <SPI.h>
#include <MFRC522.h>

//Narednih pet linija koda služi za deklaraciju pinova RFID čitača i aktiviranje klase tj. funkcija neophodnih za registrovanje taga/kartice
constexpr uint8_t RST_PIN = 5
constexpr uint8_t SS_PIN = 4;     
MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];

const char* ssid ="NAZIV_WIFI";
const char* Password ="SIFRA_WIFI"; 
const char *mqttServer="IP_ADRESA";

int payl;
int number;
char *topic="PSCGr5/garden/2023";
uint8 q=1;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
int mqttPort = 1883;  //Port putem kojeg prolaze podatci
int message = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();      
  rfid.PCD_Init();  
  connectToWiFi();  
  setupMQTT();      
}
void loop() {
//Beskonačna petlja u kojoj se konektuje ESP na MQTT broker i očitava+upisuje vrijednost RFID kartice
  if (!mqttClient.connected())  
    reconnect();
  mqttClient.loop();  

  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (rfid.PICC_ReadCardSerial()) {

    String rfidCard = toString(rfid.uid.uidByte, rfid.uid.size);
    checkCard(rfidCard);
    Serial.write(number);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
//Funkcija za konverziju adrese kartice u string
String toString(byte* buffer, byte bufferSize) {
  String rfidCard;
  for (int i = 0; i < bufferSize; i++) {
    rfidCard += String(buffer[i]);
  }
  return rfidCard;
}
//Funkcija za dodjeljivanje decimalnog broja RFID karticama
void checkCard(String rfidCard) {
  if (rfidCard == "2301257426") {
    number = 5;
  }
  if (rfidCard == "2262102827") {
    number = 6;
  }
  if (rfidCard == "2261363927") {
    number = 7;
  }
}
//Konektovanje na WiFI
void connectToWiFi() {
  Serial.print("Connecting to ");
  WiFi.begin(ssid, Password);
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker..");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected.");
      // subscribe to topic
      mqttClient.subscribe(topic, q);
    }
  }
}
//Konverzija stringa u int
int atoi(const String str) {  
  int num = 0;
  int i = 0;
  bool isNegetive = false;
  if (str[i] == '-') {
    isNegetive = true;
    i++;
  }
  while (str[i] && (str[i] >= '0' && str[i] <= '9')) {
    num = num * 10 + (str[i] - '0');
    i++;
  }
  if (isNegetive) num = -1 * num;
  return num;
}
//Callback funkcija koja se kontinualno izvršava i upisuje podatak koji dobije od udaljenih mikrokontrolera (saksija)
void callback(char* topic, byte* payload, unsigned int length) {
  String pay;
  for (int i = 0; i < length; i++) {
    pay += (char)payload[i];
  }
  int payl = atoi(pay);
  Serial.write(payl);
}

