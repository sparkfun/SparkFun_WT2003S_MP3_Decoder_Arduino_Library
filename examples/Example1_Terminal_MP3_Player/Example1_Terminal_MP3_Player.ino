/*
  WT2003 S Example1_Terminal_MP3_Player

  This example makes an MP3 player that can be controlled from the terminal window on a computer. 
  It exposes the full set of commands that are available for the WT2003S
 
  Using the WT2003S MP3 Decoder IC on the SparkFun breakout board with Arduino Uno
  By: Owen Lyke, Tina Tenbergen
  SparkFun Electronics
  Date: July 5th 2018, Aug 9 2018
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
  BUSY                 --->   7 (Optional)

  Don't forget to load some MP3s on your sdCard and plug it in too!
*/

#include "SparkFun_WT2003S_MP3_Decoder.h" // Click here to get the library: http://librarymanager/All#SparkFun_WT2003S

// Defining some status codes from the WT2003S
#define STATUS_PLAY 0x01
#define STATUS_STOP 0x02
#define STATUS_PAUSE 0x03

#define songNameGetDelay 500 // ms

SoftwareSerial mp3_sws(2,3);    // Use software serial so that the Arduino hardware serial can be used for control commands

WT2003S MP3;                    // Create an object of the WT2003S class called MP3       

void setup() {
  Serial.begin(9600);
  while(!Serial){};
  MP3.begin(mp3_sws);      // Beginning the MP3 player requires a serial port (either hardware or software!!!)

  Serial.println("MP3 Breakout - Example1 Serial Control");
  Serial.println();

  printMenu();
  
  Serial.print("Number of tracks on SD card: "); 
  Serial.println(MP3.getSongCount());

  if(MP3.getSongCount() == 0)
  {
    Serial.println("Oh no! No songs found... try adding songs or plugging in the sd card then try again");
    while(1);   // Hold here
  }

  MP3.setVolume(4);
}

void loop() {
  if(Serial.available())
  {
    char cmd = Serial.read();
    if(cmd == '+')
    {
      Serial.print("Volume up: ");
      uint8_t volume = MP3.getVolume();
      MP3.setVolume(++volume);
      Serial.print(volume);
      Serial.println("/31");
    }
    else if(cmd == '-')
    {
      Serial.print("Volume down: ");
      uint8_t volume = MP3.getVolume();
      if(--volume > 31)
      {
        volume = 0;
      }
      MP3.setVolume(volume);
      Serial.print(volume);
      Serial.println("/31");
    }
    else if(cmd == 'n')
    {
      Serial.println("Next track:");
      MP3.playNext();
      delay(songNameGetDelay);           // It takes a moment for the device to start playing the next song
      MP3.getSongName();
      Serial.print("Playing: "); 
      Serial.write(MP3.songName, 8); 
      Serial.println();
    }
    else if(cmd == 'l')
    {
      Serial.println("Last track:");
      MP3.playPrevious();
      delay(songNameGetDelay);           // It takes a moment for the device to know the next song name
      MP3.getSongName();
      Serial.print("Playing: "); 
      Serial.write(MP3.songName, 8); 
      Serial.println();
    }
    else if(cmd == 'p')
    {
      uint8_t status = MP3.getPlayStatus();
      if(status == STATUS_PLAY)
      {
        Serial.println("Paused: ('p' to resume)");
        MP3.pause();                          // Should pause when currently playing
      }
      else if(status == STATUS_PAUSE)
      {
        MP3.pause();                          // Should play when already paused
        delay(songNameGetDelay);                           // It takes a moment for the device to know the next song name
        Serial.print("Resuming: ");
        MP3.getSongName();
        Serial.write(MP3.songName, 8); 
        Serial.println();
      }
      else if(status == STATUS_STOP)
      {
        MP3.pause();                          // Plays from beginning of current track
        delay(songNameGetDelay);                           // It takes a moment for the device to know the next song name
        MP3.getSongName();
        Serial.print("Playing: "); 
        Serial.write(MP3.songName, 8); 
        Serial.println();
      }
    }
    else if(cmd == 's')
    {
      Serial.println("Stopping: ('p' to play)");
      MP3.stopPlaying();
    }
    else if(cmd == '?')
    {
      uint8_t status = MP3.getPlayStatus();
      if(status == STATUS_PLAY)
      {
        delay(songNameGetDelay);
        MP3.getSongName();
        Serial.print("Playing: "); 
        Serial.write(MP3.songName, 8); 
        Serial.println();
      }
      else if(status == STATUS_STOP)
      {
        Serial.println("MP3 player is stopped");
      }
      else
      {
        Serial.println("MP3 player is paused");
      }
    }
    else if(cmd == 'w')
    { 
      Serial.println("Set play mode to 'SingleNoLoop'");
		  MP3.setPlaymodeSingleNoLoop();
    }  
    else if(cmd == 'x')
    { 
      Serial.println("Set play mode to 'SingleLoop'");
		  MP3.setPlaymodeSingleLoop();
	  }
    else if(cmd == 'y')
    { 
      Serial.println("Set play mode to 'AllLoop'");
		  MP3.setPlaymodeAllLoop();
	  }    
	  else if(cmd == 'z')
    { 
      Serial.println("Set play mode to 'Random'");
		  MP3.setPlaymodeRandom();
	  }
	  else if((cmd <= '9') && (cmd >= '1'))
    { 
      uint8_t numSongs = MP3.getSongCount();
      if(numSongs >= (cmd - '0'))
      {
        MP3.playTrackNumber(cmd - '0');
        Serial.print("Playing track #");
        Serial.print(cmd - '0');
        Serial.print(" (in copy-to-sd order)");
        delay(songNameGetDelay);           // Give it time to switch songs
        MP3.getSongName();
        Serial.print(": "); 
        Serial.write(MP3.songName, 8); 
        Serial.println();
      }
      else
      {
        Serial.print("Error: There are only ");
        Serial.print(numSongs);
        Serial.println(" songs on the SD card.");
        Serial.print("You asked to play song # ");
        Serial.print(cmd - '0');
        Serial.println(".");
      }
    }
    else
    {
      printMenu();
    }
  }
}

void printMenu( void )
{
  Serial.println("MP3 Command List:");
  Serial.println("-----------------");
  Serial.println("'+' or '-'  : raise/lower volume");
  Serial.println("'n' or 'l'  : next/last song");
  Serial.println("'p'         : play or pause");
  Serial.println("'s'         : stop playing");
  Serial.println("'?'         : what is playing?");
  Serial.println("'w'         : set playmode single no loop");
  Serial.println("'x'         : set playmode single loop");
  Serial.println("'y'         : set playmode all loop");
  Serial.println("'z'         : set playmode random");
  Serial.println("'X'         : play the file that was copied to the SD card as the Xth file - you just type the number you want - (x in [1,9])");
  Serial.println("             (Yes, this really does go by copy order.)");
  Serial.println();
  Serial.println("Any other key to show this menu");
  Serial.println();
}

