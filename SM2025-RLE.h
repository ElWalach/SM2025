#ifndef SM2025_RLE_H_INCLUDED
#define SM2025_RLE_H_INCLUDED

#include <SDL2/SDL.h>
#include <iostream>


void RLEKompresja(int wejscie[], int dlugosc, const char* plik);


void RLEDekompresja(const char* plik);
#endif // SM2025-RLE_H_INCLUDED
