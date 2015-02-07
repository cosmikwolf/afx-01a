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
  display.clearDisplay();   // clears the screen and buffer

    display.drawTriangle( 120,0,
                          113,9,
                          127,9, WHITE);
  if (tempoBool == true) {
    display.fillTriangle( 120,0,
                          113,9,
                          127,9, WHITE);
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
 // display.println("Knob: "  + String(knob.read() % 255));
  display.println("tempo:" + String(tempo) );
  display.println("steps/beat: " + String(stepsPerBeat) );
  display.println("instrument: "  + String(instrument));
// display.println("tempoBool: " + String(tempoBool));
// display.println("loopTimer: " + String(loopTimer));
  // display.println("Knob1: "  + String(knob1Buffer));
  // display.println("Knob2: "  + String(knob2Buffer));
  display.println("Step: " + String(selectedStep) + " Length: " + stepLength[selectedStep]);
  display.println("Note: " + String(midiNotes[stepPitch[selectedStep]]) + " Velocity: " + String(stepVelocity[selectedStep]));

  display.print("Buttons: ");
  if (button0.depressed){
    display.print("0 ");
  }
  if (button1.depressed){
    display.print("1 ");
  }
  if (button2.depressed){
    display.print("2 ");
  }
  if (button3.depressed){
    display.print("3 ");
  }
  if (button4.depressed){
    display.print("4 ");
  }
  if (button5.depressed){
    display.print("5 ");
  }
  
  display.println("");
  /*

  display.print("Steps: ");
    
  for (int i=0; i < 8; i++){
    display.print(String(stepPitch[i]) + " " );
  }

  display.println("Current Step: "  + String( sequenceStep + 1 ) );
  display.println("selectedStep:" + String(selectedStep));
  display.println("knob.read():" + String(knob1.read()));


  
  for (int i; i < 6; i++){
      display.println("Button" + String(i) + "; "  + String( clicks[i] ) );
  }
    */

  display.println("\navgPeriod:" + String(avgPeriod));

  display.display();
}

void ledLoop(){
  selectedStepColor = int(millis()*2)%256;
  pixels.setPixelColor(7-selectedStep, Wheel(selectedStepColor) );      
  pixels.show();
}

