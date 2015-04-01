
void midiSetup(){
  MIDI.setHandleClock(midiClockHandler);
  MIDI.setHandleNoteOn(midiNoteOnHandler);
  MIDI.setHandleNoteOff(midiNoteOffHandler);
  MIDI.setHandleStart(midiStartContinueHandler);
  MIDI.setHandleContinue(midiStartContinueHandler);
  MIDI.setHandleStop(midiStopHandler);
}

void midiStopHandler(){
  playing = 0;
}

void midiNoteOffHandler(byte channel, byte note, byte velocity){
  Serial.println(String("Note Off: ch=") + channel + ", note=" + note + ", velocity=" + velocity);
}

void midiNoteOnHandler(byte channel, byte note, byte velocity){
  if (velocity > 0) {
    Serial.println(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity);
  } else {
    Serial.println(String("Note Off: ch=") + channel + ", note=" + note);
  }
}

void midiStartContinueHandler(){
  if (extClock == true) {
    testTimer = 0;
    playing = 1;
    tempoBlip = !tempoBlip;
    blipTimer = 0;
    firstRun = true;
    startTime = 0;
    masterPulseCount = 1;
    masterTempoTimer = 0;
    for (int i=0; i< sequenceCount; i++){
      sequence[i].clockStart(startTime);  
      sequence[i].beatPulse(beatLength);  
    }

    Serial.println("Midi Start / Continue");

    pulseTimer = 0;
  }
}

void midiClockHandler(){
  if (extClock == true) {
    if (playing){
      pulseLength = pulseTimer;
      avgPulseLength = (pulseLength + 23* avgPulseLength) / 24;
      avgPulseJitter = (abs(int(lastPulseLength) - int(pulseLength)) + 23*avgPulseJitter)/24;

   //   Serial.println(String(masterPulseCount) + "\tlastPulseLength: " + String(lastPulseLength) + "\tpulseTimer: " + String(pulseLength) + "\tjitter: " + String(abs(int(lastPulseLength) - int(pulseLength))) + "\tavgPulseLength: " + String(avgPulseLength) + "\tavgPulseJitter: " + String(avgPulseJitter));
      lastPulseLength = pulseLength;
    }
    pulseTimer = 0;

    bpm = 60.0/(24.0*(float(masterTempoTimer)/float(masterPulseCount))/1000000.0);
    avgBpm = (9*avgBpm + bpm)/10;
    //Serial.println(" bpm:" + String(bpm)  + "\tavg: " + Sturing(avgBpm) );
    //20bpm = 124825/1
    //120bpm = 20831/1

    // 1 beat = 24 puleses
    // = 24* avg pulse = 24*(masterTempoTimer/(masterPulseCount+1))
    // = 24*(20831/1) = 499944 microseconds per beat /1000000 = .4999 seconds per beat 
    if (firstRun){

        firstRun = false;

    } else {

      masterPulseCount = (masterPulseCount + 1) % 24;
    //  Serial.println("Midi Clock - mpc: " + String(masterPulseCount) + "\ttempotimer: " + String(masterTempoTimer) );

      if (masterPulseCount == 0){
        //this gets triggered every quarter note
        // just for testing latency
    //   MIDI.sendNoteOn(65,65,3);
    //   MIDI.sendNoteOff(65,65,3);
        beatLength = masterTempoTimer;
        masterTempoTimer = 0;
        tempoBlip = !tempoBlip;
        blipTimer = 0;
        for (int i=0; i< sequenceCount; i++){
          sequence[i].beatPulse(beatLength);  
        }

        Serial.println("beatPulse - beatlength: " + String(int(beatLength)) + "\tbeatLengthJitter: " + String(int(lastBeatLength) - int(beatLength))+ "\tavgPulseLength: " + String(avgPulseLength) + "\tavgPulseJitter: " + String(avgPulseJitter));
        lastBeatLength = beatLength;

      }
    }
    //masterClockFunc();
  }
}

void midiClockSyncFunc(){
  MIDI.read();
  /*  noInterrupts();

  int type, note, velocity, channel, d1, d2;
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    byte type = MIDI.getType();


    if (extClock == true) {
      switch (type) {
        case 250: // midi start
        case 251: // midi continue
         
          break;

        case 252:
          break;
      //  case 242: // midi song position pointer
      //    d1 = MIDI.getData1();
      //    d2 = MIDI.getData2();
      //    Serial.println("MIDI song position pointer: " + String(d1) + " " + String(d2));
      //    if (d1 == 0 && d2 == 0 && playing == 0){ 
      //      for (int i; i < sequenceCount; i++) {
      //        sequence[i].clockStart();  
      //        sequence[i].activeStep = 0;
      //      }
      //      Serial.println("resetting position to 0");
      //    }
      //    break;
        default:
          d1 = MIDI.getData1();
          d2 = MIDI.getData2();
         // bpm = 60/(((int(masterTempoTimer)/(masterPulseCount+10))*24)/1000000);
          Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2 + " mtt:" + String(masterTempoTimer) + "\t mpc: " + String(masterPulseCount) );
       }
    }
    
  }
 
  interrupts(); */
}