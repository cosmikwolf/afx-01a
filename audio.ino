
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include "AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
//#include "AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
//#include "AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/

  AudioSynthWaveform       waveform1_1;      //xy=243,255
  AudioSynthWaveform       waveform2_1;      //xy=225,418
  AudioSynthWaveformDc     staticSignal;
  AudioEffectEnvelope      ampEnvelope_1;      //xy=581,224
  AudioEffectEnvelope      filterEnvelope_1;      //xy=474,459
  AudioMixer4              mixer1_1;         //xy=489,343
  AudioFilterStateVariable filter1_1;        //xy=641,386
  
  AudioAnalyzePeak        peak1;
  AudioAnalyzePeak        peak2;

  AudioSynthWaveform       waveform1_2;      //xy=243,255
  AudioSynthWaveform       waveform2_2;      //xy=225,418
  AudioEffectEnvelope      ampEnvelope_2;      //xy=581,224
  AudioEffectEnvelope      filterEnvelope_2;      //xy=474,459
  AudioMixer4              mixer1_2;         //xy=489,343
  AudioFilterStateVariable filter1_2;        //xy=641,386

  AudioMixer4              mixerLeft;         //xy=489,343
  AudioMixer4              mixerRight;         //xy=489,343
  AudioOutputI2S           audioOut;           //xy=933,329

  AudioPlayMemory    sound0;
  AudioPlayMemory    sound1;  // six memory players, so we can play
  AudioPlayMemory    sound2;  // all six sounds simultaneously
  AudioPlayMemory    sound3;
  AudioMixer4        mix1;    // two 4-channel mixers are needed in
  AudioMixer4        mix2;    // tandem to combine 6 audio sources

  AudioConnection c1(sound0, 0, mix1, 0);
  AudioConnection c2(sound1, 0, mix1, 1);
  AudioConnection c3(sound2, 0, mix1, 2);
  AudioConnection c4(sound3, 0, mix1, 3);
  AudioConnection c8(mix1, 0, mixerLeft, 2);
  AudioConnection c9(mix1, 0, mixerRight, 2);

  AudioConnection patchCord1_1(waveform1_1, 0, mixer1_1, 0);
  AudioConnection patchCord2_1(waveform2_1, 0, mixer1_1, 1);
  AudioConnection patchCord4_1(mixer1_1, ampEnvelope_1);
  AudioConnection patchCord5_1(staticSignal, filterEnvelope_1);
  AudioConnection patchCord6_1(ampEnvelope_1, 0, filter1_1, 0);
  AudioConnection patchCord7_1(filterEnvelope_1, 0, filter1_1, 1);
  AudioConnection patchCord8_1(filter1_1, 0, mixerLeft, 0);
  AudioConnection patchCord9_1(filter1_1, 0, mixerRight, 0);
 
  AudioConnection patchCordPeak(filter1_1, peak1);
  AudioConnection patchCordPeak2(mixer1_1, peak2);
 
  AudioConnection patchCord1_2(waveform1_2, 0, mixer1_2, 0);
  AudioConnection patchCord2_2(waveform2_2, 0, mixer1_2, 1);
  AudioConnection patchCord4_2(mixer1_2, ampEnvelope_2);
  AudioConnection patchCord5_2(staticSignal, filterEnvelope_2);
  AudioConnection patchCord6_2(ampEnvelope_2, 0, filter1_2, 0);
  AudioConnection patchCord7_2(filterEnvelope_2, 0, filter1_2, 1);
  AudioConnection patchCord8_2(filter1_2, 0, mixerLeft, 1);
  AudioConnection patchCord9_2(filter1_2, 0, mixerRight, 1);

  AudioConnection patchCord_L(mixerLeft, 0, audioOut, 0);
  AudioConnection patchCord_R(mixerRight, 0, audioOut, 1);

  AudioControlSGTL5000 sgtl5000_1;     //xy=519,614

