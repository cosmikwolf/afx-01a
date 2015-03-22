#ifndef Sequencer_h
#define Sequencer_h


#include <NoteDatum.h>

class Sequencer
{
	public:	
		Sequencer();
		
		void initialize(uint8_t ch, uint8_t stepCount, uint8_t beatCount, uint16_t tempo);
		void setTempo(uint8_t bpm);
		void setScale(uint8_t scaleIndex);		
		void setStepPitch(uint8_t step, uint8_t pitch);
		void setGateLength(uint8_t step, uint8_t length);
		void setGateType(uint8_t step, uint8_t gate);
		void setStepVelocity(uint8_t step, uint8_t velocity);
		void setStepGlide(uint8_t step, uint8_t glideTime);
		void setStepDivider(uint8_t divider);
		void incrementBeatTracker();
		void resetbeatTracker();
		void resetStepTimer();
		void resetSequenceTimer();
		void runSequence(NoteDatum *noteData);
		uint8_t getStepPitch(uint8_t step);
		uint8_t beatTracker;		// keeps track of how long the sequence has been playing
		uint8_t	activeStep;
    uint32_t beatLength;

		void calculateLastActiveSteps();
		void calculateStepTimers();
//		void calculateStepLengthMcs();
		void pulseTempo(uint32_t beatLength);
		void clockStart();
		uint8_t 	stepCount;  		// sequence length in 1/16th notes]
		uint8_t		beatCount;
		uint16_t 	tempo;
		uint8_t 	channel;
		uint8_t 	programmedLength;
    uint8_t 	lastActiveStep;
		//uint8_t 	noteStatus[128];				// boolean which tells if the note is playing or not
    //elapsedMicros	noteTimer[128];  // timer for each note, set to 0 when note is activated, times when noteOff event arrives.
		elapsedMicros stepTimer;			// timer for step to step
		elapsedMicros sequenceTimer; // timer for sequence interval to sequence interval
		elapsedMicros beatTimer;
		//elapsedMicros tempoTimer;		// internal tempo time keeper	for sync purposes
		boolean       tempoPulse;
		boolean				firstBeat;		// this signal is sent when midi clock starts.
		uint32_t 			beatCounter;	// internal beat counter for sync purposes
		uint32_t stepLength;
		int positive_modulo(int i, int n);

		// http://www.happybearsoftware.com/implementing-a-dynamic-array.html

		struct StepDatum {
			uint16_t			beat;			    // beat in which the note is triggered - recalculated each beat
			uint32_t			offset;		    // note start time offset in mcs from the beat start - recalculated each beat
			uint8_t				pitch;		    // note pitch
			uint8_t				gateLength;		// gate length
			uint8_t 			gateType;			// gate type (hold, repeat, arpeggio)
			uint8_t				noteStatus;		// if note is playing or not
			uint8_t				notePlaying;		// stores the note that is played so it can be turned off.
			uint8_t				velocity;	    // note velocity
			uint8_t				glide;				// portamento time - to be implemented.
			uint32_t			lengthMcs;	  // length timer for step in microseconds.
			uint32_t			noteTimerMcs;
			elapsedMicros	stepTimer;		// a timer to compare with lengthMcs to determine when to send noteOff.

		};
	 	
		StepDatum stepData[64];
	//	NoteDatum	noteData;
//		uint8_t 	noteData[132];	

		void StepDatum_init(StepDatum *stepDatum);

		void StepDatum_append(StepDatum *stepDatum, int value);

		StepDatum StepDatum_get(StepDatum *stepDatum, int index);

		void StepDatum_set(StepDatum *stepDatum, int index, int value);

		void StepDatum_double_capacity_if_full(StepDatum *stepDatum);

		void StepDatum_free(StepDatum *stepDatum);



		// variables to replace with stepData array
		//uint8_t stepPitch[128];      // the pitch the step will play
		//uint8_t stepVelocity[128];   // the velocity of the step
		//uint8_t	stepGlide[128];			// 0-128 value that defines the fraction of the gate length that the note glides
    //unsigned long noteTimerMcs[128];		// an array of microsecond values for each step

		//uint8_t gateLength[128];     // the length of the gate in 1/16th note
		//uint8_t gateType[128];       // type of gate. 0 for note deactivated, 1 for normal gate, 2 for dashed gate


	private:
		//int a[4] = {0,0,0,0};

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
	¬µ timing:	-1000ms - 1000ms micro timing adjustments
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
		Period:								0-2œÄ
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

