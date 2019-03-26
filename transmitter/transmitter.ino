//Transmitter Sketch for Arduino Cubesat Comms Test
//Using the RadioHead Arduino Library
//Using a FS1000A ASK UHF transmitter/receiver module
//Transmitter board input RadioHead default of pin 12
//Written for testing by WINSAT
//By: Harrison Scarfone


#include <RH_ASK.h>    //our module uses ASK modulation so we must include this package
#include <SPI.h>    //needed for compiling
#include <TFT.h>

#define successPin 5  //define our display led pins
#define errPin 6
#define isTransPin 7

//RH_ASK transmitterBoard(2000,2,3,4,false);          //create a radiohead ASK object
RH_ASK transmitterBoard;
String output = "";
String dOutput = "";
void setup(){

  pinMode(successPin, OUTPUT);
  pinMode(errPin, OUTPUT);
  pinMode(isTransPin, OUTPUT);
  digitalWrite(successPin, HIGH);   //flash leds, show start
  digitalWrite(errPin, HIGH);
  digitalWrite(isTransPin, HIGH);
  delay(1000);
  digitalWrite(successPin, LOW);    //turn off leds, begin
  digitalWrite(errPin, LOW);
  digitalWrite(isTransPin, LOW);

  Serial.begin(9600);     //decalre max data rate between board/computer
  if(!transmitterBoard.init()){     //.init() returns boolean from intialization
    Serial.println("Intialization Error");  //prints to serial terminal
    digitalWrite(errPin, HIGH);       //show error on red led
    delay(20000);
  }else{
    digitalWrite(successPin, HIGH);   //show success on green led
    delay(2000);
    digitalWrite(successPin, LOW);
  }
}

void loop(){
  digitalWrite(isTransPin, LOW);
  String output = Serial.readString();
  //Serial.println(output);
  if(output != "" && output != dOutput){
    dOutput = output;
    const char *dataTrans = output.c_str();   
    transmitterBoard.send((uint8_t *)dataTrans, strlen(dataTrans));   //(what to send, number of packets)
    transmitterBoard.waitPacketSent();            //wait for packets to be sent
    digitalWrite(isTransPin, HIGH);
    if(transmitterBoard.txGood() < 1){
      digitalWrite(errPin, HIGH);     //sending no packets shows error light
      digitalWrite(isTransPin, LOW);
      delay(5000);
    }
  }
}
