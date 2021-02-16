//Mateusz Gieroba (322072)
#include "urzadzenia.h"

int maxdevices = 100, currdevices = 0;

bool wczytajLinie(char *s, int n)
{
  int cnt = 0;
  char c;
  bool przekroczenie = false;
  while((c = getchar()) != '\n'){
    cnt++;
    if(cnt > n){
      przekroczenie = true;
      break;
    }
    s[cnt - 1] = c;
  }
  if(przekroczenie){
    while((c = getchar()) != '\n');
    return false;
  }
  s[cnt] = '\0';
  return true;
}

Urzadzenie *wczytajUrzadzenia()
{
  FILE *plik;
  plik = fopen("urzadzenia.txt","r");
  if(plik != NULL){
    //odczytanie liczby rekordów
    int ilezapisanych = 0;
    fscanf(plik, "%d\n", &ilezapisanych);
    maxdevices += ilezapisanych;
    currdevices = ilezapisanych;
  }
  Urzadzenie *Urzadzenia = calloc(maxdevices + 1, sizeof(Urzadzenie));
  if(Urzadzenia == NULL){
    printf("Program nie mógł zaalokować pamięci i zakończył się błędem.\n");
    return NULL;
  }
  if(plik == NULL)
    return Urzadzenia;
  int cnt = 1;
  char s[300];
  while(true){
    fgets(s, 300, plik);
    if(feof(plik))
      break;
    int nr = 0, koniec = 0;
    Urzadzenia[cnt].id = atoi(s);
    while(s[nr] != ';'){
      s[nr] = ' ';
      nr++;
    }
    s[nr] = ' ';
    nr++;
    Urzadzenia[cnt].typ = atoi(s);
    while(s[nr] != ';')
      nr++;
    nr++;
    koniec = nr;
    while(s[koniec] != ';')
      koniec++;
    if(koniec != nr) strncpy(Urzadzenia[cnt].nazwa, s + nr, koniec - nr);
    else Urzadzenia[cnt].nazwa[0] = '\0';
    nr = ++koniec;
    while(s[koniec] != ';')
      koniec++;
    if(koniec != nr) strncpy(Urzadzenia[cnt].opis, s + nr, koniec - nr);
    else Urzadzenia[cnt].nazwa[0] = '\0';
    nr = ++koniec;
    Urzadzenia[cnt].czyAktywny = (bool)(s[nr] - '0');
    //printf("%d %d %s %s\n", Urzadzenia[cnt].id, Urzadzenia[cnt].typ, Urzadzenia[cnt].nazwa , Urzadzenia[cnt].opis);
    cnt++;
  }
  fclose(plik);
  return Urzadzenia;
}

Urzadzenie *rozszerzUrzadzenia(Urzadzenie *Urzadzenia)
{
  maxdevices += 100;
  Urzadzenie *noweUrzadzenia = calloc(maxdevices, sizeof(Urzadzenie));
  for(int i = 1; i <= currdevices; i++){
    noweUrzadzenia[i].id = Urzadzenia[i].id;
    noweUrzadzenia[i].typ = Urzadzenia[i].typ;
    noweUrzadzenia[i].czyAktywny = Urzadzenia[i].czyAktywny;
    for(int j = 0; j <= 5255; j++)
      noweUrzadzenia[i].rents[j] = Urzadzenia[i].rents[j];
    strcpy(noweUrzadzenia[i].nazwa, Urzadzenia[i].nazwa);
    strcpy(noweUrzadzenia[i].opis, Urzadzenia[i].opis);
  }
  free(Urzadzenia);
  return noweUrzadzenia;
}

bool zapiszUrzadzenia(Urzadzenie *Urzadzenia)
{
  FILE *plik;
  plik = fopen("urzadzenia.txt", "w");
  if(plik == NULL){
    printf("Błąd otwarcia bazy urządzeń.\n");
    return false;
  }
  fprintf(plik, "%d\n", currdevices);
  for(int i = 1; i <= currdevices; i++)
    fprintf(plik, "%d;%d;%s;%s;%d;\n", Urzadzenia[i].id, Urzadzenia[i].typ, Urzadzenia[i].nazwa, Urzadzenia[i].opis, Urzadzenia[i].czyAktywny);
  free(Urzadzenia);
  fclose(plik);
  return true;
}

Urzadzenie *dodajUrzadzenie(Urzadzenie *Urzadzenia)
{
  if(currdevices == maxdevices)
    Urzadzenia = rozszerzUrzadzenia(Urzadzenia);
  char nl, inp[210];
  currdevices++;
  printf("Podaj typ urządzenia. 1 - komputer, 2 - projektor, 3 - ekran, 4 - tablet graficzny, 5 - system nagłośnieniowy.\n");
  while(scanf("%d%c", &Urzadzenia[currdevices].typ, &nl) == 0 || Urzadzenia[currdevices].typ > 5 || Urzadzenia[currdevices].typ < 1){
    wczytajLinie(inp, 0);
    printf("Podano błędny typ urządzenia! Podaj ponownie poprawny numer typu.\n");
  }
  Urzadzenia[currdevices].id = currdevices;
  Urzadzenia[currdevices].czyAktywny = true;
  printf("Podaj nazwę urządzenia. Jej długość nie może przekroczyć 30 znaków.\n");
  while(!wczytajLinie(inp, 30))
    printf("Przekroczono limit znaków. Podaj ponownie nazwę o długości co najwyżej 30 znaków!\n");
  strcpy(Urzadzenia[currdevices].nazwa, inp);
  printf("Podaj opis urządzenia. Jego długość nie może przekroczyć 200 znaków.\n");
  while(!wczytajLinie(inp, 200))
    printf("Przekroczono limit znaków. Podaj ponownie opis o długości co najwyżej 200 znaków!\n");
  strcpy(Urzadzenia[currdevices].opis, inp);
  printf("Urządzenie zostało zapisane w systemie pod ID: %d\n", currdevices);
  return Urzadzenia;
}

