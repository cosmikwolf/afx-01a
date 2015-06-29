elapsedMicros masterTempoTimer = 0;
boolean wasPlaying = false;
uint8_t masterPulseCount =24;
double bpm;
float avgBpm;
boolean tempoBlip = false;
boolean firstRun = false;
elapsedMicros blipTimer = 0;
unsigned long beatLength = 60000000/tempo;
unsigned long avgDelta;
elapsedMicros testTimer;
elapsedMicros pulseTimer;
unsigned long lastPulseLength;
unsigned long avgPulseLength;
unsigned long avgPulseJitter;
unsigned long pulseLength;
unsigned long lastBeatLength;
unsigned long lastMicros;
unsigned long avgInterval;
unsigned long lastAvgInterval;
unsigned long intervalJitter;
unsigned long avgIntervalJitter;
unsigned long lastTimer;
unsigned long timerAvg;
elapsedMicros printTimer;

void changeTempo(uint16_t newTempo){
  tempo = newTempo;
  beatLength = 60000000/tempo;

}

void masterClockFunc(){
	// this timer runs once every millisecond 
     // Serial.println("master Clock Start");
  elapsedMicros loopTimer = 0;
  avgInterval =((micros() - lastMicros) + 9* avgInterval) / 10;
  timerAvg = (lastTimer + 9*timerAvg) /10;
  lastMicros = micros();
  
  intervalJitter = (abs(int(avgInterval) - int(lastAvgInterval)));
  avgIntervalJitter = (intervalJitter * 9 + avgIntervalJitter) / 10;
  lastAvgInterval = avgInterval;

// if (printTimer > 200000) {
//   Serial.println("avgInterval: " + String(avgInterval) + "\ttimerAvg: " + String(timerAvg) + "\tavgIntervalJitter: " + String(avgIntervalJitter));
//   printTimer = 0;
// }

  iter = iter + 1;

 //  Serial.println("timer loop 2");
  if(playing){

    if( extClock == false ){ 
      // int clock
      if (wasPlaying == false){
        Serial.println("TestTimer: " + String(testTimer));
        // if playing has just re-started, the master tempo timer and the master beat count must be reset
        MIDI.send(Start, 0, 0, 1);
        MIDI.sendSongPosition(0);
        masterTempoTimer = 0;
        masterPulseCount = 0;
        internalClockTimer = 0;
        startTime = 0;

        for (int i=0; i< sequenceCount; i++){
          sequence[i].clockStart(startTime);
          sequence[i].beatPulse(beatLength);
          sequence[i].runSequence(&noteData[i]);
        }

      } else {
      //  Serial.print(" b1 ");

        for (int i=0; i< sequenceCount; i++){
         sequence[i].runSequence(&noteData[i]);
        } 
       // Serial.print(" b2 " );

      }

      if (internalClockTimer > 60000000/tempo){
       // Serial.print(" b4 ");
       if (queuePattern != currentPattern) {
          
          changePattern(queuePattern, true, true);
       }

        for (int i=0; i< sequenceCount; i++){
          sequence[i].beatPulse(beatLength);  
        }
        tempoBlip = !tempoBlip;
        internalClockTimer = 0;
       // Serial.print(" b5 ");
      }
    } else {
      // ext clock sync
      for (int i=0; i< sequenceCount; i++){
        sequence[i].runSequence(&noteData[i]);
      }
    }


    for (int i=0; i< sequenceCount; i++){
      if (noteData[i].noteOff == true){
        for (int n=0; n< 16; n++){
          if (noteData[i].noteOffArray[n] == NULL){
            continue;
          }
         // noteOn(noteData[i].channel,noteData[i].noteOffArray[n]);
          MIDI.sendNoteOff(noteData[i].noteOffArray[n], 64, noteData[i].channel);
         // usbMIDI.sendNoteOff(noteData[i].noteOffArray[n], 64, noteData[i].channel);
          //Serial.println("noteOff: " + String(noteData[i].noteOffArray[n]) + "\tbt: " + String(sequence[selectedSequence].beatTracker) ) ;
        }
      }
    }


    for (int i=0; i< sequenceCount; i++){
      if (noteData[i].noteOn == true){
        for (int n=0; n< 16; n++){
          if (noteData[i].noteOnArray[n] == NULL){
            continue;
          }
        //  noteOn(noteData[i].channel,noteData[i].noteOnArray[n]);

        //   
          /*
          if (sequence[selectedSequence].quantizeKey == 1){
              bool quantized = false;

              uint32_t aminor = 0b101011010101;
              uint8_t noteToPlay = noteData[i].noteOnArray[n];

              uint8_t count = 0;
              while ( (0b100000000000 >> (noteToPlay  % 12) ) & ~aminor ) {
                noteToPlay += 1;
                count += 1;
                if (count > 12) {
                  break;
                  Serial.println("quantize error, breaking after 12 modifications");
                }
              }
              Serial.println("quantized note: "+ String(midiNotes[noteData[i].noteOnArray[n]]) + "to: " + String(midiNotes[noteToPlay]));
              MIDI.sendNoteOn(noteToPlay, noteData[i].noteVelArray[n], noteData[i].channel);
          } else {
              MIDI.sendNoteOn(noteData[i].noteOnArray[n], noteData[i].noteVelArray[n], noteData[i].channel);
          }

          */
          MIDI.sendNoteOn(noteData[i].noteOnArray[n], noteData[i].noteVelArray[n], noteData[i].channel);

      //  usbMIDI.sendNoteOn(noteData[i].noteOnArray[n], noteData[i].noteVelArray[n], noteData[i].channel);

      // These lines below help to troubleshoot latency and jitter issues, but they cause crashes!
      //  unsigned long delta = noteData[i].sequenceTime - noteData[i].offset;
      //  unsigned long roundTripTime = micros() - noteData[i].triggerTime;
      //    
      //  avgDelta = (delta + roundTripTime+ 9*avgDelta)/10;

      //  Serial.println(
      //    "noteOn: " + String(noteData[i].noteOnArray[n]) 
      //    + "\tbt: " + String(sequence[selectedSequence].beatTracker) 
      //    + "\tch: " + String(noteData[i].channel)
      //    + "\tseq: " + String(noteData[i].sequenceTime)
      //    + "\toff: " + String(noteData[i].offset)
      //    + "\tdelta: " + String(delta) 
      //    + "\trtt: " + String(roundTripTime)
      //    + "\ttot: " + String(delta+roundTripTime)
      //    + "\tavgDelta: " + String(avgDelta) 
      //    + "\tstartTime: " + String(startTime));
        }
      }
    }
  
  }
      //  Serial.println(" b8 ");

  wasPlaying = playing;
  lastTimer = loopTimer;
}
    // every beat,
    //  run the sequencer to determine what notes
    //  need to be played over the next beat

  // need to know which steps are going to happen 
  // over the next beat
  // we know BPM
  // we know the steps of the sequence
  // and how long each is.

  // beat:    0  1  2  3  4  5  6  7
  // step:    0  1  2     3     4  5
  // length:  1  1  2  -  2  -  1  1 

