#ifndef PERGUNTA_H
#define PERGUNTA_H

#include <string>
#include <vector>
#include <memory>

using namespace std;

// Herança, Polimorfismo e Abstração
struct Opcao {
    char letra;
    string texto;
    bool correta;
};

class BaseQuestion {
protected:
    string enunciado;
    string dica;
public:
    BaseQuestion(string e, string d = "");
    virtual ~BaseQuestion() = default;
    virtual void mostrar(bool mostrarDica = false) const = 0;
    virtual bool verificarResposta(char resp) const = 0;
    bool temDica() const;
};

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
