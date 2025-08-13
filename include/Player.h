#ifndef PLAYER_H
#define PLAYER_H

#include <string>

using namespace std;

// Encapsulamento
class Player {
private:
    string nome;
    int estrelas = 0;
    int erros = 0;
public:
    Player(string n = "Player");
    void addEstrela();
    void addErro();
    void reset();
    int getEstrelas() const;
    int getErros() const;
    string getNome() const;
    string tituloFinal() const;
};

#endif // PLAYER_H
