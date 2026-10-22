#ifndef BALANCED_TREE_HPP
#define BALANCED_TREE_HPP

#include <iostream>
#include <string>
#include <vector>

// Códigos ANSI para visualização no terminal (identificação de cores na Rubro-Negra)
namespace ANSI {
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[1;31m";
    const std::string BLACK   = "\033[1;30m";
    const std::string GREEN   = "\033[1;32m";
    const std::string CYAN    = "\033[1;36m";
    const std::string YELLOW  = "\033[1;33m";
}

// Estrutura para contabilizar métricas exigidas na análise comparativa
struct TreeMetrics {
    long long singleRotationsLeft  = 0; // Rotação simples à esquerda
    long long singleRotationsRight = 0; // Rotação simples à direita
    long long doubleRotationsLR    = 0; // Rotação dupla Esquerda-Direita (AVL)
    long long doubleRotationsRL    = 0; // Rotação dupla Direita-Esquerda (AVL)
    long long colorFlips           = 0; // Mudanças de cor (exclusivo Rubro-Negra)
    long long comparisons          = 0; // Comparações de chaves realizadas

    long long totalRotations() const {
        return singleRotationsLeft + singleRotationsRight + doubleRotationsLR + doubleRotationsRL;
    }

    void reset() {
        singleRotationsLeft = singleRotationsRight = 0;
        doubleRotationsLR = doubleRotationsRL = 0;
        colorFlips = comparisons = 0;
    }
};

// Interface comum para Árvore AVL e Árvore Rubro-Negra
class BalancedTree {
protected:
    TreeMetrics metrics;

public:
    virtual ~BalancedTree() = default;

    // Operações mínimas obrigatórias
    virtual bool insert(int key) = 0;
    virtual bool remove(int key) = 0;
    virtual bool search(int key) = 0;
    virtual void inOrder() const = 0;
    virtual void printTree() const = 0;

    // Métodos para testes e análise comparativa
    virtual int getHeight() const = 0;
    virtual bool isEmpty() const = 0;
    virtual bool validateProperties() const = 0;
    virtual void clear() = 0;
    virtual std::string getTreeName() const = 0;

    const TreeMetrics& getMetrics() const { return metrics; }
    void resetMetrics() { metrics.reset(); }
};

#endif