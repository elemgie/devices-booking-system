#include "wypozyczenia.h"
#include "urzadzenia.h"

int currents = 0, maxrents = 100;

void usunRezerwacjeSkasowanegoUrzadzenia(int id, Wypozyczenie *Wypozyczenia, Urzadzenie *Urzadzenia)
{
  for(int i = 0; i < 4537; i++)
    if(Urzadzenia[id].rents[i] > 0)
      Wypozyczenia[Urzadzenia[id].rents[i]].czyAktywne = false;
}

void liczbaNaDate(int liczba, char *data)
{
  int day = 24, month = 2, hour = 0, nr = 0;
  if(liczba - nr >= 120){
     nr += 120;
    month = 3;
    day = 1;
  }
  while(((month == 3 || month == 5 || month == 7 || month == 8) && 31*24 <= liczba - nr) || ((month == 4 || month == 6 || month == 9) && liczba - nr >= 24*30))
  {
    if(month == 3 || month == 5 || month == 7 || month == 8)
      nr += 31*24;
    else
      nr += 30*24;
    month++;
    day = 1;
  }
  day += (liczba - nr)/24;
  hour = (liczba - nr)%24;
  sprintf(data, "%02d/0%d %02d:00", day, month, hour);
  return;  
}

Wypozyczenie *rozszerzWypozyczenia(Wypozyczenie *Wypozyczenia)
{
  maxrents += 100;
  Wypozyczenie *noweWypozyczenia = calloc(maxrents, sizeof(noweWypozyczenia));
  for(int i = 1; i <= currents; i++){
    noweWypozyczenia[i].id = Wypozyczenia[i].id;
    noweWypozyczenia[i].idurzadzenia = Wypozyczenia[i].idurzadzenia;
    noweWypozyczenia[i].czyAktywne = Wypozyczenia[i].czyAktywne;
    noweWypozyczenia[i].poczatek = Wypozyczenia[i].poczatek;
    noweWypozyczenia[i].koniec = Wypozyczenia[i].koniec;
    noweWypozyczenia[i].czySemestralne = Wypozyczenia[i].czySemestralne;
    strcpy(noweWypozyczenia[i].osoba, Wypozyczenia[i].osoba);
  }
  free(Wypozyczenia);
  return noweWypozyczenia;
}

bool czyDostepneWDanymOkresie(Urzadzenie *Urzadzenia, int id, int poczatek, int koniec)
{
  for(int i = poczatek; i < koniec; i++)
    if(Urzadzenia[id].rents[i] != 0)
      return false;
  return true;
}

void zajmijOkresDostepnosci(Urzadzenie *Urzadzenia, int id, int idwyp, int poczatek, int koniec)
{//0 - wolny w danym terminie, wywołanie tej funkcji z tym atrybutem kasuje wypożyczenie w danym interwale
  for(int i = poczatek; i < koniec; i++)
    Urzadzenia[id].rents[i] = idwyp;
}

Wypozyczenie *wczytajWypozyczenia(Urzadzenie *Urzadzenia)
{
  FILE *plik;
  plik = fopen("wypozyczenia.txt","r");
  if(plik != NULL){
    //odczytanie liczby rekordów
    int ilezapisanych = 0;
    fscanf(plik, "%d\n", &ilezapisanych);
    maxrents += ilezapisanych;
    currents = ilezapisanych;
  }
  Wypozyczenie *Wypozyczenia = calloc(maxrents + 1, sizeof(Wypozyczenia));
  if(Wypozyczenia == NULL){
    printf("Program nie mógł zaalokować pamięci i zakończył się błędem.\n");
    fclose(plik);
    return NULL;
  }
  if(plik == NULL)
    return Wypozyczenia;
  int cnt = 1;
  char s[300];
  while(true){
    fgets(s, 300, plik);
    if(feof(plik))
      break;
    int nr = 0, koniec = 0;
    Wypozyczenia[cnt].id = atoi(s);
    while(s[nr] != ';'){
      s[nr] = ' ';
      nr++;
    }
    s[nr] = ' ';
    nr++;
    Wypozyczenia[cnt].idurzadzenia = atoi(s);
    while(s[nr] != ';'){
      s[nr] = ' ';
      nr++;
    }
    s[nr] = ' ';
    nr++;
    Wypozyczenia[cnt].poczatek = atoi(s);
    while(s[nr] != ';'){
      s[nr] = ' ';
      nr++;
    }
    s[nr] = ' ';
    nr++;
    Wypozyczenia[cnt].koniec = atoi(s);
    while(s[nr] != ';'){
      s[nr] = ' ';
      nr++;
    }
    nr++;
    Wypozyczenia[cnt].czySemestralne = (bool)(s[nr] - '0');
    koniec = nr = nr + 2;
    while(s[koniec] != ';')
      koniec++;
    if(koniec != nr) strncpy(Wypozyczenia[cnt].osoba, s + nr, koniec - nr);
    else Wypozyczenia[cnt].osoba[0] = '\0';
    nr = koniec = koniec + 2;
    Wypozyczenia[cnt].czyAktywne = (bool)(s[nr] - '0');
    zajmijOkresDostepnosci(Urzadzenia, Wypozyczenia[cnt].idurzadzenia, Wypozyczenia[cnt].id, Wypozyczenia[cnt].poczatek, Wypozyczenia[cnt].koniec);
    cnt++;
  }
  fclose(plik);
  return Wypozyczenia;
}

