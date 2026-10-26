#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include "BalancedTree.hpp"

class RBTree : public BalancedTree {
public:
    enum class Color { RED, BLACK };

private:
    struct Node {
        int key;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(int k, Color c, Node* l = nullptr, Node* r = nullptr, Node* p = nullptr)
            : key(k), color(c), left(l), right(r), parent(p) {}
    };

    Node* root;
    Node* NIL; // Nó sentinela único (sempre BLACK)

    // Rotações e ajustes de balanceamento
    void rotateLeft(Node* x);
    void rotateRight(Node* y);
    void insertFixup(Node* z);
    void deleteFixup(Node* x);
    void rbTransplant(Node* u, Node* v);

    // Auxiliares internos
    Node* minimum(Node* node) const;
    Node* searchNode(int key);
    void setColor(Node* node, Color newColor);
    int heightRec(Node* node) const;
    void inOrderRec(Node* node) const;
    void printTreeRec(Node* node, std::string indent, bool last) const;
    bool validateRec(Node* node, int minVal, int maxVal, int& blackHeight) const;
    void clearRec(Node* node);

public:
    RBTree();
    ~RBTree() override;

    bool insert(int key) override;
    bool remove(int key) override;
    bool search(int key) override;
    void inOrder() const override;
    void printTree() const override;

    int getHeight() const override;
    bool isEmpty() const override;
    bool validateProperties() const override;
    void clear() override;
    std::string getTreeName() const override { return "Arvore Rubro-Negra"; }
};

#endif