void audioSetup(){
  delay(10);
  Serial.println("audio setup");
  AudioMemory(32);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);

  mix1.gain(0, 0.3);
  mix1.gain(1, 0.3);
  mix1.gain(2, 0.3);
  mix1.gain(3, 0.3);

  mixerLeft.gain(0,0.5);
  mixerLeft.gain(1,0.5);
  mixerLeft.gain(2,0.5);
  mixerRight.gain(0,0.5);
  mixerRight.gain(1,0.5);
  mixerRight.gain(2,0.5);


  staticSignal.amplitude(1);
  waveform1_1.begin(0.2, 220.1, WAVEFORM_SQUARE); 
  waveform2_1.begin(0.2, 440, WAVEFORM_SINE); 
  mixer1_1.gain(0, 0.2); 
  mixer1_1.gain(1, 0.2);    
  ampEnvelope_1.delay(0);
  ampEnvelope_1.attack(0);
  ampEnvelope_1.hold(50.0);   
  ampEnvelope_1.decay(200.0);
  ampEnvelope_1.sustain(0.0);
  ampEnvelope_1.release(0.0);
  filterEnvelope_1.delay(0);
  filterEnvelope_1.attack(0);
  filterEnvelope_1.hold(10.0);    
  filterEnvelope_1.decay(70.0);
  filterEnvelope_1.sustain(0.0);
  filterEnvelope_1.release(0.0);
  filter1_1.frequency(200);
  filter1_1.resonance(3);
  filter1_1.octaveControl(3);

  waveform1_2.begin(0.2, 442, WAVEFORM_SQUARE); 
  waveform2_2.begin(0.2, 438, WAVEFORM_SAWTOOTH); 
  mixer1_2.gain(0, 0.2); 
  mixer1_2.gain(1, 0.2);    
  ampEnvelope_2.delay(0);
  ampEnvelope_2.attack(0);
  ampEnvelope_2.hold(50.0);   
  ampEnvelope_2.decay(200.0);
  ampEnvelope_2.sustain(1.0);
  ampEnvelope_2.release(0.0);
  filterEnvelope_2.delay(0);
  filterEnvelope_2.attack(0);
  filterEnvelope_2.hold(10.0);    
  filterEnvelope_2.decay(50.0);
  filterEnvelope_2.sustain(0.2);
  filterEnvelope_2.release(0.0);
  filter1_2.frequency(500);
  filter1_2.resonance(4);
  filter1_2.octaveControl(3);

}

void audioLoop(){
  // noteOn(0, 36);
  //noteOn(1, 24);
  //delay(500);
  // Serial.println("peak: " + String(peak.read())); 
  // noteOff(0);
  // noteOn(0, 48);
  // delay(500);

  //noteOff(1);
  // noteOn(1, 36);
  // noteOff(0);
  // noteOn(0, 60);
  //delay(500);

  // noteOff(0);
  // noteOn(0, 72);
  // delay(500);

  // noteOff(1);
  // noteOn(1, 48);
  // noteOff(0);
  // noteOn(0, 36);
  // delay(500);

  // noteOff(0);
  // noteOn(0, 48);
  // delay(500);

  // noteOff(1);
  // noteOn(1, 60);
  // noteOff(0);
  // noteOn(0, 60);
  // delay(500);

  // noteOff(0);
  // noteOn(0, 72);
  // delay(500);
  // noteOff(0);
  // noteOff(1);

};


void noteOn(uint8_t instrument, uint8_t midiNote){
  if (instrument == 0) { 
    waveform1_1.frequency(midiFreq[abs(midiNote)]);
    waveform2_1.frequency(midiFreq[midiNote]);
    ampEnvelope_1.noteOn();
    filterEnvelope_1.noteOn();
  } else if (instrument == 1 ){
    waveform1_2.frequency(midiFreq[abs(midiNote)]);
    waveform2_2.frequency(midiFreq[midiNote]);
    ampEnvelope_2.noteOn();
    filterEnvelope_2.noteOn();
  }
}


void noteOff(uint8_t instrument){
  if (instrument == 0) { 
    ampEnvelope_1.noteOff();
    filterEnvelope_1.noteOff();
  } else if (instrument == 1 ){
    ampEnvelope_2.noteOff();
    filterEnvelope_2.noteOff();
  }
}


