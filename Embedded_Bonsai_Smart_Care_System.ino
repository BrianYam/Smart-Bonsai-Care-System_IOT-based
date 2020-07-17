#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h> 
#include <DHT.h>
#define BLYNK_PRINT Serial 
#define TRIGGER D5
#define ECHO D6 
#define Critical D7
#define DHTWATERPUMP D8
#define SoilsensorPin A0
#define SoilWATERPUMP D1
#define DHTPIN D15 // What digital pin we're connected to 
#define DHTTYPE DHT22 // DHT 22, AM2302, AM2321
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "s9zZ_cpj1FCgx5rn5ldATJmEH5QkDeGS";
// Your WiFi credentials.
// Set password to "" for open networks. 
char ssid[] = "ID"; 
char pass[] = "Password";
const int dry=1024;
const int wet=460; 
const int Bottom=17; 
const int Top=1;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  //DHT22
float h = dht.readHumidity();
float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit delay (500);
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}
// You can send any value at any time.
// Please don't send more that 10 values per second.
Blynk.virtualWrite(V8, h);
Blynk.virtualWrite(V7, t);
}
void setup()
{
// Debug console Serial.begin(9600);
Blynk.begin(auth, ssid, pass);
dht.begin();
pinMode(TRIGGER, OUTPUT);
pinMode(ECHO, INPUT);
pinMode(BUILTIN_LED, OUTPUT);
pinMode(Critical, OUTPUT);
pinMode(DHTWATERPUMP, OUTPUT);
pinMode(DHTPIN, INPUT);
pinMode(SoilsensorPin, INPUT); 
pinMode(SoilWATERPUMP, OUTPUT);
// Setup a function to be called every second timer.setInterval(1000L, sendSensor);
}
void loop()
{
//Ultrasonic
// put your main code here, to run repeatedly:
long duration, distance;
digitalWrite(TRIGGER, LOW); 
delayMicroseconds(2);
digitalWrite(TRIGGER, HIGH); 
delayMicroseconds(10);
digitalWrite(TRIGGER, LOW);
duration = pulseIn(ECHO, HIGH);
// distance = (duration/2) / 29.1; distance= duration*0.034/2;
int percentageWaterLevel = map(distance, Top, Bottom, 100, 0);
if (distance <= 5) { Blynk.virtualWrite(V0, 255);
}

else { Blynk.virtualWrite(V0, 0);
}
if (distance >5 && distance <= 10) {
  Blynk.virtualWrite(V2, 255);
}
else { 
  Blynk.virtualWrite(V2, 0);
}
if (distance >10 && distance < 15) {
  Blynk.virtualWrite(V3, 255);
}
else {
  Blynk.virtualWrite(V3, 0);
}
if (distance >= 15 ) {
  Blynk.virtualWrite(V4, 255);
  Blynk.notify("Reserve Tank Critical Level. Please Add Water!"); 
  digitalWrite(Critical, LOW);
  delay(200); 
  digitalWrite(Critical, HIGH);
}
else { Blynk.virtualWrite(V4, 0);
digitalWrite(Critical, HIGH);
}
Blynk.virtualWrite(V5, percentageWaterLevel);
Serial.print(percentageWaterLevel);
Serial.println("%");
Serial.print(distance);
Serial.println("Centimeter:");
delay(100);
Blynk.run();
//Soil Moisture
int sensorVal = analogRead(SoilsensorPin);
// Sensor has a range of dry to wet
// translate this scale or 0% to 100%
int percentageMoisture = map(sensorVal, wet, dry, 100, 0);
if (distance >= 15 ) {
  digitalWrite(SoilWATERPUMP, LOW); 
  Blynk.virtualWrite(V1, 0);
}
else if (percentageMoisture <= 30) { 
  digitalWrite(SoilWATERPUMP, HIGH);
  Blynk.virtualWrite(V1, 255);
  delay(200);
  digitalWrite(SoilWATERPUMP, LOW);
}
else{
digitalWrite(SoilWATERPUMP, LOW); 
Blynk.virtualWrite(V1, 0);
}
Blynk.virtualWrite(V10, percentageMoisture); 
Serial.print(percentageMoisture);
Serial.println("%");
Serial.println(sensorVal);
//Serial.println(analogRead(SoilsensorPin));

delay(100);
Blynk.run();
timer.run();
//DTH11
float h = dht.readHumidity();
float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit 
if (distance >= 15 ) {
digitalWrite(DHTWATERPUMP, LOW); 
Blynk.virtualWrite(V6, 0);
}
else if (t >=32) { 
 Blynk.virtualWrite(V9, 255);
Blynk.virtualWrite(V6, 255); 
digitalWrite(DHTWATERPUMP, HIGH); 
delay(200);
digitalWrite(DHTWATERPUMP, LOW);
}
else{ 
Blynk.virtualWrite(V9, 0);
Blynk.virtualWrite(V6, 0); 
digitalWrite(DHTWATERPUMP, LOW);
}
Serial.print("Humidity: "); 
Serial.print(h);
Serial.print(" Temperature: ");
Serial.println(t);
Blynk.run();
timer.run();
delay(100);
}
