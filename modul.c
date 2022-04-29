#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "modul.h"

#define DL_LINII 1024       /* Dlugosc buforow pomocniczych */
#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4
#define HELP 1
#define BLEDNA_WARTOSC_PROGU 2
#define ZLE_PODANA_MASKA 3
#define NIEPOPRAWNY_KOLOR 4
#define MASKA_UJEMNA 5
#define B_CZERN 6
#define B_BIEL 7

/************************************************************************************
 * Funkcja wczytuje obraz PGM z pliku do tablicy       	       	       	       	    *
 *										    *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			    *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz		    *
 * \param[out] wymx szerokosc obrazka						    *
 * \param[out] wymy wysokosc obrazka						    *
 * \param[out] szarosci liczba odcieni szarosci					    *
 * \return liczba wczytanych pikseli						    *
 ************************************************************************************/

int czytaj(FILE *plik_we, t_obraz *nasz) 
{
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  
  /*Sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (plik_we==NULL)
    {
    	fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
      	return(0);
    }
  
  /* Sprawdzenie "numeru magicznego" - powinien byc P2 lub P3*/
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    {
      koniec=1;                              /* Nie udalo sie? Koniec danych! */
    }
  if ( (buf[0]!='P') || ((buf[1]!='2')&&(buf[1]!='3')) || koniec)
    {  /* Czy jest magiczne "P2" lub "P3"? */
      	fprintf(stderr,"Blad: To nie jest ani  plik PGM ani PPM\n");
      	return(0);
    }
  if (buf[1]=='2')
    {
      nasz->P = '2';
    }
  if (buf[1]=='3')
    {
      nasz->P = '3';
    }
  /* Pominiecie komentarzy */
  	do 
	  {
	    if ((znak=fgetc(plik_we))=='#')
	      {         /* Czy linia rozpoczyna sie od znaku '#'? */
		if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	  		{
			  koniec=1;                             /* Zapamietaj ewentualny koniec danych */
	  		}
	      }
    	else
	  {
	    ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
	  }                                         /* nie jest '#' zwroc go                 */
	  }while (znak=='#' && !koniec);             /* Powtarzaj dopoki sa linie komentarza i nie nastapil koniec danych         */
	
	
  	/* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  	if (fscanf(plik_we,"%d %d %d",&nasz->wymx, &nasz->wymy, &nasz->szarosci) != 3)
	  {
	    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
	    return(0);
	  }
	
	if (nasz->P == '2')
	{
	  if (nasz->piksele1 != NULL)                                     // free
	    {
	      for (int i = 0; i < nasz->wymy; i++)
		{
		  free(nasz->piksele1[i]);
		}
	      free(nasz->piksele1);
	      nasz->piksele1 = NULL;
	    }
	  
	  nasz->piksele1=(int**)malloc(sizeof(int*) * (nasz->wymy));      // alokacja
          for(int i = 0; i < nasz->wymy; i++)
	    {
	      nasz->piksele1[i]=(int*)malloc(sizeof(int) * (nasz->wymx));
	    }
	  nasz->obraz = nasz->piksele1;
	  
	  for (int i = 0; i < nasz->wymy; i++) 
		  {
		    for (int j = 0; j < nasz->wymx; j++) 
		      {
	    		if (fscanf(plik_we, "%d", &(nasz->obraz[i][j])) != 1)
			  {
			    fprintf(stderr, "Blad: Niewlasciwe wymiary obrazu\n");
			    return 0;
			  }
	  		}
		  }
	  return nasz->wymx * nasz->wymy;   /* Czytanie zakonczone sukcesem    */
	}

  	if (nasz->P == '3') //rgb                                         //free
	  {
	    if (nasz->piksele2 != NULL)
	      {
			for (int i = 0; i < nasz->wymx; i++)
			  { 
			    for (int j = 0; j < nasz->wymy; j++)
			      {	
    				free(nasz->piksele2[i][j]);	    					      }
			  }
			
			for (int i = 0; i < nasz->wymy; i++)
			  {
	  			free(nasz->piksele2[i]);
			  }
			free(nasz->piksele2);
			nasz->piksele2 = NULL;
	      }
	    
	    nasz->piksele2 = (int***)malloc(sizeof(int**)*(nasz->wymy));     //alokacja
	    for (int i = 0; i < nasz->wymy; i++)
	      {
		nasz->piksele2[i] = (int**)malloc(sizeof(int*) * (nasz->wymx));
		for(int j = 0; j < nasz->wymx; j++)
	    	{ 
		  nasz->piksele2[i][j] = (int*)malloc(sizeof(int) * 3);
	    	}
	      }
	    nasz->obraz2 = nasz->piksele2;
	    for (int i = 0; i < nasz->wymy; i++) 
		  {
		    for (int j = 0; j < nasz->wymx; j++) 
		      {
			for (int k = 0; k < 3; k++)
			  {
			    if (fscanf(plik_we,"%d",&(nasz->obraz2[i][j][k]))!=1)
			      {
				fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
				return(0);
					  }
			  }
		      }
		  }  
	    
	    return nasz->wymx * nasz->wymy * 3;
    }
}

