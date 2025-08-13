#include "../include/Player.h"
#include <string>

Player::Player(string n) : nome(move(n)) {}

void Player::addEstrela() { estrelas++; }
void Player::addErro() { erros++; }
void Player::reset() { estrelas = 0; erros = 0; }
int Player::getEstrelas() const { return estrelas; }
int Player::getErros() const { return erros; }
string Player::getNome() const { return nome; }

string Player::tituloFinal() const {
    if (estrelas >= 5) return "🏆 Você é um verdadeiro Gavião Fiel! 🦅";
    if (erros >= 3) return "🐖 Virou Palmeirense sem Mundial...";
    return "Terminou o jogo com " + to_string(estrelas) + " estrelas.";
}
