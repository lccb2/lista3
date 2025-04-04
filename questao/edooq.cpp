#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int TAM_TABELA = 1009;  // Tamanho da tabela hash (numero primo para menos colisoes)
const int TAM_BLOCO = 6;  // Tamanho de cada bloco de caracteres

// Estrutura para armazenar um bloco na hash table
struct Nodo {
    string bloco;
    int contador;
    Nodo* proximo;  // Ponteiro para tratar colisoes (encadeamento)

    Nodo(string b) : bloco(b), contador(1), proximo(nullptr) {}
};

// Classe da Tabela Hash com tratamento de colisoes por encadeamento
class TabelaHash {
private:
    vector<Nodo*> tabela;
    vector<string> ordemBlocos;  // Mant�m a ordem dos blocos na insercao

    // Funcao hash para distribuir os blocos na tabela
    int calcularHash(const string& bloco) {
        long hash = 0, p = 31, potencia = 1;
        for (char c : bloco) {
            hash = (hash + (c - 'A' + 1) * potencia) % TAM_TABELA;
            potencia = (potencia * p) % TAM_TABELA;
        }
        return hash;
    }

public:
    TabelaHash() : tabela(TAM_TABELA, nullptr) {}

    // Insere um bloco na hash table ou incrementa a contagem se ja existir
    void inserirBloco(const string& bloco) {
        int indice = calcularHash(bloco);
        Nodo* atual = tabela[indice];

        // Verifica se o bloco ja existe na lista encadeada da posicao
        while (atual) {
            if (atual->bloco == bloco) {
                atual->contador++;  // Se j� existe, so aumenta o contador
                return;
            }
            atual = atual->proximo;
        }

        // Se nao existe, cria um novo nodo e adiciona na tabela
        Nodo* novo = new Nodo(bloco);
        novo->proximo = tabela[indice];
        tabela[indice] = novo;

        // Guarda a ordem de insercao para manter a sequencia original
        ordemBlocos.push_back(bloco);
    }

    // Salva o resultado no arquivo
    void salvarResultado(const string& nomeArquivo) {
        ofstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            cerr << "Erro ao criar " << nomeArquivo << endl;
            return;
        }

        arquivo << "Bloco\tOcorrencias\n";
        for (const string& bloco : ordemBlocos) {
            int indice = calcularHash(bloco);
            Nodo* atual = tabela[indice];

            while (atual) {
                if (atual->bloco == bloco) {
                    arquivo << bloco << "\t" << atual->contador << "\n";
                    break;
                }
                atual = atual->proximo;
            }
        }

        arquivo.close();
        cout << "Arquivo '" << nomeArquivo << "' salvo com sucesso!\n";
    }

    // Libera memoria dos nodos alocados
    ~TabelaHash() {
        for (Nodo* head : tabela) {
            while (head) {
                Nodo* temp = head;
                head = head->proximo;
                delete temp;
            }
        }
    }
};

// Funcao principal
int main() {
    ifstream entrada("sequencia.txt");
    if (!entrada.is_open()) {
        cerr << "Erro ao abrir sequencia.txt" << endl;
        return 1;
    }

    string linha, sequencia;
    while (getline(entrada, linha)) {
        sequencia += linha;
    }
    entrada.close();

    TabelaHash tabela;

    // Percorre a string pegando blocos de 6 caracteres (sem sobreposicao)
    for (size_t i = 0; i + TAM_BLOCO <= sequencia.size(); i += TAM_BLOCO) {
        string bloco = sequencia.substr(i, TAM_BLOCO);
        tabela.inserirBloco(bloco);
    }

    tabela.salvarResultado("resultado1.txt");

    return 0;
}
