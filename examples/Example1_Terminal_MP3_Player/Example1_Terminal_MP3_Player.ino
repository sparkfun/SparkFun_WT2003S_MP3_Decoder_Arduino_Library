#include "SparkFun_WT2003S_MP3_Decoder.h"

SoftwareSerial mp3_sws(2,3);

WT2003S MP3;

#define NUM_TRACKS 4

#define STATUS_PLAY 0x01
#define STATUS_STOP 0x02
#define STATUS_PAUSE 0x03

bool autoplay = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){};
  MP3.begin(mp3_sws, 7);    // Its pretty cool cause you can pass either a Hardware serial port or a software serial port!

  Serial.println("MP3 Breakout - Example1 Serial Control");
  Serial.println();

  printMenu();

  Serial.print("Number of tracks on SD card: "); Serial.println(MP3.getSongCount());

  MP3.setVolume(4);
  MP3.playTrackNumber(1);
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
      delay(250);           // It takes a moment for the device to know the next somg name
      MP3.getSongName();
      Serial.print("Playing: "); Serial.write(MP3.songName, 8); Serial.println();
    }
    else if(cmd == 'l')
    {
      Serial.println("Last track:");
      MP3.playPrevious();
      delay(250);           // It takes a moment for the device to know the next somg name
      MP3.getSongName();
      Serial.print("Playing: "); Serial.write(MP3.songName, 8); Serial.println();
    }
    else if(cmd == 'p')
    {
      uint8_t status = MP3.getPlayStatus();
      if(status == STATUS_PLAY)
      {
        Serial.println("Paused: ('p' to resume)");
        autoplay = false;
        MP3.pause();  // Should pause
      }
      else if(status == STATUS_PAUSE)
      {
        Serial.println("Resuming:");
        autoplay = true;
        MP3.pause();  // Should play
        delay(100);   // Prevents autoplay from jumping to the next track right away
      }
      else if(status == STATUS_STOP)
      {
        MP3.pause();  // Plays from beginning of current track
        delay(250);           // It takes a moment for the device to know the next somg name
        MP3.getSongName();
        autoplay = true;
        Serial.print("Playing: "); Serial.write(MP3.songName, 8); Serial.println();
      }
    }
    else if(cmd == 's')
    {
      Serial.println("Stopping: ('p' to play)");
      autoplay = false;
      MP3.stopPlaying();
    }
    else if(cmd == 'a')
    {
      autoplay = !autoplay;
      Serial.print("Autoplay is ");
      if(autoplay)
      {
        Serial.println(" on");
      }
      else
      {
        Serial.println(" off");
      }
    }
    else
    {
      printMenu();
    }
  }

  // Autoplay controller
  if((MP3.isPlaying() == false) && (autoplay == true))
  {
    MP3.playNext();
    delay(250);           // It takes a moment for the device to know the next somg name
    MP3.getSongName();
    Serial.print("Playing: "); Serial.write(MP3.songName, 8); Serial.println();
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
  Serial.println("'a'         : toggle autoplay");
  Serial.println();
}

