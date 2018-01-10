#include <ArduinoJson.h>
#include <LWiFi.h>
#define LEDRED 13
#define LEDBLUE 15
#define LEDGREEN 14

const char* ssid     = "220-1";           // insert your SSID
const char* pass     = "ntuedorm";   // insert your password

WiFiClient client;

const char* server = "opendata.epa.gov.tw";  
const char* resource = "/ws/Data/ATM00625/?$format=json"; 
const char* server2 = "maker.ifttt.com";  
const char* resource2 ="/trigger/line2/with/key/RrYXeUcg-QpjiO2nzTGJ7" ; //輸入key
char json[12000]; 

void setup(){
  Serial.begin(115200);
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
  pinMode(LEDRED,OUTPUT);
  pinMode(LEDGREEN,OUTPUT);
  pinMode(LEDBLUE,OUTPUT);
  digitalWrite(LEDRED,LOW);
  digitalWrite(LEDBLUE,LOW);
  digitalWrite(LEDGREEN,LOW);
}

void loop(){
  // connect to server  
  while(!client.connect(server, 80)){
    // wait for connecting server...
  }

  //Send request to resource
  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();

  delay(100);

  //Reading stream
  client.setTimeout(10000);

  //Checking for the end of HTTP stream
  while(!client.find("\r\n\r\n")){
    // wait for finishing header stream reading ...
  }

  // Reading the response string
  client.readBytes(json, 12000);
  
  // process JSON from response string
  DynamicJsonBuffer jsonBuffer;
  JsonArray& rootArray = jsonBuffer.parseArray(json);
  JsonObject& guTingSite = rootArray[1];
  //JsonObject& songShanSite = rootArray[23];
  
  const char* site0 = guTingSite["Site"];
  double pm250 = guTingSite["PM25"];
  
  //const char* site1 = songShanSite["Site"];
  //double pm251 = songShanSite["PM25"];
  
  // Print data to Serial
  Serial.print("*** ");
  Serial.print(site0);
  Serial.println(" ***");
  Serial.print("PM25: ");
  Serial.println(pm250);
  Serial.println("--------------------"); 
  //Serial.print("*** ");
  //Serial.print(site1);
  //Serial.println(" ***");
  //Serial.print("PM25: ");
  //Serial.println(pm251);
  //Serial.println("--------------------"); 


  
  delay(2000); // the OWM free plan API does NOT allow more then 60 calls per minute

  while(!client.connect(server2, 80)){
      // wait for connecting server...
  }
  if( pm250<= 35){
    digitalWrite(LEDRED,HIGH);
    digitalWrite(LEDGREEN,LOW);
    digitalWrite(LEDBLUE,HIGH);
  }
  else if(pm250 > 35 && pm250<=53){
    digitalWrite(LEDRED,LOW);
    digitalWrite(LEDGREEN,LOW);
    digitalWrite(LEDBLUE,HIGH);
  }
    else if(pm250 > 54 && pm250< 60){
    digitalWrite(LEDRED,LOW);
    digitalWrite(LEDGREEN,HIGH);
    digitalWrite(LEDBLUE,HIGH);
  }
  else{
    digitalWrite(LEDRED,LOW);
    digitalWrite(LEDGREEN,HIGH);
    digitalWrite(LEDBLUE,LOW);
  }
  
  client.print("GET ");
  client.print(resource2);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server2);
  client.println("Connection: close");
  client.println();
  delay(1000);
  

    client.stop(); // disconnect from server
}
