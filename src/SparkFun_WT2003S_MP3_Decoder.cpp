/*

SparkFun_WT2003S_MP3_Decoder.cpp

Header file: SparkFun_WT2003S_MP3_Decoder.h

Created: June 2018
Last Updated: July 2018

Authors:
Owen Lyke, N. Seidle

*/

#include "SparkFun_WT2003S_MP3_Decoder.h"

WT2003S::WT2003S()
{
	
}

void WT2003S::begin(SoftwareSerial &serialPort)
{
	_hwSerialPort = NULL;
	_swSerialPort = &serialPort;

	_swSerialPort->begin(9600);

	_busyPin = NULL;

	for(uint8_t indi = 0; indi < MP3_NUM_CMD_BYTES; indi++)
	{
		commandBytes[indi] = 0x00;
	}
}

void WT2003S::begin(HardwareSerial &serialPort)
{
	_hwSerialPort = &serialPort;
	_swSerialPort = NULL;

	_hwSerialPort->begin(9600);

	_busyPin = NULL;

	for(uint8_t indi = 0; indi < MP3_NUM_CMD_BYTES; indi++)
	{
		commandBytes[indi] = 0x00;
	}
}

void WT2003S::begin(SoftwareSerial &serialPort, uint8_t pin)
{
	_hwSerialPort = NULL;
	_swSerialPort = &serialPort;

	_swSerialPort->begin(9600);

	_busyPin = pin;
	pinMode(_busyPin, INPUT);

	for(uint8_t indi = 0; indi < MP3_NUM_CMD_BYTES; indi++)
	{
		commandBytes[indi] = 0x00;
	}
}

void WT2003S::begin(HardwareSerial &serialPort, uint8_t pin)
{
	_hwSerialPort = &serialPort;
	_swSerialPort = NULL;

	_hwSerialPort->begin(9600);

	_busyPin = pin;
	pinMode(_busyPin, INPUT);

	for(uint8_t indi = 0; indi < MP3_NUM_CMD_BYTES; indi++)
	{
		commandBytes[indi] = 0x00;
	}
}

uint16_t WT2003S::getSongCount( void )
{
	commandBytes[0] = MP3_COMMAND_GET_SONG_COUNT;
  	sendCommand(1);

  	if (responseAvailable() == false) return (0); //Timeout

  	//Get three byte response
  	uint16_t response = 0xFFFF;
  	uint8_t i = 0;

  	if(_hwSerialPort == NULL)
  	{
  		while (_swSerialPort->available())
	  	{
	    	uint8_t incoming = _swSerialPort->read();
	    	if (i == 0) ; //This is throw away value 0xC5
	    	else if (i == 1) response = (incoming << 8); //MSB
	    	else if (i == 2) response |= incoming; //LSB

	    	i++;
	    	delay(1); //At 9600bps 1 byte takes 0.8ms
	  	}

  		return (response);
  	}
  	while (_hwSerialPort->available())
  	{
    	uint8_t incoming = _hwSerialPort->read();
    	if (i == 0) ; //This is throw away value 0xC5
    	else if (i == 1) response = (incoming << 8); //MSB
    	else if (i == 2) response |= incoming; //LSB

    	i++;
    	delay(1); //At 9600bps 1 byte takes 0.8ms
  	}

  	return (response);
}
void 		WT2003S::getSongName()
{
	commandBytes[0] = MP3_COMMAND_GET_SONG_NAME_PLAYING;
  	sendCommand(1);

  	if (responseAvailable() == false)
  	{
    	songName[0] = 'E';
    	songName[1] = 'r';
    	songName[2] = 'r';
    	songName[3] = 'o';
    	songName[4] = 'r';
    	songName[5] = '\0';
    	return;
	}

	//Get 9 byte response
	uint8_t i = 0;

	if(_hwSerialPort == NULL)
	{
		while (_swSerialPort->available())
		{
			uint8_t incoming = _swSerialPort->read();
			if (i == 0) ; //This is throw away value 0xCB
			else if (i < 9) songName[i - 1] = incoming;
			i++;
			delay(1); //At 9600bps 1 byte takes 0.8ms
		}
		songName[8] = '\0'; //Terminate this string
		return;
	}
	while (_hwSerialPort->available())
	{
		uint8_t incoming = _swSerialPort->read();
		if (i == 0) ; //This is throw away value 0xCB
		else if (i < 9) songName[i - 1] = incoming;
		i++;
		delay(1); //At 9600bps 1 byte takes 0.8ms
	}
	songName[8] = '\0'; //Terminate this string
}

uint8_t 	WT2003S::playTrackNumber(uint8_t trackNumber)
{
	commandBytes[0] = MP3_COMMAND_PLAY_INDEX_IN_ROOT;
	commandBytes[1] = trackNumber >> 8; //MSB
	commandBytes[2] = trackNumber & 0xFF; //LSB
	sendCommand(3);
	return (getResponse());
}

uint8_t 	WT2003S::playFileName(uint8_t fileNumber)
{
	commandBytes[0] = MP3_COMMAND_PLAY_FILE_IN_ROOT;
	commandBytes[1] = 'F';
	commandBytes[2] = '0' + (fileNumber / 100);
	fileNumber %= 100;
	commandBytes[3] = '0' + (fileNumber / 10);
	fileNumber %= 10;
	commandBytes[4] = '0' + fileNumber;
	sendCommand(5);
	return (getResponse());
}

uint8_t 	WT2003S::setVolume(uint8_t volumeLevel)
{
	if (volumeLevel > 31) volumeLevel = 31; //Error check
	commandBytes[0] = MP3_COMMAND_SET_VOLUME;
	commandBytes[1] = volumeLevel;
	sendCommand(2);
	return (getResponse());
}

