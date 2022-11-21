#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <string.h>

using namespace std;

ifstream f("../cuvinte_wordle.txt");

bool in_menu=true,in_game=false,in_solver=false,verif=false,ok=false,ok_guess=true,ok_num=false; //pentru a vedea in ce stadiu este jocul
char text[500],state[6];
vector <string> dictionar;
string line;

//pentru textul din meniu si instructiuni va fi folosita functia usleep, pentru a nu afisa un wall of text instant.
//de adaugat lista de cuvinte prin citire de fisier

void init_dictionar()
{
    int total_lines = 0;
    while(getline(f, line)){
        dictionar.push_back(line);
        total_lines++;
    }

}

void word_getter(char cuv[])
{int n;
    for (int i=1;i<=10;i++)
    {
        n=rand()%11453; //un for micut inainte ca sa genereze mai multe numere random si sa nu fie neaparat in ordine increasing
    }
    for (int i=0;i<5;i++)
        cuv[i]=dictionar[n][i];
    cout<<cuv; ///pentru a verifica faptul ca a ales un cuvant
}


void menu_text() //introducerea jocului
{
    strcpy(text,"Salut, hai sa jucam jocul Wordle!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"Pentru a incepe jocul, scrie cuvantul 'Joaca'!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"Daca vrei ca jocul sa se rezolve singur, scrie cuvantul 'Rezolva'!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"In cazul in care nu stii cum se joaca acest joc, scrie 'Ajutor' si vor aparea instructiuni pe ecran!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';

}

void instructions() //cum se joca jocul.
{
    strcpy(text,"In acest joc, scopul este simplu: Incearca sa ghicesti un cuvant de 5 litere! ");
    for (int i=0;i<strlen(text);i++)
    cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"Insa, incercarile trebuie sa fie numai cuvinte de 5 litere existente in dictionarul limbii romane!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"De fiecare data cand introduci un cuvant din Dictionar, vei vedea ca sub el vor aparea aceste literele R,Y,G");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"1) Daca o litera din cuvantul introdus de tine se afla in cuvantul care trebuie ghicit, aceasta ");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"2) Daca litera, in plus, se afla pe aceeasi pozitie ca in cuvantul care trebuie ghicit, aceasta va fi evidentiata cu verde");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"3) Daca litera nu apartine deloc acelui cuvant, atunci culoarea ei ramane neschimbata");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"Nota: jocul nu este case sensitive, asa ca literele mici vor fi tratate ca si cum ar fi litere mari");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';
    strcpy(text,"Exemplu de inputuri corecte: Sapte, paine, CRUCE, fOaMe");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
    cout<<'\n';

}

void uppercase(char s[]) //inputurile vor fi transformate in cuvinte cu litere mari, ca sa nu fie case-sensitive jocul
{
    for (int i=0;i<strlen(s);i++)
    {
        if (s[i]>='a' && s[i]<='z')
            s[i]-=32;
    }
}

void menu_checker(char s[]) //verificarile inputurilor din meniu, daca vrem sa jucam, sa rezolvam sau instructiuni
{
    if (strcmp(s,"JOACA")==0)
        in_menu=false,in_game=true;
    else if (strcmp(s,"REZOLVA")==0)
        in_menu=false,in_solver=true;
    else if (strcmp(s,"AJUTOR")==0)
        instructions();
    else //in cazul in care alta comanda este introdusa
        cout<<"Aceasta nu este o comanda valida!"<<'\n';

}

int list_verifier(string word){
    for(int i = 0; i < 11453; ++i){
        if(word == dictionar[i])
            return 1;
    }
    return 0;
}


void word_verifier(string correct_word, string guess_word, bool &verif){
    int cnt = 0;
    for(int i = 0; i < 5; ++i)
        if(correct_word[i] == guess_word[i])         // verific daca am litere care deja se potrivesc
            state[i] = 'G',cnt++;
        else
            state[i] = '_';


    for(int i = 0; i < 5; ++i){
        bool ok = false;
        if(state[i] == '_'){                   //daca litera nu se potriveste pe pozitie identica, o caut pe restul pozitiilor
            for(int j = 0; j < 5; ++j){
                if(correct_word[j] == guess_word[i]){   // daca la un moment dat gasesc o litera din cuvantul introdus in cel corect, dar nu pe aceeasi pozitie, o colorez in galben
                    state[i] = 'Y';
                    ok = true;
                    break;
                }
            }
            if(ok == false) state[i] = 'R';             // marchez cu rosu daca nu gasesc o litera din cuvantul introdus in celalalt
        }
    }
    for (int i=0;i<5;i++)
        cout<<state[i]<<' ';                    //
    cout<<'\n';
     if(cnt == 5){                              // verific daca toate literele se potrivesc
        verif = true;
    }
}


