#include "SM2025-ByteRun.h"
#include "SM2025-Funkcje.h"
#include <iostream>

using namespace std;

//ByteRun

void ByteRunKompresja(int wejscie[], int dlugosc){

    int i= 0;

    while(i< dlugosc){

        if((i < dlugosc- 1) && (wejscie[i]== wejscie[ i+ 1 ])){
            int j= 0;
            while (( i+ j< dlugosc- 1) &&
                   (wejscie[i+ j]== wejscie[i+ 1+ j]) &&
                   (j< 127)){
                j++;
                   }
            cout<<"("<< -j <<"), "<<(int)wejscie[ i+ j ]<<", ";

            i+= (j+1);
        }
        else {
           // i++;

           int j= 0;
           while((i+ j< dlugosc- 1) &&
                    ((wejscie[ i+ j]!= wejscie[i+ j+ 1])) &&
                    (j< 128)){
                j++;
            }

            if(( i+ j== dlugosc- 1) && ( j< 128 )){
                j++;
            }

            cout<<"("<<( j- 1)<<"), ";
            for( int k= 0; k< j; k++){
                cout<<(int)wejscie[ i+ k ]<<", ";
            }

            i+= j;
        }
    }
}
