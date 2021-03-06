/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example RF Radio Ping Pair
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two different nodes,
 * connect the role_pin to ground on one.  The ping node sends the current time to the pong node,
 * which responds by sending the value back.  The ping node can then see how long the whole cycle
 * took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

// sets the role of this unit in hardware.  Connect to GND to be the 'pong' receiver
// Leave open to be the 'ping' transmitter
const int role_pin = 7;

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  The hardware itself specifies
// which node it is.
//
// This is done through the role_pin
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role;

// time array
int timeArray[]={0,2,5,1,3,4,6};
//time variable
int t=0;
// temporary variable
float temp=0;
void setup(void)
{
  //
  // Role
  //
  role=role_pong_back;
  //
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/pingpair/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);

  
  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
  printf("Press T to transmit, press R to recieve\n");
}

void loop(void)
{
  temp=millis()/1000.0;
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  automated();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 
  // Ping out role.  Repeatedly send the current time
  //
  if(Serial.available()){
    //Serial.println(Serial.read());
    if(Serial.read()==84 || Serial.read()==116 ){
      Serial.println("****Transmit mode Press R to switch back to Recieve mode");
      role=role_ping_out;
      manual();
    }else if (Serial.read()==82 || Serial.read()==114){
      Serial.println("****Recieve mode, Press T to switch back to transmit mode");
      role=role_pong_back;
      manual();
    }else{
      automated();
    }
  }
  
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
void automated(){
  Serial.println("Automated Jogger Mate Mode..");
    t=rand()%(sizeof(timeArray)-1);
  // if there is data ready
     listen(t);
      t=rand()%(sizeof(timeArray)-1);
      if ( radio.available() )
      {
        listen(t);
      }else{
        transmit(5);    
      }
}
void listen(int wait){
  Serial.println("Auto Recieve Mode for "+(String)wait+" Seconds");
  temp=millis()/1000;
  while((millis()/1000.0-temp)<wait){
    if ( role == role_pong_back )
      {
        // if there is data ready
        if ( radio.available() )
        {
          // Dump the payloads until we've gotten everything
          unsigned long got_time;
          bool done = false;
          while (!done)
          {
            // Fetch the payload, and see if this was the last one.
            done = radio.read( &got_time, sizeof(unsigned long) );

            // Spew it
            printf("Got payload %lu...",got_time);

      // Delay just a little bit to let the other unit
      // make the transition to receiver
      delay(20);
          }

          // First, stop listening so we can talk
          radio.stopListening();

          // Send the final one back.
          radio.write( &got_time, sizeof(unsigned long) );
          printf("Sent response.\n\r");

          // Now, resume listening so we catch the next packets.
          radio.startListening();
        }
      }
    }
}
void transmit(int wait){
  Serial.println("Auto Transmit Mode.."+(String)wait+" Seconds");
      // First, stop listening so we can talk.
          radio.stopListening();

          // Take the time, and send it.  This will block until complete
          unsigned long time = millis();
          printf("Now sending %lu...",time);
          bool ok = radio.write( &time, sizeof(unsigned long) );
          
          if (ok)
            printf("ok...");
          else
            printf("failed.\n\r");

          // Now, continue listening
          radio.startListening();

          // Wait here until we get a response, or timeout (250ms)
          unsigned long started_waiting_at = millis();
          bool timeout = false;
          while ( ! radio.available() && ! timeout )
            if (millis() - started_waiting_at > 200 )
              timeout = true;

          // Describe the results
          if ( timeout )
          {
            printf("Failed, response timed out.\n\r");
          }
          else
          {
            // Grab the response, compare, and send to debugging spew
            unsigned long got_time;
            radio.read( &got_time, sizeof(unsigned long) );

            // Spew it
            printf("Got response %lu, round-trip delay: %lu\n\r",got_time,millis()-got_time);
              }

  }
  void manual(){
    Serial.println("Manual Mode..");
    if (role == role_ping_out)
  {
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    unsigned long time = millis();
    printf("Now sending %lu...",time);
    bool ok = radio.write( &time, sizeof(unsigned long) );
    
    if (ok)
      printf("ok...");
    else
      printf("failed.\n\r");

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_time;
      radio.read( &got_time, sizeof(unsigned long) );

      // Spew it
      printf("Got response %lu, round-trip delay: %lu\n\r",got_time,millis()-got_time);
    }

    // Try again 1s later
    delay(1000);
  }

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  if ( role == role_pong_back )
  {
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_time;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &got_time, sizeof(unsigned long) );

        // Spew it
        printf("Got payload %lu...",got_time);

  // Delay just a little bit to let the other unit
  // make the transition to receiver
  delay(20);
      }

      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      radio.write( &got_time, sizeof(unsigned long) );
      printf("Sent response.\n\r");

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
  }
  }
