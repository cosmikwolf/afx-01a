elapsedMicros masterTempoTimer = 0;
boolean wasPlaying = false;
uint8_t masterPulseCount =24;
double bpm;
float avgBpm;
boolean tempoBlip = false;
elapsedMicros blipTimer = 0;
uint32_t beatLength = 500000;


void midiClockSyncFunc(){
  noInterrupts();
  int type, note, velocity, channel, d1, d2;
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    byte type = MIDI.getType();
    switch (type) {
      case NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        if (velocity > 0) {
          Serial.println(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity);
        } else {
          Serial.println(String("Note Off: ch=") + channel + ", note=" + note);
        }
        break;
      case NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        Serial.println(String("Note Off: ch=") + channel + ", note=" + note + ", velocity=" + velocity);
        break;
    }

    if (extClock == true) {
      switch (type) {
        case 250: // midi start
            for (int i; i < sequenceCount; i++) {
              sequence[i].beatTracker = 0;
              sequence[i].activeStep = 0;
            }
            Serial.println("MIDI Start: resetting position to 0");

        case 251: // midi continue
          playing = 1;
          masterPulseCount = 1;
          masterTempoTimer = 0;
          tempoBlip = !tempoBlip;
          blipTimer = 0;
          // need to send pulse on the first note, with a beat length of some kind 
          for (int i=0; i< sequenceCount; i++){
            sequence[i].clockStart();  
          }
          Serial.println("Midi Start / Continue: " + String(type));
          break;

        case 252:
          playing = 0;
          break;
        case 242: // midi song position pointer
          d1 = MIDI.getData1();
          d2 = MIDI.getData2();
          Serial.println("MIDI song position pointer: " + String(d1) + " " + String(d2));
          if (d1 == 0 && d2 == 0 && playing == 0){ 
            for (int i; i < sequenceCount; i++) {
              sequence[i].beatTracker = 0;
              sequence[i].activeStep = 0;
            }
            Serial.println("resetting position to 0");
          }
          break;
        case 248: // midi clock
          bpm = 60.0/(24.0*(float(masterTempoTimer)/float(masterPulseCount))/1000000.0);
          avgBpm = (9*avgBpm + bpm)/10;
          //Serial.println(" bpm:" + String(bpm)  + "\tavg: " + String(avgBpm) );
          //20bpm = 124825/1
          //120bpm = 20831/1

          // 1 beat = 24 puleses
          // = 24* avg pulse = 24*(masterTempoTimer/(masterPulseCount+1))
          // = 24*(20831/1) = 499944 microseconds per beat /1000000 = .4999 seconds per beat 
         // masterClockFunc();
          masterPulseCount = positive_modulo(masterPulseCount + 1, 24);
        //  Serial.println("Midi Clock - mpc: " + String(masterPulseCount) + "\ttempotimer: " + String(masterTempoTimer) );

          if (masterPulseCount ==  0){
            //this gets triggered every quarter note
            beatLength = masterTempoTimer;
            masterTempoTimer = 0;
            tempoBlip = !tempoBlip;
            blipTimer = 0;
            for (int i=0; i< sequenceCount; i++){
              sequence[i].pulseTempo(beatLength);  
            }
          }
          break;
        default:
          d1 = MIDI.getData1();
          d2 = MIDI.getData2();
         // bpm = 60/(((int(masterTempoTimer)/(masterPulseCount+1))*24)/1000000);
          Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2 + " mtt:" + String(masterTempoTimer) + "\t mpc: " + String(masterPulseCount) );
       }
    }
    
  }
  interrupts();
}

