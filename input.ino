//button stuff
#include <Bounce2mcp.h>
#include <Adafruit_MCP23017.h>
#include <Encoder.h>


#define BUTTONPIN0  0 
#define BUTTONPIN1  4 
#define BUTTONPIN2  8 
#define BUTTONPIN3  12 
#define BUTTONPIN4  1 
#define BUTTONPIN5  5
#define BUTTONPIN6  9 
#define BUTTONPIN7  13 
#define BUTTONPIN8  2 
#define BUTTONPIN9  6 
#define BUTTONPIN10 10 
#define BUTTONPIN11 14
#define BUTTONPIN12 3 
#define BUTTONPIN13 7
#define BUTTONPIN14 11 
#define BUTTONPIN15 15

#define SMALLBUTTON0 9
#define SMALLBUTTON1 2
#define SMALLBUTTON2 1
#define SMALLBUTTON3 5
#define SMALLBUTTON4 0
#define SMALLBUTTON5 4
#define SMALLBUTTON6 8
#define SMALLBUTTON7 6
#define SMALLBUTTON8 10

#define ENCODERBUTTON1 12

BounceMcp buttons[16] = {};
BounceMcp smallButtons[9] = {};
BounceMcp encoderButtons[1] = {};

Adafruit_MCP23017 mcp0;
Adafruit_MCP23017 mcp1;

// Encoder Stuff
#define ENCODER1LEFTPIN 17
#define ENCODER1RIGHTPIN 16
//#define ENCODER2LEFTPIN 16
//#define ENCODER2RIGHTPIN 17
Encoder knob1(ENCODER1LEFTPIN, ENCODER1RIGHTPIN);
//Encoder knob2(ENCODER2LEFTPIN, ENCODER2RIGHTPIN);

// Encoder vars
int32_t knob1Buffer = 0;
//int32_t knob2Buffer = 0;
int32_t knob1InitValue = 0;
//int32_t knob2InitValue = 0;
int32_t knob1previousValue = 0;
//int32_t knob2previousValue = 0;
int16_t stepModeBuffer;
int16_t pitchBuffer;
int16_t gateLengthBuffer;
int8_t  gateTypeBuffer;

void buttonSetup() {
  Serial.println("button setup start");
  delay(10);

  mcp0.begin(0);      // use default address 0
  delay(10);

  mcp1.begin(1);
  delay(10);

  Serial.println("button setup start 2");

  for (int i = 0; i<16; i++) {
   mcp0.pinMode(i, INPUT);
   mcp0.pullUp(i, HIGH);  // turn on a 100K pullup internally
   mcp1.pinMode(i, INPUT);
   mcp1.pullUp(i, HIGH);  // turn on a 100K pullup internally
 }

  Serial.println("button setup start 3");

  buttons[0].attach(mcp0, BUTTONPIN0,   0 ); 
  buttons[1].attach(mcp0, BUTTONPIN1,   0 ); 
  buttons[2].attach(mcp0, BUTTONPIN2,   0 ); 
  buttons[3].attach(mcp0, BUTTONPIN3,   0 ); 
  buttons[4].attach(mcp0, BUTTONPIN4,   0 ); 
  buttons[5].attach(mcp0, BUTTONPIN5,   0 ); 
  buttons[6].attach(mcp0, BUTTONPIN6,   0 ); 
  buttons[7].attach(mcp0, BUTTONPIN7,   0 ); 
  buttons[8].attach(mcp0, BUTTONPIN8,   0 ); 
  buttons[9].attach(mcp0, BUTTONPIN9,   0 ); 
  buttons[10].attach(mcp0, BUTTONPIN10, 0 ); 
  buttons[11].attach(mcp0, BUTTONPIN11, 0 ); 
  buttons[12].attach(mcp0, BUTTONPIN12, 0 ); 
  buttons[13].attach(mcp0, BUTTONPIN13, 0 ); 
  buttons[14].attach(mcp0, BUTTONPIN14, 0 ); 
  buttons[15].attach(mcp0, BUTTONPIN15, 0 );

  smallButtons[0].attach(mcp1, SMALLBUTTON0,  5 ); 
  smallButtons[1].attach(mcp1, SMALLBUTTON1,  5 ); 
  smallButtons[2].attach(mcp1, SMALLBUTTON2,  5 ); 
  smallButtons[3].attach(mcp1, SMALLBUTTON3,  5 ); 
  smallButtons[4].attach(mcp1, SMALLBUTTON4,  5 ); 
  smallButtons[5].attach(mcp1, SMALLBUTTON5,  5 ); 
  smallButtons[6].attach(mcp1, SMALLBUTTON6,  5 ); 
  
  encoderButtons[0].attach(mcp1, ENCODERBUTTON1,  5 ); 
// encoderButtons[1].attach(mcp1, ENCODERBUTTON2,  5 );
//
  Serial.println("button setup end");

}

