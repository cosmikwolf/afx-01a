#include "Arduino.h"
#include "Sequencer.h"

Sequencer::Sequencer() {

};

void Sequencer::initialize(uint8_t ch, uint8_t stepCount, uint8_t beatCount, uint16_t tempo){
	this->channel = ch;
	this->stepCount = stepCount;
  this->beatCount = beatCount;
	this->tempo = tempo;
	this->stepTimer = 0;
	this->sequenceTimer = 0;
	for (int i=0; i < 64; i++){
    stepData[i].beat = 0;
    stepData[i].offset = 0;
    stepData[i].pitch = 0;
    stepData[i].gateLength = 1;
    stepData[i].gateType = 0;
    stepData[i].velocity = 127;
    stepData[i].glide = 0;
    stepData[i].lengthMcs = 0;
    stepData[i].stepTimer = 0;
	};
//	calculateStepLengthMcs();
  beatLength = 60000000/tempo;
  calculateStepTimers();
  calculateLastActiveSteps();

};	

void Sequencer::setTempo(uint8_t bpm){
	tempo = bpm;
}

void Sequencer::setScale(uint8_t scaleIndex){
// need to figure this one out - to quantize notes played to certain scales
};		

void Sequencer::setStepPitch(uint8_t step, uint8_t pitch){
	stepData[step].pitch = pitch;
};

void Sequencer::setGateLength(uint8_t step, uint8_t length){
	stepData[step].gateLength = length;
  calculateLastActiveSteps();
  calculateStepTimers();
};

void Sequencer::setGateType(uint8_t step, uint8_t gate){
	stepData[step].gateType = gate;
  calculateLastActiveSteps();
}

void Sequencer::setStepVelocity(uint8_t step, uint8_t velocity){
	stepData[step].velocity = velocity;
};

void Sequencer::setStepGlide(uint8_t step, uint8_t glideTime){
	stepData[step].glide = glideTime;
}


void Sequencer::calculateLastActiveSteps(){
  programmedLength = 0;

  for (int n = 0; n < 128; n++){
    if (stepData[n].gateType != 0){
      programmedLength += stepData[n].gateLength;
      lastActiveStep = n;
    }
  }
}

void Sequencer::incrementBeatTracker(){
	beatTracker++;
};

void Sequencer::resetbeatTracker(){
	beatTracker = 0;
};

void Sequencer::resetStepTimer(){
	stepTimer = 0;
}

void Sequencer::resetSequenceTimer(){
	sequenceTimer = 0;
}

void Sequencer::calculateStepTimers(){
  uint32_t noteTimerMcsCounter = 0;
  stepLength = beatLength*beatCount/stepCount;

 // Serial.println(" stepCount: " + String(stepCount) + " stepLength: " + String(stepLength) + " beatLength: " + String(beatLength) + " tempo: " + String(tempo));
  int temp = 0;
  for (int stepNum = 0; stepNum < 64; stepNum++){
    stepData[stepNum].noteTimerMcs = (stepData[stepNum].gateLength*stepLength);
    stepData[stepNum].beat = floor(noteTimerMcsCounter / beatLength) ;// positive_modulo(stepNum* stepLength,beatLength);
    stepData[stepNum].offset = stepNum*stepLength;
    noteTimerMcsCounter = noteTimerMcsCounter + stepData[stepNum].noteTimerMcs;
/*
    Serial.println( String(channel) + " " + String(stepNum) + " " +
      "ntm: " + String(stepData[stepNum].noteTimerMcs) +
      "\tbt: " + String(stepData[stepNum].beat) +
      "\toff: " + String(stepData[stepNum].offset)  + 
      "\tgaL: " + String(stepData[stepNum].gateLength) + 
      "\tgaT: " + String(stepData[stepNum].gateType) +
      "\tptch: " + String(stepData[stepNum].pitch) +
      "\tOC: " + String(noteTimerMcsCounter) 
    );
*/
    
  }
  // offsetFromCurrentBeat = stepNum * stepLength  - floor( stepNum*stepLength /beatLength)

 // for (int stepNum = 0; stepNum < 64; stepNum++){
 //   stepData[stepNum].noteTimerMcs = stepData[stepNum].gateLength*beatLength;
 //   stepData[stepNum].beat = long(stepLengthOffsetCounter/beatLength);
 //   stepData[stepNum].offset = stepNum * beatCount/stepCount - stepCount * stepData[stepNum].beat;
 //   stepLengthOffsetCounter = stepLengthOffsetCounter + stepData[stepNum].offset;
 // }
}

void Sequencer::clockStart(){
  firstBeat = true;
};


void Sequencer::pulseTempo(uint32_t beatLength){
  // this is sent every 24 pulses received from midi clock
  // and also when a play or continue command is received.
  noInterrupts();
  this->beatLength = beatLength;
  calculateStepTimers();
  if(channel == 1){
    Serial.println("pulseTempo - beatlength: " + String(int(beatLength)) + "\tmillis: " + String(millis()));
  }
  beatTimer = 0;

  tempoPulse = true;

  if(firstBeat){  
    activeStep = 0;
    stepTimer = 0;
    beatTracker = 0;
    firstBeat = false; 
  } else {
    //beatTracker= positive_modulo(beatTracker + 1, beatCount); 
    beatTracker = (beatTracker + 1) % beatCount;
  }

  if (beatTracker == 0) {
    for(int i = 0; i < 64; i++){
      // reset the note status for notes that have been played.
      // leave notes that have not been turned off yet.
    //  if (stepData[i].noteStatus == 4){
    //    stepData[i].noteStatus = 0;
    //  }
    }
    activeStep = 0;
    stepTimer = 0;
    if(channel == 1){
      Serial.println("Resetting Sequence Timer " + String(sequenceTimer));
    }
    sequenceTimer = 0;
  } 
  interrupts();

};

