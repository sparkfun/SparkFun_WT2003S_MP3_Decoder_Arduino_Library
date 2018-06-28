# description
The WT2003S is an all-in-one SD card based MP3 player. It combines the steps of obtaining a bitstream that represents audio and decoding that bitstream into a stereo audio output. A microSD card is used to store MP3 files to be played later. On the WT2003S MP3 Decoder Breakout there is also a built in audio amplifier, 3.5mm stereo headphone jack, and mono speaker output that uses a poke-home connector to make rapid prototyping a cinch. To get going just insert a microSD card with your desired files, connect the breakout to a serial port, and use the provided Arduino library for full-featured control. 

Here's what sets the WT2003S breakout board apart from other products:
- Provides direct access to the WT2003S serial interface, as opposed to the [*Qwiic MP3 Trigger*](link).
- Small form factor and single-sided PCB simplifies mounting for a project.
- Built-in amplifier allows direct connection of a speaker up to 1.4W. 

- There are no *trigger* pins that can directly play an MP3 when a certain event happens, but you can make that happen in your project with a microcontroller.
- The WT2003S only supports MP3 and WAV files, and cannot *encode* audio like the [MP3 Decoder Shield]().

You may prefer to use this product instead of the [Qwiic version](link) if you don't already use I2C in a project, have an extra serial port, or want to control the WT2003S from a computer using a common USB to serial converter. 

It's always nice to have a dedicated microSD card for a given project so consider purchasing a spare [small-capacity microSD card](link) in case you don't have one already.


# features
- Operating voltage: 3.3V
- Current consumption:
- - 40mA standy
- - 40mA when playing over headphones at any volume level
- - 150mA-300mA when driving external 8Ω speaker at full volume setting
WT2003S MP3 decoder IC provides USB access to microSD socket (up to 32GB) and MP3 decoding
Poke-home connector allows for sturdy but temporary speaker connection without soldering
Built-in TPA2005D1 based 1.4W Class-D mono amplifier

# documents
[Eagle Files]()
[Arduino Library](https://github.com/sparkfun/SparkFun_WT2003S_MP3_Decoder_Arduino_Library)
[WT2003S Datasheet]()
Please check the [repo]() for the most recent firmware and hardware design files