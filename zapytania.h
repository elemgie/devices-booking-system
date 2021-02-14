//Mateusz Gieroba (322072)
#ifndef zapytania_h
#define zapytania_h
#include "urzadzenia.h"
#include "wypozyczenia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern bool wczytajLinie(char *s, int n);
int wyznaczPoczatekTygodnia(int day, int month); //zwraca liczbę oznaczającą godzinę 00:00 w poniedziałek w danym tygodniu
void dostepnoscUrzadzen(Urzadzenie *Urzadzenia);
void drukujPlanWypozyczen(Urzadzenie *Urzadzenia);
void wypozyczeniaOsoby(Wypozyczenie *Wypozyczenia, Urzadzenie *Urzadzenia);

#endif