inline int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

unsigned long encoderLoopTime;
unsigned long smallButtonLoopTime;
unsigned long encoderButtonTime;
unsigned long matrixButtonTime;

void buttonLoop(){
  unsigned long loopTimer = micros();
  encoderLoop();
    encoderLoopTime = ((micros() - loopTimer) + 9*encoderLoopTime)/10;
    loopTimer = micros();
  smallButtonLoop();
    smallButtonLoopTime = ((micros() - loopTimer) + 9*smallButtonLoopTime)/10;
    loopTimer = micros();
  encoderButtonLoop();
    encoderButtonTime = ((micros() - loopTimer) + 9*encoderButtonTime)/10;
    loopTimer = micros();
  matrixButtonLoop();
    matrixButtonTime = ((micros() - loopTimer) + 9*matrixButtonTime)/ 10;
    loopTimer = micros();
/* if (millis() % 100 == 0){
   Serial.println("encoderLoopTime: " + String(encoderLoopTime) +
     "\tsmallButtonLoopTime: " + String(smallButtonLoopTime) +
    "\tencoderButtonTime: " + String(encoderButtonTime) +
    "\tmatrixButtonTime: " + String(matrixButtonTime) );
 }
 */
}

void encoderLoop(){
  knob1Buffer = knob1.read()/-4;
 // knob2Buffer = knob2.read()/-4;

  if (knob1Buffer != knob1previousValue) {
    // knob 1 has changed!
    knob1previousValue = knob1Buffer;
    switch (settingMode) {
      case 0: // step mode
        switch (stepMode) {
          case 0:
          // just change the note
            if (knob1Buffer + sequence[selectedSequence].getStepPitch(selectedStep) < 0){
              // you can turn off a note by turning the value to 0
              // turn off a note by setting gate type and pitch to 0 
              sequence[selectedSequence].stepData[selectedStep].gateType = 0;
              sequence[selectedSequence].setStepPitch(selectedStep, 0);
              knob1.write(4);  
              } else {
              if(sequence[selectedSequence].stepData[selectedStep].gateType == 0){
                // if a note is not active, turn it on.
                sequence[selectedSequence].stepData[selectedStep].gateType = 1; 
              } 
              // and finally set the new step value!
              sequence[selectedSequence].setStepPitch(selectedStep, positive_modulo(stepModeBuffer + knob1Buffer, 127));
            }
            break;

          case 1:
          // change the gate type
            sequence[selectedSequence].setGateLength(selectedStep, positive_modulo(stepModeBuffer + knob1Buffer, 127) );

            break;

          case 2:  
          // change length of gate
            sequence[selectedSequence].setGateType(selectedStep, positive_modulo(stepModeBuffer + knob1Buffer, 3) );
            break;

          case 3:
            break;

          case 4:

            break;
        }
        break;

      case SEQUENCE_SPED: // speed setting
        switch(menuSelection){
          case 0:
            sequence[selectedSequence].setStepCount( positive_modulo(stepModeBuffer + knob1Buffer, 63)+1 );
            break;
          case 1: 
            sequence[selectedSequence].setBeatCount( positive_modulo(stepModeBuffer + knob1Buffer, 127) + 1 );
            break;
        }
        break;
      case SEQUENCE_INST:
          int instBuffer = sequence[selectedSequence].instrument;
          sequence[selectedSequence].instrument = positive_modulo(knob1Buffer,127);
          sam2695.programChange(0, selectedSequence, sequence[selectedSequence].instrument);

        break;


      }




  }

/*
  if (knob2Buffer != knob2previousValue) {
    // knob 1 has changed!
    knob2previousValue = knob2Buffer;
   // filter1.frequency(knob1Buffer);
    selectedSequence = positive_modulo(knob2Buffer, sequenceCount);
    Serial.println("setting selectedSequence: " + String(selectedSequence));


  } */
}

