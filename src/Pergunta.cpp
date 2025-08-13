#include "../include/Pergunta.h"
#include <iostream>
#include <cctype>

BaseQuestion::BaseQuestion(string e, string d) : enunciado(move(e)), dica(move(d)) {}
bool BaseQuestion::temDica() const { return !dica.empty(); }

MultipleChoiceQuestion::MultipleChoiceQuestion(string e, vector<Opcao> alt, string d)
    : BaseQuestion(e, d), alternativas(move(alt)) {}

void MultipleChoiceQuestion::mostrar(bool mostrarDica) const {
    cout << "\n" << enunciado << "\n";
    for (const auto& a : alternativas) {
        cout << "[" << a.letra << "] " << a.texto << "\n";
    }
    if (mostrarDica && temDica()) {
        cout << "(💡 Dica: " << dica << ")\n";
    }
}

bool MultipleChoiceQuestion::verificarResposta(char resp) const {
    resp = toupper(resp);
    for (const auto& a : alternativas) {
        if (a.letra == resp) return a.correta;
    }
    return false;
}

char MultipleChoiceQuestion::pegarResposta(bool permitirDica) const {
    string entrada;
    while (true) {
        cout << "Sua resposta";
        if (permitirDica && !dica.empty()) cout << " (ou H para dica)";
        cout << ": ";
        cin >> entrada;
        if (entrada.empty()) continue;
        char r = toupper(entrada[0]);
        if (permitirDica && r == 'H' && !dica.empty()) return 'H';
        
        bool valida = false;
        for (const auto& a : alternativas) {
            if (a.letra == r) {
                valida = true;
                break;
            }
        }
        if(valida) return r;
        cout << "Opção inválida! Tente novamente.\n";
    }
}
