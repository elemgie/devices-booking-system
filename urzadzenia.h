#ifndef urzadzenia_h
#define urzadzenia_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    false = 0,
    true  = 1,
} bool;

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int maxdevices, currdevices;

typedef struct urzadzenie{

int id; //unikalne id
int typ;
char nazwa[33];
char opis[203];
int rents[4600];
bool czyAktywny;
} Urzadzenie;

bool wczytajLinie(char *s, int n);
Urzadzenie *wczytajUrzadzenia();
bool zapiszUrzadzenia(Urzadzenie *U);
Urzadzenie *rozszerzUrzadzenia(Urzadzenie *U);
Urzadzenie *dodajUrzadzenie(Urzadzenie *U);
void wypiszUrzadzenia(Urzadzenie *U, int typ, bool czyDoPliku);
bool usunUrzadzenie(Urzadzenie *U, int id); //funkcja dezaktywuje rekord ale go nie usuwa - dla celów administracyjnych dane wciąż są zapisane
void modyfikujUrzadzenie(Urzadzenie *U, int id);

#endif