#include "SM2025-ByteRun.h"
#include "SM2025-Funkcje.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//ByteRun

void ByteRunKompresja(int wejscie[], int dlugosc, const char* plikWyjsciowy){
    ofstream plik(plikWyjsciowy, ios::binary);
    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku wyjsciowego!" << endl;
        return;
    }

    int rozmiarSkompresowany = 0;
    int i = 0;

    while(i < dlugosc){
        if((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])){
            int j = 0;
            while((i + j < dlugosc - 1) &&
                  (wejscie[i + j] == wejscie[i + 1 + j]) &&
                  (j < 127)){
                j++;
            }

            cout << "(" << -j << "), " << (int)wejscie[i + j] << ", ";


            unsigned char naglowek = (unsigned char)(-j);
            unsigned char wartosc = (unsigned char)wejscie[i + j];
            plik.write((char*)&naglowek, 1);
            plik.write((char*)&wartosc, 1);
            rozmiarSkompresowany += 2;

            i += (j + 1);
        }
        else {
            int j = 0;
            while((i + j < dlugosc - 1) &&
                  ((wejscie[i + j] != wejscie[i + j + 1])) &&
                  (j < 128)){
                j++;
            }
            if((i + j == dlugosc - 1) && (j < 128)){
                j++;
            }

            cout << "(" << (j - 1) << "), ";
            for(int k = 0; k < j; k++){
                cout << (int)wejscie[i + k] << ", ";
            }


            unsigned char naglowek = (unsigned char)(j - 1);
            plik.write((char*)&naglowek, 1);
            rozmiarSkompresowany++;

            for(int k = 0; k < j; k++){
                unsigned char wartosc = (unsigned char)wejscie[i + k];
                plik.write((char*)&wartosc, 1);
                rozmiarSkompresowany++;
            }

            i += j;
        }
    }

    plik.close();
    cout << endl << "Rozmiar oryginalny: " << dlugosc << " bajtow" << endl;
    cout << "Rozmiar po kompresji: " << rozmiarSkompresowany << " bajtow" << endl;
}



void ByteRunDekompresja(const char* plikWejsciowy) {
    ifstream plikIn(plikWejsciowy, ios::binary);

    if (!plikIn.is_open()) {
        cerr << "Blad otwarcia pliku!" << endl;
        return;
    }

    cout << "\n Przed dekompresja: " << endl;


    vector<unsigned char> dane;
    unsigned char bajt;
    while (plikIn.read((char*)&bajt, 1)) {
        dane.push_back(bajt);
    }


    for (size_t i = 0; i < dane.size(); i++) {
        if (dane[i] > 128) {

            cout << "(" << -((int)(256 - dane[i])) << "), " << (int)dane[i+1] << ", ";
            i++;
        } else {

            int liczba = dane[i] + 1;
            cout << "(" << (int)dane[i] << "), ";
            for (int j = 1; j <= liczba && (i + j) < dane.size(); j++) {
                cout << (int)dane[i + j] << ", ";
            }
            i += liczba;
        }
    }

    cout << "\n\n Po dekompresji" << endl;


    for (size_t i = 0; i < dane.size(); i++) {
        if (dane[i] > 128) {

            int liczbaPowtorzen = 257 - dane[i];
            unsigned char wartosc = dane[i + 1];

            for (int j = 0; j < liczbaPowtorzen; j++) {
                cout << (int)wartosc << ", ";
            }
            i++;
        } else {

            int liczbaLiteralow = dane[i] + 1;
            for (int j = 1; j <= liczbaLiteralow && (i + j) < dane.size(); j++) {
                cout << (int)dane[i + j] << ", ";
            }
            i += liczbaLiteralow;
        }
    }

    cout << endl;
    plikIn.close();
}