uint8_t 	WT2003S::getVolume( void )
{
	commandBytes[0] = MP3_COMMAND_GET_VOLUME;
	sendCommand(1);

	//Get two byte response
	uint16_t volLevel = getTwoByteResponse();

	//First byte is 0xC1, second bye is volume level
	return (volLevel & 0xFF);
}

uint8_t 	WT2003S::setEQ(uint8_t eqType)
{
	if (eqType > 5) eqType = 0; //Error check. Set to normal by default
	commandBytes[0] = MP3_COMMAND_SET_EQ_MODE;
	commandBytes[1] = eqType;
	sendCommand(2);
	return (getResponse());
}

bool 		WT2003S::isPlaying( void )
{
	if(_busyPin == NULL)
	{
		return false;
	}
	if(digitalRead(_busyPin) == HIGH) return(true); //Song is playing
  	return (false);
}

uint8_t 	WT2003S::getPlayStatus( void )
{
	commandBytes[0] = MP3_COMMAND_GET_CURRENT_STATE;
	sendCommand(1);
	return (getTwoByteResponse() & 0xFF);
}

void 		WT2003S::pause( void )
{
	commandBytes[0] = MP3_COMMAND_PAUSE;
	sendCommand(1);
	return (getResponse());
}

uint8_t 	WT2003S::playNext( void )
{
	commandBytes[0] = MP3_COMMAND_NEXT;
	sendCommand(1);
	return (getResponse());
}

uint8_t 	WT2003S::playPrevious( void )
{
	commandBytes[0] = MP3_COMMAND_PREVIOUS;
	sendCommand(1);
	return (getResponse());
}

uint8_t 	WT2003S::stopPlaying( void )
{
	commandBytes[0] = MP3_COMMAND_STOP;
	sendCommand(1);
	return (getResponse());
}

void 		WT2003S::sendCommand(uint8_t commandLength)
{
	if(_hwSerialPort == NULL)
	{
		sendCommandSW(commandLength);
		return;
	}
	sendCommandHW(commandLength);
}

uint8_t 	WT2003S::getResponse( void )
{
	if (responseAvailable() == false) return (0xFF); //Timeout

  uint8_t response = 0xFE;
  uint8_t i = 0;

	if(_hwSerialPort == NULL)
	{
		while (_swSerialPort->available())
		{
			if (i++ == 0) response = _swSerialPort->read();
		}

		return (response);
	}
	while (_hwSerialPort->available())
	{
		if (i++ == 0) response = _hwSerialPort->read();
	}

	return (response);
}
uint16_t 	WT2003S::getTwoByteResponse( void )
{
	if (responseAvailable() == false) return (0xFF); //Timeout

  	uint16_t response = 0xFFFF;
  	uint8_t i = 0;

  	if(_hwSerialPort == NULL)
  	{
  		while (_swSerialPort->available())
		{
			uint8_t incoming = _swSerialPort->read();

			if (i == 0) response = incoming << 8; //MSB
			else if (i == 1) response |= incoming; //LSB

			i++;
			delay(1); //At 9600bps 1 byte takes 0.8ms
		}

		return (response);
  	}
	while (_hwSerialPort->available())
	{
		uint8_t incoming = _hwSerialPort->read();

		if (i == 0) response = incoming << 8; //MSB
		else if (i == 1) response |= incoming; //LSB

		i++;
		delay(1); //At 9600bps 1 byte takes 0.8ms
	}

	return (response);
}

bool 		WT2003S::responseAvailable( void )
{
	uint8_t counter = 0;

	if(_hwSerialPort == NULL)
	{
		while (_swSerialPort->available() == false)
		{
			delay(1); 

			if (counter++ > 250) 
			{
				return (false); //Timeout
			}
		}
		return (true);
	}
	while (_hwSerialPort->available() == false)
	{
		delay(1);

		if (counter++ > 250) 
		{
			return (false); //Timeout
		}
	}
	return (true);
}


void 		WT2003S::clearBuffer( void )
{
	if(_hwSerialPort == NULL)
	{
		while (_swSerialPort->available()) 
		{
			_swSerialPort->read();
			delay(1); //1 byte at 9600bps should take 1ms
		}
		return;
	}
	while (_hwSerialPort->available()) 
	{
		_hwSerialPort->read();
		delay(1); //1 byte at 9600bps should take 1ms
	}
}


void 		WT2003S::sendCommandHW(uint8_t commandLength)
{
clearBuffer(); //Clear anything in the buffer

	_hwSerialPort->write(MP3_START_CODE);
	_hwSerialPort->write(commandLength + 2); //Add one byte for 'length', one for CRC

	//Begin sending command bytes while calc'ing CRC
	byte crc = commandLength + 2;
	for (byte x = 0 ; x < commandLength ; x++) //Length + command code + parameter
	{
		_hwSerialPort->write(commandBytes[x]); //Send this byte
		crc += commandBytes[x]; //Add this byte to the CRC
	}

	_hwSerialPort->write(crc); //Send CRC
	_hwSerialPort->write(MP3_END_CODE);
}

void 		WT2003S::sendCommandSW(uint8_t commandLength)
{
	clearBuffer(); //Clear anything in the buffer

	_swSerialPort->write(MP3_START_CODE);
	_swSerialPort->write(commandLength + 2); //Add one byte for 'length', one for CRC

	//Begin sending command bytes while calc'ing CRC
	byte crc = commandLength + 2;
	for (byte x = 0 ; x < commandLength ; x++) //Length + command code + parameter
	{
		_swSerialPort->write(commandBytes[x]); //Send this byte
		crc += commandBytes[x]; //Add this byte to the CRC
	}

	_swSerialPort->write(crc); //Send CRC
	_swSerialPort->write(MP3_END_CODE);
}	