int zapisz (FILE *plik_wy, t_obraz *nasz, char komentarz[])
{
  int i,j,k,g;
  /* sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (plik_wy==NULL)
    {
      fprintf(stderr, "Blad: Nie podano uchwytu do pliku\n");
      return(0);
    }
  /* napisanie magicznej liczby */
  if(nasz->P == '2')
    {
      fprintf(plik_wy,"P2\n");
    }
  if(nasz->P == '3')
    {
      fprintf(plik_wy,"P3\n");
    }
  
  /* napisanie komentarzy */
  time_t t;
  time(&t);
  fprintf(plik_wy, "# Godzina wykonania ponizszych akcji %s",ctime(&t));
  fprintf(plik_wy, "%s\n",komentarz);
  
  /* napisanie wymiarow i wartosci szarosci */
  fprintf(plik_wy,"%d %d %d\n",nasz->wymx, nasz->wymy, nasz->szarosci);
  
  /* wprowadzanie wartosci poszczegolnych pikseli */
  if (nasz->P == '2')
    {
      for (i=0; i < nasz->wymy; i++)
	{
	  for (j=0; j < nasz->wymx; j++)
	    {
	      g++;
	      fprintf(plik_wy,"%d", nasz->obraz[i][j]);
	      if (g == 70)
		{
		  fprintf(plik_wy,"\n");
		  g=0;
		}
	      else
		{
		  fprintf(plik_wy," ");
		}
	    }
        }
    }
  if (nasz->P == '3')
    {
      for (i=0; i < nasz->wymy; i++)
	{
	  for (j=0; j < nasz->wymx; j++)
	    {
	      for (k=0; k<3; k++)
		{
		  g++;
		  fprintf(plik_wy,"%d", nasz->obraz2[i][j][k]);
		  if (g == 70)
		    {
		      fprintf(plik_wy,"\n");
		      g=0;
		    }
	          else
		    {   
		      fprintf(plik_wy," ");
		    }
		}
	    }
        }
    }
  return 1;
}

int fnegatyw (t_obraz *nasz, w_opcje *opcje)
{
  int i,j;
  if (nasz->P == '3')
      {
	switch (opcje->kolor)
	  {
	  case 'r':
	    {
	      for (i=0; i<nasz->wymy; i++)
		{
		  for (j=0; j<nasz->wymx; j++)
		    {
		      nasz->obraz2[i][j][0] = nasz->szarosci - nasz->obraz2[i][j][0];
		    }
		}
	      break;
	    }
	  case 'g':
	    {
	      for (i=0; i<nasz->wymy; i++)
		{
		  for (j=0; j<nasz->wymx; j++)
		    {
		      nasz->obraz2[i][j][1] = nasz->szarosci - nasz->obraz2[i][j][1];
		    }
		}
	      break;
	    }
	  case 'b':
	    {
	      for (i=0; i<nasz->wymy; i++)
		{
		  for (j=0; j<nasz->wymx; j++)
		    {
		      nasz->obraz2[i][j][2] = nasz->szarosci - nasz->obraz2[i][j][2];
		    }
		}
	      break;
	    }
	  case 's':
	    {
	      fkonwersja(nasz, opcje);
	      break;
	    }
          default:
	    {
	      assert(opcje->kolor=='s');
	    }
	  }
      }
  if (nasz->P == '2')
	{
	  for (i=0; i<nasz->wymy; i++)
	    {
	      for (j=0; j<nasz->wymx; j++)
		{
		  nasz->obraz[i][j] = nasz->szarosci - nasz->obraz[i][j];
		}
	    }
	}
  return 1;
}

