#include "SM2025-TransformataDCT.h"
#include "SM2025-Zmienne.h"
#include "SM2025-Funkcje.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

void wyswietlDane(macierz blok) {
    cout << "Dane pikselowe w macierzy:" << endl;
    for (int y = 0; y < rozmiarBloku; y++) {
        for (int x = 0; x < rozmiarBloku; x++) {
            cout << setw(4) << (int)blok.dane[x][y] << " ";
        }
        cout << endl;
    }
}

void wyswietlDCT(macierz blok) {
    cout << "Wspolczynniki transformaty w macierzy:" << endl;
    for (int y = 0; y < rozmiarBloku; y++) {
        for (int x = 0; x < rozmiarBloku; x++) {
            cout << fixed << setw(6) << setprecision(2) << blok.dct[x][y] << " ";
        }
        cout << endl;
    }
}

macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]) {
    float wynik[rozmiarBloku][rozmiarBloku];

    // Obliczamy DCT
    for (int v = 0; v < rozmiarBloku; v++) {
        for (int u = 0; u < rozmiarBloku; u++) {
            const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
            const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
            double wspolczynnikDCT = 0;

            for (int y = 0; y < rozmiarBloku; y++) {
                for (int x = 0; x < rozmiarBloku; x++) {
                    double uCosFactor = cos((double)(2 * x + 1) * M_PI * (double)u / (2 * (double)rozmiarBloku));
                    double vCosFactor = cos((double)(2 * y + 1) * M_PI * (double)v / (2 * (double)rozmiarBloku));
                    double pixel = (double)wartosci[x][y];
                    wspolczynnikDCT += pixel * uCosFactor * vCosFactor;
                }
            }
            wspolczynnikDCT *= (2.0 / (double)rozmiarBloku) * cu * cv;
            wynik[u][v] = wspolczynnikDCT;
        }
    }

    // Przygotowujemy macierz wyjsciowa
    macierz rezultat;
    for (int j = 0; j < rozmiarBloku; j++) {
        for (int i = 0; i < rozmiarBloku; i++) {
            rezultat.dct[i][j] = wynik[i][j];
            rezultat.dane[i][j] = wartosci[i][j];
        }
    }
    return rezultat;
}

macierz idct(float DCT[rozmiarBloku][rozmiarBloku]) {
    int wynik[rozmiarBloku][rozmiarBloku];

    // Obliczamy IDCT
    for (int x = 0; x < rozmiarBloku; x++) {
        for (int y = 0; y < rozmiarBloku; y++) {
            double pixel = 0;

            for (int u = 0; u < rozmiarBloku; u++) {
                for (int v = 0; v < rozmiarBloku; v++) {
                    const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
                    const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
                    double uCosFactor = cos((double)(2 * x + 1) * M_PI * (double)u / (2 * (double)rozmiarBloku));
                    double vCosFactor = cos((double)(2 * y + 1) * M_PI * (double)v / (2 * (double)rozmiarBloku));
                    double wspolczynnikDCT = DCT[u][v];
                    pixel += wspolczynnikDCT * uCosFactor * cu * vCosFactor * cv;
                }
            }
            pixel *= (2.0 / (double)rozmiarBloku);
            wynik[x][y] = round(pixel);
        }
    }

    macierz rezultat;
    // Przygotowujemy macierz wyjsciowa i normalizujemy wartosci
    for (int j = 0; j < rozmiarBloku; j++) {
        for (int i = 0; i < rozmiarBloku; i++) {
            if (wynik[i][j] > 255) wynik[i][j] = 255;
            if (wynik[i][j] < 0) wynik[i][j] = 0;
            rezultat.dane[i][j] = wynik[i][j];
            rezultat.dct[i][j] = DCT[i][j];
        }
    }
    return rezultat;
}


