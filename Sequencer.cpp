#include "Arduino.h"
#include "Sequencer.h"
#include <Fluxamasynth.h>

Sequencer::Sequencer() {

};

void Sequencer::initialize(uint8_t ch, uint8_t seqLength, uint8_t divider, uint16_t tempo, uint8_t instrumentSelection, Fluxamasynth synth1){
	_channel = ch;
	_sequenceLength = seqLength;
	_tempo = tempo;
	_stepDivider = divider;
	_stepTimer = 0;
	_sequenceTimer = 0;
	instrument = instrumentSelection;
	synth = synth1;
	for (int i=0; i < 8; i++){
		_gateLength[i] = 1;
		_gateType[i] = 1;
	}
	calculateStepLengthMcs();
	calculateStepTimers();
	calculateLastActiveSteps();
  synth.programChange(0, _channel, instrument);

};	

void Sequencer::changeInstrument(uint8_t selection){
	instrument = selection;
  synth.programChange(0, _channel, instrument);
}

void Sequencer::setSequenceLength(uint8_t steps){
	_sequenceLength = steps;
};

void Sequencer::setTempo(uint8_t bpm){
	_tempo = bpm;
	calculateStepLengthMcs();
}

void Sequencer::calculateStepLengthMcs(){
	_stepLengthMcs = (60000000/_tempo) / _stepDivider;
}

void Sequencer::setScale(uint8_t scaleIndex){
// need to figure this one out - to quantize notes played to certain scales
};		

void Sequencer::setStepPitch(uint8_t step, uint8_t pitch){
	_stepPitch[step] = pitch;
};

void Sequencer::setGateLength(uint8_t step, uint8_t length){
	_gateLength[step] = length;
  calculateLastActiveSteps();
	calculateStepLengthMcs();
  calculateStepTimers();
};

void Sequencer::setGateType(uint8_t step, uint8_t gate){
	_gateType[step] = gate;
  calculateLastActiveSteps();
}

void Sequencer::setStepVelocity(uint8_t step, uint8_t velocity){
	_stepVelocity[step] = velocity;
};

void Sequencer::setStepGlide(uint8_t step, uint8_t glideTime){
	_stepGlide[step] = glideTime;
}

void Sequencer::setStepDivider(uint8_t divider){
	_stepDivider = divider;
	calculateStepLengthMcs();
};

void Sequencer::calculateLastActiveSteps(){
  _programmedLength = 0;

  for (int n = 0; n < 128; n++){
    if (_gateType[n] != 0){
      _programmedLength += _gateLength[n];
      _lastActiveStep = n;
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
	_stepTimer = 0;
}

void Sequencer::resetSequenceTimer(){
	_sequenceTimer = 0;
}

void Sequencer::calculateStepTimers(){
	for (int i = 0; i < 128; i++){
		stepTimerMcs[i] = _gateLength[i]*_stepLengthMcs;
	}
}

void Sequencer::runSequence( ){
//	if (_stepTimer > _gateLength[activeStep]*(60*1000000/_tempo) * _stepLengthMcs) {
	if (_stepTimer > stepTimerMcs[activeStep]) {
    // change the note
    // increment activeStep
    // turn the gate on if step is active
    // reset stepTimer

    if ( clockTracker >= _sequenceLength ){
      // if the sequence has ended, go back to the beginning
      activeStep = 0;
      clockTracker = 0;
    } else if ( clockTracker == _programmedLength ) {
      // if the sequence is repeating, go back to the beginning
   	  activeStep = 0;
      testBoolean = true;
    } else {
    	testBoolean = false;
      activeStep++;
    }

    if (_gateType[activeStep] != 0){
      synth.noteOn(_channel, _stepPitch[activeStep], 64);   
    }

    _stepTimer = 0;
  //  _nextStepTimer = 500000;//_gateLength[activeStep]*_stepLengthMcs;
  }

  if (_stepTimer > (_gateLength[activeStep]-2000)) {
    // turn gate off if step is active
    if (_gateType[activeStep] != 0){
      synth.noteOff(_channel, _stepPitch[activeStep]);   
    }

  }
}

uint8_t Sequencer::getStepPitch(uint8_t step){
	return _stepPitch[step];
};
