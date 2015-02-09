
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
#include <Sequencer.h>
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
IntervalTimer masterClock;

const char* midiNotes[] = { "C0","C#0","D0","D#0","E0","F0","F#0","G0","G#0","A0","A#0","B0","C1","C#1","D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1","C2","C#2","D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2","C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3","C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4","C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5","C6","C#6","D6","D#6","E6","F6","F#6","G6","G#6","A6","A#6","B6","C7","C#7","D7","D#7","E7","F7","F#7","G7","G#7","A7","A#7","B7","C8","C#8","D8","D#8","E8","F8","F#8","G8","G#8","A8","A#8","B8","C9","C#9","D9","D#9","E9","F9","F#9","G9","G#9","A9","A#9","B9","C10","C#10","D10","D#10","E10","F10","F#10","G10" };
const char* instrumentNames[] = {"None","(Grand) Piano 1","(Bright) Piano 2","(El, Grd) Piano 3","Honky-tonk Piano","El. Piano 1","El. Piano 2","Harpsichord","Clavi","Celesta","Glockenspiel","Music Box","Vibraphone","Marimba","Xylophone","Tubular Bells","Santur","Drawbar Organ","Percussive Organ","Rock Organ","Church Organ","Reed Organ","Accordion (French)","Harmonica","Tango Accordion","Accoustic Guitar (nylon)","Accoustic Guitar (steel)","El. Guitar (jazz)","El. Guitar (clean)","El. Guitar (muted)","Overdriven Guitar","Distortion Guitar","Guitar Harmonics","Acoustic Bass","Finger Bass","Picked Bass","Fretless Bass","Slap Bass 1","Slap Bass 2","Synth Bass 1","Synth Bass 2","Violin","Viola","Cello","Contrabass","Tremolo Strings","Pizzicato Strings","Orchestral Harp","Timpani","String Ensemble 1","String Ensemble 2","Synth Strings 1","Synth Strings 2","Choir Aahs","Voice Oohs","Synth Voice","Orchestra Hit","Trumpet","Trombone","Tuba","Muted Trumpet","French Horn","Brass Section","Synth Brass 1","Synth Brass 2","Soprano Sax","Alto Sax","Tenor Sax","Baritone Sax","Oboe","English Horn","Bassoon","Clarinet","Piccolo","Flute","Recorder","Pan Flute","Blown Bottle","Shakuhachi","Whistle","Ocarina","Lead 1 (square)","Lead 2 (sawtooth)","Lead 3 (calliope)","Lead 4 (chiff)","Lead 5 (charang)","Lead 6 (voice)","Lead 7 (fifths)","Lead 8 (bass+lead)","Pad 1 (fantasia)","Pad 2 (warm)","Pad 3 (polysynth)","Pad 4 (choir)","Pad 5 (bowed)","Pad 6 (metallic)","Pad 7 (halo)","Pad 8 (sweep)","FX 1 (rain)","FX 2 (soundtrack)","FX 3 (crystal)","FX4 (atmosphere)","FX 5 (brightness)","FX 6 (goblins)","FX 7 (echoes)","FX 8 (sci-fi)","Sitar","Banjo","Shamisen","Koto","Kalimba","Bagpipe","Fiddle","Shanai","Tinkle Bell","Agogo","Steel Drums","Woodblock","Taiko Drum","Melodic Tom","Synth Drum","Reverse Cymbal","Guitar Fret Noise","Breath Noise","Seashore","Bird Tweet","Teleph. Ring","Helicopter","Applause","Gunshot"};
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
int avgPeriod = 0;
int avgRuntime = 0;
uint8_t sequenceLength = 12;   // sequence length in 1/16th notes
bool    stepActive[128] = {1,1,1,1,1,1,1,1};         // which steps are active
uint8_t stepPitch[128];        // the pitch the step will play
uint8_t stepVelocity[128];     // the velocity of the step
uint8_t stepLength[128] = {1,1,1,1,1,1,1,1};        // the length of the step in 1/16th note

uint8_t activeStep = 0;       // the active step playing
uint8_t lengthTracker = 0;      // number of 1/16th notes since play
uint8_t lastActiveStep = 0;   // the last active step in the stack
uint8_t programmedLength = 0;
elapsedMicros sequenceTimer;
elapsedMicros stepTimer;
elapsedMicros pixelTimer;
elapsedMicros displayTimer;
uint8_t stepLengthBuffer;
uint8_t sequenceLengthBuffer;
uint16_t timerInterruptInterval = 700;

uint8_t selectedSequence = 0;

elapsedMicros tempoTimer = 0;
bool tempoBool = false;
elapsedMicros loopTimer = 0;
int avgLoopTime = 0;

elapsedMicros runTimer = 0;
// Lets Begin!

Sequencer sequence[3];

void setup(){

  sequence[0].initialize(0, 16, 4, tempo, 39, synth);
  sequence[1].initialize(1, 16, 4, tempo, 41, synth);
  sequence[2].initialize(2, 16, 4, tempo, 52, synth);

  Serial.begin(9600);
  Serial1.begin(31250);
  Serial.println("Begin Program");
  synth.setMasterVolume(255);
  pixels.begin();

  display.begin(SSD1306_SWITCHCAPVCC);
  startupSequence();
  buttonSetup();
  // interrupt initial configuration
 // Timer3.initialize(timerInterruptInterval);
  //Timer3.attachInterrupt(timerInterrupt); 

  masterClock.begin(timerInterrupt,timerInterruptInterval);
  Serial.println("Setup Complete");

}

void loop(){
  avgLoopTime = (loopTimer + 99*avgLoopTime)/100;
  loopTimer = 0;
  Serial.println("Loop Start");
  buttonLoop();
  Serial.println("Button Loop Complete");
  ledLoop();
  Serial.println("LED Loop Complete");
//  if (displayTimer > 1000){
    displayLoop();
    Serial.println("Display Loop Complete");
    displayTimer = 0;
//  }
  if (pixelTimer > 10000) {
      Serial.print("pixelTimerStart");
    pixelRender();
    pixelTimer = 0;
      Serial.print("PixeltimerEnd");
  }
  Serial.print("LoopEnd");

}


