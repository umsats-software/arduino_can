
// Arduino Due - CAN Sample 1
// Brief CAN example for Arduino Due
// Test the transmission from CAN0 Mailbox 0 to CAN1 Mailbox 0
// By Thibaut Viard/Wilfredo Molina/Collin Kidder 2013

// Required libraries
#include "variant.h"
#include <due_can.h>
#include <Scheduler.h>
//Leave defined if you use native port, comment if using programming port
//#define Serial SerialUSB

// CAN frame max data length
#define MAX_CAN_FRAME_DATA_LEN   8

void setup()
{
  // Setup the 3 pins as OUTPUT
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  
  CAN_FRAME output;
  // Initialize CAN0 and CAN1, Set the proper baud rates here
  Can0.begin(CAN_BPS_500K);
  Can1.begin(CAN_BPS_500K);
  
  Can1.watchFor();  
  
  // Add "loop2" and "loop3" to scheduling.
  // "loop" is always started by default.
  Scheduler.startLoop(loop2);
}

void sendData()
{
  CAN_FRAME outgoing;
  
  // Prepare transmit ID, data and data length in CAN0 mailbox 0
  outgoing.id = 0x400;
  outgoing.extended = false;
  outgoing.priority = 4; //0-15 lower is higher priority
  output.length = MAX_CAN_FRAME_DATA_LEN;
  //Set first four bytes (32 bits) all at once
  output.data.low = 0x01234567;
  //Set last four bytes (32 bits) all at once
  output.data.high = 0x89ABCDEF;
  //Send out the frame on whichever mailbox is free or queue it for
  //sending when there is an opening.
  Can0.sendFrame(outgoing);
}

// Task no.1: blink LED with 1 second delay.
void loop() {
  // IMPORTANT:
  // When multiple tasks are running 'delay' passes control to
  // other tasks while waiting and guarantees they get executed.
  static bool complete = false;
  static unsigned long lastTime = millis();
  if(!complete)
  {
    if ((millis() - lastTime) > 30000) 
    {
       digitalWrite(led1, HIGH); 
       complete = true;
    }
  }
  yield();
}

void loop2(){

  CAN_FRAME incoming;
  static unsigned long lastTime = 0;

  if (Can1.available() > 0) {
  Can1.read(incoming);
  //Can0.sendFrame(incoming);
   }
  if (Can0.available() > 0) {
  Can0.read(incoming);
  //Can1.sendFrame(incoming);
  }

  if ((millis() - lastTime) > 1000) 
  {
     lastTime = millis();
     sendData();    
  }
  yield();
}

