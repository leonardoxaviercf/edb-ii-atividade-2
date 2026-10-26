#ifndef TEST_SUITE_HPP
#define TEST_SUITE_HPP

#include "BalancedTree.hpp"
#include "AVLTree.hpp"
#include "RBTree.hpp"
#include <vector>
#include <string>

class TestSuite {
public:
    // Executa todos os testes funcionais e casos de borda exigidos na Seção 3
    static void runFunctionalTests();

    // Executa o benchmark comparativo (Crescente, Decrescente e Aleatório) com o mesmo dataset
    static void runComparativeBenchmark(int numElements = 1000);

    // Demonstração visual lado a lado com dataset reduzido para captura de slides
    static void runVisualComparison();

private:
    static void testTreeEdgeCases(BalancedTree& tree);
    static void evaluateScenario(const std::string& scenarioName,
                                 const std::vector<int>& dataset,
                                 const std::vector<int>& searchKeys,
                                 const std::vector<int>& removeKeys);
};

#endif