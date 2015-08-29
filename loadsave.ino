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

void initializeFlashMemory(){
  pinMode(10, OUTPUT);
  delay(10);
  if (!SD.begin(10)) {
    Serial.println("SD Card initialization failed!");
    return;
  }
    SD.remove("data.txt");
  if (SD.exists("data.txt")) {
    Serial.println("data.txt exists.");
  } else {
    Serial.println("data.txt does not exist, creating file...");
    saveData = SD.open("data.txt", FILE_WRITE);
    saveData.close();
  } 

  Serial.println("SD Card and save file initialization complete.");
}

void saveCurrentPattern() {

  Serial.println("Saving to SD Card - " + String(micros()));
  need2save = false;  
  saveData = SD.open("data.txt", FILE_WRITE);

 for(int i=0; i < sequenceCount; i++){

 	int index = int(( i  + currentPattern * sequenceCount ) 
    * ( sizeof(sequence[0].stepData) 
      + sizeof(sequence[0].stepCount) 
      + sizeof(sequence[0].beatCount) 
      + sizeof(sequence[0].quantizeKey) )); 
  
	Serial.print(String(index) + " ");

  saveData.seek(index);
  //saveData.write(String(i));
   saveData.write((byte *) &sequence[i].stepData,sizeof(sequence[i].stepData));
   saveData.write((byte *) &sequence[i].stepData,sizeof(sequence[i].stepCount));
   saveData.write((byte *) &sequence[i].stepData,sizeof(sequence[i].beatCount));
   saveData.write((byte *) &sequence[i].stepData,sizeof(sequence[i].quantizeKey));

 //   EEPROM_writeAnything(index, sequence[i].stepData);
 //   index += sizeof(sequence[i].stepData);								
 //		EEPROM_writeAnything(index, sequence[i].stepCount);
 //   index += sizeof(sequence[i].stepCount);
 //   EEPROM_writeAnything(index, sequence[i].beatCount);
 //   index += sizeof(sequence[i].quantizeKey);
 //   EEPROM_writeAnything(index, sequence[i].quantizeKey);
  }
     saveData.println("\r");

  saveData.close();

  Serial.println(" ");
	Serial.println("Done saving to SD..." + String(micros()));


  saveData = SD.open("data.txt");
  if (saveData) {
    Serial.println("reading data.txt: " + String(saveData.size()) + " bytes");

    // read from the file until there's nothing else in it:
    while (saveData.available()) {
      Serial.print( String( saveData.read() ) + " " );
    }
    // close the file:
    saveData.close();
    Serial.println();
    Serial.println("reading complete");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening data.txt");
  }
	
}

void loadPattern(uint8_t pattern) {

	Serial.println("loading pattern: " + String(pattern)); 
//	if (instant == true) {
    saveData = SD.open("data.txt");

	  for(int i=0; i < sequenceCount; i++){

  	int index = int(( i  + pattern * sequenceCount ) * ( sizeof(sequence[0].stepData) + sizeof(sequence[0].stepCount) + sizeof(sequence[0].beatCount) )); 
    Serial.print(String(index) + " ");

    saveData.seek(index);
    saveData.read((byte *) &sequence[i].stepData, sizeof(sequence[i].stepData));
    saveData.read((byte *) &sequence[i].stepCount, sizeof(sequence[i].stepCount));
    saveData.read((byte *) &sequence[i].beatCount, sizeof(sequence[i].beatCount));
    saveData.read((byte *) &sequence[i].quantizeKey, sizeof(sequence[i].quantizeKey));

  //  SD_readAnything(index, sequence[i].stepData, saveData);
  //  index += sizeof(sequence[i].stepData);                
  //  SD_readAnything(index, sequence[i].stepCount, saveData);
  //  index += sizeof(sequence[i].stepCount);
  //  SD_readAnything(index, sequence[i].beatCount, saveData);
  //  index += sizeof(sequence[i].quantizeKey);
  //  SD_readAnything(index, sequence[i].quantizeKey, saveData);

 //   	EEPROM_readAnything(index, sequence[i].stepData);
 //   	index += sizeof(sequence[i].stepData);								
 //			EEPROM_readAnything(index, sequence[i].stepCount);
 //   	index += sizeof(sequence[i].stepCount);
 //			EEPROM_readAnything(index, sequence[i].beatCount);
 //     index += sizeof(sequence[i].quantizeKey);
 //     EEPROM_readAnything(index, sequence[i].quantizeKey);
  
    }
        saveData.close();

//  } else {
  	//return pattern;
//  }

  currentPattern = pattern;
  Serial.println("Pattern " + String(pattern) + " loaded");
}

//template <class T> int SD_readAnything(int sd, T& value, File handle)
//{
//    byte* p = (byte*)(void*)&value;
//    unsigned int i;
//    for (i = 0; i < sizeof(value); i++)
//          *p++ = handle.read(sd++);
//    return i;
//}


void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
