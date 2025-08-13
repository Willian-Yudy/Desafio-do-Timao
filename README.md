/*
 PRG1006 Programming II – Assessment
 Autor: Willian Yudy Futema (15160)
 Projeto: DESAFIO TIMÃO – Quiz Console
 Objetivo didático: demonstrar POO (encapsulamento, herança, polimorfismo),
 tratamento de exceções e testes.

 Compilar:
   g++ -std=c++17 -O2 -Wall -Wextra -pedantic -o desafio_timao main.cpp

 Executar jogo:
   ./desafio_timao

 Executar testes:
   ./desafio_timao --test

 Notas técnicas:
 - Uso de smart pointers (std::unique_ptr) para gestão de memória.
 - Polimorfismo: BaseQuestion (abstrata) + MultipleChoiceQuestion / TrueFalseQuestion.
*/

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <cctype>
#include <utility>   // std::move
using namespace std;

// ===============================
// Base de Perguntas (POO: Herança/Polimorfismo)
// ===============================
class BaseQuestion {
protected:
    string prompt;  // enunciado
    string hint;    // dica opcional
public:
    BaseQuestion(string p, string h = "") : prompt(move(p)), hint(move(h)) {}
    virtual ~BaseQuestion() = default;

    // Interface polimórfica
    virtual void print(bool showHint = false) const = 0;
    virtual bool isCorrect(char choice) const = 0;
    virtual vector<char> validChoices() const = 0;

    const string& getPrompt() const { return prompt; }
    bool hasHint() const { return !hint.empty(); }
    const string& getHint() const { return hint; }
};

// Estrutura de alternativa para múltipla escolha
struct Answer {
    char id;        // A, B, C, D
    string text;    // texto da opção
    bool correct;   // se é correta
};

// -------------------------------
// Pergunta de Múltipla Escolha
// -------------------------------
class MultipleChoiceQuestion : public BaseQuestion {
private:
    vector<Answer> options;
public:
    // TROCA principal: agora recebe vector<Answer> (evita problemas de dedução com initializer_list)
    MultipleChoiceQuestion(string p, vector<Answer> opts, string h = "")
        : BaseQuestion(move(p), move(h)), options(move(opts)) {}

    void print(bool showHint = false) const override {
        cout << "\n" << prompt << "\n";
        for (const auto& a : options) {
            cout << "  [" << a.id << "] " << a.text << "\n";
        }
        if (showHint && !hint.empty()) {
            cout << "(Dica: " << hint << ")\n";
        }
    }

    bool isCorrect(char choice) const override {
        choice = (char)toupper(choice);
        for (const auto& a : options) {
            if (a.id == choice) return a.correct;
        }
        return false;
    }

    vector<char> validChoices() const override {
        vector<char> ids;
        ids.reserve(options.size());
        for (const auto& a : options) ids.push_back(a.id);
        return ids;
    }
};

// -------------------------------
// Pergunta Verdadeiro/Falso
// -------------------------------
class TrueFalseQuestion : public BaseQuestion {
private:
    bool answerTrue; // true = Verdadeiro é correto; false = Falso é correto
public:
    TrueFalseQuestion(string p, bool isTrue, string h = "")
        : BaseQuestion(move(p), move(h)), answerTrue(isTrue) {}

    void print(bool showHint = false) const override {
        cout << "\n" << prompt << "\n";
        cout << "  [T] Verdadeiro\n";
        cout << "  [F] Falso\n";
        if (showHint && !hint.empty()) {
            cout << "(Dica: " << hint << ")\n";
        }
    }

    bool isCorrect(char choice) const override {
        choice = (char)toupper(choice);
        if (choice == 'T') return answerTrue;
        if (choice == 'F') return !answerTrue;
        return false;
    }

    vector<char> validChoices() const override { return {'T','F'}; }
};

// ===============================
// Jogador (POO: Encapsulamento)
// ===============================
class Player {
private:
    string name;
    int stars = 0;
    int mistakes = 0;

public:
    explicit Player(string n = "Player") : name(move(n)) {}

    void addStar() { ++stars; }
    void addMistake() { ++mistakes; }

