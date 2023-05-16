#include "ThingsBoard.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 5;  
constexpr uint8_t SS_PIN = 4;   
MFRC522 rfid(SS_PIN, RST_PIN);  
MFRC522::MIFARE_Key key;
byte nuidPICC[4];
uint8 q = 1;

#define WIFI_AP "WiFi"
#define WIFI_PASSWORD "SIFRA"

#define TOKEN "PrvPRVjkTgXjB6q8z8h1"  //P R O M I J E N I T I
#define THINGSBOARD_SERVER "demo.thingsboard.io"
#define SERIAL_DEBUG_BAUD 115200

WiFiClient espClient;
ThingsBoard tb(espClient);
int status = WL_IDLE_STATUS;

int number;
int ir_1_tmp = 5;   //D1
int ir_2_tmp = 4;   //D2
int ir_3_tmp = 14;  //D5

void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  pinMode(ir_1_tmp, INPUT);
  pinMode(ir_2_tmp, INPUT);
  pinMode(ir_3_tmp, INPUT);
}
void loop() {
  delay(1000);
  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }
  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    String rfidCard = toString(rfid.uid.uidByte, rfid.uid.size);
    checkCard(rfidCard);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  if (!tb.connected()) {
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }
  Serial.println("Sending data...");
  int ir_1 = digitalRead(ir_1_tmp);
  int ir_2 = digitalRead(ir_2_tmp);
  int ir_3 = digitalRead(ir_3_tmp);
  tb.sendTelemetryInt("ir_sensor_1", ir_1);
  tb.sendTelemetryInt("ir_sensor_2", ir_2);
  tb.sendTelemetryInt("ir_sensor_3", ir_3);
  tb.loop();
}

String toString(byte* buffer, byte bufferSize) {
  String rfidCard;
  for (int i = 0; i < bufferSize; i++) {
    rfidCard += String(buffer[i]);
  }
  // Serial.print(rfidCard);
  return rfidCard;
}
void checkCard(String rfidCard) {
  if (rfidCard == "2301257426") {
    number = 1;
    tb.sendTelemetryInt("RFID x05", number);
  }
  if (rfidCard == "2262102827") {
    number = 1;
    tb.sendTelemetryInt("RFID x06", number);
  }}

