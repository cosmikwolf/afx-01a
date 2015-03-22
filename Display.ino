
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
  display.setCursor(6,0);
  display.println("ZETTA");
  display.setCursor(6,32);
  display.println("±OHM±");
  display.display();
  display.startscrollleft(0, 15);
    Serial.println("display 1");

  nonBlockingRainbow(2);
    delay(100);
  nonBlockingRainbow(2);
    delay(100);
  nonBlockingRainbow(2);
    delay(100); 
  nonBlockingRainbow(2);
    delay(100);
  nonBlockingRainbow(2);
    delay(100);
  nonBlockingRainbow(2);
    delay(100);

      Serial.println("display 2");

  display.invertDisplay(1);
      Serial.println("display 3");
   nonBlockingRainbow(1);
    delay(100);

      Serial.println("display 4");

  display.invertDisplay(0);
  display.clearDisplay();   // clears the screen and buffer
  display.stopscroll();
  display.display();
}

void displayLoop() {
  //Serial.println(F("displayStart"));
  display.clearDisplay();   // clears the screen and buffer


  if (settingMode == 0){
    stepDisplay();
  } else if (settingMode == 1){
    sequenceMenuDisplay();
  } else if (settingMode == 2){
    globalMenuDisplay();
  } else {
    menuItem(settingMode);
  }

  display.display();
}

void menuItem(uint8_t menuItem){
  uint8_t globalSkip[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  switch (menuItem){
    case GLOBAL_MIDI:
      nonBlockingRainbow(5, globalSkip, 15 );

      display.setTextColor(WHITE);
      display.setCursor(9,1);
      display.setTextSize(2);
      display.println("MIDI MENU" );
      display.setCursor(0,24);

      display.setTextSize(1);
      display.print(" Midi Clock: " );
      if (extClock == true){
        display.println("External");
      } else {
        display.println("Internal");
      };
      break;

    case SEQUENCE_GENE:
      display.setTextColor(WHITE);
      display.setCursor(0,1);
      display.setTextSize(1);
      display.println("Sequence Generator");
      break;
    default:
      display.setTextColor(WHITE);
      display.setCursor(0,1);
      display.setTextSize(1);
      display.println("settingMode " + String(menuItem) + " not yet defined");
      break;
  };
}

void sequenceMenuDisplay(){

  uint8_t skip[] = {11,15};
  nonBlockingRainbow(5, skip, 2 );

  display.setTextColor(WHITE);
  display.setCursor(16,1);
  display.setTextSize(2);
  display.println("SEQUENCE");
  display.setTextSize(1);
  display.setCursor(7,21);
  display.print("NAME ");
  display.print("SPED ");
  display.print("TRAN ");
  display.print("LENG ");
  display.setCursor(7,33);
  display.print("QUAN ");
  display.print("EUCL ");
  display.print("GENE ");
  display.print("ORDE ");
  display.setCursor(7,45);
  display.print("RAND ");
  display.print("POSI ");
  display.print("GLID ");
  display.print(" ");
  display.setCursor(7,57);
  display.print("MIDI ");
  display.print("{CV} ");
  display.print("GATE ");
  display.print(" ");
}

void globalMenuDisplay(){
  // Second Press: Glbal Menu
//      MIDI  SAVE  LOAD   

  uint8_t skip[] = {4,5,6,7,8,9,10,11,12,13,14,15};
  nonBlockingRainbow(10, skip, 12 );

  display.setTextColor(WHITE);
  display.setCursor(16,1);
  display.setTextSize(2);
  display.println("~GLOBAL~");
  display.setTextSize(1);
  display.setCursor(7,21);
  display.print("MIDI ");
  display.print("SAVE ");
  display.print("LOAD ");
  display.print("FILE ");
  display.setCursor(7,33);
}

void stepDisplay(){


//  display.drawTriangle( 120,0,
//                          113,9,
//                          127,9, WHITE);
//  if (tempoBlip == true) {
//    display.fillTriangle( 120,0,
//                          113,9,
//                          127,9, WHITE);
//  }
//  if (blipTimer > 20000) {
//    tempoBlip = false;
//  }

  display.setTextSize(1);
  display.setCursor(105,0);
    display.setTextColor(WHITE);
  if (tempoBlip) { display.setTextColor(BLACK, WHITE); };
  if (extClock == true) {
    display.println("EXT");
  } else {
    display.println(String(tempo));
  }
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(106,10);
  display.print(String(selectedSequence));


  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);


    if (stepMode == 0){
      display.setTextColor(BLACK, WHITE);
    }
    display.setTextSize(3);
    display.print(String(midiNotes[sequence[selectedSequence].stepData[selectedStep].pitch]));
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(50,0);
    if (stepMode == 2){
      display.setTextColor(BLACK, WHITE);
    }
    if (sequence[selectedSequence].stepData[selectedStep].gateType != 0){
      display.println(" gate: " + String(sequence[selectedSequence].stepData[selectedStep].gateType));
    } else {
      display.println(" ~rest~");
    }
    display.setTextColor(WHITE);
    display.setCursor(50,8);

    display.println(" velo: " + String(sequence[selectedSequence].stepData[selectedStep].velocity));

    if (stepMode == 1){
      display.setTextColor(BLACK, WHITE);
    }
        display.setCursor(50,16);

    display.println(" beat: " + String(sequence[selectedSequence].stepData[selectedStep].gateLength));
   // display.println(String(sequence[selectedSequence].gateLength[selectedStep]) + " Steps"
      display.setTextColor(WHITE);


  display.setCursor(0,24);

  if (stepMode == 3){
    display.setTextColor(BLACK, WHITE);
  }
    display.println("Time: " +String(sequence[selectedSequence].stepCount) + "/" + String(sequence[selectedSequence].beatCount));
  display.setTextColor(WHITE);


 //display.println(F("tempo:") + String(sequence[selectedSequence].tempo));
  display.println( "as: " + String(sequence[selectedSequence].activeStep) + " bt: " + String(sequence[selectedSequence].beatTracker));
  //display.println(F("stepCount: ") + String(sequence[selectedSequence].stepCount));
 // display.println("millis: " + String(millis()));
  //display.println("activeSection: " + activeSection);

display.println("Playing: " + String(playing));
 // display.println("Peak: " + String(peak1.read()) + " 2: " + String(peak2.read()));
 // display.println("CPU:" + String(AudioProcessorUsage()) + " MAX:" + String(AudioProcessorUsageMax()));
 // display.println("MEM:" + String(AudioMemoryUsage()) + " MAX:" + String(AudioMemoryUsageMax()));
//  display.println(F("avgPeriod:") + String(avgPeriod));
 // display.println(F("avgLoopTime:") + String(avgLoopTime));
//  display.println(F("avgRuntime:") + String(avgRuntime));
display.println("millis: " + String(millis()));
}

