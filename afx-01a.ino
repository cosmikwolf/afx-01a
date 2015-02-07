
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Encoder.h>
#include <TimerThree.h>
#include <Adafruit_NeoPixel.h>
#include <ClickButton.h>
#include <Fluxamasynth.h>
#include <PgmChange.h>

// Encoder Stuff
#define ENCODER1LEFTPIN 18
#define ENCODER1RIGHTPIN 19
#define ENCODER2LEFTPIN 2
#define ENCODER2RIGHTPIN 3
Encoder knob1(ENCODER1LEFTPIN, ENCODER1RIGHTPIN);
Encoder knob2(ENCODER2LEFTPIN, ENCODER2RIGHTPIN);
int16_t knob1Buffer = 0;
int16_t knob2Buffer = 0;
long knob1InitValue = 0;
long knob2InitValue = 0;

//button stuff
#define BUTTONPIN0  13 // left momentary
#define BUTTONPIN1  16 // middle left momentary
#define BUTTONPIN2  15 // middle right momentary
#define BUTTONPIN3  14 // right rotary
#define BUTTONPIN4  17 // right momentary
#define BUTTONPIN5  20 // left rotary

ClickButton button0(BUTTONPIN0, LOW, CLICKBTN_PULLUP);
ClickButton button1(BUTTONPIN1, LOW, CLICKBTN_PULLUP);
ClickButton button2(BUTTONPIN2, LOW, CLICKBTN_PULLUP);
ClickButton button3(BUTTONPIN3, LOW, CLICKBTN_PULLUP);
ClickButton button4(BUTTONPIN4, LOW, CLICKBTN_PULLUP);
ClickButton button5(BUTTONPIN5, LOW, CLICKBTN_PULLUP);

int clicks[6];
bool buttonPreviousState[6] = {0,0,0,0,0,0};

//Neopixel Stuff

#define PIXELPIN       6
#define NUMPIXELS      8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_RGB + NEO_KHZ800);
int color = 0;


//Display Stuff
#define OLED_MOSI   12
#define OLED_CLK    11
#define OLED_DC     10
#define OLED_CS     8
#define OLED_RESET  9 
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//fluxamasynth stuff

Fluxamasynth synth = Fluxamasynth();
uint8_t instrument = 38;
uint8_t instrumentPrevious;
bool settingInstrument;

// sequencer stuff
const char* midiNotes[] = { "C0","C#0","D0","D#0","E0","F0","F#0","G0","G#0","A0","A#0","B0","C1","C#1","D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1","C2","C#2","D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2","C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3","C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4","C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5","C6","C#6","D6","D#6","E6","F6","F#6","G6","G#6","A6","A#6","B6","C7","C#7","D7","D#7","E7","F7","F#7","G7","G#7","A7","A#7","B7","C8","C#8","D8","D#8","E8","F8","F#8","G8","G#8","A8","A#8","B8","C9","C#9","D9","D#9","E9","F9","F#9","G9","G#9","A9","A#9","B9","C10","C#10","D10","D#10","E10","F10","F#10","G10" };
uint8_t sequenceStep = 1;
uint8_t selectedStep = 0;
uint8_t selectedStepColor = 0;
uint8_t numSteps = 8;
uint16_t tempo = 120;
uint16_t previousTempo;
uint16_t stepsPerBeat = 2;
uint16_t previousStepsPerBeat = 2;
int8_t gateType[128];
uint8_t currentStepInitPitch = 0;
unsigned long lastRunTime = 0;
int avgPeriod = 349;

bool stepActive[128];
int8_t stepPitch[128];
int8_t stepVelocity[128];
int8_t stepLength[128];
int8_t stepGateLength[128]; // 

elapsedMicros sequenceTimer
elapsedMicros stepTimer



elapsedMicros elapsedTimer = 0;
uint16_t timerInterruptInterval = 1000;

elapsedMicros tempoTimer = 0;
bool tempoBool = false;

elapsedMicros loopTimer = 0;
// Lets Begin!

void setup(){

  Serial.begin(9600);
  Serial1.begin(31250);
  synth.setMasterVolume(255);
  synth.programChange(0, 0, instrument);
  pixels.begin();

  display.begin(SSD1306_SWITCHCAPVCC);

  startupSequence();
  buttonSetup();
  // interrupt initial configuration
  Timer3.initialize(timerInterruptInterval);
  Timer3.attachInterrupt(timerInterrupt); 

}

void loop(){

  buttonLoop();
  ledLoop();
  displayLoop();
  loopTimer =0;
}