/// SCENERIO 1: steps are not based on relative beats
  // stepCount:  1 measure (4 beats) 
  //  - only 3 beats are populated with notes
  //  - sequencer will repeat the first beat to fill the space 
  // if sequence length was 1/2 a measure (2 beats)
  //  - the step scheduler would need to determine how many steps need to be scheduled
  //  - only the first 6 steps would play 
  // length is in 1/16th notes
  // beat:         0               1               2               3
  // step          0   1   4       2   3   5       6   7   5   
  // length:       1   1   2       1   1   2       1   1   2
  // stepActive:   X   X   X       -   X   -       -   X   -  
  // stepPitch:    34  34  53      52  43  35      35  32  35 
  // stepVelocity: 53  51  125     53  64  12      35  59  12 
/// SCENERIO 2: steps are based on beats <<<--- Gonna do this one, its easier to start with
  // stepCount:  1 measure (4 beats) 
  //  - only 3 beats are populated with notes
  //  - sequencer will repeat the first beat to fill the space 
  // if sequence length was 1/2 a measure (2 beats)
  //  - the step scheduler would need to determine how many steps need to be scheduled
  //  - only the first 6 steps would play 
  // length is in 1/16th notes
  // beat:         0               1               2               3
  // step          0   1   2   3   4   5   6   7   8   9   10   
  // length:       1   1   2   1   1   1   2   1   1   1   2
  // stepActive:   X   X   X   -   -   X   -   -   -   X   -  
  // stepPitch:    34  34  53  0   52  43  35  0   35  32  35 
  // stepVelocity: 53  51  125 0   53  64  12  0   35  59  12 

  // step scheduler routine needs to figure out what steps 
  // 
  // Timer1.setPeriod(50*knob.read());
  //color++ ;//knob.read();
 // color = color % 255;


/*
  activeStep++;
  activeStep = activeStep % numSteps;

  synth.allNotesOff(0);
  synth.noteOn(0, stepPitch[activeStep], 64);   
*/



