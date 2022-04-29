#ifndef NAZWA_H
#define NAZWA_H
#define MAX 1024

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct
{
  FILE *plik_we, *plik_wy;        /* uchwyty do pliku wej. i wyj. */
  int negatyw, konturowanie, progowanie, wyswietlenieobrazu,odczytanieobrazu;      /* opcje */
  int zmianapoziomow, rozmycie, filtrowanie, zapisanieobrazu;
  int w_progu;               /* wartosc progu dla opcji progowanie */
  int czern;
  int biel;
  char kolor; 
  int w[3][3];
  char *nazwapliku;
  int tylko_wyswietlic;
} w_opcje;


typedef struct 
{
  FILE *plik;
  int **obraz;
  int ***obraz2;
  int wymx;
  int wymy;
  int szarosci;
  int ***piksele2;
  int **piksele1;
  char P; //czy P3 czy P1
} t_obraz;


int czytaj(FILE* plik, t_obraz *nasz);
int zapisz(FILE* plik, t_obraz *nasz, char komentarz[]);
int fnegatyw(t_obraz *nasz, w_opcje *opcje); 
int fkonturowanie(t_obraz *nasz, w_opcje *opcje);
int frozmycie(t_obraz *nasz, w_opcje *opcje);
int fprogowanie(t_obraz *nasz, w_opcje *opcje);
int ffiltrowanie(t_obraz *nasz, w_opcje *opcje);
int fzmianapoziomow(t_obraz *nasz, w_opcje *opcje);
int fkonwersja (t_obraz *nasz, w_opcje * opcje);

int przetwarzaj_opcje(int argc, char** argv, w_opcje *opcje);        //OPCJE.C

void wyzeruj_opcje(w_opcje *opcje);            //OPCJE.C

void wyswietl(char* nazwa);

#endif