int fprogowanie (t_obraz *nasz, w_opcje *opcje)
{
  assert((opcje->w_progu>=0)&&(opcje->w_progu<=100));
  int i,j;
  int PROG=nasz->szarosci*opcje->w_progu/100;
  if (nasz->P == '3')
    {
	switch (opcje->kolor)
	  {
	  case 'r':
	    {
	      for (i=0; i<nasz->wymy; i++)
		{
		  for (j=0; j<nasz->wymx; j++)
		    {
		      if (nasz->obraz2[i][j][0] <= PROG)
			{
			  nasz->obraz2[i][j][0] = 0;
			}
		      else
			{
			  nasz->obraz2[i][j][0] = nasz->szarosci;
			}
		    }
		}
	      break;
	    }
	  case 'g':
	    {
	      for (i=0; i<nasz->wymy; i++)
		{
		  for (j=0; j<nasz->wymx; j++)
		    {
		      if (nasz->obraz2[i][j][1] <= PROG)
			{
			  nasz->obraz2[i][j][1] = 0;
			}
		      else
			{
			  nasz->obraz2[i][j][1] = nasz->szarosci;
			}
		    }
		}
	      break;
	    }
	  case 'b':
	    {
	      for (i=0; i<nasz->wymy; i++)
		{
		  for (j=0; j<nasz->wymx; j++)
		    {
		      if (nasz->obraz2[i][j][2] <= PROG)
			{
			  nasz->obraz2[i][j][2] = 0;
			}
		      else
			{
			  nasz->obraz2[i][j][2] = nasz->szarosci;
			}
		    }
		}
	      break;
	    }
	  case 's':
	    {
	      fkonwersja(nasz, opcje);
	      break;
	    }
          default:
	    {
	      assert((opcje->kolor == ('s')) || (opcje->kolor == ('g')) || (opcje->kolor == ('r')) || (opcje->kolor == ('b')));
	    }
	  }
      }
  if (nasz->P == '2')
    {
      for (i=0; i<nasz->wymy; i++)
	 {
	   for (j=0; j<nasz->wymx; j++)
	     {
	       if (nasz->obraz[i][j] <= PROG)
		 {
		   nasz->obraz[i][j] = 0;
		 }
	       else
		 {
		   nasz->obraz[i][j] = nasz->szarosci;
		 }
	     }
	 }
    }
  return 1;
}

int fkonturowanie (t_obraz *nasz, w_opcje *opcje)
{
  int i,j;
  if (nasz->P == '3')
    {
	switch (opcje->kolor)
	  {
	  case 'r':
	    {
	      for (i=0; i < nasz->wymy - 1; i++)
		{
		  for (j=0; j < nasz->wymx - 1; j++)
		    {
		      nasz->obraz2[i][j][0] = (abs(nasz->obraz2[i+1][j][0] - nasz->obraz2[i][j][0]) + abs(nasz->obraz2[i][j+1][0] - nasz->obraz2[i][j][0]))/2;
		    }
		}
	      break;
	    }
	  case 'g':
	    {
	      for (i=0; i < nasz->wymy - 1; i++)
		{
		  for (j=0; j<nasz-> wymx - 1; j++)
		    {
		      nasz->obraz2[i][j][1] = (abs(nasz->obraz2[i+1][j][1] - nasz->obraz2[i][j][1]) + abs(nasz->obraz2[i][j+1][1] - nasz->obraz2[i][j][1]))/2;
		    }
		}
	      break;
	    }
	  case 'b':
	    {
	      for (i=0; i < nasz->wymy - 1; i++)
		{
		  for (j=0; j < nasz->wymx - 1; j++)
		    {
		      nasz->obraz2[i][j][2] = (abs(nasz->obraz2[i+1][j][2] - nasz->obraz2[i][j][2]) + abs(nasz->obraz2[i][j+1][2] - nasz->obraz2[i][j][2]))/2;
		    }
		}
	      break;
	    }
          case 's':
	    {
	      fkonwersja(nasz, opcje);
	      break;
	    }
          default:
	    {
	      assert((opcje->kolor == ('s')) || (opcje->kolor == ('g')) || (opcje->kolor == ('r')) || (opcje->kolor == ('b')));
	    }
	  }
      }
  if (nasz->P == '2')
    {
	  for (i=0; i < nasz->wymy - 1; i++)
	    {
	      for (j=0; j < nasz->wymx - 1; j++)
		{
		  nasz->obraz[i][j]=(abs(nasz->obraz[i+1][j]-nasz->obraz[i][j]) + abs(nasz->obraz[i][j+1] - nasz->obraz[i][j]))/2;
		}
	    }
    }
  return 1;
}

