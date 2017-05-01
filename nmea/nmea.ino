/*
 * NMEA0183 without checksum convert to checksum
 * NMEA in on pin 10
 * NMEA out on USB(pin 1)
 */

 //remove commend to enable DEBUG mode
//#define DEBUG 
#include <SoftwareSerial.h>

SoftwareSerial nmea(10, 11); // RX, TX

uint8_t getCheckSum(char *string)
{
  int XOR = 0;  
  for (int i = 0; i < strlen(string); i++)
  {
    XOR = XOR ^ string[i];
  }
  return XOR;
}

char Output[250];
String inData;
bool debug = true;
uint8_t checksum;

void setup() {
  // initialize serial:
  Serial.begin(4800);
  nmea.begin(4800);
  #ifdef DEBUG
    Serial.println("DEBUG Mode enabled!");
    Serial.println("Don't forget to turn it off");
  #endif
  //set the char array to 0
  for( int i = 0; i < sizeof(Output);  ++i )
        Output[i] = (char)0;
}

void loop() {
  // if there's any serial available, read it:
  while (nmea.available() > 0) {
    // look for the start of the line.
    if (nmea.read() == '$') {
      //read the serial until end of line
      inData = nmea.readStringUntil('\n');
      inData.trim();//remove \r\n
      //convert to char array
      inData.toCharArray(Output, (inData.length() + 1));
      //print the incomming data
      Serial.print('$');
      Serial.print(inData);
      Serial.print('*');
      Serial.println(getCheckSum(Output),HEX);  //print the checksum
    }
  }
  if (Serial.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    #ifdef DEBUG
    if (Serial.read() == '$') {//if debug send data back with checksum
      //read the serial until end of line
      inData = Serial.readStringUntil('\n');
      inData.trim();//remove \r\n
      
      //convert to char array
      inData.toCharArray(Output, (inData.length() + 1));
      //print the incomming data
      Serial.print('$');
      Serial.print(inData);
      Serial.print('*');
      Serial.println(getCheckSum(Output),HEX);  //print the checksum
    }
    #else
      nmea.write(Serial.read());   // read it and send it out Serial (USB)
    #endif 
  }
}








