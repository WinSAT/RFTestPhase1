//Transmitter Sketch for Arduino Cubesat Comms Test
//Using the RadioHead Arduino Library
//Using a FS1000A ASK UHF transmitter/receiver module
//Transmitter board input RadioHead default of pin 12
//Written for testing by WINSAT
//By: Harrison Scarfone


#include <RH_ASK.h>    //our module uses ASK modulation so we must include this package
#include <SPI.h>    //needed for compiling
#include <TFT.h>
#include <stdio.h>

#define successPin 5  //define our display led pins
#define errPin 6
#define isTransPin 7

String output = "";
String dOutput = "";

RH_ASK transmitterBoard;          //create a radiohead ASK object
int count = 0;
char transmitBuffer[255]; // since RH_ASK.send only uses a uint8_t to denote length, the max amount of information that can be sent is 255 bytes (or a string of length 254)

unsigned long send_large_data(RH_ASK *transmitter, const uint8_t * dOut, unsigned long dataLength);

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


/*
 * send_large_data
 * Aaron Li
 * Solution for sending more than 255 bytes at a time.
 * Sends the data in MAX_BURST_SIZE byte chunks when possible and waits for the data to be transmitted
 * returns the number of bytes succesfully transmitted, breaking if a burst of data did not transmit correctly
 * 
 * In an image of 8 bit rgb pixels you can send 21845 uncompressed full colour pixels with 65535 bytes or 1,431,655,765 uncompressed full colour pixels with ~4GB
 * 
*/

#define MAX_BURST_SIZE 255

unsigned int send_large_data(RH_ASK *transmitter, uint8_t * dOut, unsigned int dataLength){
  //max length is 2**16-1 (65,535 | ~64KB) on ATMEGA based boards and 2**32-1 (4,294,967,295 | ~4GB) on the Due, or whatever a size_t is (returned by strlen)
  unsigned int sent = 0;

  for(unsigned int i = 0; i<dataLength; i+=MAX_BURST_SIZE){
    uint8_t burst_length = (uint8_t)min(dataLength - i, MAX_BURST_SIZE); // we're either sending the full burst or what's left of the message
    bool can_send = transmitter->send(dOut+i, burst_length); // start the data pointer at the offset and send up to the burst length

    if(!can_send){ // "the message is too long for the underlying radio technology"
      break;
    }
    transmitter->waitPacketSent(); // wait for burst to send

    uint8_t sent_bytes = transmitter->txGood(); // check how many bytes were sent

    if(sent_bytes != burst_length){ // if the number sent is not the amount we wanted to send
      break; // error occured
    }
    else{
      sent += sent_bytes; // add sent count to our total sent
    }
  }
  return sent;
}
