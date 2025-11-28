#ifndef SM2025_BYTERUN_H_INCLUDED
#define SM2025_BYTERUN_H_INCLUDED

#include <SDL2/SDL.h>
#include <iostream>

//ByteRun

void ByteRunKompresja(int wejscie[], int dlugosc, const char* plikWyjsciowy);

void ByteRunDekompresja(const char* plikWejsciowy);
#endif  //SM2025_BYTERUN_H_INCLUDED
