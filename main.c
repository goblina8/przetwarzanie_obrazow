#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "modul.h"          // dodanie modulu

//#define MAX 1024            /* Maksymalny rozmiar wczytywanego obrazu zwiekszony by moc wczytywac wieksze obrazy */
#define DL_LINII 1024       /* Dlugosc buforow pomocniczych */

int main(int argc, char ** argv /* OPCJE.C */)
{
  /* DEKLARACJA ZMIENNYCH */
  
  char komentarz[1000];
  int odczytano=0;
  FILE *plik_tym;
  int warunek, b=0;
  w_opcje opcje;
  t_obraz nasz;
  (&nasz)->piksele1 = NULL;
  (&nasz)->piksele2 = NULL;
  (&nasz)->obraz = NULL;
  (&nasz)->obraz2 = NULL;	
 
    // START PROGRAMU   
    // OPCJE.C
  system("clear");
  wyzeruj_opcje(&opcje);
  warunek=przetwarzaj_opcje(argc, argv, &opcje);
  switch (warunek)
    {
    case -1:
      {
	printf("Wybrano niepoprawna opcje\n");
	break;
      }
    case -2:
      {
	printf("Brak podanej nazwy\n");
	break;
      }
      case -3:
	{
	  printf("Brak podanej wartosci\n");
	  break;
	}
    case -4:
      {
	printf("Brak pliku\n");
	break;
      }
    case 1: //help
      {
        printf("-i <nazwa_pliku> nazwa pliku do wczytania (jeśli zamiast nazwy podany będzie znak '-' to zamiast z pliku obraz powinien być wczytywany ze standardowego wejścia\n");
        printf("-o <nazwa_pliku> nazwa pliku do zapisu (jeśli zamiast nazwy podany będzie znak '-' to zamiast do pliku obraz powinien być wypisany na standardowym wyjściu\n");
        printf("-d obraz po przetworzeniu powinien zostać wyświetlony\n");
        printf("-m <kolor> wybierz kolor do przetwarzania: r – czerwony, g – zielony, b – niebieski, s – konwersja do szarości\n");
        printf("-n negatyw;\n");
        printf("-p <próg> progowanie z progiem o zadanej wartości procentowej\n");
        printf("-z <czern> <biel> zmiana poziomów\n");
        printf("-k konturowanie\n");
        printf("-s s12 s12 s13 s21 s22 s23 s31 s32 s33 splot\n");
	printf("-r rozmycie\n");
	printf("podanie koloru podczas przetwarzania obrazu szarego (P2) bedzie ignorowane\n");
	break;
      }
    case 2:
      {
	printf("Bledna wartosc progu\n");
	break;
      }
    case 3:
      {
	printf("Blednie podana maska\n");
	break;
      }
    case 4:
      {
	printf("Blednie podany kolor przetwarzania\n");
	break;
      }
    case 5:
      {
	printf("Wartosci maski musza byc dodatnie\n");
	break;
      }
    case 6:
      {
	printf("Blednie podana wartosc czerni\n");
	break;
      }
    case 7:
      {
	printf("Blednie podana wartosc bieli\n");
	break;
      }
    case 0:
      {
	if ((&opcje)->odczytanieobrazu == 0)
	  {
	    printf("Blad wczytania pliku\n");
	    return 0;
	  }
	else
	  {
	    printf("Wszystko dziala poprawnie\n");
	    odczytano = czytaj((&opcje)->plik_we, &nasz);
	    fclose((&opcje)->plik_we);
	    if (odczytano != 0)
	      {
		if((((&nasz)->P == '3') && ((&opcje)->kolor == '0')) && ((&opcje)->tylko_wyswietlic == 0))
		  {
		    printf("Brak podanego koloru do przetwarzania obrazu w P3\n");
		    return 0;
		    }
		printf("poprawnie odczytano\n");
		if ((&opcje)->negatyw==1)
		  {
		    printf("negatyw\n");
		    fnegatyw(&nasz, &opcje);
		    strcat(komentarz,"# negatyw zostal wykonany\n");
		    b++;
		  }
		if ((&opcje)->konturowanie==1)
		  {
		    printf("konturowanie\n");
		    fkonturowanie(&nasz, &opcje);
		    b++;
		    strcat(komentarz,"# konturowanie zostalo wykonane\n");
		  }
		if ((&opcje)->progowanie==1)
		  {
		    printf("progowanie\n");
		    fprogowanie(&nasz, &opcje);
		    b++;
		    strcat(komentarz,"# progowanie zostalo wykonane\n");
		  }
		if ((&opcje)->zmianapoziomow==1)
		  {
		    printf("zmianapoziomow\n");
		    fzmianapoziomow(&nasz, &opcje);
		    b++;
		    strcat(komentarz,"# zmiana poziomow zostala wykonana\n");
		  }
		if ((&opcje)->rozmycie==1)
		  {
		    printf("rozmycie\n");
		    frozmycie(&nasz, &opcje);
		    b++;
		    strcat(komentarz,"# rozmycie zostalo wykonane\n");
		  }
		if ((&opcje)->filtrowanie==1)
		  {
		    printf("filtrowanie\n");
		    ffiltrowanie(&nasz, &opcje);
		    b++;
		    strcat(komentarz,"# filtrowanie zostalo wykonane\n");
		  }
		if((&opcje)->zapisanieobrazu==0)
		//Brak opcji -o oznacza wypisanie obrazu na standardowym wyjściu. 
		  {
		    (&opcje)->plik_wy=stdout;          
		    (&opcje)->zapisanieobrazu=1;
		  }
		if((&opcje)->zapisanieobrazu==1)
		  {
		    printf("zapisanie\n");
		    zapisz((&opcje)->plik_wy, &nasz, komentarz);
		    fclose((&opcje)->plik_wy);
		  }
		if ((&opcje)->wyswietlenieobrazu == 1)
		  {
		    printf("wyswietlenie\n");
		    if (b == 0)
		      { 
			wyswietl((&opcje)->nazwapliku);
		      }
		    else
		      {
			plik_tym=fopen("tymczasowy","w");
			zapisz(plik_tym, &nasz, komentarz);
			fclose(plik_tym);
			wyswietl("tymczasowy");
			printf("Uspienie\n");
			//program usuwa plik tymczasowy zanim obraz sie wyswietli, a ze obraz otwiera sie przez 13 sekund - uspilam program - inaczej nie chcial dzialac
			system("sleep 13");
			system("rm tymczasowy");
		      }
		  }
		wyzeruj_opcje(&opcje);
		break;
	      } //if odczytano
	  } //else
      } // case 0 
    } //switch
} //main

