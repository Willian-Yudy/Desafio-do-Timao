#include "../include/QuizGame.h"
#include "../include/Pergunta.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cctype>

using namespace std;

int main() {
    string nome;
    cout << "Digite seu nome: ";
    getline(cin, nome);
    if (nome.empty()) nome = "Player";

    QuizGame jogo(nome);

    jogo.adicionarPergunta(make_unique<MultipleChoiceQuestion>(
        "1. Which year was Corinthians founded?",
        vector<Opcao>{{'A',"1910",true},{'B',"1920",false},{'C',"1905",false}},
        "Começo da década de 1910"
    ));
    jogo.adicionarPergunta(make_unique<MultipleChoiceQuestion>(
        "2. How many world titles does Palmeiras have?",
        vector<Opcao>{{'A',"2",false},{'B',"1",false},{'C',"0 (51 é pinga!)",true}},
        "51 is Cachaça"
    ));
    jogo.adicionarPergunta(make_unique<MultipleChoiceQuestion>(
        "3. Which stadium is home to one of the biggest clubs in Brazil?",
        vector<Opcao>{{'A',"Morumbi",false},{'B',"Allianz Parque",false},
                       {'C',"Neo Química Arena 🏟️",true}},
        "É a casa do Timão "
    ));
    jogo.adicionarPergunta(make_unique<MultipleChoiceQuestion>(
        "4. Who is the top scorer for Corinthians at moment?",
        vector<Opcao>{{'A',"Memphis Depay",false},{'B',"Yuri Alberto",true},{'C',"Rodrigo Garro",false}},
        "Pai do Inter"
    ));
    jogo.adicionarPergunta(make_unique<MultipleChoiceQuestion>(
        "5. How many times has Corinthians won FIFA World Cup ⚽?",
        vector<Opcao>{{'A',"4",false},{'B',"2",true},{'C',"3",false}},
        "2000 e 2012"
    ));

    char jogarNovamente;
    do {
        jogo.iniciar();
        cout << "\nDeseja jogar novamente? (S/N): ";
        cin >> jogarNovamente;
        jogarNovamente = toupper(jogarNovamente);
    } while (jogarNovamente == 'S');

    cout << "\nObrigado por jogar o Desafio Timão!\n";
    return 0;
}
