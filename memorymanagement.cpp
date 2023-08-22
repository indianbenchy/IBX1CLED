#include <EEPROM.h>
#include "memorymanagement.h"
#include "globalvariables.h"

void fillWithUnderscores(char* text, int length) { //Fills whitespace with underscores for MEMORY to overwrite
    int textLength = strlen(text);
    for(int i=textLength; i<length; i++) {
        text[i] = '_';
    }
    text[length] = '\0'; // add null terminator
}

void removeUnderscores(char* text) { //Removes underscores from message
  for(int i = 0; i < strlen(text); i++){
    if(text[i] == '_'){
      for(int j = i; j < strlen(text); j++){
        text[j] = text[j+1];
      }
      i--;
    }
  }
}

void readFromMemory(char* Printerip, char* Printercode, char* PrinterID, char* EspPassword) { //Function to write the PrinterID, Printerip and AccessCode to the eeprom
  Serial.println("Reading from eeprom");
  char ParserIPmemory[ipLength+1] = "";
  for (int i = 0; i < ipLength; i++) {
    ParserIPmemory[i] = EEPROM.read(Ip_Adress+i);
  }
  ParserIPmemory[ipLength] = '\0';
  removeUnderscores(ParserIPmemory);
  for(int i = 0; i < strlen(ParserIPmemory); i++){
    if(ParserIPmemory[i] == 'Q'){
      ParserIPmemory[i] = '.';
    }
  }
  
  char ParsedCodeMemory[accessCodeLength+1] = "";
  for (int i = 0; i < accessCodeLength; i++) {
    ParsedCodeMemory[i] = EEPROM.read(Accesscode + i);
  }
  ParsedCodeMemory[accessCodeLength] = '\0';
  removeUnderscores(ParsedCodeMemory);

  char ParsedIdMemory[DeviceIdLength+1] = "";
  for (int i = 0; i < DeviceIdLength; i++) {
    ParsedIdMemory[i] = EEPROM.read(DeviceId + i);
  }
  ParsedIdMemory[DeviceIdLength] = '\0';
  removeUnderscores(ParsedIdMemory);

  char ParsedEspPassword[EspPasswordLength+1] = "";
  for (int i = 0; i < EspPasswordLength; i++) {
    ParsedEspPassword[i] = EEPROM.read(EspPasswordAddress + i);
  }
  ParsedEspPassword[EspPasswordLength] = '\0';

  Serial.println(ParserIPmemory);
  Serial.println(ParsedCodeMemory);
  Serial.println(ParsedIdMemory);
  Serial.println(ParsedEspPassword);

  strcpy(Printerip, ParserIPmemory);
  strcpy(Printercode, ParsedCodeMemory);
  strcpy(PrinterID, ParsedIdMemory);
  strcpy(EspPassword, ParsedEspPassword);
}

void writeToMemory(char* Printerip, char* Printercode, char* PrinterID, char* EspPassword) { //Function to read the PrinterID, Printerip and AccessCode from the eeprom
    int ipLength = strlen(Printerip);
    int codeLength = strlen(Printercode);
    int idLength = strlen(PrinterID);
  
    char parsediparg[ipLength+1];
    char parsedcodearg[accessCodeLength+1];
    char parsedID[DeviceIdLength+1];
  
    strcpy(parsediparg, Printerip);
    strcpy(parsedcodearg, Printercode);
    strcpy(parsedID, PrinterID);

    fillWithUnderscores(parsediparg, ipLength);
    fillWithUnderscores(parsedcodearg, accessCodeLength);
    fillWithUnderscores(parsedID, DeviceIdLength);
  
    Serial.println(parsediparg);
    Serial.println(parsedcodearg);
    Serial.println(parsedID);

    Serial.println("Writing to eeprom");

    for (int i = 0; i < ipLength; i++) {
        EEPROM.write(Ip_Adress + i , parsediparg[i]);
    }

    for (int i = 0; i < codeLength; i++) {
        EEPROM.write(Accesscode + i, parsedcodearg[i]);
    }

    for (int i = 0; i < idLength; i++) {
        EEPROM.write(DeviceId + i, parsedID[i]);
    }

    for (int i = 0; i < strlen(EspPassword); i++) {
        EEPROM.write(EspPasswordAddress + i, EspPassword[i]);
    }

    EEPROM.commit();

    Serial.println("Finished Writing to eeprom");
}

void clearMemory(){ //Incase eeprom gets messed up set pin 6 to high to clear data
    Serial.println("Clearing EEPROM");
    for (int i = 0; i < 512; i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    Serial.println("EEPROM Cleared");
}