int frozmycie (t_obraz *nasz, w_opcje *opcje)
{
  int i,j;
  if (nasz->P == '3')
    {
      switch(opcje->kolor)
	{
	case 's':
	  {
	    fkonwersja(nasz, opcje);
	    break;
	  }
	case 'r':
	  {
	    //kopia
	    int kopia[nasz->wymy][nasz->wymx];
	    for (i=0; i < nasz->wymy; i++)
	      {
		for (j=0; j < nasz->wymx; j++)
		  {
		    kopia[i][j] = nasz->obraz2[i][j][0];
		  }
	      }
	    
	    //obraz
	    for (i=0; i < nasz->wymy; i++)
	      {
		for (j=1; j < nasz->wymx-1; j++)
		  {
		    nasz->obraz2[i][j][0] = (kopia[i][j-1] + kopia[i][j] + kopia[i][j+1]) / 3;
		  }
		nasz->obraz2[i][0][0] = (kopia[i][0] + kopia[i][1]) / 2;
		nasz->obraz2[i][nasz->wymx-1][0] = (kopia[i][nasz->wymx-1] + kopia[i][nasz->wymx-2]) / 2;
	      }
	    
	    break;
	  }
	case 'g':
	  {
	    //kopia
	    int kopia[nasz->wymy][nasz->wymx];
	    for (i=0; i < nasz->wymy; i++)
	      {
		for (j=0; j < nasz->wymx; j++)
		  {
		    kopia[i][j] = nasz->obraz2[i][j][1];
		  }
	      }
	    
	    //obraz
	    for (i=0; i < nasz->wymy; i++)
	      {
		for (j=1; j < nasz->wymx-1; j++)
		  {
		    nasz->obraz2[i][j][1] = (kopia[i][j-1] + kopia[i][j] + kopia[i][j+1]) / 3;
		  }
		nasz->obraz2[i][0][1] = (kopia[i][0] + kopia[i][1]) / 2;
		nasz->obraz2[i][nasz->wymx-1][1] = (kopia[i][nasz->wymx-1] + kopia[i][nasz->wymx-2]) / 2;
	      }
	    
	    break;
	  }
	case 'b':
	  {
	     //kopia
	    int kopia[nasz->wymy][nasz->wymx];
	    for (i=0; i < nasz->wymy; i++)
	      {
		for (j=0; j < nasz->wymx; j++)
		  {
		    kopia[i][j] = nasz->obraz2[i][j][2];
		  }
	      }
	    
	    //obraz
	    for (i=0; i < nasz->wymy; i++)
	      {
		for (j=1; j < nasz->wymx-1; j++)
		  {
		    nasz->obraz2[i][j][2] = (kopia[i][j-1] + kopia[i][j] + kopia[i][j+1]) / 3;
		  }
		nasz->obraz2[i][0][2] = (kopia[i][0] + kopia[i][1]) / 2;
		nasz->obraz2[i][nasz->wymx-1][2] = (kopia[i][nasz->wymx-1] + kopia[i][nasz->wymx-2]) / 2;
	      }
	    break;
	  }
	default:
	  {
	    assert((opcje->kolor == ('s')) || (opcje->kolor == ('g')) || (opcje->kolor == ('r')) || (opcje->kolor == ('b')));
	  }
	}
    }
  if (nasz->P == '2')
    {
      //kopia
      int kopiaa[nasz->wymy][nasz->wymx];
      for (i=0; i < nasz->wymy; i++)
	{
	  for (j=0; j < nasz->wymx; j++)
	    {
	      kopiaa[i][j] = nasz->obraz[i][j];
	    }
	}
      //rozmycie
      for (i=0; i < nasz->wymy; i++)
	{
	  for (j=1; j < nasz->wymx-1; j++)
	    {
	      nasz->obraz[i][j] = (kopiaa[i][j-1] + kopiaa[i][j] + kopiaa[i][j+1]) / 3;
	    }
	  nasz->obraz[i][0] = (kopiaa[i][0] + kopiaa[i][1]) / 2;
	  nasz->obraz[i][nasz->wymx-1] = (kopiaa[i][nasz->wymx-1] + kopiaa[i][nasz->wymx-2]) / 2;
	}
    }
  return 1;
}

