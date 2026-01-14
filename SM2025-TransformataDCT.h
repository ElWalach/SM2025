#ifndef SM2025_TRANSFORMATA_DCT_H
#define SM2025_TRANSFORMATA_DCT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

const int rozmiarBloku = 16;

struct macierz {
    float dct[rozmiarBloku][rozmiarBloku];
    Uint8 dane[rozmiarBloku][rozmiarBloku];
};

void wyswietlDane(macierz blok);
void wyswietlDCT(macierz blok);
macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]);
macierz idct(float DCT[rozmiarBloku][rozmiarBloku]);
void wykonajKompresjeObrazu();
vector<float> zigzagDCT(float dctMatrix[16][16]);
void zigzagToMatrix(vector<float> zigzag, float matrix[16][16]);

#endif
