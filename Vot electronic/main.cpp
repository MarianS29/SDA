//
// Created by maria on 5/17/2021.
//

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <iomanip>
#include <unordered_set>

using namespace std;

struct persoana_din_baza{
    string nume;
    string adresa;
    string serie_nr_ci;
};

int este_minora(string cnp){
    float age=0;
    age+=(18-((cnp[1]-'0')*10+cnp[2]-'0')-1)*12+5+12-(cnp[3]-'0')*10-(cnp[4]-'0');
    if((float)age/12<18)
        return 1;
    return 0;
}

struct votat{
    string nume_votant;
    int numar_voturi=0;
};

bool sort1(votat a, votat b){
    return a.numar_voturi>b.numar_voturi;
}

FILE* f=fopen("evidenta.csv","r");
FILE* f1=fopen("filein","r");

int main(){
    unordered_set<string> baza_de_date;
    string CNP;
    char cnp[100]="";
    while(fscanf(f,"%*[^,],%*[^,],%[^,],%*[^\n]\n",cnp)!=EOF)
    {
        CNP=cnp;
        baza_de_date.insert(CNP);
    }
    char command;
    vector <string> babayaga;
    int incercari_de_frauda=0;
    map<string, int> votanti;
    set<string> cnpuri;
    int sum=0;
    //
    while(scanf("%c", &command)!=EOF){
        if(command=='+')
        {
            char cnp_votant[100],nume_votat[100];
            scanf("%s %[^\n]",cnp_votant,nume_votat);
            getchar();
            if(baza_de_date.count(cnp_votant)==0)
            {
                cout << "CNP invalid" << endl;
                incercari_de_frauda++;
                continue;
            }
            else {
                if(este_minora(cnp_votant) && cnp_votant[0]-'0'>=5){
                    cout<<"Persoana minora"<<endl;
                    continue;
                }
            }
            //           if(votanti[nume_votat].count(cnp_votant)==1)
            if(cnpuri.count(cnp_votant)==1)
            {
                cout<<"Vot deja inregistrat"<<endl;
                incercari_de_frauda++;
                continue;
            }
            else {
                votanti[nume_votat]++;
                cnpuri.insert(cnp_votant);
                sum++;
                int ok=1;
                for(int i=0; i<babayaga.size(); i++) {
                    if (nume_votat == babayaga[i]) {
                        ok = 0;
                    }
                }
                if(ok==1) {
                    babayaga.push_back(nume_votat);
                }
            }
        }
        if(command=='?') {
            getchar();
            {
                cout<<"Statistici"<<endl<<"=========="<<endl;
                for(int i=sum; i>0; i--){
                    for(int k=0; k<babayaga.size(); k++){
                        for(auto j=votanti.begin(); j!=votanti.end(); j++){
                            if(j->first==babayaga[k] && j->second==i)
                                cout<<j->first<<": "<<j->second<<" voturi "<<"("<<fixed<<setprecision(2)<<(float)j->second/sum*100<<"%)"<<endl;
                        }
                    }
                }
            }
            cout << "Incercari de frauda: " << incercari_de_frauda << endl;
        }
        if(command=='*'){
            {
                cout<<"Statistici"<<endl<<"=========="<<endl;
                for(int i=sum; i>0; i--){
                    for(int k=0; k<babayaga.size(); k++){
                        for(auto j=votanti.begin(); j!=votanti.end(); j++){
                            if(j->first==babayaga[k] && j->second==i)
                                cout<<j->first<<": "<<j->second<<" voturi "<<"("<<fixed<<setprecision(2)<<(float)j->second/sum*100<<"%)"<<endl;
                        }
                    }
                }
            }
            cout << "Incercari de frauda: " << incercari_de_frauda << endl;
            return 0;
        }
    }
    return 0;
}

//Aplicația citește fișierul cu date personale ale persoanelor ce vor vota dintr-un fișier CSV (Comma Separated Values) numit „evidenta.csv” scris în următorul format (fiecare persoană pe o singură linie):
//
//NUME PRENUME_1 PRENUME_2 PRENUME_N,ADRESA,CNP,SERIE_NR_CI
//Numărul de persoane nu este cunoscut la deschiderea fișierului; fișierul va fi citit până la sfârșitul său.
//
//De la tastatură se citesc linii de forma:
//
//+ CNP_votant Nume Complet Candidat
//sau
//?
//sau
//        *
//        În funcție de primul caracter se vor realiza următoarele operații:
//
//'+' - cineva încearcă să voteze. Se realizează următoarele verificări:
//
//Dacă CNP-ul nu există în baza de date, se afișează un mesaj de eroare: „CNP invalid” și se consideră încercare de fraudă
//
//Dacă CNP-ul există, dar persoana a votat deja, se afișează un alt mesaj de eroare „Vot deja inregistrat” și se consideră încercare de fraudă
//
//Dacă CNP-ul există dar vârsta persoanei e mai mică decât 18 (se calculează diferența dintre anul și luna din CNP și anul și luna curentă) se afișează un mesaj de eroare „Persoana minora” și NU se consideră încercare de fraudă
//
//        Altfel, se înregistrează votul pentru candidatul specificat prin nume și se marchează CNP-ul votantului ca vot exprimat.
//
//'?' - se cere statistica curentă: Se vor afișa candidații în ordinea descrescătoare a voturilor obținute (dacă există mai mulți candidați cu același număr de voturi, ordinea în care se afișează este ordinea în care au apărut în listă pentru prima oară) și numărul încercărilor de fraudă
//
//'*' - procesul electroral se încheie, se afișează statistica similar cu output-ul pentru '?' și programul se încheie.
//
//Afișarea statisticilor la introducerea unui caracter '?' sau '*' se face în formatul următor:
//
//Statistici
//==========
//Nume Candidat Unu: 1234 voturi (77.61%)
//Nume Candidat Doi: 343 voturi (21.57%)
//Nume Candidat N: 13 voturi (0.82%)
//Incercari de frauda: 2
//Procentajul voturilor se va afișa cu două zecimale. Textul care nu reprezintă valori sau nume de candidați din blocul de mai sus se va afișa exact ca mai sus.
//
//Exemplu de execuție
//        evidenta.csv:
//
//Ion Ghita Vasile,Str. Oii 3 Bucuresti,1901111123234,ZZ123456
//        Maria Vasile,Str. Oii 3 Bucuresti,2851111123234,YY123456
//        Mihaita Mihai,Str. Vacii 7 Dalga Ploii,5001111123234,WW123446
//        Lucian Lucius,Str. Matzei 5 Bucuresti,1801111123234,TT623456
//        Input-Output:
//
//?
//Statistici
//==========
//Incercari de frauda: 0
//+ 9851111123234 Ion Nastase
//CNP invalid
//+ 1901111123234 Traian Claus
//+ 2851111123234 Vadim Victor
//+ 1901111123234 Traian Claus
//Vot deja inregistrat
//?
//Statistici
//==========
//Traian Claus: 1 voturi (50.00%)
//Vadim Victor: 1 voturi (50.00%)
//Incercari de frauda: 2
//+ 5001111123234 Vadim Victor
//Persoana minora
//+ 1801111123234 Vadim Victor
//*
//Statistici
//==========
//Vadim Victor: 2 voturi (66.67%)
//Traian Claus: 1 voturi (33.33%)
//Incercari de frauda: 2
