void timerInterrupt(){
	// this timer runs once every millisecond 
  Serial.println("interrupt start");
  // this is used to measure the average run time between loops
  runTimer = 0;
  
  avgPeriod = ( avgPeriod + (int(micros()-lastRunTime)))/2;
  lastRunTime = micros();

  for (int i=0; i<3; i++){
    if ( sequence[i]._sequenceTimer > (60*1000000/sequence[i]._tempo)/sequence[i]._stepDivider ){
      sequence[i].incrementClockTracker();
      sequence[i].resetSequenceTimer();
      //pixelRender();
    }
  }

  // the single sequence way of doin it
  //stepper();
  // the multi sequence way..
  sequence[0].runSequence();
  sequence[1].runSequence();
  sequence[2].runSequence();

  if (tempoTimer > (60*1000000/tempo)) {
 	  tempoTimer = 0;
 	  tempoBool = true;
  } else if ( tempoTimer > (60*1000000/tempo)/2 ) {
    if (tempoBool == true) {
      tempoBool = false;
    }
  }

  avgRuntime = (9*avgRuntime+int(runTimer))/10;  
  Serial.println("interrupt end");

}

void pixelRender(){
   for (int i=0; i < NUMPIXELS; i++){
    if (i == (sequence[selectedSequence].activeStep) ) {
      pixels.setPixelColor(numSteps-i-1, pixels.Color(255,255,255) );      
    } else if ( i == selectedStep) {

    } else {
      pixels.setPixelColor(numSteps-i-1, Wheel( sequence[selectedSequence].getStepPitch(i) ) );
    }
  }

  pixels.show();
}



void stepper() {
  Serial.println("stepper start");
  // every timerInterrupt, run the stepper subroutine
  // to check if the stepTimer has elapsed, and if it
  // has, it should trigger the notes, and then it 
  // should reset the stepTimer.
  // stepper also should check to see if there between 
  // 1-2 ms until stepTimer elapses, so it should turn off
  // the gate signal, so that at the next iteration,
  // the gate signal can be turned back on.

  /*
  if (stepTimer > stepLength[activeStep]*(60*1000000/tempo)/4) {
    // change the note
    // increment activeStep
    // turn the gate on if step is active
    // reset stepTimer


    // TO DO:
    //    Design UI to set all step information for each step.
    //    this needs to be put where stepActive and setLength are set;
    programmedLength = 0;
    lastActiveStep = 0;

    for (int n = 0; n < 128; n++){
      if (stepActive[n]){
        programmedLength += stepLength[n];
        lastActiveStep = n;
      }
    }

    if ( lengthTracker >= sequenceLength ){
      // if the sequence has ended, go back to the beginning
      activeStep = 0;
      lengthTracker = 0;
    } else if ( lengthTracker == lastActiveStep+1  ) {
      // if the sequence is repeating, go back to the beginning
      activeStep = 0;
    } else {
      activeStep += 1;
    }

    if (stepActive[activeStep]){
      synth.noteOn(0, stepPitch[activeStep], 64);   
    }
    stepTimer = 0;
  }

  if (stepTimer > (stepLength[activeStep]-2000)) {
    // turn gate off if step is active
    if (stepActive[activeStep]){
      synth.noteOff(0, stepPitch[activeStep]);   
    }
  }
*/
 

  Serial.println("stepper end");

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


