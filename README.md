# afx-01
AFX-01 Sequencer

The beginnings of an open source hardware sequencer project based on the Teensy 3.1 and an Atmel SAM2695 sound chip.

Hardware (subject to change)
	- Teensy 3.1 Microcontroller
	- Atmel SAM2695 Sound Chip
	- 128x64 OLED Graphic LCD
	- 8x8 RGB LED note indicators
	- 8 vertical buttons
	- 8 horizontal buttons
	- 3x Rotary Encoders
	- 4x mode buttons

Feature Intentions:
	- MIDI sync
	- Live Arpeggiation Mode
	- Live Sequence Transpose Mode
	- Internal sound generation (with Atmel chip)
	- USB Bus / Battery powerable (external USB probably to start)
	- Eurorack quick mount capability? (dependins on final form factor)
	- Scale Quantization
	- Microtonal Scale Capabilities
	- Perlin Noise based generative sequences
	- Fractional Brownian Motion based iterative microtonal sequences
	- Midi Transforms
		- Pre-Quantized Randomizer
		- Cross-sequence influencer
		- Rotational logic
		- Random Step
		- Perlin noise based step
	- MIDI out
	- CV breakout / addon
		- 3x CV out
		- 3x Gate out
		- 3x CC1 out
		- 3x CC2 out
		- 4x Percussion Triggers



VARIOUS NOTES ABOUT THE PROJECT:

sequencer notes
Objective
  To create a device which will allow one to compose music on the fly without a computer, intended to sequence synthesizers in a live or studio setting. 

Simple Sample player
   - ability to play simple instruments to simulate synth sequences 
  - sine, square, triangle synth with low pass, band pass and high pass filter. 
  - envelope generator for filter and amp
  - sample synth can replace waveforms 

Step Sequencer
	- 256 step sequences possible (16 pages of 16)
	- Each step has 2 parameters
	- Note + modulation
	- Each step can also have variable length
	- Step 1 can be two beats long, while step 2 can be one beat
	- Each sequence can be measured in beats or steps
	- a 16 step sequence can be 16 beats long, or have 2 beat 	- steps and be 32 beats long
	- Each step can trigger another sequence
	- 
	- Each sequence trigger has parameters
	- Step 1 can also trigger a 


Each Set of Sequences - The controller of time
	- Can have up to 16 simultaneous sequences
	- Master Sequence (default is first sequence made, but can be 	- changed)
	- Retrigger for each sequence
	- Play once, retrigger with master sequence
	- Loop and retriever with master sequence
	- Loop with no retrigger for euclidean polyrhythms 
	- Time modifiers for each sequence
	- 
	- Each sequence - The controller of melody
	- Default note length
	- Default scale lock (scale + mode)
	- Tempo
	- Time signature (dictates step length + measure length)
	- Length in beats (max 512?)
	- Type (Step Sequence, Euclidean)
	- Methods
	- Sequence Copy

	- Random setting
		- Type (regular random, 1st order perlin, 2nd order perlin etc 	- etc
	- % chance of random trigger
	- # of choices for random influence
	- scale = multiplier of random influence - based on ableton 	- random midi patch
	- Repeat type (Mirror repeat - random will repeat in reverse on every other measure, based on previous measure)


Each Step - The controller of sound
	- Step type 
		- Note or sequence
	- Note Type:
		- Can trigger 5 notes n0..5 - on midi it will send each note. 	- If voice is monophonic, these can be used for arpeggiators
		- n0 = 0-127 note to trigger
		- n1 = Chord note 1
		- n2 = Chord note 2
		- n4 = Chord note 3
		- µT = micro timing in 1/16th of a beat time positive or 	- negative values?
	- Each note has a length, which is inherited from the sequence 	- object
	- Sequence Type
	- Sequence to play - choose from memory bank - with preview
