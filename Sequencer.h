#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"

class Sequencer
{
	public:	
		Sequencer();
		
		void initialize(uint8_t ch, uint8_t seqLength);
		void setSequenceLength(uint8_t steps);
		void setScale(uint8_t scaleIndex);		
		void setStepPitch(uint8_t step, uint8_t pitch);
		void setGateLength(uint8_t step, uint8_t length);
		void setGateType(uint8_t step, uint8_t gate);
		void setStepVelocity(uint8_t step, uint8_t velocity);
		void setStepGlide(uint8_t step, uint8_t glideTime);
		void setStepLength(float length);

	private:
		uint8_t _sequenceLength;  		// sequence length in 1/16th notes
		uint8_t _channel;
		uint8_t _stepPitch[128];      // the pitch the step will play
		uint8_t _gateLength[128];     // the length of the gate in 1/16th note
		uint8_t _gateType[128];       // type of gate. 0 for note deactivated, 1 for normal gate, 2 for dashed gate
		uint8_t _stepVelocity[128];   // the velocity of the step
		uint8_t	_stepGlide[128];			// 0-128 value that defines the fraction of the gate length that the note glides
		float		_stepLength;					// fraction of a beat that is a step. 
};

#endif

/*

The Sequence is the brain of the AFX-01

All musical logic is set here.

Each sequence at its base is just a set of numbers, stored in a few arrays.
The index of the array equates to the step number of the note value.

At every interval of the midi engine, 
each sequencer object should be polled 
to identify what, if any, notes should 
be played, and should have already played.

Each sequence can be played differently.

HOW THIS SHOULD BE RUN:

noteToPlay(time) should be called very frequently
	it should check to see what note should have played between lastRun and now 

There is basic linear sequencing. This has a few attributes:
	Step Length:			1/32 notes - 1 whole note
	Number of steps:	1 - 128
	Retrigger step: 	0  - Last

Each step has various attributes:
	Step Type:	
		Sequence Trigger Type:
			Sequence number: 	Int
			Repeat:						boolean
		Note Trigger Type:
			
	Pitch 1: note value
	Pitch 2: note value
	Pitch 3: note value
	Velocity: 0 - 127
	µ timing:	-1000ms - 1000ms micro timing adjustments
	Note Length: 1 - max steps in sequence:

There are some modifications that are distructive changes to the sequence itself:

All generators share thes attributes:
		High Note:						highest note for generation
		Low Note:							highest note for generation
		Affects Notes:				boolean
		Affects Velocity: 		boolean
		Affects Note Length:	boolean

	Random Generator: 			Sets the sequence notes based on a random generator

	Perlin Noise Generator: Sets a sequence based on pre-rendered perlin noise
		Input Value:					Input source number for perlin noise
		Octave:								The octave of the perlin noise

	Waveform Generator:			Generate midi notes based on a graphical waveform
		Wave Form:						Sin, Square, Saw, Reverse Saw, Triangle, S+H
		Period:								0-2π
		Amplitude:						0-64
		Quantization:					Scale
	
There are some post processing functions that can modify the sequence at runtime:

	Randomization:
		% chance to randomize: 0-100%
		Max High Note: 0 - 127
		Min Low Note: 0 - 127

	Reverse Order:
		Play notes in reverse order

	Random Order
		Play notes in random Order
		Note Length Preservation: boolean 
			- preserve the original length of each note, just randomize note pitch

	Inverse:
		Play the note on the opposite side of middle C

	Quantization:
		Scale: 	The A B C D E F G scales, sharps and flats, all modes + some

*/



		/*

		other notes:
		
		Dynamic Arrays:
		http://www.fredosaurus.com/notes-cpp/newdelete/50dynamalloc.html


*/