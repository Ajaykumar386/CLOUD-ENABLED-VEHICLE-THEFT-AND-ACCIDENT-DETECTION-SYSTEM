#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;                                        // Create an Instance of the TinyGPS++ object called gps
SoftwareSerial ss(9, 10); // rx,tx  //gps tx, rx
SoftwareSerial mySerial(7, 8); // 7 to D3-TX and 8 to D2-RX to send sensor and lat and lon data to node mucu

SoftwareSerial mySerial1(4,5); // 4 to D6 on nodemcu , 5 to D5 on nodemcu to send message sting to nodemcu
int x, y, z, bstate2;
int buzzer = 11;
int sw2 = 3;
String value, url1;
double latitude, longitude;
String condition ;
int acc = 0, i = 0, j = 1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200 );
  mySerial1.begin(115200);
  ss.begin(9600);
  pinMode(sw2, INPUT);
  pinMode(buzzer, OUTPUT);
   digitalWrite(buzzer, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  x = analogRead(A0);
  y = analogRead(A1);
  z = analogRead(A2);

Serial.println( "x: "+String(x));
Serial.println( "y: "+String(y));
Serial.println( "z: "+String(z));

  
  bstate2 = digitalRead(sw2);
  Serial.println( "BUTTON STATE "+String(bstate2));
  
  latitude = gps.location.lat();
  longitude = gps.location.lng();
  delay(1000);
  smartDelay(1000);
  String url1 = "$";
  url1 += "&field3=";
  url1 += condition;
  url1 += "&field1=";
  url1 += String(latitude, 6);
  url1 += "&field2=";
  url1 += String(longitude, 6);
  url1 += "*";
  
  //Serial.println(url1);
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
  
if (x >=700 && y > 700 && z>700) {
    Serial.println("accident is detected");
    acc = 1;
  }
//  else if (x >380 && y > 390) {
//    Serial.println("accident is detected");
//    acc = 1;
//  }
  else {
    Serial.println(" no accident is detected");
    acc = 0;
  }
  if (acc == 1)
  {
    Serial.println("accident happened");
   digitalWrite(buzzer, HIGH);
  // tone(buzzer ,1000);
    delay(3000);
    digitalWrite(buzzer, LOW);
   // noTone(buzzer);
    while (j == 1) {
      bstate2 = digitalRead(sw2);
      Serial.println( "BUTTON STATE "+String(bstate2));
      if ( bstate2 == 0) {
        j = 0;
      }
      else if (i == 5) {
        j = 0;
      }
      i++;
      delay(500);
    }
    if (bstate2 == 0)
    
    {
      Serial.println("vehicle is safe");
      condition = "0";
      mySerial.println(url1);
        Serial.println(url1);
    }
    else if (bstate2 == 1)
    {
      Serial.println("vehicle is not safe");
      condition = "1";
      value = "#accident occured at lat ="+String(latitude,6)+"and lon ="+String(longitude,6);
      value += "*";
      Serial.println(value);
      mySerial1.println(value);
      delay(1000);
      mySerial.println(url1);
      Serial.println(url1);
    }
  }

  
  j = 1;
i=0;
delay(1000);
}
static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
