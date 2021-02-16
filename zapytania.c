//Mateusz Gieroba (322072)
#include "zapytania.h"
#include "wypozyczenia.h"
#include "urzadzenia.h"

int wyznaczPoczatekTygodnia(int day, int month)
{
  int liczba = dataNaLiczbe(day, month, 0);
  if(liczba < 120)
    return -48;
  return ((liczba - 120)/168)*168 + 120;
}

void dostepnoscUrzadzen(Urzadzenie *Urzadzenia)
{
  FILE *stream;
  int typ;
  char nl, fil;
  printf("Jeżeli chcesz wyszukać tylko urządzenia określonego typu - podaj liczbę od 1 do 5.\n1 - komputer, 2 - projektor, 3 - ekran, 4 - tablet graficzny, 5 - system nagłośnieniowy.\nAby wyszukać wszystkie - wpisz 0\n");
  scanf("%d%c", &typ, &nl);
  if(typ < 0 || typ > 5)
    typ = 0;
  printf("Podaj początek interesującego zakresu w formacie DD/MM HH\n");
  int poczday, poczmonth, poczhour, konday, konmonth, konhour;
  scanf("%d%c%d%c%d%c", &poczday, &nl, &poczmonth, &nl, &poczhour, &nl);
  printf("Podaj koniec interesującego zakresu w formacie DD/MM HH.\n");
  scanf("%d%c%d%c%d%c", &konday, &nl, &konmonth, &nl, &konhour, &nl);
  int poczatek, koniec;
  if((poczatek = dataNaLiczbe(poczday, poczmonth, poczhour)) == -1 || (koniec = dataNaLiczbe(konday, konmonth, konhour)) == -1){
    printf("Błędna data.\n");
    return;
  }
  printf("Czy chcesz zapisać wyniki wyszukiwania do pliku? Wpisz Y/N.\n");
  scanf("%c%c", &fil, &nl);
  if(fil == 'Y')
    stream = fopen("wydruk_zapytania_dostepne.txt", "a+");
  else
    stream = stdout;
  bool czyWypisano = false;
  fprintf(stream, "\n\n\n-----------------------\nUrządzenia ");
  if(typ != 0)
    fprintf(stream, "typu %d ", typ);
  char s[20], k[20];
  liczbaNaDate(poczatek, s);
  liczbaNaDate(koniec, k);
  fprintf(stream, "dostępne w terminie %s - %s\n-----------------------\n", s, k);
  for(int i = 1; i <= currdevices; i++)
    if(Urzadzenia[i].czyAktywny && (Urzadzenia[i].typ == typ || typ == 0) && czyDostepneWDanymOkresie(Urzadzenia, i, poczatek, koniec)){
      fprintf(stream, "ID: %d\nNazwa: %s\n------------------------\n", Urzadzenia[i].id, Urzadzenia[i].nazwa);
      czyWypisano = true;
    }
  if(!czyWypisano)
    fprintf(stream, "Brak dostępnych urządzeń.\n------------------------\n");
  if(fil == 'Y')
    fclose(stream);
  return;  
}

void drukujPlanWypozyczen(Urzadzenie *Urzadzenia)
{
  printf("Podaj datę w formacie DD/MM z tygodnia, który chcesz wypisać.\n");
  int day, month;
  char nl;
  scanf("%d%c%d%c", &day,&nl, &month, &nl);
  int poczatek = wyznaczPoczatekTygodnia(day, month);
  if(poczatek == -1){
    printf("Błędna data.\n");
    return;
  }
  char fil;
  FILE *stream;
  printf("Czy chcesz zapisać wyniki wyszukiwania do pliku? Wpisz Y/N.\n");
  scanf("%c%c", &fil, &nl);
  if(fil == 'Y')
    stream = fopen("wydruk_zapytania_dostepne.txt", "a+");
  else
    stream = stdout;
  char s[20], k[20];
  int koniec = MIN(poczatek + 167, 5255);
  liczbaNaDate(MAX(poczatek, 0), s);
  liczbaNaDate(koniec, k);
  fprintf(stream, "\n\n\n---------------------------------------------\nPlan wypożyczeń w tygodniu %s - %s\n---------------------------------------------\n", s, k);
  char dzien[20];
  for(int i = 0; i <= 6; i++){
    if(poczatek + 24*i < 0 || poczatek + 24*i > 5255)
      continue;
    switch(i)
    {
      case 0:
        strcpy(dzien, "Poniedziałek");
        break;
      case 1:
        strcpy(dzien, "Wtorek");
        break;
      case 2:
        strcpy(dzien, "Środa");
        break;
      case 3:
        strcpy(dzien, "Czwartek");
        break;
      case 4:
        strcpy(dzien, "Piątek");
        break;
      case 5:
        strcpy(dzien, "Sobota");
        break;
      case 6:
        strcpy(dzien, "Niedziela");
        break;
      default:
        break;
    }
    fprintf(stream, "-----%s-----\n", dzien);
    for(int j = i*24; j < (i+1)*24; j++){
      fprintf(stream, "%02d:00 ", j%24);
      for(int k = 1; k <= currdevices; k++)
        if(Urzadzenia[k].czyAktywny && Urzadzenia[k].rents[poczatek + j] != 0)
          fprintf(stream, " %d - %s;", Urzadzenia[k].rents[poczatek + j], Urzadzenia[k].nazwa);
      fprintf(stream, "\n");
    }      
  }
  fprintf(stream, "-----------------------------------------------------------------------------------------------------------------------------\n\n\n\n");
  if(fil == 'Y')
    fclose(stream);
  return;
}

void wypozyczeniaOsoby(Wypozyczenie *Wypozyczenia, Urzadzenie *Urzadzenia)
{
  FILE *stream;
  printf("Podaj imię i nazwisko osoby, której wypożyczenia chcesz wypisać. Program przyjmie maksymalnie 80 znaków.\n");
  char dane[100];
  wczytajLinie(dane, 80);
  char fil, nl;
  printf("Czy chcesz zapisać wyniki wyszukiwania do pliku? Wpisz Y/N.\n");
  scanf("%c%c", &fil, &nl);
  if(fil == 'Y')
    stream = fopen("wydruk_zapytania_dostepne.txt", "a+");
  else
    stream = stdout;
  bool czyWypisano = false;
  fprintf(stream, "\n\n\n------------------------------------------\nWypożyczenia wpisane pod nazwiskiem %s\n--------------\n", dane);
  char s[20], k[20], sem[5];
  for(int i = 1; i <= currents; i++)
    if(Wypozyczenia[i].czyAktywne && strcmp(Wypozyczenia[i].osoba, dane) == 0){
      liczbaNaDate(Wypozyczenia[i].poczatek, s);
      liczbaNaDate(Wypozyczenia[i].koniec, k);
      czyWypisano = true;
      if(Wypozyczenia[i].czySemestralne)
        strcpy(sem, "Tak");
      else
        strcpy(sem, "Nie");      
      fprintf(stream, "ID: %d\nID urządzenia: %d\nNazwa urządzenia: %s\nOkres wypożyczenia: %s - %s\nSemestralne: %s\n--------------\n", Wypozyczenia[i].id, Wypozyczenia[i].idurzadzenia, Urzadzenia[Wypozyczenia[i].idurzadzenia].nazwa, s, k, sem);
    }
  if(!czyWypisano)
    fprintf(stream, "Brak wyników\n--------------\n");
  fprintf(stream, "------------------------------------------\n");
  if(fil == 'Y')
    fclose(stream);
  return;
}