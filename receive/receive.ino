//Receiver Sketch for Arduino Cubesat Comms Test
//Using the RadioHead Arduino Library
//Using a FS1000A ASK UHF transmitter/receiver module
//Written for testing by WINSAT
//Receiver board input RadioHead default of pin 11
//By: Harrison Scarfone

#include <RH_ASK.h>    //our module uses ASK modulation so we must include this package
#include <SPI.h>    //needed for compiling
#define successPin 5  //define our display led pins
#define errPin 6
#define isReceivePin 7

RH_ASK receiverBoard; //create an object for our receiver board

void setup(){

  digitalWrite(successPin, HIGH);   //flash leds, show start
  digitalWrite(errPin, HIGH);
  digitalWrite(isReceivePin, HIGH);
  delay(1000);
  digitalWrite(successPin, LOW);    //turn off leds, begin
  digitalWrite(errPin, LOW);
  digitalWrite(isReceivePin, LOW);

  Serial.begin(9600);     //decalre max data rate between board/computer
  Serial.println("start");

  if(!receiverBoard.init()){     //.init() returns boolean from intialization
    Serial.println("Intialization Error");  //prints to serial terminal
    digitalWrite(errPin, HIGH);        //show error on red led
    delay(20000);
  }else{
    digitalWrite(successPin, HIGH);   //show success on green led
    delay(2000);
    digitalWrite(successPin, LOW);
  }
}

void loop(){
  digitalWrite(isReceivePin, HIGH);
  uint8_t buffer[10];    //declare a 8 byte buffer to store incoming
  uint8_t bufferLength= sizeof(buffer);  //determine how many bytes are free in buffer

  if (receiverBoard.recv(buffer, &bufferLength)){  //recv function takes buffer and pointer to length
    Serial.println("=====================================");
    Serial.println("Message Received: ");     //returns a boolean if received good
    Serial.println((char*)buffer);          //if msg avail, copy to buf
    //digitalWrite(isReceivePin, HIGH);
    //digitalWrite(errPin, HIGH);
  }
  /*else{                        //if good copy, put len in buflen
    Serial.println("error");
    digitalWrite(isReceivePin, LOW);
    delay(2000);
    digitalWrite(errPin, HIGH);
  }*/
}