int ffiltrowanie (t_obraz *nasz, w_opcje *opcje)
{
  int i,j;
  int suma=0;
  int sumaw=0;

  //wole tak niz w calej fukcji poprawiac w na opcje->w (mniej pisania)
  int w[3][3];
  
  /* obliczenie sumy wspolczynnikow */ 
  for (i=0; i<3; i++)
    {
      for (j=0; j<3; j++)
	{
	  w[i][j] = opcje->w[i][j];
	  if (w[i][j] < 0)
	    {
	      assert(w[i][j]<0);
	    }
	  sumaw = sumaw + abs(w[i][j]);
	}
    }
  if (nasz->P == '3')
    {
	switch (opcje->kolor)
	  {
	  case 'r':
	    {
	      for (i=1; i<nasz->wymy-1; i++)       
		{
		  for (j=1; j<nasz->wymx-1; j++)  
		    {
		      suma = ((w[0][0])*(nasz->obraz2[i-1][j-1][0])) + ((w[0][1])*(nasz->obraz2[i-1][j][0])) + ((w[0][2])*(nasz->obraz2[i-1][j+1][0])) + ((w[1][0])*(nasz->obraz2[i][j-1][0])) + ((w[1][1])*(nasz->obraz2[i][j][0])) + ((w[1][2])*(nasz->obraz2[i][j+1][0])) + ((w[2][0])*(nasz->obraz2[i+1][j-1][0])) + ((w[2][1])*(nasz->obraz2[i+1][j][0])) + ((w[2][2])*(nasz->obraz2[i+1][j+1][0]));		 
		      nasz->obraz2[i][j][2] = suma/sumaw;
		    }
		}
	      break;
	    }
	  case 'g':
	    {
	      for (i=1; i<nasz->wymy-1; i++)       
		{
		  for (j=1; j<nasz->wymx-1; j++)  
		    {
		      suma = ((w[0][0])*(nasz->obraz2[i-1][j-1][1])) + ((w[0][1])*(nasz->obraz2[i-1][j][1])) + ((w[0][2])*(nasz->obraz2[i-1][j+1][1])) + ((w[1][0])*(nasz->obraz2[i][j-1][1])) + ((w[1][1])*(nasz->obraz2[i][j][1])) + ((w[1][2])*(nasz->obraz2[i][j+1][1])) + ((w[2][0])*(nasz->obraz2[i+1][j-1][1])) + ((w[2][1])*(nasz->obraz2[i+1][j][1])) + ((w[2][2])*(nasz->obraz2[i+1][j+1][1]));		 
		      nasz->obraz2[i][j][2] = suma/sumaw;
		    }
		}
	      break;
	    }
	  case 'b':
	    {
	      for (i=1; i<nasz->wymy-1; i++)       
		{
		  for (j=1; j<nasz->wymx-1; j++)  
		    {
		      suma = ((w[0][0])*(nasz->obraz2[i-1][j-1][2])) + ((w[0][1])*(nasz->obraz2[i-1][j][2])) + ((w[0][2])*(nasz->obraz2[i-1][j+1][2])) + ((w[1][0])*(nasz->obraz2[i][j-1][2])) + ((w[1][1])*(nasz->obraz2[i][j][2])) + ((w[1][2])*(nasz->obraz2[i][j+1][2])) + ((w[2][0])*(nasz->obraz2[i+1][j-1][2])) + ((w[2][1])*(nasz->obraz2[i+1][j][2])) + ((w[2][2])*(nasz->obraz2[i+1][j+1][2]));		 
		      nasz->obraz2[i][j][2] = suma/sumaw;
		    }
		}
	      break;
	    }
          case 's':
	    {
	      fkonwersja(nasz, opcje);
	      break;
	    }
          default:
	    {
	      assert((opcje->kolor == ('s')) || (opcje->kolor == ('g')) || (opcje->kolor == ('r')) || (opcje->kolor == ('b')));
	    }
	  }
    }
  if (nasz->P == '2')
    {
	   for (i=1; i<nasz->wymy-1; i++)       
	     {
	       for (j=1; j<nasz->wymx-1; j++)  
		 {
		   suma = ((w[0][0])*(nasz->obraz[i-1][j-1])) + ((w[0][1])*(nasz->obraz[i-1][j])) + ((w[0][2])*(nasz->obraz[i-1][j+1])) + ((w[1][0])*(nasz->obraz[i][j-1])) + ((w[1][1])*(nasz->obraz[i][j])) + ((w[1][2])*(nasz->obraz[i][j+1])) + ((w[2][0])*(nasz->obraz[i+1][j-1])) + ((w[2][1])*(nasz->obraz[i+1][j])) + ((w[2][2])*(nasz->obraz[i+1][j+1]));		 
		   nasz->obraz[i][j] = suma/sumaw;
		 }
	     }
	}
  return 1;
}

