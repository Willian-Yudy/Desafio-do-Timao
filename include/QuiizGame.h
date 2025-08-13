#ifndef QUIZGAME_H
#define QUIZGAME_H

#include "Player.h"
#include "Pergunta.h"
#include <memory>
#include <vector>

class QuizGame {
private:
    Player jogador;
    vector<unique_ptr<BaseQuestion>> perguntas;

public:
    QuizGame(string nome);
    void adicionarPergunta(unique_ptr<BaseQuestion> q);
    void iniciar();
};

#endif // QUIZGAME_H
