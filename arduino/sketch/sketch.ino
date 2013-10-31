#include <aJSON.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <WiFi.h>


boolean flicker = true;
int blinkenLichten = 750;
int nums[9] = {A0,A1,A2,A3,A4,A5,2,3,4}; //first element is always on. You've got to light the menora from something ;)
char ssid[] = "Macs";
char pass[] = "!pandzior";
int keyIndex = 0;            // your network key Index number (needed only for WEP)
long interval = 100000; // interval at which to do something (milliseconds), 5 minutes in this case.
int globalLights = -1;
unsigned long previousMillis = 0; // last time update
unsigned long currentMillis;

char response[512];
int index = 0;
boolean overflowed = false;
boolean alreadyCalled = false;
int status = WL_IDLE_STATUS;
char server[] = "dev.welikepie.com";    // name address for WeLikePie servers
char* things = "{\"dayOf\":5,\"isHappening\":true}";
// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;
boolean started = false;
char *thing;
void setup() {
  //setting all of our pins explicitly to outputs.
  pinMode(nums[0], OUTPUT);
  pinMode(nums[1], OUTPUT);
  pinMode(nums[2], OUTPUT);
  pinMode(nums[3], OUTPUT);
  pinMode(nums[4], OUTPUT);
  pinMode(nums[5], OUTPUT);
  pinMode(nums[6], OUTPUT);
  pinMode(nums[7], OUTPUT);
  pinMode(nums[8], OUTPUT);
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
    testObjects(things);
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present. We need this to connect to our API server."); 
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
  
  // if you get a connection, report back via serial:
}

void loop() {
  if(started == false){
    dialEmUp();
    started = true;
  }

  currentMillis = millis(); //code block for timing between API calls to the server.
  if(currentMillis - previousMillis > interval) {
     previousMillis = currentMillis;  
     dialEmUp();
  }
    lightEmUp(globalLights);
  // if there are incoming bytes available 
  // from the server, read them and write them to a char[]
  
  while(client.available()) {
    if(index < 511){
    char c = client.read();
      response[index] = c;
      index++;      
    }
    else{
      overflowed = true;
    }
  }
  /*else if(!client.available() && index != 0){
  echoStuff();
  }*/
  
  if (!client.connected()) {
    client.stop();
  }
}

void echoStuff(){
int setInt = 0;
String JSONstring = "";
for(int i = 0; i < index; i++){  
    //some rather dumb code to parse out one-tier JSON.
    if( response[i] == '{'){
      setInt = 1;
    }
  if(setInt>0){
    Serial.print(response[i]);
    if(response[i] == '"'){
    JSONstring+="\\\"";
    }
    else if(response[i]=='\\'){
    JSONstring+="\\\\";
    }
    else{
    JSONstring+= response[i];
    }
  }
  if( response[i] == '}'){
  setInt = 0;
  }
}
Serial.print(JSONstring);
Serial.print(JSONstring.equals("{\"dayOf\":5,\"isHappening\":true}"));
char JSON[JSONstring.length()+1]; 
JSONstring.toCharArray(JSON, JSONstring.length()+1);
for(int i = 0; i < JSONstring.length(); i++){
Serial.print(JSON[i]);
}
Serial.print("trying");
//testObjects(JSON);
testObjects();


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

void testObjects(char* input) {
  Serial.print("STARTED FUNCTION BITCHES");
   aJsonObject* root = aJson.parse(input);
  if(root == NULL){
    testObjects(in);
  }
  aJsonObject* name = aJson.getObjectItem(root, "isHappening");
  if (name != NULL) {
    Serial.print("NOT NULL PARSING OF isHappening");
    boolean th = name->valuebool;
    if(th == 255 || th == 1 || th == true){
      Serial.println("KICKING IT");
       aJsonObject* dayOf = aJson.getObjectItem(root, "dayOf");
        if (dayOf != NULL) {
          int th = dayOf->valueint;
            globalLights = th;
        }
        aJson.deleteItem(dayOf);
    }
    else if(th == 0 || th == false){
      Serial.println("KICKING OFF");
    }
  }   
  aJson.deleteItem(root);
}

void lightEmUp(int lights){
  if(lights>0){
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
}

