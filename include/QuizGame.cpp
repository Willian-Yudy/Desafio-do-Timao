#include "../include/QuizGame.h"
#include <iostream>

QuizGame::QuizGame(string nome) : jogador(nome) {}

void QuizGame::adicionarPergunta(unique_ptr<BaseQuestion> q) {
    perguntas.push_back(move(q));
}

void QuizGame::iniciar() {
    jogador.reset();
    cout << "\n==============================\n";
    cout << "    DESAFIO DO TIMÃO 🦅\n";
    cout << "==============================\n";
    cout << "Acertar 5 = Gavião | Errar 3 = Palmeirense 🐖\n";

    for (auto &p : perguntas) {
        p->mostrar(false);
        char resp = static_cast<MultipleChoiceQuestion*>(p.get())->pegarResposta(p->temDica());
        
        if (resp == 'H') {
            p->mostrar(true);
            resp = static_cast<MultipleChoiceQuestion*>(p.get())->pegarResposta(false);
        }

        if (p->verificarResposta(resp)) {
            cout << "✅ Correto!\n";
            jogador.addEstrela();
        } else {
            cout << "❌ Errado!\n";
            jogador.addErro();
        }

        if (jogador.getEstrelas() >= 5 || jogador.getErros() >= 3) break;
    }

    cout << "\n===== RESULTADO FINAL =====\n";
    cout << jogador.tituloFinal() << "\n";
}