int main()
{char s[60],guess[25],cuv[25]; //acest s e folosit PENTRU MENU INPUTS.
int contor=0;
    srand(time(NULL));
    init_dictionar();
    menu_text();
    while(in_menu)
    {
        cout<<'\n';
        strcpy(text,"Ce vrei sa faci? Scrie aici: ");
        for (int i=0;i<strlen(text);i++)
            cout<<text[i],usleep(30000);
        cin.getline(s,25);
        uppercase(s);
        menu_checker(s);

    }

    while(in_game) //aici va incepe jocul jucat de o persoana
    {
        if (ok==false)
            word_getter(cuv),cout<<'\n',ok=true;
        strcpy(text,"Ai incercat sa ghicesti cuvantul de ");
        for (int i=0;i<strlen(text);i++)
            cout<<text[i],usleep(30000);
        cout<<contor;
        strcpy(text," ori");
        for (int i=0;i<strlen(text);i++)
            cout<<text[i],usleep(30000);
        cout<<'\n';
        strcpy(text,"Scrie un cuvant: ");
        for (int i=0;i<strlen(text);i++)
            cout<<text[i],usleep(30000);
        cin>>guess;
        uppercase(guess);
        ///aici incepe codul pentru verificarea validitatii cuvantului
        if (strlen(guess)!=5)
        {
            strcpy(text,"Cuvantul trebuie sa fie format din 5 litere!");
            for (int i=0;i<strlen(text);i++)
                cout<<text[i],usleep(30000);
            cout<<'\n'<<'\n';
            continue;
        }

        for (int i=0;i<5;i++)
                if (!(guess[i]>='A' && guess[i]<='Z'))
                {
                    strcpy(text,"Cuvantul trebuie sa contina doar litere!");
                    for (int i=0;i<strlen(text);i++)
                        cout<<text[i],usleep(30000);
                    cout<<'\n'<<'\n';
                    ok_num=true;
                    break;
                }
        if (ok_num==true)
            {
                ok_num=false;
                continue;
            }
        if (list_verifier(guess)==0)
        {
            strcpy(text,"Cuvantul trebuie sa apartina Dictionarului Limbii Romane!");
            for (int i=0;i<strlen(text);i++)
                cout<<text[i],usleep(30000);
            cout<<'\n'<<'\n';
            continue;
        }


        ///sfarsit, mai jos incepe verificarea literelor
            contor++;
            word_verifier(cuv,guess,verif); //verifica apartenenta literelor prin functia word_verifier. bineinteles si guess-ul va fi case insensitive
            cout<<'\n';
            if (verif==true) //daca a gasit cuvantul potrivit
            {
                ok_guess=true;
                strcpy(text,"Felicitari, ai gasi cuvantul!");
                for (int i=0;i<strlen(text);i++)
                    cout<<text[i],usleep(30000);
                cout<<'\n';
                strcpy(text,"Vrei sa mai joci o data? DA/NU");
                for (int i=0;i<strlen(text);i++)
                    cout<<text[i],usleep(30000);
                cout<<'\n';
                while (ok_guess) //aici va verifica daca userul mai vrea sa joace din nou sau nu
                {
                    cin>>guess;
                    uppercase(guess);
                    if (strcmp(guess,"NU")==0) //daca jucatorul scrie NU
                        in_game=false,ok_guess=false;
                    else if (strcmp(guess,"DA")==0) //daca jucatorul scrie DA
                        in_game=true,ok_guess=false,ok=false,verif=false,contor=0; //resetarea valorilor pentru ca jocul sa o ia de la capat
                    else //in caz ca scrie altceva
                        cout<<"Te rog, scrie DA/NU!"<<'\n';
                }
            }

    }

    while(in_solver) //aici sa se desfasoare activitatea solverului(de modificat in urma sfatuirilor! :) )"
    {
        cout<<"De implementat"<<'\n';
        in_solver=false;
    }
    strcpy(text,"Joc terminat!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],usleep(30000);
}
