void buttonSetup() {
//  button0.debounceTime   = 0;
//  button1.debounceTime   = 0;
//  button2.debounceTime   = 0;
//  button3.debounceTime   = 0;
//  button4.debounceTime   = 0;
//  button5.debounceTime   = 0;
//  button0.multiclickTime = 10;  // Time limit for multi clicks
//  button1.multiclickTime = 10;  // Time limit for multi clicks
//  button2.multiclickTime = 10;  // Time limit for multi clicks
//  button3.multiclickTime = 10;  // Time limit for multi clicks
//  button4.multiclickTime = 10;  // Time limit for multi clicks
//  button5.multiclickTime = 10;  // Time limit for multi clicks
//  button0.longClickTime  = 500; // time until "held-down clicks" register
}

void buttonLoop(){

  for (int i=0; i <6; i++){
    buttons[i].update();
  }

  if ( buttons[0].read() == LOW){
 //   knob2Buffer = (knob2.read()-knob2InitValue) / -4;
    selectedSequence = abs(knob2.read()/4) % 3;
  }

  if ( buttons[2].read() == LOW){
    selectedStep -= 1;
    selectedStep = selectedStep 8;


  if (buttons[1].fallingEdge()){
  }

  if (buttons[1].risingEdge()){
  }

}
/*
void buttonLoop(){
  Serial.print("buttondebugstart");

  button1.Update();
  button2.Update();
  button3.Update();
  button4.Update();
  button5.Update();
  button0.Update();


  knob2Buffer = (knob2.read()-knob2InitValue) / -4;
  knob1Buffer = (knob1.read()) / -4;  
  //stepPitch[selectedStep] = abs(currentStepInitPitch + knob2Buffer);

  if (button5.clicks != 0) {
    // use mode one when the first button is pressed
    knob2InitValue = knob2.read();
    knob1InitValue = knob1.read();
    selectedStep += 1;
    selectedStep = selectedStep % 8;
    currentStepInitPitch = stepPitch[selectedStep];
  }
  

  // Start modifier button hold sections
      noInterrupts();

  if(button0.depressed) {

    if (buttonPreviousState[0] == 1) {
      //

      knob1.write(0);
      //knob2.write(0);
      previousTempo = tempo;
    }

    buttonPreviousState[0] = 0;
    tempo = knob1Buffer + previousTempo;
    selectedSequence = knob2Buffer % 3;

  } else if(button1.depressed) {
    if (buttonPreviousState[1] == 1) {
      knob1.write(0);
      previousStepsPerBeat = stepsPerBeat;
    }
    buttonPreviousState[1] = 0;
    stepsPerBeat = knob1Buffer + previousStepsPerBeat;
  } else if(button2.depressed) {
   
    if (buttonPreviousState[1] == 1) {
      knob1.write(0);
      stepLengthBuffer = sequence[selectedSequence]._gateLength[selectedStep];
    }
    buttonPreviousState[1] = 0;
    sequence[selectedSequence].setGateLength(selectedStep, knob1Buffer + stepLengthBuffer);

  } else if (button3.depressed) {
  
    if (buttonPreviousState[1] == 1) {
      knob1.write(0);
      sequenceLengthBuffer = sequenceLength;
    }
    buttonPreviousState[1] = 0;
    sequenceLength = knob1Buffer + sequenceLengthBuffer;

  } else if(button4.depressed) {
    if (buttonPreviousState[4] == 1) {
      knob1.write(0);
      instrumentPrevious = sequence[selectedSequence].instrument;
    }
    buttonPreviousState[4] = 0;
    sequence[selectedSequence].changeInstrument((knob1Buffer + instrumentPrevious) %127);

  } else {
    buttonPreviousState[0] = 1;
    buttonPreviousState[1] = 1;
    sequence[selectedSequence].setStepPitch(selectedStep,  abs(currentStepInitPitch + knob2Buffer) );
  }
    interrupts();

  if (button1.clicks == 1){
    selectedSequence = (selectedSequence +1) %3;
    knob2.write(0);
  }; 

  // end modifier button hold sections

  if (button0.clicks != 0) {
    clicks[0] += 1;
  }
  if (button1.clicks != 0) {
    clicks[1] += 1;
  }
  if (button2.clicks != 0) {
    clicks[2] += 1;
  }
  if (button3.clicks != 0) {
    clicks[3] += 1;
  }
  if (button4.clicks != 0) {
    clicks[4] += 1;
  }
  if (button5.clicks != 0) {
    clicks[5] += 1;
  }
  Serial.print("buttondebugend");

}
*/