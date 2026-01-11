#include "SM2025-LZW.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <fstream>

using namespace std;

int rozmiarSlownika = 0;
slowo slownik[65535];

void LZWinicjalizacja() {
    rozmiarSlownika = 0;
    for (int s = 0; s < 65536; s++) {
        slownik[s].kod = 0;
        slownik[s].dlugosc = 0;
        slownik[s].wSlowniku = false;
        memset(slownik[s].element, 0, sizeof(slownik[s].element));
    }

    slowo noweSlowo;
    for (int s = 0; s < 4; s++) {
        noweSlowo.dlugosc = 1;
        noweSlowo.element[0] = s;
        noweSlowo.kod = dodajDoSlownika(noweSlowo, true);
    }
}

int dodajDoSlownika(slowo nowy, bool czyWyswietlac) {
    if (rozmiarSlownika < 65536) {
        Uint16 nr = rozmiarSlownika;
        slownik[nr].kod = nr;
        slownik[nr].dlugosc = nowy.dlugosc;
        copy(begin(nowy.element), end(nowy.element), begin(slownik[nr].element));
        slownik[nr].wSlowniku = true;
        if (czyWyswietlac)
            wyswietlSlowo(slownik[nr]);
        rozmiarSlownika++;
        return nr;
    }
    return -1;
}

slowo noweSlowo() {
    slowo nSlowo;
    nSlowo.kod = 0;
    nSlowo.dlugosc = 0;
    nSlowo.wSlowniku = false;
    return nSlowo;
}

slowo noweSlowo(Uint8 znak) {
    slowo nSlowo;
    nSlowo.kod = 0;
    nSlowo.dlugosc = 1;
    nSlowo.element[0] = znak;
    nSlowo.wSlowniku = false;
    return nSlowo;
}

slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak) {
    slowo nSlowo;
    if (aktualneSlowo.dlugosc < 4096) {
        nSlowo.kod = 0;
        nSlowo.dlugosc = aktualneSlowo.dlugosc + 1;
        nSlowo.wSlowniku = false;
        copy(begin(aktualneSlowo.element), end(aktualneSlowo.element), begin(nSlowo.element));
        nSlowo.element[aktualneSlowo.dlugosc] = znak;
        return nSlowo;
    } else {
        cout << "UWAGA! przepelnienie rozmiaru znakow w pojedynczym slowie" << endl;
        nSlowo.kod = 0;
        nSlowo.dlugosc = 0;
        nSlowo.wSlowniku = false;
        nSlowo.element[0] = znak;
        return nSlowo;
    }
}

void wyswietlSlowo(slowo aktualneSlowo) {
    if (aktualneSlowo.wSlowniku)
        cout << "[" << aktualneSlowo.kod << "] ";
    else
        cout << "[X] ";
    for (int s = 0; s < aktualneSlowo.dlugosc; s++) {
        cout << (int)aktualneSlowo.element[s];
        if (s < aktualneSlowo.dlugosc - 1)
            cout << ", ";
    }
    cout << endl;
}

void wyswietlSlownik() {
    for (int nr = 0; nr < rozmiarSlownika; nr++)
        wyswietlSlowo(slownik[nr]);
}

int znajdzWSlowniku(slowo szukany) {
    for (int nr = 0; nr < rozmiarSlownika; nr++)
        if (porownajSlowa(slownik[nr], szukany))
            return nr;
    return -1;
}

bool porownajSlowa(slowo slowo1, slowo slowo2) {
    if (slowo1.dlugosc != slowo2.dlugosc)
        return false;
    for (int s = 0; s < slowo1.dlugosc; s++) {
        if (slowo1.element[s] != slowo2.element[s])
            return false;
    }
    return true;
}

void LZWKompresja(int wejscie[], int dlugosc) {
    LZWinicjalizacja();
    slowo aktualneSlowo = noweSlowo();
    slowo slowoZnak;
    Uint8 znak;
    int kod;
    int i = 0;

    Uint16* skompresowane = new Uint16[dlugosc + 1];
    int dlugoscS = 0;

    while (i < dlugosc) {
        znak = wejscie[i];
        cout << "pobieramy znak " << (int)znak << " z pozycji " << i << endl;
        slowoZnak = polaczSlowo(aktualneSlowo, znak);
        cout << "aktualne slowo: ";
        wyswietlSlowo(aktualneSlowo);
        cout << "slowo+znak: ";
        wyswietlSlowo(slowoZnak);
        kod = znajdzWSlowniku(slowoZnak);
        cout << "czy w slowniku? ";

        if (kod < 0) {
            cout << "NIE" << endl;
            cout << "na wyjscie: [" << aktualneSlowo.kod << "]" << endl;
            skompresowane[dlugoscS++] = aktualneSlowo.kod;
            dodajDoSlownika(slowoZnak, false);
            if (znajdzWSlowniku(slowoZnak) > 0) {
                slowoZnak.kod = znajdzWSlowniku(slowoZnak);
                cout << "dodajemy do slownika ";
                wyswietlSlowo(slownik[slowoZnak.kod]);
            }
            aktualneSlowo = noweSlowo(znak);
            aktualneSlowo.kod = znajdzWSlowniku(aktualneSlowo);
            aktualneSlowo.wSlowniku = true;
        } else {
            cout << "TAK: [" << kod << "]" << endl;
            aktualneSlowo = slowoZnak;
            aktualneSlowo.kod = znajdzWSlowniku(aktualneSlowo);
            aktualneSlowo.wSlowniku = true;
        }
        i++;
    }
    cout << "koniec danych" << endl;
    cout << "na wyjscie: [" << aktualneSlowo.kod << "]" << endl;
    skompresowane[dlugoscS++] = aktualneSlowo.kod;

    cout << endl;
    cout << "skompresowane wyjscie (LZW):" << endl;
    for (int c = 0; c < dlugoscS; c++)
        cout << skompresowane[c] << ", ";
    cout << endl;

    int rozmiarPrzed = dlugosc * sizeof(int);
    int rozmiarPo = sizeof(int) + dlugoscS * sizeof(Uint16);
    float stopienKompresji = (float)rozmiarPo / (float)rozmiarPrzed;

    cout << endl;
    cout << "rozmiar przed kompresja: " << rozmiarPrzed << " bajtow" << endl;
    cout << "rozmiar po kompresji: " << rozmiarPo << " bajtow" << endl;
    cout << "stopien kompresji: " << stopienKompresji << endl;

    ofstream plik("kompresjaLZW.bin", ios::binary);
    if (plik.is_open()) {
        plik.write((char*)&dlugoscS, sizeof(int));
        plik.write((char*)skompresowane, dlugoscS * sizeof(Uint16));
        plik.close();
        cout << "zapisano do pliku kompresjaLZW.bin" << endl;
    }

    cout << endl;
    cout << "aktualny slownik" << endl;
    wyswietlSlownik();

    delete[] skompresowane;
}

