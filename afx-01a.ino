#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerThree.h>
#include <Adafruit_NeoPixel.h>
#include <Fluxamasynth.h>
#include <Sequencer.h>
#include <MIDI.h>

// Neopixel Stuff
#define NEOPIXELPIN       21
#define NUMPIXELS         16
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXELPIN, NEO_RGB + NEO_KHZ800);

// Display Stuff
#define OLED_DC     4
#define OLED_CS     5
#define OLED_RESET  999 
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

// Fluxamasynth
Fluxamasynth synth = Fluxamasynth();
uint8_t instrument = 38;
uint8_t instrumentPrevious;
bool settingInstrument;

float midiFreq[128] = { 8.17, 8.66, 9.17, 9.72, 10.30, 10.91, 11.56, 12.24, 12.97, 13.75, 14.56, 15.43, 16.35, 17.32, 18.35, 19.44, 20.60, 21.82, 23.12, 24.49, 25.95, 27.50, 29.13, 30.86, 32.70, 34.64, 36.70, 38.89, 41.20, 43.65, 46.24, 48.99, 51.91, 55.00, 58.27, 61.73, 65.40, 69.29, 73.41, 77.78, 82.40, 87.30, 92.49, 97.99, 103.82, 110.00, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 184.99, 195.99, 207.65, 220.00, 233.08, 246.94, 261.62, 277.18, 293.66, 311.12, 329.62, 349.22, 369.99, 391.99, 415.30, 440.00, 466.16, 493.88, 523.25, 554.36, 587.32, 622.25, 659.25, 698.45, 739.98, 783.99, 830.60, 880.00, 932.32, 987.76, 1046.50, 1108.73, 1174.65, 1244.50, 1318.51, 1396.91, 1479.97, 1567.98, 1661.21, 1760.00, 1864.65, 1975.53, 2093.00, 2217.46, 2349.31, 2489.01, 2637.02, 2793.82, 2959.95, 3135.96, 3322.43, 3520.00, 3729.31, 3951.06, 4186.00, 4434.92, 4698.63, 4978.03, 5274.04, 5587.65, 5919.91, 6271.92, 6644.87, 7040.00, 7458.62, 7902.13, 8372.01, 8869.84, 9397.27, 9956.06, 10548.08, 11175.30, 11839.82, 12543.85 };

// Sequencer stuff - clean this shit up!
// Things that are definitely needed:
const char* midiNotes[] = { "C0","C#0","D0","D#0","E0","F0","F#0","G0","G#0","A0","A#0","B0","C1","C#1","D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1","C2","C#2","D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2","C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3","C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4","C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5","C6","C#6","D6","D#6","E6","F6","F#6","G6","G#6","A6","A#6","B6","C7","C#7","D7","D#7","E7","F7","F#7","G7","G#7","A7","A#7","B7","C8","C#8","D8","D#8","E8","F8","F#8","G8","G#8","A8","A#8","B8","C9","C#9","D9","D#9","E9","F9","F#9","G9","G#9","A9","A#9","B9","C10","C#10","D10","D#10","E10","F10","F#10","G10" };
int8_t  modeSelekt; // 0 - change step pitch, 1 - change step length
boolean playing = false;

