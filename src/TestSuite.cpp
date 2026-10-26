#include "TestSuite.hpp"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <random>

void TestSuite::testTreeEdgeCases(BalancedTree& tree) {
    tree.clear();
    std::cout << "\n============================================================\n";
    std::cout << "  TESTANDO CASOS DE BORDA E OPERACOES: " << tree.getTreeName() << "\n";
    std::cout << "============================================================\n";

    // 1. Operações em árvore vazia
    std::cout << "\n[1] Operacoes em arvore vazia:\n";
    std::cout << " -> Arvore esta vazia? " << (tree.isEmpty() ? "SIM" : "NAO") << "\n";
    std::cout << " -> Busca por ID 99 em arvore vazia: "
              << (tree.search(99) ? "Encontrado" : "Nao encontrado (Correto)") << "\n";
    std::cout << " -> Remocao de ID 99 em arvore vazia: "
              << (tree.remove(99) ? "Removido" : "Rejeitado - Inexistente (Correto)") << "\n";
    std::cout << " -> Impressao de arvore vazia: ";
    tree.printTree();

    // 2. Operações em árvore com apenas 1 elemento
    std::cout << "\n[2] Arvore com apenas 1 elemento (ID 50):\n";
    tree.insert(50);
    tree.printTree();
    std::cout << " -> Busca pelo ID 50: " << (tree.search(50) ? "Encontrado (Correto)" : "Falhou") << "\n";
    std::cout << " -> Tentativa de inserir duplicata (ID 50): "
              << (tree.insert(50) ? "Inserido (Erro)" : "Rejeitado - Duplicado (Correto)") << "\n";
    std::cout << " -> Removendo unico elemento (ID 50): "
              << (tree.remove(50) ? "Removido com sucesso" : "Falhou") << "\n";
    std::cout << " -> Arvore voltou a ficar vazia? " << (tree.isEmpty() ? "SIM" : "NAO")
              << " | Propriedades validas? " << (tree.validateProperties() ? "SIM" : "NAO") << "\n";

    // 3. Montando estrutura para testar remoções (0, 1 e 2 filhos) e buscas
    std::cout << "\n[3] Inserindo lote de encomendas [30, 15, 45, 10, 20, 40, 50, 25]:\n";
    for (int id : {30, 15, 45, 10, 20, 40, 50, 25}) {
        tree.insert(id);
    }
    tree.printTree();
    std::cout << " -> Percurso em ordem crescente: ";
    tree.inOrder();
    std::cout << " -> Busca ID existente (25): " << (tree.search(25) ? "Encontrado" : "Nao encontrado") << "\n";
    std::cout << " -> Busca ID inexistente (999): " << (tree.search(999) ? "Encontrado" : "Nao encontrado") << "\n";

    // 4. Remoção de nó folha (0 filhos), nó com 1 filho e nó com 2 filhos
    std::cout << "\n[4.1] Removendo no folha / 0 filhos (ID 10):\n";
    tree.remove(10);
    tree.printTree();
    std::cout << " -> Propriedades preservadas? " << (tree.validateProperties() ? "SIM" : "NAO") << "\n";

    std::cout << "\n[4.2] Removendo no com 1 filho (ID 20, pai apenas do 25):\n";
    tree.remove(20);
    tree.printTree();
    std::cout << " -> Propriedades preservadas? " << (tree.validateProperties() ? "SIM" : "NAO") << "\n";

    std::cout << "\n[4.3] Removendo no com 2 filhos (ID 45, pai de 40 e 50):\n";
    tree.remove(45);
    tree.printTree();
    std::cout << " -> Propriedades preservadas? " << (tree.validateProperties() ? "SIM" : "NAO") << "\n";

    std::cout << "\n[4.4] Tentando remover ID inexistente (ID 888): "
              << (tree.remove(888) ? "Removido" : "Nao encontrado - Operacao segura") << "\n";
    std::cout << " -> Propriedades preservadas ao final? "
              << (tree.validateProperties() ? "SIM" : "NAO") << "\n";
}

void TestSuite::runFunctionalTests() {
    AVLTree avl;
    RBTree rbt;

    testTreeEdgeCases(avl);
    testTreeEdgeCases(rbt);
}

void TestSuite::runVisualComparison() {
    AVLTree avl;
    RBTree rbt;

    std::vector<int> seq = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    for (int id : seq) {
        avl.insert(id);
        rbt.insert(id);
    }

    std::cout << "\n============================================================\n";
    std::cout << "  COMPARACAO VISUAL: INSERCAO CRESCENTE (10 a 100)\n";
    std::cout << "============================================================\n";

    std::cout << "\n--- [Arvore AVL] (Altura: " << avl.getHeight()
              << " | Rotacoes: " << avl.getMetrics().totalRotations() << ") ---\n";
    avl.printTree();

    std::cout << "\n--- [Arvore Rubro-Negra] (Altura: " << rbt.getHeight()
              << " | Rotacoes: " << rbt.getMetrics().totalRotations()
              << " | Ajustes de cor: " << rbt.getMetrics().colorFlips << ") ---\n";
    rbt.printTree();
}

