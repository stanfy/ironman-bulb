

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


const char *ssid = "your-ssid";
const char *password = "your-pass";

ESP8266WebServer server ( 80 );

int ledMaxVal = 255;
long interval = 1000;  
String rgb = "FFFFFF";
unsigned long previousMicros = 0;
//// Red
const int ledRed = 16;
int redVal = ledMaxVal;
int redNewVal = 70;
//// Green
const int ledGreen = 14;
int greenVal = ledMaxVal;
int greenNewVal = 70;
//// Blue
const int ledBlue = 12;
int blueVal = ledMaxVal;
int blueNewVal = 100;


////////////////////////////////////////////

void handleGet() {
	char responce[400];

	snprintf ( responce, 400,

"{\"r\": [{\"value\":\"%d\"}],\
  \"g\": [{\"value\":\"%d\"}],\
  \"b\": [{\"value\":\"%d\"}],\
  \"up\": [{\"value\":\"%d\"}]}",

	  redVal, greenVal, blueVal, (millis()/1000)
	);
	server.send ( 200, "application/json", responce );
}

void handleNotFound() {
  	String message = "404";
  	server.send ( 404, "text/plain", message );
  	Serial.print ( "HTTP args: " );
  	Serial.println (server.args());
}

void handleSet() {
	String message = "";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
  

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
		int tmp = 0;
		
		if (server.argName(i) == "rgb" )
    		{
      			if ( server.arg(i) != rgb) 
      			{
          			rgb =  server.arg(i);
          			convert(rgb, &message);
      			}
    		}
	}

	server.send ( 200, "text/plain", message );
}



int convert( String str, String *msg  ) 
{
       int number = (int) strtol( &str[0], NULL, 16);
       redNewVal = (number >> 16);
       greenNewVal = (number >> 8 & 0xFF);
       blueNewVal = (number & 0xFF);
  
}


void setup ( void ) {

	Serial.begin ( 115200 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );
  
	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

  
	server.on ( "/",  handleNotFound );
	server.on ( "/get", handleGet );
  	server.on ( "/set", handleSet );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );

  	pinMode ( ledBlue, OUTPUT );
  	pinMode ( ledGreen, OUTPUT );
  	pinMode ( ledRed, OUTPUT );
  	
	analogWriteRange(255);
  	analogWriteFreq(500);
  
  	Serial.println(redNewVal);
  	Serial.println(greenNewVal);
  	Serial.println(blueNewVal);
}

void smoothChange (int pin, int *currentVal, int newVal) 
{
	if (*currentVal > newVal) *currentVal-=1;
     	else if (*currentVal < newVal) *currentVal+=1;
     	analogWrite(pin, ledMaxVal - *currentVal);
     	yield();
}

void loop ( void ) {
	
	server.handleClient();
  	byte isChanged = 0;
  	unsigned long currentMicros = micros();
  	unsigned long diff = currentMicros - previousMicros;
  	if (diff < 0) 
  	{ 
    		diff = 1; 
    		previousMicros = 0;
  	}
  
  
  	if ( diff > 5000 )
  	{
      		if (redNewVal != redVal) smoothChange(ledRed, &redVal, redNewVal ); 
    
     		if (greenNewVal != greenVal) smoothChange(ledGreen, &greenVal, greenNewVal );
		
		if (blueNewVal != blueVal) smoothChange(ledBlue, &blueVal, blueNewVal );   

		previousMicros = currentMicros;
      
	} 
  
}