int dataNaLiczbe(int day, int month, int hour)
{
  if(month < 2 || month > 9 || day < 1 || hour < 0 || hour > 23 || day > 31 || (month == 2 && day < 24))
    return -1;
  if((month == 4 || month == 6 || month == 9) && day > 30)
    return -1;
  if(month == 2 && day > 28)
    return -1;
  int mi, nr;
  nr = 0;
  mi = 3;
  if(month > 2)
    nr += 120;
  while(mi < month){
    if(mi == 3 || mi == 5 || mi == 7 || mi == 8)
      nr += 24*31;
    else
      nr += 24*30;
    mi++;
  }
  nr += (day - 1)*24;
  if(month == 2)
    nr = 0 + 24*(day - 24);
  nr += hour;
  return nr;
}


void wypiszWypozyczenia(Wypozyczenie *Wypozyczenia, int wid, bool czyDoPliku)//id: 0 - wypisanie wszystkich aktywnych, -1 - wypisanie wszystkich w dostępie administratora
{
  if(wid < -1 || wid > currents){
    printf("Błędne ID wypożyczenia.\n");
    return;
  }
  FILE *wypis;
  if(czyDoPliku)
    wypis = fopen("wydruk_wypozyczen.txt", "a+");
  else
    wypis = stdout;
  char s[100], k[100], sem[10];
  fprintf(wypis, "--------------------------------------\n");
  if(wid < 1){
    for(int i = 1; i <= currents; i++)
      if(wid == -1 || Wypozyczenia[i].czyAktywne == true){
        liczbaNaDate(Wypozyczenia[i].poczatek, s);
        liczbaNaDate(Wypozyczenia[i].koniec, k);
        if(Wypozyczenia[i].czySemestralne)
          strcpy(sem, "Tak");
        else
          strcpy(sem, "Nie");        
        fprintf(wypis, "-------------------------\nID: %d\nID urządzenia: %d\nWypożyczenie semestralne: %s\nOkres wypożyczenia: %s - %s\nWypożyczający: %s\n", Wypozyczenia[i].id, Wypozyczenia[i].idurzadzenia, sem, s, k, Wypozyczenia[i].osoba);
      }
  }
  else{
    liczbaNaDate(Wypozyczenia[wid].poczatek, s);
    liczbaNaDate(Wypozyczenia[wid].koniec, k);
    if(Wypozyczenia[wid].czySemestralne)
      strcpy(sem, "Tak");
    else
      strcpy(sem, "Nie");        
    fprintf(wypis, "-------------------------\nID: %d\nID urządzenia: %d\nWypożyczenie semestralne: %s\nOkres wypożyczenia: %s - %s\nWypożyczający: %s\n", Wypozyczenia[wid].id, Wypozyczenia[wid].idurzadzenia, sem, s, k, Wypozyczenia[wid].osoba);
  }
  fprintf(wypis, "--------------------------------------\n");
  if(czyDoPliku)
    fclose(wypis);
  return;
}

