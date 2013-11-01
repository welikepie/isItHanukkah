
struct __freelist
{
  size_t sz;
  struct __freelist *nx;
};

extern struct __freelist *__flp;
//extern uint8_t* __brkval;

#include <aJSON.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <WiFi.h>

boolean flicker = true;
int blinkenLichten = 750;
int nums[9] = {A0,A1,A2,A3,A4,A5,2,3,4}; //first element is always on. You've got to light the menora from something ;)
char ssid[] = "Macs"; //WPA2 specific programming. Aww yiss.
char pass[] = "!pandzior";
char server[] = "dev.welikepie.com";    // name address for WeLikePie servers

uint8_t* __brkval;
long interval = 10000; // interval at which to do something (milliseconds), 5 minutes in this case.
long startTime = 0;
int globalLights = -1;
unsigned long previousMillis = 0; // last time update
unsigned long currentMillis;
String JSONstring;
String responses = "";
boolean beWrite = false;
int status = WL_IDLE_STATUS;
WiFiClient client;
boolean started = false;
String s= "{\"dayOf\":2,\"isHappening\":true}";
boolean th;

void setup() {
  Serial.println("-=====-");
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

    // check for the presence of the shield:
 // echoStuff(s);  
  if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present. We need this to connect to our API server. We're stopping for now."); 
      // don't continue:
      while(true);
    } 
  Serial.println("After WiFi status");
  // attempt to connect to Wifi network:
  if (status != WL_CONNECTED) { 
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
	startTime = millis();
	Serial.println(startTime);
	  started = true;
  }

  currentMillis = millis() - startTime; //code block for timing between API calls to the server.
  if((currentMillis - previousMillis) > interval) {
     previousMillis = currentMillis;  
     dialEmUp();
  //echoStuff(s);  
  }
    lightEmUp(globalLights);
  // if there are incoming bytes available 
  // from the server, read them and write them to a char[]
 

  while(client.available()) {
	char c = client.read();
    if(c =='{'){  
		beWrite = true;
	}
	if(beWrite == true){
	responses += c;
      Serial.print(c);
	}
    if(c == '}'){
		//client.flush();
		//echoStuff(responses);
	beWrite = false;
	Serial.println(freeRam());
	echoStuff(responses);
	}
   }  
  if (!client.connected() ) {
	  client.stop();
	  responses = NULL;
  }
  
}

void echoStuff(String inputs){
Serial.println("STARTING ECHO");
  Serial.println(freeRam());
JSONstring = "";
  Serial.println(freeRam());
//Serial.println(indexor);
for(int i = 0; i < inputs.length(); i++){  
    //some rather dumb code to parse out one-tier JSON.
    //Serial.println(inputs[i]);
	  Serial.println(freeRam());
    if(inputs[i] == '"'){
    //JSONstring+="\\\"";
    JSONstring+='"';  
	}
    else if(inputs[i]=='\\'){
//    JSONstring+="\\\\";
    }
    else{
    JSONstring+= inputs[i];
    }
  
}

//Serial.print("trying");
testObjects(JSONstring, JSONstring.length());
JSONstring = NULL;
}

void dialEmUp() {
Serial.println("-----------------------------------");
Serial.println(freeRam());
Serial.println("-----------------------------------");
if(client.connected()){
  client.stop();
 }
  if (client.connect(server, 80)) {
    Serial.println("connected to server"); 
    // Make a HTTP request:
    client.println("GET /isItHannukah/api/doTheyKnow/?forceDay=5 HTTP/1.1");
    client.println("Host: dev.welikepie.com");
    client.println("Connection: close");
    client.println();
  }
Serial.println("-----------------------------------");
Serial.println(freeRam());
Serial.println("-----------------------------------");

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

void testObjects(String input, int longness) {
	Serial.println("++++++++++++++++++++++++");
	  Serial.println(freeRam());
aJsonObject* dayOf;
aJsonObject* root;
aJsonObject* name;
char JSON[64];
Serial.println("---");
for(int i = 0; i < 64; i++){
  JSON[i] = input.charAt(i);
}
  Serial.println("STARTED FUNCTION BITCHES");
   root = aJson.parse(JSON);
   //input is not what was expected and doesn't match input we would like.
  if(root == NULL){
    Serial.println("recursive error!");
//    testObjects(input);
  }
  else{
name = aJson.getObjectItem(root, "isHappening");
  if (name != NULL) {
    th = name->valuebool;
	Serial.println(th);
    if(th == 255 || th == 1 || th == true){
       dayOf = aJson.getObjectItem(root, "dayOf");
        if (dayOf != NULL) {
          int th = dayOf->valueint;
		  //globalLights = 0;
		  //globalLights = random(0,8);
          globalLights = th;
			Serial.println();
			Serial.print(globalLights);
			Serial.print(":");
			Serial.print(th);
	    }
	    if(dayOf == NULL){
			Serial.println("Some shit died.");
		}
//aJson.deleteItem(dayOf);
    }else{Serial.println("ISNOT HANNUKAH");}
//aJson.deleteItem(name);
  }   else{Serial.println("ISDAY_IS_NULL");}
//aJson.deleteItem(root);
    Serial.println();
	Serial.println(freeRam());
  if(root != NULL){
	aJson.deleteItem(root);
  }
  Serial.println();
  Serial.println(freeRam());
  Serial.println("De- Malloc'ed all the mems.");
 }
}

int freeRam () 
{
  extern int __heap_start; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


void lightEmUp(int lights){
  if(lights>=0){
    if(lights >= 8){
      lights = 8;
    }
    for(int i = 0; i <= 8; i++){
		if(i <= lights){
		  if(flicker==true){
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
		}else{
			digitalWrite(nums[i],LOW);
		}
    }
  }
}