void Sequencer::runSequence(NoteDatum *noteData){
  // a[0] = noteOn action boolean
  // a[1] = channel
  // a[2] = note pitch
  // a[3] = note velocity
  // a[4] = noteOff action boolean
  // a[5] = noteOff pitch

  // reset the note data being sent to the master clock
//  noteData[0] = 0;
//  noteData[4] = 0;

  noteData->noteOff = false;
  noteData->noteOn = false;
  for(int i = 0; i < 16; i++){
    noteData->noteOnArray[i] = NULL;
    noteData->noteVelArray[i] = NULL;
    noteData->noteOffArray[i] = NULL;
  }
  noteData->channel = 0;         
  noteData->noteOnStep = 0;      
  noteData->noteOffStep = 0;     

//  if ( sequenceTimer >= (15*1000000/tempo)*stepDivider ){
//    incrementBeatTracker();
//    resetSequenceTimer();
//  }

// if (beatTimer > beatLength){
//   beatTracker++;
//   beatTimer = 0;
// }

 //if (beatTracker  >= beatCount){
 //  beatTracker = 0;
 //}
  if (stepTimer > stepLength) {
    activeStep = positive_modulo(activeStep + 1, stepCount);
    stepTimer = 0;
  }

//  int n = 4;
  // figure out which notes need to be turned off
  for(int i = 0; i < 64; i++){
    if ( (stepData[i].stepTimer > stepData[i].noteTimerMcs) && (stepData[i].noteStatus == 1) ){
      // if the note is playing and has played out the gate length, end the note.
      noteData->noteOff = true;
      noteData->channel = channel;
      noteData->noteOffStep = i;
      int n = 0;
      for (int f=0; f<16; f++){
        if (noteData->noteOffArray[f] == NULL){
          noteData->noteOffArray[f] = stepData[i].notePlaying;
          break;
        }
      }
      stepData[i].noteStatus = 4;

   //   noteData[4] = 1;
   //   noteData[1] = channel;
   //   n += 1;
   //   noteData[n] = stepData[i].notePlaying;
   //   stepData[i].noteStatus = 4;
     // Serial.println("Turned off a note that played for " + String(stepData[i].stepTimer));
    }
  }
  
  for (int stepNum = 0; stepNum < 64; stepNum++){
    if((stepData[stepNum].gateType != 0 ) ){
      if ( sequenceTimer >= stepData[stepNum].offset ) {
        if (stepData[stepNum].noteStatus == 0)  {
         //   if (stepNum > activeStep){
         //     activeStep = stepNum;
         //   }
          if(stepData[stepNum].pitch > 0){
             Serial.println("second condition: " + String(stepNum) + "\tst:" + String(sequenceTimer) + "   slsn:" +String(stepLength*stepNum) + "\tst:" + String(stepTimer) + "\tof:" + String(stepData[stepNum].offset) + "\tns:" + String(stepData[stepNum].noteStatus) + "\tgt:" + String(stepData[stepNum].gateType) + "\tnt:" + String(stepData[stepNum].noteTimerMcs) );
          }

          // Serial.println("Note should be played!" + String(stepNum));
          noteData->noteOn = true;
          noteData->channel = channel;
          noteData->noteOnStep = stepNum;
          for (int i=0; i<16; i++){
            if (noteData->noteOnArray[i] == NULL){
              noteData->noteOnArray[i] = stepData[stepNum].pitch;
              noteData->noteVelArray[i] = stepData[stepNum].velocity;
              break;
            }
          }
          
      //    noteData[0] = 1;
      //    noteData[1] = channel;
      //    noteData[2] = stepData[stepNum].pitch;
      //    noteData[3] = stepData[stepNum].velocity;
      //      //temp for debug
      //    noteData[127] = stepNum;
          // end temp for debug
          stepData[stepNum].stepTimer = 0;
          stepData[stepNum].noteStatus = 1;
          stepData[stepNum].notePlaying = stepData[stepNum].pitch;
          // noteStatus indicates the status of the next note
          // 0 indicates not playing, not queued
          // 1 indicates the note is currently playing
          // 2 indicates the note is currently queued.
          // 3 indicates that the note is currently playing and currently queued
          // 4 indicates that the note has been played this iteration
          // stepData[activeStep].noteStatus = stepData[activeStep].pitch;   
        }
      } else {
        if (stepData[stepNum].noteStatus == 4){
          stepData[stepNum].noteStatus = 0;
        }
      }
    }
  }
}

uint8_t Sequencer::getStepPitch(uint8_t step){
	return stepData[step].pitch;
};

int Sequencer::positive_modulo(int i, int n) {
    return (i % n + n) % n;
}


