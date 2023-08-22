#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <Arduino.h>

void readFromMemory(char* Printerip, char* Printercode, char* PrinterID, char* EspPassword);

void writeToMemory(char* Printerip, char* Printercode, char* PrinterID, char* EspPassword);

void clearMemory();

#endif 