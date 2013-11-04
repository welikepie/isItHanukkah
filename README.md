#Is It Hannukah?#

#Arduino Implementation#
##What is it?##
This is a sketch which needs the arduino Wi-Fi shield to function. It also needs LED lights wired up to the outputs as denoted by the nums[] array.

Once the Arduino is wired up, the malloc.c file in the Arduino compiler needs to be replaced, owing to a memory allocation bug in the compiler.
What this means in a practical sense is that your program will stop running without this modification after a relatively short period of time.

To replace the malloc.c file, locate your arduino installation directory and replace the malloc.c file. In addition to adding the malloc.c file,
the aJSON zipped file needs to be installed to your Arduino compiler.

Once this has been done, then the .ino file in the arduino folder can be uploaded to the board using a compiler of choice. Once the board has been
 primed with the code, it is a case of powering the board up for a joyfully created menorah.

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