int fzmianapoziomow(t_obraz *nasz, w_opcje *opcje)
{
  assert(((opcje->biel>=0)&&(opcje->biel<=100))&&((opcje->czern>=0)&&(opcje->czern<=100)));
   int i,j;
   if (nasz->P == '3')
     {
	switch (opcje->kolor)
	  {
	  case 'r':
	    {
	      for (i=0; i<nasz->wymy; i++)
		{
		  for (j=0; j<nasz->wymx; j++)
		    {
		      if (nasz->obraz2[i][j][0] <= opcje->czern)
			{
			  nasz->obraz2[i][j][0] = 0;
			}
		      if ((opcje->czern < nasz->obraz2[i][j][0]) && (nasz->obraz2[i][j][0] < opcje->biel)) 
			{
			  nasz->obraz2[i][j][0] = ((nasz->obraz2[i][j][0] - opcje->czern) * (nasz->szarosci/(opcje->biel - opcje->czern)));
			}
		      if (nasz->obraz2[i][j][0] >= opcje->biel)
			{
			  nasz->obraz2[i][j][0] = nasz->szarosci;
			}
		    }
		}
	      break;
	    }
	  case 'g':
	    {
	      for (i=0; i<nasz->wymy; i++)
		{
		  for (j=0; j<nasz->wymx; j++)
		    {
		      if (nasz->obraz2[i][j][1] <= opcje->czern)
			{
			  nasz->obraz2[i][j][1] = 0;
			}
		      if ((opcje->czern < nasz->obraz2[i][j][1]) && (nasz->obraz2[i][j][1] < opcje->biel)) 
			{
			  nasz->obraz2[i][j][1] = ((nasz->obraz2[i][j][1] - opcje->czern) * (nasz->szarosci/(opcje->biel - opcje->czern)));
			}
		      if (nasz->obraz2[i][j][1] >= opcje->biel)
			{
			  nasz->obraz2[i][j][1] = nasz->szarosci;
			}
		    }
		}
	      break;
	    }
	  case 'b':
	    {
	      for (i=0; i<nasz->wymy; i++)
		{
		  for (j=0; j<nasz->wymx; j++)
		    {
		      if (nasz->obraz2[i][j][2] <= opcje->czern)
			{
			  nasz->obraz2[i][j][2] = 0;
			}
		      if ((opcje->czern < nasz->obraz2[i][j][2]) && (nasz->obraz2[i][j][2] < opcje->biel)) 
			{
			  nasz->obraz2[i][j][2] = ((nasz->obraz2[i][j][2] - opcje->czern) * (nasz->szarosci/(opcje->biel - opcje->czern)));
			}
		      if (nasz->obraz2[i][j][2] >= opcje->biel)
			{
			  nasz->obraz2[i][j][2] = nasz->szarosci;
			}
		    }
		}
	      break;
	    }
          case 's':
	    {
	      fkonwersja(nasz, opcje);
	      break;
	    }
          default:
	    {
	      assert((opcje->kolor == ('s')) || (opcje->kolor == ('g')) || (opcje->kolor == ('r')) || (opcje->kolor == ('b')));
	    }
	  }
     }
   if (nasz->P == '2')
     {
	  for (i=0; i<nasz->wymy; i++)
	    {
	      for (j=0; j<nasz->wymx; j++)
		{
		  if (nasz->obraz[i][j] <= opcje->czern)
		    {
		      nasz->obraz[i][j] = 0;
		    }
		  if ((opcje->czern < nasz->obraz[i][j]) && (nasz->obraz[i][j] < opcje->biel)) 
		    {
		      nasz->obraz[i][j] = ((nasz->obraz[i][j] - opcje->czern) * (nasz->szarosci/(opcje->biel - opcje->czern)));
		    }
		  if (nasz->obraz[i][j] >= opcje->biel)
		    {
		      nasz->obraz[i][j] = nasz->szarosci;
		    }
		}
	    }
     }
   return 1;
}

int fkonwersja (t_obraz *nasz, w_opcje *opcje)
{
  int i, j, k;
  if (nasz->P == '3')
    {
      // free P2
      if (nasz->piksele1 != NULL)
      {
	for (i = 0; i < nasz->wymy; i++)
	  {
	    free(nasz->piksele1[i]);
	  }
	free(nasz->piksele1);
	nasz->piksele1 = NULL;
      }
        
      // save P2
      nasz->piksele1=(int**)malloc(sizeof(int*) * (nasz->wymy));
      for(i=0; i < nasz->wymy; i++)
	{
	nasz->piksele1[i]=(int*)malloc(sizeof(int) * (nasz->wymx));
	}
      nasz->obraz = nasz->piksele1;
      
      // new obraz P2
      for (i=0; i < nasz->wymy; i++)
      {
	for (j=0; j < nasz->wymx; j++)
	  {
	   nasz->piksele1[i][j] = 0;
	   for (k=0; k<3; k++)
	     {
	       nasz->piksele1[i][j] += nasz->piksele2[i][j][k];
	     }
	   nasz->piksele1[i][j] /= 3;
	  }
      }
      nasz->P = '2';
      
      /* BUS ERROR ?
      // free P3
      if (nasz->piksele2 != NULL) //rgb
	{
	  for (i=0; i < nasz->wymx; i++)
	    { 
	      for (j=0; j < nasz->wymy; j++) 
		{
		  free(nasz->piksele2[i][j]);
		}
	    }
	  for (i=0; i < nasz->wymy; i++)
	    {
	      free(nasz->piksele2[i]);
	    }
          free(nasz->piksele2);
	  nasz->piksele2 = NULL;
	}
      */
    }
  else
    {
      assert(nasz->P != '3');
    }
  return 1;
}


/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */

void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */
  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
system(polecenie);             /* wykonanie polecenia        */
}



// OPCJE.C

