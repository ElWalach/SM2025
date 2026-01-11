#ifndef SM2025_LZW_H_INCLUDED
#define SM2025_LZW_H_INCLUDED
#include <SDL2/SDL.h>
#include <iostream>

struct slowo {
    Uint16 kod = 0;
    Uint8 dlugosc = 0;
    Uint8 element[4096];
    bool wSlowniku = false;
};

extern int rozmiarSlownika;
extern slowo slownik[65535];

void LZWinicjalizacja();
int dodajDoSlownika(slowo nowy, bool czyWyswietlac = true);
slowo noweSlowo();
slowo noweSlowo(Uint8 znak);
slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak);
void wyswietlSlowo(slowo aktualneSlowo);
void wyswietlSlownik();
int znajdzWSlowniku(slowo szukany);
bool porownajSlowa(slowo slowo1, slowo slowo2);
void LZWKompresja(int wejscie[], int dlugosc);
void LZWDekompresja(const char* plikWejsciowy);

#endif // SM2025_LZW_H_INCLUDED