void matrixButtonLoop(){

  for (int i=0; i < numSteps; i++){

    buttons[i].update();
    
    if (buttons[i].fell()){
      switch (settingMode) {
        case 0:
          stepModeButtonHandler(i);
          break; 
        case 1:
          sequencerMenuButtonHandler(i);
          break;
        case 2:
          globalMenuButtonHandler(i);
          break;
        default:
          menuItemButtonHandler(settingMode, i);
          break;
      }
    }
    
  }
}

// adding a menu item requires an entry here, in the menuItemButtonHandler, as well as a 
// menuItem case in the display.ino file. all cases should be referenced by a #define that 
// represents the menu item.

void menuItemButtonHandler(uint8_t selectedMode, uint8_t buttonNum){
  switch (selectedMode) {
    case GLOBAL_MIDI:
      switch(buttonNum){
        case 0:
          extClock = !extClock;
          break;
      }
      break;
    case SEQUENCE_SPED:
      switch(buttonNum){
        case 0:
          menuSelection = 0;
          stepModeBuffer = sequence[selectedSequence].stepCount;
          break;
        case 4:
          menuSelection = 1;
          stepModeBuffer = sequence[selectedSequence].beatCount;
          break;
      }
      break;
    case SEQUENCE_GENE:
    //      display.println("Clear All Notes");
    //  display.println("Random some notes");
     // display.println("Random all notes");

      switch(buttonNum){
        case 0:
          Serial.println("Clear All Notes");
          for(int i=0; i <16; i++){
            sequence[selectedSequence].stepData[i].gateType = 0;//random(2);
            sequence[selectedSequence].stepData[i].gateLength = 1;//random(2);
            sequence[selectedSequence].stepData[i].velocity = 72;//random(2);
            sequence[selectedSequence].setStepPitch(i, 24);
          }
          sequence[selectedSequence].stepCount = 16;//random(2);
          sequence[selectedSequence].beatCount = 4;//random(2);
          settingMode = 0;
          break;
        case 4:
          Serial.println("Random some notes");
          for(int i=0; i <16; i++){
            sequence[selectedSequence].stepData[i].gateType = random(2);
            sequence[selectedSequence].setStepPitch(i, random(1, 127));
          }
          settingMode = 0;
          break;
        case 8:
          Serial.println("Random all notes");
          for(int i=0; i <16; i++){
            sequence[selectedSequence].stepData[i].gateType = 1;//random(2);
            sequence[selectedSequence].setStepPitch(i, random(1, 127));
          };
          settingMode = 0;
          break;
      }
      break;

      case SEQUENCE_QUAN:
        switch(buttonNum){
          case 0:
            sequence[selectedSequence].quantizeKey = 1;
            settingMode = 0;
          break;
          case 4:
            sequence[selectedSequence].quantizeKey = 2;
            settingMode = 0;
          break;
          case 8:
            sequence[selectedSequence].quantizeKey = 0;
            settingMode = 0;
          break;
        }
      break;
    case PATTERN_SELECT:

    //  changePattern(buttonNum, true, true);

      if (smallButtons[4].read() == false){
        changePattern(buttonNum, true, true);
      } else {
        changePattern(buttonNum, true, false);
      }

      settingMode = 0;
      break;
   
    case SEQUENCE_SELECT:
      if (buttonNum < sequenceCount){
        selectedSequence = buttonNum;
        Serial.println("setting selectedSequence: " + String(selectedSequence));
        settingMode = 0;
      }
      break;


   }

};


