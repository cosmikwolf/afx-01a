void changePattern(uint8_t pattern, boolean saveFirst, boolean instant){
	//Serial.println("currentPattern: " + String(currentPattern) + "\tsequenceCount: " + String(sequenceCount));
	if(saveFirst){
		saveCurrentPattern();
	}

  if (instant || !playing) {
    loadPattern(pattern);    
    Serial.println("Changing pattern instantly: " + String(pattern) + " instant: " + String(instant) + " playing: " + String(playing));
  } else {
    queuePattern = pattern;
    Serial.println("Queueing pattern: " + String(pattern));
  }

}


void saveCurrentPattern() {

  Serial.println("Saving to EEPROM - " + String(micros()));
  need2save = false;  
  for(int i=0; i < sequenceCount; i++){

  	int index = int(( i  + currentPattern * sequenceCount ) * ( sizeof(sequence[0].stepData) + sizeof(sequence[0].stepCount) + sizeof(sequence[0].beatCount) )); 
		Serial.print(String(index) + " ");
    EEPROM_writeAnything(index, sequence[i].stepData);
    index += sizeof(sequence[i].stepData);								
 		EEPROM_writeAnything(index, sequence[i].stepCount);
    index += sizeof(sequence[i].stepCount);
 		EEPROM_writeAnything(index, sequence[i].beatCount);

  }

  Serial.println();
	Serial.println("Done updating EEPROM..." + String(micros()));
	
}

void loadPattern(uint8_t pattern) {

	Serial.println("loading pattern: " + String(pattern)); 
//	if (instant == true) {
	  for(int i=0; i < sequenceCount; i++){

    	int index = int(( i  + pattern * sequenceCount ) * ( sizeof(sequence[0].stepData) + sizeof(sequence[0].stepCount) + sizeof(sequence[0].beatCount) )); 

    	EEPROM_readAnything(index, sequence[i].stepData);
    	index += sizeof(sequence[i].stepData);								
 			EEPROM_readAnything(index, sequence[i].stepCount);
    	index += sizeof(sequence[i].stepCount);
 			EEPROM_readAnything(index, sequence[i].beatCount);

    }
//  } else {
  	//return pattern;
//  }

  currentPattern = pattern;
  Serial.println("Pattern " + String(pattern) + " loaded");
}