/********************************************************************/
/*                                                                  */
/* ALTERNATYWNA DO PRZEDSTAWIONEJ NA WYKLADZIE WERSJA OPRACOWANIA   */
/* PARAMETROW WYWOLANIA PROGRAMU UWZGLEDNIAJACA OPCJE Z PARAMETRAMI */
/* Z ODPOWIEDNIO ZAPROPONOWANYMI STRUKTURAMI DANYCH PRZEKAZUJACYMI  */
/* WCZYTANE USTAWIENIA                                              */
/*                                    COPYRIGHT (c) 2007-2013 ZPCiR */
/*                                                                  */
/* Autorzy udzielaja kazdemu prawa do kopiowania tego programu      */
/* w calosci lub czesci i wykorzystania go w dowolnym celu, pod     */
/* warunkiem zacytowania zrodla                                     */
/*                                                                  */
/********************************************************************/

void wyzeruj_opcje(w_opcje * opcje)
{
  opcje->plik_we=NULL;
  opcje->plik_wy=NULL;
  opcje->negatyw=0;
  opcje->konturowanie=0;
  opcje->progowanie=0;
  opcje->zmianapoziomow=0;
  opcje->rozmycie=0;
  opcje->filtrowanie=0;
  opcje->w_progu=0;
  opcje->czern=0;
  opcje->biel=0;
  opcje->kolor='0';
  opcje->zapisanieobrazu=0;
  opcje->wyswietlenieobrazu=0;
  opcje->odczytanieobrazu=0;
  opcje->nazwapliku=NULL;
  opcje->tylko_wyswietlic=1;
}

