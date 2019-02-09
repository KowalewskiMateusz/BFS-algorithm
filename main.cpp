
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
using namespace std;


struct lista
{
    int wierzcholek;          // numer wierzcholka
    struct lista * nastepna; // nastepny element listy
};

class kolejka
{
    lista *poczatek;          //poczatek kolejki
    lista *koniec;            //koniec kolejki
public:
    kolejka()
    {
        poczatek = koniec = NULL;            //konstruktor tworzenia pustej kolejki
    }
    bool czy_pusta()
    {
        return poczatek;                 //sprawdza czy kolejka jest pusta
    }
    int pierwszy()
    {
        if(poczatek) return poczatek->wierzcholek; //zwaraca numer pierwszego wierzcholka w kolejce
    }
    void dodaj(int wierzcholek)                          //dodaje do kolejki
    {
        lista * a = new lista;                   //tworzenie nowego elementu
        a->nastepna = NULL;                      //ustawienie wskaznika na nastepny element na NULL
        a->wierzcholek = wierzcholek ;            //ustawienie w nowym elemencie wierzcholka, ktory dodajemy
        if(koniec) koniec->nastepna = a;          //jezeli cos jest w kolejce to ustawia element jako nastepny, a jezeli nie ma to ustawia poczatek i koniec na tym samym elemencie
        else     poczatek = a;
       koniec = a;
    }
    void usun()
    {
        if(poczatek)                      //o ile cos jest w kolejece
        {
            lista * a = poczatek;            //wskaznik na poczatek kolejki
            poczatek = poczatek->nastepna;          //poczatek przestawia na nastepny element
            if(!poczatek) koniec = NULL;           //jak nie ma poczatku to nie ma też konca więc koniec trzeba też przestawic na null
            delete a;                              // usuwamy ten element z poczatku, ktory pobralismy
        }
    }
    ~kolejka()
    {
        while(poczatek)usun();                 //destruktor, przy niszczeniu obiektu usuwa wszystkie elementy i zwalnia pamięć
    }
};
void przeszukiwanie_wszerz(lista** lista_s, int wierzcholki, int start,int stop);
void losuj(int mapa[][20]);
int liczba_wierzcholkow(int mapa[40][20]);
void lista_sasiedztwa(int mapa[40][20], lista **lista_s,int wierzcholki);
void start_stop(int &start,int &stop, int wierzcholki);
int main()
{

    int wierzcholki,start,stop;      //definicje zmiennych
    lista **lista_s;
    lista  *p,*r;

    srand(time(NULL));
    int mapa[40][20];             //mapa
    losuj(mapa);
    wierzcholki=liczba_wierzcholkow(mapa);
    lista_s = new lista * [wierzcholki];        //tworzenie listy sąsiadow
    for(int i = 0; i < wierzcholki; i++) lista_s[i] = NULL;   //zerowanie wszyskich pol
    lista_sasiedztwa(mapa,lista_s,wierzcholki);
    start_stop(start,stop,wierzcholki);
    cout<<"Wierzcholek startowy to "<<start<<endl;
    cout<<"Wierzcholek koncowy to "<<stop<<endl;
    for(int i=0; i<40; i++)
    {
        for(int j=0; j<20; j++)
            cout<<setw (8)<<mapa[i][j];
        cout<<endl;
    }
    przeszukiwanie_wszerz(lista_s,wierzcholki,start,stop);
    for(int i = 0; i < wierzcholki; i++)          //usuwanie listy sasaidow przy konczeniu programu
  {
    p = lista_s[i];

    while(p)
    {
      r = p;
      p = p->nastepna;
      delete r;
    }
  }
}


void przeszukiwanie_wszerz(lista** lista_s, int wierzcholki, int start,int stop)
{
    kolejka kol;
    bool * odwiedzono, znaleziono=false;
    int  * a,b,c,d;
    lista * lista1;

    odwiedzono = new bool[wierzcholki];   // Tworzymy tablice odwiedzin
    for(int i = 0; i < wierzcholki; i++)   // Tablice odwiedzin zerujemy
        odwiedzono[i] = false;

    a = new int[wierzcholki];          // Tworzymy tablice sciezki

    a[start] = -1;

    kol.dodaj(start);              // W kolejce umieszczamy wierzcholek startowy

    odwiedzono[start] = true;// i oznaczamy go jako startowy
    while(kol.czy_pusta())
    {
        b = kol.pierwszy();         // Pobieramy z kolejki wierzcholek b
        kol.usun();

        if(b == stop)            // Sprawdzamy koniec sciezki
        {
            znaleziono = true;        // Zaznaczamy sukces
            break;               // Przerywamy petle
        }

        // Przegladamy sasiadow wierzcholka v

        for(lista1 = lista_s[b]; lista1; lista1 = lista1->nastepna)             //lista dla wybranego wierzcholka z kolejki
        {
            c = lista1->wierzcholek;
            if(!odwiedzono[c])
            {
                a[c] = b;             // W a zapisujemy fragment sciezki
                kol.dodaj(c);         // sasiad zostaje umieszczony w kolejce
                odwiedzono[c] = true; // i oznaczony jako odwiedzony
            }
        }
    }
    int tab[200];
    int i=0;
    if(!znaleziono) cout << "Taka sciezka nie istnieje";// sciezka nie została znaleziona (w tym programie nigdy się to nie zdarzy)
    else
    {
        cout<<endl;
        cout<<"Droga robota biegnie przez nastepujace pola: "<<endl;
        while(b >=0)
        {
            i++;
            tab[i]=b;// Wypisujemy wierzchołki ścieżki
            b = a[b];            // Cofamy się do poprzedniego wierzchołka ścieżki
        }
        for(int j=i;j>0;j--)
        {
            cout<<setw(4)<<tab[j];
        }
    }
    delete [] a;
    delete [] odwiedzono;

}