    int getStars()   const { return stars; }
    int getMistakes() const { return mistakes; }
    const string& getName() const { return name; }

    string title() const {
        if (mistakes >= 3) return "Palmeirense 😬";
        if (stars >= 5)    return "Gavião 🦅";
        return "Fiel em treino";
    }
};

// ===============================
// Motor do Quiz
// ===============================
class QuizGame {
private:
    vector<unique_ptr<BaseQuestion>> questions;
    size_t current = 0;
    Player player;
    vector<pair<string,bool>> answersLog; // (prompt, acertou?)

    static bool isIn(const vector<char>& valid, char c) {
        for (char v : valid) if (v == c) return true;
        return false;
    }

    static char askChoice(const vector<char>& valid, bool allowHint) {
        while (true) {
            cout << "Sua resposta ";
            if (allowHint) cout << "(";
            for (size_t i=0;i<valid.size();++i) {
                cout << valid[i];
                if (i+1<valid.size()) cout << "/";
            }
            if (allowHint) cout << " ou H p/ dica";
            cout << "): ";

            string in; getline(cin, in);
            if (in.empty()) {
                cout << "Entrada vazia. Tente novamente.\n";
                continue;
            }
            char c = (char)toupper(in[0]);
            if (allowHint && c=='H') return 'H';
            if (isIn(valid, c)) return c;
            cout << "Opção inválida. Tente novamente.\n";
        }
    }

public:
    explicit QuizGame(Player p) : player(move(p)) {}

    void addQuestion(unique_ptr<BaseQuestion> q) {
        questions.push_back(move(q));
    }

    void shuffleQuestions(unsigned seed = (unsigned)time(nullptr)) {
        shuffle(questions.begin(), questions.end(), default_random_engine(seed));
    }

    void start() {
        cout << "\n===== DESAFIO TIMÃO – CORINTHIANS QUIZ =====\n";
        cout << "Responda 5 perguntas corretamente para se tornar um Gavião.\n";
        cout << "Com 3 erros, você será considerado palmeirense.\n";
        cout << "Boa sorte, " << player.getName() << "!\n";

        while (current < questions.size()
            && player.getMistakes() < 3
            && player.getStars() < 5) {

            BaseQuestion* q = questions[current].get();
            try {
                // 1) imprime sem dica
                q->print(false);

                // 2) pergunta escolha
                char choice = askChoice(q->validChoices(), /*allowHint*/ q->hasHint());

                // se pediu dica, imprime com dica e pergunta novamente
                if (choice == 'H') {
                    q->print(true);
                    choice = askChoice(q->validChoices(), /*allowHint*/ false);
                }

                bool ok = q->isCorrect(choice);
                answersLog.push_back({ q->getPrompt(), ok });

                if (ok) {
                    player.addStar();
                    cout << "✅ Correto! Estrelas: " << player.getStars() << "\n";
                } else {
                    player.addMistake();
                    cout << "❌ Errado. Erros: " << player.getMistakes() << "\n";
                }

            } catch (const exception& e) {
                cerr << "Erro inesperado: " << e.what() << "\n";
            }
            ++current;
        }

        // Resultado
        cout << "\n===== RESULTADO =====\n";
        cout << "Acertos: " << player.getStars()
             << " | Erros: " << player.getMistakes() << "\n";
        cout << "Título: " << player.title() << "\n";

        // Resumo das perguntas
        cout << "\nResumo das respostas:\n";
        for (const auto& [p, ok] : answersLog) {
            cout << (ok ? "[OK] " : "[X] ") << p << "\n";
        }

        if (player.getStars() >= 5) {
            cout << "Parabéns, " << player.getName() << "! Você provou ser da Fiel!\n";
        } else if (player.getMistakes() >= 3) {
            cout << "Que pena... virou palmeirense hoje. Treine mais!\n";
        } else {
            cout << "Jogo encerrado.\n";
        }
    }
};

