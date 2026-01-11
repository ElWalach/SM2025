
#ifndef TRANSFORMATADCT_H
#define TRANSFORMATADCT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <iomanip>
#include <cmath>

const int rozmiarBloku = 16;

struct macierz {
    float dct[rozmiarBloku][rozmiarBloku];
    Uint8 dane[rozmiarBloku][rozmiarBloku];
};

void wyswietlDane(macierz blok);
void wyswietlDCT(macierz blok);
macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]);
macierz idct(float DCT[rozmiarBloku][rozmiarBloku]);

#endif
