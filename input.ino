//button stuff
#include <Bounce2mcp.h>
#include <Adafruit_MCP23017.h>
#include <Encoder.h>

#define BUTTONPIN0  8 
#define BUTTONPIN1  9 
#define BUTTONPIN2  10 
#define BUTTONPIN3  11 
#define BUTTONPIN4  12 
#define BUTTONPIN5  13
#define BUTTONPIN6  14 
#define BUTTONPIN7  15 
#define BUTTONPIN8  7 
#define BUTTONPIN9  6 
#define BUTTONPIN10 5 
#define BUTTONPIN11 4
#define BUTTONPIN12 3 
#define BUTTONPIN13 2
#define BUTTONPIN14 1 
#define BUTTONPIN15 0

#define SMALLBUTTON0 7
#define SMALLBUTTON1 6
#define SMALLBUTTON2 5
#define SMALLBUTTON3 4
#define SMALLBUTTON4 3
#define SMALLBUTTON5 2
#define SMALLBUTTON6 1

#define ENCODERBUTTON1 14
#define ENCODERBUTTON2 15


BounceMcp buttons[16] = {};
BounceMcp smallButtons[7] = {};
BounceMcp encoderButtons[2] = {};

Adafruit_MCP23017 mcp0;
Adafruit_MCP23017 mcp1;


// Encoder Stuff
#define ENCODER1LEFTPIN 17
#define ENCODER1RIGHTPIN 16
#define ENCODER2LEFTPIN 2
#define ENCODER2RIGHTPIN 3
Encoder knob1(ENCODER1LEFTPIN, ENCODER1RIGHTPIN);
Encoder knob2(ENCODER2LEFTPIN, ENCODER2RIGHTPIN);

// Encoder vars
int32_t knob1Buffer = 0;
int32_t knob2Buffer = 0;
int32_t knob1InitValue = 0;
int32_t knob2InitValue = 0;
int32_t knob1previousValue = 0;
int32_t knob2previousValue = 0;
int16_t modeSelektBuffer;
int16_t pitchBuffer;
int16_t gateLengthBuffer;
int8_t  gateTypeBuffer;


void buttonSetup() {
  Serial.println("button setup start");

  mcp0.begin(4);      // use default address 0
  delay(10);
    Serial.println("button setup middle1");

  mcp1.begin(1);
  delay(10);
  Serial.println("button setup middle2");

  for (int i = 0; i<16; i++) {
    mcp0.pinMode(i, INPUT);
    mcp0.pullUp(i, HIGH);  // turn on a 100K pullup internally
    mcp1.pinMode(i, INPUT);
    mcp1.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }


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
  encoderButtons[1].attach(mcp1, ENCODERBUTTON2,  5 );

  Serial.println("button setup end");

}

inline int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

void buttonLoop(){

  encoderLoop();
  smallButtonLoop();
  encoderButtonLoop();

}

void encoderLoop(){
    knob1Buffer = knob1.read()/-4;
  knob2Buffer = knob2.read()/-4;

  if (knob1Buffer != knob1previousValue) {
    // knob 1 has changed!
    knob1previousValue = knob1Buffer;
   // filter1.frequency(knob1Buffer);
    selectedSequence = positive_modulo(knob1Buffer, 16);
    Serial.println("setting selectedSequence: " + String(selectedSequence));

  }

  if (knob2Buffer != knob2previousValue) {
    // knob 1 has changed!
    knob2previousValue = knob2Buffer;

    switch (modeSelekt) {
      case 0:
      // just change the note
        if (knob2Buffer + sequence[selectedSequence].getStepPitch(selectedStep) < 0){
          // you can turn off a note by turning the value to 0
          // turn off a note by setting gate type and pitch to 0 
          sequence[selectedSequence].gateType[selectedStep] = 0;
          sequence[selectedSequence].setStepPitch(selectedStep, 0);
        } else {
          if(sequence[selectedSequence].gateType[selectedStep] == 0){
            // if a note is not active, turn it on.
            sequence[selectedSequence].gateType[selectedStep] = 1; 
          } 
          // and finally set the new step value!
          sequence[selectedSequence].setStepPitch(selectedStep, positive_modulo(modeSelektBuffer + knob2Buffer, 127));
        }
        break;

      case 1:
      // change the gate type
        sequence[selectedSequence].setGateLength(selectedStep, positive_modulo(modeSelektBuffer + knob2Buffer, 127) );

        break;

      case 2:  
      // change length of gate
        sequence[selectedSequence].setGateType(selectedStep, positive_modulo(modeSelektBuffer + knob2Buffer, 3) );
        break;

      case 3:
        //change the step divider
        sequence[selectedSequence].setStepDivider(positive_modulo(modeSelektBuffer + knob2Buffer, 16));
        break;

      case 4:

        break;
    }


  }

  //modeSelektor
  for (int i=0; i < numSteps; i++){

    buttons[i].update();

    if (buttons[i].fell()){
      if(selectedStep == i && modeSelekt != 1){
        modeSelekt = 1; // change the step length
        knob2.write(0);
        modeSelektBuffer = sequence[selectedSequence].gateLength[i];
      } else if (selectedStep == i && modeSelekt == 1){
        modeSelekt = 2; // change the step length
        knob2.write(0);
        modeSelektBuffer = sequence[selectedSequence].gateType[i];
      } else {
        modeSelekt = 0;
        selectedStep = i;
        // since the selected step changed, we need to reset the knob2 value to 0
        knob2.write(0);
        modeSelektBuffer = sequence[selectedSequence].getStepPitch(selectedStep);
      }

    }

  }

}

void smallButtonLoop(){
  for (int i=0; i <7; i++){
    smallButtons[i].update();

    if (smallButtons[i].fell()){
      switch (i){
        // left row bottom up
        case 0:
          playing = !playing;
          break;

        case 1:
          modeSelekt = 3;
          modeSelektBuffer = sequence[selectedSequence].stepDivider;
          break;

        case 2:
          break;

        case 3:
          break;

        case 4: // randomize the current sequence
          for(int i=0; i <16; i++){
            sequence[selectedSequence].gateType[i] = random(2);
            sequence[selectedSequence].setStepPitch(i, random(1, 127));
          }
          break;

        // right two, bottom up
        case 5:
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
  for (int i=0; i <2; i++){
    encoderButtons[i].update();
    if (encoderButtons[i].read() == LOW){

    }
  }
}