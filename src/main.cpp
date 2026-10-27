#include <iostream>
#include <memory>
#include <sstream>
#include <limits>
#include "AVLTree.hpp"
#include "RBTree.hpp"
#include "TestSuite.hpp"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void interactiveTreeMenu(BalancedTree& tree) {
    int option = -1;
    while (option != 0) {
        std::cout << "\n------------------------------------------------------------\n";
        std::cout << "  MODO INTERATIVO: " << tree.getTreeName() << " [Logistica Amazon]\n";
        std::cout << "------------------------------------------------------------\n";
        std::cout << "  1. Inserir pacote (ID unico)\n";
        std::cout << "  2. Inserir multiplos pacotes (separados por espaco)\n";
        std::cout << "  3. Remover pacote (ID)\n";
        std::cout << "  4. Buscar pacote (ID)\n";
        std::cout << "  5. Percurso em ordem crescente\n";
        std::cout << "  6. Visualizar estrutura da arvore\n";
        std::cout << "  7. Exibir telemetria e validar balanceamento\n";
        std::cout << "  8. Limpar arvore\n";
        std::cout << "  0. Voltar ao menu principal\n";
        std::cout << "Escolha uma operacao: ";

        if (!(std::cin >> option)) {
            clearInput();
            std::cout << "Entrada invalida.\n";
            continue;
        }

        if (option == 1) {
            int key;
            std::cout << "Digite o ID do pacote para inserir: ";
            if (std::cin >> key) {
                if (tree.insert(key)) {
                    std::cout << "[OK] Pacote " << key << " registrado com sucesso.\n";
                } else {
                    std::cout << "[AVISO] Pacote " << key << " ja existe! Insercao ignorada.\n";
                }
                tree.printTree();
            } else {
                clearInput();
            }
        } else if (option == 2) {
            std::cout << "Digite os IDs separados por espaco (ex: 10 20 30 40 50 25): ";
            clearInput();
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int key;
            while (iss >> key) {
                bool ok = tree.insert(key);
                std::cout << " -> Inserindo " << key << ": "
                          << (ok ? "OK" : "Rejeitado (Duplicado)") << "\n";
            }
            tree.printTree();
        } else if (option == 3) {
            int key;
            std::cout << "Digite o ID do pacote para remover: ";
            if (std::cin >> key) {
                if (tree.remove(key)) {
                    std::cout << "[OK] Pacote " << key << " removido da arvore.\n";
                } else {
                    std::cout << "[AVISO] Pacote " << key << " nao encontrado na estrutura.\n";
                }
                tree.printTree();
            } else {
                clearInput();
            }
        } else if (option == 4) {
            int key;
            std::cout << "Digite o ID do pacote para buscar: ";
            if (std::cin >> key) {
                bool found = tree.search(key);
                std::cout << (found ? "[ENCONTRADO] O pacote consta no sistema.\n"
                                    : "[NAO ENCONTRADO] O pacote nao existe na arvore.\n");
            } else {
                clearInput();
            }
        } else if (option == 5) {
            std::cout << "Elementos em ordem crescente: ";
            tree.inOrder();
        } else if (option == 6) {
            std::cout << "\nEstrutura atual (" << tree.getTreeName() << "):\n";
            tree.printTree();
        } else if (option == 7) {
            const auto& m = tree.getMetrics();
            std::cout << "\n--- Telemetria e Diagnostico (" << tree.getTreeName() << ") ---\n";
            std::cout << "Altura atual: " << tree.getHeight() << "\n";
            std::cout << "Total de rotacoes: " << m.totalRotations()
                      << " (Simples Esq: " << m.singleRotationsLeft
                      << ", Simples Dir: " << m.singleRotationsRight
                      << ", Dupla LR: " << m.doubleRotationsLR
                      << ", Dupla RL: " << m.doubleRotationsRL << ")\n";
            std::cout << "Ajustes de cor (Color Flips): " << m.colorFlips << "\n";
            std::cout << "Comparacoes acumuladas: " << m.comparisons << "\n";
            std::cout << "Propriedades de balanceamento preservadas? "
                      << (tree.validateProperties() ? "SIM (Valido)" : "NAO (Invalido)") << "\n";
        } else if (option == 8) {
            tree.clear();
            std::cout << "Arvore reiniciada.\n";
        }
    }
}

int main() {
    AVLTree avl;
    RBTree rbt;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n============================================================\n";
        std::cout << "  AMAZON ENGINEERING CHALLENGE - ARVORES BALANCEADAS\n";
        std::cout << "============================================================\n";
        std::cout << "  1. Utilizar Arvore AVL (Modo Interativo)\n";
        std::cout << "  2. Utilizar Arvore Rubro-Negra (Modo Interativo)\n";
        std::cout << "  3. Executar Bateria de Testes de Borda e Validacao (Secao 3)\n";
        std::cout << "  4. Comparacao Visual Lado a Lado (N = 10 Crescente)\n";
        std::cout << "  5. Executar Benchmark Comparativo Black Friday (N = 1000)\n";
        std::cout << "  0. Encerrar programa\n";
        std::cout << "Selecione uma opcao: ";

        if (!(std::cin >> choice)) {
            clearInput();
            std::cout << "Opcao invalida.\n";
            continue;
        }

        switch (choice) {
            case 1:
                interactiveTreeMenu(avl);
                break;
            case 2:
                interactiveTreeMenu(rbt);
                break;
            case 3:
                TestSuite::runFunctionalTests();
                break;
            case 4:
                TestSuite::runVisualComparison();
                break;
            case 5:
                TestSuite::runComparativeBenchmark(1000);
                break;
            case 0:
                std::cout << "Encerrando sistema...\n";
                break;
            default:
                std::cout << "Opcao invalida.\n";
                break;
        }
    }

    return 0;
}