void TestSuite::evaluateScenario(const std::string& scenarioName,
                                 const std::vector<int>& dataset,
                                 const std::vector<int>& searchKeys,
                                 const std::vector<int>& removeKeys) {
    AVLTree avl;
    RBTree rbt;

    // 1. Fase de Inserção
    for (int key : dataset) {
        avl.insert(key);
        rbt.insert(key);
    }

    int avlHeightAfterIns = avl.getHeight();
    int rbtHeightAfterIns = rbt.getHeight();
    long long avlRotIns   = avl.getMetrics().totalRotations();
    long long rbtRotIns   = rbt.getMetrics().totalRotations();
    long long rbtColIns   = rbt.getMetrics().colorFlips;
    bool avlValidIns      = avl.validateProperties();
    bool rbtValidIns      = rbt.validateProperties();

    // 2. Fase de Busca (zera comparações para medir custo exclusivo de busca)
    long long avlCompBeforeSearch = avl.getMetrics().comparisons;
    long long rbtCompBeforeSearch = rbt.getMetrics().comparisons;

    for (int key : searchKeys) {
        avl.search(key);
        rbt.search(key);
    }

    long long avlSearchComps = avl.getMetrics().comparisons - avlCompBeforeSearch;
    long long rbtSearchComps = rbt.getMetrics().comparisons - rbtCompBeforeSearch;

    // 3. Fase de Remoção (30% dos pacotes entregues/removidos)
    for (int key : removeKeys) {
        avl.remove(key);
        rbt.remove(key);
    }

    int avlHeightFinal = avl.getHeight();
    int rbtHeightFinal = rbt.getHeight();
    long long avlRotTotal = avl.getMetrics().totalRotations();
    long long rbtRotTotal = rbt.getMetrics().totalRotations();
    long long rbtColTotal = rbt.getMetrics().colorFlips;
    bool avlValidFinal = avl.validateProperties();
    bool rbtValidFinal = rbt.validateProperties();

    std::cout << "\n>>> CENARIO: " << scenarioName << " (N = " << dataset.size() << " encomendas)\n";
    std::cout << std::left << std::setw(32) << "Metrica"
              << std::setw(22) << "Arvore AVL"
              << std::setw(22) << "Arvore Rubro-Negra" << "\n";
    std::cout << std::string(76, '-') << "\n";

    std::cout << std::left << std::setw(32) << "Altura pos-insercao"
              << std::setw(22) << avlHeightAfterIns
              << std::setw(22) << rbtHeightAfterIns << "\n";

    std::cout << std::left << std::setw(32) << "Rotacoes na insercao"
              << std::setw(22) << avlRotIns
              << std::setw(22) << rbtRotIns << "\n";

    std::cout << std::left << std::setw(32) << "Rotacoes Simples (Esq / Dir)"
              << std::setw(22) << (std::to_string(avl.getMetrics().singleRotationsLeft) + " / " + std::to_string(avl.getMetrics().singleRotationsRight))
              << std::setw(22) << (std::to_string(rbt.getMetrics().singleRotationsLeft) + " / " + std::to_string(rbt.getMetrics().singleRotationsRight)) << "\n";

    std::cout << std::left << std::setw(32) << "Rotacoes Duplas (LR / RL)"
              << std::setw(22) << (std::to_string(avl.getMetrics().doubleRotationsLR) + " / " + std::to_string(avl.getMetrics().doubleRotationsRL))
              << std::setw(22) << "N/A (Usa Simples)" << "\n";

    std::cout << std::left << std::setw(32) << "Comparacoes nas buscas"
              << std::setw(22) << avlSearchComps
              << std::setw(22) << rbtSearchComps << "\n";

    std::cout << std::left << std::setw(32) << "Altura final (pos-remocoes)"
              << std::setw(22) << avlHeightFinal
              << std::setw(22) << rbtHeightFinal << "\n";

    std::cout << std::left << std::setw(32) << "Total de rotacoes (Ins + Rem)"
              << std::setw(22) << avlRotTotal
              << std::setw(22) << rbtRotTotal << "\n";

    std::cout << std::left << std::setw(32) << "Ajustes de cor (Color Flips)"
              << std::setw(22) << "0 (Usa Altura)"
              << std::setw(22) << rbtColTotal << " (Ins: " + std::to_string(rbtColIns) + ")" << "\n";

    std::cout << std::left << std::setw(32) << "Propriedades validas?"
              << std::setw(22) << ((avlValidIns && avlValidFinal) ? "SIM (100%)" : "FALHOU")
              << std::setw(22) << ((rbtValidIns && rbtValidFinal) ? "SIM (100%)" : "FALHOU") << "\n";
}

void TestSuite::runComparativeBenchmark(int numElements) {
    std::cout << "\n============================================================================\n";
    std::cout << "  AMAZON BLACK FRIDAY BENCHMARK: AVL vs RUBRO-NEGRA (N = " << numElements << ")\n";
    std::cout << "============================================================================\n";

    // 1. Conjunto em Ordem Crescente
    std::vector<int> ascData(numElements);
    std::iota(ascData.begin(), ascData.end(), 1);

    // 2. Conjunto em Ordem Decrescente
    std::vector<int> descData = ascData;
    std::reverse(descData.begin(), descData.end());

    // 3. Conjunto Não Ordenado (Aleatório com semente fixa para reprodutibilidade exata)
    std::vector<int> randData = ascData;
    std::mt19937 rng(42);
    std::shuffle(randData.begin(), randData.end(), rng);

    // Chaves de busca (inclui todas as existentes + 20% de chaves inexistentes)
    std::vector<int> searchKeys = randData;
    for (int i = 1; i <= numElements / 5; ++i) {
        searchKeys.push_back(numElements + i * 10);
    }

    // Chaves para remoção (remove 30% dos elementos + tentativas inexistentes)
    std::vector<int> removeKeys(randData.begin(), randData.begin() + (numElements * 3 / 10));
    removeKeys.push_back(-50);
    removeKeys.push_back(numElements + 9999);

    evaluateScenario("1. Insercao em Ordem CRESCENTE", ascData, searchKeys, removeKeys);
    evaluateScenario("2. Insercao em Ordem DECRESCENTE", descData, searchKeys, removeKeys);
    evaluateScenario("3. Insercao NAO ORDENADA (Aleatoria)", randData, searchKeys, removeKeys);
}