// things of which I am unsure
IntervalTimer masterClock;
IntervalTimer midiClockSync;
const char* instrumentNames[] = {"None","(Grand) Piano 1","(Bright) Piano 2","(El, Grd) Piano 3","Honky-tonk Piano","El. Piano 1","El. Piano 2","Harpsichord","Clavi","Celesta","Glockenspiel","Music Box","Vibraphone","Marimba","Xylophone","Tubular Bells","Santur","Drawbar Organ","Percussive Organ","Rock Organ","Church Organ","Reed Organ","Accordion (French)","Harmonica","Tango Accordion","Accoustic Guitar (nylon)","Accoustic Guitar (steel)","El. Guitar (jazz)","El. Guitar (clean)","El. Guitar (muted)","Overdriven Guitar","Distortion Guitar","Guitar Harmonics","Acoustic Bass","Finger Bass","Picked Bass","Fretless Bass","Slap Bass 1","Slap Bass 2","Synth Bass 1","Synth Bass 2","Violin","Viola","Cello","Contrabass","Tremolo Strings","Pizzicato Strings","Orchestral Harp","Timpani","String Ensemble 1","String Ensemble 2","Synth Strings 1","Synth Strings 2","Choir Aahs","Voice Oohs","Synth Voice","Orchestra Hit","Trumpet","Trombone","Tuba","Muted Trumpet","French Horn","Brass Section","Synth Brass 1","Synth Brass 2","Soprano Sax","Alto Sax","Tenor Sax","Baritone Sax","Oboe","English Horn","Bassoon","Clarinet","Piccolo","Flute","Recorder","Pan Flute","Blown Bottle","Shakuhachi","Whistle","Ocarina","Lead 1 (square)","Lead 2 (sawtooth)","Lead 3 (calliope)","Lead 4 (chiff)","Lead 5 (charang)","Lead 6 (voice)","Lead 7 (fifths)","Lead 8 (bass+lead)","Pad 1 (fantasia)","Pad 2 (warm)","Pad 3 (polysynth)","Pad 4 (choir)","Pad 5 (bowed)","Pad 6 (metallic)","Pad 7 (halo)","Pad 8 (sweep)","FX 1 (rain)","FX 2 (soundtrack)","FX 3 (crystal)","FX4 (atmosphere)","FX 5 (brightness)","FX 6 (goblins)","FX 7 (echoes)","FX 8 (sci-fi)","Sitar","Banjo","Shamisen","Koto","Kalimba","Bagpipe","Fiddle","Shanai","Tinkle Bell","Agogo","Steel Drums","Woodblock","Taiko Drum","Melodic Tom","Synth Drum","Reverse Cymbal","Guitar Fret Noise","Breath Noise","Seashore","Bird Tweet","Teleph. Ring","Helicopter","Applause","Gunshot"};
uint8_t selectedStep = 0;
uint8_t multiSelect[16];
uint8_t selectedStepColor = 0;
uint8_t numSteps = 16;
uint16_t tempo = 120;
uint16_t previousTempo;
uint16_t stepsPerBeat = 2;
uint16_t previousStepsPerBeat = 2;
int8_t gateType[128];
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
uint8_t currentStepInitPitch = 0;
uint8_t stepLengthBuffer;
uint8_t sequenceLengthBuffer;
float timerInterruptInterval = 100000.0;
float clockSyncInterruptInterval = 100.0;
uint8_t selectedSequence = 0;


unsigned long lastRunTime = 0;

int iter = 1;
elapsedMillis testTimer1;
elapsedMillis testTimer2;
int avgPeriod = 0;
int avgRuntime = 0;
bool tempoBool = false;
elapsedMicros loopTimer = 0;
int avgLoopTime = 0;
elapsedMicros runTimer = 0;
String activeSection;
// Lets Begin!

Sequencer sequence[16];

void setup(){
  SPI.setMOSI(7);
  SPI.setSCK(14);
  Serial.begin(38400);

  MIDI.begin(MIDI_CHANNEL_OMNI);

  display.begin(SSD1306_SWITCHCAPVCC);

  sequence[0].initialize(1, 16, 1, tempo, 115);
  sequence[1].initialize(2, 16, 2, tempo, 116);
  sequence[2].initialize(3, 16, 3, tempo, 39);
  sequence[3].initialize(4, 16, 4, tempo, 115);
  sequence[4].initialize(5, 16, 5, tempo, 116);
  sequence[5].initialize(6, 16, 5, tempo, 39);
  sequence[6].initialize(7, 16, 4, tempo, 115);
  sequence[7].initialize(8, 16, 4, tempo, 116);
  sequence[8].initialize(9, 16, 4, tempo, 39);
  sequence[9].initialize(10, 16, 4, tempo, 115);
  sequence[10].initialize(11, 16, 4, tempo, 115);
  sequence[11].initialize(12, 16, 4, tempo, 116);
  sequence[12].initialize(13, 16, 4, tempo, 39);
  sequence[13].initialize(14, 16, 4, tempo, 115);
  sequence[14].initialize(15, 16, 4, tempo, 116);
  sequence[15].initialize(16, 16, 4, tempo, 39);

  pixels.begin();

  startupSequence();
  buttonSetup();  
  audioSetup();
  Serial.println("Beginning Master Clock");
  masterClock.begin(masterClockFunc,timerInterruptInterval);
  Serial.println("Setup Complete");
  midiClockSync.begin(midiClockSyncFunc, clockSyncInterruptInterval);
}

void loop(){
  
 // selectedSequence = selectedSequence;
  avgLoopTime = (loopTimer + 99*avgLoopTime)/100;
  loopTimer = 0;
  // Serial.println("Loop Start");
  buttonLoop();
  //Serial.println("Button Loop Complete");
  ledLoop();
  // Serial.println("LED Loop Complete");
  displayLoop();
  // Serial.println("Display Loop Complete");
  if (pixelTimer > 10000) {
   // Serial.print("pixelTimerStart");
    pixelRender();
    pixelTimer = 0;
    //Serial.print("PixeltimerEnd");
  }
  //   Serial.print("AudioLoop Start");
  audioLoop();
  // Serial.print("LoopEnd");
}