// ===============================
// Banco de Questões (MC + TF)
// ===============================
static vector<unique_ptr<BaseQuestion>> buildDefaultQuestions() {
    vector<unique_ptr<BaseQuestion>> qs;

    // Múltipla Escolha
    qs.push_back(make_unique<MultipleChoiceQuestion>(
        "Em que ano o Corinthians foi fundado?",
        vector<Answer>{ {'A',"1908",false}, {'B',"1910",true}, {'C',"1912",false}, {'D',"1914",false} },
        "Início da década de 1910"
    ));
    qs.push_back(make_unique<MultipleChoiceQuestion>(
        "Quem foi o adversário na final do Mundial de 2012?",
        vector<Answer>{ {'A',"Chelsea",true}, {'B',"Barcelona",false}, {'C',"Bayern",false}, {'D',"Real Madrid",false} },
        "Time inglês"
    ));
    qs.push_back(make_unique<MultipleChoiceQuestion>(
        "O estádio atual do Corinthians é a Arena...",
        vector<Answer>{ {'A',"Itaquera",false}, {'B',"Neo Química",true}, {'C',"Pacaembu",false}, {'D',"Barueri",false} },
        "Nome comercial"
    ));

    // Verdadeiro/Falso
    qs.push_back(make_unique<TrueFalseQuestion>(
        "O Corinthians venceu a Libertadores de 2012 de forma invicta.", true, "Campanha histórica"
    ));
    qs.push_back(make_unique<TrueFalseQuestion>(
        "O mascote oficial do Corinthians é o Porco.", false, "Organizada Gaviões"
    ));
    qs.push_back(make_unique<TrueFalseQuestion>(
        "Paolo Guerrero marcou o gol do título no Mundial de 2012.", true, "Atacante peruano"
    ));

    return qs;
}

// ===============================
// Testes de Unidade (básicos)
// ===============================
static int testsPassed = 0;
static int testsRun = 0;

#define EXPECT_TRUE(msg, expr) do { \
    ++testsRun; \
    if (expr) { ++testsPassed; cout << "  [OK] " << msg << "\n"; } \
    else { cout << "  [FAIL] " << msg << "\n"; } \
} while(0)

void runTests() {
    cout << "\n===== EXECUTANDO TESTES =====\n";

    // Player
    {
        Player p("Test");
        p.addStar(); p.addStar();
        p.addMistake();
        EXPECT_TRUE("Player stars == 2", p.getStars() == 2);
        EXPECT_TRUE("Player mistakes == 1", p.getMistakes() == 1);
        EXPECT_TRUE("Player name == Test", p.getName() == "Test");
    }

    // MultipleChoiceQuestion
    {
        MultipleChoiceQuestion q(
            "Ano de fundação?",
            vector<Answer>{ {'A',"1908",false}, {'B',"1910",true} }
        );
        EXPECT_TRUE("MCQ correct on B", q.isCorrect('B') == true);
        EXPECT_TRUE("MCQ incorrect on A", q.isCorrect('A') == false);
        auto v = q.validChoices();
        EXPECT_TRUE("MCQ valid choices size", v.size() == 2);
    }

    // TrueFalseQuestion
    {
        TrueFalseQuestion q("Venceu invicto em 2012?", true);
        EXPECT_TRUE("TF true correct", q.isCorrect('T') == true);
        EXPECT_TRUE("TF false incorrect", q.isCorrect('F') == false);
        auto v = q.validChoices();
        EXPECT_TRUE("TF valid choices are T/F", v.size() == 2 && v[0]=='T' && v[1]=='F');
    }

    cout << "===== TESTES: " << testsPassed << "/" << testsRun << " PASSARAM =====\n\n";
}

// ===============================
// main
// ===============================
int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Modo teste
    if (argc > 1 && string(argv[1]) == "--test") {
        runTests();
        return (testsPassed == testsRun) ? 0 : 1;
    }

    cout << "Digite seu nome: ";
    string name;
    getline(cin, name);
    if (name.empty()) {
        try {
            throw invalid_argument("Nome nao pode ser vazio.");
        } catch (const exception& e) {
            cerr << "Aviso: " << e.what() << " Usando nome padrao 'Player'.\n";
            name = "Player";
        }
    }

    Player p(name);
    QuizGame game(p);

    auto qs = buildDefaultQuestions();
    for (auto& q : qs) game.addQuestion(move(q));

    game.shuffleQuestions();
    game.start();

    return 0;
}
