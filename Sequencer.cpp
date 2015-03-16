#include "Arduino.h"
#include "Sequencer.h"
#include <Fluxamasynth.h>

Sequencer::Sequencer() {

};

void Sequencer::initialize(uint8_t ch, uint8_t seqLength, uint8_t divider, uint16_t tempo, uint8_t instrumentSelection){
	this->channel = ch;
	this->sequenceLength = seqLength;
	this->tempo = tempo;
	this->stepDivider = divider;
	this->stepTimer = 0;
	this->sequenceTimer = 0;
	this->instrument = instrumentSelection;
	for (int i=0; i < 16; i++){
		gateLength[i] = 1;
		gateType[i] = 0;
    stepPitch[i] = 0;
	}
	calculateStepLengthMcs();
	calculateStepTimers();
	calculateLastActiveSteps();
//  synth.programChange(0, channel, instrument);
};	

void Sequencer::changeInstrument(uint8_t selection){
	instrument = selection;
}

void Sequencer::setsequenceLength(uint8_t steps){
	sequenceLength = steps;
};

void Sequencer::setTempo(uint8_t bpm){
	tempo = bpm;
	calculateStepLengthMcs();
}

void Sequencer::calculateStepLengthMcs(){
	_stepLengthMcs = (60000000/4/tempo) * stepDivider;
}

void Sequencer::setScale(uint8_t scaleIndex){
// need to figure this one out - to quantize notes played to certain scales
};		

void Sequencer::setStepPitch(uint8_t step, uint8_t pitch){
	stepPitch[step] = pitch;
};

void Sequencer::setGateLength(uint8_t step, uint8_t length){
	gateLength[step] = length;
  calculateLastActiveSteps();
	calculateStepLengthMcs();
  calculateStepTimers();
};

void Sequencer::setGateType(uint8_t step, uint8_t gate){
	gateType[step] = gate;
  calculateLastActiveSteps();
}

void Sequencer::setStepVelocity(uint8_t step, uint8_t velocity){
	stepVelocity[step] = velocity;
};

void Sequencer::setStepGlide(uint8_t step, uint8_t glideTime){
	stepGlide[step] = glideTime;
}

void Sequencer::setStepDivider(uint8_t divider){
	stepDivider = divider;
	calculateStepLengthMcs();
  calculateStepTimers();
};

void Sequencer::calculateLastActiveSteps(){
  programmedLength = 0;

  for (int n = 0; n < 128; n++){
    if (gateType[n] != 0){
      programmedLength += gateLength[n];
      lastActiveStep = n;
    }
  }
}

void Sequencer::incrementClockTracker(){
	clockTracker++;
};

void Sequencer::resetClockTracker(){
	clockTracker = 0;
};

void Sequencer::resetStepTimer(){
	stepTimer = 0;
}

void Sequencer::resetSequenceTimer(){
	sequenceTimer = 0;
}

void Sequencer::calculateStepTimers(){
	for (int i = 0; i < 128; i++){
		noteTimerMcs[i] = gateLength[i]*_stepLengthMcs;
	}
}

uint8_t * Sequencer::runSequence( ){

//	if (stepTimer > gateLength[activeStep]*(60*1000000/tempo) * _stepLengthMcs) {
  // a[0] = noteOn action boolean
  // a[1] = channel
  // a[2] = note pitch
  // a[3] = note velocity
  // a[4] = noteOff action boolean
  // a[5] = noteOff pitch

  //memset(noteData, 0, sizeof(noteData));
  //Serial.println("ch:" + String(channel));

  noteData[0] = 0;
  noteData[4] = 0;

  if ( sequenceTimer >= (15*1000000/tempo)*stepDivider ){
    incrementClockTracker();
    resetSequenceTimer();
  }

  if (stepTimer >= noteTimerMcs[activeStep]) {

    // if the stepTimer is greater than or equal to the current gate length,
    // change the note
    // increment activeStep
    // turn the gate on if step is active
    // reset stepTimer

    if ( clockTracker >= sequenceLength ){
      // if the sequence has ended, go back to the beginning
      activeStep = 0;
      clockTracker = 0;
    } else {
      activeStep = positive_modulo(activeStep + 1, int(sequenceLength));
    }
    //Serial.println("StepTimer: " + String(stepTimer) + " noteTimer: " + String(noteTimerMcs[activeStep]));
    stepTimer = 0;

    if (gateType[activeStep] != 0){
      noteData[0] = 1;
      noteData[1] = channel;
      noteData[2] = stepPitch[activeStep];
      noteData[3] = stepVelocity[activeStep];
      noteTimer[activeStep] = 0;
      notePlaying[activeStep] = stepPitch[activeStep];
    }
  //  _nextStepTimer = 500000;//gateLength[activeStep]*_stepLengthMcs;

  }

  int n = 4;
 // Serial.println("n: ");

  for(int i = 0; i < 128; i++){
  //  Serial.println("i: " + String(i));
    if ( noteTimer[i] > noteTimerMcs[i] && notePlaying[i] != 0 ){
      // if the note is playing and has played out the gate length, end the note.
      noteData[4] = 1;
      noteData[1] = channel;
      n += 1;
      noteData[n] = notePlaying[i];
      notePlaying[i] = 0;
    } 
  }
  
  return noteData;
}

uint8_t Sequencer::getStepPitch(uint8_t step){
	return stepPitch[step];
};

int Sequencer::positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

