//Mateusz Gieroba (322072)
#ifndef wypozyczenia_h
#define wypozyczenia_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "urzadzenia.h"

int maxrents, currents;

//System obsługuje okres od 24 lutego 2021 do 30 września 2021 dzieląc dzień na 24 godzinne części, 0 - 0:00 24 luty, 5255 - 23:00 30 września
typedef struct wypozyczenie{
  int id, idurzadzenia, poczatek, koniec;
  bool czySemestralne;
  char osoba[80];
  bool czyAktywne;
} Wypozyczenie;
extern bool wczytajLinie(char *s, int n);
int dataNaLiczbe(int day, int month, int hour);
void liczbaNaDate(int liczba, char *s);
Wypozyczenie *wczytajWypozyczenia(Urzadzenie *Urzadzenia);
bool czyDostepneWDanymOkresie(Urzadzenie *Urzadzenia, int id, int poczatek, int koniec);
void zajmijOkresDostepnosci(Urzadzenie *Urzadzenia, int id, int idwyp, int poczatek, int koniec);
Wypozyczenie *dodajWypozyczenie(Wypozyczenie *Wypozyczenia, Urzadzenie *Urzadzenia);
void usunWypozyczenie(Wypozyczenie *Wypozyczenia, int idwypozyczenia, Urzadzenie *Urzadzenia);
void wypiszWypozyczenia(Wypozyczenie *Wypozyczenia, int id, bool czyDoPliku);
bool zapiszWypozyczenia(Wypozyczenie *Wypozyczenia);
Wypozyczenie *rozszerzWypozyczenia(Wypozyczenie *stareWypozyczenia);
void usunRezerwacjeSkasowanegoUrzadzenia(int id, Wypozyczenie *Wypozyczenia, Urzadzenie *Urzadzenia);
#endif