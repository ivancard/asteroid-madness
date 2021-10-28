#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <list>
using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void gotoxy(int x, int y)
{
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD dwPos;

    dwPos.X = x;
    dwPos.Y = y;

    SetConsoleCursorPosition(hCon, dwPos);
}

void ocultarCursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 2
;
    cci.bVisible = FALSE;

    SetConsoleCursorInfo(hCon, &cci);

}

void pintar_limites(){

    for (int i = 2;i <119 ; i++ ){
        gotoxy(i,1); printf("%c",205);
        gotoxy(i,28); printf("%c",205);
    }

    for (int i=2;i<28 ;i++ ){
        gotoxy(2,i); printf("%c", 186);
        gotoxy(118,i); printf("%c", 186);
    }

    gotoxy(2,1); printf("%c", 201);
    gotoxy(2,28); printf("%c", 200);
    gotoxy(118,1); printf("%c", 187);
    gotoxy(118,28); printf("%c", 188);

}

class Nave{
    private:
        int x,y;
        int corazones ;
        int vidas ;

    public:
        Nave(int _x, int _y, int _corazones,int _vidas);
        int X() {return x;}
        int Y() {return y;}
        int getVidas(){return vidas;}
        void cor(){corazones--;}
        void pintar();
        void borrar();
        void mover();
        void pintarCorazones();
        void morir();
};

void Nave::pintar(){
    gotoxy(x, y);
    printf("  %c", 30);
    gotoxy(x, y+1);
    printf(" %c%c%c", 40, 207, 41);
    gotoxy(x, y+2);
    printf("%c%c %c%c", 30, 190, 190, 30);
}

void Nave::borrar(){
    gotoxy(x, y);  printf("        ");
    gotoxy(x, y+1);printf("        ");
    gotoxy(x, y+2);printf("        ");
}

void Nave::mover(){



    if (kbhit()){  //function kbhit returns a boolean if a key was pressed.

            char tecla = getch(); //function returns the key pressed.
            borrar();

            if ( (tecla == 'a' || tecla==IZQUIERDA) && x>3) x--;
            if ( (tecla == 's' || tecla==DERECHA) && x+6 < 118) x++;
            if ( (tecla == 'w' || tecla==ARRIBA) && y > 2) y--;
            if ( (tecla == 's' || tecla==ABAJO) && y+3 < 28 ) y++;

            pintar();
            pintarCorazones();
        }
}

Nave::Nave(int _x, int _y, int _corazones, int _vidas){

    x= _x;
    y= _y;
    corazones = _corazones;
    vidas = _vidas;

}

void Nave::pintarCorazones(){
    gotoxy(70, 0); printf("VIDAS %d", vidas);
    gotoxy(84,0); printf("Salud");
    gotoxy(90,0); printf("     ");
    for (int i = 0; i <corazones ; i++ ){
       gotoxy(90+i,0) ;printf("%c", 3);
    }
}

void Nave::morir(){
    if (corazones==0){

        borrar();
        gotoxy(x,y);   printf("   **   ");
        gotoxy(x,y+1); printf("  ****  ");
        gotoxy(x,y+2); printf("   **   ");
        Sleep(200);
        borrar();

        gotoxy(x,y);   printf(" * ** * ");
        gotoxy(x,y+1); printf("  ****  ");
        gotoxy(x,y+2); printf(" * ** * ");
        Sleep(200);
        borrar();

        vidas--;
        corazones = 3;
        pintarCorazones();
        pintar();
    }
}

class AST{
    private:
        int x,y;
    public:
        AST(int _x,int _y) : x(_x), y(_y){}
        void pintar();
        void mover();
        void choque(class Nave &N);
        int X() {return x;}
        int Y() {return y;}
};

void AST::pintar(){
    gotoxy(x,y); printf("%c",184);
}

void AST::mover(){
    gotoxy(x,y); printf(" ");
    y++;
    if (y>27){
        x=rand()%113+4;
        y=2;
    }
    pintar();

}

void AST::choque(class Nave &N){
    if ( x >= N.X() && x < N.X()+6 && y >= N.Y() && y+2 <= N.Y()+2 ){
        N.cor();
        N.borrar();
        N.pintar();
        N.pintarCorazones();
        x=rand()%113+4;
        y=2;
    }

}

class Bala {
private:
    int x, y;

public:
    Bala(int _x, int _y):x(_x), y(_y){}
    void mover();
    bool fuera();
    int X() {return x;}
    int Y() {return y;}

};

void Bala::mover(){
    gotoxy(x,y); printf(" ");
    y--;
    gotoxy(x,y); printf("*");
}

bool Bala::fuera(){
    if (y == 2) return true;
    return false;
}
int main(){

    ocultarCursor();
    pintar_limites();
    int x=58, y=20; //Starter point.

    Nave N(x, y, 3, 3);
    N.pintar();
    N.pintarCorazones();

    list<AST*> A;
    list<AST*> :: iterator itA;

    for (int i=0; i<5 ; i++){
        A.push_back(new AST(rand()%115+3, rand()%3+2));
    }

    list<Bala*> B;
    list<Bala*> :: iterator it;

    bool game_over = false;

    int puntos = 0;


    while (!game_over){

        gotoxy(4,0); printf("PUNTOS: %d", puntos);

        if (kbhit()){
            char tecla = getch();
            if(tecla==32) B.push_back(new Bala(N.X()+2, N.Y()-1));
            if(tecla==27) game_over = true;

        }

        for (it = B.begin() ; it != B.end();it++ ){
            (*it)->mover();
            if((*it)->fuera()){
                gotoxy((*it)->X(), (*it)->Y()); printf(" ");
                delete(*it);
                it = B.erase(it);
            }
        }

        for (itA = A.begin(); itA != A.end() ;  itA++ ){
            (*itA)->mover();
            (*itA)->choque(N);
        }

        for (itA = A.begin(); itA != A.end() ;  itA++ ){
            for (it = B.begin() ; it != B.end();it++ ){
                if((*itA)->X() == (*it)->X() && ( (*itA)->Y() +1 == (*it)->Y() || (*itA)->Y() == (*it)->Y())){

                    gotoxy((*it)->X(), (*it)->Y()); printf(" ");
                    delete(*it);
                    it = B.erase(it);

                    A.push_back(new AST(rand()%115+3, 2));
                    gotoxy((*itA)->X(), (*itA)->Y()); printf(" ");
                    delete(*itA);
                    itA = A.erase(itA);

                    puntos++;
                }
            }
        }



        if(N.getVidas() == 0) game_over = true;

        N.morir();
        N.mover();

        Sleep(30);
    }

    return 0;
}
