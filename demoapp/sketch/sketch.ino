
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

int blinkingProbability = 750;//probability as to how many times per thousand checks the lights should stay off.
boolean flicker = true; //Boolean as to whether the lights should flicker
int nums[9] = {A0,A1,A2,A3,A4,A5,2,3,4}; //first element is always on. You've got to light the menora from something ;)
char ssid[] = ""; //WPA specific programming. Aww yiss.
char pass[] = "";//WPA password
long interval = 300000; // interval at which to check the server (milliseconds), 5 minutes in this case.
boolean goOutBool = true; //boolean as to whether the lights should turn off.
long goOutTime = 21600000; // turns itself off after 6 hours.
String apiRequest = "doTheyKnow/?forceDay=4"; //API call. Debug call to light up all of the menorah! Leave the /? in, even if not adding any specific options.

uint8_t* __brkval;
char server[] = "dev.welikepie.com";    // name address for WeLikePie servers
int blinkLight = blinkingProbability;
long goOutStart = 0;
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
  //Initialize Serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for Serial port to connect. Needed for Leonardo only
  }

    // check for the presence of the shield:
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
  // if you get a connection, report back via Serial:
}

void loop() {
  if(started == false){
	  //getting the time that loop() started at. This becomes important for timing our code execution.
	  dialEmUp();
	  startTime = millis();
	//Serial.println(startTime);
	  started = true;
  }

  currentMillis = millis() - startTime; //code block for timing between API calls to the server.
  if((currentMillis - previousMillis) > interval) {
     previousMillis = currentMillis;  
     dialEmUp(); //function call if the interval has passed.
  }
    lightEmUp(globalLights);//call lights function with globally set globalLights integer


  // if there are incoming bytes available 
  // from the server, read them and write them to a char[]
  // We start parsing on a { character and stop parsing on a } 
  // character. Very simple, 1D JSON parsing.
  while(client.available()) {
	char c = client.read();
    if(c =='{'){  
		beWrite = true;
	}
	if(beWrite == true){
	responses += c;
      //Serial.print(c);
	}
    if(c == '}'){
		//client.flush();
	beWrite = false;
	//Serial.println(freeRam());
	Serial.println(responses);
	testObjects(responses, responses.length());
	}
   }  
  if (!client.connected() ) {
	  client.stop(); //stop client if there's no more data
	  responses = NULL; //clear the memory, always clear the memory.
  }
  
}

void dialEmUp() {
//Serial.println("-----------------------------------");
//Serial.println(freeRam());
//Serial.println("-----------------------------------");
if(client.connected()){
  client.stop();
 }
  if (client.connect(server, 80)) {
    Serial.println("connected to server and carrying out API request."); 
    // Make a HTTP request:
    client.println("GET /isItHannukah/api/"+apiRequest+" HTTP/1.1");
    // forces the day to return as the eighth day of hannukah.
    client.println("Host: dev.welikepie.com");
    client.println("Connection: close");
    client.println();
  }
//Serial.println("-----------------------------------");
//Serial.println(freeRam());
//Serial.println("-----------------------------------");

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
	//Serial.println("++++++++++++++++++++++++");
	  //Serial.println(freeRam());
aJsonObject* dayOf;
aJsonObject* root;
aJsonObject* name;
char JSON[128];
//Serial.println("---");
for(int i = 0; i < 128; i++){ //128 characters are enough for the JSON object string ... right?
 JSON[i] = input.charAt(i);
}

  //Serial.println("Started Parsing Function");
   root = aJson.parse(JSON);
   //input is not what was expected and doesn't match input we would like.
  if(root == NULL){
    Serial.println("Error in the input! Ruh Roh Shaggy! You could be out of memory.");
//    testObjects(input);
  }
  else{
name = aJson.getObjectItem(root, "isHappening");
  if (name != NULL) { //retrieve whether it is Hannukah or not according to the API
    th = name->valuebool;
	//Serial.println(th);
    if(th == 255 || th == 1 || th == true){ //if true...
       dayOf = aJson.getObjectItem(root, "dayOf"); //get the dayOf attribute from the JSON
        if (dayOf != NULL) { //if it isn't null (i.e. we've ran out of memory)...
          int th = dayOf->valueint; //..get the value...
		  if(goOutBool == true){ //..and check if the lights are supposed to go out...
			  if(globalLights!=th){//..if they are...
				goOutStart = millis(); //..start a number to count from
			  }
		  }
          globalLights = th; //..set the global lights ammount to the ammount from the API
	    }
	    if(dayOf == NULL){
			Serial.println("The parsing couldn't happen. Are you running out of memory?");
		}
    }else{
		Serial.println("Isn't Hannukah yet!");
		globalLights = -1;

	}
  }   else{Serial.println("Parsing out the day of Hannukah failed. Are you running out memory?");}
//	Serial.println(freeRam());
  if(root != NULL){
	aJson.deleteItem(root);
	for(int i = 0; i <= 8; i++){//cycle through light
		digitalWrite(nums[i],LOW);
	}
  }
 // Serial.println();
 // Serial.println(freeRam());
 // Serial.println("De- Malloc'ed all the mems.");
 }
}

int freeRam ()  //function for checking free ram
{
  extern int __heap_start; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


void lightEmUp(int lights){ //LIGHTS!
  if(lights>=0){ //if number passed in is greater than eight set to eight.
    if(lights >= 8){
      lights = 8;
    }
	if((millis() - goOutStart) <= goOutTime || goOutBool == false){ //check if lights shouldn't go out (or choice for them to go out is disabled)
	if(blinkLight > blinkingProbability){--blinkLight;}
		for(int i = 0; i <= 8; i++){//cycle through lights
			if(i <= lights){		//if I less than | equal to lights
			  if(flicker==true){	//if flicker is true
				if(random(1000) > blinkLight){ //random percentage to flicker on
				  digitalWrite(nums[i],HIGH); //ON!
				}
				else{
				  digitalWrite(nums[i],LOW); //OFF!
				}
			  }
			  else{
				  digitalWrite(nums[i],HIGH); //if no flicker, on
			  }
			}else{ //if not part of lights, turn off.
				digitalWrite(nums[i],LOW);
			}
		}
	}
	else{ //if supposed to fade out
	if(blinkLight < 1000){++blinkLight;} //increment temporary variable
	if(globalLights<8){
	if(flicker==true){ //flickering for the Shamash (always on)
		if(random(1000) > blinkingProbability){
		  digitalWrite(nums[0],HIGH);
		}
		else{
		  digitalWrite(nums[0],LOW);
		}
	  }
    else{
	  digitalWrite(nums[0],HIGH);
	 }	//same as the fade in code, except the variable used to determine if on or off is incremented towards its maximum
		for(int i = 1; i <= 8; i++){
			if(i <= lights){
			  if(flicker==true){
				if(random(1000) > blinkLight){
				  digitalWrite(nums[i],HIGH);
				}
				else{
				  digitalWrite(nums[i],LOW);
				}
			  }
			  else{
				  digitalWrite(nums[i],LOW);//and the lights set to off if it isn't flickering. 
			  }
			}else{
				digitalWrite(nums[i],LOW);
			}
		}
	}
	else{
	for(int i = 0; i <= 8; i++){
			if(i <= lights){
			  if(flicker==true){
				if(random(1000) > blinkLight){
				  digitalWrite(nums[i],HIGH);
				}
				else{
				  digitalWrite(nums[i],LOW);
				}
			  }
			  else{
				  digitalWrite(nums[i],LOW);//and the lights set to off if it isn't flickering. 
			  }
			}else{
				digitalWrite(nums[i],LOW);
			}
		}
	}
	}
  }
}