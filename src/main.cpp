#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>


const char *ssid     = "Get-2G-DCAD71";
const char *password = "UXJ4RTUG8B";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int hoursSinceLastPressed = 2;
int pos = 0;
int lastHour = 0;


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

Servo myservo;


void pressButton(){
  Serial.println("Trying to press...");
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(100);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(100);                       // waits 15ms for the servo to reach the position
  // }
  myservo.write(100);
  delay(1000);
  myservo.write(0);
}

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  myservo.attach(D1);
}

void loop() {
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());
  if(timeClient.getHours() != lastHour){
    hoursSinceLastPressed++;
    lastHour = timeClient.getHours();
  }
  if((timeClient.getHours() >= 5) && timeClient.getHours() <= 23){
    if(hoursSinceLastPressed >= 2){
      pressButton();
      hoursSinceLastPressed = 0;
    }
  }
  delay(60000);
}