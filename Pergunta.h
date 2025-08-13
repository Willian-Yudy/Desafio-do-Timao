#ifndef PERGUNTA_H
#define PERGUNTA_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cctype>

using namespace std;

// Múltipla escolha e struct para agrupar os dados de cada alternativa 
struct Opcao {
    char letra;
    string texto;
    bool correta;
};

// Herança, Abstração
class BaseQuestion {
protected:
    string enunciado;
    string dica;
public:
    BaseQuestion(string e, string d = "") : enunciado(e), dica(d) {}
    virtual ~BaseQuestion() = default;
    virtual void mostrar(bool mostrarDica = false) const = 0;
    virtual bool verificarResposta(char resp) const = 0;
    
    bool temDica() const { return !dica.empty(); }
};

// Herança
class MultipleChoiceQuestion : public BaseQuestion {
private:
    vector<Opcao> alternativas; 
public:
    MultipleChoiceQuestion(string e, vector<Opcao> alt, string d = "");
    void mostrar(bool mostrarDica = false) const override;
    bool verificarResposta(char resp) const override;
    char pegarResposta(bool permitirDica) const;
};

#endif // PERGUNTA_H
