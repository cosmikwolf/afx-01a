void startupSequence(){

  display.clearDisplay();   // clears the screen and buffer

  display.display(); // show splashscreen
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("SPACE\rCADET");
  display.display(); // show splashscreen
  display.startscrollleft(0, 15);
  theaterChaseRainbow(1);
  display.invertDisplay(1);
  theaterChaseRainbow(1);
  display.invertDisplay(0);
  display.clearDisplay();   // clears the screen and buffer
  display.stopscroll();

  pixels.setBrightness(64);
}
void displayLoop() {
  //Serial.println(F("displayStart"));
  display.clearDisplay();   // clears the screen and buffer


    display.drawTriangle( 120,0,
                          113,9,
                          127,9, WHITE);
  if (tempoBool == true) {
    display.fillTriangle( 120,0,
                          113,9,
                          127,9, WHITE);
  }
  Serial.println(F("displaydebug1"));

  display.setTextSize(1);

  display.setTextColor(WHITE);
 // Serial.println(F("displaydebug2"));
  
  display.setCursor(0,0);
 // display.println(F(sequence[selectedSequence].instrument));

  display.println(F(instrumentNames[sequence[selectedSequence].instrument+1]));
  

  display.println(F("tempo:") + String(sequence[selectedSequence]._tempo));
  display.println(F("as: ") + String(sequence[selectedSequence].activeStep) + F(" ct: ") + String(sequence[selectedSequence].clockTracker));
  display.println(F("sequenceLength: ") + String(sequence[selectedSequence]._sequenceLength));
display.println(String(FreeRam()));

Serial.println(F("displaydebug3"));
  display.println(F("lastActiveStep: ") + String(lastActiveStep));
 
  if (stepLength[selectedStep] == 1) {
    display.println(F("Step: ") + String(selectedStep) + " -  " + sequence[selectedSequence]._gateLength[selectedStep] + F(" beat"));
  } else {
    display.println(F("Step: ") + String(selectedStep) + " " + sequence[selectedSequence]._gateLength[selectedStep] + F(" beats"));
  }
Serial.println(F("displaydebug4"));

  if (stepActive[selectedStep] == true){
    display.println(F("Pitch: ") + String(midiNotes[sequence[selectedSequence]._stepPitch[selectedStep]]) + F(" Vel: ") + String(sequence[selectedSequence]._stepVelocity[selectedStep]));
  } else {
    display.println(F("Rest Step"));
  }

Serial.println(F("displaydebug5"));
  display.println(F("avgPeriod:") + String(avgPeriod));
Serial.println(F("display debug 6"));

  display.println(F("avgLoopTime:") + String(avgLoopTime));
  display.println(F("avgRuntime:") + String(avgRuntime));
  display.setTextSize(2);
  display.setCursor(100,0);
Serial.println(F("display debug 7"));

  display.print(String(selectedSequence));
Serial.println(F("display debug 8"));
  display.display();
Serial.println(F("display debug 9"));

Serial.println(F("displayLoopEnd"));

}
uint32_t FreeRam(){ // for Teensy 3.0
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);

    // The difference is the free, available ram.
    return stackTop - heapTop;
}
void ledLoop(){
  selectedStepColor = int(millis()*2)%256;
  pixels.setPixelColor(7-selectedStep, Wheel(selectedStepColor) );      
  pixels.show();
}

