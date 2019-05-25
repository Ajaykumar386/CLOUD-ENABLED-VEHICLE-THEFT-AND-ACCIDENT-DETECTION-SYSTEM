#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include<SoftwareSerial.h>
#include<Servo.h>

#define sw D1
String msg91_data;
String data,url,line;
int l;
SoftwareSerial mySerial(D2,D3); // incoming sensor data
SoftwareSerial mySerial1(D5 ,D6); // retriew mesg from these pins
String talkBackCommand,tbRequest,hostm,msgurl,url1;
Servo servo;
int randomNumber;
int State ;

const char* ssid     = "Australia";//Enter the ssid of your router
const char* password = "australia1";//Enter the password of your router


void setup() {
  Serial.begin(115200);
  pinMode(D1,INPUT);
  mySerial.begin(115200);

   mySerial1.begin(115200);
  servo.attach(D4);

  
  delay(10);
  

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
//  l=1;
//  msg91();
}

void loop() 
{
 Sensor_Data();
delay(1000);
thingspeak();
delay(1000);
getTalkBack(); // Get the command issued from mobile app/web app

if(talkBackCommand == "9999") {
  servo.write(180);
  Serial.println("srvoo");
  delay(30000);
 servo.write(0);
  }
else{
 }

 
delay(10000);

msg91();


}
 



/*......................................Sensor Data.....................................*/
void Sensor_Data()
{
   State = digitalRead(D1);
  Serial.println(State);
  if ( State == 0){
  url1="/api/sendhttp.php?mobiles=9550712349&authkey=275942AbjkTBSmQv5cd4f716&route=4&sender=TESTIN&message=NEED ACCESS&country=91";
   msgurl = url1;
  msggg();
  }
  delay(1000);  
}
/*.....................................Uploading to Cloud...............................*/


void getTalkBack()
{
const char* host = "api.thingspeak.com";
const String talkBackAPIKey = "USQX4JVP6FZ5XKVS";
const String talkBackID = "33125";



WiFiClient client; // Initialize a Wi-Fi client

tbRequest="GET /talkbacks/"+ talkBackID + "/commands/execute?api_key=" + talkBackAPIKey;
Serial.println(tbRequest);


  if (client.connect("api.thingspeak.com", 80))
  {
    client.println(tbRequest);
    if (client.connected())
      {
        talkBackCommand="";
        while(client.connected() && !client.available()) delay(10); //waits for data
        while (client.connected() || client.available())
            {
              talkBackCommand = client.readStringUntil('\n');
            }
        Serial.print("Command -> ");
        Serial.println(talkBackCommand);
        Serial.println();
      }
  client.stop();
  Serial.flush();
 }

}

void thingspeak(){

const char* thingspeak_host = "api.thingspeak.com";
const String privateKey = "FLGKGH19NQHJLVRP";

if(mySerial.available()>0){
  Serial.println("data available");

if(mySerial.find('$')){
    Serial.println("$ available");
  data = mySerial.readStringUntil('*');
  Serial.println(data);



Serial.print("connecting to ");
  Serial.println(thingspeak_host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(thingspeak_host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String thingspeak_url = "/update";
  thingspeak_url += "?api_key=";
  thingspeak_url += privateKey;
  thingspeak_url+= data;
 
  
  Serial.print("Requesting thingspeak_url: ");
  Serial.println(thingspeak_url);
  
  // This will send the request to the server
  client.print(String("GET ") + thingspeak_url + " HTTP/1.1\r\n" +
               "Host: " +thingspeak_host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(1000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}
}
  
  Serial.println();
  Serial.println("closing connection");
}

void msg91(){
url1="/api/sendhttp.php?mobiles=9550712349&authkey=275942AbjkTBSmQv5cd4f716&route=4&sender=TESTIN&message=HELLO BOSS&country=91";
 msgurl = url1;
// thingspeak_host="api.msg91.com";
  if( mySerial1.available()>0){
    
 
//   msg91_data = mySerial1.readStringUntil('*');
//   Serial.println(msg91_data);

  if (mySerial1.find('#')){
     msg91_data = mySerial1.readStringUntil('*');
     Serial.println("data is available for message");
    Serial.println("found #");
    
l=1;
     Serial.println(msg91_data);
  }}
    if(l==1){
    
msggg();
    }
  l=0;
}


void msggg(){
  hostm="api.msg91.com";
  Serial.print("connecting to ");
  Serial.println(hostm);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("api.msg91.com", httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request

 

  Serial.print("msgurl: ");
  Serial.println(msgurl);
  
  // This will send the request to the server
  client.print(String("GET ") + msgurl + " HTTP/1.1\r\n" +
               "Host: " +hostm + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(1000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}
