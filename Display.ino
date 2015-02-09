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
  Serial.println("displayStart");
  display.clearDisplay();   // clears the screen and buffer

    display.drawTriangle( 120,0,
                          113,9,
                          127,9, WHITE);
  if (tempoBool == true) {
    display.fillTriangle( 120,0,
                          113,9,
                          127,9, WHITE);
  }
Serial.println("displaydebug1");

  display.setTextSize(1);

  display.setTextColor(WHITE);
Serial.println("displaydebug2");

  display.setCursor(0,0);
  display.println(String(instrumentNames[sequence[selectedSequence].instrument+1]));
  display.println("tempo:" + String(sequence[selectedSequence]._tempo));
  display.println("as: " + String(sequence[selectedSequence].activeStep) + " ct: " + String(sequence[selectedSequence].clockTracker));
  display.println("sequenceLength: " + String(sequence[selectedSequence]._sequenceLength));
Serial.println("displaydebug3");

  display.println("lastActiveStep: " + String(lastActiveStep));
 /* 
  if (stepLength[selectedStep] == 1) {
    display.println("Step: " + String(selectedStep) + " -  " + sequence[selectedSequence]._gateLength[selectedStep] + " beat");
  } else {
    display.println("Step: " + String(selectedStep) + " " + sequence[selectedSequence]._gateLength[selectedStep] + " beats");
  }
Serial.println("displaydebug4");

  if (stepActive[selectedStep] == true){
    display.println("Pitch: " + String(midiNotes[sequence[selectedSequence]._stepPitch[selectedStep]]) + " Vel: " + String(sequence[selectedSequence]._stepVelocity[selectedStep]));
  } else {
    display.println("Rest Step");
  }
*/
Serial.println("displaydebug5");

  display.println("avgPeriod:" + String(avgPeriod));
Serial.println("display debug 6");

  display.println("avgLoopTime:" + String(avgLoopTime));
  display.println("avgRuntime:" + String(avgRuntime));
  display.setTextSize(2);
  display.setCursor(100,0);
Serial.println("display debug 7");

  display.print(String(selectedSequence));
Serial.println("display debug 8");
  
  noInterrupts();
  display.display();
Serial.println("display debug 9");

  interrupts();
Serial.println("displayLoopEnd");
}

void ledLoop(){
  selectedStepColor = int(millis()*2)%256;
  pixels.setPixelColor(7-selectedStep, Wheel(selectedStepColor) );      
  pixels.show();
}