/*
SPRAWOZDANIE - PRZETWARZANIE OBRAZOW 2 (lub - jak lubie mowic zdrobniale - LENA 2) 
GABRIELA KACZMAREK (259234)  28.12.2020R

Napisany przeze mnie program sluzy do obrobek zdjec w formacie ppm i pgm (P2 i P3). Zdjecia kolorowe sa przetwarzane za pomoca jednego, wybranego przez uzytkowanika koloru lub 
 w szarosciach (wtedy obraz jest konwertowany na pgm, gdzie pikselem jest srednia z kolorow odpowiadajacych za piksel w kolorowym obrazku). 
Dla szarych obrazow program dziala analogicznie jak w przypadku PRZETWARZANIA OBRAZOW 1. Jednakze wprowadzilam modyfikacje zwiazane z menu uzytkownika - teraz, by wlaczyc funkcje
 nalezy podczas wlaczania programu wybrac co ten ma zrobic. Calosc dziala zgodnie z opisem ze strony kursu. Dodatkowo zaopatrzylam program w opcje -h, ktora wypisuje swego rodzaju
 samouczek do programu. Przykladwe wywolanie programu, by przeksztalcic obrazek Lena.ppm w kolorze czerwonym i zmienic go na negatyw to:
./Lena2 -i Lena.ppm -m r -n
Procz zmiany funkcjonowania obslugi programu - zmienilam tez statyczne tablice na te alokowane dynamicznie. 

Testy programu opisane ponizej zostaly wykonane kolejno dla 2 roznych obrazow (jednego czarno-bialego, drugiego w kolorze). Trzeci obraz sluzyl jedynie do sprawdzenia, jak zachowa
 sie program, gdy "dostanie" plik o niewlasciwym formacie.

1) Claudia.pgm 
2) Lena2.ppm
3) kubus.gif

Test 1 wygladal nastepujaco:

       PLIK WCZYTANY                                           OCZEKIWANE REZULTATY
         kubus.gif                                   informacja, ze plik jest w zlym formacie
nazwa.nieistniejacego.pliku.ppm                             informacja o braku pliku

W pierwszej kolejnosci nalezy sprawdzic poprawnosc dzialania pojedynczych opcji:

1)  czy plik sie poprawnie wczytuje, wtedy kiedy powinien (-i nazwa, magiczna liczba) - aby to zweryfikowac nalezy wlaczyc program dla kilku odpowiednich obrazkow. 

2) czy plik zapisuje sie lub zostaje wypisany na standardowym wyjsciu w zaleznosci od wybranej opcji (-o nazwa_do_zapisania, -o -, brak -o [w 2 ostatnich przypadkach program
 wypisuje plik na standardowym wejsciu]) i tu ponownie powinna nastapic proba zapisania obrazkow pod nowa nazwa.

3) analogicznie nalezy zweryfikowac dzialanie kazdej opcji, zarowno dla obrazu w szarosci, jak i kolorowego (z uwzglednieniem kolejno kazej opcji koloru (-m {s,r,g,b} - z pomoca
 zewnetrznego programu porownac czy to co wyswietla nasz program jest oczekiwanym rezultatem, ktorego mozemy sie spodziewac na podstawie dzialania innego programu. Ten etap 
testowania jest dosc czasochlonny. W pierwszej kolejnosci podajemy tylko poprawne wartosci (np 0<prog<100,-m b, -s (podanych 9 dodatkich cyfr) itd...). Na weryfikacje asercji
 takze przyjdzie pora :).
 
4)  Gdy wszystko dziala poprawnie nalezy sprawdzic czy program wykona z obrazkiem poprawne dzialania, kiedy chcemu nalozyc kilka filtrow w tym celu trzeba przetestowac to w roznych
    konfiguracjach a nastepnie na biezaco wyswietlac obraz by weryfikowac poprawnosc dzialania programu.

5)  Z opcjami wczytywania i zapisu tez nalezy sie "pobawic", na zmiane nadpisujac plik, zmieniajac go, wczytujac od nowa, zmieniajac nazwe, wystwietlajac, a pozniej zweryfikowac
    w folderze, czy wszytsko dziala jak nalezy (czy plik tymczasowy na ktorym dzialamy jest taki sam jak koncowo zapisany? Czy plik tymczasowy jest usuwany? itd...). 

6)  Kolejnym etapem jest weryfikacja, czy program poprawnie zapisuje w pliku - nazwe i date przeprowadzonych zmian. Aby to sprawdzic, nalezy przesledzic dzialania prowadzone
    na obrazie, a nastepnie zweryfikowac posiadane dane, z tymi, ktore program wpisal do obrazow. Oczekiwanym wynikiem jest to, ze jedne i drugie dane beda sie pokrywac.

7)  Oczywistym jest, ze aby dzilanie programu bylo poprawne - kazda opcja dostepna w menu uzytkownika musi spelniac swoje zadanie. dlatego kazdej z nich nalezy uzyc choc raz i
    obserwowac czy program reaguje zgodnie z oczekiwaniami.

8)  Nalezy tez sprawdzic, czy program odpowiednio reaguje na bledna obsluge przez uzytkownika. Czy przerywa dzialanie, gdy podaje mu sie za mala ilosc cyfr splotu? Czy przerywa
 dzialanie, gdy prbujemy uzyc nieistniejacej opcji? Czy na kolorowych obrazach dziala tylko wtedy, kiedy wybierzemy kolor przetwarzania? Czyweryfikuje wartosci bieli, czerni
 i progu, aby nalezaly do (0,100)? Czy -i wymaga podania nazwy pliku, a -o odpowiednio reguje na - lub czy program wypisuje plik na standardowym wyjsciu, kiedy opcji -o calkowicie
 brakuje? Czy kolejnosc wpisania opcji nie wplywa na dzialanie programu? Aby zweryfikowac te wszytskie rzeczy - nalezy wielokrotnie wlaczyc program dla danych obrazow i po prostu
 "lopatologicznie" sprawdzac jego zachowanie w kazdej sytuacji.

W rezultacie program kompiluje sie i dziala poprawnie, tj. zgodnie z moimi oczekiwaniami. 

Przyznam, ze nie do konca zrozumialam, o co chodzilo w poleceniu z:
     "-i <nazwa_pliku> nazwa pliku do wczytania (jeśli zamiast nazwy podany będzie znak '-' to zamiast z pliku obraz powinien być wczytywany ze standardowego wejścia"
dlatego tez ominelam ten problem (-i -).


Jesli chodzi o moje wnioski - walka z Lena2 okazala sie dla mnie bardzo czasochlonna i wyczerpujaca. Do tego stopnia, ze calkiem zrezygnowalam z chocby proby podjecia sie
napisania zadania dodatkowego. "Zabawa" z Przetwarzaniem obrazow 2 okazala sie byc dla mnie na tyle nieprzyjemna, ze przyznam ze w trakcie pracy z programem targaly mna bardzo 
mocne i negatywne emocje. Jako ze mam w naturze emocje przelewac na papier - powstala Oda do Leny2. A skoro juz i tak powstala, zdecydowalam sie zalaczyc ja do sprawozdania. 
Domyslam sie, ze dla Pana praca z analizowaniem naszych programow takze nie jest ani latwa ani przyjemna, wiec moze owa Oda pozwoli Panu na chwile ochloniecia, tak samo jak
pozwolila na to mnie.

                                  Oda do Leny2 
					
			Leno, patrzysz na mnie tak zalotnie
			Jakbyś wcale nie mknęła ochotnie
			Po segmentacyjnych błędów szynach...
			Dość twych gierek! Skupmy sie na czynach!
			
			Ja ciągiem znaków z mej klawiatury
			Przerywam te asercjowe bzdury
			Dynamicznie trzeba zapisywać!
			Chciałaś mną do Sylwestra pogrywać?
			
			Na cześć wykładowcy w filtry Cię ubieram,
			Jak wpisze i- to się nie pozbieram.
			Robię na szaro, albo w kolorze 			
			Już nie przeszkodzisz więcej w wyborze!
			
			Ty na wartości oczekiwane
			Szaty masz teraz konturowane.
			Ja odpowiednio dobiorę progi 
			i cyk - oto rozmyte czerwone nogi!
					
			Dnie spędzałam w ciebie wpatrzona,
			W noce śniłaś mi się skończona...
			Oh, zero-jedynkowa piękności!
			Dałaś mi całkiem nieźle w me kości...
			   
 */

