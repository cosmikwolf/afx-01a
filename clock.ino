elapsedMicros masterTempoTimer = 0;
boolean wasPlaying = false;
uint8_t masterPulseCount =24;
double bpm;
elapsedMicros lastClock = 0;
float avgBpm;
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
      default:
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
       // bpm = 60/(((int(masterTempoTimer)/(masterPulseCount+1))*24)/1000000);
        bpm = 60.0/(24.0*(float(masterTempoTimer)/float(masterPulseCount))/1000000.0);
        //Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2 + " mtt:" + String(masterTempoTimer) + "\t mpc: " + String(masterPulseCount) + 

        avgBpm = (9*avgBpm + bpm)/10;
        Serial.println(" bpm:" + String(bpm)  + "\tavg: " + String(avgBpm) );
        //20bpm = 124825/1
        //120bpm = 20831/1

        // 1 beat = 24 puleses
        // = 24* avg pulse = 24*(masterTempoTimer/(masterPulseCount+1))
        // = 24*(20831/1) = 499944 microseconds per beat /1000000 = .4999 seconds per beat 

        masterPulseCount++;
        if (masterPulseCount > 24){
          masterPulseCount = 1;
          masterTempoTimer = 0;
        }
        
        lastClock = 0;
    }
  }
  interrupts();
}

void masterClockFunc(){
	// this timer runs once every millisecond 
  noInterrupts();

  iter = iter + 1;

 //  Serial.println("timer loop 2");
  if(playing){
    if (wasPlaying == false){
      // if playing has just re-started, the master tempo timer and the master beat count must be reset
      masterTempoTimer = 0;
      masterPulseCount = 0;
    }
    wasPlaying = true;
    // get all note events
 
    uint8_t *s0 = sequence[0].runSequence();
    uint8_t *s1 = sequence[1].runSequence();
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

   if (int(s0[4]) == 1){
      for (int n = 5; n<132; n++){
        if (s0[n] != 0){
          Serial.println("noteOff: " + String(s0[n]) + " ct: " + String(sequence[selectedSequence].clockTracker)) ;
          noteOff(0);
          MIDI.sendNoteOff(s0[n], 64, s0[1]);
        }
      }
    };
 
  if (s0[0] == 1) {
    testTimer1 = 0;
    noteOn(0,int(s0[2]));
    MIDI.sendNoteOn(int(s0[2]), 100, s0[1]);
    Serial.println("noteOn: " + String(s0[2]) + " ct: " + String(sequence[selectedSequence].clockTracker) + " millis: " + String(millis() ) + "   - " + String(iter) ) ;
    Serial.println("track 0 time to play: " + String(testTimer1));
  } 

  if (int(s1[4]) == 1){
      for (int n = 5; n<132; n++){
        if (s1[n] != 0){
          activeSection = activeSection + " " + String(s1[n]);
              Serial.println("noteOff: " + String(s1[n]) + " ct: " + String(sequence[selectedSequence].clockTracker)) ;
          noteOff(1);
          MIDI.sendNoteOff(s1[n], 64, s1[1]);
        }
      }
    };
 

  if (s1[0] == 1) {
    testTimer2 = 0;

    noteOn(1,int(s1[2]));
    MIDI.sendNoteOn(int(s1[2]), 100, s1[1]);
    activeSection = "on " + String(s1[2]);
    Serial.println("noteOn: " + String(s1[2]) + " ct: " + String(sequence[selectedSequence].clockTracker) + " millis: " + String(millis())+ "   - " + String(iter) ) ;

       Serial.println("track 1 time to play: " + String(testTimer2));

  } 



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
  // sequenceLength:  1 measure (4 beats) 
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
  // sequenceLength:  1 measure (4 beats) 
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


