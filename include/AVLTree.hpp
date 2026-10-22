#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

class AVLTree : public BalancedTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;

        explicit Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    // Funções auxiliares internas
    int height(Node* node) const;
    int getBalanceFactor(Node* node) const;
    void updateHeight(Node* node);

    // Rotações puras
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);

    // Operações recursivas
    Node* insertRec(Node* node, int key, bool& inserted);
    Node* removeRec(Node* node, int key, bool& removed);
    Node* getMinValueNode(Node* node) const;
    bool searchRec(Node* node, int key);
    void inOrderRec(Node* node) const;
    void printTreeRec(Node* node, std::string indent, bool last) const;
    bool validateRec(Node* node, int minVal, int maxVal) const;
    void clearRec(Node* node);

public:
    AVLTree();
    ~AVLTree() override;

    bool insert(int key) override;
    bool remove(int key) override;
    bool search(int key) override;
    void inOrder() const override;
    void printTree() const override;

    int getHeight() const override;
    bool isEmpty() const override;
    bool validateProperties() const override;
    void clear() override;
    std::string getTreeName() const override { return "Arvore AVL"; }
};

#endif