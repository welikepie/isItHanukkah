#Is It Hannukah?#

##What is it?##

Is It Hannukah is a project consisting of a backend API and an arduino project, with the sole intention of developing a Menorah using the web.

The backend API is RESTful with three seperate endpoints at current, written in PHP, documented further down this document.

The Arduino portion is a sketch.ino file which lets the end user create their own menorah by providing the middleman between their circuits and the IsItHannukah API (some setup required).

#Arduino Implementation#

##What is it?##
An Arduino sketch which turns your arduino in to a Menorah. It uses a custom created API to find out of it is indeed Hannukah. 
To run this sketch, internet conenctivity is required. The .ino file caters to a WiFi shield being used, but could also be re-written to use the Ethernet shield instead.

##How do I get it working?##
To get the sketch up and running, you will need a version of the Arduino IDE that supports the WiFi shield.

Once the Arduino IDE is running, we need to replace a file to fix a bug in the memory management code included with the IDE.

###Replacing the malloc.c file###
Malloc.c is the file which governs the memory management in the Arduino core. As the program this code was sourced from was patched after it was included 
with Arduino, we need to include this patched code.

The way we do this is by first finding where the malloc.c file lives in the Arduino folder. 

On windows, this will be the directory you installed Arduino to, usually Program Files(x86)/Arduino, with the filepath hardware/arduino/cores/arduino/avr-libc relative from that point.

On mac, this will usually be the Applications folder. We then need to right click on the Arduino install, and select "Show package contents". From this point, the filepath is /Contents/Resources/Java/hardware/arduino/cores/arduino/avr-libc.

Having now found malloc.c, we need to replace it with the malloc.c file found in the arduino folder of the repository.

###Outputs for lights###
As the code stands at the time of commit, the outputs are as follows;
- A0 is the output for the Shamash (The candle that's always on during Hannukah)
- A1 is the output for the candle lit on the first day of Hannukah
- A2 is the output for the candle lit on the second day of Hannukah
- A3 is the output for the candle lit on the third day of Hannukah
- A4 is the output for the candle lit on the fourth day of Hannukah
- A5 is the output for the candle lit on the fifth day of Hannukah
- 2 is the output for the candle lit on the sixth day of Hannukah
- 3 is the output for the candle lit on the seventh day of Hannukah
- 4 is the output for the candle lit on the eighth day of Hannukah

As the wiring of the circuit is dependant on what you wish to achieve, that is left as an exercise to the reader.

###Installing the aJson library###

To install the aJson library (and indeed any other library), there is a guide written by the Arduino team [at this site.](http://arduino.cc/en/Guide/Libraries)
The files for the library are contained in the aJson-master.zip file which is contained in the arduino folder of the repository.

###Running the Sketch###

Now we have patched the IDE and imported the libraries, The sketch.ino file contains the code necessary for the Menorah. What we do then, is load the sketch.ino file, 
which is found in the arduino/sketch/Visual Micro folder in to the Arduino IDE. Note that the repository also includes a Visual Micro project (for use with Visual Studio). 
Independant on the method of opening the sketch, the previous patching and imported libraries will still take effect.

Upon opening the sketch, there are some configuring steps which need to take place for the menorah.

- The "flicker" boolean is used to determine of the output produces a flickering effect or not.
- The "blinkenlichten" integer determines how many times, out of 1000, the output is set to off.
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

##Function List;##

	doTheyKnow
	isSundown
	isSunrise

###doTheyKnow###


End point: /api/doTheyKnow/index.php
	
	Functions:
		doTheyKnow()
		doTheyKnow(float lat, float lon)
		doTheyKnow(int forceDay)

Overloaded endpoint.
 
In its instance of taking no arguments, it returns whether it is Hannukah or not in London, England.
In its instance of taking two arguments, it returns whether it is Hannukah or not at whatever latitude (lat) and longitude (lon) are provided.
In its instance of taking one argument, it returns as if Hannukah is happening on the day specified by the forceDay parameter.

Returned object has the attribute of "isHappening" regardless, with true marking it is hannukah, and false marking it as not being Hannukah.
If isHappning is true, the "dayOf" attribute will be set to the current day of Hannukah. 
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
		isSundown()
		isSundown(float lat, float lon)

Overloaded endpoint.
In its instance of taking no arguments, it returns an object specifying whether the sun has set in London, England.
In its instance of taking no arguments, it returns an object specifying whether the sun has set at the provided latitude and longitude.

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
		isSunrise()
		isSunrise(float lat, float lon)

Overloaded endpoint.
In its instance of taking no arguments, it returns an object specifying whether the sun has risen in London, England.
In its instance of taking no arguments, it returns an object specifying whether the sun has risen at the provided latitude and longitude.

Returned Object has three attributes; one to denote each type of sunrise. These each have a boolean flag attached to them to check if that type of sunrise has happened yet.

Example;

	{
		"civil_sunrise":false,
		"nautical_sunrise":false,
		"astronomical_sunrise":true
	}