void ledLoop(){
// selectedStepColor = int(millis()/5)%255;
// pixels.setPixelColor(selectedStep, Wheel(selectedStepColor) );      
// pixels.show();
}



void pixelRender(){
  if (settingMode == 0){
    for (int i=0; i < NUMPIXELS; i++){
      if (i == (sequence[selectedSequence].activeStep) ) {
        pixels.setPixelColor(i, pixels.Color(255,255,255) );      
      } else if ( i == selectedStep) {
         pixels.setPixelColor(selectedStep, Wheel(int(millis()/3)%255) );      
      } else {
        if(sequence[selectedSequence].stepData[i].gateType == 0){
          pixels.setPixelColor(i, pixels.Color(0,0,0));
        } else {
          pixels.setPixelColor(i, Wheel( sequence[selectedSequence].getStepPitch(i) ) );
        }
      }
    }
    pixels.show();
  }
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

void nonBlockingRainbow(uint8_t interval) {
  uint16_t i, j;

    j = positive_modulo( millis()/interval, 255 );

    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((5*i+j) & 255));
    }
    pixels.show();

}

void nonBlockingRainbow(uint8_t interval, uint8_t *skipArr, uint8_t skipArrSize) {
  uint16_t i,n, j;

  j = positive_modulo( millis()/interval, 255 );

  for(i=0; i<pixels.numPixels(); i++) {
    bool skip = false;
    for(n=0; n< skipArrSize; n++){
      if (skipArr[n] == i){
        skip = true;
      }
    }

    if (!skip){
      pixels.setPixelColor(i, Wheel((5*i+j) & 255));
    } else {
      pixels.setPixelColor(i, pixels.Color(10,10,10));
    }
  }

  pixels.show();

}