void LZWDekompresja(const char* plikWejsciowy) {
    ifstream plik(plikWejsciowy, ios::binary);
    if (!plik.is_open()) {
        cout << "Blad otwarcia pliku: " << plikWejsciowy << endl;
        return;
    }

    int dlugoscS;
    plik.read((char*)&dlugoscS, sizeof(int));

    Uint16* skompresowane = new Uint16[dlugoscS];
    plik.read((char*)skompresowane, dlugoscS * sizeof(Uint16));
    plik.close();

    cout << "wczytane skompresowane dane:" << endl;
    for (int c = 0; c < dlugoscS; c++)
        cout << skompresowane[c] << ", ";
    cout << endl << endl;

    LZWinicjalizacja();

    int* wyjscie = new int[dlugoscS * 4096];
    int dlugoscW = 0;

    Uint16 staryKod = skompresowane[0];
    cout << "kod [" << staryKod << "] znajduje sie w slowniku" << endl;
    cout << "na wyjscie:[" << staryKod << "] ";
    for (int s = 0; s < slownik[staryKod].dlugosc; s++) {
        wyjscie[dlugoscW++] = slownik[staryKod].element[s];
        cout << (int)slownik[staryKod].element[s];
        if (s < slownik[staryKod].dlugosc - 1)
            cout << ", ";
    }
    cout << endl;

    Uint8 znak = slownik[staryKod].element[0];

    for (int i = 1; i < dlugoscS; i++) {
        Uint16 nowyKod = skompresowane[i];
        slowo slowo_kod;

        if (nowyKod < rozmiarSlownika) {
            cout << "kod [" << nowyKod << "] znajduje sie w slowniku" << endl;
            slowo_kod = slownik[nowyKod];
            cout << "na wyjscie:[" << nowyKod << "] ";
            for (int s = 0; s < slowo_kod.dlugosc; s++) {
                wyjscie[dlugoscW++] = slowo_kod.element[s];
                cout << (int)slowo_kod.element[s];
                if (s < slowo_kod.dlugosc - 1)
                    cout << ", ";
            }
            cout << endl;

            znak = slowo_kod.element[0];
            slowo nowyElement = polaczSlowo(slownik[staryKod], znak);
            cout << "nowy element slownika: {" << (int)slownik[staryKod].element[0];
            for (int s = 1; s < slownik[staryKod].dlugosc; s++)
                cout << ", " << (int)slownik[staryKod].element[s];
            cout << "} + pierwszy znak poprzedniego wyjscia" << endl;

            dodajDoSlownika(nowyElement, false);
            cout << "dodajemy do slownika ";
            wyswietlSlowo(slownik[rozmiarSlownika - 1]);
        } else {
            cout << "kod [" << nowyKod << "] nie znajduje sie w slowniku" << endl;
            slowo_kod = polaczSlowo(slownik[staryKod], znak);
            cout << "nowy element slownika: {";
            for (int s = 0; s < slowo_kod.dlugosc; s++) {
                cout << (int)slowo_kod.element[s];
                if (s < slowo_kod.dlugosc - 1)
                    cout << ", ";
            }
            cout << "}" << endl;

            dodajDoSlownika(slowo_kod, false);
            cout << "dodajemy do slownika ";
            wyswietlSlowo(slownik[rozmiarSlownika - 1]);

            cout << "na wyjscie:[" << (rozmiarSlownika - 1) << "] ";
            for (int s = 0; s < slowo_kod.dlugosc; s++) {
                wyjscie[dlugoscW++] = slowo_kod.element[s];
                cout << (int)slowo_kod.element[s];
                if (s < slowo_kod.dlugosc - 1)
                    cout << ", ";
            }
            cout << endl;

            znak = slowo_kod.element[0];
        }

        staryKod = nowyKod;
    }

    cout << "koniec danych" << endl << endl;
    cout << "aktualny slownik:" << endl;
    wyswietlSlownik();

    cout << endl << "zdekompresowane dane:" << endl;
    for (int c = 0; c < dlugoscW; c++)
        cout << wyjscie[c] << ", ";
    cout << endl;

    delete[] skompresowane;
    delete[] wyjscie;
}
