#include "Arduino.h"
#include "Sequencer.h"

Sequencer::Sequencer() {

};

void Sequencer::initialize(uint8_t ch, uint8_t seqLength){
	
	_channel = ch;
	_sequenceLength = seqLength;

};	

void Sequencer::setSequenceLength(uint8_t steps){
	
	_sequenceLength = steps;

};

void Sequencer::setScale(uint8_t scaleIndex){
// need to figure this one out - to quantize notes played to certain scales
};		

void Sequencer::setStepPitch(uint8_t step, uint8_t pitch){

	_stepPitch[step] = pitch;

};

void Sequencer::setGateLength(uint8_t step, uint8_t length){
	
	_gateLength[step] = length;

};

void Sequencer::setGateType(uint8_t step, uint8_t gate){

	_gateType[step] = gate;

}

void Sequencer::setStepVelocity(uint8_t step, uint8_t velocity){
	
	_stepVelocity[step] = velocity;

};

void Sequencer::setStepGlide(uint8_t step, uint8_t glideTime){

	_stepGlide[step] = glideTime;

}

void Sequencer::setStepLength(float length){

	_stepLength = length;

};

