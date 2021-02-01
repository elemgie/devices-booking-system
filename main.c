#include "urzadzenia.h"
int main()
{
  Urzadzenie *Urzadzenia = wczytajUrzadzenia();
  if(Urzadzenia == NULL){
    printf("Błąd wczytywania bazy. Program zakończył się. Sprawdź plik urzadzenia.txt\n");
    return 1;
  }
  char command = 'P', nl, aux;
  int aux1;
  while(command != 'Q'){
    printf("Podaj kod czynności, którą chcesz wykonać.\nA - dodanie urządzenia, D - usunięcie urządzenia, L - wypisanie listy urządzeń, M - modyfikacja urządzenia,\nQ - zakończenie programu.\n");
    scanf("%c%c", &command, &nl);
    if(command == 'A')
      Urzadzenia = dodajUrzadzenie(Urzadzenia);
    if(command == 'D'){
      printf("Podaj ID usuwanego urządzenia.\n");
      scanf("%d%c", &aux1, &nl);
      usunUrzadzenie(Urzadzenia, aux1);
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
    if(command == 'Q'){
      if(!zapiszUrzadzenia(Urzadzenia))
      {
        printf("Doszło do błędu zapisu. Wykonaj ręczne kopie plików urzadzenia.txt oraz wypozyczenia.txt celem zapobiegnięcia utracie danych.\n");
        printf("Kiedy skończysz wciśnij ENTER, program się zakończy.\n");
        scanf("%c", &nl);
      }
      else
        printf("Program zakończył się poprawnie. Baza została zapisana do plików.\n");
    }
  }
  return 0;
}