Wypozyczenie *dodajWypozyczenie(Wypozyczenie *Wypozyczenia, Urzadzenie *Urzadzenia)
{
  printf("Podaj ID urządzenia, które chcesz wypożyczyć.\n");
  int id, poczatekmonth, poczatekday, poczatekhour, koniecmonth, koniecday, koniechour;
  char nl, sem;
  scanf("%d%c", &id, &nl);
  if(id > currdevices || !Urzadzenia[id].czyAktywny){
    printf("Próbujesz wypożyczyć nieistniejące urządzenie!\n");
    return Wypozyczenia;
  }
  printf("Czy wypożyczenie ma być cykliczne? Wtedy podany interwał zostanie zablokowany w każdym tygodniu od podanej daty do końca letniego semestru. Wpisz Y/N\n");
  scanf("%c%c", &sem, &nl);
  printf("Podaj datę rozpoczęcia wypożyczenia w formacie DD/MM HH. D - dzień, M - miesiąc, H - godzina.\n");
  scanf("%d%c%d%c%d%c", &poczatekday, &nl, &poczatekmonth, &nl, &poczatekhour, &nl);
  int poczatek = dataNaLiczbe(poczatekday, poczatekmonth, poczatekhour);
  if(poczatek == -1){
    printf("Podana data jest błędna. System obsługuje zakres 24 luty 2021 - 30 września 2021, godziny od 0 do 23.\n");
    return Wypozyczenia;
  }
  printf("Podaj datę zakończenia wypożyczenia w formacie DD/MM HH. Oznacza ona, że o tej godzinie urządzenie nie będzie już w Twoim posiadaniu.\n");
  scanf("%d%c%d%c%d%c", &koniecday, &nl, &koniecmonth, &nl, &koniechour, &nl);
  int koniec = dataNaLiczbe(koniecday, koniecmonth, koniechour);
  if(koniec == -1){
    printf("Podana data jest błędna. System obsługuje zakres 24 luty 2021 - 30 września 2021, godziny od 0 do 23\n");
    return Wypozyczenia;
  } 
  if(koniec <= poczatek){
    printf("Okres wypożyczenia musi wynosić co najmniej godzinę!\n");
    return Wypozyczenia;
  }
  if(!czyDostepneWDanymOkresie(Urzadzenia, id, poczatek, koniec)){
    printf("Wybrane urządzenie nie jest dostępne w zadanym okresie. Proszę sprawdzić swoje żądanie z dostępnością urządzeń.\n");
    return Wypozyczenia;
  }
  printf("Proszę podać imię i nazwisko wypożyczającego. Dane mogą mieć maksymalnie 80 znaków.\n");
  char inp[100];
  char s[100], k[100];
  liczbaNaDate(poczatek, s);
  liczbaNaDate(koniec, k);
  while(!wczytajLinie(inp, 80))
    printf("Przekroczono limit znaków. Podaj ponownie dane o długości co najwyżej 80 znaków!\n");
  strcpy(Wypozyczenia[++currents].osoba, inp);
  Wypozyczenia[currents].id = currents;
  Wypozyczenia[currents].idurzadzenia = id;
  if(sem == 'Y')
    Wypozyczenia[currents].czySemestralne = true;
  Wypozyczenia[currents].czyAktywne = true;
  Wypozyczenia[currents].poczatek = poczatek;
  Wypozyczenia[currents].koniec = koniec;
  zajmijOkresDostepnosci(Urzadzenia, id, currents, poczatek, koniec);
  printf("Zarezerwowano termin %s - %s.\n", s, k);
  while(sem == 'Y' && poczatek + 168 <= 4536 && koniec + 168 <= 4536){
    poczatek += 168;
    koniec += 168;
    liczbaNaDate(poczatek, s);
    liczbaNaDate(koniec, k);
    if(czyDostepneWDanymOkresie(Urzadzenia, id, poczatek, koniec)){
      zajmijOkresDostepnosci(Urzadzenia, id, currents, poczatek, koniec);
      printf("Zarezerwowano termin %s - %s.\n", s, k);
    }
    else
      printf("Nie zarezerwowano terminu %s - %s, gdyż w jego trakcie urządzenie jest niedostępne.\n", s, k);
  }
  printf("Twoje wypożyczenie znajduje się w systemie pod ID: %d.\n", currents);
  return Wypozyczenia;
}

bool zapiszWypozyczenia(Wypozyczenie *Wypozyczenia)
{
  FILE *plik;
  plik = fopen("wypozyczenia.txt", "w");
  if(plik == NULL){
    printf("Błąd otwarcia bazy urządzeń.\n");
    return false;
  }
  fprintf(plik, "%d\n", currents);
  for(int i = 1; i <= currents; i++)
    fprintf(plik, "%d;%d;%d;%d;%d;%s;%d;\n", Wypozyczenia[i].id, Wypozyczenia[i].idurzadzenia, Wypozyczenia[i].poczatek, Wypozyczenia[i].koniec, Wypozyczenia[i].czySemestralne, Wypozyczenia[i].osoba, Wypozyczenia[i].czyAktywne);
  free(Wypozyczenia);
  fclose(plik);
  return true;
}

void usunWypozyczenie(Wypozyczenie *Wypozyczenia, int id, Urzadzenie *Urzadzenia)
{
  if(id < 0 || id > currents || !Wypozyczenia[id].czyAktywne){
    printf("Niepoprawne ID istniejącego wypożyczenia.\n");
    return;
  }
  wypiszWypozyczenia(Wypozyczenia, id, false);
  printf("Czy na pewno chcesz usunąć to wypożyczenie? Wpisz Y bądź N.\n");
  char nl, aux;
  scanf("%c%c", &aux, &nl);
  if(aux != 'Y'){
    printf("Kasowanie przerwano.\n");
    return;
  }
  Wypozyczenia[id].czyAktywne = false;
  int pocz = Wypozyczenia[id].poczatek, kon = Wypozyczenia[id].koniec;
  for(int i = pocz; i < kon; i++)
    Urzadzenia[Wypozyczenia[id].idurzadzenia].rents[i] = 0;
  while(Wypozyczenia[id].czySemestralne && pocz < 4550 && kon < 4550){
    pocz += 168;
    kon += 168;
    if(Urzadzenia[Wypozyczenia[id].idurzadzenia].rents[pocz] == id)
      for(int i = pocz; i < kon; i++)
        Urzadzenia[Wypozyczenia[id].idurzadzenia].rents[i] = 0;
  }
  return;
}