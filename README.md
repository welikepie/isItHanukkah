#Is It Hannukah?#

##What is it?##

_Is It Hannukah_ is a project consisting of an API and an arduino project, with the sole intention of creating a Menorah using the web.

The API is RESTful with three seperate endpoints, written in PHP, documented further down this document.

To get this running an Arduino we put a sketch.ino file on to it. This lets you create your own Menorah by providing the middleman between the Arduino circuits and the IsItHannukah API.

#Arduino Implementation#

##What is it?##
IsitHannukah uses a sketch file to turn an Arduino in to a Menorah. It uses a custom created API to find out of it is indeed Hannukah. 
**To run this sketch, internet connectivity is required.**
The sketch file that we are using for the Arduino requires a WiFi shield to be used with the Arduino. 

##How do I get it working?##
To get the sketch up and running, you will need a version of the Arduino IDE that supports the WiFi shield.
You can download the IDE here: [Arduino IDE](http://arduino.cc/en/Main/Software)

###Replacing the malloc.c file###

At the time of writing, there is a bug within the memory management code included in the Arduino IDE. The free() function used to free up memory does not work properly with the version of C/C++ included. This has been fixed in the C compiler itself but the changes have not been merged to the Arduino IDE's code base, so we've included the patched file within this repo and will tell you how to work with it.

**Malloc.c** is the file which governs the memory management in the Arduino core.

First we need to find where the malloc.c file is included within the Arduino folder.

####On Windows: 
The malloc.c file will be in the directory you installed Arduino to, usually Program Files(x86)/Arduino, with the filepath hardware/arduino/cores/arduino/avr-libc relative from that point.
So: 
> Program Files(x86)/Arduino/hardware/arduino/cores/arduino/avr-libc

####On Mac:
This will likely be in the Applications folder. Right click Arduino.app and select "_show package contents_". We then need to right click on the Arduino install, and select "Show package contents".
You'll find mallo.c here: 
> /Contents/Resources/Java/hardware/arduino/cores/arduino/avr-libc.

Once you've found the malloc.c file; copy and paste it into the "demoapp" folder of this repository. 

###Outputs for lights###
These are the outputs for the lights used in the Arduino application:

- A0 is the output for the Shamash (The candle that's always on during Hannukah)
- A1 is the output for the candle lit on the first day of Hannukah
- A2 is the output for the candle lit on the second day of Hannukah
- A3 is the output for the candle lit on the third day of Hannukah
- A4 is the output for the candle lit on the fourth day of Hannukah
- A5 is the output for the candle lit on the fifth day of Hannukah
- 2 is the output for the candle lit on the sixth day of Hannukah
- 3 is the output for the candle lit on the seventh day of Hannukah
- 4 is the output for the candle lit on the eighth day of Hannukah

Here is a wiring diagram:

- WIFI Shield not depicted, though it must be attached to the Arduino in order to get IsitHannukah working


###Installing the aJson library###

To parse a response from the API to use within the demoapp, you need to install the aJSON library into the Arduino IDE. The Arduino team have written a handy guide to help you do this, which you can find here: [aJSON Library Installation](http://arduino.cc/en/Guide/Libraries).
The files for the library are contained in the **aJson-master.zip** file which is contained in the demoapp folder of this repository.

###Running the Sketch###

Now we have patched the IDE and imported the libraries, we can get to work on our Arduino Menorah. 
The sketch.ino file contains the code necessary for the Menorah, we need to load this in to the Arduino IDE. The sketch.ino file can be found in the demoapp/sketch folder.
What we do then, is load the sketch.ino file, which can be done by opening up the Arduino IDE and File > Open and navigate to the sketch.ino file. 

Upon opening the sketch, there are some configuring steps which need to take place for the menorah.

- The "flicker" boolean is used to determine of the output produces a flickering effect or not.
- The "blinkLight" integer determines how many times, out of 1000, the output is set to off.
- The "interval" integer determines after how many milliseconds the API is re-polled. Every ten minutes works rather well. Under every 10 seconds chokes the arduino up.
- The "goOutBool" boolean determines if the lights are supposed to go out or not. Set to true to have them go out, false to stay lit.
- The "goOutTime" integer determines after how many milliseconds the lights should go out, if they should go out.
- [MANDATORY] the ssid[] is where you need to write your wireless network ID.
- [MANDATORY] the pass[] is where you need to set the password for your wireless network.
- [MANDATORY] the apiRequest is where you make you API request to the server. As of current, the only endpoint supported is the doTheyKnow endpoint.

With the sketch opened and configured, we can now upload it to the board and watch things happen.

*Note: If the sketch refuses to connect to the router, it is possible your WiFi shield has outdated firmware, which will need updating.
This is easiest done using a Mac and the instructions prepared by the Arduino folks over at [this site](http://arduino.cc/en/Hacking/WiFiShieldFirmwareUpgrading).
Carrying out the update of the board is left as an exercise for the reader.*

###Expected Behaviour###

If it isn't Hannukah, none of the outputs will function.
If it is the first day of Hannukah, the Shamash output and the first output should have output.
If it is the nth day of Hannukah, the Shamash output and the nth output should have output.

#API#
##What is it?##
This is an API to determine whether it is Hanukkah, and what day of Hannukah we currently find ourselves in.
Additionally, there is also code to determine whether the sun is down at a particular latitude/longitude pairing as well as whether the sun is up.

##How to interact with the API;##

Sending a GET request to the endpoints with the parameters specified as a query string will yield the response to the query.

##API Base URL##

At the time of publication, the base URL for the API is http://dev.welikepie.com/isItHannukah . All API endpoints can be appended to this baseURL to produce a call.

##Function List;##

	doTheyKnow
	isSundown
	isSunrise

###doTheyKnow###


End point: /api/doTheyKnow/index.php
	
	Functions:
		doTheyKnow()  -  /api/doTheyKnow/index.php
		doTheyKnow(float lat, float lon)  -  /api/doTheyKnow/index.php?lat=31&lon=32
		doTheyKnow(int forceDay)  -  /api/doTheyKnow/index.php?forceDay=5
 
- In its instance of taking no arguments, it returns whether it is Hannukah or not in London, England.
- In its instance of taking two arguments, it returns whether it is Hannukah or not at whatever latitude (lat) and longitude (lon) are provided.
- In its instance of taking one argument, it returns as if Hannukah is happening on the day specified by the forceDay parameter.

Returned object has the attribute of "isHappening" regardless, with "true" marking it is hannukah, and "false" marking it as not being Hannukah.
If isHappening is true, the "dayOf" attribute will be set to the current day of Hannukah.

Example;

	During Hannukah;
		{
			"dayOf" : 1,
			"isHappening" : true
		}
	
	Outside of Hannukah;
		{
			"isHappening" : false
		}

###isSundown###


End point: /api/isSundown/index.php

	Functions;
		isSundown()  -  /api/isSundown/index.php
		isSundown(float lat, float lon)  -  /api/isSundown/index.php?lat=31&lon=32

- In its instance of taking no arguments, it returns an object specifying whether the sun has set in London, England.
- In its instance of taking no arguments, it returns an object specifying whether the sun has set at the provided latitude and longitude.

Returned Object has three attributes; one to denote each type of sunset. These each have a boolean flag attached to them to check if that type of sunset has happened yet.

Example;

	{
		"civil_sundown":false,
		"nautical_sundown":false,
		"astronomical_sundown":false
	}

###isSunrise###


End point: /api/isSunrise/index.php

	
	Functions
		isSunrise()  -  /api/isSunrise/index.php
		isSunrise(float lat, float lon)  -  /api/isSunrise/index.php?lat=31&lon=32

- In its instance of taking no arguments, it returns an object specifying whether the sun has risen in London, England.
- In its instance of taking no arguments, it returns an object specifying whether the sun has risen at the provided latitude and longitude.

Returned Object has three attributes; one to denote each type of sunrise. These each have a boolean flag attached to them to check if that type of sunrise has happened yet.

Example;

	{
		"civil_sunrise":false,
		"nautical_sunrise":false,
		"astronomical_sunrise":true
	}

