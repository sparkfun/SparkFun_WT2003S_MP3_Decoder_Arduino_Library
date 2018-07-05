/*
  WT2003 S Example2_Hardware_Busy_Pin

  This example shows how to hook up and use the hardware busy pin to know when the WT2003S is playing a file
  
  Using the WT2003S MP3 Decoder IC on the SparkFun breakout board with Arduino Uno
  By: Owen Lyke
  SparkFun Electronics
  Date: July 5th 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14810

  Hardware Connections:
  WT2003S Breakout Pin ---> Arduino Pin 
  -------------------------------------
  TXO                  --->   2
  RXI                  --->   3
  5V                   --->  5V
  GND                  ---> GND
  BUSY                 --->   7 (Needed for busy signal)
*/

#include "SparkFun_WT2003S_MP3_Decoder.h" // Click here to get the library: http://librarymanager/All#SparkFun_WT2003S

// Defining some status codes from the WT2003S
#define STATUS_PLAY 0x01
#define STATUS_STOP 0x02
#define STATUS_PAUSE 0x03

#define BUSY_PIN_NUMBER 7

SoftwareSerial mp3_sws(2,3);    // Use software serial so that the Arduino hardware serial can be used for control commands

WT2003S MP3;                    // Create an object of the WT2003S class called MP3
        
bool autoplay = true;           

void setup() {
//  Serial.begin(9600);
  Serial.begin(115200);
  while(!Serial){};
  MP3.begin(mp3_sws, BUSY_PIN_NUMBER);      // Adding the second parameter (after the serial port) allows you to choose a busy pin number

  Serial.println("MP3 Breakout - Example2 Hardware Busy Pin Usage");
  Serial.println();

  Serial.println("This example will qeury the playing status both in software and in hardware.");
  Serial.println("- the getPlayStatus() method uses software");
  Serial.println("- and the isPlaying() method samples the busy pin");
  
  Serial.print("Number of tracks on SD card: "); 
  Serial.println(MP3.getSongCount());

  if(MP3.getSongCount() == 0)
  {
    Serial.println("Oh no! No songs found... try adding songs or plugging in the sd card then try again");
    while(1);   // Hold here
  }

  printMenu();

  MP3.setVolume(4);
  MP3.playTrackNumber(1);
}

void loop(){
  if(Serial.available())
  {
    char cmd = Serial.read();

    if(cmd == 'p')
    {
      uint8_t status = MP3.getPlayStatus();
      if(status == STATUS_PLAY)
      {
//        Serial.println("Paused: ('p' to resume)");
//        autoplay = false;
        MP3.pause();                          // Should pause when currently playing
      }
      else if(status == STATUS_PAUSE)
      {
        MP3.pause();                          // Should play when already paused
      }
      else if(status == STATUS_STOP)
      {
        MP3.pause();                          // Plays from beginning of current track
      }
    }
    else if(cmd == 's')
    {
      Serial.println("Stopping: ('p' to play)");
      autoplay = false;
      MP3.stopPlaying();
    }
    else
    {
      printMenu();
    }
    
  }

      Serial.print("Is anything playing? Software says: ");
    if(MP3.getPlayStatus() == STATUS_PLAY)
    {
      Serial.print("yes, ");
    }
    else
    {
      Serial.print("no, ");
    }
    Serial.print(" hardware says: ");
    if(MP3.isPlaying() == true)
    {
      Serial.print("yes. ");
    }
    else
    {
      Serial.print("no. ");
    }
    Serial.println();
    delay(250);
}

void printMenu( void )
{
  Serial.println("MP3 Command List:");
  Serial.println("-----------------");
  Serial.println("'p'         : play or pause");
  Serial.println("'s'         : stop playing");
  Serial.println();
}