void losuj(int mapa[][20])
{

    int A[5][5]= {{-1,-1,0,-1,-1},{-1,0,0,-1,-1},{0,0,-1,0,0},{-1,0,0,0,-1},{-1,-1,0,-1,-1}};   //inicjalizacje i definicje segmentow
    int B[5][5]= {{-1,-1,0,0,-1},{0,-1,-1,0,-1},{0,0,0,0,0},{-1,0,0,0,-1},{-1,-1,0,0,-1}};
    int C[5][5]= {{0,0,0,0,0},{0,0,-1,0,0},{0,0,-1,-1,0},{-1,0,-1,-1,-1},{-1,0,0,0,0}};
    int D[5][5]= {{-1,-1,0,0,0},{-1,0,0,-1,0},{0,0,-1,0,0},{0,-1,0,0,-1},{-1,0,0,-1,-1}};
    int E[5][5]= {{0,0,0,-1,-1},{-1,0,0,0,-1},{0,0,-1,0,0},{0,0,-1,0,0},{0,0,0,0,-1}};
    int F[5][5]= {{-1,-1,0,-1,-1},{-1,-1,0,-1,-1},{0,0,0,0,0},{-1,-1,0,-1,-1},{-1,-1,0,-1,-1}};
    char los;
    int r=0,l=0;
    for(int i=0; i<32; i++)                              //losowanie jaki segment gdzie i dodawanie
    {
        los=rand()%6+65;
        if(los=='A')
        {
            for(int j=0; j<5; j++)
            {
                for(int k=0; k<5; k++)
                {
                    mapa[j+r][k+l]=A[j][k];
                }

            }

        }
        if(los=='B')
        {
            for(int j=0; j<5; j++)
            {
                for(int k=0; k<5; k++)
                {
                    mapa[j+r][k+l]=B[j][k];
                }

            }

        }
        if(los=='C')
        {
            for(int j=0; j<5; j++)
            {
                for(int k=0; k<5; k++)
                {
                    mapa[j+r][k+l]=C[j][k];
                }

            }

        }
        if(los=='D')
        {
            for(int j=0; j<5; j++)
            {
                for(int k=0; k<5; k++)
                {
                    mapa[j+r][k+l]=D[j][k];
                }

            }

        }
        if(los=='E')
        {
            for(int j=0; j<5; j++)
            {
                for(int k=0; k<5; k++)
                {
                    mapa[j+r][k+l]=E[j][k];
                }

            }

        }
        if(los=='F')
        {
            for(int j=0; j<5; j++)
            {
                for(int k=0; k<5; k++)
                {
                    mapa[j+r][k+l]=F[j][k];
                }

            }
        }
        l=l+5;                             //licznik ktory przeskakuje na kolejne piątki kolumn i wierszy (kazdy segment jest 5x5)
        if(l==20)
        {
            r=r+5;
            l=0;
        }

    }

}


int liczba_wierzcholkow(int mapa[][20])
{
    int d=0,ilosc=0;                         //zlicza ilosc wierzcholkow i kazdemu z nich nadaje cyfry
    for(int i=0; i<40; i++)
    {
        for(int j=0; j<20; j++)
        {
            if(mapa[i][j]==0)
            {
                ilosc++;
                mapa[i][j]=d;
                d++;
            }
        }
    }
    return ilosc;

}


void start_stop(int &start,int &stop, int wierzcholki)
{
    start=rand()%wierzcholki;
    stop=rand()%wierzcholki;

}


void lista_sasiedztwa(int mapa[][20], lista **lista_s, int wierzcholki)
{
    lista *a,*b;
    int g;
    for(int i=0; i<40; i++)
    {
        for(int j=0; j<20; j++)
        {

            if(i!=39)
            {

                if(mapa[i+1][j]>=0)
                {
                    a = new lista;    // nowy element listy
                    a->wierzcholek=mapa[i+1][j] ;  //ma wczesniej ustalony numer
                    g =mapa[i][j];
                    a->nastepna = lista_s[g]; // Dodajemy go na poczatek listy
                    lista_s[g] = a;                // i ten element staje sie poczatkiem

                }
            }

            if(j!=19)
            {
                if(mapa[i][j+1]>=0)
                {
                    a = new lista;
                    a->wierzcholek =mapa[i][j+1] ;
                    g =mapa[i][j];
                    a->nastepna = lista_s[g];
                    lista_s[g] = a;

                }
            }
            if(i!=0)
            {
                if(mapa[i-1][j]>=0)
                {

                    a = new lista;
                    a->wierzcholek =mapa[i-1][j] ;
                    g =mapa[i][j];
                    a->nastepna = lista_s[g];
                    lista_s[g] = a;

                }
            }
            if(j!=0)
            {
                if(mapa[i][j-1]>=0)
                {

                    a = new lista;
                    a->wierzcholek =mapa[i][j-1] ;
                    g =mapa[i][j];
                    a->nastepna = lista_s[g];
                    lista_s[g] = a;

                }
            }
        }

    }
    cout<<endl;

    for(int i = 0; i <wierzcholki; i++)              //wypisywanie listy sasiedztwa (Dla sprawdzenia poprawności listy)
    {

        cout << "Lista["<< i<< "] =";
        a = lista_s[i];
        while(a)
        {
            cout << setw(5) << a->wierzcholek; //wypisujemy wierzcholek
            a = a->nastepna;                   //a przestawiamy na nastepny element
        }
        cout << endl;
    }


}


