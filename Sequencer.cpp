#include "Arduino.h"
#include "Sequencer.h"

Sequencer::Sequencer() {

};

void Sequencer::initialize(uint8_t ch, uint8_t stepCount, uint8_t beatCount, float tempo){
	this->channel = ch;
	this->stepCount = stepCount;
  this->beatCount = beatCount;
	this->tempo = tempo;
	this->stepTimer = 0;
	this->sequenceTimer = 0;
	for (int i=0; i < 64; i++){
    stepData[i].gateLength = 1;
    stepData[i].velocity = 127;
	};
  beatLength = 60000000/tempo;
  calculateStepTimers();
  monophonic == true;
};	

void Sequencer::setTempo(uint16_t tempo){
  this->tempo = tempo;
  beatLength = 60000000/tempo;
  calculateStepTimers();
}

void Sequencer::setStepPitch(uint8_t step, uint8_t pitch){
	stepData[step].pitch = pitch;
};

void Sequencer::setGateLength(uint8_t step, uint8_t length){
	stepData[step].gateLength = length;
  calculateStepTimers();
};

void Sequencer::setGateType(uint8_t step, uint8_t gate){
	stepData[step].gateType = gate;
}

void Sequencer::setStepVelocity(uint8_t step, uint8_t velocity){
	stepData[step].velocity = velocity;
};

void Sequencer::setStepGlide(uint8_t step, uint8_t glideTime){
	stepData[step].glide = glideTime;
}

void Sequencer::calculateStepTimers(){
  uint32_t noteTimerMcsCounter = 0;
  stepLength = beatLength*beatCount/stepCount;

 // Serial.println(" stepCount: " + String(stepCount) + " stepLength: " + String(stepLength) + " beatLength: " + String(beatLength) + " tempo: " + String(tempo));
  for (int stepNum = 0; stepNum < 64; stepNum++){
    stepData[stepNum].noteTimerMcs = (stepData[stepNum].gateLength*stepLength);
    stepData[stepNum].beat = floor(noteTimerMcsCounter / beatLength);
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
}

void Sequencer::clockStart(){
  firstBeat = true;
};

void Sequencer::beatPulse(uint32_t beatLength){
  // this is sent every 24 pulses received from midi clock
  // and also when a play or continue command is received.
  noInterrupts();
  this->beatLength = beatLength;
  calculateStepTimers();
  if(channel == 1){
    Serial.println("beatPulse - beatlength: " + String(int(beatLength)) + "\tbeatTracker: " + String(beatTracker));
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
    // We are resetting the note status when the sequenceTimer is no longer larger than 
    // the offset value. This means that runSequence needs to run at least once before
    // it can reset the note statuses. For the first note, that means it wont ever reset
    // so we must reset it manually here.
    stepData[0].noteStatus = 0;
  } 
  interrupts();

};

void Sequencer::runSequence(NoteDatum *noteData){
  // clear noteData from last iteration.
  unsigned long timer = micros();

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

  if (stepTimer > stepLength) {
    activeStep = positive_modulo(activeStep + 1, stepCount);
    stepTimer = 0;
  }

  for (int stepNum = 0; stepNum < 64; stepNum++){
    // set notes to be stopped, and marked as played.
    if ( (stepData[stepNum].stepTimer > stepData[stepNum].noteTimerMcs) && (stepData[stepNum].noteStatus == 1) ){
      // if the note is playing and has played out the gate length, end the note.
      noteData->noteOff = true;
      noteData->channel = channel;
      noteData->noteOffStep = stepNum;
      int n = 0;
      for (int f=0; f<16; f++){
        if (noteData->noteOffArray[f] == NULL){
          noteData->noteOffArray[f] = stepData[stepNum].notePlaying;
          break;
        }
      }
      stepData[stepNum].noteStatus = 4;
    }

    // set notes to be played
    if((stepData[stepNum].gateType != 0 ) ){

      if ( sequenceTimer >= stepData[stepNum].offset ) {
        if (stepData[stepNum].noteStatus == 0)  {
         //   if (stepNum > activeStep){
         //     activeStep = stepNum;
         //   }
         /* if(stepData[stepNum].pitch > 0){
             Serial.println("third condition: " + String(stepNum) + "\tst:" + String(sequenceTimer) + "   slsn:" +String(stepLength*stepNum) + "\tste:" + String(stepTimer) + "\tof:" + String(stepData[stepNum].offset) + "\tns:" + String(stepData[stepNum].noteStatus) + "\tgt:" + String(stepData[stepNum].gateType) + "\tnt:" + String(stepData[stepNum].noteTimerMcs) );
            Serial.println("timekeeper: " + String(timekeeper));
          } */

          // Serial.println("Note should be played!" + String(stepNum));


          //shut off any notes that are playing.
          for (int stepNum = 0; stepNum < 64; stepNum++){
            if(stepData[stepNum].noteStatus == 1){
              noteData->noteOff = true;
              noteData->channel = channel;
              noteData->noteOffStep = stepNum;
              int n = 0;
              for (int f=0; f<16; f++){
                if (noteData->noteOffArray[f] == NULL){
                  noteData->noteOffArray[f] = stepData[stepNum].notePlaying;
                  break;
                }
              }
              stepData[stepNum].noteStatus = 4;
            }
          }
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

          noteData->triggerTime = micros();
          noteData->sequenceTime = sequenceTimer;
          noteData->offset = stepData[stepNum].offset;
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

  timekeeper = ((micros() - timer)+9*timekeeper)/10;
}

uint8_t Sequencer::getStepPitch(uint8_t step){
	return stepData[step].pitch;
};

int Sequencer::positive_modulo(int i, int n) {
    return (i % n + n) % n;
}


