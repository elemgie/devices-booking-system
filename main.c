//Mateusz Gieroba (322072)
#include "urzadzenia.h"
#include "wypozyczenia.h"
#include "zapytania.h"
int main()
{
  Urzadzenie *Urzadzenia = wczytajUrzadzenia();
  Wypozyczenie *Wypozyczenia = wczytajWypozyczenia(Urzadzenia);
  if(Urzadzenia == NULL || Wypozyczenia == NULL){
    printf("Błąd wczytywania bazy. Program zakończył się. Sprawdź pliki urzadzenia.txt oraz wypozyczenia.txt\n");
    return 1;
  }
  char command = 'P', nl, aux, czysciciel[10];
  int aux1;
  while(command != 'Q'){
    printf("\nPodaj kod czynności, którą chcesz wykonać.\nI - wypisuje dostępne polecenia.\n");
    scanf("%c%c", &command, &nl);
    if(nl != '\n'){
      printf("Błąd wczytywania komendy.\n");
      wczytajLinie(czysciciel, 0);
      command = 'P';
      nl = '\n';
    }
    if(command == 'I')
      printf("A - dodanie urządzenia, B - dodanie wypożyczenia, D - usunięcie urządzenia, I - wypisanie dostępnych poleceń\nL - wypisanie listy urządzeń, M - modyfikacja urządzenia, N - wypisanie jakiegoś wypożyczenia\nR - skasowanie wypożyczenia, Q - zakończenie programu, W - urządzenia dostępne w zadanym terminie\nX - drukuj tygodniowy plan wypożyczeń, Y - sprawdź wypożyczenia danej osoby.\n");
    if(command == 'A')
      Urzadzenia = dodajUrzadzenie(Urzadzenia);
    if(command == 'B')
      Wypozyczenia = dodajWypozyczenie(Wypozyczenia, Urzadzenia);
    if(command == 'D'){
      printf("Podaj ID usuwanego urządzenia.\n");
      scanf("%d%c", &aux1, &nl);
      if(usunUrzadzenie(Urzadzenia, aux1))
        usunRezerwacjeSkasowanegoUrzadzenia(aux1, Wypozyczenia, Urzadzenia);
    }
    if(command == 'L'){
      printf("Jeżeli chcesz wypisać urządzenia określonego typu - wpisz jego numer. W przeciwnym wypadku - wpisz -1.\n");
      printf("1 - komputer, 2 - projektor, 3 - ekran, 4 - tablet graficzny, 5 - system nagłośnieniowy\n");
      while(scanf("%d%c", &aux1, &nl) == 0 || aux1 < -1 || aux1 > 5)
        printf("Podałeś błędny numer typu. Podaj poprawny!\n");
      printf("Jeżeli chcesz wypisać urządzenia do pliku - wpisz Y. W przeciwnym przypadku wpisz N\n");
      scanf("%c%c", &aux, &nl);
      if(aux == 'Y')
        wypiszUrzadzenia(Urzadzenia, aux1, true);
      else
        wypiszUrzadzenia(Urzadzenia, aux1, false);
    }
    if(command == 'M'){
      printf("Podaj ID urządzenia, które chcesz zmodyfikować.\n");
      scanf("%d%c", &aux1, &nl);
      modyfikujUrzadzenie(Urzadzenia, aux1);
    }
    if(command == 'N'){
      printf("Jeżeli chcesz wypisać konkretne wypożyczenie - wpisz jego ID.\nW przeciwnym wypadku - wpisz 0 - wypisane zostaną wszystkie aktywne wypożyczenia.\n");
      while(scanf("%d%c", &aux1, &nl) == 0 || aux1 < -1 || nl != '\n')
        printf("Niepoprawne żądanie. Podaj ID bądź odpowiedni kod!\n");
      printf("Jeżeli chcesz wypisać do pliku - wpisz Y. W przeciwnym przypadku wpisz N\n");
      scanf("%c%c", &aux, &nl);
      if(aux == 'Y')
        wypiszWypozyczenia(Wypozyczenia, aux1, true);
      else
        wypiszWypozyczenia(Wypozyczenia, aux1, false);
    }
    if(command == 'Q'){
      if(!zapiszUrzadzenia(Urzadzenia) || !zapiszWypozyczenia(Wypozyczenia))
      {
        printf("Doszło do błędu zapisu. Wykonaj ręczne kopie plików urzadzenia.txt oraz wypozyczenia.txt celem zapobiegnięcia utracie danych.\n");
        printf("Kiedy skończysz wciśnij ENTER, program się zakończy.\n");
        scanf("%c", &nl);
      }
      else
        printf("Program zakończył się poprawnie. Baza została zapisana do plików.\n");
    }
    if(command == 'R'){
      printf("Podaj ID wypożyczenia, które chcesz skasować.\n");
      scanf("%d%c", &aux1, &nl);
      usunWypozyczenie(Wypozyczenia, aux1, Urzadzenia);
    }
    if(command == 'W')
      dostepnoscUrzadzen(Urzadzenia);
    if(command == 'X')
      drukujPlanWypozyczen(Urzadzenia);
    if(command == 'Y')
      wypozyczeniaOsoby(Wypozyczenia, Urzadzenia);
  }
  return 0;
}
