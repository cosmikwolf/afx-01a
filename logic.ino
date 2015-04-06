void changePattern(uint8_t pattern, boolean saveFirst){
	if(saveFirst){
	    Serial.println("Saving to EEPROM - " + String(micros()));
	    need2save = false;  
	    for(int i=0; i < sequenceCount; i++){
	    int index = int(i * sizeof(sequence[0].stepData)+ (currentPattern * sequenceCount * sizeof(sequence[0].stepData) ) ); 
	      EEPROM_writeAnything(index, sequence[i].stepData);
	    }
	    Serial.println("Done updating EEPROM..." + String(micros()));
	}

	Serial.println("loading pattern: " + String(pattern));

  for(int i=0; i< sequenceCount; i++){
    int index = int(i * sizeof(sequence[0].stepData)+ (pattern * sequenceCount * sizeof(sequence[0].stepData) ) ); 
    EEPROM_readAnything(index, sequence[i].stepData);
  }

  currentPattern = pattern;

}