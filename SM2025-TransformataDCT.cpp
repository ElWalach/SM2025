#include "SM2025-TransformataDCT.h"

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

    for (int v = 0; v < rozmiarBloku; ++v) {
        for (int u = 0; u < rozmiarBloku; ++u) {
            const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
            const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
            double wspolczynnikDCT = 0;

            for (int y = 0; y < rozmiarBloku; ++y) {
                for (int x = 0; x < rozmiarBloku; ++x) {
                    double uCosFactor = cos((double)(2 * x + 1) * M_PI *
                        (double)u / (2 * (double) rozmiarBloku));
                    double vCosFactor = cos((double)(2 * y + 1) * M_PI *
                        (double)v / (2 * (double) rozmiarBloku));
                    double pixel = (double)wartosci[x][y];
                    wspolczynnikDCT += pixel * uCosFactor * vCosFactor;
                }
            }
            wspolczynnikDCT *= (2.0 / (double) rozmiarBloku) * cu * cv;
            wynik[u][v] = wspolczynnikDCT;
        }
    }

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

    for (int x = 0; x < rozmiarBloku; ++x) {
        for (int y = 0; y < rozmiarBloku; ++y) {
            double pixel = 0;

            for (int u = 0; u < rozmiarBloku; ++u) {
                for (int v = 0; v < rozmiarBloku; ++v) {
                    const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
                    const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
                    double uCosFactor = cos((double)(2 * x + 1) * M_PI *
                        (double)u / (2 * (double) rozmiarBloku));
                    double vCosFactor = cos((double)(2 * y + 1) * M_PI *
                        (double)v / (2 * (double) rozmiarBloku));
                    double wspolczynnikDCT = DCT[u][v];
                    pixel += wspolczynnikDCT * uCosFactor * cu * vCosFactor * cv;
                }
            }
            pixel *= (2.0 / (double) rozmiarBloku);
            wynik[x][y] = round(pixel);
        }
    }

    macierz rezultat;
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