void masterClockFunc(){
	// this timer runs once every millisecond 
     // Serial.println("master Clock Start");

  noInterrupts();

  iter = iter + 1;

 //  Serial.println("timer loop 2");
  if(playing){
    if (wasPlaying == false){
      // if playing has just re-started, the master tempo timer and the master beat count must be reset
      masterTempoTimer = 0;
      masterPulseCount = 0;

      internalClockTimer = 0;
      for (int i=0; i< sequenceCount; i++){
        sequence[i].clockStart();
        sequence[i].pulseTempo(beatLength);  
      }
    }

    if( extClock == false ){ 
      if (internalClockTimer > 60000000/tempo){
        for (int i=0; i< sequenceCount; i++){
          sequence[i].pulseTempo(beatLength);  
        }
        tempoBlip = !tempoBlip;
        internalClockTimer = 0;
      }
    }

    wasPlaying = true;
    // get all note events
    //Serial.println("about to run sequences s0");
  for (int i=0; i< sequenceCount; i++){
     sequence[i].runSequence(&noteData[i]);
  }
    //Serial.println("about to run sequences s1");
  // uint8_t *s2 = sequence[2].runSequence();
  // uint8_t *s3 = sequence[3].runSequence();
  // uint8_t *s4 = sequence[4].runSequence();
  // uint8_t *s5 = sequence[5].runSequence();
  // uint8_t *s6 = sequence[6].runSequence();
  // uint8_t *s7 = sequence[7].runSequence();
  // uint8_t *s8 = sequence[8].runSequence();
  // uint8_t *s9 = sequence[9].runSequence();
  // uint8_t *s10 = sequence[10].runSequence();
  // uint8_t *s11 = sequence[11].runSequence();
  // uint8_t *s12 = sequence[12].runSequence();
  // uint8_t *s13 = sequence[13].runSequence();
  // uint8_t *s14 = sequence[14].runSequence();
  // uint8_t *s15 = sequence[15].runSequence();

  for (int i=0; i< sequenceCount; i++){
    if (noteData[i].noteOn == true){
      for (int n=0; n< 16; n++){
        if (noteData[i].noteOnArray[n] == NULL){
          continue;
        }
        noteOn(noteData[i].channel,noteData[i].noteOnArray[n]);
        MIDI.sendNoteOn(noteData[i].noteOnArray[n], noteData[i].noteVelArray[n], noteData[i].channel);
        Serial.println("noteOn: " + String(noteData[i].noteOnArray[n]) + "\tbt: " + String(sequence[selectedSequence].beatTracker) + "\tjitter: " + String(sequence[selectedSequence].sequenceTimer - sequence[selectedSequence].stepData[noteData[i].noteOnStep].offset) ) ;
      }
    }
  }

  for (int i=0; i< sequenceCount; i++){
    if (noteData[i].noteOff == true){
      for (int n=0; n< 16; n++){
        if (noteData[i].noteOffArray[n] == NULL){
          continue;
        }
        noteOn(noteData[i].channel,noteData[i].noteOffArray[n]);
        MIDI.sendNoteOff(noteData[i].noteOffArray[n], 64, noteData[i].channel);
        Serial.println("noteOff: " + String(noteData[i].noteOffArray[n]) + "\tbt: " + String(sequence[selectedSequence].beatTracker) + "\tjitter: " + String(sequence[selectedSequence].sequenceTimer - sequence[selectedSequence].stepData[noteData[i].noteOffStep].offset - sequence[selectedSequence].stepData[noteData[i].noteOffStep].noteTimerMcs) ) ;
      }
    }
  }
/*  if (int(s0[4]) == 1){
    for (int n = 5; n<132; n++){
      if (s0[n] != 0){
        Serial.println("noteOff: " + String(s0[n]) + " bt: " + String(sequence[selectedSequence].beatTracker)) ;
        noteOff(0);
        MIDI.sendNoteOff(s0[n], 64, s0[1]);
      }
    }
  };
 
  if (s0[0] == 1) {
    noteOn(0,int(s0[2]));
    MIDI.sendNoteOn(int(s0[2]), 100, s0[1]);
    Serial.println("noteOn: " + String(s0[2]) + "\tbt: " + String(sequence[selectedSequence].beatTracker) + "\tjitter: " + String(sequence[selectedSequence].sequenceTimer - sequence[selectedSequence].stepData[s0[127]].offset) ) ;
  } 

  if (int(s1[4]) == 1){
    for (int n = 5; n<132; n++){
      if (s1[n] != 0){
        activeSection = activeSection + " " + String(s1[n]);
            Serial.println("noteOff: " + String(s1[n]) + " bt: " + String(sequence[selectedSequence].beatTracker)) ;
        noteOff(1);
        MIDI.sendNoteOff(s1[n], 64, s1[1]);
      }
    }
  };
 

  if (s1[0] == 1) {
    noteOn(1,int(s1[2]));
    MIDI.sendNoteOn(int(s1[2]), 100, s1[1]);
    activeSection = "on " + String(s1[2]);
    Serial.println("noteOn: " + String(s1[2]) + " bt: " + String(sequence[selectedSequence].beatTracker) + " millis: " + String(millis())+ "   - " + String(iter) ) ;
  } 

*/
//    // Serial.println("timer loop 3");
//    if (s1[0] == 1) {
//      //Serial.println("seq 1 trigger");
//
//      if (s1[2] == 1) {
//        Serial.println("playing kick");
//  //      sound3.play(AudioSampleKick);
//      } else if (s1[2] == 2) {
//  //      sound0.play(AudioSampleSnare);
//      } else if (s1[2] == 3) {
//  //      sound2.play(AudioSampleHihat);
//      } else if (s1[2] == 4) {
//  //      sound1.play(AudioSampleTomtom);
//      } else if (s1[2] == 5) {
//      //  sound4.play(AudioSampleGong);
//      } else if (s1[2] == 6) {
//      //  sound5.play(AudioSampleCashregister);
//      }
//
//    }
// 


  }

  interrupts();

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