void wykonajKompresjeObrazu() {
    // Krok 1: Konwersja na skalê szaroœci (lewa po³owa ekranu)
    SDL_Color kolor;
    vector<vector<float>> obrazSzary(szerokosc/2, vector<float>(wysokosc/2));

    for (int y = 0; y < wysokosc/2; y++) {
        for (int x = 0; x < szerokosc/2; x++) {
            kolor = getPixel(x, y);
            // Konwersja RGB na jasnoœæ (Y)
            float jasnosc = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
            obrazSzary[x][y] = jasnosc;

            // Wyœwietlenie w skali szaroœci po prawej stronie
            Uint8 szary = (Uint8)SDL_clamp((int)jasnosc, 0, 255);
            setPixel(x + szerokosc/2, y, szary, szary, szary);
        }
    }

    SDL_UpdateWindowSurface(window);
    SDL_Delay(1000);

    // Krok 2-9: Przetwarzanie bloków 16x16
    int blokSize = 16;
    int blokiX = (szerokosc/2) / blokSize;
    int blokiY = (wysokosc/2) / blokSize;

    for (int by = 0; by < blokiY; by++) {
        for (int bx = 0; bx < blokiX; bx++) {
            // Krok 2: Wyodrêbnienie bloku 16x16
            Uint8 blok[16][16];
            for (int y = 0; y < blokSize; y++) {
                for (int x = 0; x < blokSize; x++) {
                    int px = bx * blokSize + x;
                    int py = by * blokSize + y;
                    blok[x][y] = (Uint8)obrazSzary[px][py];
                }
            }

            // Krok 3: DCT
            macierz dctWynik = dct(blok);

            // Krok 4: Zbieranie zygzakiem
            vector<float> zigzag;
            for (int diag = 0; diag < 2*blokSize-1; diag++) {
                if (diag % 2 == 0) { // Parzysta przek¹tna - w dó³
                    for (int i = min(diag, blokSize-1); i >= max(0, diag-blokSize+1); i--) {
                        int j = diag - i;
                        zigzag.push_back(dctWynik.dct[j][i]);
                    }
                } else { // Nieparzysta przek¹tna - w górê
                    for (int i = max(0, diag-blokSize+1); i <= min(diag, blokSize-1); i++) {
                        int j = diag - i;
                        zigzag.push_back(dctWynik.dct[j][i]);
                    }
                }
            }

            // Krok 5: Modyfikacja wspó³czynników
            // a) Wyzerowanie ostatnich wspó³czynników (zachowujemy 50%)
            int zachowaj = zigzag.size() / 2;
            for (int i = zachowaj; i < zigzag.size(); i++) {
                zigzag[i] = 0.0f;
            }

            // b) Zaokr¹glenie niezerowych wspó³czynników
            for (int i = 0; i < zachowaj; i++) {
                if (fabs(zigzag[i]) > 0.001f) {
                    zigzag[i] = roundf(zigzag[i]);
                }
            }

            // Krok 6: Wyœwietlenie pierwszych kilku wspó³czynników (opcjonalnie)
            if (bx == 0 && by == 0) {
                cout << "Pierwsze 10 wspó³czynników po modyfikacji:" << endl;
                for (int i = 0; i < min(10, (int)zigzag.size()); i++) {
                    cout << fixed << setprecision(2) << zigzag[i] << " ";
                }
                cout << endl;
            }

            // Krok 7: Odbudowanie macierzy wspó³czynników
            float odtworzonyDCT[16][16] = {0};
            int idx = 0;
            for (int diag = 0; diag < 2*blokSize-1; diag++) {
                if (diag % 2 == 0) {
                    for (int i = min(diag, blokSize-1); i >= max(0, diag-blokSize+1); i--) {
                        int j = diag - i;
                        odtworzonyDCT[j][i] = (idx < zigzag.size()) ? zigzag[idx] : 0.0f;
                        idx++;
                    }
                } else {
                    for (int i = max(0, diag-blokSize+1); i <= min(diag, blokSize-1); i++) {
                        int j = diag - i;
                        odtworzonyDCT[j][i] = (idx < zigzag.size()) ? zigzag[idx] : 0.0f;
                        idx++;
                    }
                }
            }

            // Krok 8: iDCT
            macierz idctWynik = idct(odtworzonyDCT);

            // Krok 9: Wizualizacja wyników
            for (int y = 0; y < blokSize; y++) {
                for (int x = 0; x < blokSize; x++) {
                    int px = bx * blokSize + x;
                    int py = by * blokSize + y;
                    Uint8 wartosc = idctWynik.dane[x][y];
                    // Wyœwietlamy w prawej dolnej æwiartce ekranu
                    setPixel(px + szerokosc/2, py + wysokosc/2, wartosc, wartosc, wartosc);
                }
            }
        }
    }

    SDL_UpdateWindowSurface(window);
    cout << "Kompresja DCT zakonczona. Lewa gora: oryginal, Prawa gora: szary, Prawy dol: skompresowany" << endl;
}




