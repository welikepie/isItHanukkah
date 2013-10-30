/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 Circuit:
 * WiFi shield attached
 
 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */
 
#include <SPI.h>
#include <WiFi.h>
#include <aJSON.h>
#include <avr/pgmspace.h>


boolean flicker = true;
int blinkenLichten = 850;

long interval = 100000; // interval at which to do something (milliseconds), 5 minutes in this case.

int nums[8] = {2,3,4,5,6,8,9,13};
char ssid[] = "Macs";
char pass[] = "!pandzior";
int keyIndex = 0;            // your network key Index number (needed only for WEP)
unsigned long previousMillis = 0; // last time update
char response[512];
int index = 0;
boolean overflowed = false;
boolean alreadyCalled = false;
int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "dev.welikepie.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;
char *str = "{\"dayOf\":\"5\",\"isHappening\":\"true\"}";
char *thing;
void setup() {
  pinMode(nums[0], OUTPUT);
  pinMode(nums[1], OUTPUT);
  pinMode(nums[2], OUTPUT);
  pinMode(nums[3], OUTPUT);
  pinMode(nums[4], OUTPUT);
  pinMode(nums[5], OUTPUT);
  pinMode(nums[6], OUTPUT);
  pinMode(nums[7], OUTPUT);
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
   
    //if(str(json_get_value(token_list, "isHappening")).equals("true")){
     // Serial.print("THINGZANDSHIT");
    //}
    
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present"); 
      // don't continue:
      while(true);
    } 
  
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server"); 
    // Make a HTTP request:
    client.println("GET /isItHannukah/api/doTheyKnow/?forceDay=5 HTTP/1.1");
    client.println("Host: dev.welikepie.com");
    client.println("Connection: close");
    client.println();
  }
  dialEmUp();
}

void loop() {
  lightEmUp(8);
   unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
     previousMillis = currentMillis;  
    Serial.println("DIALING");
    dialEmUp();
     // do something
  }
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while(client.available()) {
    char c = client.read();
    if(index < 511){
      response[index] = c;
      index++;
    }
    else{
      overflowed = true;
    }
  }
if(client.available() == 0 && index > 0 && alreadyCalled == false){
    alreadyCalled = true;
    echoStuff();
  }
  if (!client.connected()) {
    client.stop();
  }
}
void echoStuff(){
int setInt = 0;
String JSONstring = "";
JSONstring+='"';
for(int i = 0; i < index; i++){  
  if(response[i] == '{'){
    setInt = 1;
  }
  if(setInt>0 && response[i]!='"' ){
    JSONstring+=response[i];
  }
  if(response[i] == '}'){
  setInt = 0;
  }
}
JSONstring+='"';
Serial.print(JSONstring);
char JSON[JSONstring.length()]; 
JSONstring.toCharArray(JSON, JSONstring.length());
//json_to_token_list(JSON,token_list);

Serial.print("trying");
//Serial.print(json_get_value(token_list, "isHappening"));
 aJsonObject* jsonObject = aJson.parse(str);
     thing = aJson.print(jsonObject);
     Serial.print(thing);
    Serial.print(strlen(thing));
    Serial.print("trying");
   // aJsonObject* name = aJson.getObjectItem(jsonObject,"isHappening");
    //Serial.print(jsonObject->type);
   // Serial.println(name->type);
    //Serial.print(jsonObject->isHappening);
    Serial.print("success?");
Serial.print("success?");
//if(json_get_value(token_list, "isHappening") == "true"){
//  Serial.print("THINGZANDSHIT");
//}
//release_token_list(token_list);

alreadyCalled = false;
index = 0;
}
void dialEmUp() {
  if (client.connect(server, 80)) {
    index = 0;
    overflowed = false;
    Serial.println("connected to server"); 
    // Make a HTTP request:
    client.println("GET /isItHannukah/api/doTheyKnow/?forceDay=5 HTTP/1.1");
    client.println("Host: dev.welikepie.com");
    client.println("Connection: close");
    client.println();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void lightEmUp(int lights){
  if(lights >= 8){
  lights = 8;
  }
  for(int i = 0; i <= lights; i++){
    if(flicker!=false){
      if(random(1000) > blinkenLichten){
        digitalWrite(nums[i],HIGH);
      }
      else{
        digitalWrite(nums[i],LOW);
      }
    }
    else{
        digitalWrite(nums[i],HIGH);
    }
  }
}

