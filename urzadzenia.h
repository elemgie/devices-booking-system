#ifndef urzadzenia_h
#define urzadzenia_h
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct urzadzenie{

int id; //unikalne id
int typ;
char nazwa[31];
char opis[201];
bool czyAktywny;
} Urzadzenie;

bool wczytajLinie(char *s, int n);
Urzadzenie *wczytajUrzadzenia();
bool zapiszUrzadzenia(Urzadzenie *U);
Urzadzenie *rozszerzUrzadzenia(Urzadzenie *U);
Urzadzenie *dodajUrzadzenie(Urzadzenie *U);
void wypiszUrzadzenia(Urzadzenie *U, int typ, bool czyDoPliku);
void usunUrzadzenie(Urzadzenie *U, int id); //funkcja dezaktywuje rekord ale go nie usuwa - dla celów administracyjnych dane wciąż są zapisane
void modyfikujUrzadzenie(Urzadzenie *U, int id);

#endif