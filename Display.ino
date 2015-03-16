
int color = 0;


#if (SSD1306_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


void startupSequence(){
  pixels.setBrightness(45);

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

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);



    display.setTextSize(2);
    display.print(String(midiNotes[sequence[selectedSequence].stepPitch[selectedStep]]));
    display.setTextSize(1);
    display.setCursor(30,0);
    display.println(" Vel:  " + String(sequence[selectedSequence].stepVelocity[selectedStep]));
    display.setCursor(30,8);
    if (modeSelekt == 2){
      display.setTextColor(BLACK, WHITE);
    }
    if (sequence[selectedSequence].gateType[selectedStep] != 0){
      display.println(" Gate: " + String(sequence[selectedSequence].gateType[selectedStep]));
    } else {
      display.println(" Step Off");
    }
    display.setTextColor(WHITE);
   // display.println(String(sequence[selectedSequence].gateLength[selectedStep]) + " Steps"
  

  display.setCursor(0,16);

  if (modeSelekt == 3){
    display.setTextColor(BLACK, WHITE);
  }
    display.println("Speed: " + String(sequence[selectedSequence].stepDivider) + "x");
  display.setTextColor(WHITE);


 //display.println(F("tempo:") + String(sequence[selectedSequence].tempo));
  display.println(F("as: ") + String(sequence[selectedSequence].activeStep) + F(" ct: ") + String(sequence[selectedSequence].clockTracker));
  //display.println(F("sequenceLength: ") + String(sequence[selectedSequence].sequenceLength));
 // display.println("millis: " + String(millis()));
  //display.println("activeSection: " + activeSection);

  if (modeSelekt == 1){
    display.setTextColor(BLACK, WHITE);
  }
  if (stepLength[selectedStep] == 1) {
    display.println(F("Step: ") + String(selectedStep) + " -  " + sequence[selectedSequence].gateLength[selectedStep] + F(" beat"));
  } else {
    display.println(F("Step: ") + String(selectedStep) + " - " + sequence[selectedSequence].gateLength[selectedStep] + F(" beats"));
  }
  display.setTextColor(WHITE);

display.println(activeSection);
 // display.println("Peak: " + String(peak1.read()) + " 2: " + String(peak2.read()));
 // display.println("CPU:" + String(AudioProcessorUsage()) + " MAX:" + String(AudioProcessorUsageMax()));
 // display.println("MEM:" + String(AudioMemoryUsage()) + " MAX:" + String(AudioMemoryUsageMax()));
//  display.println(F("avgPeriod:") + String(avgPeriod));
 // display.println(F("avgLoopTime:") + String(avgLoopTime));
//  display.println(F("avgRuntime:") + String(avgRuntime));
  display.setTextSize(2);
  display.setCursor(100,0);
  display.print(String(selectedSequence));
  display.display();
}

void ledLoop(){
// selectedStepColor = int(millis()/5)%255;
// pixels.setPixelColor(selectedStep, Wheel(selectedStepColor) );      
// pixels.show();
}



void pixelRender(){
   for (int i=0; i < NUMPIXELS; i++){
    if (i == (sequence[selectedSequence].activeStep) ) {
      pixels.setPixelColor(i, pixels.Color(255,255,255) );      
    } else if ( i == selectedStep) {
      selectedStepColor = int(millis()/5)%255;
       pixels.setPixelColor(selectedStep, Wheel(selectedStepColor) );      

    } else {
      if(sequence[selectedSequence].gateType[i] == 0){
        pixels.setPixelColor(i, pixels.Color(0,0,0));
      } else {
        pixels.setPixelColor(i, Wheel( sequence[selectedSequence].getStepPitch(i) ) );
      }
    }
  }

  pixels.show();
}


// NeoPixel Subs
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < pixels.numPixels(); i=i+3) {
          pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        pixels.show();
       
        delay(wait);
       
        for (int i=0; i < pixels.numPixels(); i=i+3) {
          pixels.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

uint32_t freemem(){ // for Teensy 3.0
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

