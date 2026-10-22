#include <iostream>
#include "AVLTree.hpp"

int main() {
    AVLTree avl;

    std::cout << "=== Teste Inicial: Amazon Engineering Challenge (AVL) ===\n";
    
    // Teste de inserção em ordem crescente (forçando rotações à esquerda)
    int pacotes[] = {10, 20, 30, 40, 50, 25};
    for (int id : pacotes) {
        std::cout << "\n-> Inserindo pacote ID " << id << ":\n";
        avl.insert(id);
        avl.printTree();
    }

    // Teste de duplicata
    std::cout << "\n-> Tentando inserir ID duplicado (30): "
              << (avl.insert(30) ? "Inserido" : "Rejeitado (Duplicado)") << "\n";

    // Percurso em ordem
    std::cout << "-> Percurso em ordem crescente: ";
    avl.inOrder();

    // Validação das propriedades de balanceamento
    std::cout << "-> Propriedades AVL preservadas? "
              << (avl.validateProperties() ? "SIM" : "NAO") << "\n";
    std::cout << "-> Altura final: " << avl.getHeight()
              << " | Total de rotacoes: " << avl.getMetrics().totalRotations() << "\n";

    return 0;
}