void wypiszUrzadzenia(Urzadzenie *Urzadzenia, int typ, bool czyDoPliku) //typ = -1 - dostęp administracyjny drukujący wszystkie wprowadzone kiedykolwiek rekordy
{
  FILE *stream;
  if(czyDoPliku)
    stream = fopen("wydruk_urzadzen.txt", "a+");
  else
    stream = stdout;
  bool czyCosWypisano = false;
  fprintf(stream, "--------------------------------------\n");
  for(int i = 1; i <= currdevices; i++)
    if(((Urzadzenia[i].typ == typ || typ == 0) && Urzadzenia[i].czyAktywny == true) || typ == -1){
      char s[50];
      czyCosWypisano = true;
      switch(Urzadzenia[i].typ){
        case 1:
          strcpy(s, "komputer");
          break;
        case 2:
          strcpy(s, "projektor");
          break;
        case 3:
          strcpy(s, "ekran");
          break;
        case 4:
          strcpy(s, "tablet graficzny");
          break;
        case 5:
          strcpy(s, "system nagłośnieniowy");
          break;
        default:
          strcpy(s, "Błędny typ, sprawdź plik urzadzenia.txt");
          break;
      }
      fprintf(stream, "-------------------------\nID: %d\nTyp: %s\nNazwa: %s\nOpis: %s\n", Urzadzenia[i].id, s, Urzadzenia[i].nazwa, Urzadzenia[i].opis);
    }
  if(!czyCosWypisano)
    printf("W bazie nie ma urządzeń spełniających podane wymagania\n");
  fprintf(stream, "--------------------------------------\n\n\n\n");
  if(czyDoPliku)
    fclose(stream);
  return;
}

bool usunUrzadzenie(Urzadzenie *Urzadzenia, int id)
{
  if(Urzadzenia[id].czyAktywny == false || id > currdevices || id < 1){
    printf("Próbujesz usunąć nieistniejące urządzenie. Nieładnie!\n");
    return false;
  }
  printf("-------------------------\nID: %d\nTyp: %d\nNazwa: %s\nOpis: %s\n-------------------------\n", Urzadzenia[id].id, Urzadzenia[id].typ, Urzadzenia[id].nazwa, Urzadzenia[id].opis);
  char x, nl;
  printf("Czy na pewno chcesz usunąć to urządzenie? Jeżeli tak - wpisz Y/N\n");
  scanf("%c%c", &x, &nl);
  if(x == 'Y')
    Urzadzenia[id].czyAktywny = false;
  return true;
}

void modyfikujUrzadzenie(Urzadzenie *Urzadzenia, int id)
{
  if(id < 1 || id > currdevices || Urzadzenia[id].czyAktywny == false){
    printf("Próbujesz zmodyfikować nieistniejące urządzenie! Nieładnie.\n");
    return;
  }
  int czytyp = 0;
  char nl, inp[210];
  printf("Jeżeli chcesz zmienić typ urządzenia - wpisz nowy typ.\n1 - komputer, 2 - projektor, 3 - ekran, 4 - tablet graficzny, 5 - system nagłośnieniowy\nW przeciwnym przypadku wpisz 0.\n");
  while(scanf("%d%c", &czytyp, &nl) == 0 || czytyp > 5 || czytyp < 0)
    printf("Podano błędny typ urządzenia! Podaj ponownie poprawny numer typu.\n");
  if(czytyp != 0)
    Urzadzenia[id].typ = czytyp;
  printf("Jeżeli chcesz zmienić nazwę urządzenia, wpisz nową nazwę. Pamiętaj o ograniczeniu 30 znaków! W przeciwnym przypadku wciśnij jedynie enter.\n");
  while(!wczytajLinie(inp, 30))
    printf("Przekroczono limit znaków. Podaj ponownie nazwę o długości co najwyżej 30 znaków!\n");
  if(strlen(inp) > 0)
    strcpy(Urzadzenia[id].nazwa, inp);
  printf("Jeżeli chcesz zmienić opis urządzenia, wpisz nowy opis. Pamiętaj o ograniczeniu 200 znaków! W przeciwnym przypadku wciśnij jedynie enter.\n");
  while(!wczytajLinie(inp, 200))
    printf("Przekroczono limit znaków. Podaj ponownie opis o długości co najwyżej 200 znaków!\n");
  if(strlen(inp) > 0)
    strcpy(Urzadzenia[id].opis, inp);
  return;
}