void sequencerMenuButtonHandler(uint8_t buttonId){
  menuSelection = 127;
  switch (buttonId) {
    case 0:
      settingMode = SEQUENCE_NAME;
      break;
    case 1:
      settingMode = SEQUENCE_SPED;
      break;
    case 2:
      settingMode = SEQUENCE_TRAN;
      break;
    case 3:
      settingMode = SEQUENCE_INST;
      break;
    case 4:
      settingMode = SEQUENCE_QUAN;
      break;
    case 5:
      settingMode = SEQUENCE_EUCL;
      break;
    case 6:
      settingMode = SEQUENCE_GENE;
      break;
    case 7:
      settingMode = SEQUENCE_ORDE;
      break;
    case 8:
      settingMode = SEQUENCE_RAND;
      break;
    case 9:
      settingMode = SEQUENCE_POSI;
      break;
    case 10:
      settingMode = SEQUENCE_GLID;
      break;
    case 11:
      settingMode = 0;
      break;
    case 12:
      settingMode = SEQUENCE_MIDI;
      break;
    case 13:
      settingMode = SEQUENCE_CV;
      break;
    case 14:
      settingMode = SEQUENCE_GATE;
      break;
    case 15:
      settingMode = 0;
      break;
  }

}

void globalMenuButtonHandler(uint8_t buttonId){
    switch (buttonId) {
      case 0:
        settingMode = GLOBAL_MIDI;
        break;
      case 1:
        settingMode = GLOBAL_SAVE;
        break;
      case 2:
        settingMode = GLOBAL_LOAD;
        break;
      case 3:
        settingMode = GLOBAL_FILE;
        break;
      default:
        settingMode = 0;
        break;
  }
}

void stepModeButtonHandler(uint8_t i){
  need2save = true;
  saveTimer = 0;
  if(selectedStep == i && stepMode == 0){
    stepMode = 1; // change the step length
    knob1.write(0);
    stepModeBuffer = sequence[selectedSequence].stepData[i].gateLength;
  } else if (selectedStep == i && stepMode != 0){
    stepMode = positive_modulo(stepMode + 1, 3); // change the step length
    knob1.write(0);
    stepModeBuffer = sequence[selectedSequence].stepData[i].gateType;
  } else {
    stepMode = 0;
    selectedStep = i;
    // since the selected step changed, we need to reset the knob2 value to 0
    knob1.write(0);
    knob1previousValue = 0;
    stepModeBuffer = sequence[selectedSequence].getStepPitch(selectedStep);
  }
}




void smallButtonLoop(){
  for (int i=0; i <9; i++){
    smallButtons[i].update();
    if (smallButtons[i].fell()){
          Serial.println("button fell " + String(i));

      switch (i){
        // left row bottom up
        case 0:
          if (settingMode == PATTERN_SELECT){
            stepMode = 0;
            settingMode = 0;
          } else {
            stepMode = 4; 
            settingMode = PATTERN_SELECT;
          }
          break;

        case 1:
          stepMode = 3;
          break;

        case 2:
          stepMode = 4;
          settingMode = TEMPO_SET;
          break;

        case 3:
          settingMode = positive_modulo(settingMode + 1, 3);
          break;

        case 4:
          playing = false;
          for(int s = 0; s < sequenceCount; s++){
            sequence[s].activeStep = 0;
            sam2695.allNotesOff(s);
          }
          break;

        // right two, bottom up
        case 5:
          playing = !playing;
          break;

        case 6:
          break;

      }
    }
  }


  if (smallButtons[0].fell()){
  }
}
 
void encoderButtonLoop(){
  encoderButtons[0].update();
  if ( encoderButtons[0].fell() ){
    if (settingMode == SEQUENCE_SELECT){
      stepMode = 0;
      settingMode = 0;
    } else {
      stepMode = 4; 
      settingMode = SEQUENCE_SELECT;
    }  
  }
}

