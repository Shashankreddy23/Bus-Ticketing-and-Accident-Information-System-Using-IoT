#include <Wire.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "ThingSpeak.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int irPin = A0;
#define Buz 16
#define bulb 2
#define key1 12
#define key2 0
#define ledr 14
//#define ledg 15
int count = 0;
char input[13];
int i = 0, k = 0;
float latitude , longitude;
String  lat_str , lng_str;

const char* ssid = "Mintu";   // your network SSID (name)
const char* password = "shashank23"; // your network password

WiFiClient  client;

TinyGPSPlus gps;
SoftwareSerial SerialGPS(13, 15);

unsigned long myChannelNumber = 1872840;
const char * myWriteAPIKey = "WDRNV72TIRX2EADX";
// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

void setup() {
  Serial.begin(9600);
  pinMode(Buz, OUTPUT);
  pinMode(bulb, OUTPUT);
  pinMode(irPin, INPUT_PULLUP);
  pinMode(ledr, OUTPUT);
  // pinMode(ledg, OUTPUT);
  delay(200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  SerialGPS.begin(9600);
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Bus ticketing and accident information provider system");
  display.display();
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(5000);
    }
  }
  display.setCursor(0, 30);
  display.println("WiFi connected..!");
  display.print("Got IP:");
  display.println(WiFi.localIP());
  Serial.print("Got IP:");
  Serial.println(WiFi.localIP());
  display.display();
  delay(2000);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 30);
  display.println("Waiting for GPS");
  display.display();
  digitalWrite(Buz, LOW);
  digitalWrite(bulb, LOW);
  digitalWrite(ledr, LOW);
  // digitalWrite(ledg, LOW);
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("Latitude =");
        display.println(lat_str);
        display.display();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println("Longitude =");
        display.println(lng_str);
        display.display();
      }
      delay(1000);
      Serial.println();
    }
  }
  long buttonState = analogRead(irPin);
  if (buttonState == HIGH)
  {
    display.setCursor(0, 30);
    display.setTextColor(WHITE, BLACK);
    display.print(F("ACCIDENT OCCURED "));
    display.setTextColor(WHITE, BLACK);
    display.print(i);
    display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 40);
    display.println("Latitude =");
    display.println(lat_str);
    display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Longitude =");
    display.println(lng_str);
    display.display();
    display.clearDisplay();
    ThingSpeak.setField(1, i);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  }
  unsigned int x, i1 = 0, cmp1 = 0, cmp2 = 0, cmp3 = 0, cmp4 = 0;
  char rec[13], card1[13] = "4D00864B9616", card2[13] = "4D008659D84A", card3[13] = "4D00864B7EFE";
  cmp1 = cmp2 = cmp3 = 0; delay(1000);

  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("WAITING FOR STOP TICKETS");
  display.display();
  for (count = 0 ; count < 12; count++)
  {
    while (!Serial.available());
    input[count] = Serial.read();
  }
  input[12] = 0;
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println(input);
  display.display();
  delay(2000);
  int card = 0;
  if (strncmp(card1, input, 12) == 0) {
    card = 1;
  }
  if (strncmp(card2, input, 12) == 0) {
    card = 2;
  }
  if (strncmp(card3, input, 12) == 0) {
    card = 3;
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("card:");
  display.println((int)card);
  display.display();
  if (card == 1)
  {
    //  digitalWrite(ledg, HIGH);
    digitalWrite(bulb, HIGH);
    digitalWrite(ledr, LOW);
    if (key1 == LOW)
    {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 30);
      display.println("BUS STOP NO:5");
      display.println("CHARGES :50");
      display.display();
      ThingSpeak.setField(2, i);
      ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      delay(2000);
    }
    cmp1 = 0;
  }
  else if (card == 2)
  {
    //   digitalWrite(ledg, HIGH);
    digitalWrite(bulb, LOW);
    digitalWrite(ledr, LOW);
    if (key2 == LOW)
    {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 40);
      display.println("BUS STOP NO:8");
      display.println("CHARGES :80");
      display.display();
      ThingSpeak.setField(3, card);
      ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      delay(2000);
    }
    cmp2 = 0;
  }
  else if (card == 3)
  {
    //   digitalWrite(ledg, LOW);
    digitalWrite(bulb, LOW);
    digitalWrite(ledr, HIGH);
    digitalWrite(Buz, HIGH);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 50);
    display.println("NO BALANCE AVAILABLE");
    display.display();
    delay(2000);
    digitalWrite(Buz, LOW);
    cmp3 = 0;
  }
}