int przetwarzaj_opcje(int argc, char **argv, w_opcje *opcje)
{
  int i, prog, czernn, biell, wartosc_w, odczytano;
  char kolorr;
  char *nazwa_pliku_we, *nazwa_pliku_wy;
  
  wyzeruj_opcje(opcje);
  opcje->plik_wy=stdout;                                   /* na wypadek gdy nie podano opcji "-o" */
  
  for (i=1; i<argc; i++)
    {
      if (argv[i][0] != '-')                               /* blad: to nie jest opcja - brak znaku "-" */
	{
	  return B_NIEPOPRAWNAOPCJA;
	}
      switch (argv[i][1])
	{
	case 'h':
	  {
	    return HELP;
          }
	case 'i':
	  {                                               /* opcja z nazwa pliku wejsciowego */
	    if (++i<argc)
	      {                                           /* wczytujemy kolejny argument jako nazwe pliku */
		nazwa_pliku_we=argv[i];
		if (strcmp(nazwa_pliku_we,"-")==0)        /* gdy nazwa jest "-"        */
		  {
		    opcje->plik_we=stdin;                 /* ustwiamy wejscie na stdin */
		    opcje->nazwapliku=nazwa_pliku_we;
		  }
		else                                        /* otwieramy wskazany plik   */
		  {
		    opcje->plik_we=fopen(nazwa_pliku_we,"r");
		    opcje->odczytanieobrazu=1;
		    opcje->nazwapliku=nazwa_pliku_we;
		  }
	      }
	    else
	      {
		return B_BRAKNAZWY;                       /* blad: brak nazwy pliku */
	      }
	    break;
	  }
	case 'o':
	  {                                                 /* opcja z nazwa pliku wyjsciowego */
	  if (++i<argc)
	    {                                             /* wczytujemy kolejny argument jako nazwe pliku */
	      nazwa_pliku_wy=argv[i];
	      if (strcmp(nazwa_pliku_wy,"-")==0)
	        {                                         /* gdy nazwa jest "-"         */
		  opcje->plik_wy=stdout;                  /* ustwiamy wyjscie na stdout */
		  opcje->zapisanieobrazu=1;
		  opcje->nazwapliku=nazwa_pliku_wy;
		}
	      else  
		{                                           /* otwieramy wskazany plik    */
		  opcje->plik_wy=fopen(nazwa_pliku_wy,"w");
		  opcje->zapisanieobrazu=1;
		  opcje->nazwapliku=nazwa_pliku_wy;
		}	
	    }
	  else
	    {
	      return B_BRAKNAZWY;                            /* blad: brak nazwy pliku */
	    }
	  break;
	  }
	case 'd':
	  {
	    opcje->wyswietlenieobrazu=1;
	    break;
	  }
	case 'p':
	  {
	    if (++i<argc)
	      {                                            /* wczytujemy kolejny argument jako wartosc progu */
		if (sscanf(argv[i],"%d",&prog)==1)
		  {
		    if ((prog >= 0) && (prog <= 100))
		      {
			opcje->progowanie=1;
			opcje->tylko_wyswietlic=0;
			opcje->w_progu=prog;
			break;
		      }
		    else
		      {
			return BLEDNA_WARTOSC_PROGU;
		      }
		  }
		else
		  {
		    return B_BRAKWARTOSCI;              /* blad: niepoprawna wartosc progu */
		  }
	      }
	    else
	      {
		return B_BRAKWARTOSCI;                        /* blad: brak wartosci progu */
	      } 
	    break;
	  }
	case 'n':
	  {                                                 /* mamy wykonac negatyw */
	    opcje->tylko_wyswietlic=0;
	    opcje->negatyw=1;
	    break;
	  }
	case 'k':
	  {                                                 /* mamy wykonac konturowanie */
	    opcje->konturowanie=1;
	    opcje->tylko_wyswietlic=0;
	    break;
	  }
	case 'r':
	  {                                                 /* mamy wykonac rozmycie */
	    opcje->rozmycie=1;
	    opcje->tylko_wyswietlic=0;
	    break;
	  }
	case 's':
	  {                                                 /* mamy wykonac filtrowanie */
	    int k=0;
	    int l=0;
	    int j=0;
	    for (j=0; j < 9; j++)
	      {
		if (++i<argc)
		  {
		    if (sscanf(argv[i], "%d", &wartosc_w) == 1)
		      {
			if (wartosc_w < 0)
			  {
			    return MASKA_UJEMNA;
			  }
			opcje->w[k][l] = wartosc_w;
			if (l != 2)
			  {
			    l++;
			  }
			else
			  {
			    l=0;
			    k++;
			  }		   
		      }
		    else
		      {
			return B_BRAKWARTOSCI;
		      }
		  }
		else
		  {
		    return B_BRAKWARTOSCI;
		  }
	      }
	    opcje->tylko_wyswietlic=0;
	    opcje->filtrowanie=1;
	    break;
	  }
	case 'm':
	  {
	    if (++i<argc)
	      {
		/* wczytujemy kolejny argument jako znak koloru */
		if (sscanf(argv[i],"%c",&kolorr)==1)
		  {
		    switch(kolorr)
		      {
		      case 's':
			{
			  opcje->kolor='s';
			  break;
			}
		      case 'r':
			{
			  opcje->kolor='r';
			  break;
			}
		      case 'g':
			{
			  opcje->kolor='g';
			  break;
			}
		      case 'b':
			{
			  opcje->kolor='b';
			  break;
			}
		      default:
			{
			  return NIEPOPRAWNY_KOLOR;
			}
		      }
		  }    		
		else 
		  {
		    return B_BRAKWARTOSCI;                      /* blad: niepoprawna wartosc */
		  }              
	      }
	    else
	      {
		return B_BRAKWARTOSCI;                            /* blad: brak wartosci  */
	      }
	    break;
	  }
	case 'z':
	  {                                                     /* zmiana poziomow  */
	    if (++i<argc)
	      {                                          
		if (sscanf(argv[i],"%d",&czernn)==1)
		  {
		    if ((czernn >= 0) && (czernn <= 100))
		      {
			opcje->czern=czernn;
		        if (++i<argc)
			   {		
		             if (sscanf(argv[i], "%d", &biell) == 1)
			       {
				 if ((biell >= 0) && (biell <= 100))
				   {
				     opcje->progowanie=1;
				     opcje->tylko_wyswietlic=0;
				     opcje->biel=biell;
				     break;
				   }
				 else
				   {
				     return B_BIEL;
				   }  
			       }
			     else
			       {
			         return B_BRAKWARTOSCI;
			       }
			   }   
			else
			  {
			    return B_BRAKWARTOSCI;
			  }
		      }
		    else
		      {
			return B_CZERN;
		      }
		  }
		else
		  {
		    return B_BRAKWARTOSCI;              
		  }
	      }
	    else
	      {
		return B_BRAKWARTOSCI;                     
	      } 
	  }
	default:                                                        
	  {
	    return B_NIEPOPRAWNAOPCJA;
	  }
	} /*koniec switch */
    } /* koniec for */
  if (opcje->plik_we!=NULL)
    {     /* ok: wej. strumien danych zainicjowany */
      return W_OK;
    }
  else
    {
      return B_BRAKPLIKU;
    }  /* blad:  nie otwarto pliku wejsciowego  */
}


// free
/*
if ((nasz->P == '2') && (nasz->piksele1 != NULL)) //P2
  {
    for (i=0; i<nasz->wymy; i++)
      {
	free(nasz->piksele1[i]);
	}
    free(nasz->piksele1);
  }

if ((nasz->P == '3') &&(nasz->piksele2 != NULL)) //rgb
  {
    for (i=0; i < nasz->wymx; i++)
      { 
    for (j=0; j < nasz->wymy; j++) 
      {
	free(nasz->piksele2[i][j]);
      }
      }
    
    for (int i = 0; i < nasz->wymy; i++)
      {
	free(nasz->piksele2[i]);
      }
    free(nasz->piksele2);
  }
*/


  
